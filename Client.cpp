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
	m_loaded(false)
{
	m_mainWindowLog = new Fl_Text_Buffer();
	m_lobby = new Lobby();
}

Client::~Client()
{

}

void Client::OnTick(void* _userData)
{
	// No socket == no connection, so there is nothing to be done
	if (m_socket == nullptr)
	{
		return;
	}

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


	// Process events from server
	if (eventsFromServer != "")
	{
		//printf("Client received: %s", eventsFromServer.c_str());


		// First time recieving information from server will be the player information, which will flag m_loaded as true
		if (!m_loaded)
		{
			m_lobby->LoadLobbyInformation(eventsFromServer);
			m_loaded = true;
		}

		//Cycle through events from server and apply changes
		for (pugi::xml_node currentEvent = eventsDocument.first_child().first_child(); currentEvent; currentEvent = currentEvent.next_sibling())
		{
			// Enact event based on event type
			std::string eventName = currentEvent.attribute("type").value();
			int playerId = currentEvent.attribute("id").as_int();

			if (eventName == "new_plr")
			{
				//Add player to lobby, send out lobby info
				std::shared_ptr<Player> newPlayer = m_lobby->CreateNewPlayer(playerId);

				std::string logString = newPlayer.get()->GetUsername() + " has joined\n";
				m_mainWindowLog->append(logString.c_str());
			}
			else if (eventName == "plr_leave")
			{
				//Remove player from lobby, resend new lobby info
				std::shared_ptr<Player> deletedPlayer = m_lobby->RemovePlayer(currentEvent.attribute("id").as_int());

				std::string logString = deletedPlayer.get()->GetUsername() + " has left\n";
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

	//Get any events that need to be sent
	pugi::xml_document toSend;
	m_lobby->FlushEvents(toSend);

	if (toSend.first_child().first_child())
	{
		Send(toSend);
	}

	//Stop this function from running on server close
	if (m_lobby->IsClosed())
	{
		m_socket->CloseConnection();
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
		newPlayerEvent.append_child("Events");
		newPlayerEvent.first_child().append_child("Event").append_attribute("type").set_value("new_plr");

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