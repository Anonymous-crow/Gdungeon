include(FindPackageHandleStandardArgs)

find_package(Python3
  REQUIRED
  COMPONENTS Interpreter
)

execute_process(
  COMMAND ${Python3_EXECUTABLE} -m pip show furo
  RESULT_VARIABLE FURO_CHECK_RESULT
  OUTPUT_VARIABLE FURO_CHECK_OUTPUT
)
mark_as_advanced(FURO_CHECK_OUTPUT)
mark_as_advanced(FURO_CHECK_RESULT)

string(REPLACE "\n" ";" FURO_CHECK_LINES "${FURO_CHECK_OUTPUT}")
foreach(_line ${FURO_CHECK_LINES})
  string(FIND ${_line} "Version:" _VERSION)
  if(NOT (_VERSION EQUAL "-1"))
    string(REPLACE "Version: " "" Furo_VERSION ${_line})
  endif()
  string(FIND ${_line} "Location:" _LOCATION)
  if(NOT (_LOCATION EQUAL "-1"))
    string(REPLACE "Location: " "" Furo_LOCATION ${_line})
    string(STRIP Furo_LOCATION ${Furo_LOCATION})
    string(APPEND Furo_LOCATION "/furo")
  endif()
endforeach()

if(${FURO_CHECK_RESULT} EQUAL 0)
  set(Furo_FOUND YES)
else()
  set(Furo_FOUND NO)
endif()

find_package_handle_standard_args(Furo
  FOUND_VAR Furo_FOUND
  VERSION_VAR Furo_VERSION
  REQUIRED_VARS Furo_LOCATION
)
