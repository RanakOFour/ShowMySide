#include <string>
#include <stdint.h>
#include <vector>
#include <memory>

#pragma once


/**
	An implementation of the blowfish algorithm for network encryption
*/
class Blowfish
{
private:

	/**
		P-Box values used for Blowfish
	*/
	int64_t pBox[18];

	/**
		The S-Boxes are a set of fixes values, so I just load them from a string
	*/
	int32_t sBox[4][8][32];

	int32_t F(int32_t _Lp);
	int64_t encode(int64_t _input);
	int64_t decode(int64_t _cipher);

	Blowfish();

public:
	~Blowfish();

	static void Encrypt(std::string _input, std::vector<char>& _output);
	static void Decrypt(std::string _cipher, std::vector<char>& _output);
	static void Initialise();
};

