file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(unit-tests ${SOURCES})
set_property(TARGET unit-tests PROPERTY INTERPROCEDURAL_OPTIMIZATION
                                        ${ENABLE_IPO})
target_include_directories(unit-tests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(unit-tests PRIVATE urbis Catch2 Catch2WithMain)

if(ENABLE_PCH)
  target_precompile_headers(unit-tests PRIVATE <catch2/catch_test_macros.hpp>)
endif()

include(Catch)
catch_discover_tests(unit-tests TEST_PREFIX "unit.")
