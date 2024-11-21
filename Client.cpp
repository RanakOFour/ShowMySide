#include "Client.h"
#include "ClientSocket.h"
#include "Pugixml/pugixml.hpp"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>

Client::Client() :
	Timer(0.3),
	m_currentMessage("")
{
	m_Socket = new ClientSocket();
}

Client::~Client()
{

}

bool Client::Connect(const char* _ipToConnect)
{
	if (m_Socket->Connect(_ipToConnect))
	{
		printf("Connected successfully\n");
		return true;
	}

	printf("Failed to connect to %s\n", _ipToConnect);
	return false;
}

void Client::Send(const std::string _message)
{
	pugi::xml_document newDoc;
	pugi::xml_node message = newDoc.append_child("c_msg");
	message.append_attribute("text").set_value(_message.c_str());

	std::stringstream ss;
	newDoc.save(ss);
	std::string messageString = ss.str();
	printf("XML to Send: %s", messageString.c_str());

	m_Socket->Send(messageString);
}

void Client::OnTick(void* _userData)
{
	if (m_Socket->m_Connected)
	{
		pugi::xml_document doc;
		while (m_Socket->Receive(m_currentMessage))
		{
			printf("Client received: %s", m_currentMessage.c_str());
			pugi::xml_parse_result result = doc.load_string(m_currentMessage.c_str());
			if (result)
			{
				throw std::runtime_error(result.description());
			}

			pugi::xml_node messages = doc.child("s_msg");
			std::string output = messages.child_value("player");
			output.append(": ");
			output.append(messages.child_value("text"));

			printf("Message recieved: %s", output.c_str());
		}
	}

	printf("Client Tick!\n");
	Fl::repeat_timeout(0.3, Tick, this);
}