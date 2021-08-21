# PROJECT {{{

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(
  Urbis
  VERSION 0.0.1
  LANGUAGES C CXX)

# }}}
# MODULES {{{

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(FeatureSummary)
include(CheckIPOSupported)
include(EnableExtraCompilerWarnings)
include(EnableProfiling)
include(CMakeDependentOption)
include(SetDefaultBuildType)
include(GenerateVersionHeader)
include(AddLintTarget)

find_package(CCache)
find_package(ClangFormat)
find_package(ClangTidy)
find_package(CmakeFormat)
find_package(CmakeLint)
find_package(CppCheck)
find_package(CppLint)
find_package(IWYU)

# }}}
# SETUP {{{

set_default_build_type("Debug")
check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT output)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

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

# }}}
# OPTIONS {{{

option(ENABLE_TESTS "Enable unit-tests" ON)

cmake_dependent_option(ENABLE_IPO "Enable interprocedural-optimizations" ON
                       "IPO_SUPPORTED" OFF)
cmake_dependent_option(ENABLE_CCACHE "Enable CCache compiler launcher" ON
                       "CCACHE_FOUND" OFF)
cmake_dependent_option(ENABLE_PCH "Enable precompiled-headers" ON
                       "CMAKE_VERSION VERSION_GREATER_EQUAL 3.16.0" OFF)

option(ENABLE_LINTING "Enable automated linter testing" ON)
cmake_dependent_option(
  LINT_CLANGTIDY "Enable the clang-tidy static analyzer" ON
  "ENABLE_LINTING;CLANGTIDY_FOUND;EXPORT_COMPILE_COMMANDS" OFF)
cmake_dependent_option(
  LINT_CPPCHECK "Enable the cppcheck static analyzer" ON
  "ENABLE_LINTING;CPPCHECK_FOUND;EXPORT_COMPILE_COMMANDS" OFF)
cmake_dependent_option(
  LINT_CPPLINT "Enable the cpplint static analyzer" ON
  "ENABLE_LINTING;CPPLINT_FOUND;EXPORT_COMPILE_COMMANDS" OFF)
cmake_dependent_option(LINT_IWYU "Enable the iwyu static analyzer" ON
                       "ENABLE_LINTING;IWYU_FOUND;EXPORT_COMPILE_COMMANDS" OFF)
cmake_dependent_option(LINT_CMAKELINT "Enable the cmake-lint static analyzer"
                       ON "ENABLE_LINTING;CMAKELINT_FOUND" OFF)

option(ENABLE_FORMATTING "Enable automated formatting tests" ON)
cmake_dependent_option(
  FORMAT_CLANGFORMAT "Enable the clang-formatter formatter" ON
  "ENABLE_FORMATTING;CLANGFORMAT_FOUND" OFF)
cmake_dependent_option(
  FORMAT_CMAKEFORMAT "Enable the clang-formatter formatter" ON
  "ENABLE_FORMATTING;CMAKEFORMAT_FOUND" OFF)

# }}}
# GLOBAL CONFIGURATION {{{

if(ENABLE_CCACHE)
  set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_EXECUTABLE}")
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_EXECUTABLE}")
endif()

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

if(ENABLE_TESTS)
  include(CTest)
  enable_testing()
endif()

# }}}
# TARGETS {{{

add_subdirectory(extern)

add_subdirectory(src)

if(ENABLE_TESTS)
  add_subdirectory(test)
endif()

# }}}
# SUMMARY {{{

# }}}

# vim: foldmethod=marker foldlevel=0
