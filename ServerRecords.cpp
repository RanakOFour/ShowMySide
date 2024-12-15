#include "ServerRecords.h"
#include "PlayerInfo.h"
#include <vector>
#include <sstream>

ServerRecords::ServerRecords() :
	m_logDocument(),
	m_lobbyInfo(),
	m_playerXML(),
	m_playerInfos(),
	m_nextPlayerID(0)
{
	m_logDocument.append_child("Event").append_attribute("LobbyCreated").set_value("rightnow");
	m_lobbyInfo.append_child("Players");
}

ServerRecords::~ServerRecords()
{

}

int ServerRecords::CreateNewPlayer()
{
	//Add new playerinfo to players vector, then to xml_node vector
	m_playerInfos.push_back(std::make_shared<PlayerInfo>(m_nextPlayerID));

	//Make a pointer to a new XML player node in m_lobbyInfo
	std::shared_ptr<pugi::xml_node> newPlayerNode = std::make_shared<pugi::xml_node>(m_lobbyInfo.child("Players").append_child("Player"));

	// Pass new playerInfo into XML document, and hook up the new node into m_playerXML
	pugi::xml_document playerNodeDoc;

	// I need to use m_playerInfos.size() - 1, as the next id may not be the player's position if a player leaves
	playerNodeDoc.load_string(m_playerInfos[m_playerInfos.size() - 1].get()->AsXMLString().c_str());

	pugi::xml_node playerNode = playerNodeDoc.child("Player");

	//Copy across the new player values into the xml
	newPlayerNode->append_attribute("id").set_value(playerNode.attribute("id").value());
	newPlayerNode->append_attribute("username").set_value(playerNode.attribute("username").value());
	newPlayerNode->append_attribute("shape").set_value(playerNode.attribute("shape").value());
	newPlayerNode->append_attribute("destination").set_value(playerNode.attribute("destination").value());
	newPlayerNode->append_attribute("start").set_value(playerNode.attribute("start").value());
	m_playerXML.push_back(newPlayerNode);
	++m_nextPlayerID;

	return m_nextPlayerID - 1;
}

void ServerRecords::RemovePlayer(int _id)
{
	for (int i = 0; i < m_playerInfos.size(); i++)
	{
		if (m_playerInfos[i].get()->GetID() == _id)
		{
			m_playerInfos.erase(m_playerInfos.begin() + i);
			break;
		}
	}
}


void ServerRecords::ChangeAttribute(int _id, std::string& _attributeName, std::string& _newValue)
{
	m_playerInfos[_id]->ChangeAttribute(_attributeName, _newValue);
	m_playerXML[_id]->attribute(_attributeName.c_str()).set_value(_newValue.c_str());
}

void ServerRecords::LogEvent(pugi::xml_node& _eventXML)
{
	m_logDocument.insert_child_after("Event", _eventXML);
	m_logDocument.save_file("./Log/headless.txt");
}

std::string ServerRecords::AsXMLString()
{
	std::stringstream ss;
	m_lobbyInfo.save(ss);
	std::string lobbyAsString = ss.str();
	printf("Lobby Info:\n%s", lobbyAsString.c_str());
	return lobbyAsString;
}