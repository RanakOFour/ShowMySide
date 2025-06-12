#include "Network/Client.h"
#include "Network/ClientSocket.h"
#include "Network/Lobby.h"
#include "Network/Player.h"
#include "pugixml.hpp"

#include "FL/Fl_Text_Buffer.H"
#include "FL/Fl_Text_Display.H"

#include <stdexcept>
#include <sstream>
#include <string>

Client::Client() :
	Timer((double)(1.0f / 120.0f)),
	m_socket(nullptr),
	m_lobby(),
	m_mainWindowLog()
{

}

Client::~Client()
{
	m_mainWindowLog.call_predelete_callbacks();
}

void Client::OnTick()
{
	// No socket == no connection, so there is nothing to be done
	if (m_socket == nullptr)
	{
		return;
	}
	
	std::string messagesFromServer;
	m_socket->Receive(messagesFromServer);


	// Process events from server
	if (messagesFromServer != "")
	{

		pugi::xml_document eventsDocument;

		pugi::xml_parse_result parse = eventsDocument.load_string(messagesFromServer.c_str());
		if (parse.status)
		{
			printf("Invalid xml parsed\n");
		}

		//printf("Client received: %s", eventsFromServer.c_str());

		if (eventsDocument.child("ServerInfo"))
		{
			if (!m_lobby.Loaded())
			{
				m_lobby.LoadLobbyInformation(messagesFromServer);
			}
			else
			{
				std::string serverInfo = "Server Version: ";
				serverInfo.append(eventsDocument.child("ServerInfo").attribute("version").as_string() + std::string("\n"));

				
				m_mainWindowLog.append(serverInfo.c_str());
			}
		}
		else
		{
			//Cycle through events from server and apply changes
			for (pugi::xml_node currentEvent = eventsDocument.child("Events").first_child(); currentEvent; currentEvent = currentEvent.next_sibling())
			{
				// Enact event based on event type
				std::string eventName = currentEvent.attribute("type").value();
				int playerId = currentEvent.attribute("id").as_int();

				if (eventName == "new_plr")
				{
					//Add player to lobby, send out lobby info
					std::shared_ptr<Player> newPlayer = m_lobby.CreateNewPlayer(playerId);

					std::string logString = newPlayer.get()->GetUsername() + " has joined\n";
					m_mainWindowLog.append(logString.c_str());
				}
				else if (eventName == "plr_leave")
				{
					//Remove player from lobby
					std::string playerName = m_lobby.GetUsername(playerId);
					std::shared_ptr<Player> deletedPlayer = m_lobby.RemovePlayer(playerId);

					std::string logString = playerName + " has left\n";
					m_mainWindowLog.append(logString.c_str());

					//If the first player has left, the server has closed
					if (playerId == 0)
					{
						m_lobby.Closed(true);
						m_lobby.hide();
						m_mainWindowLog.append("Server closed due to host leaving\n");
						break;
					}
				}
				else if (eventName == "attr_change")
				{
					// Change specified attribute
					std::string attribute = currentEvent.attribute("attribute").value();
					std::string value = currentEvent.attribute("value").value();
					m_lobby.ChangeAttribute(playerId, attribute, value);
				}
				else if (eventName == "move")
				{
					std::string start = currentEvent.attribute("start").value();
					std::string dest = currentEvent.attribute("destination").value();

					m_lobby.ChangeAttribute(playerId, "start", start);
					m_lobby.ChangeAttribute(playerId, "destination", dest);
				}
				else if (eventName == "new_message")
				{
					std::string message = currentEvent.attribute("text").value();
					m_lobby.ShowMessage(playerId, message);
					m_lobby.redraw();


					std::string logString = m_lobby.GetUsername(playerId) + ": " + message + "\n";
					m_mainWindowLog.append(logString.c_str());
				}

			}
		}
	}

	pugi::xml_document toSend;

	//Update lobby with new information
	m_lobby.Update();

	//Get any events that need to be sent
	m_lobby.FlushEvents(toSend);

	// toSend will always have at least <Events> node as a child
	if (toSend.first_child().first_child())
	{
		Send(toSend);
	}

	if(m_lobby.Closed())
	{
		//If a socket is present then the user has closed the lobby, and it still needs to send off the plr_leave event
		if (m_socket != nullptr)
		{
			m_socket->CloseConnection();
			m_socket = nullptr;
		}
	}
}


bool Client::Connect(std::string& _ipToConnect)
{
	m_socket = std::make_unique<ClientSocket>();

	if (m_socket->Connect(_ipToConnect))
	{
		//Request xml info then apply
		//printf("Connected successfully!\n");

		pugi::xml_document newPlayerEvent;
		newPlayerEvent.append_child("Events");
		newPlayerEvent.first_child().append_child("Event").append_attribute("type").set_value("new_plr");

		std::stringstream ss;
		newPlayerEvent.save(ss);
		std::string messageString = ss.str();
		printf("Request to Send:\n%s", messageString.c_str());

		m_socket->Send(messageString);
		return true;
	}

	printf("Failed to connect to %s\n", _ipToConnect.c_str());
	return false;
}

void Client::Send(pugi::xml_document& _nodeToSend)
{
	std::stringstream ss;
	_nodeToSend.save(ss);
	std::string messageString = ss.str();
	//printf("XML to Send: %s", messageString.c_str());

	m_socket->Send(messageString);
}

void Client::SetLogDisplay(Fl_Text_Display* _outputLog)
{
	_outputLog->buffer(m_mainWindowLog);
}

void Client::Close()
{
	m_lobby.Closed(true);
	if (m_socket)
	{
		m_socket->CloseConnection();
	}

	printf("Connection closed\n");
}

bool Client::Closed()
{
	return m_lobby.Closed();
	printf("Lobby closed\n");
}