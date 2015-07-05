

#include "optimize.h"
#include "find_paths.h"
#include "rearrange_paths.h"
#include "compile_paths.h"


//the two main working buffers
lzr_point_buffer points;
lzr_path_buffer  paths;

//accessors
#define points(i) (points.points[i])
#define paths(i)  (paths.paths[i])


static void load()
{
    //create some bogus data to play with
    //                        x, y, r, g, b, i
    // lzr_point p0 = POINT_INIT(0, 0, 1, 1, 1, 1); points(0) = p0;
    // lzr_point p1 = POINT_INIT(9, 1, 1, 1, 1, 1); points(1) = p1;
    // lzr_point p2 = POINT_INIT(2, 2, 1, 1, 1, 1); points(2) = p2;
    // lzr_point p3 = POINT_INIT(3, 3, 1, 1, 1, 1); points(3) = p3;
    // lzr_point p4 = POINT_INIT(4, 4, 1, 1, 1, 1); points(4) = p4;
    // lzr_point p5 = POINT_INIT(5, 5, 1, 1, 1, 0); points(5) = p5;
    // lzr_point p6 = POINT_INIT(6, 6, 1, 1, 1, 1); points(6) = p6;
    // lzr_point p7 = POINT_INIT(7, 7, 1, 1, 1, 1); points(7) = p7;
    // lzr_point p8 = POINT_INIT(8, 8, 1, 1, 1, 0); points(8) = p8;
    // lzr_point p9 = POINT_INIT(9, 9, 1, 1, 1, 1); points(9) = p9;
    //                        x, y, r, g, b, i
    lzr_point p0 = POINT_INIT(0, 0, 1, 1, 1, 1); points(0) = p0;
    lzr_point p1 = POINT_INIT(0, 8, 1, 1, 1, 1); points(1) = p1;
    lzr_point p2 = POINT_INIT(0, 8, 1, 1, 1, 0); points(2) = p2;

    lzr_point p6 = POINT_INIT(1, 0, 1, 1, 1, 1); points(3) = p6;
    lzr_point p7 = POINT_INIT(1, 8, 1, 1, 1, 1); points(4) = p7;
    lzr_point p8 = POINT_INIT(1, 8, 1, 1, 1, 0); points(5) = p8;

    lzr_point p4 = POINT_INIT(2, 0, 1, 1, 1, 1); points(6) = p4;
    lzr_point p3 = POINT_INIT(2, 8, 1, 1, 1, 1); points(7) = p3;
    lzr_point p5 = POINT_INIT(2, 8, 1, 1, 1, 0); points(8) = p5;

    points.length = 9;
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
        printf("%zu: [%zu, %zu] a=%f b=%f\n", i, paths(i).a.i, paths(i).b.i, paths(i).a.angle, paths(i).b.angle);        
    }
}

// int main(int argc, char* argv[])
int main()
{
    load();

    //pretty simple
    find_paths(&points, &paths);
    rearrange_paths(&points, &paths);
    compile_paths(&points, &paths);

    print_buffers();
    return 0;
}