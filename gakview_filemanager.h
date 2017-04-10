#ifndef GAKVIEW_FILEMANAGER_H_
#define GAKVIEW_FILEMANAGER_H_

#include <string>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

typedef std::vector<std::string> FileList;
typedef std::string File;

class FileManager
{
  public:
    FileManager(char **argv) : index_(-1), files_(), argv_(argv) {}
    bool Init() {InitFiles(); InitIndex(); return true;} // TODO return
    void Next();
    void Previous();
    int index_;
    FileList files_;
  private:
    bool InitFiles();
    bool InitIndex();
    char **argv_;
    // Disallow copy constructor and assignment operator.
    FileManager(const FileManager&);
    void operator=(const FileManager&);
};
#endif
