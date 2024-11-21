#include "Timer.h"
#include <string>

#pragma once
class ClientSocket;

//This is a clientsocket wrapper
class Client : public Timer
{
private:
	ClientSocket* m_Socket;
	std::string m_currentMessage;

public:
	Client();
	~Client();
	bool Connect(const char* _ipToConnect);
	void Send(const std::string _message);
	void OnTick(void* _userData);
};

