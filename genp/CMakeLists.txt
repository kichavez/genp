# CMakeList.txt : CMake project for genp, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


set(SDL2_INCLUDE_DIR C:/SDL/SDL2-2.0.14/include)
set(SDL2_LIB_DIR C:/SDL/SDL2-2.0.14/lib/x64)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

# Add source to this project's executable.
add_executable (genp "main.cpp")
target_link_libraries(${PROJECT_NAME} SDL2main SDL2)

# TODO: Add tests and install targets if needed.
