#include "ClientSocket.h"
#include <ws2tcpip.h>
#include <string>
#include <memory>
#include <stdexcept>

ClientSocket::ClientSocket() : m_socket(INVALID_SOCKET)
{
	m_closed = false;
}

ClientSocket::~ClientSocket()
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
	}
}

bool ClientSocket::Receive(std::string& _message)
{
	char buffer[128] = { 0 };
	int bytes = ::recv(m_socket, buffer, sizeof(buffer) - 1, 0);
	if (bytes == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			throw std::runtime_error("Read failed");
		}
		return false;
	}
	else if (bytes == 0)
	{
		m_closed = true;
		return false;
	}
	_message = buffer;
	return true;
}

void ClientSocket::Send(const std::string& _message)
{
	int bytes = ::send(m_socket, _message.c_str(), _message.length(), 0);
	if (bytes <= 0)
	{
		throw std::runtime_error("Failed to send data");
	}
}

bool ClientSocket::Connect(std::string _connectInfo)
{
	addrinfo *result = nullptr,
		hints;

	std::string ipAddress = _connectInfo.substr(0, _connectInfo.find(":"));
	std::string portNum = _connectInfo.substr(_connectInfo.find(":"), _connectInfo.size() - ipAddress.size());

	printf("IP: ", ipAddress);
	printf("Port num: ", portNum);

	int addrResult = getaddrinfo(ipAddress.c_str(), portNum.c_str(), &hints, &result);
	if (addrResult != 0) {
		printf("getaddrinfo failed with error: %d\n", addrResult);
		WSACleanup();
		return 1;
	}
}