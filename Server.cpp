#include "Server.h"
#include "Timer.h"
#include "ServerSocket.h"
#include "ServerRecords.h"
#include "Pugixml/pugixml.hpp"

#include <stdexcept>
#include <thread>

Server::Server(int _port) :
	m_socket(_port),
	m_records(),
	m_serverClosed(false)
{

	// Set network monitoring on another thread so that it doesn't conflist with client stuff
	// Also using fltk timeout gets messed up when you have too many timers aparrently
	// Also it needs to be a pointer so that it can be instantiated here, after the server and lobby have been instantiated
	m_networkingThread = std::thread(&Server::MonitorNetwork, this);
}

Server::~Server()
{

}

void Server::MonitorNetwork()
{
	while (!m_serverClosed)
	{
		//Get repackaged xml message from serversocket
		pugi::xml_document eventsFromClient = m_socket.Update();

		if (eventsFromClient.child("Events").first_child() != NULL)
		{
			//printf("Server messages recieved\n");

			//Cycle through events and apply changes based on event type
			for (pugi::xml_node currentEvent = eventsFromClient.child("Events").first_child(); currentEvent; currentEvent = currentEvent.next_sibling())
			{
				m_records.LogEvent(currentEvent);

				// Enact event based on event type
				std::string eventName = currentEvent.attribute("type").value();
				//printf("Parsing current event type: %s\n", eventName.c_str());

				if (eventName == "new_plr")
				{
					int newPlayersId = m_records.CreateNewPlayer();
					currentEvent.append_attribute("id").set_value(newPlayersId);
					m_socket.SetNewPlayerID(newPlayersId);

					m_socket.SendServerInfo(m_records.FindPlayer(newPlayersId), m_records.AsXMLString());
				}
				else if (eventName == "plr_leave")
				{
					m_records.RemovePlayer(currentEvent.attribute("id").as_int());
					m_socket.RemoveConnection(currentEvent.attribute("id").as_int());
				}
				else if (eventName == "attr_change")
				{
					std::string attribute = currentEvent.attribute("attribute").value();
					std::string value = currentEvent.attribute("value").value();
					m_records.ChangeAttribute(currentEvent.attribute("id").as_int(), attribute, value);
				}
				else if (eventName == "server_info_pls")
				{
					m_socket.SendServerInfo(m_records.FindPlayer(currentEvent.attribute("id").as_int()), m_records.AsXMLString());
				}

				m_records.LogEvent(currentEvent);
				//Only other event types are 'new_message' and 'close_server', but that is handled by individual clients
			}

			//Echo events out to the clients
			m_socket.Send(eventsFromClient);
		}
	}
}

bool Server::CloseServer()
{
	// End the thread, then send out close_server event
	m_serverClosed = true;
	m_networkingThread.join();

	pugi::xml_document closeServerMessage;
	pugi::xml_node closeEvent = closeServerMessage.append_child("Events").append_child("Event");
	closeEvent.append_attribute("type").set_value("close_server");

	m_socket.Send(closeServerMessage);

	//m_socket->CloseConnections();

	return true;
}

std::string Server::GetIPAddress()
{ 
	return m_socket.m_ipAddress;
}