#include "gakview.h"

// TODO use argc and add option (ex : -f, -bg)
int main(int argc, char **argv)
{
  GakView app(argv);
  app.Run();

  return 0;
}
