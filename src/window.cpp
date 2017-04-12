#include "GakView/window.hpp"
#include "GakView/image.hpp"

gakview::Window::~Window()
{
  if(m_window_surface)
    SDL_FreeSurface(m_window_surface);
  if(m_window)
    SDL_DestroyWindow(m_window);
  SDL_Quit();
}

bool gakview::Window::Create()
{
  SDL_Init(SDL_INIT_VIDEO);
  m_window = SDL_CreateWindow(
      "GakView",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      640,
      480,
      SDL_WINDOW_SHOWN);
  m_window_surface = SDL_GetWindowSurface(m_window);

  // Clear screen
  SDL_UpdateWindowSurface(m_window);

  return true;
}

void gakview::Window::Update(gakview::Image &image)
{
  // Clear screen
  SDL_FillRect(m_window_surface, NULL,
      SDL_MapRGB(m_window_surface->format, 0x00, 0x00, 0x00));

  // Display image on window
  File file = image->files[image->index];
  m_image_surface = SDL_LoadBMP(file.c_str());
  if(!SDL_BlitSurface(m_image_surface, NULL, m_window_surface, NULL))
    SDL_UpdateWindowSurface(m_window);
}
