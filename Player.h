#include "FL/Fl_PNG_Image.H"
#include "FL/Fl_Box.H"
#include "PlayerInfo.h"
#include <string>

#pragma once
class Messagebox;
class Player : public Fl_Box
{
private:
	Messagebox* m_messageBox;
	PlayerInfo m_playerInfo;
public:
	Player();
	Player(int _id);
	Player(PlayerInfo _info);
	~Player();
	void SetDestination(int _x, int _y);
	void ChangeAttribute(std::string _attributeName, std::string _newValue);
	void ShowMessage(std::string _message, Fl_Window* _winToAdd);
	void OnTick();
	std::string AsXMLString();
};

