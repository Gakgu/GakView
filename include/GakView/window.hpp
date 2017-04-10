#ifndef GAKVIEW_WINDOW_H_
#define GAKVIEW_WINDOW_H_

#include <SDL2/SDL.h>

namespace gakview
{
class Image;
class Window
{
  public:
    Window() : m_window(NULL), m_window_surface(NULL), m_image_surface(NULL) {}
    ~Window();
    bool Create(Image &image);
    void Update(Image &image);
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
