#include <winsock2.h>
#include <WS2tcpip.h>
#include <memory>
#include <vector>
#include <string>

class ClientSocket;
class ServerSocket
{
public:
	std::string m_ipAddress;
	std::shared_ptr<ClientSocket> accept();
	void on_tick();
	ServerSocket(int _port);
	~ServerSocket();
private:
	int m_tickNum;
	SOCKET m_socket;
	std::vector<std::shared_ptr<ClientSocket>> m_clients;
	ServerSocket(const ServerSocket& _copy);
	ServerSocket& operator=(const ServerSocket& _assign);
};
