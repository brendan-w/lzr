

#include <stdio.h>
#include "optimize.h"


void path_split(lzr_point_buffer* points, lzr_path_buffer* paths)
{
	size_t n = 0;         //number of completed paths (index of the path currently being built)
	bool in_path = false; //whether the loop is inside an unterminated path

	for(size_t i = 0; i < points->length; i++)
	{
		lzr_point p = points->points[i];

		if(IS_BLANKED(p))
		{
			if(in_path)
			{
				//encountered first blanked point when IN a path
				//close the open path
				paths->paths[n].bi = i - 1; //store the index of the last point in the path
				n++;
				in_path = false;
			}
			// else, do nothing, discard blanked points
		}
		else //a lit point
		{
			if(in_path)
			{
				//test the angle this point makes with previous/next points
			}
			else
			{
				//encountered first lit point when not in a path
				//open a new path
				paths->paths[n].ai = i; //store the index of the first point in the path
				in_path = true;
			}
		}
	}

	//if a path is still open, close it
	if(in_path)
	{
		paths->paths[n].bi = points->length - 1;
		n++;
		in_path = false;
	}

	paths->length = n;
}
