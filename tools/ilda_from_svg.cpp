#include <unistd.h>
#include <cstdio>

#define NANOSVG_IMPLEMENTATION  // Expands implementation
#include "nanosvg.h"
#include "gsl/gsl_util"

#include "lzr/lzr.hpp"

using namespace lzr;

int main(int argc, char* argv[])
{
    char opt;
    while ((opt = getopt(argc, argv, "h")) != -1)
    {
        switch (opt)
        {
            case 'h':
            default:
                fprintf(stderr, "Usage: %s [-h] svg-file\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Expected SVG file after options\n");
        exit(EXIT_FAILURE);
    }

    // Load SVG
    NSVGimage* image;
    image = nsvgParseFromFile(argv[optind], "px", 96);
    if (image == nullptr) {
        std::perror("Failed to open SVG file");
        return EXIT_FAILURE;
    }

    // Free the image at the end of the scope
    auto _ = gsl::finally([&]{ nsvgDelete(image); });

    printf("size: %f x %f\n", image->width, image->height);
    // for (NSVGshape *shape = image->shapes; shape != NULL; shape = shape->next) {
    //     for (NSVGpath *path = shape->paths; path != NULL; path = path->next) {
    //         for (int i = 0; i < path->npts-1; i += 3) {
    //             float* p = &path->pts[i*2];
    //             drawCubicBez(p[0],p[1], p[2],p[3], p[4],p[5], p[6],p[7]);
    //         }
    //     }
    // }

    return EXIT_SUCCESS;
}
