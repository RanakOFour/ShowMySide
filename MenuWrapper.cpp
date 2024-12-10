#include "MenuWrapper.h"
#include "MainWindow.h"
#include "FL/Fl_Flex.H"

MenuWrapper::MenuWrapper(MainWindow* _mw) :
	Fl_Flex(0, 0, _mw->w(), 30, "")
{
	m_menuBar = new Fl_Menu_Bar(0, 0, w(), 30, "");
	m_menuBar->add("&Start/&Join Server", NULL, MainWindow::OnClientStart, _mw);
	m_menuBar->add("&Start/&Create Server", NULL, MainWindow::OnServerStart, _mw, FL_MENU_DIVIDER);
	m_menuBar->add("&Start/&Exit", NULL, MainWindow::OnExit, _mw);

	                            /*MainWindow::ShowAbout*/
	m_menuBar->add("&About", NULL, NULL, 0);
	
							   /*MainWindow::ShowHelp*/
	m_menuBar->add("&Help", NULL, NULL, 0);
	m_menuBar->show();

	fixed(m_menuBar, 30);
	end();
}

MenuWrapper::~MenuWrapper()
{

}