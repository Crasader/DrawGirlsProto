#ifndef __ProbSelector__
#define __ProbSelector__

#include <initializer_list>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>

#include <assert.h>
#include "Well512.h"
#include "cocos2d.h"
class ProbSelector
{
protected:
	std::vector<float> m_probs;
public:
	ProbSelector(std::initializer_list<float> s) : m_probs(s)
	{

	}
	int getResult()
	{
		
		std::vector<float> ps = m_probs;
		
		
		static Well512 well512;
		
		float sum = std::accumulate(ps.begin(), ps.end(), 0.f);
		assert(sum > 0.9f && sum <= 1.1f);
		
		float _01 = well512.GetFloatValue();
//		_01 = 0.623509f;
		//	cocos2d::CCLog("%f", _01);
		float t = 0;
		int _index = 0;
		bool finded = false;
		
		//for(auto i : ps) // C++ 11 라면 이 줄을 활성화.
		for(std::vector<float>::iterator iter = ps.begin(); iter != ps.end(); ++iter)
		{
			float i = *iter;
			t += i;
			_index++;
			if(_01 < t)
			{
				finded = true;
				break;
				//				va_end(argptr);
				//				return _index - 1;
			}
		}
		if(finded)
			return _index - 1;
		else
			return -1;
	}
	static int sel(double args, ...);
};

#endif
