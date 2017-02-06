
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
    n->v = 0.0;
    frame_effect->n = n;
    frame_effect->frames = frames;
    clip->effects.push_back(frame_effect);


    //rotation
    RotateEffect* rotate = new RotateEffect();
    Constant* cx = new Constant();
    Constant* cy = new Constant();
    Linear* theta = new Linear();
    cx->v = 0.0;
    cy->v = 0.0;
    theta->input_map[&(theta->v)] = "clip_time";
    theta->from_a = 0.0;
    theta->from_b = 1.0;
    theta->to_a = 0.0;
    theta->to_b = 10.0;
    rotate->cx = cx;
    rotate->cy = cy;
    rotate->theta = theta;
    clip->effects.push_back(rotate);

    // make a translate effect
    TranslateEffect* translate = new TranslateEffect();
    Constant* x = new Constant();
    Constant* y = new Constant();
    x->input_map[&(x->v)] = "clip_time";
    y->v = 0.0;
    translate->x = x;
    translate->y = y;
    clip->effects.push_back(translate);

    Show show;
    show.clips["test clip"] = clip;
    show.timeline.push_back({ clip, 0.0, 1.0 });

    /*
     * Run the show
     */
    FrameList output;
    for(double t = 0.0; t <= 1.0; t += 0.01)
    {
        output.push_back(show(t));
    }

    /*
     * Write ILDA file
     */

    f = ilda_open("/tmp/output.ilda", "w");

    if(ilda_write(f, 0, output) != LZR_SUCCESS)
    {
        fprintf(stderr, "Error saving frames\n");
        return EXIT_FAILURE;
    }

    printf("Wrote %zu frames to output.ilda\n", output.size());
    printf("Wrote %zu points\n", output[0].size());

    ilda_close(f);

    show.save("/tmp/show/");
}
