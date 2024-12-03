#include "Timer.h"
#include <string>

#pragma once
class ClientSocket;
class Lobby;

//This is a clientsocket wrapper
class Client : public Timer
{
private:
	ClientSocket* m_socket;
	Lobby* m_lobby;

public:
	Client();
	~Client();
	bool Connect(const char* _ipToConnect);
	void Send(const std::string _message);
	void OnTick(void* _userData);
};

