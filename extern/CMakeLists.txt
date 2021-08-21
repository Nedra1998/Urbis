set(OpenGL_GL_PREFERENCE "GLVND" CACHE STRING "")

set(BUILD_TESTS OFF CACHE BOOL "")
set(SKIP_PORTABILITY_TEST ON CACHE BOOL "")
set(SKIP_PERFORMANCE_COMPARISON ON CACHE BOOL "")
set(BUILD_RAYLIB_CPP_EXAMPLES OFF CACHE BOOL "")

add_subdirectory(Catch2 EXCLUDE_FROM_ALL)
add_subdirectory(cxxopts EXCLUDE_FROM_ALL)
add_subdirectory(fmt EXCLUDE_FROM_ALL)
add_subdirectory(raylib EXCLUDE_FROM_ALL)
add_subdirectory(raylib-cpp EXCLUDE_FROM_ALL)
add_subdirectory(spdlog EXCLUDE_FROM_ALL)
add_subdirectory(cereal EXCLUDE_FROM_ALL)
