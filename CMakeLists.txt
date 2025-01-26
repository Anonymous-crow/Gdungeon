cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(Gdungeon VERSION 0.0.1 LANGUAGES C CXX)

set(THIS_HOME_DIR ${PROJECT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${THIS_HOME_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${THIS_HOME_DIR}/bin>)

set(PROJECT_SOURCE_DIR "${THIS_HOME_DIR}/src")
set(PROJECT_INCLUDE_DIR "${THIS_HOME_DIR}/include")

set(CMAKE_DEBUG_POSTFIX d)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(WIN32)

    include(FetchContent)
    FetchContent_Declare(SQLite
        URL "https://www.sqlite.org/2024/sqlite-amalgamation-3470200.zip"
        DOWNLOAD_EXTRACT_TIMESTAMP true
    )
    FetchContent_MakeAvailable(SQLite)
    add_library(sqlite3 STATIC
        ${sqlite_SOURCE_DIR}/sqlite3.c
    )
    target_include_directories(sqlite3 PRIVATE
        ${sqlite_SOURCE_DIR}
    )
else()
    find_package(SQLite3 REQUIRED)
endif()

find_package(Doxygen)
if (DOXYGEN_FOUND)
    # something
endif()

if(PROJECT_IS_TOP_LEVEL)
  include(CTest)
  enable_testing()
  add_subdirectory("tests")
endif()

add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/classes.cpp
    ${PROJECT_SOURCE_DIR}/catalog.cpp
    ${PROJECT_SOURCE_DIR}/cli.cpp
    ${PROJECT_SOURCE_DIR}/arena.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}-${CMAKE_GENERATOR}-${CMAKE_C_COMPILER_ID}")

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIR})

target_compile_definitions(${PROJECT_NAME}
    PRIVATE PROGRAM_NAME="${PROJECT_NAME}"
    PRIVATE VERSION="${PROJECT_VERSION}"
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        sqlite3
    )
    target_include_directories(${PROJECT_NAME} PUBLIC "${sqlite_SOURCE_DIR}")
else()
    target_link_libraries(${PROJECT_NAME} 
        PRIVATE SQLite::SQLite3
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${THIS_HOME_DIR}/assets/cards.db
                       ${EXECUTABLE_OUTPUT_PATH}/assets/cards.db
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CPACK_PACKAGE_DIRECTORY ${THIS_HOME_DIR}/package)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)
install(DIRECTORY assets/ DESTINATION assets/)

option(CPACK_BINARY_TZ off)
# set(CPACK_RPM_PACKAGE_REQUIRES "sqlite3")

set(CPACK_PACKAGE_VENDOR "ComradeCrow")
# set(CPACK_RESOURCE_FILE_LICENSE ${THIS_HOME_DIR}/LICENSE.txt)
# set(CPACK_RESOURCE_FILE_README ${THIS_HOME_DIR}/README.md)
include(CPack)
