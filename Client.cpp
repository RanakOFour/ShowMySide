#include "Client.h"
#include "ClientSocket.h"
#include "Lobby.h"
#include "Player.h"
#include "Pugixml/pugixml.hpp"
#include "FL/Fl_Text_Buffer.H"
#include "FL/Fl_Text_Display.H"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>

Client::Client() :
	Timer((double)(1.0f / 120.0f)),
	m_socket(nullptr),
	m_lobby(nullptr)
{
	m_mainWindowLog = new Fl_Text_Buffer();
}

Client::~Client()
{

}

void Client::OnTick(void* _userData)
{
	//Don't run before connected to a lobby
	if (m_socket != nullptr)
	{
		pugi::xml_document eventsDocument;
		std::string eventsFromServer;

		// Loop through getting information from server until it has it all
		bool dataComplete{ false };
		while (!dataComplete)
		{
			std::string currentDataPull;
			m_socket->Receive(currentDataPull);

			eventsFromServer.append(currentDataPull);
			pugi::xml_parse_result result = eventsDocument.load_string(eventsFromServer.c_str());
			if ((currentDataPull == "" && eventsFromServer == "") || !result.status)
			{
				dataComplete = true;
			}
		}

		if (eventsFromServer != "")
		{
			//printf("Client received: %s", eventsFromServer.c_str());
			//Load lobby information if recieved
			if (m_lobby == nullptr)
			{
				m_lobby = new Lobby(eventsFromServer);
				m_lobby->show();
			}
			else
			{

				//Cycle through messages and apply changes
				for (pugi::xml_node currentEvent = eventsDocument.child("Events").first_child(); currentEvent; currentEvent = currentEvent.next_sibling())
				{
					// Enact event based on event type
					std::string eventName = currentEvent.attribute("type").value();
					int playerId = currentEvent.attribute("id").as_int();

					if (eventName == "new_plr")
					{
						//Add player to lobby, send out lobby info
						std::shared_ptr<Player> newPlayer = m_lobby->CreateNewPlayer(playerId);
						
						std::string logString = "Player with id " + newPlayer.get()->GetUsername() + " has joined\n";
						m_mainWindowLog->append(logString.c_str());
					}
					else if (eventName == "plr_leave")
					{
						//Remove player from lobby, resend new lobby info
						std::shared_ptr<Player> deletedPlayer = m_lobby->RemovePlayer(currentEvent.attribute("id").as_int());
						
						std::string logString = "Player with id " + deletedPlayer.get()->GetUsername() + " has left\n";
						m_mainWindowLog->append(logString.c_str());
					}
					else if (eventName == "attr_change")
					{
						// Change specified attribute
						std::string attribute = currentEvent.attribute("attribute").value();
						std::string value = currentEvent.attribute("value").value();
						m_lobby->ChangeAttribute(playerId, attribute, value);
					}
					else if (eventName == "new_message")
					{
						std::string text = currentEvent.attribute("text").value();
						m_lobby->ShowMessage(playerId, text);
						m_lobby->redraw();

						
						std::string logString = m_lobby->GetUsername(playerId) + ": " + text + "\n";
						m_mainWindowLog->append(logString.c_str());
					}

					//Only other event type is 'new_message', but that is handled by individual clients
				}
			}
		}

		if (m_lobby != nullptr)
		{
			m_lobby->Update();

			//Get any events that need to be sent
			pugi::xml_document toSend;
			m_lobby->FlushEvents(toSend);

			if (toSend.child("Event"))
			{
				Send(toSend);
			}
		}
	}
}


bool Client::Connect(std::string& _ipToConnect)
{
	m_socket = new ClientSocket();

	if (m_socket->Connect(_ipToConnect))
	{
		//Request xml info then apply
		//printf("Connected successfully!\n");

		pugi::xml_document newPlayerEvent;
		newPlayerEvent.append_child("Event");
		newPlayerEvent.child("Event").append_attribute("type").set_value("new_plr");

		std::stringstream ss;
		newPlayerEvent.save(ss);
		std::string messageString = ss.str();
		//printf("Request to Send:\n%s", messageString.c_str());

		m_socket->Send(messageString);
		return true;
	}

	//printf("Failed to connect to %s\n", _ipToConnect.c_str());
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

Lobby* Client::GetLobby()
{
	return m_lobby;
}