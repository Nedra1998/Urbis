# PROJECT
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(
  Urbis
  VERSION 0.0.1
  LANGUAGES C CXX)

# MODULES
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(FeatureSummary)
include(CheckIPOSupported)
include(EnableExtraCompilerWarnings)
include(EnableProfiling)
include(CMakeDependentOption)
include(SetDefaultBuildType)
include(GenerateVersionHeader)

# OPTIONS
set_default_build_type("Debug")
option(ENABLE_PCH "Enable precompiled headers" ON)

check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT output)
cmake_dependent_option(ENABLE_IPO "Enable interprocedural optimizations" ON
                       "IPO_SUPPORTED" OFF)

# CONFIGURATION

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug|RelWithDebInfo")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  if(NOT EXISTS "${PROJECT_SOURCE_DIR}/compile_commands.json")
    execute_process(
      COMMAND
        ${CMAKE_COMMAND} -E create_symlink
        "${PROJECT_BINARY_DIR}/compile_commands.json"
        "${PROJECT_SOURCE_DIR}/compile_commands.json")
  endif()
endif()

# GIT HOOKS

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug" AND EXISTS
                                             "${PROJECT_SOURCE_DIR}/.git")
  find_program(COMMITLINT commitlint)
  if(NOT COMMITLINT)
    message(
      FATAL_ERROR
        "commitlint was not found in your path, it is required before making changes"
    )
  elseif(NOT EXISTS "${PROJECT_SOURCE_DIR}/.git/hooks/commit-msg")
    execute_process(
      COMMAND
        ${CMAKE_COMMAND} -E create_symlink
        "${PROJECT_SOURCE_DIR}/.github/hooks/commit-msg"
        "${PROJECT_SOURCE_DIR}/.git/hooks/commit-msg")
  endif()
endif()

# TARGETS

add_subdirectory(extern)
add_subdirectory(src)

# SUMMARY
