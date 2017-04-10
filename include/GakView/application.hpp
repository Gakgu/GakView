#ifndef APPLICATION_H_
#define APPLICATION_H_
namespace gakview
{
class Window;
class Image;
class Application
{
  public:
    Application(int argc, char **argv);
    ~Application();
    void Run();
  private:
    gakview::Window* m_window;
    gakview::Image* m_image;
    int m_argc;
    char **m_argv;

    // Disallow copy constructor and assignment operator.
    Application(const Application&);
    void operator=(const Application&);
};
} // namespace gakview
#endif
