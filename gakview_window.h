#ifndef GAKVIEW_WINDOW_H_
#define GAKVIEW_WINDOW_H_

#include <string>

#include <SDL2/SDL.h>

class Window
{
  public:
    Window() : window_(NULL), surface_(NULL) {}
    ~Window();
    bool CreateWindow(const std::string &title);
    void Render();
    SDL_Window *window_;
    SDL_Surface *surface_;
  private:
    const int kWidth_ = 640;
    const int kHeight_ = 480;
    // Disallow copy constructor and assignment operator.
    Window(const Window&);
    void operator=(const Window&);
};

#endif
