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
	static CountingBMLabel* create(string init_value, string font_filename, float t_duration, string t_format);
	
	virtual void setString(const char* after_value);
	virtual const char* getString();
	virtual void setScale(float t_scale);
	void onChangeScale(bool t_b);
	
private:
	bool is_on_change_scale;
	bool is_changing;
	float sign_value;
	float keep_value;
	float base_value;
	float decrease_value;
	float increase_value;
	string keep_value_string;
	float duration;
	string show_format;
	float base_scale;
	
	void startChanging(const char* after_value);
	void changing(float dt);
	void stopChanging();
	
	void myInit(string init_value, string font_filename, float t_duration, string t_format);
};

#endif /* defined(__DrawingJack__CountingBMLabel__) */
