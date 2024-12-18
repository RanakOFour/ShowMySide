#include "MenuWrapper.h"
#include "MainWindow.h"
#include "FL/Fl_Flex.H"

#include <memory>

MenuWrapper::MenuWrapper(MainWindow* _mw) :
	Fl_Flex(0, 0, _mw->w(), 30, "")
{
	m_menuBar = std::make_shared<Fl_Menu_Bar>(0, 0, w(), 30, "");
	m_menuBar->add("&Start/&Join Server", NULL, MainWindow::OnClientStart, _mw);
	m_menuBar->add("&Start/&Create Server", NULL, MainWindow::OnServerStart, _mw, FL_MENU_DIVIDER);
	m_menuBar->add("&Start/&Exit", NULL, MainWindow::OnExit, _mw);


	m_menuBar->add("&About", NULL, MainWindow::ShowAbout, _mw);

	m_menuBar->show();

	fixed(m_menuBar.get(), 30);
	end();
}

MenuWrapper::~MenuWrapper()
{

}