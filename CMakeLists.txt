cmake_minimum_required(VERSION 3.7.1)
project(Game)

add_executable("${PROJECT_NAME}" src/main/main.cpp)

add_subdirectory("glfw")
target_include_directories("${PROJECT_NAME}" PUBLIC glfw/include)
target_link_libraries("${PROJECT_NAME}" glfw)
SET(SDL2_LINKER_FLAGS COMMAND sdl2-config --libs)
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -framework OpenGL -framework GLUT")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${SDL2_LINKER_FLAGS}")
