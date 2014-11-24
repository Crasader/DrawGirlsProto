#include "CipherUtils.h"
#include "cocos2d.h"

using std::string;
CipherUtils::CipherUtils(void)
{
}

CipherUtils::~CipherUtils(void)
{
}

void CipherUtils::removeZerosPadding(string& str)
{
  string::size_type pos = str.find_last_not_of((char)0);
  if(pos != string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(' ');
    if(pos != string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
}

std::string CipherUtils::encrypt(const char *keyString, const char* plain)
{
	assert(strlen(keyString) == AES::DEFAULT_KEYLENGTH);
	byte key[ AES::DEFAULT_KEYLENGTH ];
	
	strncpy((char *)key, keyString, AES::DEFAULT_KEYLENGTH);
	std::string retValue;
	try
	{
//		return plain;
		std::string hexCode;
		
//		StringSource( plain, true,
//								 new HexEncoder( new StringSink( hexCode ) )
//								 ); // StringSource
		hexCode = plain;
		ECB_Mode< AES >::Encryption e;
		// ECB Mode does not use an IV
		e.SetKey( key, sizeof(key) );		
		
		std::string cipher;
		// The StreamTransformationFilter adds padding
		//  as required. ECB and CBC Mode must be padded
		//  to the block size of the cipher.
		StringSource(hexCode, true,
								 new StreamTransformationFilter( e,
																								new StringSink( cipher ), StreamTransformationFilter::ZEROS_PADDING
																								) // StreamTransformationFilter
								 ); // StringSource
		StringSource( cipher, true,
								 new HexEncoder( new StringSink( retValue ) )
								 ); // StringSource

	}
	catch( CryptoPP::Exception& e )
	{
		CCLOG("%s", e.what());
		//cerr << "Caught Exception..." << endl;
		//cerr << e.what() << endl;
		//cerr << endl;
	}
	
	return retValue;
}

std::string CipherUtils::decrypt(const char *keyString, const char* cipher)
{
	assert(strlen(keyString) == AES::DEFAULT_KEYLENGTH);
	byte key[ AES::DEFAULT_KEYLENGTH ];
	strncpy((char *)key, keyString, AES::DEFAULT_KEYLENGTH);
	std::string decrypted;	
	try
	{
//		return cipher;
		std::string cipher2;
		StringSource ( cipher, true,
										new HexDecoder( new StringSink( cipher2 ) ) );
		
		ECB_Mode< AES >::Decryption d;
		// ECB Mode does not use an IV
		d.SetKey( key, sizeof(key) );
		std::string hexCode;
		StringSource s2( cipher2, true,
									 new StreamTransformationFilter( d,
																									new StringSink( hexCode ), StreamTransformationFilter::ZEROS_PADDING
																									) // StreamTransformationFilter
									 ); // StringSource
		CipherUtils::removeZerosPadding(hexCode);
		decrypted = hexCode;
//		StringSource s1( hexCode, true,
//										new HexDecoder( new StringSink( decrypted ) ) );
		
		
	}
	catch( CryptoPP::Exception& e )
	{
		CCLOG("%s", e.what());
	}
	return decrypted;
}
//std::string CipherUtils::encryptForBinary(const char *keyString, const void *plain, size_t length)
//{
//	assert(strlen(keyString) == AES::DEFAULT_KEYLENGTH);
//	byte key[ AES::DEFAULT_KEYLENGTH ];
//	strncpy((char *)key, keyString, AES::DEFAULT_KEYLENGTH);
//	std::string retValue;
//	try
//	{
//		std::string cipher;
//		std::string hexCode;
//		
//		StringSource( (const byte*)plain, length, true,
//								 new HexEncoder( new StringSink( hexCode ) )
//								 ); // StringSource
//		
//		ECB_Mode< AES >::Encryption e;
//		// ECB Mode does not use an IV
//		e.SetKey( key, sizeof(key) );
//		
//		
//		// The StreamTransformationFilter adds padding
//		//  as required. ECB and CBC Mode must be padded
//		//  to the block size of the cipher.
//		StringSource(hexCode, true,
//								 new StreamTransformationFilter( e,
//																								new StringSink( cipher ), StreamTransformationFilter::ZEROS_PADDING
//																								) // StreamTransformationFilter
//								 ); // StringSource
//		StringSource( cipher, true,
//								 new Base32Encoder( new StringSink( retValue ) )
//								 ); // StringSource
//	}
//	catch( CryptoPP::Exception& e )
//	{
//		//cerr << "Caught Exception..." << endl;
//		//cerr << e.what() << endl;
//		//cerr << endl;
//	}
//	
//	return retValue;
//}
//std::string CipherUtils::decryptForBinary(const char *keyString, const char* cipher)
//{
//	assert(strlen(keyString) == AES::DEFAULT_KEYLENGTH);
//	byte key[ AES::DEFAULT_KEYLENGTH ];
//	strncpy((char *)key, keyString, AES::DEFAULT_KEYLENGTH);
//	std::string decrypted;
//	try
//	{
//		//		return cipher;
//		std::string cipher2;
//		StringSource ( cipher, true,
//									new HexDecoder( new StringSink( cipher2 ) ) );
//		
//		ECB_Mode< AES >::Decryption d;
//		// ECB Mode does not use an IV
//		d.SetKey( key, sizeof(key) );
//		std::string hexCode;
//		StringSource s2( cipher2, true,
//										new StreamTransformationFilter( d,
//																									 new StringSink( hexCode ), StreamTransformationFilter::ZEROS_PADDING
//																									 ) // StreamTransformationFilter
//										); // StringSource
//		CipherUtils::removeZerosPadding(hexCode);
//		decrypted = hexCode;
//		//		StringSource s1( hexCode, true,
//		//										new HexDecoder( new StringSink( decrypted ) ) );
//		
//		
//	}
//	catch( CryptoPP::Exception& e )
//	{
//		printf("%s", e.what());
//	}
//	return decrypted;
//}

std::string CipherUtils::encryptDESBASE64(const char *keyString, const char* plain)
{
	
	assert(strlen(keyString) == DES::DEFAULT_KEYLENGTH);
	byte key[ DES::DEFAULT_KEYLENGTH ];
	byte iv[DES::BLOCKSIZE] = {0,};
	strncpy((char *)key, keyString, DES::DEFAULT_KEYLENGTH);
	std::string hexCode;
	try
	{
		std::string cipher;
		ECB_Mode< DES >::Encryption e;
		// ECB Mode does not use an IV
		e.SetKey(key, sizeof(key) ); // , iv );
		// The StreamTransformationFilter adds padding
		//  as required. ECB and CBC Mode must be padded
		//  to the block size of the cipher.
		StringSource(plain, true,
								 new StreamTransformationFilter( e,
																								new StringSink( cipher )
																								) // StreamTransformationFilter
								 ); // StringSource
		StringSource( cipher, true,
								 new Base64Encoder( new StringSink( hexCode ) )
								 ); // StringSource
	}
	catch( CryptoPP::Exception& e )
	{
		//cerr << "Caught Exception..." << endl;
		//cerr << e.what() << endl;
		//cerr << endl;
	}
	return hexCode;
}
std::string CipherUtils::decryptDESBASE64(const char *keyString, const char* cipher)
{
	assert(strlen(keyString) == DES::DEFAULT_KEYLENGTH);
	byte key[ DES::DEFAULT_KEYLENGTH ];
	strncpy((char *)key, keyString, DES::DEFAULT_KEYLENGTH);
	byte iv[DES::BLOCKSIZE] = {0,};
	std::string decrypted;	
	try
	{
		std::string hexCode;
		StringSource s1( cipher, true,
										new Base64Decoder( new StringSink( hexCode ) ) );
		
		ECB_Mode< DES >::Decryption d;

		// ECB Mode does not use an IV
		d.SetKey(key, sizeof(key) ); // , iv );
		StringSource s2( hexCode, true,
										new StreamTransformationFilter( d,
																									 new StringSink( decrypted )
																									 ) // StreamTransformationFilter
										); // StringSource
//		CipherUtils::removeZerosPadding(decrypted);
		
	}
	catch( CryptoPP::Exception& e )
	{
		//cerr << "Caught Exception..." << endl;
		//cerr << e.what() << endl;
		//cerr << endl;
	}
	return decrypted;
}


//#include "cocos2d.h"
std::string CipherUtils::encryptAESBASE64(const char *keyString, const char* plain)
{
	assert(strlen(keyString) == AES::DEFAULT_KEYLENGTH);
	byte key[ AES::DEFAULT_KEYLENGTH ];
	memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
	strncpy((char *)key, keyString, AES::DEFAULT_KEYLENGTH);
	std::string cipher;
	try
	{
		ECB_Mode< AES >::Encryption e;
		// ECB Mode does not use an IV
		e.SetKey( key, sizeof(key) );
		
		std::string base64;
//		cipher.clear();
		// The StreamTransformationFilter adds padding
		//  as required. ECB and CBC Mode must be padded
		//  to the block size of the cipher.
		
		StringSource(plain, true,
								 new StreamTransformationFilter( e,
																								new StringSink( base64 ), StreamTransformationFilter::ZEROS_PADDING
																								) // StreamTransformationFilter
								 ); // StringSource
		
		StringSource( base64, true,
								 new Base64Encoder( new StringSink( cipher ) )
								 ); // StringSource
	}
	catch( CryptoPP::Exception& e )
	{
		printf("%s", e.what());
		//cerr << "Caught Exception..." << endl;
		//cerr << e.what() << endl;
		//cerr << endl;
	}
	return cipher;
}
std::string CipherUtils::decryptAESBASE64(const char *keyString, const char* cipher)
{
	assert(strlen(keyString) == AES::DEFAULT_KEYLENGTH);
	byte key[ AES::DEFAULT_KEYLENGTH ];
	memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
	memcpy(key, keyString, AES::DEFAULT_KEYLENGTH);
//	strncpy((char *)key, keyString, AES::DEFAULT_KEYLENGTH);
//	CCLOG("key 0 %c", key[0]);
	std::string decrypted;
	try
	{
		std::string base64;
		StringSource s1( cipher, true,
										new Base64Decoder( new StringSink( base64 ) ) );
		ECB_Mode< AES >::Decryption d;
		
		string hexStr;
		CryptoPP::StringSource s3(base64, true,
															new CryptoPP::HexEncoder( new CryptoPP::StringSink( hexStr )));
//		CCLOG("base64 data %s", hexStr.c_str());
		// ECB Mode does not use an IV
		d.SetKey( key, sizeof(key) );
		
		StringSource s2( base64, true,
										new StreamTransformationFilter( d,
																									 new StringSink( decrypted ), StreamTransformationFilter::ZEROS_PADDING
																									 ) // StreamTransformationFilter
										); // StringSource
		CipherUtils::removeZerosPadding(decrypted);		
	}
	catch( CryptoPP::Exception& e )
	{
		CCLOG("%s", e.what());
	}
	return decrypted;	
}






