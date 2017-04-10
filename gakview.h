#ifndef GAKVIEW_H_
#define GAKVIEW_H_

#include <string>

#include "gakview_image.h"
#include "gakview_window.h"
#include "gakview_filemanager.h"

class GakView
{
  public:
    GakView(char **argv) : window_(), file_manager_(argv), image_() {}
    void Run();
  private:
    Window window_;
    FileManager file_manager_;
    Image image_;
    // Disallow copy constructor and assignment operator.
    GakView(const GakView&);
    void operator=(const GakView&);
};

#endif
