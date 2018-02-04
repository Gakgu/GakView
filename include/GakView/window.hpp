#ifndef GAKVIEW_WINDOW_H_
#define GAKVIEW_WINDOW_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

namespace gakview
{

  class Window
  {
    public:
      Window() : m_window(NULL), m_window_surface(NULL), m_image_surface(NULL) 
    {}
      ~Window();
      int Create();
      int Update(std::string &image_path);
    private:
      SDL_Window *m_window;
      SDL_Surface *m_window_surface;
      SDL_Surface *m_image_surface;

      // Disallow copy constructor and assignment operator.
      Window(const Window&);
      void operator=(const Window&);
  };
} // namespace gakview
#endif // GAKVIEW_WINDOW_H_
