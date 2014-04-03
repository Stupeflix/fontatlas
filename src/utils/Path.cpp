
#include "utils/Path.hpp"

namespace utils {

std::string getFileName(std::string const &path) {
  size_t slash = path.find_last_of('/');
  return slash != std::string::npos ? path.substr(slash + 1) : path;
}

}