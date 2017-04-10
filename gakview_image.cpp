#include "gakview_image.h"

void Image::SetImage(Window &window, const std::string &file)
{
  // TODO
  // Should add SDL_Image();
  SDL_Surface* image_surface_ = SDL_LoadBMP(file.c_str());
  if(!SDL_BlitSurface(image_surface_, NULL, window.surface_, NULL))
    SDL_UpdateWindowSurface(window.window_);
  // TestCode //
#include <stdio.h>
  printf("current file : %s\n", file.c_str());
}
