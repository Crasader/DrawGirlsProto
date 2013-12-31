#pragma once
//#include "crypto/CCCrypto.h"

#include <type_traits>
#include <string>
#include <cstdlib>
class KSProtectStr
{
	
private:
	std::string m_buff;
private:
	void encrypt(const std::string& data)
	{
		m_buff = data;
	}
	
public:
	
	std::string getV() const
	{
		return m_buff;
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
	T m_buff;
private:
	void encrypt(const T& data)
	{
		m_buff = data;
		//		m_cipherTextLength = CCCrypto::encryptAES256(&data, m_plainTextLength, m_buff,
//																								 m_bufferLength, key, keyLen);
	}
	
public:
	
	T getV() const
	{
		return m_buff;
	}
	explicit KSProtectVar(typename std::enable_if<std::is_scalar<T>::value, const T&>::type v) : m_buff(0)
	{
		encrypt(v);
	}
	KSProtectVar(const KSProtectVar<T>& copyCtor)
	{
		encrypt(copyCtor.getV());
	}
	KSProtectVar() : m_buff(0)
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
		*this = KSProtectVar<T>(arg);
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



