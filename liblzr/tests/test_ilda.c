

#include <stdio.h>
#include <assert.h>

#include <lzr.h>
#include "../ilda/lzr_ilda.h"

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

    lzr_ilda_file* f = lzr_ilda_read("../../../Downloads/ildatest.ild");

    printf("n_projectors: %zu\n", lzr_ilda_projector_count(f));

    lzr_ilda_close(f);

    return 0;
}
