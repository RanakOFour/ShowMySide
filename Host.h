#include "Timer.h"
#include <string>

class ServerSocket;
class Client;
#pragma once
class Host : public Timer
{
private:
	ServerSocket* m_Server;

public:
	Host(int _port, double _tickTime);
	~Host();
	std::string GetIP();
	void OnTick(void* _userData);
};

