#include "Timer.h"


class LobbyState;
class Lobby :
    public Timer
{
private:
    LobbyState* m_lobbyState;
public:
    Lobby();
    ~Lobby();
    void OnTick(void* _userData);

    int m_playerCount;
};

