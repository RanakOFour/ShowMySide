#include "PlayerInfo.h"
#include "Messagebox.h"

#include "FL/Fl_Group.H"
#include "FL/Fl_Box.H"

#include <string>
#include <memory>

#pragma once

class ClientPlayer;

/**
 The player as represented inside the Client's Lobby. Numerical information is stored inside the PlayerInfo property, as a copy of it is kept on the Server's Server
 as the Server does not need all the FLTK information included here. So, I guess this is more a GUI wrapper for PlayerInfo.
*/
class Player : public Fl_Group
{
	friend class ClientPlayer;
private:
	Messagebox m_messageBox;
	Fl_Box m_playerImage;
	PlayerInfo m_playerInfo;
	double m_movementStep;

public:

	/**
		Player constructor for creating new players locally as a result of a 'new_plr' event
	*/
	Player(int _id);

	/**
		This constructor is used when initially creating the Lobby, using XML info from server
	*/
	Player(PlayerInfo _info);


	~Player();


	/**
		Takes XML properties and passes along to PlayerInfo
	*/
	void ChangeAttribute(std::string& _attributeName, std::string& _newValue);

	/**
		Sets up Messagebox property to show given message
	*/
	void ShowMessage(std::string& _message);

	/**
		Increments movement position 
	*/
	void Update();

	/**
		Literally just calls PlayerInfo::AsXMLString(), as all the info is stored there
	*/
	std::string AsXMLString();

	std::string GetUsername();

	int GetID();
};

