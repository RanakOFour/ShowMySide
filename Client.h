#include "Timer.h"
#include "Pugixml/pugixml.hpp"
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

	/**
		Routinely checks if there are new events from the server, then applies them to the Lobby.
		After that, it checks if there are any new events from the Lobby, and sends them off to the server
	*/
	void OnTick(void* _userData);
public:
	Client();
	~Client();

	/**
		Asks the ClientSocket property to connect to given ip. Returns true on successful connection
	*/
	bool Connect(const char* _ipToConnect);

	/**
		Packages an event string into and XML node, then converts that to a string before shipping off
	*/
	void Send(std::string& _message);

	/**
		Converts a given XML node to a string then ships off.
	*/
	void Send(pugi::xml_document& _nodeToSend);
	Lobby* GetLobby();
};

