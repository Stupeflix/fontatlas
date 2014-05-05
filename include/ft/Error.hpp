
#pragma once

#include <string>
#include <exception>
#include "ft/ft.hpp"
#include "utils/convert.hpp"

namespace ft {

/**
 * Initialize freetype errors.
 */
#undef __FTERRORS_H__
#define FT_ERRORDEF( e, v, s )  { e, s },
#define FT_ERROR_START_LIST     {
#define FT_ERROR_END_LIST       { 0, 0 } };
static const struct {
    int          code;
    const char*  message;
} FT_Errors[] =
#include <fterrors.h>

/**
 * Simple exception class to handle freetype errors.
 */
class Error : public std::exception {
 public:

  /**
   * Create a freetype error.
   */
  Error(FT_Error const &error,
                std::string const &file = "",
                int line = -1) throw() {
    _error = error;
    if (!file.empty())
      _what += file + ": ";
    if (line > 0)
      _what += utils::convert<std::string>(line) + ": ";
    _what = "freetype fails with code ";
    _what += utils::convert<std::string>(getCode()) + " and message \"";
    _what += std::string(FT_Errors[error].message) + "\".";
  }

  /**
   * Empty destructor.
   */
  virtual ~Error() throw() {}

  /**
   * Return the error code.
   */
  size_t getCode() const {
    return FT_Errors[_error].code;
  }

  /**
   * Return the non-formated message.
   */
  char const *getMessage() const {
    return FT_Errors[_error].message;
  }

  /**
   * Return the reason of the exception.
   */
  virtual char const *what() const throw() {
    return _what.c_str();
  }

 private:
  FT_Error _error;
  std::string _what;
};

}