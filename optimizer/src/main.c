

#include "optimize.h"
#include "find_paths.h"


//the two main working buffers
lzr_point_buffer points;
lzr_path_buffer  paths;

//accessors
#define points(i) (points.points[i])
#define paths(i)  (paths.paths[i])


static void load()
{
    //create some bogus data to play with
    lzr_point p0 = { .x=0, .y=0, .r=1, .g=1, .b=1, .i=1 }; points(0) = p0;
    lzr_point p1 = { .x=9, .y=1, .r=1, .g=1, .b=1, .i=1 }; points(1) = p1;
    lzr_point p2 = { .x=2, .y=2, .r=1, .g=1, .b=1, .i=1 }; points(2) = p2;
    lzr_point p3 = { .x=3, .y=3, .r=1, .g=1, .b=1, .i=1 }; points(3) = p3;
    lzr_point p4 = { .x=4, .y=4, .r=1, .g=1, .b=1, .i=1 }; points(4) = p4;
    lzr_point p5 = { .x=5, .y=5, .r=1, .g=1, .b=1, .i=0 }; points(5) = p5;
    lzr_point p6 = { .x=6, .y=6, .r=1, .g=1, .b=1, .i=1 }; points(6) = p6;
    lzr_point p7 = { .x=7, .y=7, .r=1, .g=1, .b=1, .i=1 }; points(7) = p7;
    lzr_point p8 = { .x=8, .y=8, .r=1, .g=1, .b=1, .i=0 }; points(8) = p8;
    lzr_point p9 = { .x=9, .y=9, .r=1, .g=1, .b=1, .i=1 }; points(9) = p9;

    points.length = POINT_BUFFER_SIZE;
}

static void print_buffers()
{
    printf("\nPoint buffer:\n");
    for(size_t i = 0; i < points.length; i++)
    {
        printf("%zu: (%d, %d) i=%d\n", i, points(i).x, points(i).y, points(i).i);
    }

    printf("\nPath buffer:\n");
    for(size_t i = 0; i < paths.length; i++)
    {
        printf("%zu: [%zu, %zu] a=%f b=%f\n", i, paths(i).ai, paths(i).bi, paths(i).a_angle, paths(i).b_angle);        
    }
}

// int main(int argc, char* argv[])
int main()
{
    load();
    find_paths(&points, &paths);
    print_buffers();
    return 0;
}
