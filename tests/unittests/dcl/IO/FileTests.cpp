#include <gtest/gtest.h>

#include <dcl/IO/File.h>

TEST(File, ConstructsWithEmptyString) {
  dcl::IO::File file { "", dcl::IO::Permissions::None };
  EXPECT_EQ(file.getBytes(), nullptr);
  EXPECT_EQ(file.getFd(), -1);
  EXPECT_EQ(file.getSize(), 0);
}

TEST(File, ConstructsWithString) {
  dcl::IO::File file { "/usr/lib/dyld", dcl::IO::Permissions::Read };
  EXPECT_TRUE(file.getBytes() != nullptr);
  EXPECT_TRUE(file.getFd() != -1);
  EXPECT_TRUE(file.getSize() != 0);
}