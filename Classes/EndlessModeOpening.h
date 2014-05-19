//
//  EndlessModeOpening.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 19..
//
//

#ifndef __DGproto__EndlessModeOpening__
#define __DGproto__EndlessModeOpening__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class EndlessModeOpening : public CCLayer
{
public:
	virtual bool init();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
	CREATE_FUNC(EndlessModeOpening);
	
private:
	
	int touch_priority;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	TouchSuctionLayer* suction;
	
	bool is_menu_enable;
	
	CCSprite* gray;
	
	CCScale9Sprite* main_case;
	
	void setMain();
};

#endif /* defined(__DGproto__EndlessModeOpening__) */
