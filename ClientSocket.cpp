#include "ClientSocket.h"
#include "Blowfish.h"

#include <iostream>
#include <ws2tcpip.h>
#include <string>
#include <memory>
#include <stdexcept>

ClientSocket::ClientSocket() : 
	m_socket(INVALID_SOCKET),
	m_id(-1)
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

bool ClientSocket::Connect(std::string& _serverName)
{
	addrinfo* result = nullptr,
		* ptr = nullptr,
		hints{ 0 };
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = AI_PASSIVE;

	//We are SO BACK!!!
	int addrResult = getaddrinfo(_serverName.c_str(), "8080", &hints, &result);
	if (addrResult != 0) {
		printf("getaddrinfo failed with error: %d\n", addrResult);
		WSACleanup();
		return false;
	}

	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (m_socket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	int iResult = connect(m_socket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("Invalid iResult!\n");
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}

	u_long mode = 1;
	if (ioctlsocket(m_socket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to set non-blocking");
	}

	freeaddrinfo(result);

	if (m_socket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		return false;
	}

	return true;
}

void ClientSocket::Send(std::string& _message)
{
	std::vector<char> encryptedXML;
	Blowfish::Encrypt(_message, encryptedXML);
	std::string messageToSend = std::string(encryptedXML.begin(), encryptedXML.end());
	//printf("Encrypt: %s\n", messageToSend.c_str());

	int bytes = ::send(m_socket, messageToSend.c_str(), messageToSend.length(), 0);
	if (bytes <= 0)
	{
		throw std::runtime_error("Failed to send data");
	}
	else
	{
		//printf("Bytes sent on socket %d: %d\n", (int)m_socket, bytes);
	}
}

bool ClientSocket::Receive(std::string& _message)
{
	if (m_closed)
	{
		return false;
	}

	char buffer[256] = { 0 };
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

	std::string encrypedMessage = std::string(buffer, bytes);

	std::vector<char> decryptedChar;
	Blowfish::Decrypt(encrypedMessage, decryptedChar);
	_message = std::string(decryptedChar.begin(), decryptedChar.end());
	//printf("Decrypt Pre: %s\nDecrypt Post: %s\n", encrypedMessage.c_str(), _message.c_str());

	return true;
}

void ClientSocket::CloseConnection()
{
	int result = closesocket(m_socket);

	if (result == 0)
	{
		printf("Connection with server terminated\n");
	}

	m_closed = true;
}

void ClientSocket::SetID(int _id)
{
	m_id = _id;
}

int ClientSocket::GetID()
{
	return m_id;
}

bool ClientSocket::Closed()
{
	return m_closed;
}
