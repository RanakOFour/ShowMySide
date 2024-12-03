#include "FL/Fl_PNG_Image.H"
#include "Pugixml/pugixml.hpp"
#include "ImagePool.h"
#include <string>

#pragma once
class PlayerInfo
{
	friend class Player;
private:
	ImagePool::ImageType m_shapeImage;
	std::string m_username;
	int m_id;
	int m_currentDestination[2];
	int m_startingPosition[2];
	double m_movementInterpolationStep;

public:
	PlayerInfo(int _id);
	PlayerInfo(int _id, std::string _username, int _destination[], int _current[], int _shape);
	~PlayerInfo();
	void ChangeAttribute(std::string _attributeName, std::string _newValue);
	int Id();
	std::string AsXMLString();
};

