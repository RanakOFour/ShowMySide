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
	m_playerInfo(_id)
{
	m_playerImage = new Fl_Box(0, 0, 100, 100, m_playerInfo.m_username.c_str());
	m_playerImage->label(m_playerInfo.m_username.c_str());
	m_playerImage->image(ImagePool::GetImage(m_playerInfo.m_shapeImage));
	m_messageBox = new Messagebox(x(), y());
}


Player::Player(PlayerInfo _info) :
	Fl_Group(300, 400, 100, 100, ""),
	m_messageBox(),
	m_playerImage(),
	m_playerInfo(_info)
{
	m_playerImage = new Fl_Box(0, 0, 100, 100, m_playerInfo.m_username.c_str());
	m_playerImage->label(m_playerInfo.m_username.c_str());
	m_playerImage->image(ImagePool::GetImage(m_playerInfo.m_shapeImage));
	m_messageBox = new Messagebox(x(), y());
}

Player::~Player()
{

}

void Player::SetDestination(int _destX, int _destY)
{
	m_playerInfo.m_startingPosition[0] = m_playerImage->x();
	m_playerInfo.m_startingPosition[1] = m_playerImage->y();
	m_playerInfo.m_currentDestination[0] = _destX - (w() / 2);
	m_playerInfo.m_currentDestination[1] = _destY - (h() / 2);
	m_playerInfo.m_movementInterpolationStep = 0;
}

void Player::ChangeAttribute(std::string _attributeName, std::string _newValue)
{
	m_playerInfo.ChangeAttribute(_attributeName, _newValue);
}

void Player::ShowMessage(std::string _message)
{
	m_messageBox->DisplayMessage(_message);
}

void Player::OnTick()
{

	//Interpolate linearly from current position to destination
	if (m_playerInfo.m_movementInterpolationStep < 1)
	{
		int newX = m_playerInfo.m_startingPosition[0] + (m_playerInfo.m_movementInterpolationStep * (m_playerInfo.m_currentDestination[0] - m_playerInfo.m_startingPosition[0]));
		int newY = m_playerInfo.m_startingPosition[1] + (m_playerInfo.m_movementInterpolationStep * (m_playerInfo.m_currentDestination[1] - m_playerInfo.m_startingPosition[1]));
		m_playerImage->position(newX, newY);
		m_messageBox->position(newX + 20, newY - 100);

		m_playerInfo.m_movementInterpolationStep += 0.01;
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