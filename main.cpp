#include <iostream>
#include "InitWindow.h"

int main(int argc, char** argv)
{
	Fl::scheme("gtk+");

	InitWindow win(640, 360, "Test");
	win.show();

	return Fl::run();
}