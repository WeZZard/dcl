#include <gtest/gtest.h>

#include <dcl/Binary/Darwin/MachO.h>
#include <dcl/IO/File.h>

TEST(MachO, empty_swift) {
  dcl::IO::File file{
    BLOBS_PATH "/macOS/empty_swift", dcl::IO::Permissions::Read};
}
