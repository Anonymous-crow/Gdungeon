cmake_minimum_required(VERSION 3.10.0)
project(Gdungeon VERSION 0.1.0 LANGUAGES C CXX)

set(THIS_HOME_DIR ${PROJECT_SOURCE_DIR})

set(PROJECT_SOURCE_DIR "src")

set(EXECUTABLE_OUTPUT_PATH ${THIS_HOME_DIR}/bin)

find_package(SQLite3 REQUIRED)

if(PROJECT_IS_TOP_LEVEL)
  include(CTest)
  enable_testing()
endif()

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/classes.cpp
    src/catalog.cpp
    src/cli.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}-${CMAKE_GENERATOR}-${CMAKE_C_COMPILER_ID}")

target_compile_definitions(${PROJECT_NAME}
    PRIVATE PROGRAM_NAME="${PROJECT_NAME}"
    PRIVATE VERSION="${PROJECT_VERSION}"
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE SQLite::SQLite3
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION packbin)
include(CPack)