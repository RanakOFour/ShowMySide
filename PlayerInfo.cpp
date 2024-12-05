#include "PlayerInfo.h"
#include "ImagePool.h"
#include "Pugixml/pugixml.hpp"
#include <sstream>

PlayerInfo::PlayerInfo(int _id) :
	m_imageType(ImagePool::ImageType::TRIANGLE),
	m_username("Player " + std::to_string(_id)),
	m_id(_id)
{
	m_currentDestination[0] = 250;
	m_currentDestination[1] = 250;
	m_startingPosition[0] = 250;
	m_startingPosition[1] = 250;
	m_movementInterpolationStep = 0;
}

PlayerInfo::PlayerInfo(int _id, std::string& _username, int _destination[], int _current[], int _shape) :
	m_imageType((ImagePool::ImageType)_shape),
	m_id(_id),
	m_username(_username)
{
	m_currentDestination[0] = _destination[0];
	m_currentDestination[1] = _destination[1];
	m_startingPosition[0] = _current[0];
	m_startingPosition[1] = _current[1];
	m_movementInterpolationStep = 0;
}

PlayerInfo::~PlayerInfo()
{

}

void PlayerInfo::ChangeAttribute(std::string& _attributeName, std::string& _newValue)
{
	// Raw dog _newVal into playerinfo value matching _attributeName
	if (_attributeName == "shape")
	{
		m_imageType = (ImagePool::ImageType)atoi(_newValue.c_str());
	}
	else if (_attributeName == "destination")
	{
		m_currentDestination[0] = atoi(_newValue.substr(0, _newValue.find(',')).c_str());
		m_currentDestination[1] = atoi(_newValue.substr(_newValue.find(',') + 1, _newValue.size()).c_str());
		m_movementInterpolationStep = 0;
	}
	else if (_attributeName == "start")
	{
		m_startingPosition[0] = atoi(_newValue.substr(0, _newValue.find(',')).c_str());
		m_startingPosition[1] = atoi(_newValue.substr(_newValue.find(',')+ 1, _newValue.size()).c_str());
	}
	else if (_attributeName == "username")
	{
		m_username = _newValue;
	}
}

std::string PlayerInfo::AsXMLString()
{
	//Convert to <Player id, username, shape, Destination>
	pugi::xml_document playerDoc;
	pugi::xml_node playerNode = playerDoc.append_child("Player");
	playerNode.append_attribute("id").set_value(m_id);

	playerNode.append_attribute("username").set_value(m_username.c_str());
	playerNode.append_attribute("shape").set_value(m_imageType);

	//Don't judge
	playerNode.append_attribute("destination").set_value(std::string(std::to_string(m_currentDestination[0]) + "," + std::to_string(m_currentDestination[1])).c_str());
	playerNode.append_attribute("start").set_value(std::string(std::to_string(m_startingPosition[0]) + "," + std::to_string(m_startingPosition[1])).c_str());

	std::stringstream ss;
	playerDoc.save(ss);
	std::string playerNodeString = ss.str();
	return playerNodeString;
}