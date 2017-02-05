
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
     * Construct a show
     */
    Clip* clip = new Clip("test clip");

    FrameEffect* frame_effect = new FrameEffect();
    Constant* n = new Constant();
    n->x = 0.0;
    frame_effect->n = n;
    frame_effect->frames = frames;
    clip->effects.push_back(frame_effect);

    // make a translate effect
    TranslateEffect* translate = new TranslateEffect();
    Constant* x = new Constant();
    Constant* y = new Constant();
    x->x = 0.0;
    y->x = 0.0;
    translate->x = x;
    translate->y = y;

    Show show;
    show.clips["test clip"] = clip;
    show.timeline.push_back({ clip, 0.0, 1.0 });

    /*
     * Run the show
     */
    FrameList output;
    output.push_back(show(0.5));

    /*
     * Write ILDA file
     */

    f = ilda_open("output.ilda", "w");

    if(ilda_write(f, 0, output) != LZR_SUCCESS)
    {
        fprintf(stderr, "Error saving frames\n");
        return EXIT_FAILURE;
    }

    printf("Wrote %zu frames to output.ilda\n", output.size());
    printf("Wrote %zu points\n", output[0].size());

    ilda_close(f);
}
