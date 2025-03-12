#if _WIN32
	#include <winsock2.h>
#else
	#define WSADATA int
#endif

#pragma once
struct Wsa
{
public:
	Wsa();
	~Wsa();
private:
	WSADATA m_wsadata;
	Wsa(const Wsa& _copy);
};