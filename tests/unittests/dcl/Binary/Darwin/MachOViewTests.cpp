#include <gtest/gtest.h>

#include <dcl/ADT/ByteOrder.h>
#include <dcl/IO/File.h>
#include <dcl/Binary/Darwin/MachOView.h>
#include <dcl/Binary/Darwin/Collections.h>
#include <dcl/Binary/Darwin/Dyld/DyldFixupChains.h>
#include <dcl/Binary/Darwin/Dyld/DyldInfo.h>

TEST(MachOView, constructor_with_valid_file) {
  dcl::IO::File file { BLOBS_PATH "/macOS/empty_swift", dcl::IO::Permissions::Read };
  dcl::Binary::Darwin::MachOView view { file.getBytes() };
  EXPECT_NE(view.getBytes(), nullptr);
}

TEST(MachOView, empty_swift_to_1_slice) {
  dcl::IO::File file { BLOBS_PATH "/macOS/empty_swift", dcl::IO::Permissions::Read };
  dcl::Binary::Darwin::MachOView view { file.getBytes() };
  size_t sliceCount = std::distance(view.begin(), view.end());
  EXPECT_EQ(sliceCount, 1);
}

TEST(MachOView, empty_swift) {
  dcl::IO::File file { BLOBS_PATH "/macOS/empty_swift", dcl::IO::Permissions::Read };
  dcl::Binary::Darwin::MachOView view { file.getBytes() };
  for (auto eachSlice : view) {
    switch (eachSlice.getMachOFormat()) {
      case dcl::Binary::Darwin::Format::BigEndianess32Bit: {
        eachSlice.getMachO<dcl::Binary::Darwin::Remote<uint32_t>, dcl::ADT::BigEndianess>();
        break;
      }
      case dcl::Binary::Darwin::Format::BigEndianess64Bit: {
        auto machO = eachSlice.getMachO<dcl::Binary::Darwin::Remote<uint64_t>, dcl::ADT::BigEndianess>();
        dcl::Binary::Darwin::LoadCommandCollection loadCommands { machO->getHeader() };
        for (auto& eachLoadCommand : loadCommands) {
          auto command = eachLoadCommand.getCommand();
        }
        break;
      }
      case dcl::Binary::Darwin::Format::LittleEndianess32Bit: {
        eachSlice.getMachO<dcl::Binary::Darwin::Remote<uint32_t>, dcl::ADT::LittleEndianess>();
        break;
      }
      case dcl::Binary::Darwin::Format::LittleEndianess64Bit: {
        auto machO = eachSlice.getMachO<dcl::Binary::Darwin::Remote<uint64_t>, dcl::ADT::LittleEndianess>();
        dcl::Binary::Darwin::LoadCommandCollection loadCommands { machO->getHeader() };
        for (auto& eachLoadCommand : loadCommands) {
          auto command = eachLoadCommand.getCommand();
        }
        break;
      }
      case dcl::Binary::Darwin::Format::Unknown:
        FAIL();
        break;
    }
  }
  size_t sliceCount = std::distance(view.begin(), view.end());
  EXPECT_EQ(sliceCount, 1);
}