#include "CipherUtils.h"


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

void CipherUtils::encrypt(const char *keyString, const char* plain, string &cipher)
{
	assert(strlen(keyString) == AES::DEFAULT_KEYLENGTH);
	byte key[ AES::DEFAULT_KEYLENGTH ];
	
	strncpy((char *)key, keyString, AES::DEFAULT_KEYLENGTH);
	try
	{
		cipher.clear();
		StringSource( plain, true,
								 new HexEncoder( new StringSink( cipher ) )
								 ); // StringSource
		
		ECB_Mode< AES >::Encryption e;
		// ECB Mode does not use an IV
		e.SetKey( key, sizeof(key) );
		
		
		
		// The StreamTransformationFilter adds padding
		//  as required. ECB and CBC Mode must be padded
		//  to the block size of the cipher.
		StringSource(cipher, true,
								 new StreamTransformationFilter( e,
																								new StringSink( cipher ), StreamTransformationFilter::ZEROS_PADDING
																								) // StreamTransformationFilter
								 ); // StringSource
	}
	catch( CryptoPP::Exception& e )
	{
		//cerr << "Caught Exception..." << endl;
		//cerr << e.what() << endl;
		//cerr << endl;
	}
}
void CipherUtils::encrypt(const char *keyString, const void *plain, size_t length, std::string& cipher)
{
	assert(strlen(keyString) == AES::DEFAULT_KEYLENGTH);
	byte key[ AES::DEFAULT_KEYLENGTH ];
	strncpy((char *)key, keyString, AES::DEFAULT_KEYLENGTH);
	try
	{
		cipher.clear();
		StringSource( (const byte*)plain, length, true,
								 new HexEncoder( new StringSink( cipher ) )
								 ); // StringSource
		
		ECB_Mode< AES >::Encryption e;
		// ECB Mode does not use an IV
		e.SetKey( key, sizeof(key) );
		
		
		
		// The StreamTransformationFilter adds padding
		//  as required. ECB and CBC Mode must be padded
		//  to the block size of the cipher.
		StringSource(cipher, true,
								 new StreamTransformationFilter( e,
																								new StringSink( cipher ), StreamTransformationFilter::ZEROS_PADDING
																								) // StreamTransformationFilter
								 ); // StringSource
	}
	catch( CryptoPP::Exception& e )
	{
		//cerr << "Caught Exception..." << endl;
		//cerr << e.what() << endl;
		//cerr << endl;
	}
}
void CipherUtils::decrypt(const char *keyString, const char* cipher, string& decrypted)
{
	assert(strlen(keyString) == AES::DEFAULT_KEYLENGTH);
	byte key[ AES::DEFAULT_KEYLENGTH ];
	strncpy((char *)key, keyString, AES::DEFAULT_KEYLENGTH);
	
	try
	{
		StringSource s1( cipher, true,
									 new HexDecoder( new StringSink( decrypted ) ) );
		
		ECB_Mode< AES >::Decryption d;
		// ECB Mode does not use an IV
		d.SetKey( key, sizeof(key) );
		
		StringSource s2( decrypted, true,
									 new StreamTransformationFilter( d,
																									new StringSink( decrypted ), StreamTransformationFilter::ZEROS_PADDING
																									) // StreamTransformationFilter
									 ); // StringSource
		
		CipherUtils::removeZerosPadding(decrypted);
		
	}
	catch( CryptoPP::Exception& e )
	{
		//cerr << "Caught Exception..." << endl;
		//cerr << e.what() << endl;
		//cerr << endl;
	}
}
