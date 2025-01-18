set(EXECUTABLE_OUTPUT_PATH "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "")

add_library(testClasses SHARED
    ${PROJECT_SOURCE_DIR}/classes.cpp
)
target_include_directories(testClasses PRIVATE ${PROJECT_INCLUDE_DIR})
set_target_properties(testClasses 
            PROPERTIES 
            VERSION 
            ${PROJECT_VERSION}
)
set_target_properties(testClasses 
            PROPERTIES 
            SOVERSION 
            ${PROJECT_VERSION_MAJOR}
)


add_library(testCatalog SHARED
    ${PROJECT_SOURCE_DIR}/catalog.cpp
)
target_include_directories(testCatalog PRIVATE ${PROJECT_INCLUDE_DIR})
set_target_properties(testCatalog 
            PROPERTIES 
            VERSION 
            ${PROJECT_VERSION}
)
set_target_properties(testCatalog 
            PROPERTIES 
            SOVERSION 
            ${PROJECT_VERSION_MAJOR}
)

add_executable(addPartyMember
    addPartyMember.cpp
)
target_link_libraries(addPartyMember PRIVATE
    testCatalog
    testClasses
    SQLite::SQLite3
)
target_include_directories(addPartyMember 
    PRIVATE 
    ${PROJECT_INCLUDE_DIR}
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
    testCatalog
    testClasses
    SQLite::SQLite3
)
target_include_directories(removePartyMember 
    PRIVATE 
    ${PROJECT_INCLUDE_DIR}
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

