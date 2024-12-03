#include <iostream>
#include "Wsa.h"
#include "MainWindow.h"
#include "ImagePool.h"

int main(int argc, char** argv)
{
	Fl::scheme("gtk+");
	Wsa wsa;
	printf("Initialized Winsock\n");

	ImagePool::Initialise();
	printf("Initialsed image pool\n");

	MainWindow win(640, 360, "Thingamabob");
	win.show();

	return Fl::run();
}