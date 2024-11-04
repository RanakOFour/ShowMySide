#include "Timer.h"
#include <string>
#include <FL/Fl_Window.H>

class ServerSocket;
class ClientSocket;
struct MainWindow : public Fl_Window, public Timer
{
private:
	ServerSocket* m_Server;
	ClientSocket* m_Client;
	std::string m_serverConnectInfo;
	bool m_isServer;
	void on_tick(void* _userData);
public:
	MainWindow(int x, int y, int w, int h, std::string name, bool hasServer);
	~MainWindow();
};

