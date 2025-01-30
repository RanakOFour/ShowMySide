#include "ServerSocket.h"
#include "ClientSocket.h"
#include "pugixml.hpp"


#include <stdexcept>
#include <sstream>
#include <string>
#include <memory>
#include <vector>

ServerSocket::ServerSocket(int _port)
	: m_socket(INVALID_SOCKET),
	m_clients()
{
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

	//Set non-blocking
	u_long mode = 1;
	if (ioctlsocket(m_socket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to set non-blocking");
	}

	//printf("Successfully opened socket %d\n", (int)m_socket);

	freeaddrinfo(result);

	//Next  to get the IP address of the Server another socket is opened and probed for it's IPv4 address
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock == -1) {
		throw std::runtime_error("Could not socket\n");
	}

	sockaddr_in loopback;
	//loopback needs to have properties set
	loopback.sin_family = AF_INET;
	loopback.sin_addr.s_addr = 1337;
	
	// using whatever port
	loopback.sin_port = htons(9);

	//connect socket at loopback address
	if (connect(sock, reinterpret_cast<sockaddr*>(&loopback), sizeof(loopback)) == -1)
	{
		closesocket(sock);
		throw std::runtime_error("Could not connect\n");
	}

	// Get socket information from loopback
	socklen_t addrlen = sizeof(loopback);
	if (getsockname(sock, reinterpret_cast<sockaddr*>(&loopback), &addrlen) == -1)
	{
		closesocket(sock);
		throw std::runtime_error("Could not getsockname\n");
	}

	// Socket is no longer needed
	closesocket(sock);

	// Get the ip address from loopback's socket information
	char buffer[22];
	if (inet_ntop(AF_INET, &loopback.sin_addr, buffer, INET_ADDRSTRLEN) == 0x0)
	{
		throw std::runtime_error("Could not inet_ntop\n");
	}

	m_ipAddress = buffer;

	//printf("IP: %s\n", m_ipAddress.c_str());
}

ServerSocket::~ServerSocket()
{
	closesocket(m_socket);
}

std::shared_ptr<ClientSocket> ServerSocket::Accept()
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

pugi::xml_document ServerSocket::Update()
{
	//create document to repackage messages into and return
	pugi::xml_document clientEventList;
	pugi::xml_node clientEventsParent = clientEventList.append_child("Events");

	//Accept any new connections and add them to m_clients, tell Server to add them to Lobby
	std::shared_ptr<ClientSocket> client = Accept();

	if (client)
	{
		//printf("Client Connected!\n");

		m_clients.push_back(client);
	}


	for (size_t ci = 0; ci < m_clients.size(); ++ci)
	{
		//Close connection if nothing
		if (m_clients.at(ci)->m_closed)
		{
			//printf("Client Disconnected\n");
			m_clients.erase(m_clients.begin() + ci);
			--ci;
			continue;
		}


		//recieved messages are put into here
		std::string clientMessageAsString;

		m_clients.at(ci)->Receive(clientMessageAsString);
		

		if(clientMessageAsString != "")
		{
			//printf("Message recieved as server: %s\n", clientMessageAsString.c_str());


			//Parse client message into xml and pass up to Server
			pugi::xml_document currentEventDoc;

			//Parses document
			pugi::xml_parse_result clientMessageResult = currentEventDoc.load_string(clientMessageAsString.c_str());
			if (clientMessageResult.status)
			{
				throw std::runtime_error(clientMessageResult.description());
			}

			//Copy events from this client into return document
			for (pugi::xml_node currentEvent = currentEventDoc.first_child().first_child(); currentEvent; currentEvent = currentEvent.next_sibling())
			{
				clientEventsParent.append_copy(currentEvent);
			}
		}
	}

	return clientEventList;
}

void ServerSocket::Send(pugi::xml_document& _xmlToSend)
{
	std::stringstream ss;
	_xmlToSend.save(ss);
	std::string xmlAsString = ss.str();

	for (size_t ci = 0; ci < m_clients.size(); ci++)
	{
		m_clients[ci]->Send(xmlAsString);
	}
}

void ServerSocket::SendTo(int _index, std::string _xmlToSend)
{
	
	m_clients.at(_index)->Send(_xmlToSend);
}

void ServerSocket::SetNewPlayerID(int _id)
{
	m_clients[m_clients.size() - 1].get()->SetID(_id);
}

void ServerSocket::RemoveConnection(int _id)
{
	for (int i = 0; i < m_clients.size(); i++)
	{
		if (m_clients[i].get()->GetID() == _id)
		{
			m_clients.erase(m_clients.begin() + i);
			break;
		}
	}
}

void ServerSocket::Close()
{
	while(m_clients.size() > 0)
	{
		m_clients[0]->CloseConnection();
		m_clients[0].reset();
		m_clients.erase(m_clients.begin());
	}
}