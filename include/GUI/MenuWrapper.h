#include "FL/Fl_Group.H"
#include "FL/Fl_Menu_Bar.H"

#pragma once
class MainWindow;

/** An %Fl_Flex containing the menu bar, which is fixed vertically at 30 pixels. */
class MenuWrapper :
    public Fl_Group
{
private:
    Fl_Menu_Bar m_menuBar;

public:

    /** Gets called during MainWindow constructor, the MainWindow gets passed in to hook
    up events from the MainWindow to buttons in the MenuBar. */
    MenuWrapper(MainWindow* _mwWithEvents);
    ~MenuWrapper();

    void AddLobbyButton(MainWindow* _mw);
    void RemoveLobbyButton();
};

