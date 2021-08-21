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

if(ENABLE_FORMATTING)
  add_lint_target(
    urbis-clangformat TEST
    TARGET urbis
    EXECUTABLE "${CLANGFORMAT_EXECUTABLE}"
    TEST_ARGS "--Werror" "--dry-run"
    FIX_ARGS "-i" EXTRA_ARGS "--dry-run")
endif()

if(ENABLE_LINTING)
  add_lint_target(
    urbis-cppcheck TEST
    TARGET urbis
    EXECUTABLE "${CPPCHECK_EXECUTABLE}"
    ARGS "--cppcheck-build-dir=${CMAKE_CURRENT_BINARY_DIR}"
         "--project=${PROJECT_BINARY_DIR}/compile_commands.json" "--std=c++17"
         "-i${PROJECT_SOURCE_DIR}/extern" "-i${PROJECT_SOURCE_DIR}/test")

  add_lint_target(
    urbis-clangtidy TEST
    TARGET urbis
    EXECUTABLE "${CLANGTIDY_EXECUTABLE}"
    ARGS "-p" "${PROJECT_BINARY_DIR}/compile_commands.json"
    TEST_ARGS "--warnings-as-errors=*"
    FIX_ARGS "--fix-errors" "--fix")

  add_lint_target(
    urbis-iwyu
    TARGET urbis
    EXECUTABLE "${IWYU_TOOL_EXECUTABLE}"
    ARGS "-p" "${PROJECT_BINARY_DIR}/compile_commands.json" "--output-format"
         "iwyu") 
endif()

add_executable(urbis-exec "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
set_property(TARGET urbis-exec PROPERTY INTERPROCEDURAL_OPTIMIZATION
                                        ${ENABLE_IPO})
target_link_libraries(urbis-exec PRIVATE urbis cxxopts)

if(ENABLE_PCH)
  target_precompile_headers(urbis-exec PRIVATE <cxxopts.hpp>)
endif()
