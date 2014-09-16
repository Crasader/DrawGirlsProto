
#ifndef DrawingJack_AlertEngine_h
#define DrawingJack_AlertEngine_h

#include "cocos2d.h"

using namespace cocos2d;

typedef void (CCObject::*SEL_AlertFuncII)(int, int);
#define alertfuncII_selector(_SELECTOR) (SEL_AlertFuncII)(&_SELECTOR)

class AlertEngine : public CCNode
{
public:
	static AlertEngine* sharedInstance();
	
	void addDoubleAlert(const char* t_title, const char* t_message, const char* t_leftButtonText, const char* t_rightButtonText, int t_tag, CCObject* t_target, SEL_AlertFuncII t_selector);
	void addSingleAlert(const char* t_title, const char* t_message, const char* t_checkButtonText, int t_tag, CCObject* t_target, SEL_AlertFuncII t_selector);
	void addDoubleAlert(const char *t_title, const char *t_message, const char *t_leftButtonText, const char *t_rightButtonText, int t_tag, std::function<void(int,int)> fc);
	void doAction(int t_alertNumber, int t_buttonNumber);
	
private:
	
	CCObject* target;
	SEL_AlertFuncII selector;
	std::function<void(int,int)> func;
	bool addable;
	
	void myInit()
	{
		func=nullptr;
		target=nullptr;
		selector=nullptr;
//		addable = true;
	}
};

#endif
