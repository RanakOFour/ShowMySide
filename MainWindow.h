#include <string>
#include <FL/Fl_Double_Window.H>

class ServerSocket;
class ClientSocket;
class Host;
class Client;
class MainWindow : public Fl_Double_Window
{
private:
	Host* m_Host;
	Client* m_Client;

	/**
		Called in OnJoinServer() to take the IPv4 address from an Fl_Input widget and pass it to the Client to connect. Returns true on a succesful connection
	*/
	bool AttemptConnection(const char* _ipAddress);


	static void OnSendData(Fl_Widget* _widget, void* _userData);
	static void OnJoinServer(Fl_Widget* _widget, void* _userData);
	static void OnClientStart(Fl_Widget* _widget, void* _userData);
	static void OnServerStart(Fl_Widget* _widget, void* _userData);
	static void OnExit(Fl_Widget* _widget, void* _userData);

public:
	MainWindow(int _w, int _h, std::string _name);
	~MainWindow();

	/**
		There are only 3 different layouts, to be real. The empty screen when the program boots, the join game screen, then the normal gameplay screen.
		The only reason HOST is listed is because the host needs to also add an Fl_Label with the ip address of the machine so that other clients can
		join.
	*/
	enum LayoutType
	{
		SPLASH_SCREEN = 0,
		JOIN_GAME = 1,
		HOST = 2,
		IN_GAME = 3
	};

	/**
		Takes a LayoutType enum, then deletes all children of the window except the menubar at the top and after that instantated and adds all the required FLTK objects
		needed for the layout
	*/
	void ChangeLayout(LayoutType _newState);

	/**
		Called by OnSendData to ask the client to send a message stored in an Fl_Multiline_Input
	*/
	void Send(const std::string _msg);
};

