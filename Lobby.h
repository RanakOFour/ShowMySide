#include <vector>
#include "FL/Fl_Double_Window.H"
#include "Pugixml/pugixml.hpp"
#include "Timer.h"

class Player;
class PlayerInfo;
class Lobby :
    public Fl_Double_Window
{
private:
    std::vector<std::string> m_events;
    std::vector<Player*> m_players;
    Player* m_clientPlayer;

    //Override Fl_Widget::handle() for mouse and keyboard inputs
    int handle(int _event);
    void LogEvent(const char* _type, void* _data);

public:
    Lobby(std::string _docToLoad);
    ~Lobby();
    void OnTick();
    std::vector<std::string> FlushEvents();
    void CreateNewPlayer(int _id);
    void RemovePlayer(int _id);
    void ChangeAttribute(int _id, std::string _attributeName, std::string _newValue);
    void ShowMessage(int _id, std::string _message);
};

