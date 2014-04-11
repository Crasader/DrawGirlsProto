#include "EncryptCharsA.h"

std::string encryptChars(const std::string& chars)
{	
	std::string retValue(chars.begin(), chars.end());
	for(auto& i : retValue)
	{
		i += 3;
	}
	return retValue;
}

