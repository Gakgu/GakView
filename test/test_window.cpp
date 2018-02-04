#include <GakView/window.hpp>
#include <gtest/gtest.h>

#include <string>

TEST(Window, Create)
{
  gakview::Window w;
  EXPECT_EQ(0, w.Create());
}

TEST(Window, Update)
{
  gakview::Window w;
  std::string str1("/home/gakgu/Downloads/asdf");
  std::string str2("/home/gakgu/Downloads/f15.bmp");
  std::string str3("/home/gakgu/Downloads/asdf.jpg");
  EXPECT_EQ(-1, w.Update(str1));
  EXPECT_EQ(0, w.Update(str2));
  EXPECT_EQ(0, w.Update(str3));
}
