#include "Pugixml/pugixml.hpp"

#include <winsock2.h>
#include <WS2tcpip.h>
#include <memory>
#include <vector>
#include <string>

#pragma once

class ClientSocket;
class Server;
class ServerSocket
{
	friend class Server;
private:
	std::vector<std::shared_ptr<ClientSocket>> m_clients;
	SOCKET m_socket;
	std::string m_ipAddress;
	int m_idJustAdded;

	/**
		Checks for any new connections, and returns a smart pointer to a new connection if there is one
	*/
	std::shared_ptr<ClientSocket> Accept();

public:
	ServerSocket(int _port);
	~ServerSocket();

	/**
		Probes all clients for any event messages, storing any inside of an XML document, which is returned.
		Called in Server::MonitorNetwork() once per loop
	*/
	pugi::xml_document Update();

	/**
		Converts given xml document into a string which is then shipped off to all clients
	*/
	void Send(pugi::xml_document& _xmlToSend);

	/**
		Sends server XML from Server to a client, then blacklists them from recieving new events until the server recieves a confirmation
	*/
	void SendServerInfo(int _index, std::string _xmlToSend);

	void SetNewPlayerID(int _id);
	void RemoveConnection(int _id);
};
