#include <winsock2.h>
#include <string>

class Client;
class ServerSocket;

/**
	Container for one end of a peer-to-peer connection. For each ClientSocket on a Client, it connects to another in the Host and that is the basis on which the networking works.
	ServerSocket contains a list of all ClientSocket connections.
*/
class ClientSocket
{
private:
	friend class Client;
	friend class ServerSocket;
	SOCKET m_socket;
	bool m_closed;

public:
	ClientSocket();
	~ClientSocket();

	/**
		Connects to a specified ip address (_serverName). Returns true on a successful connection
	*/
	bool Connect(std::string& _serverName);

	/**
		Ships off _message to the other end of the connection
	*/
	void Send(std::string& _message);

	/*
		Probes other end of the connection for any messages. Returns true if any information is recieved, which is loaded as a string inside of _message which is then processed by Client or ServerSocket
	*/
	bool Receive(std::string& _message);
};
