#include <gtest/gtest.h>

#include <dcl/OS/File.h>

TEST(File, ConstructsWithEmptyString) {
  dcl::OS::File file { "", 0 };
  EXPECT_EQ(file.getBytes(), nullptr);
  EXPECT_EQ(file.getFd(), -1);
  EXPECT_EQ(file.getSize(), 0);
}
