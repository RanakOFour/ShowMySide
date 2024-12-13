#include "Pugixml/pugixml.hpp"
#include <winsock2.h>
#include <WS2tcpip.h>
#include <memory>
#include <vector>
#include <string>

class ClientSocket;
class Host;
class ServerSocket
{
	friend class Host;
private:
	std::vector<std::shared_ptr<ClientSocket>> m_clients;
	SOCKET m_socket;
	std::string m_ipAddress;
	int m_idJustAdded;

	/**
		Probes all clients for any event messages, storing any inside of an XML document, which is returned
	*/
	pugi::xml_document Update();

	/**
		Checks for any new connections, and returns a smart pointer to a new connection if there is one
	*/
	std::shared_ptr<ClientSocket> Accept();

	/**
		Converts given xml document into a string which is then shipped off to all clients
	*/
	void Send(pugi::xml_document& _xmlToSend);

	/**
		Sends server XML from Host to a client, then blacklists them from recieving new events until the server recieves a confirmation
	*/
	void SendServerInfo(std::string _xmlToSend);

public:
	ServerSocket(int _port);
	~ServerSocket();
};
