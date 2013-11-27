#ifndef __HatGacha__
#define __HatGacha__

#include "cocos2d.h"
USING_NS_CC;

#define __TYPE__ HatGacha
class HatGacha : public CCLayer
{
public:
	HatGacha();
	virtual ~HatGacha();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	static __TYPE__* create() \
	{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init())
    { \
			pRet->autorelease(); \
			return pRet; \
    } \
    else \
    { \
			delete pRet; \
			pRet = NULL; \
			return NULL; \
    } \
	}

	//virtual void registerWithTouchDispatcher();
};

#undef __TYPE__


#endif
