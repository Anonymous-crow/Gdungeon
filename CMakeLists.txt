cmake_minimum_required(VERSION 3.5.0)
project(Gdungeon VERSION 0.1.0 LANGUAGES C CXX)

set(THIS_HOME_DIR ${PROJECT_SOURCE_DIR})

set(PROJECT_SOURCE_DIR "src")

set(EXECUTABLE_OUTPUT_PATH ${THIS_HOME_DIR}/bin)

if(PROJECT_IS_TOP_LEVEL)
  include(CTest)
  enable_testing()
endif()

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/classes.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}-${CMAKE_GENERATOR}-${CMAKE_C_COMPILER_ID}")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)