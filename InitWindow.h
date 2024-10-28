#include <vector>
#include <memory>
#include <string>
#include <FL/Fl_Window.H>

class InitWindow : public Fl_Window
{
private:
	static void StartClient(Fl_Widget* widget, void* data);
	static void StartServer(Fl_Widget* widget, void* data);
public:
	InitWindow(int w, int h, std::string name);
	~InitWindow();
};

