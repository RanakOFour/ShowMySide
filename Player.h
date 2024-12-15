#include "PlayerInfo.h"
#include "FL/Fl_PNG_Image.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Group.H"
#include <string>

#pragma once
class Messagebox;

/**
 The player as represented inside the Client's Lobby. Numerical information is stored inside the PlayerInfo property, as a copy of it is kept on the Server's Server
 as the Server does not need all the FLTK information included here. So, I guess this is more a GUI wrapper for PlayerInfo.
*/
class Player : public Fl_Group
{
private:
	Messagebox* m_messageBox;
	Fl_Box* m_playerImage;
	PlayerInfo m_playerInfo;
	double m_movementStep;

public:

	/**
		Player constructor for creating new players locally / from a 'new_plr' event from Client
	*/
	Player(int _id);

	/**
		This constructor is used when initially creating the Lobby, as it gets a list of players as XML from the Server 
	*/
	Player(PlayerInfo _info);

	~Player();


	/**
		Takes XML properties and passes along to PlayerInfo
	*/
	void ChangeAttribute(std::string& _attributeName, std::string& _newValue);


	/**
		Returns two XML events to change the startingPosition and currentDestination properties of PlayerInfo.
		No actual data in PlayerInfo needs to change as the client will recieve the event from the server aswell
	*/
	pugi::xml_document CreateMovementEvent(int _x, int _y);

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

