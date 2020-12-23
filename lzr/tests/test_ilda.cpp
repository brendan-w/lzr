
#include <cstdio>
#include <string.h>

#include "lzr/lzr.hpp"

#define TMP_ILDA_FILE "/tmp/output.ilda"
#define NAME "NAME----"
#define COMPANY "COMPANY-"

using namespace lzr;

int compare_frames(FrameList& original, FrameList& frames)
{
    if(original.size() == frames.size())
    {
        for(size_t f = 0; f < frames.size(); f++)
        {
            if(original[f].size() == frames[f].size())
            {
                for(size_t p = 0; p < frames[f].size(); p++)
                {
                    if(original[f][p] != frames[f][p])
                    {
                        fprintf(stderr, "Points don't match\n");
                        return EXIT_FAILURE;
                    }
                }
            }
            else
            {
                fprintf(stderr, "Read a different number of points for frame %zu\n", f);
                return EXIT_FAILURE;
            }
        }
    }
    else
    {
        fprintf(stderr, "Read a different number of frames (%zu --> %zu)\n", original.size(), frames.size());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: test_ilda <ilda-file>");
        return EXIT_SUCCESS;
    }

    /*
     * Open ILDA file
     */
    FrameList original;

    // load the ILDA file
    ILDA* f = ilda_open(argv[1], "r");

    // printf("Found %zu projector(s)\n", ilda_projector_count(f));
    // printf("Found %zu frames for projector 0\n", ilda_frame_count(f, 0));

    if(ilda_read(f, 0, original) != LZR_SUCCESS)
    {
        fprintf(stderr, "Error loading frames\n");
        return EXIT_FAILURE;
    }

    ilda_close(f);

    /*
     * Write ILDA file
     */

    f = ilda_open(TMP_ILDA_FILE, "w");

    if(ilda_write(f, 0, original, NAME, COMPANY) != LZR_SUCCESS)
    {
        fprintf(stderr, "Error saving frames\n");
        return EXIT_FAILURE;
    }

    // printf("Wrote %zu frames to output.ilda\n", ilda_frame_count(f, 0));

    ilda_close(f);

    /*
     * Read the ILDA file we just wrote
     */

    f = ilda_open(TMP_ILDA_FILE, "r");

    // printf("Found %zu projector(s)\n", ilda_projector_count(f));
    // printf("Found %zu frames for projector 0\n", ilda_frame_count(f, 0));

    char name[9];
    char company[9];
    FrameList frames;
    if(ilda_read(f, 0, frames, name, company) != LZR_SUCCESS)
    {
        fprintf(stderr, "Error loading frames\n");
        return EXIT_FAILURE;
    }

    ilda_close(f);

    std::remove(TMP_ILDA_FILE);

    if(strcmp(NAME, name) != 0)
    {
        fprintf(stderr, "name string doesn't match\n");
        return EXIT_FAILURE;
    }

    if(strcmp(COMPANY, company) != 0)
    {
        fprintf(stderr, "company string doesn't match\n");
        return EXIT_FAILURE;
    }

    // check that the frames we read are identical to the original
    return compare_frames(original, frames);
}
