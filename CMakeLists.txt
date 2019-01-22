cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

project(lightdemo)
add_subdirectory(win)

if(WIN32)
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W1" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	endif()
endif()

add_executable(
	lightdemo

	src/main.cpp

	#renderer
	src/render/render.h
	src/render/renderer.cpp
	src/render/quad.cpp
	src/render/shadow.cpp

	#entity
	src/entity/entity.h
	src/entity/entity.cpp

	#shader programs
	asset/quad.vert
	asset/quad.frag
	asset/shadowcaster.vert
	asset/shadowcaster.frag

	build/assets
)

file(GLOB assetfiles asset/*)

add_custom_command(
	OUTPUT
		${CMAKE_CURRENT_LIST_DIR}/build/assets
	COMMAND
		cd ${CMAKE_CURRENT_LIST_DIR}/asset && roll ../build/assets
	DEPENDS ${assetfiles}
)

target_link_libraries(lightdemo PRIVATE win)
target_include_directories(lightdemo PRIVATE win/include)
