//
//  TypingBox.h
//  DGproto
//
//  Created by 사원3 on 2014. 7. 23..
//
//

#ifndef __DGproto__TypingBox__
#define __DGproto__TypingBox__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BackKeyBase.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum KindTutorialType
{
	kNewItem_gacha = 1,
	kNewMission_collector,
	kNewMission_catcher,
	kNewMission_business,
	kUI_achieve,
	kUI_card,
	kUI_diary,
	kUI_rank,
	kNewMission_percent,
	kNewMission_score,
	kNewMission_combo,
	kNewMission_gold,
	kNewMission_turn,
	kUI_hellmode,
	kUI_friend,
	kNewMission_hell
};

class TouchSuctionLayer;
class KSLabelTTF;

class TypingBox : public CCLayer, public CCScrollViewDelegate//, public BackKeyBase
{
public:
	
	//"kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64)
	//"kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64)
	static TypingBox* create(int t_touch_priority, string t_box_filename, CCRect t_box_9_out, CCRect t_box_9_in, CCRect t_box_in_label, CCSize t_box_in_size, CCPoint t_box_position, CCPoint t_button_position = ccp(0,0));
	void setHide();
	void showAnimation(float t_time);
	void startTyping(string t_box_string, function<void()> t_end_func);
	void stopTyping();
	void setTouchOffScrollAndButton();
	void setTouchSuction(bool t_b);
	void setBoxScale(float t_s);
	
	static void changeTypingBox(TypingBox* from_tb, TypingBox* to_tb, CCSprite* from_cha, CCSprite* to_cha);
	
	virtual void keyBackClicked(void);
	
private:
	
	int touch_priority;
	string box_filename;
	CCRect box_9_out;
	CCRect box_9_in;
	CCRect box_in_label;
	CCSize box_in_size;
	CCPoint box_position;
	CCPoint button_position;
	string box_string;
	int add_string_frame;
	int typing_frame;
	int text_length;
	bool is_typing;
	
	int typing_sound_number;
	
	bool is_menu_enable;
	
	function<void()> end_func;
	
	TouchSuctionLayer* suction;
	
	CCScale9Sprite* box_img;
	CCScrollView* scroll_view;
	KSLabelTTF* typing_label;
	CCControlButton* next_button;
	
	void controlButtonAction(CCObject* sender, CCControlEvent t_event);
	
	void myInit(int t_touch_priority, string t_box_filename, CCRect t_box_9_out, CCRect t_box_9_in, CCRect t_box_in_label, CCSize t_box_in_size, CCPoint t_box_position, CCPoint t_button_position);
	
	void typing();
	
	
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
};

#endif /* defined(__DGproto__TypingBox__) */
