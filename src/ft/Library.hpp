
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include "ft/Error.hpp"

namespace ft {

/**
 * Initialize and release the freetype library.
 */
class Library {
 public:

  /**
   * Return the handle of the freetype library.
   */
  static FT_Library &getHandle() {
    if (_initialized)
      return _handle;
    FT_Error error = FT_Init_FreeType(&_handle);
    if (error)
      throw Error(error, __FILE__, __LINE__);
    atexit(&Library::_atExit);
    return _handle;
  }

 private:

  /**
   * Release freetype.
   */
  static void _atExit() {
    if (_initialized)
      FT_Done_FreeType(_handle);
  }

  static FT_Library _handle;
  static bool _initialized;

};

}