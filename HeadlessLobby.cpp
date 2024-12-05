#include "HeadlessLobby.h"
#include "PlayerInfo.h"
#include <vector>
#include <sstream>

HeadlessLobby::HeadlessLobby() :
	m_logDocument(),
	m_lobbyInfo(),
	m_playerXML(),
	m_players(),
	m_playerCount(0)
{
	m_logDocument.append_child("Event").append_attribute("LobbyCreated").set_value("rightnow");
	m_lobbyInfo.append_child("Players");
}

HeadlessLobby::~HeadlessLobby()
{

}

void HeadlessLobby::CreateNewPlayer()
{
	//Add player to xml and players vector
	m_players.push_back(new PlayerInfo(m_playerCount));

	//Ungodly statement to make a pointer that wont go out of date
	std::shared_ptr<pugi::xml_node> newPlayerNode = std::make_shared<pugi::xml_node>(m_lobbyInfo.child("Players").append_child("Player"));

	//Passing back and forth info as the raw pugi::xml_node corrupts the attributes n stuff, so I just pass it as a string and then convert it.
	pugi::xml_document playerNodeDoc;
	playerNodeDoc.load_string(m_players[m_playerCount]->AsXMLString().c_str());

	pugi::xml_node playerNode = playerNodeDoc.child("Player");

	//Copy across the new player values into the xml
	newPlayerNode->append_attribute("id").set_value(playerNode.attribute("id").value());
	newPlayerNode->append_attribute("username").set_value(playerNode.attribute("username").value());
	newPlayerNode->append_attribute("shape").set_value(playerNode.attribute("shape").value());
	newPlayerNode->append_attribute("destination").set_value(playerNode.attribute("destination").value());
	newPlayerNode->append_attribute("start").set_value(playerNode.attribute("start").value());
	m_playerXML.push_back(newPlayerNode);
	m_playerCount++;
}

void HeadlessLobby::RemovePlayer(int _id)
{
	if (_id < m_players.size())
	{
		delete m_players[_id];
	}
}


void HeadlessLobby::ChangeAttribute(int _id, std::string& _attributeName, std::string& _newValue)
{
	m_players[_id]->ChangeAttribute(_attributeName, _newValue);
	m_playerXML[_id]->attribute(_attributeName.c_str()).set_value(_newValue.c_str());
}

void HeadlessLobby::LogEvent(std::string& _eventXML)
{
	//Load string into xml and plug into m_logDocument, then save to file
	pugi::xml_document eventDoc;
	eventDoc.load_string(_eventXML.c_str());

	m_logDocument.insert_child_after("Event", eventDoc);
	m_logDocument.save_file("./Log/headless.txt");
}

void HeadlessLobby::LogEvent(pugi::xml_node _eventXML)
{
	m_logDocument.insert_child_after("Event", _eventXML);
	m_logDocument.save_file("./Log/headless.txt");
}

std::string HeadlessLobby::AsXMLString()
{
	std::stringstream ss;
	m_lobbyInfo.save(ss);
	std::string lobbyAsString = ss.str();
	printf("Lobby Info:\n%s", lobbyAsString.c_str());
	return lobbyAsString;
}