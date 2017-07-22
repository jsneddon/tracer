cmake_minimum_required (VERSION 3.0)
project (Tracer)

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()



# The version number.
set (Tracer_VERSION_MAJOR 1)
set (Tracer_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
"${PROJECT_SOURCE_DIR}/TracerConfig.h.in"
"${PROJECT_BINARY_DIR}/TracerConfig.h"
)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

include_directories ("${PROJECT_SOURCE_DIR}/math")
add_subdirectory (math)

include_directories ("${PROJECT_SOURCE_DIR}/geometry")
add_subdirectory (geometry)

add_executable(Tracer main.cpp)
target_link_libraries (Tracer math geometry)

install (TARGETS Tracer DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/TracerConfig.h"
DESTINATION include)
