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
	ProbSelector(){}
	void pushProb(float p)
	{
		m_probs.push_back(p);
	}
	int getResult();
	static int sel(double args, ...);
};

#endif
