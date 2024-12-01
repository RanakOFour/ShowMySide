#include "FL/Fl_RGB_Image.H"
#include <string>

#pragma once
class Player
{
private:
	Fl_RGB_Image *m_ShapeImage;
	std::string m_Name;
	std::string m_CurrentMessage;
	int m_playerNum;
	int m_currentTrajectory[2];

public:
	Player();
	~Player();
	void ProcessDelta(std::string _delta);
	void OnTick();
};

