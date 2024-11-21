#include <winsock2.h>
#include <string>

class ServerSocket;
class ClientSocket
{
private:
	friend class ServerSocket;
	SOCKET m_socket;

public:
	bool m_closed;
	bool m_Connected;
	bool Connect(const std::string& _serverName);
	void Send(const std::string& _message);
	bool Receive(std::string& _message);
	ClientSocket();
	~ClientSocket();
};
