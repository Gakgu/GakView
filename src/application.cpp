#include "GakView/application.hpp"

namespace gakview
{
Application::Application(char const **argv) : m_succes_init(-1)
{
  Init(argv);
}

int Application::Init(char const **argv)
{
  int succes = 0;
  if(m_image.SetFileList(argv) != 0)
    succes = -1;
  m_image.SetIndex(argv);

  return succes;
}

int Application::Run()
{
  int succes = 0;
  if(m_window.Create() != 0)
    succes = -1;
  else
  {
    m_window.Update(m_image.m_fileList.at(m_image.m_index));

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
              break;
            case SDLK_LEFT:
              if(m_image.Previous() == 0)
                if(m_window.Update(m_image.m_fileList.at(m_image.m_index)) == -1)
                {
                  succes = -1;
                  quit = true;
                }
              break;
            case SDLK_RIGHT:
              if(m_image.Next() == 0)
                if(m_window.Update(m_image.m_fileList.at(m_image.m_index)) == -1)
                {
                  succes = -1;
                  quit = true;
                }
              break;
          }
        }
      }
    }
  }
  return succes;
}
}
