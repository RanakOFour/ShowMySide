#include "Network/Server.h"
#include "Timer.h"
#include "Network/ServerSocket.h"
#include "Network/ServerRecords.h"
#include "pugixml.hpp"

#if _WIN32
#else
	#include <unistd.h>
#endif

#include <iostream>
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

/* msleep(): Sleep for the requested number of milliseconds. Godspeed, random stackoverflow person */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do 
	{
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void Server::MonitorNetwork()
{
	while (!m_serverClosed)
	{
		//Get repackaged xml message from serversocket
		pugi::xml_document eventsFromClient = m_socket.Update();

		if (eventsFromClient.child("Events").first_child() != NULL)
		{
			printf("Server messages recieved\n");

			//Cycle through events and apply changes based on event type
			for (pugi::xml_node currentEvent = eventsFromClient.child("Events").first_child(); currentEvent; currentEvent = currentEvent.next_sibling())
			{
				m_records.LogEvent(currentEvent);

				// Enact event based on event type
				std::string eventName = currentEvent.attribute("type").value();
				//printf("Parsing current event type: %s\n", eventName.c_str());

				if (eventName == "new_plr")
				{
					m_socket.SendTo(m_records.GetNextPlayerIndex(), m_records.AsXMLString());

					int newPlayersId = m_records.CreateNewPlayer();
					m_socket.SetNewPlayerID(m_records.FindPlayer(newPlayersId));
					currentEvent.append_attribute("id").set_value(newPlayersId);
					
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
				else if (eventName == "move")
				{
					std::string start = currentEvent.attribute("start").value();
					std::string destination = currentEvent.attribute("destination").value();
					m_records.ChangeAttribute(currentEvent.attribute("id").as_int(), "start", start);
					m_records.ChangeAttribute(currentEvent.attribute("id").as_int(), "destination", destination);
				}
				else if (eventName == "server_info_pls")
				{
					m_socket.SendTo(m_records.FindPlayer(currentEvent.attribute("id").as_int()), m_records.AsXMLString());
				}

				m_records.LogEvent(currentEvent);
				//Only other event types are 'new_message' and 'close_server', but that is handled by individual clients
			}

			// Prevents server from throttling
			#if _WIN32
				Sleep(20);
			#else
				msleep(20);
			#endif

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

	pugi::xml_document closeEvent;
	pugi::xml_node eventNode = closeEvent.append_child("Events");
	eventNode.append_child("Event").append_attribute("type").set_value("close_server");

	m_socket.Send(closeEvent);
	m_socket.Close();

	return true;
}

std::string Server::GetIPAddress()
{ 
	return m_socket.m_ipAddress;
}