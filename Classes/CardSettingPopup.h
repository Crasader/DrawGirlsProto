//
//  CardSettingPopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 20..
//
//

#ifndef __DGproto__CardSettingPopup__
#define __DGproto__CardSettingPopup__

#include "cocos2d.h"

//#include <deque>
#include <map>

USING_NS_CC;
using namespace std;

class CardListViewer;
class ListViewerScroll;
class CardSettingPopup : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CardSettingPopup);
	
	virtual void onEnter();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
private:
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* main_case;
	CCSprite* gray;
	
	CCSprite* selected_card_img;
	CCMenu* selected_card_menu;
	CCSprite* card_option_case;
	CCLabelTTF* card_option_script;
	CCNode* star_parent;
	CCMenu* mount_menu;
	
	CCSprite* selected_img;
	CCSprite* check_img;
	
	map<int, CCPoint> align_default_position_list;
	
	CCSprite* align_list_img;
	CardListViewer* my_clv;
	ListViewerScroll* t_lvs;
	
	CCPoint inner_card_distance;
	
	int recent_mounted_number;
	
	void showPopup();
	void endShowPopup();
	void hidePopup();
	void endHidePopup();
	
	CCPoint getContentPosition(int t_tag);
	void removeMountingCard();
	void mountingCard(int card_stage, int card_level);
	
	void createCardList();
	void alignChange();
	void addMountedCase();
	void removeMountedCase();
	
	bool is_menu_enable;
};

#endif /* defined(__DGproto__CardSettingPopup__) */
