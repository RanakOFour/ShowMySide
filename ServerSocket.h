#include <winsock2.h>
#include <WS2tcpip.h>
#include <memory>
#include <vector>
#include <string>

class ClientSocket;
class Host;
class ServerSocket
{
	friend class Host;
private:
	SOCKET m_socket;
	int m_idJustAdded;
	std::vector<std::shared_ptr<ClientSocket>> m_clients;
	std::string m_ipAddress;
	std::shared_ptr<ClientSocket> Accept();
	std::string OnTick();
	void Send(std::string _xmlToSend);
	void SendTo(int _id, std::string _xmlToSend);

public:
	ServerSocket(int _port);
	~ServerSocket();
};
