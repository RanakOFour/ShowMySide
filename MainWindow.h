#include "Timer.h"
#include <string>
#include <FL/Fl_Window.H>

class ServerSocket;
class ClientSocket;
class Host;
class Client;
class MainWindow : public Fl_Window
{
private:
	Host* m_Host;
	Client* m_Client;
	int m_CurrentLayout;
	bool AttemptConnection(const char* _ipAddress);

public:
	MainWindow(int _w, int _h, std::string _name);
	~MainWindow();

	// More understandable this way
	enum LayoutType
	{
		SPLASH_SCREEN = 0,
		DUMMY_HOST = 1,
		JOIN_GAME = 2,
		HOST = 3,
		IN_GAME = 4
	};
	void ChangeLayout(LayoutType _newState);

	void Send(const std::string _msg);

	//Button events
	static void OnSendData(Fl_Widget* _widget, void* _userData);
	static void OnJoinServer(Fl_Widget* _widget, void* _userData);
	static void OnClientStart(Fl_Widget* _widget, void* _userData);
	static void OnServerStart(Fl_Widget* _widget, void* _userData);
	static void OnExit(Fl_Widget* _widget, void* _userData);
};

