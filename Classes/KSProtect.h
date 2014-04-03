#pragma once
//#include "crypto/CCCrypto.h"

#include <type_traits>
#include <string>
#include <cstdlib>
#include "CipherUtils.h"
class KSProtectStr
{
	
private:
	std::string m_cipher;
private:
	void encrypt(const std::string& data)
	{
		char *keyString = "drawgirls__jjang";
		CipherUtils::encrypt(keyString, data, m_cipher);
	}
	
public:
	
	std::string getV() const
	{
		std::string decrypted;
		char *keyString = "drawgirls__jjang";
		CipherUtils::decrypt(keyString, m_cipher, decrypted);
		return decrypted;
	}
	explicit KSProtectStr(const std::string& v)
	{
		encrypt(v);
	}
	KSProtectStr()
	{
		encrypt("");
	}
	~KSProtectStr()
	{
//		free((void*)m_buff);
	}
};
template<typename T>
class KSProtectVar
{
private:
	//T m_buff;
	std::string m_cipher;
private:
	void encrypt(const T& data)
	{
		char *keyString = "drawgirls__jjang";
		CipherUtils::encrypt(keyString, &data, sizeof(data), m_cipher);
		//m_buff = data;
		//		m_cipherTextLength = CCCrypto::encryptAES256(&data, m_plainTextLength, m_buff,
//																								 m_bufferLength, key, keyLen);
	}
	
public:
	
	std::string toString(T arg) const
	{
		int varSizeInByte = sizeof(arg);
		std::string retValue;
		retValue.resize(varSizeInByte);
		char* pV = (char*)&arg;
		for(int i=0; i<varSizeInByte; i++)
		{
			retValue[i] = pV[i];
		}

		return retValue;
	}
	T toVar(const std::string& str) const
	{
		T retValue;
		char* pV = (char*)&retValue;
		int varSizeInByte = sizeof(T);

		for(int i=0; i<varSizeInByte; i++)
		{
			pV[i] = str[i];
		}
		return retValue;
	}
	T getV() const
	{
		std::string decrypted;
		char *keyString = "drawgirls__jjang";
		CipherUtils::decrypt(keyString, m_cipher, decrypted);
		return toVar(decrypted);
	}
	explicit KSProtectVar(typename std::enable_if<std::is_scalar<T>::value, const T&>::type v)
	{
		encrypt(v);
	}
	KSProtectVar(const KSProtectVar<T>& copyCtor)
	{
		encrypt(copyCtor.getV());
	}
	KSProtectVar()
	{
		encrypt(static_cast<T>(0));
	}
	~KSProtectVar()
	{
//		if(m_buff)
//		{
//			free((void*)m_buff);
//			m_buff = 0;
//		}
	}
	T operator+(const T& arg) const
	{
		return getV() + arg;
	}
	T operator+(const KSProtectVar<T>& arg) const
	{
		return getV() + arg.getV();
	}
	
	T operator-(const T& arg) const
	{
		return getV() - arg;
	}
	
	T operator-(const KSProtectVar<T>& arg) const
	{
		return getV() - arg.getV();
	}
	T operator*(const T& arg) const
	{
		return getV() * arg;
	}
	
	T operator*(const KSProtectVar<T>& arg) const
	{
		return getV() * arg.getV();
	}
	T operator/(const T& arg) const
	{
		return getV() / arg;
	}
	T operator/(const  KSProtectVar<T>arg) const
	{
		return getV() / arg.getV();
	}
	T operator=(const T& arg)
	{
		encrypt(arg);
		return getV();
	}
	T operator=(const KSProtectVar<T>& arg)
	{
		encrypt(arg.getV());
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
	T operator+=(const T& arg)
	{
		encrypt(getV() + arg);
		return getV();
	}
	T operator+=(const KSProtectVar<T>& arg)
	{
		encrypt(getV() + arg.getV());
		return getV();
	}
	T operator-=(const T& arg)
	{
		encrypt(getV() - arg);
		return getV();
	}
	T operator-=(const KSProtectVar<T>& arg)
	{
		encrypt(getV() - arg.getV());
		return getV();
	}
	T  operator *= (const T & arg) // A *= B
	{
		// Do the assignement... Beware of auto assignement
		encrypt(getV() * arg);
		return getV();
	}
	T  operator *= (const  KSProtectVar<T>& arg) // A *= B
	{
		// Do the assignement... Beware of auto assignement
		encrypt(getV() * arg.getV());
		return getV();
	}
	
	T operator /= (const T & arg) // A /= B
	{
		// Do the assignement... Beware of auto assignement
		encrypt(getV() / arg);
		return getV();
	}
	
	T operator /= (const KSProtectVar<T> & arg) // A /= B
	{
		// Do the assignement... Beware of auto assignement
		encrypt(getV() / arg.getV());
		return getV();
	}
	T operator %= (const T & arg) // A %= B
	{
		encrypt(getV() % arg);
		return getV();
	}
	
	T operator %= (const KSProtectVar<T> & arg) // A %= B
	{
		encrypt(getV() % arg.getV());
		return getV();
	}
	bool operator!()
	{
		return !getV();
	}
	bool operator==(const T& arg)
	{
		return getV() == arg;
	}
	bool operator==(const KSProtectVar<T> & arg)
	{
		return getV() == arg.getV();
	}
	bool operator <(const T& arg)
	{
		return getV() < arg;
	}
	bool operator <(const KSProtectVar<T>& arg)
	{
		return getV() < arg.getV();
	}
	bool operator != (const T & arg) // A != B
	{
		return getV() != arg;
	}
	
	bool operator != (const KSProtectVar<T> & arg) // A != B
	{
		return getV() != arg.getV();
	}
	bool operator > (const T & arg) // A > B
	{
		return getV() > arg;
	}
	bool operator > (const KSProtectVar<T> & arg) // A > B
	{
		return getV() > arg.getV();
	}
	
	bool operator <= (const T & arg) // A <= B
	{
		return getV() <= arg;
	}
	
	bool operator <= (const KSProtectVar<T>& arg) // A <= B
	{
		return getV() <= arg.getV();
	}
	bool operator >= (const T & arg) // A => B
	{
		return getV() >= arg;
	}

	bool operator >= (const KSProtectVar<T> & arg) // A => B
	{
		return getV() >= arg.getV();
	}
};



