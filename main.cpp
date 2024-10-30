#include <iostream>
#include "Wsa.h"
#include "InitWindow.h"

int main(int argc, char** argv)
{
	Fl::scheme("gtk+");
	Wsa wsa;
	printf("Initialized Winsock\n");

	InitWindow win(640, 360, "Test");
	win.show();

	return Fl::run();
}