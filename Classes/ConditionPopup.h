//
//  ConditionPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 12..
//
//

#ifndef __DGproto__ConditionPopup__
#define __DGproto__ConditionPopup__

#include "cocos2d.h"
#include "StarGoldData.h"
#include "SilhouetteData.h"

USING_NS_CC;

enum CDT_Zorder{
	kCDT_Z_back = 1,
	kCDT_Z_content
};

class ConditionPopup : public CCNode
{
public:
	static ConditionPopup* create(CCObject* t_close, SEL_CallFunc d_close);
	
	void holdingPopup();
	
private:
	
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	
	int ing_frame;
	int end_frame;
	
	CCSprite* cdt_back;
	CCLabelTTF* content;
	
	void startHiding();
	
	void hiding();
	
	void endCall();
	
	void myInit(CCObject* t_close, SEL_CallFunc d_close);
};

#endif /* defined(__DGproto__ConditionPopup__) */
