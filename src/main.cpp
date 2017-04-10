#include "GakView/application.hpp"

// TODO use argc and add option (ex : -f, -bg)
int main(int argc, char **argv)
{
  gakview::Application app(argc, argv);
  app.Run();

  return 0;
}
