#include "Host.h"
#include "Client.h"
#include "Timer.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Pugixml/pugixml.hpp"

Host::Host(int _port, double _tickTimer) :
	Timer(0.25),
	m_Server(nullptr)
{
	m_Server = new ServerSocket(_port);
}

Host::~Host()
{

}

void Host::OnTick(void* _userData)
{
	m_Server->OnTick();

	//printf("Host Tick");
	Fl::repeat_timeout(0.25, Tick, this);
}

std::string Host::GetIP()
{
	return m_Server->m_ipAddress;
}