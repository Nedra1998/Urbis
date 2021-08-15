# PROJECT
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(
  Urbis
  VERSION 0.1.0
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

# TARGETS

add_subdirectory(src)

# SUMMARY
