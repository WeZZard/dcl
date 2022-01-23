enable_testing()

include_directories(../../../lib/os)

add_executable(
  libdclOS_unittests
  FileTests.cpp
)

target_link_libraries(
  libdclOS_unittests
  dclOS
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(libdclOS_unittests)
