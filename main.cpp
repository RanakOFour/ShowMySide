#include "Wsa.h"
#include "MainWindow.h"
#include "ImagePool.h"
#include <iostream>

int main(int argc, char** argv)
{
	Fl::scheme("gtk+");
	Wsa wsa;
	printf("Initialized Winsock\n");

	ImagePool::Initialise();
	printf("Initialsed image pool\n");

	MainWindow win(640, 360, "ShowMySide");
	win.show();

	return Fl::run();
}