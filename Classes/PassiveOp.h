#pragma once
#include <functional>
template <class _Tp>
struct PassiveOp : public std::binary_function<_Tp, _Tp, _Tp>
{
	virtual _Tp operator()(const _Tp& a, const _Tp& b) const = 0;//(const _Tp& a, const _Tp& b) const
};

template <class _Tp>
struct DecreaseOp : public PassiveOp<_Tp>
{
	virtual _Tp operator()(const _Tp& a, const _Tp& b) const
	{
		return a*(1 - b);
	}
};


template <class _Tp>
struct AddOp : public PassiveOp<_Tp>
{
	virtual ~AddOp(){
	}
	virtual _Tp operator()(const _Tp& a, const _Tp& b) const
	{
		return a + b;
	}
};


template <class _Tp>
struct SubtractOp : public PassiveOp<_Tp>
{
	virtual ~SubtractOp(){
	}
	virtual _Tp operator()(const _Tp& a, const _Tp& b) const
	{
		return a - b;
	}
};

template <class _Tp>
struct MultOp : public PassiveOp<_Tp>
{
	virtual ~MultOp(){
	}
	virtual _Tp operator()(const _Tp& a, const _Tp& b) const
	{
		return a * b;
	}
};
