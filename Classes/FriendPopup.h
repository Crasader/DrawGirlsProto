//
//  AchievePopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 1. 4..
//
//

#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "AchieveData.h"
#include "jsoncpp/json.h"

#include "CCMenuLambda.h"
#include "BackKeyBase.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum class FriendTabCode{
	kList = 0,
	kAdd,
	kManage
};
class CommonButton;
class LoadingLayer;
class ScrollBar;
class KSLabelTTF;

class FriendPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate, public CCEditBoxDelegate//, public BackKeyBase
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	virtual bool init();
	virtual ~FriendPopup()
	{
		if(input_text1)
		{
			input_text1->removeFromParent();
		}
	}
	
	// a selector callback
	void menuAction(CCObject* pSender);
	virtual void editBoxReturn(CCEditBox* editBox);
	// preprocessor macro for "static create()" constructor ( node() deprecated )
	CREATE_FUNC(FriendPopup);
	
	virtual void onEnter();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
private:
	CCEditBox* input_text1;
	
	FriendTabCode recent_code;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* gray;
	CCSprite* main_case;
	
	//	ScrollBar* m_scrollBar;
	std::vector<std::string> m_sendList; // 친구요청 보낸 리스트. 버튼 감추기 위해 필요함.
	std::function<void(CCObject*)> m_manageButtonCallback;
	std::function<void(CCObject*)> m_listButtonCallback;
	bool is_menu_enable;
	int m_touchPriority;
	CCMenuLambda* tab_menu;
	CCMenuItemLambda* list_menu;
	CCMenuItemLambda* add_menu;
	CCMenuItemLambda* manage_menu;
	
	CCNode* m_friendListContainer;
	CCNode* m_friendAddContainer;
	CCNode* m_friendManageContainer;
	Json::Value m_votedFriendList;
	Json::Value m_friendList;
//	CCMenuItem* all_reward_menu;
	
	CCTableView* friend_table;
	CCLabelTTF* empty_ment;
	
//	vector<AchieveGroup> achieve_list;
	
	void showPopup();
	void endShowPopup();
	
	void hidePopup();
	void endHidePopup();
	
	
	LoadingLayer* loading_layer;
	void resultSaveUserData(Json::Value result_data);
	
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	
	virtual void scrollViewDidScroll(CCScrollView* view);
	virtual void scrollViewDidZoom(CCScrollView* view);
	
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCSize cellSizeForTable(CCTableView *table);
	
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	virtual void registerWithTouchDispatcher ();
	
	void setListMenu();
	void setAddMenu();
	void setManageMenu();
	
	void setFriendTable();
	void resultAllTakeSaveUserData(Json::Value result_data);
};

