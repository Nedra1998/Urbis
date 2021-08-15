if(__generate_version_header)
  return()
endif()
set(__generate_version_header YES)

find_package(Git QUIET)
set(__version_header_template
    "${CMAKE_CURRENT_LIST_DIR}/GenerateVersionHeader.hpp.in")

function(generate_version_header _dest)
  set(_source "${__version_header_template}")
  if(NOT _dest)
    set(_dest "${CMAKE_CURRENT_BINARY_DIR}/version.hpp")
  endif()

  string(MAKE_C_IDENTIFIER ${PROJECT_NAME} _id)
  string(TOUPPER ${_id} _project_upper)
  string(TOLOWER ${_id} _project_lower)

  if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    include(GetGitRevisionDescription)
    get_git_head_revision(_refspec _hashvar)
    execute_process(
      COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
      WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
      OUTPUT_VARIABLE _commit_sha
      RESULT_VARIABLE _result
      OUTPUT_STRIP_TRAILING_WHITESPACE)

    set(PROJECT_VERSION_BUILD
        "${_commit_sha}"
        CACHE STRING "git short sha" FORCE)
  endif()

  if(NOT EXISTS "${_source}")
    message(
      FATAL_ERROR
        "GenerateVersionHeader.cmake: Version template file ${_source} was not found"
    )
  else()
    configure_file("${_source}" "${_dest}" @ONLY)
  endif()
endfunction()
