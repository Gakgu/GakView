#include "GakView/application.hpp"
#include "GakView/image.hpp"
#include "GakView/window.hpp"

gakview::Application::Application(int argc, char **argv)
  : m_window(NULL), m_image(NULL), m_argc(argc), m_argv(argv)
{
  // TODO : Use shared_ptr or unique_ptr
  m_window = new Window;
  m_image = new Image;
  SDL_Delay(2000);
}

gakview::Application::~Application()
{
  delete m_image;
  delete m_window;
}

void gakview::Application::Run()
{
  m_image->Init(m_argv);
  m_window->Create();
  m_window->Update(*m_image);

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
            m_image->Previous();
            m_window->Update(*m_image);
            break;
          case SDLK_RIGHT:
            m_image->Next();
            m_window->Update(*m_image);
            break;
        }
      }
    }
  }
}
