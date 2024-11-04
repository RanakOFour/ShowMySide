#include <winsock2.h>
#include <string>

class ServerSocket;
class ClientSocket
{
public:
	ClientSocket();
	~ClientSocket();
	bool Connect(std::string _connectInfo);
	bool Receive(std::string& _message);
	void Send(const std::string& _message);
	bool m_closed;
private:
	friend class ServerSocket;
	SOCKET m_socket;
	ClientSocket(const ClientSocket& _copy);
	ClientSocket& operator=(const ClientSocket& _assign);
};
