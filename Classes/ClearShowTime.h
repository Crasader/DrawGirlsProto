//
//  ClearShowTime.h
//  galsprototype
//
//  Created by 사원3 on 13. 8. 26..
//
//

#ifndef __galsprototype__ClearShowTime__
#define __galsprototype__ClearShowTime__

#include "cocos2d.h"

USING_NS_CC;

enum ClearShowTimeZorder{
	kCST_Z_clear = 1,
	kCST_Z_boader,
	kCST_Z_showtime,
	kCST_Z_showtime_back
};

class ClearShowTime : public CCNode
{
public:
	static ClearShowTime* create(bool t_exchanged, bool t_is, CCNode* t_game_node, CCObject* t_changeScene, SEL_CallFunc d_changeScene);
	
	virtual void visit();
	
	void startClearView(CCPoint t_p);
	
private:
	
	bool is_end_schedule;
	
	CCRect view_rect;
	bool is_showtime;
	CCNode* game_node;
	CCSprite* side_back;
	
	int frame_cnt;
	
	int silType;
	
	CCSprite* clear_img;
	CCSprite* showtime_back;
	CCSprite* showtime_img;
	
	CCObject* target_changeScene;
	SEL_CallFunc delegate_changeScene;
	
	void changeScene();
	
	void addViewHeight();
	
	void myInit(bool t_exchanged, bool t_is, CCNode* t_game_node, CCObject* t_changeScene, SEL_CallFunc d_changeScene);
};

#endif /* defined(__galsprototype__ClearShowTime__) */
