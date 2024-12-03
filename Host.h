#include "Timer.h"
#include "Pugixml/pugixml.hpp"
#include <string>

#pragma once
class ServerSocket;
class HeadlessLobby;

class Host
{
private:
	ServerSocket* m_server;
	HeadlessLobby* m_lobby;
	bool m_closeServer;
public:
	Host(int _port, double _tickTime);
	~Host();
	void MonitorNetwork();
	std::string GetIP();
};

