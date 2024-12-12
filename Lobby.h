#include "Timer.h"
#include "Pugixml/pugixml.hpp"
#include "FL/Fl_Double_Window.H"
#include <string>
#include <vector>
#include <memory>

class Player;
class Chatbox;
class Lobby :
    public Fl_Double_Window
{
private:
    pugi::xml_document m_events;
    std::vector<std::shared_ptr<Player>> m_players;
    std::shared_ptr<Player> m_clientPlayer;
    Chatbox* m_chatBox;
    std::string m_textFromChatbox;

    /**
        Overrides Fl_Widget::handle() so that m_clientPlayer's destination will update on a player's click. Will also open the ChatBox when pressing the 't' key.
    */
    int handle(int _event);

    /**
        Specific function for mouse events to make handle overload cleaner to look at
    */
    void HandleMouseEvent(int _mouseButton);

    /**
        Specific function for keyboard events to escape throwing a bunch of code into a single case statement of a switch
    */
    void HandleKeyboardEvent(int _key);

public:
    /**
        _docToLoad is an XML document listing all the players in the lobby on join
    */
    Lobby(std::string& _docToLoad);
    ~Lobby();

    /**
        Takes any new events in m_events, and copies out the information into _document before deleting any nodes in m_events.
    */
    void FlushEvents(pugi::xml_document& _document);

    /*
        Updates player positions. That's all it does right now.
    */
    void Update();

    /**
        Handles new_plr events from the Client
    */
    std::shared_ptr<Player> CreateNewPlayer(int _id);

    /**
        Handles plr_leave events from the Client
    */
    std::shared_ptr<Player> RemovePlayer(int _id);

    /**
        Handles attr_change events from the Client
    */
    void ChangeAttribute(int _id, std::string& _attributeName, std::string& _newValue);

    /**
        Handles new_message events from the Client
    */
    void ShowMessage(int _id, std::string& _message);

    std::string GetUsername(int _id);
};

