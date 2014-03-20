CMAKE_MINIMUM_REQUIRED( VERSION 2.8.5 )

project(fontatlas)

# Config.
set(CMAKE_CXX_FLAGS "-ggdb3 -std=c++0x -Wno-write-strings")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules"
                      "${CMAKE_MODULE_PATH}" )

# Find required libraries.
find_package(Math REQUIRED)
find_package(FreeType REQUIRED)
find_package(PNG REQUIRED)

# Set source files.
file(GLOB_RECURSE HEADERS "src/*.hpp")
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Include libs directories.
include_directories(${FREETYPE_INCLUDE_DIRS}
                    ${MATH_INCLUDE_DIRS}
                    ${PNG_INCLUDE_DIRS}
                    src)

# Create executable and link libraries.
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} app/main.cpp)
add_library(${PROJECT_NAME}_lib ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${MATH_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARIES})

# Copy fonts into build directory for testing.
file(GLOB FONTS "fonts/*.ttf")
file(COPY ${FONTS}   DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/fonts)

# Generate install rule.
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)