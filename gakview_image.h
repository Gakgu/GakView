#ifndef GAKVIEW_IMAGE_H_
#define GAKVIEW_IMAGE_H_

#include <string>

#include <SDL2/SDL.h>
#include "gakview_window.h"

class Image
{
  public:
    Image() : image_surface_(NULL){}
    void SetImage(Window &window, const std::string &file);
  private:
    SDL_Surface* image_surface_;
    // Disallow copy constructor and assignment operator.
    Image(const Image&);
    void operator=(const Image&);
};

#endif
