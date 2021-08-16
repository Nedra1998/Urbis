set(OpenGL_GL_PREFERENCE "GLVND")

set(SKIP_PORTABILITY_TEST ON)
set(SKIP_PERFORMANCE_COMPARISON ON)

add_subdirectory(cxxopts EXCLUDE_FROM_ALL)
add_subdirectory(fmt EXCLUDE_FROM_ALL)
add_subdirectory(raylib EXCLUDE_FROM_ALL)
add_subdirectory(spdlog EXCLUDE_FROM_ALL)
add_subdirectory(cereal EXCLUDE_FROM_ALL)
