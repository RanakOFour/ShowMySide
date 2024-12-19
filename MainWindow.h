#include "FileTextDisplay.h"
#include "MenuWrapper.h"
#include "Timer.h"

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
	ABOUT = 4
};

#pragma once

class Server;
class Client;

class MainWindow : public Timer, public Fl_Double_Window
{
private:
	///@{
	/** m_Server and m_Client are not initialised in the constructor, so they are pointers */
	std::unique_ptr<Server> m_Server;
	std::unique_ptr<Client> m_Client;
	/// @}
	FileTextDisplay m_aboutText;
	MenuWrapper m_menuWrapper;

	Fl_Box m_splashImage;
	Fl_Input m_ipInput;
	Fl_Output m_ipAddressBox;
	Fl_Button m_connectBtn;
	Fl_Text_Display m_lobbyEventLogDisplay;

	void OnTick();
public:
	MainWindow(int _w, int _h, std::string _name);
	~MainWindow();

	/**
		Takes a LayoutType enum, then deletes all children of the window except the menubar at the top and after that instantated and adds all the required FLTK objects
		needed for the layout
	*/
	void ChangeLayout(LayoutType _newState);


	
	///@{ @name FLTK Events
	/**
		Used mainly by the MenuWrapper and buttons 
	*/
	static void OnJoinServer(Fl_Widget* _widget, void* _userData);
	static void OnClientStart(Fl_Widget* _widget, void* _userData);
	static void OnServerStart(Fl_Widget* _widget, void* _userData);
	static void OnExit(Fl_Widget* _widget, void* _userData);
	static void ShowAbout(Fl_Widget* _widget, void* _userData);
	///@}
};
