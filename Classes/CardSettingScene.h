//
//  CardSettingScene.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 18..
//
//

#ifndef __DGproto__CardSettingScene__
#define __DGproto__CardSettingScene__

#include "cocos2d.h"
#include "AlertEngine.h"
#include "GameData.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "CardListViewer.h"
//#include <deque>
#include <map>

USING_NS_CC;
using namespace std;

class CardSettingScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CardSettingScene);
	
private:
	CCSprite* selected_card_img;
	CCMenu* selected_card_menu;
	CCSprite* card_option_case;
	CCLabelTTF* card_option_script;
	CCNode* star_parent;
	
	CCSprite* selected_img;
	CCSprite* check_img;
	
	map<int, CCPoint> align_default_position_list;
	
	CCSprite* align_list_img;
	
	CardListViewer* my_clv;
	
	CCPoint inner_card_distance;
	
	int recent_mounted_number;
	
	CCPoint getContentPosition(int t_tag);
	void removeMountingCard();
	void mountingCard(int card_stage, int card_level);
	
	void createCardList();
	void alignChange();
	
	bool is_menu_enable;
	void alertAction(int t1, int t2);
	virtual void keyBackClicked(void);
};

#endif /* defined(__DGproto__CardSettingScene__) */
