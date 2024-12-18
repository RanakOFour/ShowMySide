#include "ClientPlayer.h"
#include "Player.h"
#include "PlayerInfo.h"
#include "Pugixml/pugixml.hpp"

#include <memory>

ClientPlayer::ClientPlayer() :
	m_clientPlayer(nullptr)
{
}

ClientPlayer::~ClientPlayer()
{

}

void ClientPlayer::SetClientPlayer(std::shared_ptr<Player> _player)
{
	m_clientPlayer.swap(_player);
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
	return m_clientPlayer->GetID();
}

std::string ClientPlayer::GetUsername()
{
	return m_clientPlayer->GetUsername();
}