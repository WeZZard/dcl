enable_testing()

add_executable(
  libdclIO_unittests
  FileTests.cpp
)

target_include_directories(
  libdclIO_unittests
  INTERFACE
  ../../../lib/IO
)

target_link_libraries(
  libdclIO_unittests
  dclIO
  gtest_main
)

include(GoogleTest)

gtest_discover_tests(libdclIO_unittests)
