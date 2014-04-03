#pragma once


#include "cryptopp/cryptlib.h"
using CryptoPP::Exception;

#include "cryptopp/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include "cryptopp/aes.h"
using CryptoPP::AES;

#include "cryptopp/ccm.h"
using CryptoPP::ECB_Mode;

class CipherUtils
{
private:
	CipherUtils(void);
	~CipherUtils(void);
	
public:
	static void removeZerosPadding(std::string& str);
	static void encrypt(char *keyString, const std::string& plain, std::string& cipher);
	static void encrypt(char *keyString, const void* plain, size_t length, std::string& cipher);
	static void decrypt(char *keyString, const std::string& cipher, std::string& decrypted);
};
