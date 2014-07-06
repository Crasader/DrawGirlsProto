//
//  EasingAction.cpp
//  DGproto
//
//  Created by ksoo k on 2014. 3. 20..
//
//

#include <stdio.h>
#include <cmath>

float getTime(float time)
{
	//	return time;
	time = time - 1;
	float overshoot = 3.50158f;
	
	
	return (time * time * ((overshoot + 1) * time + overshoot) + 1);
	
	return -1 * cosf(time * (float)M_PI_2) + 1;
}

float easeOut(float time)
{
	return powf(time, 1 / 3.f);
}
float elasticOut(float time)
{
	float m_fPeriod = 0.3f;
	float newT = 0;
	if (time == 0 || time == 1)
	{
		newT = time;
	}
	else
	{
		float s = m_fPeriod / 4;
		newT = powf(1.2, -10 * time) * sinf((time - s) * M_PI_2 / m_fPeriod * 2.5) + 1;
	}
	
	
	return newT;
}
float elasticOut2(float time)
{
	float m_fPeriod = 0.3f;
	float newT = 0;
	if (time == 0 || time == 1)
	{
		newT = time;
	}
	else
	{
		float s = m_fPeriod / 4;
		newT = 1.2f * powf(1.2, -10 * time) * sinf((time - s) * M_PI_2 / m_fPeriod * 2.5) + 1;
	}
	
	
	return newT;
}
float bounceOut(float time)
{
//	float temp = 7.5625f;
	float temp = 7.5625f;
	if (time < 1 / 2.75)
	{
		return temp * time * time;
	} else
    if (time < 2 / 2.75)
    {
			time -= 1.5f / 2.75f;
			return temp * time * time + 0.75f;
    } else
			if(time < 2.5 / 2.75)
			{
        time -= 2.25f / 2.75f;
        return temp * time * time + 0.9375f;
			}
	
	time -= 2.625f / 2.75f;
	return temp * time * time + 0.984375f;
}

float boundTime(float time)
{
	if (time < 1 / 2.75)
	{
		return 7.5625f * time * time;
	} else
    if (time < 2 / 2.75)
    {
			time -= 1.5f / 2.75f;
			return 7.5625f * time * time + 0.75f;
    } else
			if(time < 2.5 / 2.75)
			{
        time -= 2.25f / 2.75f;
        return 7.5625f * time * time + 0.9375f;
			}
	
	time -= 2.625f / 2.75f;
	return 7.5625f * time * time + 0.984375f;
}

float expoIn(float time)
{
	return time == 0 ? 0 : powf(2, 10 * (time/1 - 1)) - 1 * 0.001f;
}