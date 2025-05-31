#include "Wsa.h"
#include <stdexcept>
Wsa::Wsa()
{
	#if _WIN32
		if (WSAStartup(MAKEWORD(2, 2), &m_wsadata) != 0)
		{
			throw std::runtime_error("Failed to initialize Winsock");
		}
	#endif
}
Wsa::~Wsa()
{
	#if _WIN32
		WSACleanup();
	#endif
}
