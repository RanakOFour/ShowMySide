#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Input.H"
#include "FL/Fl_Output.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Text_Display.H"
#include "FL/Fl_Box.H"
#include <string>

/**
		There are only 3 different layouts, in reality. The empty screen when the program boots, the join game screen, then the normal gameplay screen.
		The only reason HOST is listed is because the host needs to also add an Fl_Label with the ip address of the machine so that other clients can
		join.
	*/
enum LayoutType
{
	SPLASH_SCREEN = 0,
	JOIN_GAME = 1,
	HOST = 2,
	IN_GAME = 3,
	ABOUT = 4,
	HELP = 5
};


class ServerSocket;
class ClientSocket;
class Host;
class Client;
class MenuWrapper;
class MainWindow : public Fl_Double_Window
{
private:
	Host* m_Host;
	Client* m_Client;
	MenuWrapper* m_menuWrapper;
	Fl_Box* m_splashImage;
	Fl_Input* m_ipInput;
	Fl_Output* m_ipAddressBox;
	Fl_Button* m_connectBtn;
	Fl_Text_Display* m_lobbyEventLog;

	/**
		Called in OnJoinServer() to take the IPv4 address from an Fl_Input widget and pass it to the Client to connect. Returns true on a succesful connection
	*/
	bool AttemptConnection(std::string& _ipAddress);

public:
	MainWindow(int _w, int _h, std::string _name);
	~MainWindow();

	/**
		Takes a LayoutType enum, then deletes all children of the window except the menubar at the top and after that instantated and adds all the required FLTK objects
		needed for the layout
	*/
	void ChangeLayout(LayoutType _newState);

	static void OnJoinServer(Fl_Widget* _widget, void* _userData);
	static void OnClientStart(Fl_Widget* _widget, void* _userData);
	static void OnServerStart(Fl_Widget* _widget, void* _userData);
	static void OnExit(Fl_Widget* _widget, void* _userData);
	static void ShowAbout(Fl_Widget* _widget, void* _userData);
	static void ShowHelp(Fl_Widget* _widget, void* _userData);
};
