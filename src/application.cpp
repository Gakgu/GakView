#include "GakView/application.hpp"

namespace gakview
{
Application::Application(char const **argv) : m_succes_init(-1)
{
  Init(argv);
}

int Application::Init(char const **argv)
{
  if(m_image.SetFileList(argv) == 0)
    if(m_window.Create() == 0)
      m_succes_init = 0;

  return m_succes_init;
}

int Application::Run()
{
  if(m_succes_init != 0)
    return 0;

  int succes = 0;
  bool quit = false;
  SDL_Event event;
  while(!quit)
  {
    while(SDL_PollEvent(&event) != 0)
    {
      if(event.type == SDL_QUIT)
        quit = true;
      else if(event.type == SDL_KEYDOWN && event.key.repeat == 0)
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_UP:
            quit = true;
            break;
          case SDLK_LEFT:
            if(m_image.Previous())
              if(!m_window.Update(m_image.m_fileList.at(m_image.m_index)))
              {
                succes = -1;
                quit = true;
              }
            break;
          case SDLK_RIGHT:
            if(m_image.Next())
              if(m_window.Update(m_image.m_fileList.at(m_image.m_index)))
              {
                succes = -1;
                quit = true;
              }
            break;
        }
      }
    }
  }
  return succes;
}
}
