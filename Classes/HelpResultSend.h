#ifndef __HelpResult__
#define __HelpResult__

#include "cocos2d.h"
USING_NS_CC;

#include <string>
#define __TYPE__ HelpResultSend
class HelpResultSend : public CCLayer
{
public:
	HelpResultSend();
	virtual ~HelpResultSend();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(const std::string& corp_id, bool isSuccess);
	static __TYPE__* create(const std::string& corp_id, bool isSuccess) \
	{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(corp_id, isSuccess))
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
