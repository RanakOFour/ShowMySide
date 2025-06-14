#include "Network/ClientSocket.h"
#include "Blowfish.h"

#include <string>
#include <memory>
#include <stdexcept>

#if _WIN32
	#include <ws2tcpip.h>
#else
	#include <unistd.h> 
	#include <arpa/inet.h>
	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <fcntl.h>
	#define INVALID_SOCKET (~0)
	#define SOCKET_ERROR (-1)
#endif

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
		#if _WIN32
			closesocket(m_socket);
		#else
			close(m_socket);
		#endif
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

	int addrResult = getaddrinfo(_serverName.c_str(), "8080", &hints, &result);
	if (addrResult != 0) {
		printf("getaddrinfo failed with error: %d\n", addrResult);

		#if _WIN32
			WSACleanup();
		#endif

		return false;
	}

	for(addrinfo* p = result; p != NULL; p = p->ai_next)
	{
        m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if(m_socket == INVALID_SOCKET)
		{
			//throw std::runtime_error("Could not connect socket");
			continue;
		}

		int connectRes = connect(m_socket, p->ai_addr, p->ai_addrlen);
        if (connectRes < 0)
		{
            printf("client: connect");
            close(m_socket);
            continue;
        }

        break;
    }

	u_long mode = 1;

	#if _WIN32
		if (ioctlsocket(m_socket, FIONBIO, &mode) == SOCKET_ERROR)
		{
			throw std::runtime_error("Failed to set non-blocking");
		}
	#else
		if(fcntl(m_socket, F_SETFL, O_NONBLOCK) < 0)
    	{
        	throw std::runtime_error("Failed to set non-blocking\n");
    	}
	#endif

	freeaddrinfo(result);

	if (m_socket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		
		#if _WIN32
			WSACleanup();
		#endif

		return false;
	}

	return true;
}

// bool ClientSocket::Connect(std::string& _serverName)
// {
// 	m_socket = socket(AF_INET, SOCK_STREAM, 0);

//     if (m_socket < 0)
//     {
//         printf("Socket creation error \n");
//         return false;
//     }

// 	sockaddr_in l_serv_addr;
//     l_serv_addr.sin_family = AF_INET;
//     l_serv_addr.sin_port = htons(8080);

//     // Convert IPv4 and IPv6 addresses from text to binary
//     if (inet_pton(AF_INET, _serverName.c_str(), &l_serv_addr.sin_addr) <= 0) 
//     {
//         printf("Invalid address/ Address not supported \n");
//         return false;
//     }

// 	// Set non-blocking
// 	#if _WIN32
// 		if (ioctlsocket(m_socket, FIONBIO, &mode) == SOCKET_ERROR)
//  		{
//  			throw std::runtime_error("Failed to set non-blocking");
// 			return false;
//  		}
// 	#else
//     	if(fcntl(m_socket, F_SETFL, O_NONBLOCK) < 0)
//     	{
//         	throw std::runtime_error("Failed to set non-blocking\n");
//         	return false;
//     	}
// 	#endif

//     // Connect to server
//     if (connect(m_socket, (struct sockaddr *)&l_serv_addr, sizeof(l_serv_addr)) < 0) {
//         // Non-blocking connect will return immediately
//         // Check errno to distinguish between connection in progress and connection failed
//         if (errno != EINPROGRESS) 
// 		{
//             perror("connection failed");
//             return false;
//         }
//     }

// 	return true;
// }

void ClientSocket::Send(std::string& _message)
{
	std::vector<char> encryptedXML;
	Blowfish::Encrypt(_message, encryptedXML);
	std::string messageToSend = std::string(encryptedXML.begin(), encryptedXML.end());

	int bytes = send(m_socket, messageToSend.c_str(), messageToSend.length(), 0);
	if (bytes <= 0)
	{
		throw std::runtime_error("Failed to send data!\n");
	}
	else
	{
		printf("Bytes sent on socket %d: %d\n", (int)m_socket, bytes);
	}
}

void ClientSocket::Receive(std::string& _message)
{
	if (m_closed)
	{
		return;
	}

	std::string receivedText;
	std::string currentTextPull;
	int totalBytes{ 0 };

	do
	{
		currentTextPull = "";
		char buffer[256] = { 0 };
		int bytes = ::recv(m_socket, buffer, sizeof(buffer) - 1, 0);

		//Leave the loop when bytes returns WSAEWOULDBLOCK
		// It is a non-fatal error and just means that the non-blocking socket couldn't do it's thing
		if (bytes == SOCKET_ERROR || bytes < 0)
		{
			#if _WIN32
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					throw std::runtime_error("Read failed");
				}
			#endif

			break;
		}

		currentTextPull.append(buffer, bytes);
		receivedText.append(currentTextPull);

		totalBytes += bytes;

		printf("Current bytes: %d", totalBytes);
	} while (currentTextPull != "");

	if (receivedText == "")
	{
		return;
	}

	printf("Bytes recieved: %d\n", totalBytes);

	std::vector<char> decryptedChar;
	Blowfish::Decrypt(receivedText, decryptedChar);
	_message = std::string(decryptedChar.begin(), decryptedChar.end());
	//printf("Decrypt Pre: %s\nDecrypt Post: %s\n", encrypedMessage.c_str(), _message.c_str());
}

void ClientSocket::CloseConnection()
{
	int result = -40;
	
	#if _WIN32
		result = closesocket(m_socket);
	#else
		result = close(m_socket);
	#endif

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
