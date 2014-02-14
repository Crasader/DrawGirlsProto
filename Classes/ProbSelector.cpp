//
//  ProbSelector.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 25..
//
//

#include "ProbSelector.h"

int ProbSelector::sel(double args, ...)
{
	va_list argptr;
	va_start(argptr, args);        
	std::vector<float> ps;
	ps.push_back(args);
	for (int i = 0;; i++) 
	{
		double f = va_arg(argptr, double);
		if(f == 0.0)
			break;
		
		
		ps.push_back(f);
		//			cocos2d::CCLog("%f", f);
	}
	
	
	
	static Well512 well512;
	
	float sum = std::accumulate(ps.begin(), ps.end(), 0.f);
	assert(sum > 0.9f && sum <= 1.1f);
	
	float _01 = well512.GetFloatValue();
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
	va_end(argptr);
	if(finded)
		return _index - 1;
	else
		return -1;

}
//void ProbSelector::pushProb(double f)
//{
//	ps.push_back(f);
//}


//int ProbSelector::sel()
//{
//	double sum = std::accumulate(ps.begin(), ps.end(), 0);
//	assert(sum > 0.9 && sum <= 1.1);
//
//	float _01 = well512.GetFloatValue();
//	double t = 0;
//	int _index = 0;
//	for(auto i : ps)
//	{
//		t += i;
//		_index++;
//		if(_01 < t)
//		{
//			return _index - 1;
//		}
//	}
//	
//	return -1;
//}

int ProbSelector::getResult()
{
	
	std::vector<double> ps = m_probs;
	
	double sum = std::accumulate(ps.begin(), ps.end(), 0.0);
	std::uniform_real_distribution<> dist(0, sum);// 범위지정 
	double _01 = dist(m_rEngine);

	// {1,2,3} 으로 입력이 들어왔고, _01 이 3.5 라면
	// 각 요소의 누적값 : [1, 3, 6] 에서 봤을 때 3과 6 사이에 속하니까 1 이 반환 되어야함.
	// 루프당 누적값은 t 임.
	double t = 0;
	int _index = 0;
	bool finded = false;
	
	//for(auto i : ps) // C++ 11 라면 이 줄을 활성화.
	for(std::vector<double>::iterator iter = ps.begin(); iter != ps.end(); ++iter)
	{
		double i = *iter;
		t += i;
		_index++;
		if(_01 < t)
		{
			finded = true;
			break;
		}
	}
	if(finded)
		return _index - 1;
	else
		return -1;
}



