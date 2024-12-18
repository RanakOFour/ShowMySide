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

	void SetClientPlayer(std::shared_ptr<Player> _player);

	pugi::xml_document CreateMovementEvent(int _x, int _y);

	int GetID();
	std::string GetUsername();
};

