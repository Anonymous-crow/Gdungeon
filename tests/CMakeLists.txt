set(EXECUTABLE_OUTPUT_PATH "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "")

if(UNIX)
    set(sqlite_SOURCE_DIR "")
endif()


add_library(testCommon SHARED
        ${PROJECT_SOURCE_DIR}/catalog.cpp
        ${PROJECT_SOURCE_DIR}/classes.cpp
)
target_include_directories(testCommon PRIVATE 
        ${PROJECT_INCLUDE_DIR}
        ${sqlite_SOURCE_DIR}
        ${PROJECT_BINARY_DIR}
)
if(WIN32)
    target_link_libraries(testCommon PRIVATE
            sqlite3
    )
else()
    target_link_libraries(testCommon PRIVATE
        SQLite::SQLite3
    )
endif()
set_target_properties(testCommon 
        PROPERTIES 
        VERSION 
        ${PROJECT_VERSION}
)
set_target_properties(testCommon 
        PROPERTIES 
        SOVERSION 
        ${PROJECT_VERSION_MAJOR}
)
# generate_export_header(testCommon)

add_executable(addPartyMember
    addPartyMember.cpp
)
target_link_libraries(addPartyMember PRIVATE
    testCommon
)
target_include_directories(addPartyMember 
    PRIVATE 
    ${PROJECT_INCLUDE_DIR}
    ${sqlite_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
)
add_test(NAME addPartyMember
         COMMAND addPartyMember
         WORKING_DIRECTORY ${THIS_HOME_DIR}
)
set (passRegex "^Number of players: 4" "^rudy mikey slick victor")
set_property (TEST addPartyMember
              PROPERTY PASS_REGULAR_EXPRESSION "${passRegex}"
)

add_executable(removePartyMember
    removePartyMember.cpp
)
target_link_libraries(removePartyMember PRIVATE
    testCommon
)
target_include_directories(removePartyMember 
    PRIVATE 
    ${PROJECT_INCLUDE_DIR}
    ${sqlite_SOURCE_DIR}
)
add_test(NAME removePartyMember
         COMMAND removePartyMember
         WORKING_DIRECTORY ${THIS_HOME_DIR}
)
set (passRegex "^All Tests Passed")
set (failRegex "Error" "Fail")
set_property (TEST removePartyMember
              PROPERTY PASS_REGULAR_EXPRESSION "${passRegex}"
)

