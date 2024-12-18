#include <random>
#include <string>

#pragma once

static class Encryption
{
public:

	/**
		Shifts text in _plain by m_key, then XORs each char by a corresponding character from _key.
		_key is actually the username of the player sending the message.
		Only the message's text is actually encrypted
	*/
	static std::string Encrypt(std::string _plain, std::string _key);
	static std::string Decrypt(std::string _cipher, std::string _key);
};

