cmake_minimum_required (VERSION 3.7)

#project name
project (Infographics C CXX)

#allows the use of ${CMAKE_CURRENT_SOURCE_DIR} variable
set(CMAKE_INCLUDE_CURRENT_DIR ON)


#Do that to access SDL as an external. No need to do that for Opengl as it is on every OS.
set(SFML_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Externals/SFML-2.4.2/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Externals/SFML-2.4.2/lib)

			  
#Sets Src as the project folder
aux_source_directory(Src SRC_LIST)


#creates the project from Src
add_executable(${PROJECT_NAME} ${SRC_LIST})


#post-build copy of the dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${PROJECT_SOURCE_DIR}/Externals/SFML-2.4.2/bin"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

		
#A simple find_package works to get opengl
find_package(OpenGL REQUIRED)


#include and links the externals
include_directories( ${OPENGL_INCLUDE_DIRS} ${SFML_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} sfml-main sfml-graphics sfml-system sfml-window)