//
//  CountingBMLabel.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 6. 6..
//
//

#include "CountingBMLabel.h"
#include "KSUtil.h"

CountingBMLabel* CountingBMLabel::create(string init_value, string font_filename, float t_duration, string t_format, bool t_is_comma)
{
	CountingBMLabel* t_cbml = new CountingBMLabel();
	t_cbml->myInit(init_value, font_filename, t_duration, t_format, t_is_comma);
	t_cbml->autorelease();
	return t_cbml;
}

void CountingBMLabel::setString(const char* after_value)
{
	stopAllActions();
	if(is_changing)
		stopChanging();
	startChanging(after_value);
}

const char* CountingBMLabel::getString()
{
	return keep_value_string.c_str();
}

void CountingBMLabel::startChanging(const char* after_value)
{
	if(base_scale == -1.f)
		base_scale = getScale();
	
	is_changing = true;
	
	if(is_on_change_scale)
	{
		CCScaleTo* t_scale1 = CCScaleTo::create(0.2f, 1.5f*base_scale);
		CCScaleTo* t_scale2 = CCScaleTo::create(0.5f, 1.f*base_scale);
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale1, t_scale2);
		runAction(t_seq);
	}
	
	base_value = atof(keep_value_string.c_str()); // 원래 가지고 있던 골드
	keep_value_string = after_value;
	keep_value = atof(after_value) - base_value; // 이번에 얻은 골드
	if(keep_value >= 0.f)	sign_value = 1.f;
	else					sign_value = -1.f;
	keep_value = fabsf(keep_value);
	decrease_value = keep_value;
	increase_value = 0.f;
	schedule(schedule_selector(CountingBMLabel::changing));
}

void CountingBMLabel::changing(float dt)
{
	if(decrease_value > 0)
	{
		float decreaseUnit = keep_value / duration * dt;
		
		if(decrease_value < decreaseUnit)
		{
			increase_value += decrease_value*sign_value;
			decrease_value = 0;
		}
		else {
			if(decreaseUnit <= 0.01)
			{
				increase_value += decrease_value*sign_value;
				decrease_value = 0;
			}
			else {
				decrease_value -= decreaseUnit;
				increase_value += decreaseUnit*sign_value;
			}
		}
		if(is_comma)
			CCLabelBMFont::setString(KS::insert_separator(CCString::createWithFormat(show_format.c_str(),base_value+increase_value)->getCString()).c_str());
		else
			CCLabelBMFont::setString(CCString::createWithFormat(show_format.c_str(),base_value+increase_value)->getCString());
	}
	else
		stopChanging();
}

void CountingBMLabel::stopChanging()
{
	unschedule(schedule_selector(CountingBMLabel::changing));
	is_changing = false;
	if(is_comma)
		CCLabelBMFont::setString(KS::insert_separator(keep_value_string.c_str()).c_str());
	else
		CCLabelBMFont::setString(keep_value_string.c_str());
}

void CountingBMLabel::setScale(float t_scale)
{
	base_scale = t_scale;
	CCLabelBMFont::setScale(t_scale);
}

void CountingBMLabel::onChangeScale(bool t_b)
{
	is_on_change_scale = t_b;
}

void CountingBMLabel::myInit(string init_value, string font_filename, float t_duration, string t_format, bool t_is_comma)
{
	is_comma = t_is_comma;
	is_on_change_scale = true;
	base_scale = -1;
	is_changing = false;
	duration = t_duration;
	keep_value_string = init_value.c_str();
	if(t_format == "%d")
		show_format = "%.0f";
	else
		show_format = t_format.c_str();
	if(is_comma)
		CCLabelBMFont::initWithString(KS::insert_separator(init_value.c_str()).c_str(), font_filename.c_str(), kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
	else
		CCLabelBMFont::initWithString(init_value.c_str(), font_filename.c_str(), kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
	stopChanging();
}