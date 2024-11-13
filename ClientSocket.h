#include <winsock2.h>
#include <string>

class ServerSocket;
class ClientSocket
{
public:
	bool m_closed;
	bool Connect(const std::string& _serverName);
	void Send(const std::string& _message);
	bool Receive(std::string& _message);
	ClientSocket();
	~ClientSocket();
private:
	friend class ServerSocket;
	SOCKET m_socket;
	ClientSocket& operator=(const ClientSocket& _assign);
};
