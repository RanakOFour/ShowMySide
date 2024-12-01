#include "Timer.h"
#include <string>

#pragma once

class Lobby;
class ServerSocket;
class Host : public Timer
{
private:
	Lobby* m_Lobby;
	ServerSocket* m_Server;
public:
	Host(int _port, double _tickTime);
	~Host();
	std::string GetIP();
	void OnTick(void* _userData);
};

