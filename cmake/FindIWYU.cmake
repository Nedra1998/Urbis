# include(FindPackageMacro)
# find_executable(IWYU "[0-9]+\\\\.[0-9]+" "iwyu" "include-what-you-use")
include(FindPackageHandleStandardArgs)

macro(find_executable NAME VERSION)
  string(TOUPPER "${NAME}" EXEC)
  file(TO_CMAKE_PATH "${${EXEC}_ROOT_DIR}" ${EXEC}_ROOT_DIR)
  set(${EXEC}_ROOT_DIR
      "${${EXEC}_ROOT_DIR}"
      CACHE PATH "Path to search for ${NAME}")

  set(_old_app_bundle_settings ${CMAKE_FIND_APPBUNDLE})
  set(CMAKE_FIND_APPBUNDLE NEVER)

  if(${EXEC}_EXECUTABLE AND NOT EXISTS "${${EXEC}_EXECUTABLE}")
    set(${EXEC}_EXECUTABLE
        "notfound"
        CACHE PATH FORCE "")
  endif()

  if(${EXEC}_ROOT_DIR)
    find_program(
      ${EXEC}_EXECUTABLE
      NAMES ${ARGN}
      PATHS "${${EXEC}_ROOT_DIR}"
      NO_DEFAULT_PATH)
  else()
    find_program(${EXEC}_EXECUTABLE NAMES ${ARGN})
  endif()

  set(CMAKE_FIND_APPBUNDLE ${_old_app_bundle_settings})

  if("${VERSION}")
    execute_process(
      COMMAND "${${EXEC}_EXECUTABLE}" --version
      OUTPUT_VARIABLE ${EXEC}_VERSION
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(REGEX REPLACE ".*(${VERSION}).*" "\\1" ${EXEC}_VERSION
                         "${${EXEC}_VERSION}")
  endif()
  if(${EXEC}_FOUND OR ${EXEC}_MARK_AS_ADVANCED)
    mark_as_advanced(${EXEC}_ROOT_DIR)
  endif()
  mark_as_advanced(${EXEC}_EXECUTABLE)
endmacro()

find_executable(IWYU "[0-9]+\\\\.[0-9]+" "iwyu" "include-what-you-use")
find_executable(IWYU_TOOL "" "iwyu-tool")
find_executable(IWYU_FIX_INCLUDES "" "iwyu-fix-includes")

find_package_handle_standard_args(
  IWYU
  REQUIRED_VARS IWYU_EXECUTABLE IWYU_TOOL_EXECUTABLE
                IWYU_FIX_INCLUDES_EXECUTABLE
  VERSION_VAR ${EXEC}_VERSION)
