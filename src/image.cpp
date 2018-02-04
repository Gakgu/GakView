#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <GakView/image.hpp>
#include <algorithm>

using boost::filesystem::path;
using boost::filesystem::initial_path;
using std::string;

namespace gakview
{
// return Succes : file path, fail : path()
string Image::GetFilePath(char const **argv)
{
  path file_path = initial_path() / path(argv[1]);
  if(exists(file_path) == true)
  {
    if(is_directory(file_path) == true)
    {
      file_path = file_path;
    }
    else
      file_path = file_path.parent_path();
  }
  else if(exists(file_path.parent_path()) == true)
    file_path = file_path.parent_path();
  else
    file_path = path();

  return file_path.string();
}

string Image::GetFileName(char const **argv)
{
  string file_name;
  
  if(is_directory(initial_path() / path(argv[1])) == true)
    file_name = path().string();
  else
    file_name = path(argv[1]).filename().string();

  return file_name;
}

int Image::SetFileList(char const **argv)
{
  using boost::filesystem::directory_entry;
  using boost::filesystem::directory_iterator;

  int succes = 0;
  if(GetFilePath(argv) == string())
    succes = -1;
  else
  {

    for(directory_entry &x :
        directory_iterator(GetFilePath(argv)))
    {
      string file_extension = x.path().extension().string();
      std::transform(file_extension.begin(), file_extension.end(), 
          file_extension.begin(), ::tolower);

      if(file_extension == string(".tag") ||
          file_extension == string(".bmp") ||
          file_extension == string(".pnm") ||
          file_extension == string(".pbm") ||
          file_extension == string(".ppm") ||
          file_extension == string(".xpm") ||
          file_extension == string(".xcf") ||
          file_extension == string(".pcx") ||
          file_extension == string(".gif") ||
          file_extension == string(".jpg") ||
          file_extension == string(".jpeg") ||
          file_extension == string(".tif") ||
          file_extension == string(".tiff") ||
          file_extension == string(".lbm") ||
          file_extension == string(".iff") ||
          file_extension == string(".png"))
        m_fileList.push_back(x.path().string());
    }

    sort(m_fileList.begin(), m_fileList.end());
  }

  return succes;
}

int Image::SetIndex(char const **argv)
{
  int succes = 0;

  if(m_fileList.size() == 0 || GetFileName(argv) == string())
    succes = -1;
  else
  {
    for(int i = 0; i < static_cast<int>(m_fileList.size()); i++)
      if(path(m_fileList.at(i)).filename() == GetFileName(argv))
      {
        m_index = i;
        break;
      }

    if(m_index == -1)
      succes = -1;
  }

  return succes;
}

int Image::Next()
{
  int succes = 0;

  if(m_index < static_cast<int>(m_fileList.size()) - 1)
    m_index++;
  else if(m_index == -1)
    m_index++;
  else
    succes = -1;

  return succes;
}

int Image::Previous()
{
  int succes = 0;

  if(m_index > 0)
    m_index--;
  else if(m_index == -1)
    m_index++;
  else
    succes = -1;

  return succes;
}
}
