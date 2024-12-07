#include "Host.h"
#include "Lobby.h"
#include "Timer.h"
#include "ServerSocket.h"
#include "HeadlessLobby.h"
#include "Pugixml/pugixml.hpp"
#include <stdexcept>
#include <thread>

Host::Host(int _port, double _tickTimer) :
	m_server(nullptr),
	m_lobby(nullptr),
	m_closeServer(false)
{
	m_server = new ServerSocket(_port);
	m_lobby = new HeadlessLobby();

	// Set network monitoring on another thread so that it doesn't conflist with client stuff
	// Also using fltk timeout gets messed up when you have too many timers aparrently, and this was my idea of getting rid of a repeat_timeout call
	m_networkingThread = new std::thread(&Host::MonitorNetwork, this);
	m_networkingThread->detach();
}

Host::~Host()
{

}

void Host::MonitorNetwork()
{
	while (!m_closeServer)
	{
		//Get repackaged xml message from serversocket
		pugi::xml_document eventsFromClient = m_server->OnTick();

		if (eventsFromClient.child("Events").first_child() != NULL)
		{
			printf("Host messages recieved\n");

			//Cycle through messages and apply changes
			for (pugi::xml_node currentEvent = eventsFromClient.child("Events").first_child(); currentEvent; currentEvent = currentEvent.next_sibling())
			{
				m_lobby->LogEvent(currentEvent);

				// Enact event based on event type
				std::string eventName = currentEvent.attribute("type").value();
				printf("Parsing current event type: %s\n", eventName.c_str());

				if (eventName == "new_plr")
				{
					//Add player to lobby
					m_lobby->CreateNewPlayer();
					m_server->SendServerInfo(currentEvent.attribute("id").as_int(), m_lobby->AsXMLString());
				}
				else if (eventName == "plr_leave")
				{
					//Remove player from lobby, resend new lobby info
					m_lobby->RemovePlayer(currentEvent.attribute("id").as_int());
				}
				else if (eventName == "attr_change")
				{
					// Change specified attribute
					std::string attribute = currentEvent.attribute("attribute").value();
					std::string value = currentEvent.attribute("value").value();
					m_lobby->ChangeAttribute(currentEvent.attribute("id").as_int(), attribute, value);
				}

				//Only other event type is 'new_message', but that is handled by individual clients
			}

			//Ship events out to the clients
			m_server->Send(eventsFromClient);
		}
	}
}

std::string Host::GetIPAddress()
{
	return m_server->m_ipAddress;
}