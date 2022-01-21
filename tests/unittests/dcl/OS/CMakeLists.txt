enable_testing()

include_directories(../../../lib/os)

add_executable(
  unittests_libdclOS
  FileTests.cpp
)

target_link_libraries(
  unittests_libdclOS
  dclOS
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(unittests_libdclOS)
