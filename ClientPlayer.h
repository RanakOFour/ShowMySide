#include "Player.h"
#include "Pugixml/pugixml.hpp"

#include <memory>


#pragma once

class ClientPlayer
{
private:
	std::shared_ptr<Player> m_clientPlayer;

public:
	ClientPlayer();
	~ClientPlayer();

	/**
		Assigns m_clientPlayer
	*/
	void SetClientPlayer(std::shared_ptr<Player> _player);

	/**
		Takes in the mouse's coordinates and returns a move Event in the form
		<Event type="move" start="X,Y" destination="X,Y" id="id"></Event>
	*/
	pugi::xml_document CreateMovementEvent(int _x, int _y);

	int GetID();
	std::string GetUsername();
};

