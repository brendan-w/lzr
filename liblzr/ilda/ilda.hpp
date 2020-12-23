#pragma once

#include "liblzr/core/core.hpp"

namespace lzr {

/******************************************************************************/
/*  ILDA File Handlers                                                        */
/******************************************************************************/

class ILDA;

// "r" = read
// "w" = write
//Will return NULL on failure
LIBLZR_EXPORT ILDA* ilda_open(const char* filename, const char* mode);

//closes the ILDA file, and releases the parsing context
LIBLZR_EXPORT int ilda_close(ILDA* f);

//Reads all frames for the the given projector descriptor, and returns them
//The array you pass to "name" or "company" must be at least 9 chars in length
LIBLZR_EXPORT int ilda_read(ILDA* f, size_t pd, FrameList& frame_list);
LIBLZR_EXPORT int ilda_read(ILDA* f, size_t pd, FrameList& frame_list, char* name, char* company);

//write frame(s) for the given projector descriptor to the ILDA file (file must be opened with ilda_open(<filename>, "w") )
//if no name/company strings are given, they will be null strings.
LIBLZR_EXPORT int ilda_write(ILDA* f, size_t pd, Frame& frame, const char* name=nullptr, const char* company=nullptr);
LIBLZR_EXPORT int ilda_write(ILDA* f, size_t pd, FrameList& frame_list, const char* name=nullptr, const char* company=nullptr);

//returns the number of projectors that the ILDA specifies graphics for
LIBLZR_EXPORT size_t ilda_projector_count(ILDA* f);

//returns the number of frames for the given projector descriptor
LIBLZR_EXPORT size_t ilda_frame_count(ILDA* f, size_t pd);

//return the error string, set whenever the functions above return an error
LIBLZR_EXPORT const char* ilda_error(ILDA* ilda);

} // namespace lzr
