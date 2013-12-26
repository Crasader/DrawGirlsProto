#pragma once

#include <type_traits>

template<typename T>
class KSProtectVar
{
private:
	typedef  T encedT;
	typedef  T pureT;
	T data;
private:
	T getConv(T arg) const
	{
		int varSizeInByte = sizeof(arg);
		char* pV = (char*)&arg;
		for(int i=0; i<varSizeInByte; i++)
		{
			pV[i] ^= 0xcd;
		}
		return arg;
	}
public:
	explicit KSProtectVar(pureT v)
	{
		static_assert(std::is_scalar<pureT>::value, "SCALAR");
		data = getConv(v);
	}
	T getVar()
	{
		T g = getConv(data);
		return g;
	}
	T operator+(pureT arg) const
	{
		return getConv(getConv(data) + arg);
	}
	T operator+=(pureT arg)
	{
		data = getConv(getConv(data) + arg);
		return data;
	}
	T operator-(pureT arg) const
	{
		return getConv(getConv(data) - arg);
	}
	T operator-=(pureT arg)
	{
		data = getConv(getConv(data) - arg);
		return data;
	}
	T operator=(pureT arg)
	{
		data = getConv(arg);
		return data;
	}
	T operator--(int) // postfix
	{
		pureT _a = getConv(data);
		_a--;
		data = getConv(_a);
		return getConv(data);
	}
	T operator--() // prefix
	{
		pureT _a = getConv(data);
		pureT retValue = _a;
		_a--;
		data = getConv(_a);
		return retValue;
	}
	
	T operator++(int) // postfix
	{
		pureT _a = getConv(data);
		_a++;
		data = getConv(_a);
		return getConv(data);
	}
	T operator++() // prefix
	{
		pureT _a = getConv(data);
		pureT retValue = _a;
		_a++;
		data = getConv(_a);
		return retValue;
	}
};