
#pragma once

#include <sstream>

namespace utils {

/**
 * Convert a value from any basic type to another using
 * standard stream.
 */
template<typename To, typename From>
To convert(From const &from) {
  std::stringstream s;
  To to;
  s << std::boolalpha << from;
  s >> to;
  return to;
}

}
