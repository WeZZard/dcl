cmake_minimum_required(VERSION 3.10)

project(dcl)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

################################################################################
# GTEST
################################################################################

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

################################################################################
# SUBDIRECTORIES
################################################################################

add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(tests)
