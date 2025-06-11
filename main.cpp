#include "Wsa.h"
#include "GUI/MainWindow.h"
#include "GUI/ImagePool.h"
#include "Blowfish.h"
#include "FL/Fl.H"

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