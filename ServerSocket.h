#include <winsock2.h>
#include <memory>

class ClientSocket;
class ServerSocket
{
public:
	ServerSocket(int _port);
	~ServerSocket();
	std::shared_ptr<ClientSocket> accept();
private:
	SOCKET m_socket;
	ServerSocket(const ServerSocket& _copy);
	ServerSocket& operator=(const ServerSocket& _assign);
};
