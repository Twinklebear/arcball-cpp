cmake_minimum_required(VERSION 3.5)
project(arcball)

if (NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

find_package(glm REQUIRED)

add_library(arcball_camera arcball_camera.cpp)

set_target_properties(arcball_camera PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON)

target_compile_definitions(arcball_camera PRIVATE
	-DGLM_ENABLE_EXPERIMENTAL)

target_include_directories(arcball_camera PUBLIC
	$<BUILD_INTERFACE:${arcball_SOURCE_DIR}>
	$<BUILD_INTERFACE:${GLM_INCLUDE_DIRS}>
	$<INSTALL_INTERFACE:include>)

add_subdirectory(example)

install(TARGETS arcball_camera EXPORT arcball_cameraTargets
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include)

install(FILES arcball_camera.h
	DESTINATION include)

install(EXPORT arcball_cameraTargets
	DESTINATION lib/cmake/arcball_camera/)

install(FILES arcball_cameraConfig.cmake
	DESTINATION lib/cmake/arcball_camera/)

