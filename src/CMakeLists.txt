file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
generate_version_header("${CMAKE_CURRENT_BINARY_DIR}/include/version.hpp")
list(APPEND SOURCES "${CMAKE_CURRENT_BINARY_DIR}/include/version.hpp")

add_executable(urbis ${SOURCES})
target_compile_features(urbis PRIVATE cxx_std_17)
target_include_directories(urbis PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
                                         "${CMAKE_CURRENT_BINARY_DIR}/include")
target_link_libraries(urbis cxxopts fmt spdlog cereal)
