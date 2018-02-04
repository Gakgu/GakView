#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <GakView/window.hpp>
#include <GakView/image.hpp>

namespace gakview
{
  class Application
  {
    public:
      Application(char const **argv);
      int Init(char const **argv);
      int Run();
    private:
      Window m_window;
      Image m_image;
      int m_succes_init;

      // Disallow copy constructor and assignment operator.
      Application(const Application&);
      void operator=(const Application&);
  };
} // namespace gakview
#endif
