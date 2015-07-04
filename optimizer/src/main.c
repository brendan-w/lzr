

#include "optimize.h"
#include "path_split.h"


//the two main working buffers
lzr_point_buffer points;
lzr_path_buffer  paths;

//accessors
#define points(i) (points.points[i])
#define paths(i)  (paths.paths[i])


//create some bogus data to play with
static void load()
{
	for(int i = 0; i < POINT_BUFFER_SIZE; i++)
	{
        lzr_point* p = &points(i);
		p->x = i;
        p->y = i;
        p->r = p->g = p->b = 1; //non-zero color
        p->i = (i % 3 == 0) ? 0 : 1;
	}
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
        printf("%zu: [%d, %d]\n", i, paths(i).ai, paths(i).bi);        
    }
}

// int main(int argc, char* argv[])
int main()
{
    load();
    path_split(&points, &paths);
    print_buffers();
	return 0;
}
