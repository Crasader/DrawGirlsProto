#pragma once
//#include "crypto/CCCrypto.h"

#include <type_traits>
#include <string>
#include <cstdlib>
#include "CipherUtils.h"
#include "EncryptCharsA.h"

#include "ks19937.h"
class KSProtectStr
{
	
private:
	std::string m_cipher;
private:
	void encrypt(const std::string& data)
	{
		const char *keyString = "drawgirls__jjang";
		m_cipher = CipherUtils::encrypt(encryptChars(keyString).c_str(), data.c_str());
	}
	
public:
	
	std::string getV() const
	{
		std::string decrypted;
		const char *keyString = "drawgirls__jjang";
		decrypted = CipherUtils::decrypt(encryptChars(keyString).c_str(), m_cipher.c_str());
		return decrypted;
	}
	explicit KSProtectStr(const std::string& v)
	{
		encrypt(v);
	}
	
	std::string operator=(const std::string& arg)
	{
		encrypt(arg);
		return getV();
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
	T m_savedValue;
	
private:
	void encrypt(const T& data)
	{
		m_salt = ks19937::getIntValue(5, 50);
		m_savedValue = data;
		char* pt = (char*)&m_savedValue;
		char* saltPointer = (char*)&m_salt;
		for(int i=0; i<sizeof(T); i++)
		{
			pt[i] ^= saltPointer[i];
		}
	}
	
public:
	T m_salt;
	T getV() const
	{
		T copySavedValue = m_savedValue;
		char* pt = (char*)&copySavedValue;
		char* saltPointer = (char*)&m_salt;
		for(int i=0; i<sizeof(T); i++)
		{
			pt[i] ^= saltPointer[i];
		}
		return copySavedValue;
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



