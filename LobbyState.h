#include <vector>
#include <string>

#pragma once

class Player;
class LobbyState
{
private:
	std::vector<Player*> m_Players;
public:
	LobbyState();
	~LobbyState();
	void AddPlayer(int _id);
	void RemovePlayer(int _id);
	void ProcessDelta(std::string _xmlString);
};

