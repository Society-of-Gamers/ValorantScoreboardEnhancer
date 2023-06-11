# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.17)

# Set project name
project(ValorantScoreboardEnhancer)

# Set C++ standard to use
set(CMAKE_CXX_STANDARD 20)

# Set binary and current binary directories
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_BINARY_DIR})

 # Set executable output path
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

 # Set module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

 # Add subdirectory for source code
add_subdirectory(src)

# add_subdirectory(tests)

# find_library(Boost COMPONENTS program_options REQUIRED)
# find_library(OpenCV REQUIRED)
# find_library(SFML COMPONENTS graphics audio network REQUIRED)

# add_library(Boost COMPONENTS program_options REQUIRED)
# add_library(OpenCV REQUIRED)
# add_library(SFML COMPONENTS graphics audio netowrk REQUIRED)

# link_libraries(Boost OpenCV SFML)
# target_link_libraries(ValorantScoreboardEnhancer Boost OpenCV SFML)