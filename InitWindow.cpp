#include "InitWindow.h"
#include "ThirdsColumn.h"
#include <string>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Flex.H>

InitWindow::InitWindow(int w, int h, std::string name) : Fl_Window(w, h, name.c_str())
{
	int thirdW = w / 3;
	resizable(*this);

	Fl_Button* clientButton = new Fl_Button(thirdW - 75, h - (h / 3) - 20, 150, 40, "Start Client");
	Fl_Button* serverButton = new Fl_Button((thirdW * 2) - 75, h - (h / 3) - 20, 150, 40, "Start Server + Client");
	add(*clientButton);
	add(*serverButton);

	clientButton->callback(StartClient);
	serverButton->callback(StartServer);
	
}

InitWindow::~InitWindow()
{

}

void InitWindow::StartClient(Fl_Widget* widget, void* data)
{
	std::printf("Starting client \n");
}

void InitWindow::StartServer(Fl_Widget* widget, void* data)
{
	std::printf("Starting server... \n");

	//Server will also run a client
	StartClient(widget, (void*)0);
}