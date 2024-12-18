#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Input.H"
#include "FL/Fl_Output.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Text_Display.H"

#include <string>
#include <memory>

/**
		There are only 3 different layouts, in reality. The empty screen when the program boots, the join game screen, then the normal gameplay screen.
		The only reason Server is listed is because the Server needs to also add an Fl_Label with the ip address of the machine so that other clients can
		join.
	*/
enum class LayoutType
{
	SPLASH_SCREEN = 0,
	JOIN_GAME = 1,
	SERVER = 2,
	IN_GAME = 3,
	ABOUT = 4,
	HELP = 5
};

class Server;
class Client;
class MenuWrapper;
class FileTextDisplay;

class MainWindow : public Fl_Double_Window
{
private:
	std::shared_ptr<Server> m_Server;
	std::shared_ptr<Client> m_Client;
	std::shared_ptr<FileTextDisplay> m_aboutText;
	std::shared_ptr<MenuWrapper> m_menuWrapper;

	std::shared_ptr<Fl_Box> m_splashImage;
	std::shared_ptr<Fl_Input> m_ipInput;
	std::shared_ptr<Fl_Output> m_ipAddressBox;
	std::shared_ptr<Fl_Button> m_connectBtn;
	std::shared_ptr<Fl_Text_Display> m_lobbyEventLogDisplay;
	std::shared_ptr<Fl_Text_Display> m_serverInfoDisplay;

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
};
