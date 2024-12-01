#include <winsock2.h>
#include <WS2tcpip.h>
#include <memory>
#include <vector>
#include <string>

class ClientSocket;
class ServerSocket
{
private:
	SOCKET m_socket;
	std::vector<std::shared_ptr<ClientSocket>> m_clients;
	std::shared_ptr<ClientSocket> accept();
	ServerSocket(const ServerSocket& _copy);

public:
	ServerSocket(int _port);
	~ServerSocket();
	std::string m_ipAddress;
	void OnTick();
};
