find_package(Sphinx)

if(Sphinx_FOUND)
  find_package(Furo)
  find_package(Breathe REQUIRED)

  set(SPHINX_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
  set(SPHINX_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")

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
    "${SPHINX_SOURCE_DIR}/conf.py"
    @ONLY
  )
else()
  message(WARNING
    "Sphinx not found! Documentation not available"
  )
endif()
