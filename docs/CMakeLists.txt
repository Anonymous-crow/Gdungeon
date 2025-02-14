find_package(Sphinx REQUIRED)

find_package(Furo)
find_package(Breathe REQUIRED)
find_package(SphinxClick REQUIRED)

set(SPHINX_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(SPHINX_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(SPHINX_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/sphinx")

string(TIMESTAMP SPHINX_TARGET_YEAR "%Y" UTC)
set(SPHINX_TEMPLATES_PATH "${SPHINX_SOURCE_DIR}/_templates")
set(SPHINX_STATIC_PATH "${SPHINX_SOURCE_DIR}/_static")
if(Furo_FOUND)
  set(SPHINX_HTML_THEME "furo")
else()
  set(SPHINX_HTML_THEME "alabaster")
endif()
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/SPHINX_CONF_IN"
  "${SPHINX_BIN_DIR}/conf.py"
  @ONLY
)
set(DOCS_DEPENDS "${SPHINX_BIN_DIR}/conf.py")
add_custom_command(
  OUTPUT "${SPHINX_BUILD_DIR}/html/index.html"
  COMMAND "${Sphinx_EXECUTABLE}" -b dirhtml -c "${SPHINX_BIN_DIR}"
    "${SPHINX_SOURCE_DIR}" "${SPHINX_BUILD_DIR}/html"
  COMMENT "Building Sphinx HTML"
)
list(APPEND DOCS_DEPENDS "${SPHINX_BUILD_DIR}/html/index.html")

string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

add_custom_command(
  OUTPUT "${SPHINX_BUILD_DIR}/latex/${PROJECT_NAME_LOWER}.tex"
  COMMAND "${Sphinx_EXECUTABLE}" -b latex --conf-dir
    "${SPHINX_BIN_DIR}" "${SPHINX_SOURCE_DIR}" "${SPHINX_BUILD_DIR}/latex"
  COMMENT "Building Sphinx LaTeX"
)

add_custom_command(
  OUTPUT "${SPHINX_BUILD_DIR}/latex/${PROJECT_NAME_LOWER}.pdf"
  WORKING_DIRECTORY "${SPHINX_BUILD_DIR}/latex"
  COMMAND make
  DEPENDS "${SPHINX_BUILD_DIR}/latex/${PROJECT_NAME_LOWER}.tex"
  COMMENT "Building Sphinx PDF"
)

if(SPHINX_PDF)
  list(APPEND DOCS_DEPENDS
    "${SPHINX_BUILD_DIR}/latex/${PROJECT_NAME_LOWER}.pdf"
  )
endif()
add_custom_target(docs ALL
  DEPENDS ${DOCS_DEPENDS}
  COMMENT "Build Sphinx Documentation"
)
add_dependencies(docs doxygen_docs)
