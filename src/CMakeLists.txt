file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
generate_version_header("${CMAKE_CURRENT_BINARY_DIR}/include/version.hpp")
list(APPEND SOURCES "${CMAKE_CURRENT_BINARY_DIR}/include/version.hpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_library(urbis ${SOURCES})
target_compile_features(urbis PUBLIC cxx_std_17)
set_property(TARGET urbis PROPERTY INTERPROCEDURAL_OPTIMIZATION ${ENABLE_IPO})
target_include_directories(urbis PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
                                        "${CMAKE_CURRENT_BINARY_DIR}/include")
target_link_libraries(
  urbis
  PUBLIC fmt spdlog
  PRIVATE cereal raylib raylib-cpp)

if(ENABLE_PCH)
  target_precompile_headers(
    urbis
    PUBLIC
    <fmt/format.h>
    <spdlog/spdlog.h>
    <spdlog/common.h>
    PRIVATE
    <raylib.h>
    <cereal/cereal.hpp>)
endif()

add_executable(urbis-exec "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
set_property(TARGET urbis-exec PROPERTY INTERPROCEDURAL_OPTIMIZATION
                                        ${ENABLE_IPO})
target_link_libraries(urbis-exec PRIVATE urbis cxxopts)

if(ENABLE_PCH)
  target_precompile_headers(urbis-exec PRIVATE <cxxopts.hpp>)
endif()
