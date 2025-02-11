set(EXECUTABLE_OUTPUT_PATH "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "")


if(UNIX)
  set(sqlite_SOURCE_DIR "")
endif()
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets)
add_custom_target(link_target ALL
  COMMAND ${CMAKE_COMMAND} -E create_symlink
  ${THIS_HOME_DIR}/assets/cards.db
  ${CMAKE_CURRENT_BINARY_DIR}/assets/cards.db
  COMMENT "ADDING SYMLINK FOR TESTS"
)


add_library(testCommon SHARED
  ${PROJECT_SOURCE_DIR}/catalog.cpp
  ${PROJECT_SOURCE_DIR}/classes.cpp
  ${PROJECT_SOURCE_DIR}/arena.cpp
)
add_dependencies(testCommon link_target)
target_include_directories(testCommon PRIVATE
  ${PROJECT_INCLUDE_DIR}
  ${sqlite_SOURCE_DIR}
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
)
add_test(NAME addPartyMember
  COMMAND addPartyMember
  WORKING_DIRECTORY ${THIS_HOME_DIR}
)
set(PASS_REGEX "^Number of players: 4" "^rudy mikey slick victor")
set_property(TEST addPartyMember
  PROPERTY PASS_REGULAR_EXPRESSION "${PASS_REGEX}"
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
set(PASS_REGEX "^All Tests Passed")
set(FAIL_REGEX "Error" "Fail")
set_property(TEST removePartyMember
  PROPERTY PASS_REGULAR_EXPRESSION "${PASS_REGEX}"
)
set_property(TEST removePartyMember
  PROPERTY FAIL_REGULAR_EXPRESSION "${FAIL_REGEX}"
)


add_executable(addCard
  addCard.cpp
)
target_link_libraries(addCard PRIVATE
  testCommon
)
target_include_directories(addCard
  PRIVATE
  ${PROJECT_INCLUDE_DIR}
  ${sqlite_SOURCE_DIR}
)
add_test(NAME addCard
  COMMAND addCard
  WORKING_DIRECTORY ${THIS_HOME_DIR}
)
set(PASS_REGEX "All Tests Passed")
set_property(TEST addCard
  PROPERTY PASS_REGULAR_EXPRESSION "${PASS_REGEX}"
)


add_executable(removeCard
  removeCard.cpp
)
target_link_libraries(removeCard PRIVATE
  testCommon
)
target_include_directories(removeCard
  PRIVATE
  ${PROJECT_INCLUDE_DIR}
  ${sqlite_SOURCE_DIR}
)
add_test(NAME removeCard
  COMMAND removeCard
  WORKING_DIRECTORY ${THIS_HOME_DIR}
)
set_property(TEST removeCard
  PROPERTY PASS_REGULAR_EXPRESSION "${PASS_REGEX}"
)

add_custom_target(buildTests)
add_dependencies(buildTests addCard removeCard addPartyMember removePartyMember)
