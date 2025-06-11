#include "pugixml.hpp"

#include <vector>
#include <memory>

#pragma once

class PlayerInfo;

class ServerRecords
{
private:
	pugi::xml_document m_logDocument;
	pugi::xml_document m_lobbyInfo;
	std::vector<std::shared_ptr<pugi::xml_node>> m_playerXML;
	std::vector<std::shared_ptr<PlayerInfo>> m_playerInfos;
	int m_nextPlayerID;

public:
	ServerRecords();
	~ServerRecords();

	/**
		Loops through m_players to find player with matching id and returns index in list.
        If the id is not found, return -1
	*/
	int FindPlayer(int _id);

	/**
		Returns m_nextPlayerID
	*/
	int GetNextPlayerID();

	/**
		Returns size of m_playerInfos
	*/
	int GetNextPlayerIndex();

	/**
		Generates default PlayerInfo object, then inserts it into m_players and an xml copy into m_lobbyInfo, and references into m_playerXML
	*/
	int CreateNewPlayer();

	/**
		Deletes specified player from all sources
	*/
	void RemovePlayer(int _id);

	/**
		Takes XML values and applies changes
	*/
	void ChangeAttribute(int _id, std::string _attributeName, std::string& _newValue);
	
	/**
		Add eventXML into m_logDocument
	*/
	void LogEvent(pugi::xml_node& _eventXML);

	std::string AsXMLString();
};

