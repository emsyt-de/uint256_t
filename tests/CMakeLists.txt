find_package(GTest REQUIRED)

add_executable(UInt256Tests)

target_link_libraries(UInt256Tests PRIVATE UInt256 GTest::GTest  GTest::Main)

gtest_discover_tests(UInt256Tests)

add_subdirectory(testcases)

