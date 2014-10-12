#pragma once
#include "cocos2d.h"
#include "hspConnector.h"
#include "cocos-ext.h"
#include "KSUtil.h"
#include "CCMenuLambda.h"
#include "GDWebSprite.h"
#include "DataStorageHub.h"
#include <chrono>
#include "ScrollBar.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"
#include "SilhouetteData.h"
#include "CardCase.h"
#include "DownloadFile.h"
#include "StarGoldData.h"
#include "FriendData.h"
#include "DimmedPopup.h"
#include "CommonButton.h"
USING_NS_CC;

using namespace cocos2d::extension;
using namespace std;

enum FriendPopupZorder{
	kFriendSendHeart,
	kZorderDeleteFriend,
	kProfileImg,
	kUserNickName,
	kLastConnectDate,
	kAddedFriendCount,
	kZorderPopup
};

enum FriendTableViewTag{
	kFriendTableTagTitle = 0x1000,
	kFriendTableTagSend,
	kFriendTableTagMenu,
	kFriendTableTagProfileImg,
	kFriendDelete
};


#define SAFE_REMOVECHILD(X) do{if(X){ X->removeFromParentAndCleanup(true); X = 0;}}while(0);


class FriendListTableView : public CCTableView
{
public:
	static FriendListTableView* create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container);
};
class FriendListPopup : public DimmedPopup, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static FriendListPopup* create(CCObject* t_close, SEL_CallFunc d_close);
	virtual ~FriendListPopup(){
		StageImgLoader::sharedInstance()->removeTD();
	}
	void finishedOpen();
	
	
	
	
	void myInit(CCObject* t_close, SEL_CallFunc d_close);
	
	
	
	void loadRank();
	
	
	void drawRank();
	
	
	////////////////////////////////////////////////////////
	// touch button ////////////////////////////////////////
	////////////////////////////////////////////////////////
	
	
	
	
	
	
	
	////////////////////////////////////////////////////////
	// tableview	////////////////////////////////////////
	////////////////////////////////////////////////////////
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	
	virtual void scrollViewDidScroll(CCScrollView* view);
	
	virtual void scrollViewDidZoom(CCScrollView* view);
	
	CCSprite* used_card_img;
	int loading_card_number;
	int after_loading_card_number;
	int last_selected_card_number;
	vector<DownloadFile> df_list;
//	vector<CopyFile> cf_list;
	
	
	
	int ing_download_cnt;
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCSize cellSizeForTable(CCTableView *table);
	
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	
	void touchFriend(CCObject*, CCControlEvent);
	
	CCPoint getContentPosition(int t_tag);
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		return true;
		
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
	}
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
	}
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
protected:
	std::vector<FriendData> m_scoreList;
//	Json::Value m_scoreList;
	
	FriendListTableView* rankTableView;
	
	bool is_menu_enable;
	
	int touched_number;
	CCObject* target_close;
	
	CCSprite* m_currentSelectSprite;
	ScrollBar* m_scrollBar;
	CCLabelBMFont* m_friendLimitFnt;
};


