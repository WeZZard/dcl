#include <gtest/gtest.h>

#include <dcl/ADT/ByteOrder.h>
#include <dcl/Binary/Darwin/Collections.h>
#include <dcl/Binary/Darwin/Dyld/DyldFixupChains.h>
#include <dcl/Binary/Darwin/Dyld/DyldInfo.h>
#include <dcl/Binary/Darwin/MachOView.h>
#include <dcl/IO/File.h>

using namespace dcl::Binary::Darwin;

TEST(MachOView, constructor_with_valid_file) {
  dcl::IO::File file{
    BLOBS_PATH "/macOS/empty_swift", dcl::IO::Permissions::Read};
  MachOView view{file.getBytes()};
  EXPECT_NE(view.getBytes(), nullptr);
}

TEST(MachOView, empty_swift_to_1_slice) {
  dcl::IO::File file{
    BLOBS_PATH "/macOS/empty_swift", dcl::IO::Permissions::Read};
  MachOView view{file.getBytes()};
  size_t sliceCount = std::distance(view.begin(), view.end());
  EXPECT_EQ(sliceCount, 1);
}

TEST(MachOView, empty_swift) {
  dcl::IO::File file{
    BLOBS_PATH "/macOS/empty_swift", dcl::IO::Permissions::Read};
  MachOView view{file.getBytes()};
  for (auto eachSlice : view) {
    switch (eachSlice.getMachOFormat()) {
    case Format::BigEndianess32Bit: {
      eachSlice.getMachO<Remote<uint32_t>, dcl::ADT::BigEndianess>();
      break;
    }
    case Format::BigEndianess64Bit: {
      auto machO =
        eachSlice.getMachO<Remote<uint64_t>, dcl::ADT::BigEndianess>();
      LoadCommandCollection loadCommands{machO->getHeader()};
      for (auto& eachLoadCommand : loadCommands) {
        auto command = eachLoadCommand.getCommand();
      }
      break;
    }
    case Format::LittleEndianess32Bit: {
      eachSlice.getMachO<Remote<uint32_t>, dcl::ADT::LittleEndianess>();
      break;
    }
    case Format::LittleEndianess64Bit: {
      auto machO =
        eachSlice.getMachO<Remote<uint64_t>, dcl::ADT::LittleEndianess>();
      LoadCommandCollection loadCommands{machO->getHeader()};
      for (auto& eachLoadCommand : loadCommands) {
        auto command = eachLoadCommand.getCommand();
      }
      break;
    }
    case Format::Unknown:
      FAIL();
      break;
    }
  }
  size_t sliceCount = std::distance(view.begin(), view.end());
  EXPECT_EQ(sliceCount, 1);
}