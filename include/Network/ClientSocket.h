#include <string>

#if _WIN32
	#include <winsock2.h>
#else
	#include <sys/socket.h>
	#define SOCKET int
#endif

#pragma once
class Client;
class ServerSocket;

/**
	Container for one end of a peer-to-peer connection. For each ClientSocket on a Client, it connects to another in the Server and that is the basis on which the networking works.
	ServerSocket contains a list of all ClientSocket connections.
*/
class ClientSocket
{
private:
	friend class Client;
	friend class ServerSocket;
	SOCKET m_socket;
	bool m_closed;
	int m_id;

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
	
	/**
		Puts any recieved information into _message
	*/
	void Receive(std::string& _message);

	void CloseConnection();

	void SetID(int _id);
	int GetID();
	bool Closed();
};
