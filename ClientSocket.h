#include <winsock2.h>
#include <string>

class Client;
class ServerSocket;
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

	bool Connect(const std::string& _serverName);
	void Send(const std::string& _message);
	bool Receive(std::string& _message);
};
