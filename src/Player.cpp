#include "Player.h"
#include "PlayerInfo.h"
#include "Messagebox.h"
#include "ImagePool.h"
#include "pugixml.hpp"

#include "FL/Fl_Box.H"
#include "FL/Fl_PNG_Image.H"
#include "FL/Fl_Group.H"
#include "FL/Fl_Box.H"

#include <string>

Player::Player(int _id) :
	Fl_Group(300, 400, 100, 100, ""),
	m_messageBox(x(), y()),
	m_playerImage(0, 0, 100, 100, m_playerInfo.m_username.c_str()),
	m_playerInfo(_id),
	m_movementStep(0)

{
	m_playerImage.label(m_playerInfo.m_username.c_str());
	m_playerImage.image(ImagePool::GetImage(m_playerInfo.m_imageType).get());
	end();
}


Player::Player(PlayerInfo _info) :
	Fl_Group(300, 400, 100, 100, ""),
	m_messageBox(x(), y()),
	m_playerImage(0, 0, 100, 100, m_playerInfo.m_username.c_str()),
	m_playerInfo(_info),
	m_movementStep(0)
{
	m_playerImage.label(m_playerInfo.m_username.c_str());
	m_playerImage.image(ImagePool::GetImage(m_playerInfo.m_imageType).get());
	end();
}

Player::~Player()
{

}

void Player::ChangeAttribute(std::string& _attributeName, std::string& _newValue)
{
	// Edit playerinfo, then apply change to GUI player
	m_playerInfo.ChangeAttribute(_attributeName, _newValue);

	// No changes needed for destination, as m_movementStep is cahgned with start
	if (_attributeName == "shape")
	{
		m_playerImage.image(ImagePool::GetImage((ImageType)atoi(_newValue.c_str())).get());
	}
	else if (_attributeName == "start")
	{
		m_movementStep = 0;
	}
	else if (_attributeName == "username")
	{
		m_playerImage.label(m_playerInfo.m_username.c_str());
	}
}

void Player::ShowMessage(std::string& _message)
{
	m_messageBox.DisplayMessage(_message);
}

void Player::Update()
{
	if (m_movementStep < 1)
	{
		//Interpolate linearly from current position to destination
		int newX = m_playerInfo.m_startingPosition[0] + (m_movementStep * (m_playerInfo.m_currentDestination[0] - m_playerInfo.m_startingPosition[0]));
		int newY = m_playerInfo.m_startingPosition[1] + (m_movementStep * (m_playerInfo.m_currentDestination[1] - m_playerInfo.m_startingPosition[1]));

		m_playerImage.position(newX, newY);
		m_messageBox.position(newX + 20, newY - 100);

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