# Find the path to the fontatlas library.

FIND_PATH(FONTATLAS_INCLUDE_DIRS
  PATHS
  /include
  /usr/include
  /usr/local/include
)

FIND_LIBRARY(FONTATLAS_LIBRARY
  NAMES fontatlas libfontatlas
  PATHS
  /
  /usr
  /usr/local
  PATH_SUFFIXES lib64 lib
)

# handle the QUIETLY and REQUIRED arguments and set FONTATLAS_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FontAtlas DEFAULT_MSG FONTATLAS_LIBRARY FONTATLAS_INCLUDE_DIRS)
