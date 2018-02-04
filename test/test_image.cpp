#include <GakView/image.hpp>
#include <gtest/gtest.h>
#include <string>

TEST(Image, SetIndex)
{
  gakview::Image i;

  // expected test succes
  char const *r[2] = {"asdf", "../"};
  EXPECT_EQ(0, i.SetFileList(r));
  EXPECT_EQ(-1, i.SetIndex(r));
  i.m_fileList.clear();
  i.m_index = -1;

  char const *f[2] = {"asdf", "../../../Downloads/f15.bmp"};
  EXPECT_EQ(0, i.SetFileList(f));
  EXPECT_EQ(0, i.SetIndex(f));
  i.m_fileList.clear();
  i.m_index = -1;

  char const *g[2] = {"asdf", "../../../Downloads/asdf"};
  EXPECT_EQ(0, i.SetFileList(g));
  EXPECT_EQ(-1, i.SetIndex(g));
  i.m_fileList.clear();
  i.m_index = -1;

  char const *h[2] = {"asdf", "../../../Downloads"};
  EXPECT_EQ(0, i.SetFileList(h));
  EXPECT_EQ(-1, i.SetIndex(h));
  i.m_fileList.clear();
  i.m_index = -1;
  

  // expected test fail
  char const *p[2] = {"asdf", "asdfdd"};
  EXPECT_EQ(0, i.SetFileList(p));
  EXPECT_EQ(-1, i.SetIndex(p));
  i.m_fileList.clear();
  i.m_index = -1;

  char const *q[2] = {"asdf", ""};
  EXPECT_EQ(0, i.SetFileList(q));
  EXPECT_EQ(-1, i.SetIndex(q));
  i.m_fileList.clear();
  i.m_index = -1;

  char const *w[2] = {"asdf", "/"};
  EXPECT_EQ(0, i.SetFileList(w));
  EXPECT_EQ(-1, i.SetIndex(w));
  i.m_fileList.clear();
  i.m_index = -1;

  char const *e[2] = {"asdf", ".."};
  EXPECT_EQ(0, i.SetFileList(e));
  EXPECT_EQ(-1, i.SetIndex(e)) << "hihihi";
  i.m_fileList.clear();
  i.m_index = -1;
}

TEST(Image, GetFilePath)
{
  gakview::Image i;

  char const *a[2] = {"asdf", "../"};
  EXPECT_STREQ("/home/gakgu/Desktop/GakView/test/../", i.GetFilePath(a).c_str());
  EXPECT_STREQ("", i.GetFileName(a).c_str());
  char const *b[2] = {"asdf", ".."};
  EXPECT_STREQ("/home/gakgu/Desktop/GakView/test/..", i.GetFilePath(b).c_str());
  EXPECT_STREQ("", i.GetFileName(b).c_str());
  char const *c[2] = {"asdf", "../ss"};
  EXPECT_STREQ("/home/gakgu/Desktop/GakView/test/..", i.GetFilePath(c).c_str());
  EXPECT_STREQ("ss", i.GetFileName(c).c_str());
  char const *d[2] = {"asdf", "aa"};
  EXPECT_STREQ("/home/gakgu/Desktop/GakView/test", i.GetFilePath(d).c_str());
  EXPECT_STREQ("aa", i.GetFileName(d).c_str());
  char const *e[2] = {"asdf", "/."};
  EXPECT_STREQ("/home/gakgu/Desktop/GakView/test", i.GetFilePath(d).c_str());
  EXPECT_STREQ("", i.GetFileName(e).c_str());
  char const *f[2] = {"asdf", "."};
  EXPECT_STREQ("/home/gakgu/Desktop/GakView/test", i.GetFilePath(d).c_str());
  EXPECT_STREQ("", i.GetFileName(f).c_str());
  char const *g[2] = {"asdf", "./"};
  EXPECT_STREQ("/home/gakgu/Desktop/GakView/test", i.GetFilePath(d).c_str());
  EXPECT_STREQ("", i.GetFileName(g).c_str());
  char const *h[2] = {"asdf", "./aa"};
  EXPECT_STREQ("/home/gakgu/Desktop/GakView/test", i.GetFilePath(d).c_str());
  EXPECT_STREQ("aa", i.GetFileName(h).c_str());
}
