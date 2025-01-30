#include "Player.h"
#include "pugixml.hpp"

#include <memory>


#pragma once

class ClientPlayer
{
private:
	std::shared_ptr<Player> m_clientPlayer;

public:
	ClientPlayer(int _id);
	~ClientPlayer();

	/**
		Takes in the mouse's coordinates and returns a move Event in the form
		<Event type="move" start="X,Y" destination="X,Y" id="id"></Event>
	*/
	pugi::xml_document CreateMovementEvent(int _x, int _y);

	int GetID();
	std::string GetUsername();
	std::shared_ptr<Player> GetPlayer();
};

