#include "Timer.h"
#include "Pugixml/pugixml.hpp"
#include "FL/Fl_Text_Buffer.H"
#include "FL/Fl_Text_Display.H"
#include <string>

#pragma once
class ClientSocket;
class Lobby;

//This is a clientsocket wrapper
class Client : public Timer
{
private:
	ClientSocket* m_socket;
	Lobby* m_lobby;
	Fl_Text_Buffer* m_mainWindowLog;
	bool m_loaded;

	/**
		Routinely checks if there are new events from the server, then applies them to the Lobby.
		After that, it checks if there are any new events from the Lobby, and sends them off to the server
	*/
	void OnTick();
public:
	Client();
	~Client();

	/**
		Asks the ClientSocket property to connect to given ip. Returns true on successful connection
	*/
	bool Connect(std::string& _ipToConnect);


	/**
		Converts a given XML node to a string then ships off.
	*/
	void Send(pugi::xml_document& _nodeToSend);

	/**
		Sets value of m_mainWindowLog. This isn't done in the constructor m_mainWindowLog only starts to exist (It's created in MainWindow::ChangeLayout()) after the client has been initalised.
	*/
	void SetLogDisplay(Fl_Text_Display* _outputLog);

	Lobby* GetLobby();
};

