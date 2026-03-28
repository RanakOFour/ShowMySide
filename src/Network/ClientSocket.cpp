#include "Network/ClientSocket.h"
#include "Blowfish.h"

#include <string>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include <vector>

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
	#include <errno.h>
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
        m_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
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

	freeaddrinfo(result);

	if (m_socket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		
		#if _WIN32
			WSACleanup();
		#endif

		return false;
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

	// Prefix a 4-byte little-endian length so the receiver can always
	// extract exactly one complete message even if two arrive in one recv().
	uint32_t len = (uint32_t)encryptedXML.size();
	std::vector<char> frame(4 + encryptedXML.size());
	frame[0] = (char)((len >>  0) & 0xFF);
	frame[1] = (char)((len >>  8) & 0xFF);
	frame[2] = (char)((len >> 16) & 0xFF);
	frame[3] = (char)((len >> 24) & 0xFF);
	std::copy(encryptedXML.begin(), encryptedXML.end(), frame.begin() + 4);

	size_t totalSent = 0;
	while (totalSent < frame.size())
	{
		int bytes = ::send(m_socket, frame.data() + totalSent, (int)(frame.size() - totalSent), 0);
		if (bytes < 0)
		{
			#if _WIN32
				if (WSAGetLastError() != WSAEWOULDBLOCK)
					throw std::runtime_error("Failed to send data!\n");
			#else
				if (errno != EAGAIN && errno != EWOULDBLOCK)
					throw std::runtime_error("Failed to send data!\n");
			#endif
			// Send buffer temporarily full — drop this message rather than block
			printf("Send would block on socket %d, dropping\n", (int)m_socket);
			return;
		}
		if (bytes == 0)
			throw std::runtime_error("Connection closed during send\n");
		totalSent += (size_t)bytes;
	}
	printf("Bytes sent on socket %d: %zu\n", (int)m_socket, totalSent);
}

void ClientSocket::Receive(std::string& _message)
{
	if (m_closed)
		return;

	// Drain the socket into the persistent buffer
	{
		char buf[4096];
		for (;;)
		{
			int bytes = ::recv(m_socket, buf, sizeof(buf), 0);
			if (bytes > 0)
			{
				m_receiveBuffer.append(buf, (size_t)bytes);
			}
			else if (bytes == 0)
			{
				// Peer closed connection cleanly
				break;
			}
			else
			{
				#if _WIN32
					if (WSAGetLastError() != WSAEWOULDBLOCK)
						throw std::runtime_error("Read failed");
				#else
					if (errno != EAGAIN && errno != EWOULDBLOCK)
						throw std::runtime_error("Read failed");
				#endif
				break; // EAGAIN — no more data right now
			}
		}
	}

	// Extract one complete length-prefixed frame from the buffer.
	// The sender prepends a 4-byte little-endian payload length before the
	// encrypted data, so two back-to-back messages never get merged.
	if (m_receiveBuffer.size() < 4)
		return; // length header not yet complete

	uint32_t payloadLen =
		((uint8_t)m_receiveBuffer[0]) |
		((uint8_t)m_receiveBuffer[1] << 8) |
		((uint8_t)m_receiveBuffer[2] << 16) |
		((uint8_t)m_receiveBuffer[3] << 24);

	if (m_receiveBuffer.size() < 4 + payloadLen)
		return; // payload not yet complete

	printf("Bytes received on socket %d: %u\n", (int)m_socket, payloadLen);

	std::string encrypted = m_receiveBuffer.substr(4, payloadLen);
	m_receiveBuffer.erase(0, 4 + payloadLen); // leave any next message for the next tick

	std::vector<char> decryptedChar;
	Blowfish::Decrypt(encrypted, decryptedChar);
	_message = std::string(decryptedChar.begin(), decryptedChar.end());
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
