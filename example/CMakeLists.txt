set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

add_definitions(-DSDL_MAIN_HANDLED)

find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(example example.cpp shader.cpp gl_core_4_5.c)

set_target_properties(example PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON)

target_include_directories(example PUBLIC
	$<BUILD_INTERFACE:${SDL2_INCLUDE_DIR}>
	$<BUILD_INTERFACE:${OPENGL_INCLUDE_DIR}>
	$<BUILD_INTERFACE:${GLM_INCLUDE_DIRS}>)

target_link_libraries(example PUBLIC
	arcball_camera
	${SDL2_LIBRARY}
	${OPENGL_LIBRARIES})


