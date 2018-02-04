#ifndef GAKVIEW_IMAGE_H_
#define GAKVIEW_IMAGE_H_

#include <string>
#include <vector>
#include <SDL2/SDL.h>

namespace gakview
{
class Image
{
  public:
    Image() : m_fileList(), m_index(-1) {}
    int Next();
    int Previous();
    std::string GetFilePath(char const **argv);
    std::string GetFileName(char const **argv);
    std::string GetImagePath(){ return std::string(); }
    int SetFileList(char const **argv);
    int SetIndex(char const **argv);
    std::vector<std::string> m_fileList;
    int m_index;
  private:
    // Disallow copy constructor and assignment operator.
    Image(const Image&);
    void operator=(const Image&);
};
} // namespace gakview
#endif //GAKVIEW_IMAGE_H_
