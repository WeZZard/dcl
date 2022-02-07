enable_testing()

add_executable(
  libdclBinary_unittests
  ./Darwin/MachOTests.cpp
  ./Darwin/MachOViewTests.cpp
)

add_subdirectory(Darwin)

target_link_libraries(
  libdclBinary_unittests
  dclIO
  dclBinary
  dclBlobGen
  gtest_main
)

include(GoogleTest)

gtest_discover_tests(libdclBinary_unittests)
