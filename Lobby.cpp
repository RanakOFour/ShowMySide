#include <memory>
#include "Lobby.h"
#include "Player.h"
#include "PlayerInfo.h"
#include "Timer.h"
#include "Pugixml/pugixml.hpp"

Lobby::Lobby(std::string _docToLoad) :
	Fl_Double_Window(800, 600),
	m_events(),
	m_players(),
	m_clientPlayer(nullptr)
{
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
}

Lobby::~Lobby()
{

}

int Lobby::handle(int _event)
{
	if (_event == FL_PUSH && Fl::event_button() == FL_LEFT_MOUSE)
	{
		m_clientPlayer->SetDestination(Fl::event_x(), Fl::event_y());
		pugi::xml_document playerDoc;
		playerDoc.load_string(m_clientPlayer->AsXMLString().c_str());
		std::string destination[2] = { "destination", playerDoc.child("Player").attribute("destination").value() };


		LogEvent("attr_change", (void*)destination);
		return 1;
	}

	return 0;
}

void Lobby::LogEvent(const char* _type, void* _data)
{
	//Create an event and log into m_events
	pugi::xml_document newEvent;
	pugi::xml_node eventInfo = newEvent.append_child("Event");
	eventInfo.append_attribute("type").set_value(_type);

	if (_type == "attr_change")
	{
		//// Change specified attribute
		//std::string data[2] = ;

		//eventInfo.append_attribute("attribute").set_value(attrName->c_str());
		//eventInfo.append_attribute("value").set_value(newVal->c_str());
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

void Lobby::ChangeAttribute(int _id, std::string _attributeName, std::string _newValue)
{
	m_players[_id]->ChangeAttribute(_attributeName, _newValue);
}

void Lobby::ShowMessage(int _id, std::string _message)
{
	m_players[_id]->ShowMessage(_message, this);
}