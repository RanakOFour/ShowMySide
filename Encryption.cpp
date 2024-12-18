#include "Encryption.h"

#include <random>
#include <string>
#include <vector>

std::string Encryption::Encrypt(std::string _plain, std::string _key)
{
	int shift = _key.size();
	std::string cipherText;

	for (int i = 0; i < _plain.size(); i++)
	{
		//Caesar
		unsigned char shifted = (unsigned char)_plain[i] + shift;

		// XOR the value by the letter of _key looped
		// e.g. if _key = 'John' and _plain = 'test text is here'
		//		
		//		Key Value:  JohnJohnJohnJohnJ
		//		Plain Text: test text is here

		shifted = shifted ^ (unsigned char)_key[i % _key.size()];

		cipherText.push_back(shifted);
	}

	return cipherText;
}

std::string Encryption::Decrypt(std::string _cipher, std::string _key)
{
	int caesarKey = _key.size();
	std::string plainText;

	for (int i = 0; i < _cipher.size(); i++)
	{
		// Undo XOR
		unsigned char decrypedChar = (unsigned char)_cipher[i] ^ (unsigned char)_key[i % _key.size()];

		decrypedChar = decrypedChar - caesarKey;

		plainText.push_back(decrypedChar);
	}

	return plainText;
}