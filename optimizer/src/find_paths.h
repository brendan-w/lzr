

#ifndef FIND_PATHS_H
#define FIND_PATHS_H

/*
    Splits a point buffer into individual path segments.
    Loads the result into the given path buffer.
*/
void find_paths(lzr_point_buffer* points, lzr_path_buffer* paths);


#endif /* FIND_PATHS_H */
