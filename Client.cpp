#include "Client.h"
#include "ClientSocket.h"
#include "Lobby.h"
#include "Player.h"
#include "Encryption.h"
#include "Pugixml/pugixml.hpp"

#include "FL/Fl_Text_Buffer.H"
#include "FL/Fl_Text_Display.H"
#include "FL/fl_ask.H"

#include <iostream>
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

}

void Client::OnTick()
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

		if (eventsDocument.child("ServerInfo"))
		{
			if (!m_lobby.IsLoaded())
			{
				m_lobby.LoadLobbyInformation(eventsFromServer);
			}
			else
			{
				std::string otherText = "Server Version: ";
				otherText.append(eventsDocument.child("ServerInfo").attribute("version").as_string());

				fl_alert(otherText.c_str());
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
					std::string cipherText = currentEvent.attribute("text").as_string();
					std::string message = Encryption::Decrypt(cipherText, m_lobby.GetUsername(playerId));

					m_lobby.ShowMessage(playerId, message);
					m_lobby.redraw();


					std::string logString = m_lobby.GetUsername(playerId) + ": " + message + "\n";
					m_mainWindowLog.append(logString.c_str());
				}
				else if (eventName == "close_server")
				{
					m_socket->CloseConnection();
					m_socket.reset();
					m_socket = nullptr;

					m_lobby.Closed(true);
				}

				//Only other event type is 'new_message', but that is handled by individual clients
			}
		}
	}

	if (!m_lobby.IsClosed())
	{
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

void Client::Close()
{
	m_lobby.Closed(true);
	if (m_socket)
	{
		m_socket->CloseConnection();
	}
}