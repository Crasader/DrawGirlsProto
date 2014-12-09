//
//  CharacterDetailPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 10. 31..
//
//

#ifndef __DGproto__CharacterDetailPopup__
#define __DGproto__CharacterDetailPopup__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class CharacterDetailPopup : public CCLayer
{
public:
	static CharacterDetailPopup* create(int t_touch_priority, int t_cha_idx, function<void()> t_end_func, function<void()> t_select_func, function<void()> t_strength_func, bool t_is_have = true);
	
private:
	int touch_priority;
	function<void()> end_func;
	function<void()> select_func;
	function<void()> strength_func;
	bool is_have;
	
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	
	int cha_idx;
	
	CCSprite* gray;
	CCNode* m_container;
	CCSprite* back_case;
	CCControlButton* select_button;
	CCControlButton* strength_button;
	
	void selectAction(CCObject* sender, CCControlEvent t_event);
	void strengthAction(CCObject* sender, CCControlEvent t_event);
	void myInit(int t_touch_priority, int t_cha_idx, function<void()> t_end_func, function<void()> t_select_func, function<void()> t_strength_func, bool t_is_have);
};

#endif /* defined(__DGproto__CharacterDetailPopup__) */
