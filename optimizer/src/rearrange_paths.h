

#ifndef FIND_PATHS_H
#define FIND_PATHS_H

/*
    Arranges the paths in the path buffer to minimize blank time,
    and route the laser in a sensical path.
*/
void rearrange_paths(lzr_point_buffer* points, lzr_path_buffer* paths);


#endif /* FIND_PATHS_H */
