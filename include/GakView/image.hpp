#ifndef GAKVIEW_IMAGE_H_
#define GAKVIEW_IMAGE_H_

#include <string>
#include <vector>
#include <SDL2/SDL.h>

typedef std::string File;
typedef std::vector<std::string> FileList;

namespace gakview
{
class Image
{
  public:
    Image() {}
    // method
    bool Init(char **argv);
    SDL_Surface* GetSurface() const {return info.image_surface;}
    void Next();
    void Previous();
    // variable
    struct image_info
    {
      SDL_Surface* image_surface = NULL;
      int index = 0;
      FileList files;// current file is file[index]
    } info;
    // operator
    const image_info* operator->() const { return &info; }
  private:
    bool IndexInit(char **argv);

    // Disallow copy constructor and assignment operator.
    Image(const Image&);
    void operator=(const Image&);
};
} // namespace gakview
#endif //GAKVIEW_IMAGE_H_
