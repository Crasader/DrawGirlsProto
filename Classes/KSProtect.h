#pragma once
#include "crypto/CCCrypto.h"

#include <type_traits>

class KSProtectStr
{
	
private:
	int m_plainTextLength;
	int m_bufferLength;
	void* m_buff;
	int m_cipherTextLength;
private:
	void encrypt(const std::string& data)
	{
		const char* key = "cocos2dx";
		m_plainTextLength = data.size();
		m_bufferLength = m_plainTextLength + CCCrypto::getAES256KeyLength();
		int keyLen = strlen(key);
		m_buff = malloc(m_bufferLength);
		m_cipherTextLength = CCCrypto::encryptAES256(data.c_str(), m_plainTextLength, m_buff,
																								 m_bufferLength, key, keyLen);
	}
	
public:
	
	std::string getV() const
	{
		const char* key = "cocos2dx";
		int keyLen = strlen(key);
		void* plaintext2 = (char*)malloc(m_bufferLength);
    int plaintextLength2 = CCCrypto::decryptAES256(m_buff, m_cipherTextLength, plaintext2,
																									 m_bufferLength, key, keyLen);
		std::string ret = (char*)plaintext2;
		free(plaintext2);
		return ret;
	}
	explicit KSProtectStr(const std::string& v)
	{
		encrypt(v);
	}
	~KSProtectStr()
	{
		free((void*)m_buff);
	}
};
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



