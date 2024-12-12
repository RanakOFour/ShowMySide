#include "Host.h"
#include "Lobby.h"
#include "Timer.h"
#include "ServerSocket.h"
#include "Server.h"
#include "Pugixml/pugixml.hpp"
#include <stdexcept>
#include <thread>

Host::Host(int _port, double _tickTimer) :
	m_socket(nullptr),
	m_server(nullptr),
	m_serverClosed(false)
{
	m_socket = new ServerSocket(_port);
	m_server = new Server();

	// Set network monitoring on another thread so that it doesn't conflist with client stuff
	// Also using fltk timeout gets messed up when you have too many timers aparrently
	// Also it needs to be a pointer so that it can be instantiated here, after the server and lobby have been instantiated
	m_networkingThread = std::thread(&Host::MonitorNetwork, this);
}

Host::~Host()
{

}

void Host::MonitorNetwork()
{
	while (!m_serverClosed)
	{
		//Get repackaged xml message from serversocket
		pugi::xml_document eventsFromClient = m_socket->Update();

		if (eventsFromClient.child("Events").first_child() != NULL)
		{
			//printf("Host messages recieved\n");

			//Cycle through messages and apply changes
			for (pugi::xml_node currentEvent = eventsFromClient.child("Events").first_child(); currentEvent; currentEvent = currentEvent.next_sibling())
			{
				m_server->LogEvent(currentEvent);

				// Enact event based on event type
				std::string eventName = currentEvent.attribute("type").value();
				//printf("Parsing current event type: %s\n", eventName.c_str());

				if (eventName == "new_plr")
				{
					//Add player to lobby
					m_server->CreateNewPlayer();
					m_socket->SendServerInfo(currentEvent.attribute("id").as_int(), m_server->AsXMLString());
				}
				else if (eventName == "plr_leave")
				{
					//Remove player from lobby, resend new lobby info
					m_server->RemovePlayer(currentEvent.attribute("id").as_int());
				}
				else if (eventName == "attr_change")
				{
					// Change specified attribute
					std::string attribute = currentEvent.attribute("attribute").value();
					std::string value = currentEvent.attribute("value").value();
					m_server->ChangeAttribute(currentEvent.attribute("id").as_int(), attribute, value);
				}

				m_server->LogEvent(currentEvent);
				//Only other event type is 'new_message', but that is handled by individual clients
			}

			//Ship events out to the clients
			m_socket->Send(eventsFromClient);
		}
	}
}

bool Host::CloseServer()
{
	// End the thread, then send out close_server event
	m_serverClosed = true;
	m_networkingThread.join();

	pugi::xml_document closeServerMessage;
	pugi::xml_node closeEvent = closeServerMessage.append_child("Event");
	closeEvent.append_attribute("type").set_value("close_server");

	m_socket->Send(closeServerMessage);

	//m_socket->CloseConnections();

	return true;
}

std::string Host::GetIPAddress()
{
	return m_socket->m_ipAddress;
}