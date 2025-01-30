#include "MainWindow.h"
#include "ImagePool.h"
#include "Blowfish.h"
#include "Wsa.h"
#include <iostream>

#include <string>

int main(int argc, char** argv)
{
	Wsa::Wsa();
	Fl::scheme("gtk+");
	ImagePool::Initialise();

	Blowfish::Initialise();

	MainWindow win(640, 360, "ShowMySide");
	win.show();

	return Fl::run();
}