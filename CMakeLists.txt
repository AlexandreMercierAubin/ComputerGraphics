cmake_minimum_required (VERSION 3.5.1)

#project name
project (Infographics C CXX)

#allows the use of ${CMAKE_CURRENT_SOURCE_DIR} variable
set(CMAKE_INCLUDE_CURRENT_DIR ON)


#Do that to access SDL as an external. No need to do that for Opengl as it is on every OS.

if(WIN32 OR MSVS OR MSYS OR MINGW)
	MESSAGE("This is a windows system")
	if(CMAKE_CL_64)
		MESSAGE("Building ofr X64")
		set(sdlExternal /Externals/SDL2-2.0.7/lib/x64)
		set(glewExternal /Externals/glew-2.1.0/lib/Release/x64)
		
	else()
		MESSAGE("Building for x86")
		set(sdlExternal /Externals/SDL2-2.0.7/lib/x86)
		set(glewExternal /Externals/glew-2.1.0/lib/Release/Win32)
		
	endif()
	
	find_library(SDL2_LIBRARIES NAMES SDL2 SDL2main SDL2test 
			  PATHS ${CMAKE_CURRENT_SOURCE_DIR}${sdlExternal})
	find_library(GLEW_LIBRARIES NAMES GLEW glew32 glew glew32s
			  PATHS ${CMAKE_CURRENT_SOURCE_DIR}${glewExternal})
			  
	set(SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Externals/SDL2-2.0.7/include)
	set(GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Externals/glew-2.1.0/include)
	
	MESSAGE("SDL2 found:" ${SDL2_FOUND})
	MESSAGE("GLEW found:" ${GLEW_FOUND})
elseif(APPLE)
	MESSAGE("Really dude? anyway, this might or might not work")
	find_package(GLEW REQUIRED)
	find_package(SDL2 REQUIRED)
	MESSAGE("SDL2 found:" ${SDL2_FOUND})
	MESSAGE("GLEW found:" ${GLEW_FOUND})
elseif(UNIX AND NOT APPLE)
	MESSAGE("Linux, you are cooler than that apple dude")
	find_package(SDL2 REQUIRED)
	find_package(GLEW REQUIRED)
	#INCLUDE(FindPkgConfig)
	#PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	MESSAGE("SDL2 found:" ${SDL2_FOUND})
	MESSAGE("GLEW found:" ${GLEW_FOUND})
endif()

			  
#Sets Src as the project folder
aux_source_directory(Src SRC_LIST)


#creates the project from Src
add_executable(${PROJECT_NAME} ${SRC_LIST})


if(WIN32 OR MSVS OR MSYS OR MINGW)
	#post-build copy of the dll for window users
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
			"${PROJECT_SOURCE_DIR}${sdlExternal}/SDL2.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
			
	#post-build copy of the dll for window users
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
			"${PROJECT_SOURCE_DIR}${glewExternal}/glew32.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()

		
#A simple find_package works to get opengl
find_package(OpenGL REQUIRED)
MESSAGE("OpenGL found:" ${OpenGL_FOUND})

#include and links the externals
include_directories(${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${GLEW_LIBRARIES})
