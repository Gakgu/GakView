#include "gakview_filemanager.h"

bool FileManager::InitFiles()
{
  using namespace boost::filesystem;

  // TODO
  // Add case of argv_[1] == NULL
  path files_path(argv_[1]);
  path dir_path(initial_path());

  for(directory_entry &x : directory_iterator(dir_path))
      // TODO
      // This code check file extension.
      // It's operated but ugly.
    {
      File file = x.path().filename().string();
      file = file.substr(file.find_last_of(".") + 1);
      if( file == "bmp" || file == "jpg" || file == "png")
        files_.push_back(x.path().filename().string());
    }

  sort(files_.begin(), files_.end());

  return true;
}

bool FileManager::InitIndex()
{
  // Get index
  // TODO : Add case of index_ == -1
  for(unsigned int index = 0; index < files_.size(); index++)
    if(files_[index] == argv_[1])
    {
      index_ = index;
      break;
    }

  return true;
}

void FileManager::Next()
{
  // TODO : use C++ caster
  if(index_ < (signed)files_.size() - 1)
    index_++;
}

void FileManager::Previous()
{
  if(index_ > 0)
    index_--;
}
