#include "include/MainWindow.h"
#include "include/ImagePool.h"
#include "include/Blowfish.h"
#include <iostream>

#include <string>

int main(int argc, char** argv)
{
	initialize_sockets()
	Fl::scheme("gtk+");
	ImagePool::Initialise();

	Blowfish::Initialise();

	MainWindow win(640, 360, "ShowMySide");
	win.show();

	return Fl::run();
}

int initialize_sockets() {
#ifdef _WIN32
    WSADATA wsaData;
	print("Winsock init\n");
    return WSAStartup(MAKEWORD(2, 2), &wsaData);
#else
    return 0; // No initialization required on Linux
#endif
}

void cleanup_sockets() {
#ifdef _WIN32
    WSACleanup();
#endif
}