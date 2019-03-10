include(CMakeFindDependencyMacro)

find_dependency(glm REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/arcball_cameraTargets.cmake")

