#include "Lobby.h"
#include "Timer.h"
#include "Player.h"
#include "PlayerInfo.h"
#include "ChatBox.h"
#include "ImagePool.h"
#include "Pugixml/pugixml.hpp"
#include <memory>

Lobby::Lobby(std::string& _docToLoad) :
	Fl_Double_Window(0, 0, 1200, 800, "Lobby"),
	m_events(),
	m_players(),
	m_clientPlayer(nullptr),
	m_textFromChatbox()
{
	icon(ImagePool::GetImage(ImageType::ICON).get());


	//Loads xml document from text, then copies out the data from the document into m_players
	pugi::xml_document xmlDoc;
	xmlDoc.load_string(_docToLoad.c_str());

	for (pugi::xml_node currentPlayer = xmlDoc.child("Players").first_child(); currentPlayer; currentPlayer = currentPlayer.next_sibling())
	{
		int id = atoi(currentPlayer.attribute("id").value());
		int shapeNum = atoi(currentPlayer.attribute("shape").value());
		std::string username = currentPlayer.attribute("username").value();
		std::string destinationString = currentPlayer.attribute("destination").value();
		std::string startString = currentPlayer.attribute("start").value();
		
		int destination[2];
		destination[0] = atoi(destinationString.substr(0, destinationString.find(',')).c_str());
		destination[1] = atoi(destinationString.substr(destinationString.find(',') + 1, destinationString.size()).c_str());


		int start[2];
		start[0] = atoi(startString.substr(0, startString.find(',')).c_str());
		start[1] = atoi(startString.substr(startString.find(',') + 1, startString.size()).c_str());

		PlayerInfo currentInfo(id, username, destination, start, shapeNum);
		m_players.push_back(std::make_shared<Player>(currentInfo));
	}

	m_clientPlayer = m_players[m_players.size() - 1];
	add(m_clientPlayer.get());

	m_chatBox = new Chatbox();
	add(m_chatBox);
	hide();

	end();
	show();
	take_focus();
}

Lobby::~Lobby()
{

}

int Lobby::handle(int _event)
{
	switch (_event)
	{

	// Enable window to take focus
	case FL_FOCUS:
		return 1;

	// Handle mouse event
	case FL_PUSH:
		HandleMouseEvent(Fl::event_button());
		return 1;

	// Keybaord event
	case FL_KEYDOWN:
		HandleKeyboardEvent(Fl::event_key());
		return 1;
	}

	return 0;
}

void Lobby::HandleMouseEvent(int _mouseButton)
{
	// Private information form Player is needed for the movement event, hence the need to an entire function
	if (_mouseButton == FL_LEFT_MOUSE && !m_chatBox->visible())
	{
		pugi::xml_document newEvent = m_clientPlayer->CreateMovementEvent(Fl::event_x(), Fl::event_y());
		m_events.append_copy(newEvent.first_child());
		m_events.append_copy(newEvent.last_child());
	}
}

void Lobby::HandleKeyboardEvent(int _key)
{
	switch (_key)
	{

	// Open chatbox to type a new message
	case 'T':
	case 't':
		if (!m_chatBox->visible())
		{
			m_chatBox->Display(0);
			redraw();
		}

		break;

	// Open chatbox to type a new username
	case 'Q':
	case 'q':
		if (!m_chatBox->visible())
		{
			m_chatBox->Display(1);
			redraw();
		}

		break;

	// Any keyboard event 1-4 will change which character the player is
	case '1':
	case '2':
	case '3':
	case '4':
	{
		pugi::xml_node eventNode = m_events.append_child("Event");
		eventNode.append_attribute("type").set_value("attr_change");
		eventNode.append_attribute("attribute").set_value("shape");
		eventNode.append_attribute("value").set_value(_key - 49);
	}
		break;
	}
}

void Lobby::FlushEvents(pugi::xml_document& _document)
{
	if (m_events.child("Event") != NULL)
	{
		_document.append_child("Event");
		for (pugi::xml_attribute currentAttr = m_events.child("Event").first_attribute(); currentAttr; currentAttr = currentAttr.next_attribute())
		{
			_document.child("Event").append_copy(currentAttr);
		}

		m_events.remove_child("Event");
	}
}

void Lobby::Update()
{
	//GetInput (if any)
	// On click set Destination (change attr)
	// On t open chat box
	// move characters

	for (int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->Update();
	}

	m_textFromChatbox = m_chatBox->FlushMessage();
	// Handles chatbox text if any
	if (m_textFromChatbox != "")
	{
		switch (m_chatBox->m_mode)
		{
		case 0:
		{
			pugi::xml_node newEvent = m_events.append_child("Event");
			newEvent.append_attribute("type").set_value("new_message");
			newEvent.append_attribute("text").set_value(m_textFromChatbox.c_str());

			break;
		}
		
		case 1:
		{
			// <Event type="attr_change", attribute="username", value=_newName>
			pugi::xml_node eventNode = m_events.append_child("Event");
			eventNode.append_attribute("type").set_value("attr_change");
			eventNode.append_attribute("attribute").set_value("username");
			eventNode.append_attribute("value").set_value(m_textFromChatbox.c_str());
			break;
		}
		}
	}

	redraw();
}

//Creates player object and fills out _playerNode object
std::shared_ptr<Player> Lobby::CreateNewPlayer(int _id)
{
	//Add player to xml and players vector
	m_players.push_back(std::make_shared<Player>(_id));
	add(m_players[m_players.size() - 1].get());

	return m_players[m_players.size() - 1];
}

std::shared_ptr<Player> Lobby::RemovePlayer(int _id)
{
	std::shared_ptr<Player> deletedPlayer = nullptr;
	int index{ 0 };

	for (int i = 0; i < m_players.size(); i++)
	{
		if (m_players[i].get()->GetID() == _id)
		{
			index = i;
			deletedPlayer = m_players[i];
			break;
		}
	}

	if (deletedPlayer)
	{
		m_players.erase(m_players.begin() + (index - 1));
		return deletedPlayer;
	}

	return nullptr;
}

void Lobby::ChangeAttribute(int _id, std::string& _attributeName, std::string& _newValue)
{
	m_players[_id]->ChangeAttribute(_attributeName, _newValue);
}

void Lobby::ShowMessage(int _id, std::string& _message)
{
	m_players[_id]->ShowMessage(_message);
}

std::string Lobby::GetUsername(int _id)
{
	return m_players[_id]->GetUsername();
}