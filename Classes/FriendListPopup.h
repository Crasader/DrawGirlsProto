#ifndef __FriendListPopup__
#define __FriendListPopup__

#include "cocos2d.h"
USING_NS_CC;


class FriendListPopup : public CCLayer
{
public:
	FriendListPopup();
	virtual ~FriendListPopup();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	CREATE_FUNC(FriendListPopup);
	//virtual void registerWithTouchDispatcher();
};

#endif
