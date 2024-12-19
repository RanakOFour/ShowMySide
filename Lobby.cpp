#include "Lobby.h"
#include "Player.h"
#include "PlayerInfo.h"
#include "ChatBox.h"
#include "ImagePool.h"
#include "ClientPlayer.h"
#include "Pugixml/pugixml.hpp"

#include <vector>
#include <memory>

Lobby::Lobby() :
	Fl_Double_Window(0, 0, 1200, 800, "Lobby"),
	m_events(),
	m_players(),
	m_chatBox(),
	m_clientPlayer(),
	m_playerId(-1),
	m_textFromChatbox(),
	m_closed(false),
	m_hasLoaded(false)
{
	icon(ImagePool::GetImage(ImageType::ICON).get());

	m_events.append_child("Events");

	callback(CloseWindow, this);

	end();
}

Lobby::~Lobby()
{

}

int Lobby::handle(int _event)
{
	switch (_event)
	{

	case FL_FOCUS:
		return 1;

	case FL_PUSH:
		HandleMouseEvent(Fl::event_button());
		return 1;

	case FL_KEYDOWN:
		HandleKeyboardEvent(Fl::event_key());
		return 1;
	}

	return 0;
}

void Lobby::HandleMouseEvent(int _mouseButton)
{
	// Private information from Player is needed for the movement event, hence the need for an entire function

	// That is dumb, if it's needed it shouldn't be private, different approach required
	// ClientPlayer : Player with Create__Event()?
	// :thumbs_up:
	if (_mouseButton == FL_LEFT_MOUSE && !m_chatBox.visible())
	{
		
		pugi::xml_document newEvent = m_clientPlayer.CreateMovementEvent(Fl::event_x(), Fl::event_y());
		m_events.first_child().append_copy(newEvent.first_child());
	}
}

void Lobby::HandleKeyboardEvent(int _key)
{
	switch (_key)
	{

	// Open chatbox to type a new message
	case 'T':
	case 't':
		if (!m_chatBox.visible())
		{
			m_chatBox.Display(0);
			redraw();
		}

		break;

	// Open chatbox to type a new username
	case 'Q':
	case 'q':
		if (!m_chatBox.visible())
		{
			m_chatBox.Display(1);
			redraw();
		}

		break;

	// Create server info request event
	case 'H':
	case 'h':
		m_events.first_child().append_child("Event").append_attribute("type").set_value("server_info_pls");
		m_events.first_child().child("Event").append_attribute("id").set_value(m_playerId);
		break;

	// Any keyboard event 1-4 will change which character the player is
	case '1':
	case '2':
	case '3':
	case '4':
	{
		// <Event type="attr_change", attribute="shape", value=_key-49>
		pugi::xml_node newEvent = m_events.first_child().append_child("Event");
		newEvent.append_attribute("type").set_value("attr_change");
		newEvent.append_attribute("attribute").set_value("shape");
		newEvent.append_attribute("value").set_value(_key - 49);
		newEvent.append_attribute("id").set_value(m_playerId);
	}
		break;
	}
}

void Lobby::CloseWindow(Fl_Widget* _widget, void* _userData)
{
	Lobby* _lobby = (Lobby*)_userData;

	//Add player leave event
	pugi::xml_node eventNode = _lobby->m_events.first_child().append_child("Event");
	eventNode.append_attribute("type").set_value("plr_leave");
	eventNode.append_attribute("id").set_value(_lobby->m_playerId);

	_lobby->m_closed = true;
	_lobby->hide();
}

void Lobby::LoadLobbyInformation(std::string& _docToLoad)
{
	//Loads xml document from text, then copies out the data from the document into m_players
	pugi::xml_document xmlDoc;
	xmlDoc.load_string(_docToLoad.c_str());

	for (pugi::xml_node currentPlayer = xmlDoc.child("ServerInfo").first_child(); currentPlayer; currentPlayer = currentPlayer.next_sibling())
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
		std::shared_ptr<Player> newPlayer = std::make_shared<Player>(currentInfo);
		m_players.push_back(newPlayer);
		add(m_players[m_players.size() - 1].get());
	}

	m_clientPlayer.SetClientPlayer(m_players[m_players.size() - 1]);
	m_playerId = m_clientPlayer.GetID();
	m_hasLoaded = true;

	//Add chatbox here so it gets drawn over the players
	add(m_chatBox);

	show();
	redraw();
}

void Lobby::Update()
{
	// Update each player's position
	for (int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->Update();
	}


	// Check for chatbox messsage
	m_textFromChatbox = m_chatBox.FlushMessage();

	if (m_textFromChatbox != "")
	{
		switch (m_chatBox.m_mode)
		{
		case 0:
		{
			// <Event type="new_message", text=m_textFromChatbox.c_str()>
			pugi::xml_node newEvent = m_events.first_child().append_child("Event");
			newEvent.append_attribute("type").set_value("new_message");
			newEvent.append_attribute("text").set_value(m_textFromChatbox.c_str());
			newEvent.append_attribute("id").set_value(m_playerId);

			break;
		}

		case 1:
		{
			// <Event type="attr_change", attribute="username", value=m_textFromChatbox.c_str()>
			pugi::xml_node newEvent = m_events.first_child().append_child("Event");
			newEvent.append_attribute("type").set_value("attr_change");
			newEvent.append_attribute("attribute").set_value("username");
			newEvent.append_attribute("value").set_value(m_textFromChatbox.c_str());
			newEvent.append_attribute("id").set_value(m_playerId);

			break;
		}
		}
	}

	redraw();
}

void Lobby::FlushEvents(pugi::xml_document& _document)
{
	if (m_events.first_child().first_child() != NULL)
	{
		_document.append_child("Events");

		for (pugi::xml_node currentNode = m_events.first_child().first_child(); currentNode; currentNode = currentNode.next_sibling())
		{
			_document.first_child().append_copy(currentNode);
			m_events.first_child().remove_child(currentNode);
		}
	}
}

int Lobby::FindPlayer(int _id)
{
	for (int i = 0; i < m_players.size(); i++)
	{
		if (m_players[i]->GetID() == _id)
		{
			return i;
		}
	}

	return -1;
}

//Creates player object and fills out _playerNode object
std::shared_ptr<Player> Lobby::CreateNewPlayer(int _id)
{
	//Add player to xml and players vector
	m_players.push_back(std::make_shared<Player>(_id));
	add(m_players[m_players.size() - 1].get());

	//Puts the chatbox at the back of the list of children so it gets drawn over everything else
	remove(m_chatBox);
	add(m_chatBox);

	return m_players[m_players.size() - 1];
}

std::shared_ptr<Player> Lobby::RemovePlayer(int _id)
{
	int playerIndex = FindPlayer(_id);
	std::shared_ptr<Player> deletedPlayer = m_players[playerIndex];
	m_players.erase(m_players.begin() + playerIndex);

	return nullptr;
}

void Lobby::ChangeAttribute(int _id, std::string _attributeName, std::string& _newValue)
{
	m_players[FindPlayer(_id)]->ChangeAttribute(_attributeName, _newValue);
}

void Lobby::ShowMessage(int _id, std::string& _message)
{
	m_players[FindPlayer(_id)]->ShowMessage(_message);
}

std::string Lobby::GetUsername(int _id)
{
	return m_players[FindPlayer(_id)]->GetUsername();
}

std::string Lobby::GetClientUsername()
{
	return m_players[FindPlayer(m_playerId)]->GetUsername();
}

void Lobby::Closed(bool _closed)
{
	m_closed = _closed;
	if (m_closed)
	{
		hide();
	}
}

bool Lobby::IsClosed()
{
	return m_closed;
}

bool Lobby::IsLoaded()
{
	return m_hasLoaded;
}