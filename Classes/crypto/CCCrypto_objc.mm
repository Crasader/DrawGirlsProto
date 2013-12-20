#include "CCCrypto.h"

#if CC_LUA_ENGINE_ENABLED > 0
extern "C" {
#include "lua.h"
#include "tolua_fix.h"
}
#endif

#import <CommonCrypto/CommonCryptor.h>



int CCCrypto::getAES256KeyLength(void)
{
	return kCCKeySizeAES256;
}

int CCCrypto::cryptAES256(bool isDecrypt,
                          const void* input,
                          int inputLength,
                          void* output,
                          int outputBufferLength,
                          const void* key,
                          int keyLength)
{
	int maxKeyLength = getAES256KeyLength();
	unsigned char key_[maxKeyLength];
	memset(key_, 0, sizeof(key_));
	memcpy(key_, key, keyLength > maxKeyLength ? maxKeyLength : keyLength);
	
	int bufferSize = inputLength + kCCBlockSizeAES128;
	void* buffer = malloc(bufferSize);
	size_t dataUsed = 0;
	
	CCCryptorStatus status = CCCrypt(isDecrypt ? kCCDecrypt : kCCEncrypt,
																	 kCCAlgorithmAES128,
																	 kCCOptionPKCS7Padding | kCCOptionECBMode,
																	 key_, maxKeyLength,
																	 NULL,
																	 input, inputLength,
																	 buffer, bufferSize,
																	 &dataUsed);
	
	if (status == kCCSuccess)
	{
		if (outputBufferLength >= dataUsed + 1)
		{
			memset(output, 0, outputBufferLength);
			memcpy(output, buffer, dataUsed);
			return dataUsed;
		}
		// output buffer too small
		return 0;
	}
	
	switch (status)
	{
		case kCCParamError:
			CCLOG("[CCCrypto] cryptAES256(%s) param error", isDecrypt ? "Decrypt" : "Encrypt");
			break;
		case kCCBufferTooSmall:
			CCLOG("[CCCrypto] cryptAES256(%s) buffer too small", isDecrypt ? "Decrypt" : "Encrypt");
			break;
		case kCCMemoryFailure:
			CCLOG("[CCCrypto] cryptAES256(%s) memory failure", isDecrypt ? "Decrypt" : "Encrypt");
			break;
		case kCCAlignmentError:
			CCLOG("[CCCrypto] cryptAES256(%s) alignment error", isDecrypt ? "Decrypt" : "Encrypt");
			break;
		case kCCDecodeError:
			CCLOG("[CCCrypto] cryptAES256(%s) decode error", isDecrypt ? "Decrypt" : "Encrypt");
			break;
		case kCCUnimplemented:
			CCLOG("[CCCrypto] cryptAES256(%s) unimplemented", isDecrypt ? "Decrypt" : "Encrypt");
			break;
		default:
			CCLOG("[CCCrypto] cryptAES256(%s) unknown error", isDecrypt ? "Decrypt" : "Encrypt");
	}
	
	return 0;
}

#if CC_LUA_ENGINE_ENABLED > 0

cocos2d::LUA_STRING CCCrypto::cryptAES256Lua(bool isDecrypt,
                                             const void* input,
                                             int inputLength,
                                             const void* key,
                                             int keyLength)
{
	int bufferSize = inputLength + getAES256KeyLength();
	void* buffer = malloc(bufferSize);
	int dataUsed = cryptAES256(isDecrypt, input, inputLength, buffer, bufferSize, key, keyLength);
	cocos2d::CCLuaStack* stack = cocos2d::CCLuaEngine::defaultEngine()->getLuaStack();
	stack->clean();
	if (dataUsed > 0)
	{
		stack->pushString(static_cast<const char*>(buffer), dataUsed);
	}
	else
	{
		stack->pushNil();
	}
	free(buffer);
	return 1;
}

#endif

