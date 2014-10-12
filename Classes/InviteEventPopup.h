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
#include "DimmedPopup.h"
#include "CommonButton.h"
USING_NS_CC;

using namespace cocos2d::extension;
using namespace std;

enum InvitePopupZorder{
	kInvite_Z_gray = 1,
	kInvite_Z_back,
	kInvite_Z_close,
	kInvite_Z_content,
	kInvite_Z_rankTable,
	kInvite_Z_send,
	kInvite_Z_profileImg,
	kInvite_Z_usedCardImg,
	kInvite_Z_count,
};

enum InviteTableViewTag{
	kInviteTagTitle = 0x1000,
	kInviteTagScore,
	kInviteTagRank,
	kInviteTagSend,
	kInviteTagMenu,
	kInviteTagProfileImg,
};



#define SAFE_REMOVECHILD(X) do{if(X){ X->removeFromParentAndCleanup(true); X = 0;}}while(0);


class InviteTableView : public CCTableView
{
public:
	static InviteTableView* create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container);
};


class InviteEventPopup : public DimmedPopup, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static InviteEventPopup* create(CCObject* t_close, SEL_CallFunc d_close);
	virtual ~InviteEventPopup(){
		StageImgLoader::sharedInstance()->removeTD();
	}
	void finishedOpen();
	

	
	
	void myInit(CCObject* t_close, SEL_CallFunc d_close);
	
	
	
	void loadRank();
	
	
	void drawRank(Json::Value obj);
	
	
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
	
	void menuAction(CCObject* sender)
	{
	}
	
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
	
	Json::Value m_scoreList;
	
	InviteTableView* rankTableView;
	
	bool is_menu_enable;
	
	int touched_number;
	CommonButton *closeBtn;
	
	CCSprite* m_currentSelectSprite;
	ScrollBar* m_scrollBar;
	
	int m_currentInviteCount;
	CCLabelBMFont* m_inviteCountFnt;
};


