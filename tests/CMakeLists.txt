#-----------------------------------------------------------------------------------------------------------------------
# Google Test
#-----------------------------------------------------------------------------------------------------------------------
include(GoogleTest)
enable_testing()

#-----------------------------------------------------------------------------------------------------------------------
# Target: cpp_hash_table_test
# Alias: collections::cpp_hash_table_test
# Type: Executable
# Dependencies: GTest::gtest_main, GTest::gmock_main, collections::cpp_hash_table_headers
#-----------------------------------------------------------------------------------------------------------------------
# Target definition
add_executable(cpp_hash_table_test)

# Target alias
add_executable(collections::cpp_hash_table_test ALIAS cpp_hash_table_test)

# Target sources
target_sources(cpp_hash_table_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_hash_table_test.cpp
)

# Target dependencies
target_link_libraries(cpp_hash_table_test
    PRIVATE
        collections::cpp_hash_table_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(cpp_hash_table_test)