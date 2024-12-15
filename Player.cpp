#include <string>
#include <sstream>
#include "FL/Fl_Box.H"
#include "FL/Fl_PNG_Image.H"
#include "FL/Fl_Group.H"
#include "FL/Fl_Box.H"
#include "Pugixml/pugixml.hpp"
#include "Player.h"
#include "PlayerInfo.h"
#include "Messagebox.h"
#include "ImagePool.h"

Player::Player(int _id) :
	Fl_Group(300, 400, 100, 100, ""),
	m_messageBox(),
	m_playerImage(),
	m_playerInfo(_id),
	m_movementStep(0)

{
	m_playerImage = new Fl_Box(0, 0, 100, 100, m_playerInfo.m_username.c_str());
	m_playerImage->label(m_playerInfo.m_username.c_str());
	m_playerImage->image(ImagePool::GetImage(m_playerInfo.m_imageType).get());
	m_messageBox = new Messagebox(x(), y());
}


Player::Player(PlayerInfo _info) :
	Fl_Group(300, 400, 100, 100, ""),
	m_messageBox(),
	m_playerImage(),
	m_playerInfo(_info),
	m_movementStep(0)
{
	m_playerImage = new Fl_Box(0, 0, 100, 100, m_playerInfo.m_username.c_str());
	m_playerImage->label(m_playerInfo.m_username.c_str());
	m_playerImage->image(ImagePool::GetImage(m_playerInfo.m_imageType).get());
	m_messageBox = new Messagebox(x(), y());
}

Player::~Player()
{

}

void Player::ChangeAttribute(std::string& _attributeName, std::string& _newValue)
{
	// Edit playerinfo, then apply change to GUI player
	m_playerInfo.ChangeAttribute(_attributeName, _newValue);

	if (_attributeName == "shape")
	{
		m_playerImage->image(ImagePool::GetImage((ImageType)atoi(_newValue.c_str())).get());
	}
	else if (_attributeName == "username")
	{
		m_playerImage->label(m_playerInfo.m_username.c_str());
	}
	else if (_attributeName == "start")
	{
		m_movementStep = 0;
	}
}

pugi::xml_document Player::CreateMovementEvent(int _destX, int _destY)
{
	//We don't need to actually change any values, because they will change when we recieve these events from the server later
	//The movement step is not accounted for inside of PlayerInfo, so it is set here


	// <Event type="attr_change", attribute="start", value=playerImageX,playerImageY, id=m_Id>
	// <Event type="attr_change", attribute="destination", value=_mouseX,_mouseY, id=m_Id>
	pugi::xml_document newEvent;

	pugi::xml_node startNode = newEvent.append_child("Event");
	startNode.append_attribute("type").set_value("attr_change");
	startNode.append_attribute("attribute").set_value("start");
	startNode.append_attribute("value").set_value(std::string(std::to_string(m_playerImage->x()) + "," + std::to_string(m_playerImage->y())).c_str());
	startNode.append_attribute("id").set_value(m_playerInfo.m_id);

	pugi::xml_node destinationNode = newEvent.append_child("Event");
	destinationNode.append_attribute("type").set_value("attr_change");
	destinationNode.append_attribute("attribute").set_value("destination");
	destinationNode.append_attribute("value").set_value(std::string(std::to_string(_destX - (w() / 2)) + "," + std::to_string(_destY - (h() / 2))).c_str());
	destinationNode.append_attribute("id").set_value(m_playerInfo.m_id);

	return newEvent;
}

void Player::ShowMessage(std::string& _message)
{
	m_messageBox->DisplayMessage(_message);
}

void Player::Update()
{

	//Interpolate linearly from current position to destination
	if (m_movementStep < 1)
	{
		int newX = m_playerInfo.m_startingPosition[0] + (m_movementStep * (m_playerInfo.m_currentDestination[0] - m_playerInfo.m_startingPosition[0]));
		int newY = m_playerInfo.m_startingPosition[1] + (m_movementStep * (m_playerInfo.m_currentDestination[1] - m_playerInfo.m_startingPosition[1]));
		m_playerImage->position(newX, newY);
		m_messageBox->position(newX + 20, newY - 100);

		m_movementStep += 0.01;

		//printf("Current player position: %d, %d\n", m_playerImage->x(), m_playerImage->y());
	}
	else
	{
		m_playerInfo.m_startingPosition[0] = m_playerInfo.m_currentDestination[0];
		m_playerInfo.m_startingPosition[1] = m_playerInfo.m_currentDestination[1];
	}
}

std::string Player::AsXMLString()
{
	return m_playerInfo.AsXMLString();
}

std::string Player::GetUsername()
{
	return m_playerInfo.m_username;
}

int Player::GetID()
{
	return m_playerInfo.m_id;
}