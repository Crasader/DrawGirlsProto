//
//  CountingBMLabel.h
//  DrawingJack
//
//  Created by 사원3 on 13. 6. 6..
//
//

#ifndef __DrawingJack__CountingBMLabel__
#define __DrawingJack__CountingBMLabel__

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class CountingBMLabel : public CCLabelBMFont
{
public:
	static CountingBMLabel* create(string init_value, string font_filename, float t_duration);
	
	virtual void setString(const char* after_value);
	virtual const char* getString();
	
private:
	bool is_changing;
	float sign_value;
	float keep_value;
	float base_value;
	float decrease_value;
	float increase_value;
	string keep_value_string;
	float duration;
	
	void startChanging(const char* after_value);
	void changing(float dt);
	void stopChanging();
	
	void myInit(string init_value, string font_filename, float t_duration);
};

#endif /* defined(__DrawingJack__CountingBMLabel__) */
