include(FindPackageHandleStandardArgs)

find_package(Python3
  REQUIRED
  COMPONENTS Interpreter
)

execute_process(
  COMMAND ${Python3_EXECUTABLE} -m pip show sphinx-click
  RESULT_VARIABLE SCLICK_CHECK_RESULT
  OUTPUT_VARIABLE SCLICK_CHECK_OUTPUT
)
mark_as_advanced(SCLICK_CHECK_OUTPUT)
mark_as_advanced(SCLICK_CHECK_RESULT)

string(REPLACE "\n" ";" SCLICK_CHECK_LINES "${SCLICK_CHECK_OUTPUT}")
foreach(_line ${SCLICK_CHECK_LINES})
  string(FIND ${_line} "Version:" _VERSION)
  if(NOT (_VERSION EQUAL "-1"))
    string(REPLACE "Version: " "" SphinxClick_VERSION ${_line})
  endif()
  string(FIND ${_line} "Location:" _LOCATION)
  if(NOT (_LOCATION EQUAL "-1"))
    string(REPLACE "Location: " "" SphinxClick_LOCATION ${_line})
    string(STRIP SphinxClick_LOCATION ${SphinxClick_LOCATION})
    cmake_path(ABSOLUTE_PATH SphinxClick_LOCATION NORMALIZE)
    cmake_path(APPEND SphinxClick_LOCATION "sphinx_click")
  endif()
endforeach()

if(${SCLICK_CHECK_RESULT} EQUAL 0)
  set(SphinxClick_FOUND YES)
else()
  set(SphinxClick_FOUND NO)
endif()

find_package_handle_standard_args(SphinxClick
  FOUND_VAR SphinxClick_FOUND
  VERSION_VAR SphinxClick_VERSION
  REQUIRED_VARS SphinxClick_LOCATION
)
