#ifndef __JoinGameFriendPopup__
#define __JoinGameFriendPopup__

#include "cocos2d.h"
USING_NS_CC;


class JoinGameFriendPopup : public CCLayer
{
public:
	JoinGameFriendPopup();
	virtual ~JoinGameFriendPopup();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	CREATE_FUNC(JoinGameFriendPopup);
	//virtual void registerWithTouchDispatcher();
};

#endif
