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
		set(sdlImageExternal /Externals/SDL2_image-2.0.2/lib/x64)
		set(glewExternal /Externals/glew-2.1.0/lib/Release/x64)
		set(assimpExternal /Externals/glew-2.1.0/lib/x64)
		
	else()
		MESSAGE("Building for x86")
		set(sdlExternal /Externals/SDL2-2.0.7/lib/x86)
		set(sdlImageExternal /Externals/SDL2_image-2.0.2/lib/x86)
		set(glewExternal /Externals/glew-2.1.0/lib/Release/Win32)
		set(assimpExternal /Externals/Assimp/lib/x86)
		
	endif()
	
	find_library(SDL2_LIBRARIES NAMES SDL2 SDL2main SDL2test
			  PATHS ${CMAKE_CURRENT_SOURCE_DIR}${sdlExternal})
			  
	find_library(SDL2IMAGE_LIBRARIES NAMES SDL2_image
			  PATHS ${CMAKE_CURRENT_SOURCE_DIR}${sdlExternal})
			  
	find_library(GLEW_LIBRARIES NAMES GLEW glew32 glew glew32s
			  PATHS ${CMAKE_CURRENT_SOURCE_DIR}${glewExternal})
			  
	find_library(ASSIMP_LIBRARIES NAMES GLEW assimp
			  PATHS ${CMAKE_CURRENT_SOURCE_DIR}${assimpExternal})
			  
	set(SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Externals/SDL2-2.0.7/include)
	set(SDL2IMAGE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Externals/SDL2-2.0.7/include)
	set(GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Externals/glew-2.1.0/include)
	set(ASSIMP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Externals/Assimp/include)
	
elseif(APPLE)
	MESSAGE("Really dude? anyway, this might or might not work")
	find_package(GLEW REQUIRED)
	find_package(SDL2 REQUIRED)
	find_package(ASSIMP REQUIRED)

elseif(UNIX AND NOT APPLE)
	MESSAGE("Linux, you are cooler than that apple dude")
	find_package(SDL2 REQUIRED)
	find_package(GLEW REQUIRED)
	find_package(ASSIMP REQUIRED)

	MESSAGE("SDL2 found:" ${SDL2_FOUND})

	set (CMAKE_CXX_FLAGS "-std=c++14")
endif()


MESSAGE("SDL2 found:" ${SDL2_FOUND})
MESSAGE("SDL2 found:" ${SDL2IMAGE_FOUND})
MESSAGE("GLEW found:" ${GLEW_FOUND})
MESSAGE("ASSIMP found:" ${ASSIMP_FOUND})
		  
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
			"${PROJECT_SOURCE_DIR}${sdlExternal}/SDL2_image.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
			"${PROJECT_SOURCE_DIR}${sdlExternal}/libpng16-16.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
			"${PROJECT_SOURCE_DIR}${sdlExternal}/zlib1.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
			"${PROJECT_SOURCE_DIR}${sdlExternal}/libtiff-5.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
			"${PROJECT_SOURCE_DIR}${sdlExternal}/libwebp-7.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
			"${PROJECT_SOURCE_DIR}${glewExternal}/glew32.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
			"${PROJECT_SOURCE_DIR}${assimpExternal}/assimp-vc140-mt.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
			

endif()

#A simple find_package works to get opengl
find_package(OpenGL REQUIRED)
MESSAGE("OpenGL found:" ${OpenGL_FOUND})

set(GLM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Externals/glm-0.9.9-a2)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory 
			"${PROJECT_SOURCE_DIR}/Resources"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources)


#include and links the externals
include_directories(${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${GLM_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${GLEW_LIBRARIES} ${ASSIMP_LIBRARIES})
