include(FindPackageHandleStandardArgs)

find_package(Python3
  REQUIRED
  COMPONENTS Interpreter
)

get_filename_component(_PYTHON_DIR "${Python3_EXECUTABLE}" DIRECTORY)
set(_PY_PATHS
  "${_PYTHON_DIR}/.."
  "${_PYTHON_DIR}"
  "${_PYTHON_DIR}/bin"
  "${_PYTHON_DIR}../Scripts"
  "${_PYTHON_DIR}/Scripts"
)

find_program(Breathe_EXECUTABLE
  NAMES breathe-apidoc breathe-apidoc.exe
  HINTS _PY_PATHS
)

execute_process(
  COMMAND ${Breathe_EXECUTABLE} --version
  RESULT_VARIABLE _RESPONSE_CODE
  OUTPUT_VARIABLE _Breathe_RESPONSE
)

if(_RESPONSE_CODE EQUAL 0)
  string(REPLACE 
    "Breathe (breathe-apidoc) " 
    "" 
    Breathe_VERSION ${_Breathe_RESPONSE}
  )
  string(STRIP 
    ${Breathe_VERSION}
    Breathe_VERSION
  )
endif()

mark_as_advanced(Breathe_EXECUTABLE)

find_package_handle_standard_args(Breathe
  VERSION_VAR Breathe_VERSION
  REQUIRED_VARS
  Breathe_EXECUTABLE
)
