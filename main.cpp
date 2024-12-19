#include "Wsa.h"
#include "MainWindow.h"
#include "ImagePool.h"
#include "Blowfish.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
	Fl::scheme("gtk+");
	Wsa wsa;
	printf("Initialized Winsock\n");

	ImagePool::Initialise();

	Blowfish::Initialise();

	MainWindow win(640, 360, "ShowMySide");
	win.show();

	return Fl::run();
}