cmake_minimum_required(VERSION 3.10)
project(SDL2Game)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -g -Wall")

# Source files
file(GLOB SOURCES "src/*.cpp")

# Include directories
include_directories(
    include
    /usr/local/include/SDL2
    /usr/include/SDL2
)

# Link directories
link_directories(/usr/local/lib)

# Output directory for debug build
set(DEBUG_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/debug)

# Debug build target
add_executable(main_debug ${SOURCES})
set_target_properties(main_debug PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DEBUG_OUTPUT_DIR})
target_link_libraries(main_debug SDL2main SDL2 SDL2_image SDL2_ttf SDL2_mixer)

# Custom target that builds and runs
add_custom_target(run ALL
    COMMAND ${DEBUG_OUTPUT_DIR}/main
    DEPENDS main_debug
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building and running SDL2 debug executable..."
)
