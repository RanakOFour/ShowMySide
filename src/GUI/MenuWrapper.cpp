#include "GUI/MenuWrapper.h"
#include "GUI/MainWindow.h"
#include "FL/Fl_Group.H"

MenuWrapper::MenuWrapper(MainWindow* _mw) :
	Fl_Group(0, 0, _mw->w(), 30, ""),
	m_menuBar(0, 0, w(), 30, "")
{
	m_menuBar.add("&Start/&Join Server", 0, MainWindow::OnClientStart, _mw);
	m_menuBar.add("&Start/&Create Server", 0, MainWindow::OnServerStart, _mw, FL_MENU_DIVIDER);
	m_menuBar.add("&Start/&Exit", 0, MainWindow::OnExit, _mw);


	m_menuBar.add("&About", 0, MainWindow::ShowAbout, _mw);

	m_menuBar.show();
	end();
}

MenuWrapper::~MenuWrapper()
{

}
