#include <string>
#include <sstream>
#include "FL/Fl_Box.H"
#include "FL/Fl_PNG_Image.H"
#include "Pugixml/pugixml.hpp"
#include "Player.h"
#include "PlayerInfo.h"
#include "Messagebox.h"
#include "ImagePool.h"

Player::Player() :
	Fl_Box(300, 400, 150, 150, ""),
	m_messageBox(),
	m_playerInfo(0)
{
	label(m_playerInfo.m_username.c_str());
	image(ImagePool::GetImage(m_playerInfo.m_shapeImage));
}

Player::Player(int _id) :
	Fl_Box(300, 400, 100, 100, ""),
	m_messageBox(),
	m_playerInfo(_id)
{
	label(m_playerInfo.m_username.c_str());
	image(ImagePool::GetImage(m_playerInfo.m_shapeImage));
}


Player::Player(PlayerInfo _info) :
	Fl_Box(300, 400, 100, 100, ""),
	m_messageBox(),
	m_playerInfo(_info)
{
	label(m_playerInfo.m_username.c_str());
	image(ImagePool::GetImage(m_playerInfo.m_shapeImage));
}

Player::~Player()
{

}

void Player::SetDestination(int _destX, int _destY)
{
	m_playerInfo.m_startingPosition[0] = x();
	m_playerInfo.m_startingPosition[1] = y();
	m_playerInfo.m_currentDestination[0] = _destX;
	m_playerInfo.m_currentDestination[1] = _destY;
	m_playerInfo.m_movementInterpolationStep = 0;
}

void Player::ChangeAttribute(std::string _attributeName, std::string _newValue)
{
	m_playerInfo.ChangeAttribute(_attributeName, _newValue);
}

void Player::ShowMessage(std::string _message, Fl_Window* _winToAdd)
{
	m_messageBox = new Messagebox(x(), y(), _message, _winToAdd);
	
	return m_messageBox->show();
}

void Player::OnTick()
{

	//Interpolate linearly from current position to destination
	if (m_playerInfo.m_movementInterpolationStep < 1)
	{
		int newX = m_playerInfo.m_startingPosition[0] + (m_playerInfo.m_movementInterpolationStep * (m_playerInfo.m_currentDestination[0] - m_playerInfo.m_startingPosition[0]));
		int newY = m_playerInfo.m_startingPosition[1] + (m_playerInfo.m_movementInterpolationStep * (m_playerInfo.m_currentDestination[1] - m_playerInfo.m_startingPosition[1]));
		position(newX, newY);


		m_playerInfo.m_movementInterpolationStep += 0.02;
	}
}

std::string Player::AsXMLString()
{
	return m_playerInfo.AsXMLString();
}