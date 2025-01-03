cmake_minimum_required(VERSION 3.10.0)
project(Gdungeon VERSION 0.1.0 LANGUAGES C CXX)

set(THIS_HOME_DIR ${PROJECT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${THIS_HOME_DIR}/bin)

set(PROJECT_SOURCE_DIR "src")


if(WIN32)

    include(ExternalProject)
    ExternalProject_Add(
        sqlite3
        PREFIX ${CMAKE_BINARY_DIR}/sqlite3
        URL "https://www.sqlite.org/2024/sqlite-amalgamation-3470200.zip"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
        DOWNLOAD_EXTRACT_TIMESTAMP true
    )
    ExternalProject_Get_Property(sqlite3 source_dir)
    set(Sqlite3IncludeDir ${source_dir})
    message(Sqlite3IncludeDir=${Sqlite3IncludeDir})

else()
    find_package(SQLite3 REQUIRED)
endif()


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

if(WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE ${Sqlite3IncludeDir})
else()
    target_link_libraries(${PROJECT_NAME} 
        PRIVATE SQLite::SQLite3
    )
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION packbin)
include(CPack)