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
#include <random>
class ProbSelector
{
protected:
	std::vector<double> m_probs;

	std::mt19937 m_rEngine; // 엔진
public:
	ProbSelector(const std::initializer_list<double>& s) : m_probs(s)
	{
		std::random_device rd;	
		m_rEngine.seed(rd());
	}
	ProbSelector()
	{
		std::random_device rd;
		m_rEngine.seed(rd());
	}

	void pushProb(double p)
	{
		m_probs.push_back(p);
	}
	int getResult();
	static int sel(double args, ...);
};

#endif
