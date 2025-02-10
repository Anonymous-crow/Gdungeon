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

find_program(Sphinx_EXECUTABLE
  NAMES sphinx-build sphinx-build.exe
  HINTS _PY_PATHS
)

find_program(Sphinx_QUICKSTART
  NAMES sphinx-quickstart sphinx-quickstart.exe
  HINTS _PY_PATHS
)

execute_process(COMMAND ${Sphinx_EXECUTABLE} --version
  RESULT_VARIABLE _CODE
  OUTPUT_VARIABLE _OUT_VERSION
)

if(_CODE EQUAL 0)
  string(REPLACE
    "sphinx-build "
    ""
    Sphinx_VERSION
    ${_OUT_VERSION}
  )
  string(STRIP 
    ${Sphinx_VERSION}
    Sphinx_VERSION
  )
endif()

mark_as_advanced(Sphinx_EXECUTABLE)
mark_as_advanced(Sphinx_QUICKSTART)

find_package_handle_standard_args(Sphinx
  VERSION_VAR Sphinx_VERSION
  REQUIRED_VARS
    Sphinx_EXECUTABLE
    Sphinx_QUICKSTART
  HANDLE_COMPONENTS
)
