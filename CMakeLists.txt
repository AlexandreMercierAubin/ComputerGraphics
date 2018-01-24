cmake_minimum_required (VERSION 3.7)

project (Infographics C CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Externals/SDL2-2.0.7/include)
find_library(SDL2_LIBRARY NAMES SDL2 SDL2main SDL2test 
			  PATHS ${CMAKE_CURRENT_SOURCE_DIR}/Externals/SDL2-2.0.7/lib/x86)



aux_source_directory(Src SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/Externals/SDL2-2.0.7/lib/x86"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

find_package(OpenGL REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${SDL2_LIBRARY})