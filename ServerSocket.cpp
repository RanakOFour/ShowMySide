#include "ServerSocket.h"
#include "ClientSocket.h"
#include <ws2tcpip.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <memory>
#include <vector>

ServerSocket::ServerSocket(int _port)
	: m_socket(INVALID_SOCKET),
	m_clients()
{
	m_tickNum = 0;
	
	//Pre stuff
	addrinfo hints = { 0 };
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = AI_PASSIVE;
	addrinfo* result = NULL;

	//Store address info in result
	if (getaddrinfo(NULL, std::to_string(_port).c_str(), &hints, &result) != 0)
	{
		throw std::runtime_error("Failed to resolve server address or port");
	}

	//Create socket with address info
	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (m_socket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		throw std::runtime_error("Failed to create socket");
	}

	//Bind address to socket (give socket an ip address for others to connect to)
	if (bind(m_socket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
	{
		freeaddrinfo(result);
		throw std::runtime_error("Failed to bind socket");
	}

	//Open the socket so that it will listen to incoming signals
	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to listen on socket");
	}

	u_long mode = 1;
	if (ioctlsocket(m_socket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to set non-blocking");
	}

	printf("Successfully opened socket %d\n", (int)m_socket);

	freeaddrinfo(result);

	//Next we need to get the IP address of the host
	//Open a new socket
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock == -1) {
		std::cerr << "Could not socket\n";
	}

	sockaddr_in loopback;
	//loopbakc needs to have properties set
	loopback.sin_family = AF_INET;
	loopback.sin_addr.s_addr = 1337;   // can be any IP address
	loopback.sin_port = htons(9);      // using debug port

	//connect socket at loopback address
	if (connect(sock, reinterpret_cast<sockaddr*>(&loopback), sizeof(loopback)) == -1) {
		::closesocket(sock);
		std::cerr << "Could not connect\n";
	}

	// Get socket information from loopback
	socklen_t addrlen = sizeof(loopback);
	if (getsockname(sock, reinterpret_cast<sockaddr*>(&loopback), &addrlen) == -1) {
		::closesocket(sock);
		std::cerr << "Could not getsockname\n";
	}

	// Socket is no longer needed
	::closesocket(sock);

	// Get the ip address from loopback's socket information
	char buffer[22];
	if (inet_ntop(AF_INET, &loopback.sin_addr, buffer, INET_ADDRSTRLEN) == 0x0) {
		std::cerr << "Could not inet_ntop\n";
	}
	m_ipAddress = buffer;

	printf("IP: %s\n", m_ipAddress.c_str());
}

ServerSocket::~ServerSocket()
{
	closesocket(m_socket);
}

std::shared_ptr<ClientSocket> ServerSocket::accept()
{
	SOCKET socket = ::accept(m_socket, NULL, NULL);
	if (socket == INVALID_SOCKET)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			throw std::runtime_error("Failed to accept socket");
		}
		return std::shared_ptr<ClientSocket>(); // Equivalent to NULL
	}
	std::shared_ptr<ClientSocket> rtn = std::make_shared<ClientSocket>();
	rtn->m_socket = socket;
	return rtn;
}

void ServerSocket::on_tick()
{
	std::shared_ptr<ClientSocket> client = accept();
	if (client)
	{
		printf("Client Connected!\n");
		m_clients.push_back(client);
	}

	for (size_t ci = 0; ci < m_clients.size(); ++ci)
	{
		std::string message;
		while (m_clients.at(ci)->Receive(message))
		{
			printf("Message recieved: %s\n", message.c_str());

			if (message.c_str() == "CLOSED")
			{
				m_clients.at(ci)->m_closed = true;
			}
		}
		if (m_clients.at(ci)->m_closed)
		{
			printf("Client Disconnected\n");
			m_clients.erase(m_clients.begin() + ci);
			--ci;
		}
	}

	printf("Server Tick %d\n", m_tickNum);
	++m_tickNum;
}
