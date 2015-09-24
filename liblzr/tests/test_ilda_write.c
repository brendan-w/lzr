

#include <stdio.h>
#include <assert.h>
#include <lzr.h>

#define N_FRAMES 2

int main()
{
    lzr_ilda_file* f = lzr_ilda_write("ilda_write_test.ild");

    lzr_frame* frames = (lzr_frame*) calloc(sizeof(lzr_frame), N_FRAMES);


    //read the frames
    lzr_ilda_write_frames(f, 0, frames, N_FRAMES);




    for(size_t i = 0; i < frames[0].n_points; i++)
    {
        lzr_point p = frames[0].points[i];
        // printf("(%f, %f) i=%d\n", p.x, p.y, p.i);
    }

    free(frames);
    lzr_ilda_close(f);

    return 0;
}
