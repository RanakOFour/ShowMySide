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
	m_clientPlayer(nullptr)
{
	icon(ImagePool::GetImage(ImagePool::ImageType::ICON));


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
		m_players.push_back(new Player(currentInfo));
	}

	m_clientPlayer = m_players[m_players.size() - 1];
	add(m_clientPlayer);

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
	case FL_FOCUS:
		return 1;

	case FL_PUSH:
		if (Fl::event_button() == FL_LEFT_MOUSE && !m_chatBox->visible())
		{
			pugi::xml_document newEvent = m_clientPlayer->SetDestination(Fl::event_x(), Fl::event_y());
			m_events.append_copy(newEvent.first_child());
			m_events.append_copy(newEvent.last_child());
			return 1;
		}
		break;

	case FL_KEYDOWN:
		HandleKeyboardEvent(Fl::event_key());
		break;
	}

	return 0;
}

void Lobby::HandleKeyboardEvent(int _key)
{
	switch (_key)
	{
	case 't':
		printf("'t' key pressed!\n");
		if (!m_chatBox->visible())
		{
			m_chatBox->Display(0);
			redraw();
		}
		break;

	case 'q':
		printf("q pressed!\n");
		m_chatBox->Display(1);

		break;

	case '1':
	case '2':
	case '3':
	case '4':
	{
		pugi::xml_document newEvent = m_clientPlayer->ChangeImage((ImagePool::ImageType)(_key - 49));
		m_events.append_copy(newEvent.child("Event"));
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

void Lobby::OnTick()
{
	//GetInput (if any)
	// On click set Destination (change attr)
	// On t open chat box
	// move characters

	for (int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->OnTick();
	}

	redraw();
}

//Creates player object and fills out _playerNode object
void Lobby::CreateNewPlayer(int _id)
{
	//Add player to xml and players vector
	m_players.push_back(new Player(_id));
	add(m_players[m_players.size() - 1]);
}

void Lobby::RemovePlayer(int _id)
{
	if (_id < m_players.size())
	{
		delete m_players[_id];
	}
}

void Lobby::ChangeAttribute(int _id, std::string& _attributeName, std::string& _newValue)
{
	m_players[_id]->ChangeAttribute(_attributeName, _newValue);
}

void Lobby::ShowMessage(int _id, std::string& _message)
{
	m_players[_id]->ShowMessage(_message);
}