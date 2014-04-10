//
//  StringCodec.cpp
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 4..
//
//

#include "StringCodec.h"
#include <sstream>
//#include "stlencoders/base32.hpp"
#include "stlencoders/base16.hpp"
#include "McbDES2.hpp"
#include <vector>

#include "CipherUtils.h"
#include "EncryptCharsA.h"
#include <string.h>
std::basic_string<char> stringEnc(string plainText)
{
//	return plainText;
	if(plainText == "")
		return "";
	 std::basic_string<char> strPlainText = plainText;
	 std::basic_string<char> strCryptogram;
	 
	/*
	 *************************************************************************
	 * Grab 8 bytes keys from somewhere.
	 *************************************************************************
	 */
	unsigned char * lpKey1 = (unsigned char*)"JSHSKSYH";
//    unsigned char * lpKey2 = (unsigned char*)"22222222";
	
	
	McbDES des;
	des.McbSetDES();
	des.McbSetKey1(lpKey1);
	
	unsigned long cbCryptogram =
	des.McbCalcCryptogramSize(strPlainText.size());
	
	strCryptogram.resize(cbCryptogram);
	
	des.McbSetOutputBuffer((unsigned char*)strCryptogram.data(),
							 strCryptogram.size());
	
	if (des.McbEncrypt(strPlainText.c_str()))
	{
		strCryptogram.resize(des.McbGetCryptogramSize());
	}
	
	ostringstream oss;
	 std::ostreambuf_iterator<char> out(oss);
	 stlencoders::base16<char>::encode(strCryptogram.begin(), strCryptogram.end(), out);
	return oss.str();
	//return base64_encode(strCryptogram.c_str(), strCryptogram.size());
}
string stringDecode(std::basic_string<char> encodedText)
{
//	return "";
	if(encodedText == "")
		return "";
	vector<char> v;
	 stlencoders::base16<char>::decode(encodedText.begin(), encodedText.end(), back_inserter(v));
	
	
	unsigned char * lpKey1 = (unsigned char*)"JSHSKSYH";
	McbDES des;
	des.McbSetDES();
	des.McbSetKey1(lpKey1);
	
	des.McbDecrypt((unsigned char*)v.data(),
					 v.size());
	
	const unsigned char* decData = des.McbGetPlainText();
	return (char*)decData;
}
std::basic_string<char> stringEncWithAES(string plainText)
{
	std::string retValue;
	CipherUtils::encrypt(encryptChars("nonevoidmodebase").c_str(), plainText.c_str(), retValue);
	return retValue;
}
string stringDecodeWithAES(std::basic_string<char> encodedText)
{
	//const char * lpKey1 = encryptChars({'m', 'a', 'i', 'i', 'p', 'o', 'i', 'n'}).c_str();
	string retValue;
	CipherUtils::decrypt(encryptChars("nonevoidmodebase").c_str(), encodedText.c_str(), retValue);
	return retValue;
}

