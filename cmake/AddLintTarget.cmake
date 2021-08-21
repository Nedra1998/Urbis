function(add_lint_target NAME)
  set(options TEST)
  set(single_value_args TARGET EXECUTABLE COMMENT TEST_NAME)
  set(multi_value_args SOURCES ARGS EXTRA_ARGS TEST_ARGS FIX_ARGS)
  cmake_parse_arguments("" "${options}" "${single_value_args}"
                        "${multi_value_args}" ${ARGN})

  if(_TARGET AND TARGET ${_TARGET})
    get_target_property(sources ${_TARGET} SOURCES)
    list(APPEND _SOURCES ${sources})
  endif()

  if(NOT _EXECUTABLE AND NOT EXISTS "${_EXECUTABLE}")
    message(FATAL_ERROR "Lint target requires an executable")
  endif()

  if(NOT _COMMENT)
    if(_TARGET)
      set(_COMMENT "Executing ${_EXECUTABLE} on ${_TARGET}")
    else()
      set(_COMMENT "Executing ${_EXECUTABLE}")
    endif()
  endif()

  if(NOT _TEST_NAME)
    if(_TARGET)
      get_filename_component(exec_basename "${_EXECUTABLE}" NAME_WE)
      set(_TEST_NAME "${exec_basename}.${_TARGET}")
    else()
      set(_TEST_NAME "${NAME}")
    endif()
  endif()

  add_custom_target(
    "${NAME}"
    COMMAND ${_EXECUTABLE} ${_ARGS} ${_EXTRA_ARGS} ${_SOURCES}
    COMMENT "${_COMMENT}"
    DEPENDS ${_SOURCES})

  if(_FIX_ARGS)
    add_custom_target(
      "${NAME}-fix"
      COMMAND ${_EXECUTABLE} ${_ARGS} ${_FIX_ARGS} ${_SOURCES}
      COMMENT "${_COMMENT}"
      DEPENDS ${_SOURCES})
  endif()

  if(_TEST OR _TEST_ARGS)
    add_test(NAME ${_TEST_NAME} COMMAND ${_EXECUTABLE} ${_ARGS} ${_TEST_ARGS}
                                        ${_SOURCES})
  endif()
endfunction()
