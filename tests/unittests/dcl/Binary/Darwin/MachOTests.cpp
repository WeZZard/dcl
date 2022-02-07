#include <gtest/gtest.h>

#include <dcl/Binary/Darwin/MachO.h>
#include <dcl/BlobGen/BlobGen.h>
#include <dcl/IO/File.h>

TEST(MachO, empty_swift) {
  /* auto path = dcl::BlobGen::getBlobPath("source_code.swift",
   *                                       "darwin-aarch64e-iphoneos",
   *                                       SwiftInXcodeDefaultToolchain);
   */
  dcl::IO::File file{
    BLOBS_PATH "/macOS/empty_swift", dcl::IO::Permissions::Read};
}
