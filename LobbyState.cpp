#include "LobbyState.h"

LobbyState::LobbyState() :
	m_Players()
{
}

LobbyState::AddPlayer(int _id)
{
	Player newPlayer = new Player(_id);
}