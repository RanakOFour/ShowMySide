#include "Timer.h"
#include <string>
#include <FL/Fl_Window.H>

class ServerSocket;
class ClientSocket;
struct MainWindow : Fl_Window, Timer
{
private:
	ServerSocket* m_Server;
	ClientSocket* m_Client;
	bool m_isServer;
public:
	MainWindow(int x, int y, int w, int h, std::string name, bool hasServer);
	~MainWindow();
	void on_tick(void* _userData);
};

