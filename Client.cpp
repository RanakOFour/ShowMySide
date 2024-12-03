#include "Client.h"
#include "ClientSocket.h"
#include "Lobby.h"
#include "Pugixml/pugixml.hpp"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>

Client::Client() :
	Timer(0.25),
	m_socket(nullptr),
	m_lobby(nullptr)
{
}

Client::~Client()
{

}

bool Client::Connect(const char* _ipToConnect)
{
	m_socket = new ClientSocket();

	if (m_socket->Connect(_ipToConnect))
	{
		//Request xml info then apply
		printf("Connected successfully!\n");

		pugi::xml_document newPlayerEvent;
		newPlayerEvent.append_child("Event");
		newPlayerEvent.child("Event").append_attribute("type").set_value("new_plr");

		std::stringstream ss;
		newPlayerEvent.save(ss);
		std::string messageString = ss.str();
		printf("Request to Send:\n%s", messageString.c_str());

		m_socket->Send(messageString);
		return true;
	}

	printf("Failed to connect to %s\n", _ipToConnect);
	return false;
}

void Client::Send(const std::string _message)
{
	pugi::xml_document newDoc;
	pugi::xml_node message = newDoc.append_child("Event");
	message.append_attribute("type").set_value("new_message");
	message.append_attribute("text").set_value(_message.c_str());

	std::stringstream ss;
	newDoc.save(ss);
	std::string messageString = ss.str();
	printf("XML to Send: %s", messageString.c_str());

	m_socket->Send(messageString);
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
		while(!dataComplete)
		{
			std::string currentDataPull;
			m_socket->Receive(currentDataPull);

			eventsFromServer.append(currentDataPull);
			pugi::xml_parse_result result = eventsDocument.load_string(eventsFromServer.c_str());
			if (currentDataPull == "" || result.status)
			{
				dataComplete = true;
			}
		}

		if (eventsFromServer != "")
		{
			printf("Client received: %s", eventsFromServer.c_str());
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

					if (eventName == "new_plr")
					{
						//Add player to lobby, send out lobby info
						m_lobby->CreateNewPlayer(currentEvent.attribute("id").as_int());
					}
					else if (eventName == "plr_leave")
					{
						//Remove player from lobby, resend new lobby info
						m_lobby->RemovePlayer(currentEvent.attribute("id").as_int());
					}
					else if (eventName == "attr_change")
					{
						// Change specified attribute
						m_lobby->ChangeAttribute(currentEvent.attribute("id").as_int(), currentEvent.attribute("attribute").value(), currentEvent.attribute("value").value());
					}
					else if (eventName == "new_message")
					{
						m_lobby->ShowMessage(currentEvent.attribute("id").as_int(), currentEvent.attribute("text").value());
						m_lobby->redraw();
					}

					//Only other event type is 'new_message', but that is handled by individual clients
				}
			}
		}

		m_lobby->OnTick();
	}

	Fl::repeat_timeout(1 / 30, Tick, this);
}