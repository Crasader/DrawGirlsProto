//
//  StringCodec.h
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 4..
//
//

#ifndef __BasketWorldCup2__StringCodec__
#define __BasketWorldCup2__StringCodec__
#include <iostream>
using namespace std;


std::basic_string<char> stringEncWithAES(string plainText);
string stringDecodeWithAES(std::basic_string<char> encodedText);

std::basic_string<char> stringEnc(string plainText);
//std::basic_string<char> stringEncWithDES(string plainText);
//string stringDecodeWithDES(std::basic_string<char> encodedText);

#endif /* defined(__BasketWorldCup2__StringCodec__) */
