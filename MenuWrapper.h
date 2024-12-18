#include "FL/Fl_Flex.H"
#include "FL/Fl_Menu_Bar.H"

#include <memory>

#pragma once
class MainWindow;

/**
    An fl_flex containing the menu bar, which is fixed vertically at 30 pixels.
*/
class MenuWrapper :
    public Fl_Flex
{
private:
    std::shared_ptr<Fl_Menu_Bar> m_menuBar;

public:
    MenuWrapper(MainWindow* _mwWithEvents);
    ~MenuWrapper();
};

