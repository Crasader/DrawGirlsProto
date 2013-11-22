#ifndef __ChallengeSend__
#define __ChallengeSend__

#include "cocos2d.h"
USING_NS_CC;
#include <string>


class ChallengeSend : public CCLayer
{
public:
	ChallengeSend();
	virtual ~ChallengeSend();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(const std::string& user_id, const std::string& nickname, int score);
	static ChallengeSend* create(const std::string& user_id, const std::string& nick, int score) \
	{ \
    ChallengeSend *pRet = new ChallengeSend(); \
    if (pRet && pRet->init(user_id, nick, score)) \
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

#endif
