#include "Network/ClientPlayer.h"
#include "Network/Player.h"
#include "Network/PlayerInfo.h"
#include "pugixml.hpp"

#include <memory>

ClientPlayer::ClientPlayer(int _id) :
	m_clientPlayer(nullptr)
{
	m_clientPlayer = std::make_shared<Player>(_id);
}

ClientPlayer::~ClientPlayer()
{

}

pugi::xml_document ClientPlayer::CreateMovementEvent(int _x, int _y)
{
	// <Event type="attr_change", attribute="start", value=playerImageX,playerImageY, id=m_Id>
	// <Event type="attr_change", attribute="destination", value=_mouseX,_mouseY, id=m_Id>

	std::string clientStart = std::to_string(m_clientPlayer->m_playerImage.x()) + "," + std::to_string(m_clientPlayer->m_playerImage.y());
	std::string clientDestination = std::to_string(_x - m_clientPlayer->w()) + "," + std::to_string(_y - m_clientPlayer->h());
	pugi::xml_document newEvent;

	pugi::xml_node eventNode = newEvent.append_child("Event");
	eventNode.append_attribute("type").set_value("move");
	eventNode.append_attribute("start").set_value(clientStart.c_str());
	eventNode.append_attribute("destination").set_value(clientDestination.c_str());
	eventNode.append_attribute("id").set_value(m_clientPlayer->m_playerInfo.GetID());

	return newEvent;
}

int ClientPlayer::GetID()
{
	if (m_clientPlayer == nullptr)
	{
		return -1;
	}

	return m_clientPlayer->GetID();
}

std::string ClientPlayer::GetUsername()
{
	if (m_clientPlayer == nullptr)
	{
		return "";
	}

	return m_clientPlayer->GetUsername();
}

std::shared_ptr<Player> ClientPlayer::GetPlayer()
{

	std::shared_ptr<Player> playerPtr = std::shared_ptr<Player>(m_clientPlayer);
	return playerPtr;
}
