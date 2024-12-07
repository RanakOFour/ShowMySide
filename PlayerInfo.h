#include "ImagePool.h"
#include "Pugixml/pugixml.hpp"
#include "FL/Fl_PNG_Image.H"
#include <string>

#pragma once
class PlayerInfo
{
	friend class Player;
private:
	/**
		We want to store the type of image it is, as that's better XML information than having to query what image is being displayed and working backwards
	*/
	ImagePool::ImageType m_imageType;
	std::string m_username;
	int m_id;
	int m_currentDestination[2];
	int m_startingPosition[2];

public:
	/**
		Used in HeadlessLobby and Lobby when instantiating a new 'default' player with default settings
	*/
	PlayerInfo(int _id);

	/**
		Used in Player when instantiating from XML based information
	*/
	PlayerInfo(int _id, std::string& _username, int _destination[], int _current[], int _shape);
	~PlayerInfo();

	/**
		Takes XML attribute and value, and does exactly what it says on the tin
	*/
	void ChangeAttribute(std::string& _attributeName, std::string& _newValue);

	/**
		Loads properties into an pugi::xml_document object, then saves to a string.
		The information is stored as below:
		<Player id, username, destination, start, shape>

		Interpolation step is not carried over, as it is not information important to the server
	*/
	std::string AsXMLString();
};
