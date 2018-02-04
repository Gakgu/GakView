#include <GakView/application.hpp>
#include <gtest/gtest.h>
#include <iostream>

TEST(Application, Init)
{
  char const *a[2] = {"asdf", "../gakview"};
  char const *b[2] = {"asdf", "../"};
  char const *c[2] = {"asdf", "../gakview"};
  char const *d[2] = {"asdf", "/"};
  gakview::Application app(a);

  EXPECT_EQ(0, app.Init(a)) << "a";
  EXPECT_EQ(0, app.Init(c)) << "c";
  EXPECT_EQ(0, app.Init(b)) << "b";
  EXPECT_EQ(0, app.Init(d)) << "d";
}
