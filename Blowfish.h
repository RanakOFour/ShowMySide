#include <string>
#include <stdint.h>
#include <vector>
#include <memory>

#pragma once


/**
	An implementation of the blowfish algorithm for the network's encryption
*/
class Blowfish
{
private:
	/// @name KeyValues
	///@{
	/**
		pBox has 18 64 bit keys, used to XOR the left side of the plaintext.
		sBox is, logically, 4 different 32 x 8 grids of 32 bit integers.
	*/
	int64_t pBox[18];
	int32_t sBox[4][8][32];
	/// @}

	
	///@{ @name The Actual Encryption Algorithm
	/**  Main functions for encoding/decoding the strings */

	/**
		F() takes in the left hand side of the 64-bit input as a parameter, it is then split into
		4 8-bit ints, which are used to determine which keys from sBox are going to be used to generate
		the return value. Each 8 bit value is first right-shifted by 5 to use the remaining 3 bits for 
		the second array index. Then, the 8-bit get's shifted again by 3 to use the remaining 5 to the third index.
		The first sBox index corresponds to each slice of the left input.

		The first and second sBox value are bitwise added, then the third is XORed to the result, and the final
		and the last slice is added once again to _that_ result.
	*/
	int32_t F(int32_t _Lp);

	/**
		The main encoding method taking 8 characters of the main input string compressed into
		a 64-bit integer. The algorithm starts with splitting the 64-bit int into 2 32-bit ints
		(Left and Right). Then, Left is XORed with pBox[0], and the result is entered into F().
		The result of F() get's XORed with Right and then the values of left and right swap.

		Now, the value stored in Left gets XORed with pBox[1], and the result entered into F()
		etc. etc. The cycle repeats until the algorithm reaches pBox[15] where Left and Right
		have their values swapped and are then XORed respectively by pBox[17] and pBox[16].

		The two values are then placed back together and returned.
	*/
	int64_t encode(int64_t _input);

	/**
		The decryption of the Blowfish cipher involves doing the steps to encode the text, but 
		in reverse order. Read above.
	*/
	int64_t decode(int64_t _cipher);
	///@}

	Blowfish();

public:
	~Blowfish();


	///@{ @name Interface Functions

	/**
		
	*/
	static void Encrypt(std::string _input, std::vector<char>& _output);
	static void Decrypt(std::string _cipher, std::vector<char>& _output);
	
	/**
		Called in main.cpp to prepare the Blowfish* m_self inside of Blowfish.cpp,
		which is used to call the non-static functions.
	*/
	static void Initialise();
	///@}
};

