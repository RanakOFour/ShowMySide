#include <winsock2.h>

#pragma once
struct Wsa
{
public:
	Wsa();
	~Wsa();
private:
	WSADATA m_wsadata;
	Wsa(const Wsa& _copy);
	Wsa& operator=(const Wsa& _assign);
};