#include "gakview_window.h"

Window::~Window()
{
  if(surface_)
    SDL_FreeSurface(surface_);
  if(window_)
    SDL_DestroyWindow(window_);
  SDL_Quit();
}

bool Window::CreateWindow(const std::string &title)
{
  SDL_Init(SDL_INIT_VIDEO);
  window_ = SDL_CreateWindow(
      title.c_str(),
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      kWidth_,
      kHeight_,
      SDL_WINDOW_SHOWN);
  surface_ = SDL_GetWindowSurface(window_);

  // Clear screen
  SDL_FillRect(surface_, NULL, SDL_MapRGB(surface_->format, 0x00, 0x00, 0x00));
  SDL_UpdateWindowSurface(window_);

  return true;
}
