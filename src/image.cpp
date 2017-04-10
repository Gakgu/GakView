#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include "GakView/image.hpp"

// TODO : Fix this ugly code
bool gakview::Image::Init(char **argv)
{
  using boost::filesystem::path;
  using boost::filesystem::directory_entry;
  using boost::filesystem::directory_iterator;
  using boost::filesystem::initial_path;
  // TODO
  // Add case of argv_[1] == NULL
  path files_path(argv[1]);
  path dir_path(initial_path());

  for(directory_entry &x : directory_iterator(dir_path))
    // TODO
    // This code check file extension.
    // It's operated but ugly.
  {
    File file = x.path().filename().string();
    file = file.substr(file.find_last_of(".") + 1);
    if( file == "bmp" || file == "jpg" || file == "png")
      info.files.push_back(x.path().filename().string());
  }

  sort(info.files.begin(), info.files.end());
  IndexInit(argv);

  return true;
}

bool gakview::Image::IndexInit(char **argv)
{
  // Get index
  // TODO : Add case of index_ == -1
  for(unsigned int index = 0; index < info.files.size(); index++)
    if(info.files[index] == argv[1])
    {
      info.index = index;
      break;
    }

  return true;
}

void gakview::Image::Next()
{
  // TODO : use C++ caster
  if(info.index < (signed)info.files.size() - 1)
    info.index++;
}

void gakview::Image::Previous()
{
  if(info.index > 0)
    info.index--;
}
