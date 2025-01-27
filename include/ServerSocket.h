#ifdef _WIN32
	#include <winsock2.h>
	#include <ws2tcpip.h.h>
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>
#endif

include "Pugixml/pugixml.hpp"
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

	/**
		Checks for any new connections, and returns a smart pointer to a new connection if there is one
	*/
	std::shared_ptr<ClientSocket> Accept();

public:

	/**
		Used in Server::Server() constructor
	*/
	ServerSocket(int _port);
	~ServerSocket();

	/**
		Probes all clients for any event messages, storing any inside of an XML document, which is returned.
		Called in Server::MonitorNetwork() once per loop
	*/
	pugi::xml_document Update();

	/**
		Converts given xml document into a string which is then encrypted
		and shipped off to all clients
	*/
	void Send(pugi::xml_document& _xmlToSend);

	/**
		Sends server XML from Server to a single client
	*/
	void SendTo(int _index, std::string _xmlToSend);

	void SetNewPlayerID(int _id);
	void RemoveConnection(int _id);
	void Close();
};
