#include <Blowfish.h>
#include <Pugixml/pugixml.hpp>

#include <string>
#include <vector>
#include <fstream>
#include <stdint.h>
#include <memory>



// The compiler does not agree with this being a smart pointer
static Blowfish* m_self;

void Blowfish::Initialise()
{
	if (m_self == nullptr)
	{
		m_self = new Blowfish();
	}
}

Blowfish::Blowfish() :
	pBox{ 0	},
	sBox{ 0 }
{
	// Load pBox values 
	std::fstream file("./text/pBox.txt");
	std::string currentBox;

	int i = 0;
	while (std::getline(file, currentBox))
	{
		pBox[i] = 0;

		//Shift character binary value into 32 bit integer (8 bit per character = 4 characters per int)
		for (int j = 0; j < 4; j++)
		{
			int64_t shift = (32 - (8 * (j + 1)));
			pBox[i] += ((int64_t)currentBox[j] << shift);
		}

		currentBox = "";
		++i;
	}
	file.close();

	// Unholy creation to read all the values out of all the sBox files into the sBoxes
	for (int i = 0; i < 4; i++)
	{
		int j = 0;
		int k = 0;
		int readCount{ 0 };
		std::string path = "./text/s" + std::to_string(i + 1) + ".txt";
		file.open(path.c_str());

		while (std::getline(file, currentBox))
		{
			for (int l = 0; l < 8; l++)
			{
				int64_t shift = (64 - (8 * (l + 1)));
				sBox[i][j][k] += ((int64_t)currentBox[l] << shift);
			}


			currentBox = "";
			++k;
			if (k == 32)
			{
				k = 0;
				++j;
			}
		}
		file.close();
	}

	printf("Initialised blowfish\n");
}

Blowfish::~Blowfish()
{

}

int32_t Blowfish::F(int32_t _Lp)
{
	int32_t result{ 0 };
	int8_t split[4];
	int64_t sOut[4];

	/* 
		Take 8 bits from _Lp, then get the 3 right most bits and use as col value (0-7),
		then use the remaining 5 bits for row value (0-31)
	*/

	for (int i = 0; i < 4; i++)
	{
		split[i] = _Lp >> (32 - (8 * (i + 1)));
		int8_t col = split[i] >> 5;
		int8_t row = split[i] >> 3;

		// All the bits in the row before the 8 bit slice shifted up 8 minus
		// the same bits with the slice will leave just the slice
		sOut[i] = sBox[i][col][row];

		switch(i)
		{
		case 1:
			result = sOut[0] & sOut[1];
			break;
		case 2:
			result = result ^ sOut[2];
			break;
		case 3:
			result = result & sOut[3];
			break;
		}
	}


	return result;
}

int64_t Blowfish::encode(int64_t _input)
{
	int64_t left = _input >> 32;
	int64_t right = _input - (left << 32);

	for (int i = 0; i < 16; i++)
	{
		left = left ^ pBox[i];
		int64_t fResult = F(left);

		right = fResult ^ right;

		// Swap left and right for next iteration
		int64_t temp = left;
		left = right;
		right = temp;
	}

	// Undo last swap
	int64_t temp = left;
	left = right;
	right = temp;

	// Output whitening
	left = left ^ pBox[17];
	right = right ^ pBox[16];

	left = left << 32;
	int64_t toReturn = left + right;
	return toReturn;
}

int64_t Blowfish::decode(int64_t _cipher)
{
	int64_t left = _cipher >> 32;
	int64_t right = _cipher - (left << 32);

	// undo whitening
	left = left ^ pBox[17];
	right = right ^ pBox[16];

	// Undo last swap
	int64_t temp = left;
	left = right;
	right = temp;

	for (int i = 15; i != -1; i--)
	{// Swap left and right for nect iteration
		int64_t temp = left;
		left = right;
		right = temp;
		
		int64_t fResult = F(left);

		right = right ^ fResult;

		left = left ^ pBox[i];
	}

	left = left << 32;
	int64_t toReturn = left + right;
	return toReturn;
}

void Blowfish::Encrypt(std::string _input, std::vector<char>& _output)
{
	if (m_self == nullptr)
	{
		return;
	}

	int currentIndex{ 0 };

	while (currentIndex < _input.size())
	{
		// Load next 8 characters into nextByte
		int64_t nextByte{ 0 };
		for (int i = 0; i < 8 && currentIndex < _input.size(); i++)
		{
			int64_t shift = (64 - (8 * (i + 1)));
			nextByte += (int64_t)_input[currentIndex] << shift;
			++currentIndex;
		}

		int64_t encodedBytes = m_self->encode(nextByte);

		//Unload encrypted bytes into toReturn
		for (int i = 0; i < 8 && currentIndex < _input.size() + 1; i++)
		{
			int64_t shift = (64 - (8 * (i + 1)));
			char currentChar = (char)((encodedBytes >> shift) - ((encodedBytes >> shift) << 8));
			_output.push_back(currentChar);
		}
	}
}

void Blowfish::Decrypt(std::string _cipher, std::vector<char>& _output)
{
	if (m_self == nullptr)
	{
		return;
	}

	int currentIndex{ 0 };

	while (currentIndex < _cipher.size())
	{
		// Load next 8 characters into nextByte
		int64_t nextByte{ 0 };
		for (int i = 0; i < 8 && currentIndex < _cipher.size(); i++)
		{
			int64_t shift = (64 - (8 * (i + 1)));
			nextByte += (int64_t)_cipher[currentIndex] << shift;
			++currentIndex;
		}

		int64_t decodedBytes = m_self->decode(nextByte);

		//Unload encrypted bytes into toReturn
		for (int i = 0; i < 8 && currentIndex < _cipher.size() + 1; i++)
		{
			int64_t shift = (64 - (8 * (i + 1)));
			char currentChar = (char)((decodedBytes >> shift) - ((decodedBytes >> shift) << 8));
			_output.push_back(currentChar);
		}
	}
}