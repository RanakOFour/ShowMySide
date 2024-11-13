#include "Timer.h"
#include <string>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>

class ServerSocket;
class ClientSocket;
struct MainWindow : public Fl_Window, public Timer
{
private:
	ServerSocket* m_Server;
	ClientSocket* m_Client;
	Fl_Input* m_ipAddr;
	std::string m_serverConnectInfo;
	int m_state;
	void changeInterface(int _state);
	void on_tick(void* _userData);
	static void onServer(Fl_Widget* _widget, void* _userData);
	static void onClient(Fl_Widget* _widget, void* _userData);
	static void onConnect(Fl_Widget* _widget, void* _userData);
	static void onExit(Fl_Widget* _widget, void* _userData);
public:
	MainWindow(int _w, int _h, std::string _name);
	~MainWindow();
};

