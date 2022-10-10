cmake_minimum_required(VERSION 3.7.1)
add_definitions(-w -std=c++11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
project(Game)

# TODO this does not work
execute_process(COMMAND python3 codegen.py OUTPUT_VARIABLE BLAH)

file(GLOB_RECURSE SRC_FILES "src/*.cpp" "resources/scripts/*.cpp")
add_executable("${PROJECT_NAME}" "${SRC_FILES}")
# https://cmake.org/pipermail/cmake/2012-June/050674.html
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list src/*.h resources/scripts/*.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()
HEADER_DIRECTORIES(H_DIRS)
target_include_directories("${PROJECT_NAME}" PUBLIC "${H_DIRS}")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries("${PROJECT_NAME}" ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

add_subdirectory("glfw")
target_include_directories("${PROJECT_NAME}" PUBLIC glfw/include)
target_link_libraries("${PROJECT_NAME}" glfw)
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -framework OpenGL -framework GLUT")
