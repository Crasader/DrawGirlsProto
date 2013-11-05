//
//  StageSettingScene.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 18..
//
//

#ifndef __DGproto__StageSettingScene__
#define __DGproto__StageSettingScene__

#include "cocos2d.h"
#include "AlertEngine.h"
#include "GameData.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include <deque>
#include "ItemListViewer.h"
#include "HeartTime.h"

USING_NS_CC;
using namespace std;

class StageSettingScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(StageSettingScene);
	
private:
	int selected_stage;
	vector<ITEM_CODE> item_list;
	deque<bool> is_selected_item;
	deque<int> card_options;
	bool is_menu_enable;
	
	ItemListViewer* my_ilv;
	
	HeartTime* heart_time;
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked(void);
	
	CCPoint getContentPosition(int t_tag);
	void popupClose();
	
	void itemSetting();
	
	void buySuccessItem(int t_clicked_item_number, int cnt);
	
	string convertToItemCodeToItemName(ITEM_CODE t_code);
};

#endif /* defined(__DGproto__StageSettingScene__) */
