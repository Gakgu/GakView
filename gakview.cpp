#include "gakview.h"

void GakView::Run()
{
  window_.CreateWindow("title");
  file_manager_.Init();
  image_.SetImage(window_, file_manager_.files_[file_manager_.index_]);

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
            file_manager_.Previous();
            image_.SetImage(window_, file_manager_.files_[file_manager_.index_]);
              while(*SDL_GetKeyboardState(NULL) == 1)
                SDL_Delay(100);
            break;
          case SDLK_RIGHT:
              file_manager_.Next();
              image_.SetImage(window_, file_manager_.files_[file_manager_.index_]);
              while(*SDL_GetKeyboardState(NULL) == 1)
                SDL_Delay(100);
            break;
        }
      }
    }
  }
}
