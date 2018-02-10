#include "GakView/window.hpp"

namespace gakview
{
Window::~Window()
{
  if(m_window_surface)
    SDL_FreeSurface(m_window_surface);
  if(m_window)
    SDL_DestroyWindow(m_window);
  SDL_Quit();
}

int Window::Create()
{
  int succes = 0;

  if(SDL_Init(SDL_INIT_VIDEO) != 0)
    succes = -1;

  m_window = SDL_CreateWindow(
      "GakView",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      640,
      480,
      SDL_WINDOW_SHOWN);
  if(m_window == NULL)
    succes = -1;

  m_window_surface = SDL_GetWindowSurface(m_window);
  if(m_window_surface == NULL)
    succes = -1;

  // Clear screen
  SDL_UpdateWindowSurface(m_window);

  return succes;;
}

int Window::Update(std::string &image_path)
{
  int succes = 0;

  // Clear screen
  if(SDL_FillRect(m_window_surface, NULL,
        SDL_MapRGB(m_window_surface->format, 0x00, 0x00, 0x00)) != 0)
    succes = -1;

  // Draw screen
  if(image_path != std::string())
  {
    m_image_surface = IMG_Load(image_path.c_str());
    if(m_image_surface == NULL)
      succes = -1;
    //m_image_surface.width();

    if(!SDL_BlitSurface(m_image_surface, NULL, m_window_surface, NULL))
      SDL_UpdateWindowSurface(m_window);
  }

  return succes;
}
}
