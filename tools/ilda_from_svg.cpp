#include <unistd.h>
#include <cstdio>
#include <cmath>
#include <algorithm>  // std::clamp

#define NANOSVG_IMPLEMENTATION  // Expands implementation
#include "nanosvg.h"
#include "gsl/gsl_util"

#include "lzr/lzr.hpp"

using namespace lzr;

float sample_cubic_bezier(float p0, float p1, float p2, float p3, float t) {
    using std::pow;
    const float t_1 = 1.0 - t;
    return (powf(t_1, 3) * p0) +
           (3 * powf(t_1, 2) * t * p1) +
           (3 * t_1 * powf(t, 2) * p2) +
           (powf(t, 3) * p3);
}

void append_nanosvg_path(const NSVGpath *path, Frame& output_frame, size_t samples, unsigned int nanosvg_color) {
    // The path contains a set of cubic beziers, sharing common start/endpoints, so we consume points by 3's
    for (int i = 0; i < path->npts-1; i += 3)
    {
        float* p = &path->pts[i*2];
        for (size_t s = 0; s <= samples; s++)
        {
            // TODO don't repeat so many calculations here
            const float t = static_cast<float>(s) / samples;
            Point lzr_point(sample_cubic_bezier(p[0], p[2], p[4], p[6], t),
                            sample_cubic_bezier(p[1], p[3], p[5], p[7], t),
                            static_cast<uint8_t>(nanosvg_color & 0x0000FF),
                            static_cast<uint8_t>((nanosvg_color & 0x00FF00) >> 8),
                            static_cast<uint8_t>((nanosvg_color & 0xFF0000) >> 16),
                            255);
            if (i == 0 && s == 0)
            {
                output_frame.add_with_blank_jump(lzr_point);
            }
            else
            {
                output_frame.add(lzr_point);
            }
        }
    }
}

bool load_svg(const char* filename, Frame& frame, bool scale_to_bbox)
{
    // Load SVG
    NSVGimage* image;
    image = nsvgParseFromFile(filename, "px", 96);
    if (image == nullptr)
    {
        std::perror("Failed to open SVG file");
        return false;
    }

    // Free the image at the end of the scope
    auto _ = gsl::finally([&]{ nsvgDelete(image); });

    for (NSVGshape *shape = image->shapes; shape != NULL; shape = shape->next)
    {
        // Ignore shapes without a solid stroke for now.
        // TODO: support gradients
        if (shape->stroke.type == NSVG_PAINT_COLOR)
        {
            for (NSVGpath *path = shape->paths; path != NULL; path = path->next)
            {
                append_nanosvg_path(path, frame, 10, shape->stroke.color);
            }
        }
    }

    if (frame.empty())
    {
        fprintf(stderr, "Empty frame\n");
        return false;
    }

    // Add a return-to-home blanking jump
    frame.add_with_blank_jump(frame.front());

    // re-scale the frame to fit in our [-1,1] range. Note that we also
    // compensate for the fact that computer graphics has the origin in
    // the top left here.
    if (scale_to_bbox)
    {
        Point min;
        Point max;
        frame.bounding_box(min, max);
        const float factor = 2.0f / std::max(max.x - min.x, max.y - min.y) * 0.999f;
        scale(frame, {0, 0}, factor, -factor);
        const Point bbox_center = frame.bounding_box_center();
        translate(frame, -bbox_center.x, -bbox_center.y);
    }
    else
    {
        // scale the maximum document dimension to be [-1,1]
        const float factor = 2.0f / std::max(image->width, image->height);
        scale(frame, {0, 0}, factor, -factor);
        translate(frame, -1, 1);
    }

    // There can be some minor floating point rounding issues with the scale above.
    // Since we're placing beams directly at the edge, we want to clamp them to
    // the [-1,1] range to prevent them from getting clipped.
    for (Point& p : frame)
    {
        p.x = std::clamp(p.x, -1.0f, 1.0f);
        p.y = std::clamp(p.y, -1.0f, 1.0f);
    }

    return true;
}

int main(int argc, char* argv[])
{
    // Options
    bool scale_to_bbox = false;
    bool optimize = false;

    char opt;
    while ((opt = getopt(argc, argv, "hbo")) != -1)
    {
        switch (opt)
        {
            case 'b':
                scale_to_bbox = true;;
                break;
            case 'o':
                optimize = true;;
                break;
            case 'h':
            default:
                fprintf(stderr, "Usage: %s [options] svg-file ilda-output-file\n", argv[0]);
                fprintf(stderr, "Options:\n");
                fprintf(stderr, "    -h        Prints this help output\n");
                fprintf(stderr, "    -b        Scale the lit bounding-box to fit the frame, instead of using the document bounds.\n");
                fprintf(stderr, "    -o        Optimize the frame.\n");
                exit(EXIT_FAILURE);
        }
    }

    if (optind + 2 != argc)
    {
        fprintf(stderr, "Expected SVG file and output ILDA file after options\n");
        exit(EXIT_FAILURE);
    }

    Frame frame;
    if (!load_svg(argv[optind], frame, scale_to_bbox))
    {
        return EXIT_FAILURE;
    }

    decimate(frame);

    if (optimize)
    {
        Optimizer opt;
        if (opt.run(frame) != LZR_SUCCESS)
        {
            return EXIT_FAILURE;
        }
    }

    // Write to ILDA file
    ILDA* f = ilda_open(argv[optind + 1], "w");
    if (f == nullptr)
    {
        perror("Failed to open ILDA file: ");
        return EXIT_FAILURE;
    }

    // ilda_write internally clips any remaining beams outside of [-1,1]
    if (ilda_write(f, 0, frame) != LZR_SUCCESS)
    {
        perror("Failed to open ILDA file: ");
        return EXIT_FAILURE;
    }
    ilda_close(f);

    return EXIT_SUCCESS;
}
