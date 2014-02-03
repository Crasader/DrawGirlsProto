#include "cocos2d.h"

USING_NS_CC;
class KSCoverLayer : public cocos2d::CCLayerColor
{
public:
	static int touchPriority;
	bool animation;



	virtual void registerWithTouchDispatcher();

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	KSCoverLayer()
	{
		animation = true;
		//		touchPriority-=2;
	}
	CREATE_FUNC(KSCoverLayer);
	virtual ~KSCoverLayer() {
		//		touchPriority+=2;
	}

	bool init();
};
