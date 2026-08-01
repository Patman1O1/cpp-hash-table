#-----------------------------------------------------------------------------------------------------------------------
# Google Test
#-----------------------------------------------------------------------------------------------------------------------
include(GoogleTest)
enable_testing()

#-----------------------------------------------------------------------------------------------------------------------
# Target: hash_table_test
# Alias: collections::hash_table_test
# Type: Executable
# Dependencies: GTest::gtest_main, GTest::gmock_main, collections::hash_table_headers
#-----------------------------------------------------------------------------------------------------------------------
# Target definition
add_executable(hash_table_test)

# Target alias
add_executable(collections::hash_table_test ALIAS hash_table_test)

# Target sources
target_sources(hash_table_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/hash_table_test.cpp
)

# Target dependencies
target_link_libraries(hash_table_test
    PRIVATE
        collections::hash_table_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(hash_table_test)
