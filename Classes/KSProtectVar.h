#pragma once
#include "crypto/CCCrypto.h"

#include <type_traits>

template<typename T>

class KSProtectVar
{
private:
	int m_plainTextLength;
	int m_bufferLength;
	void* m_buff;
	int m_cipherTextLength;
private:
	void encrypt(const T& data)
	{
		const char* key = "cocos2dx";
		m_plainTextLength = sizeof(T);
		m_bufferLength = m_plainTextLength + CCCrypto::getAES256KeyLength();
		int keyLen = strlen(key);
		m_buff = new char[m_bufferLength];
		m_cipherTextLength = CCCrypto::encryptAES256(&data, m_plainTextLength, m_buff,
																								 m_bufferLength, key, keyLen);
	}
	
public:
	
	T getV() const
	{
		const char* key = "cocos2dx";
		int keyLen = strlen(key);
		T* plaintext2 = (T*)(new char[m_bufferLength]);
    int plaintextLength2 = CCCrypto::decryptAES256(m_buff, m_cipherTextLength, plaintext2,
																									 m_bufferLength, key, keyLen);
		T ret = *plaintext2;
		delete [] plaintext2;
		return ret;
	}
	explicit KSProtectVar(const T& v)
	{
		static_assert(std::is_scalar<T>::value, "SCALAR");
		encrypt(v);
	}
	~KSProtectVar()
	{
		delete [] m_buff;
	}
	T operator+(const T& arg) const
	{
		return getV() + arg;
	}
	T operator+(const KSProtectVar<T>& arg) const
	{
		return getV() + arg.getV();
	}
	T operator+=(const T& arg)
	{
		encrypt(getV() + arg);
		return getV();
	}
	T operator-(const T& arg) const
	{
		return getV() - arg;
	}
	
	T operator-=(const T& arg)
	{
		encrypt(getV() - arg);
		return getV();
	}
	T operator=(const T& arg)
	{
		encrypt(arg);
		return getV();
	}
	T operator--(int) // postfix
	{
		T original = getV();
		*this -= 1;
		return original;
	}
	T operator--() // prefix
	{
		*this -= 1;
		return getV();
	}
	
	T operator++(int) // postfix
	{
		T original = getV();
		*this += 1;
		return original;
	}
	T operator++() // prefix
	{
		*this += 1;
		return getV();
	}
};



