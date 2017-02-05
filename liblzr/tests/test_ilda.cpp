
#include <liblzr.hpp>
#include <engine.hpp>

using namespace lzr;

int main(int argc, char* argv[])
{
	/*
	 * Open ILDA file
	 */
	FrameList frames;

	// load the ILDA file
    ILDA* f = ilda_open(argv[1], "r");

    printf("Found %zu projector(s)\n", ilda_projector_count(f));
    printf("Found %zu frames for projector 0\n", ilda_frame_count(f, 0));

    if(ilda_read(f, 0, frames) != LZR_SUCCESS)
    {
        fprintf(stderr, "Error loading frames\n");
        return EXIT_FAILURE;
    }

    ilda_close(f);

	/*
	 * Write ILDA file
	 */

    f = ilda_open("output.ilda", "w");

    if(ilda_write(f, 0, frames) != LZR_SUCCESS)
    {
        fprintf(stderr, "Error saving frames\n");
        return EXIT_FAILURE;
    }

    printf("Wrote %zu frames to output.ilda\n", frames.size());

    ilda_close(f);
}
