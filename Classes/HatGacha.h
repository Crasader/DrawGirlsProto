#ifndef __HatGacha__
#define __HatGacha__

#include "cocos2d.h"
#include <functional>
USING_NS_CC;

#define __TYPE__ HatGacha
class HatGacha : public CCLayer
{
public:
	std::function<void(void)> m_closeCallback;
	HatGacha();
	virtual ~HatGacha();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(std::function<void(void)> closeCallback);
	static __TYPE__* create(std::function<void(void)> closeCallback) \
	{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(closeCallback))
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
