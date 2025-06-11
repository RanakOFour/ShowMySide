#include "Network/ServerRecords.h"
#include "Network/PlayerInfo.h"

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
	m_lobbyInfo.append_child("ServerInfo").append_attribute("version").set_value("4.20.69");
}

ServerRecords::~ServerRecords()
{

}

int ServerRecords::FindPlayer(int _id)
{
	for (int i = 0; i < m_playerInfos.size(); i++)
	{
		if (m_playerInfos[i]->GetID() == _id)
		{
			return i;
		}
	}

	return -1;
}

int ServerRecords::GetNextPlayerID()
{
	return m_nextPlayerID;
}

int ServerRecords::GetNextPlayerIndex()
{
	if (m_nextPlayerID == 0)
	{
		return 0;
	}

	return m_playerInfos.size();
}

int ServerRecords::CreateNewPlayer()
{
	//Add new playerinfo to players vector, then to xml_node vector
	m_playerInfos.push_back(std::make_shared<PlayerInfo>(m_nextPlayerID));

	//Make a pointer to a new XML player node in m_lobbyInfo
	std::shared_ptr<pugi::xml_node> newPlayerNode = std::make_shared<pugi::xml_node>(m_lobbyInfo.child("ServerInfo").append_child("Player"));

	// Pass new playerInfo into XML document, and hook up the new node into m_playerXML
	pugi::xml_document playerNodeDoc;

	// I need to use m_playerInfos.size() - 1, as the next id may not be the player's position if a player leaves
	playerNodeDoc.load_string(m_playerInfos[m_playerInfos.size() - 1]->AsXMLString().c_str());

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
	int playerIndex = FindPlayer(_id);

	std::shared_ptr<PlayerInfo> deletedPlayer = m_playerInfos[playerIndex];
	m_lobbyInfo.first_child().remove_child(*m_playerXML[playerIndex].get());
	m_playerXML.erase(m_playerXML.begin() + playerIndex);
	m_playerInfos.erase(m_playerInfos.begin() + playerIndex);
}

void ServerRecords::ChangeAttribute(int _id, std::string _attributeName, std::string& _newValue)
{
	int playerIndex = FindPlayer(_id);

	m_playerInfos[playerIndex]->ChangeAttribute(_attributeName, _newValue);
	m_playerXML[playerIndex]->attribute(_attributeName.c_str()).set_value(_newValue.c_str());
}

void ServerRecords::LogEvent(pugi::xml_node& _eventXML)
{
	m_logDocument.append_copy(_eventXML);
	m_logDocument.save_file("./logs/serverlog.txt");
}

std::string ServerRecords::AsXMLString()
{
	std::stringstream ss;
	m_lobbyInfo.save(ss);
	std::string lobbyAsString = ss.str();
	printf("Lobby Info:\n%s", lobbyAsString.c_str());
	return lobbyAsString;
}