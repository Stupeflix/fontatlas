CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

project(fontatlas)

if(APPLE)
    set(CMAKE_CXX_FLAGS "-ggdb3 -stdlib=libstdc++ -std=c++11 -Wno-write-strings")
else()
    set(CMAKE_CXX_FLAGS "-ggdb3 -std=c++11 -Wno-write-strings")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules"
                      "${CMAKE_MODULE_PATH}" )

# Find required libraries.

find_package(Math REQUIRED)
find_package(PkgConfig REQUIRED)
if(PKGCONFIG_FOUND)
    pkg_check_modules(FREETYPE freetype2)
endif()
find_package(PNG REQUIRED)

# Set source files.

file(GLOB_RECURSE HEADERS "include/*.hpp")
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Include libs directories.

include_directories(${FREETYPE_INCLUDE_DIRS}
                    ${MATH_INCLUDE_DIRS}
                    ${PNG_INCLUDE_DIRS}
                    include)

# Create executables and libraries
if(FABFILE_COMPILATION)
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
else(FABFILE_COMPILATION)
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
endif(FABFILE_COMPILATION)

target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${MATH_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARIES})

if(NOT FABFILE_COMPILATION)
    add_executable(font_data ${SOURCES} apps/font_data.cpp)
    target_link_libraries(font_data ${FREETYPE_LIBRARIES})
    target_link_libraries(font_data ${MATH_LIBRARIES})
    target_link_libraries(font_data ${PNG_LIBRARIES})

    add_executable(font_meta_data ${SOURCES} apps/font_meta_data.cpp)
    target_link_libraries(font_meta_data ${FREETYPE_LIBRARIES})
    target_link_libraries(font_meta_data ${MATH_LIBRARIES})
    target_link_libraries(font_meta_data ${PNG_LIBRARIES})

    # Generate install rule.

    install(FILES   ${HEADERS}              DESTINATION include/fontatlas)
    install(TARGETS font_data       RUNTIME DESTINATION bin)
    install(TARGETS font_meta_data  RUNTIME DESTINATION bin)
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
endif(NOT FABFILE_COMPILATION)
