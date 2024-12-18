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
	m_clientPlayer.reset(_player.get());
}

pugi::xml_document ClientPlayer::CreateMovementEvent(int _x, int _y)
{
	// <Event type="attr_change", attribute="start", value=playerImageX,playerImageY, id=m_Id>
	// <Event type="attr_change", attribute="destination", value=_mouseX,_mouseY, id=m_Id>

	std::string clientStart = std::to_string(m_clientPlayer->m_playerImage.x()) + "," + std::to_string(m_clientPlayer->m_playerImage.y());
	std::string clientDestination = std::to_string(_x - (m_clientPlayer->w() / 2)) + "," + std::to_string(_y - (m_clientPlayer->w() / 2));
	pugi::xml_document newEvent;

	pugi::xml_node startNode = newEvent.append_child("Event");
	startNode.append_attribute("type").set_value("attr_change");
	startNode.append_attribute("attribute").set_value("start");
	startNode.append_attribute("value").set_value(clientStart.c_str());
	startNode.append_attribute("id").set_value(m_clientPlayer->m_playerInfo.GetID());

	pugi::xml_node destinationNode = newEvent.append_child("Event");
	destinationNode.append_attribute("type").set_value("attr_change");
	destinationNode.append_attribute("attribute").set_value("destination");
	destinationNode.append_attribute("value").set_value(clientDestination.c_str());
	destinationNode.append_attribute("id").set_value(m_clientPlayer->m_playerInfo.GetID());

	return newEvent;
}

int ClientPlayer::GetID()
{
	return m_clientPlayer->GetID();
}