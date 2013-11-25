#ifndef __HatGacha__
#define __HatGacha__

#include "cocos2d.h"
USING_NS_CC;


class HatGacha : public CCLayer
{
public:
	HatGacha();
	virtual ~HatGacha();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	CREATE_FUNC(HatGacha);
	//virtual void registerWithTouchDispatcher();
};

#endif
