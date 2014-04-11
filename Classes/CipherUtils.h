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

#include "cryptopp/des.h"
using CryptoPP::DES;

#include "cryptopp/base64.h"
using CryptoPP::Base64Encoder;
using CryptoPP::Base64Decoder;
class CipherUtils
{
private:
	CipherUtils(void);
	~CipherUtils(void);
	
public:
	static void removeZerosPadding(std::string& str);
	static std::string encrypt(const char *keyString, const char* plain); // HEX 변환후 AES
	static std::string encrypt(const char *keyString, const void* plain, size_t length); // HEX 변환후 AES (데이터 암호화용)
	static std::string decrypt(const char *keyString, const char* cipher); // AES 풀고 HEX 품.
	
	static std::string encryptDESBASE64(const char *keyString, const char* plain);
	static std::string decryptDESBASE64(const char *keyString, const char* cipher);
	
	static std::string encryptAESBASE64(const char *keyString, const char* plain);
	static std::string decryptAESBASE64(const char *keyString, const char* cipher);
};
