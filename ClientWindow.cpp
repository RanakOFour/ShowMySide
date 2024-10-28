#include "ClientWindow.h"
#include <string>
#include <FL/Fl_Window.H>
#include <FL/Fl_Flex.H>

ClientWindow::ClientWindow(int x, int y, int w, int h, std::string name, bool hasServer) : Fl_Window(x, y, w, h, name.c_str())
{

}

ClientWindow::~ClientWindow()
{

}