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

bool Image::IsImage(path const &file)
{
  bool is_image = true;
  string file_extension = file.extension().string();

  std::transform(file_extension.begin(), file_extension.end(), 
      file_extension.begin(), ::tolower);

  if(file_extension != string(".tag"))
    if(file_extension != string(".bmp"))
      if(file_extension != string(".pnm"))
        if(file_extension != string(".pbm"))
          if(file_extension != string(".ppm"))
            if(file_extension != string(".xpm"))
              if(file_extension != string(".xcf"))
                if(file_extension != string(".pcx"))
                  if(file_extension != string(".gif"))
                    if(file_extension != string(".jpg"))
                      if(file_extension != string(".jpeg"))
                        if(file_extension != string(".tif"))
                          if(file_extension != string(".tiff"))
                            if(file_extension != string(".lbm"))
                              if(file_extension != string(".iff"))
                                if(file_extension != string(".png"))
                                  is_image = false;

  return is_image;
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
      if(IsImage(x.path()) == true)
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

  if(m_index < static_cast<int>(m_fileList.size()) - 1 && m_index != -1)
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
  else
    succes = -1;

  return succes;
}
}
