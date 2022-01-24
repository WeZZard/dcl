#include <gtest/gtest.h>

#include <dcl/IO/File.h>
#include <dcl/Binary/Darwin/MachO.h>

TEST(MachO, empty_swift) {
  dcl::IO::File file { BLOBS_PATH "/macOS/empty_swift", dcl::IO::Permissions::Read };
}
