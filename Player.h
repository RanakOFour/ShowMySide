#include "FL/Fl_PNG_Image.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Group.H"
#include "PlayerInfo.h"
#include <string>

#pragma once
class Messagebox;

/**
 The player as represented inside the Client's Lobby. Numerical information is stored inside the PlayerInfo property, as a copy of it is kept on the Host's HeadlessLobby
 as the Host does not need all the FLTK information included here. So, I guess this is more a GUI wrapper for PlayerInfo.
*/
class Player : public Fl_Group
{
private:
	Messagebox* m_messageBox;
	Fl_Box* m_playerImage;
	PlayerInfo m_playerInfo;
public:

	/**
		Player constructor for creating new players locally / from a 'new_plr' event from Client
	*/
	Player(int _id);

	/**
		This constructor is used when initially creating the Lobby, as it gets a list of players as XML from the Host 
	*/
	Player(PlayerInfo _info);

	~Player();

	/**
		Sets m_currentDestination and updates m_startingPosition, then resets m_movementInterpolationStep for animating.
		These properties come from PlayerInfo.
	*/
	void SetDestination(int _x, int _y);

	/**
		Takes XML properties and passes along to PlayerInfo
	*/
	void ChangeAttribute(std::string _attributeName, std::string _newValue);

	/**
		Sets up Messagebox property to show given message
	*/
	void ShowMessage(std::string _message);

	/**
		Increments 
	*/
	void OnTick();

	/**
		Literally just calls PlayerInfo::AsXMLString(), as all the info is stored there
	*/
	std::string AsXMLString();
};

