

#ifndef FIND_PATHS_H
#define FIND_PATHS_H

/*
    Rebuilds the point buffer according to the modified path buffer.
    Inserts/generates new blanking jumps.
*/
void compile_paths(lzr_point_buffer* points, lzr_path_buffer* paths);


#endif /* FIND_PATHS_H */
