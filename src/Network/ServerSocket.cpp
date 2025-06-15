#include "Network/ServerSocket.h"
#include "Network/ClientSocket.h"
#include "pugixml.hpp"

#if _WIN32
	#include <ws2tcpip.h>
#else
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <fcntl.h>
	#define INVALID_SOCKET (~0)
	#define SOCKET_ERROR (-1)
#endif

#include <stdexcept>
#include <sstream>
#include <string>
#include <memory>
#include <vector>

ServerSocket::ServerSocket(int _port) :
	m_socket(INVALID_SOCKET),
	m_clients()
{
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

	for(addrinfo* p = result; p != NULL; p = p->ai_next)
	{
		m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if(m_socket == INVALID_SOCKET)
		{
			//throw std::runtime_error("Could not connect socket");
			continue;
		}

		//Bind address to socket (give socket an ip address for others to connect to)
		if (bind(m_socket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
		{
			#if _WIN32
				closesocket(m_socket);
			#else
				close(m_socket);
			#endif

			continue;
			//throw std::runtime_error("Failed to bind socket");
		}

		//Set non-blocking
		u_long mode = 1;
		int nonBlockCheck;
		#if _WIN32
			nonBlockCheck = ioctlsocket(m_socket, FIONBIO, &mode);
			if (nonBlockCheck == SOCKET_ERROR)
			{
				printf("Failed to set non-blocking: %d\n", nonBlockCheck);
				continue;
			}
		#else
			nonBlockCheck = fcntl(m_socket, F_SETFL, O_NONBLOCK);
			if(nonBlockCheck < 0)
    		{
				printf("Failed to set non-blocking: %d\n", nonBlockCheck);
				continue;
    		}
		#endif

		//Open the socket so that it will listen to incoming signals
		if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
		{
			continue;
		}

		break;
	}

	// Grab host IP address

	char l_ipAddress[INET_ADDRSTRLEN];

	// For some reason, just doing it with the connection result gives 0.0.0.0
	gethostname(l_ipAddress, INET_ADDRSTRLEN);
	getaddrinfo(l_ipAddress, std::to_string(_port).c_str(), &hints, &result);

	sockaddr_in* l_address = (struct sockaddr_in*)result->ai_addr;
	inet_ntop(AF_INET, &(l_address->sin_addr), l_ipAddress, sizeof(l_ipAddress));

	freeaddrinfo(result);

	m_ipAddress = l_ipAddress;
}

ServerSocket::~ServerSocket()
{
	#if _WIN32
		closesocket(m_socket);
	#else
		close(m_socket);
	#endif
}

std::shared_ptr<ClientSocket> ServerSocket::Accept()
{
	SOCKET socket = ::accept(m_socket, NULL, NULL);
	if (socket == INVALID_SOCKET)
	{
		#if _WIN32
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				throw std::runtime_error("Failed to accept socket");
			}
		#endif
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
		printf("Client Connected!\n");

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
			printf("Message recieved as server: %s\n", clientMessageAsString.c_str());


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