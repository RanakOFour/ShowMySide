#include <winsock2.h>
#include <memory>
#include <string>

class ClientSocket;
class ServerSocket
{
public:
	ServerSocket(int _port);
	~ServerSocket();
	std::shared_ptr<ClientSocket> accept();
	void on_tick();
	std::string m_ipAddress;
private:
	int m_tickNum;
	SOCKET m_socket;
	char* GetIP();
	ServerSocket(const ServerSocket& _copy);
	ServerSocket& operator=(const ServerSocket& _assign);
};
