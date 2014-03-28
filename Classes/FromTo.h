//
//  FromTo.h
//  DGproto
//
//  Created by ksoo k on 13. 9. 12..
//
//

#ifndef __DGproto__FromTo__
#define __DGproto__FromTo__


#include <cmath>
#include <functional>


template <typename T>
class FromToWithDuration2
{
private:
	T originFrom;
	T from;
	T to;
	float duration;
	T dx;
	//	bool positive;
	
	float flowTime;
	bool nextTurnIsFinish;
	std::function<float(float)> m_timeFunctor;
public:
	FromToWithDuration2(){
		m_timeFunctor = nullptr;
	}
	FromToWithDuration2(T _from, T _to, float _duration, std::function<float(float)> tf = nullptr) : originFrom(_from), nextTurnIsFinish(false)
	{
		nextTurnIsFinish = false;
		flowTime = 0.f;
		from = _from;
		to = _to;
		duration = _duration;
		dx = (to - from) / duration;
		m_timeFunctor = tf;
		// positive
	}
	
	void init(T _from, T _to, float _duration, std::function<float(float)> tf = nullptr)
	{
		nextTurnIsFinish = false;
		flowTime = 0.f;
		originFrom = _from;
		from = _from;
		to = _to;
		duration = _duration;
		if(duration != 0)
			dx = (to - from) / duration;
		
		m_timeFunctor = tf;
		// positive
		
	}
	bool step(float dt)
	{
		if(nextTurnIsFinish)
			return false; // finish
		flowTime += dt;
		if(duration)
			from = from + dx * dt;
		else
			from = to;
		if(duration <= flowTime)
		{
			from = to;
			nextTurnIsFinish = true;
		}
		
		return true;
	}
	bool absStep(float time)
	{
		if(nextTurnIsFinish)
			return false; // finish
		
		
		flowTime = time;
		if(m_timeFunctor)
			time = m_timeFunctor(time / duration) * duration;
		else
			time = time;
//		time = time - 1;
//		
//		float overshoot = 1.70158f;
//		
//    
//    time = (time * time * ((overshoot + 1) * time + overshoot) + 1);
		if(duration)
			from = originFrom + dx * time;
		else
			from = to;
		if(duration <= flowTime)
		{
			from = to;
			nextTurnIsFinish = true;
		}
		
		return true;
	}
	T getValue(){return from;}
	T getTo(){return to;}
};

/// duration 동안에 from 에서 to 로 변수를 변화시킴.


template <typename T>
class FromToWithDuration
{
private:
	T originFrom;
	T from;
	T to;
	float duration;
	T dx;
	bool positive;
public:
	FromToWithDuration(){}
	FromToWithDuration(T _from, T _to, float _duration) : originFrom(_from)
	{
		from = _from;
		to = _to;
		duration = _duration;
		dx = (to - from) / duration;
		// positive
		if(!(_to < _from))
		{
			positive = true;
		}
		else
		{
			positive = false;
		}
	}
	void init(T _from, T _to, float _duration)
	{
		originFrom = _from;
		from = _from;
		to = _to;
		duration = _duration;
		if(duration != 0)
			dx = (to - from) / duration;
		// positive
		if(!(_to < _from))
		{
			positive = true;
		}
		else
		{
			positive = false;
		}
	}
	bool step(float dt)
	{
		if(duration)
			from += dx * dt;
		else
			from = to;
		if(positive)
		{
			if(!(from < to))
			{
				from = to;
				return false; // finish
			}
		}
		else
		{
			if(!(to < from))
			{
				from = to;
				return false; // finish
			}
		}
		return true;
	}
	float getValue(){return from;}
	float getTo(){return to;}
};


/// 매번 dx 만큼 동안에 from 에서 to 로

template <typename T>
class FromTo
{
private:
	T originFrom;
	T from;
	T to;
	T dx;
	bool positive;
public:
	FromTo(){}
	FromTo(T _from, T _to, T _dx) : originFrom(_from)
	{
		from = _from;
		to = _to;
		_dx = fabs(_dx);
		
		// positive
		if(!(_to < _from))
		{
			dx = _dx;
			positive = true;
		}
		else
		{
			dx = -_dx;
			positive = false;
		}
		
	}
	T getTo(){return to;}
	void init(T _from, T _to, T _dx)
	{
		originFrom = _from;
		from = _from;
		to = _to;
		_dx = fabs(_dx);
		// positive
		if(!(_to < _from))
		{
			dx = _dx;
			positive = true;
		}
		else
		{
			dx = -_dx;
			positive = false;
		}
		
	}
	bool step()
	{
		from += dx;
		if(positive)
		{
			if(!(from < to))
			{
				from = to;
				return false; // finish
			}
		}
		else
		{
			if(!(to < from))
			{
				from = to;
				return false; // finish
			}
		}
		return true;
	}
	float getValue()
	{
		return from;
	}
};


#endif /* defined(__DGproto__FromTo__) */
