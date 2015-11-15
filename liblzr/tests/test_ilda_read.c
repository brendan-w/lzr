

#include <stdio.h>
#include <assert.h>

#include <lzr.h>
#include "../ilda/ilda_utils.h"

int main()
{
    //check sizes of types
    assert(sizeof(ilda_header)           == 32);
    assert(sizeof(ilda_status)           == 1 );
    assert(sizeof(ilda_color)            == 3 );
    assert(sizeof(ilda_point_2d_indexed) == 6 );
    assert(sizeof(ilda_point_2d_true)    == 8 );
    assert(sizeof(ilda_point_3d_indexed) == 8 );
    assert(sizeof(ilda_point_3d_true)    == 10);
    printf("struct sizes OK\n");

    lzr_ilda_file* f = lzr_ilda_read("../../Downloads/CanadaFlag.ild");

    size_t n_projectors = lzr_ilda_projector_count(f);
    printf("n_projectors: %zu\n", n_projectors);
    if(n_projectors == 0) return -1;

    size_t n_frames = lzr_ilda_frame_count(f, 0); //projector 0
    printf("n_frames: %zu\n", n_frames);

    lzr_frame* frames = (lzr_frame*) calloc(sizeof(lzr_frame), n_frames);


    //read the frames
    lzr_ilda_read_frames(f, 0, frames);

    printf("n_points: %d\n", frames[0].n_points);

    for(size_t i = 0; i < frames[0].n_points; i++)
    {
        lzr_point p = frames[0].points[i];
        // printf("(%f, %f) i=%d\n", p.x, p.y, p.i);
    }

    free(frames);
    lzr_ilda_close(f);

    return 0;
}
