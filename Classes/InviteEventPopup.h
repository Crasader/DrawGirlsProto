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

//enum InvitePopupMenuTag{
//	kRP_MT_close = 0x2000,
//	kRP_MT_invite,
//	kRP_MT_send,
//	kRP_MT_send_close,
//	kRP_MT_invite_close,
//	kRP_MT_invite_rank,
//	kRP_MT_invite_send,
//	kRP_MT_invite_send_close
//};

//enum RankPopupState{
//	kRP_State_rank = 1,
//	kRP_State_send,
//	kRP_State_invite,
//	kRP_State_invite_send
//};

namespace{
	CCSize cellSize3 = CCSizeMake(238, 47);
}
#define SAFE_REMOVECHILD(X) do{if(X){ X->removeFromParentAndCleanup(true); X = 0;}}while(0);


class InviteTableView : public CCTableView
{
public:
	static InviteTableView* create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container);
};
class InviteEventPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static InviteEventPopup* create(CCObject* t_close, SEL_CallFunc d_close);
	virtual ~InviteEventPopup(){
		StageImgLoader::sharedInstance()->removeTD();
	}
	void finishedOpen();
	
	int getInviteIsSendable(std::string userId, int base_s = 60 * 60 * 24 * 31); // 31일.;
	void setInviteSendTime(string userId);
	
	
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
	vector<CopyFile> cf_list;
	
	
	
	int ing_download_cnt;
	
	
	
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCSize cellSizeForTable(CCTableView *table);
	
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	
	void touchFriend(CCObject*, CCControlEvent);
	
	CCPoint getContentPosition(int t_tag);
	
	void menuAction(CCObject* sender)
	{
		//		if(!is_menu_enable)
		//			return;
		//
		//		is_menu_enable = false;
		//
		//		int tag = ((CCNode*)sender)->getTag();
		//
		//		if(tag == kRP_MT_close)
		//		{
		//			(target_close->*delegate_close)();
		//			removeFromParent();
		//		}
		//		else if(tag == kRP_MT_invite)
		//		{
		//			touched_number = -1;
		////			back->removeFromParent();
		////
		////			back = CCSprite::create("rank_invite_back.png");
		////			back->setPosition(ccp(240,160));
		////			addChild(back, kRP_Z_back);
		////
		////			CCSprite* n_invite_close = CCSprite::create("ui_common_close.png");
		////			CCSprite* s_invite_close = CCSprite::create("ui_common_close.png");
		////			s_invite_close->setColor(ccGRAY);
		////
		////			CCMenuItem* invite_close_item = CCMenuItemSprite::create(n_invite_close, s_invite_close, this, menu_selector(RankPopup::menuAction));
		////			invite_close_item->setTag(kRP_MT_invite_close);
		////
		////			invite_close_menu = CCMenu::createWithItem(invite_close_item);
		////			invite_close_menu->setPosition(getContentPosition(kRP_MT_invite_close));
		////			back->addChild(invite_close_menu);
		////
		////
		////			CCSprite* n_invite_rank = CCSprite::create("rank_invite_rank.png");
		////			CCSprite* s_invite_rank = CCSprite::create("rank_invite_rank.png");
		////			s_invite_rank->setColor(ccGRAY);
		////
		////			CCMenuItem* invite_rank_item = CCMenuItemSprite::create(n_invite_rank, s_invite_rank, this, menu_selector(RankPopup::menuAction));
		////			invite_rank_item->setTag(kRP_MT_invite_rank);
		////
		////			invite_rank_menu = CCMenu::createWithItem(invite_rank_item);
		////			invite_rank_menu->setPosition(getContentPosition(kRP_MT_invite_rank));
		////			back->addChild(invite_rank_menu);
		////
		////
		////			CCSprite* n_invite_send = CCSprite::create("rank_invite_send.png");
		////			CCSprite* s_invite_send = CCSprite::create("rank_invite_send.png");
		////			s_invite_send->setColor(ccGRAY);
		////
		////			CCMenuItem* invite_send_item = CCMenuItemSprite::create(n_invite_send, s_invite_send, this, menu_selector(RankPopup::menuAction));
		////			invite_send_item->setTag(kRP_MT_invite_send);
		////
		////			invite_send_menu = CCMenu::createWithItem(invite_send_item);
		////			invite_send_menu->setPosition(getContentPosition(kRP_MT_invite_send));
		////			back->addChild(invite_send_menu);
		//
		//			touched_number = 0;
		//			my_state = kRP_State_invite;
		//			is_menu_enable = true;
		//		}
		//		else if(tag == kRP_MT_send)
		//		{
		//			touched_number = -1;
		//			back->removeFromParent();
		//
		//			back = CCSprite::create("rank_send_back.png");
		//			back->setPosition(ccp(240,160));
		//			addChild(back, kRP_Z_back);
		//
		//			CCSprite* n_send_close = CCSprite::create("option_short_close.png");
		//			CCSprite* s_send_close = CCSprite::create("option_short_close.png");
		//			s_send_close->setColor(ccGRAY);
		//
		//			CCMenuItem* send_close_item = CCMenuItemSprite::create(n_send_close, s_send_close, this, menu_selector(RankPopup::menuAction));
		//			send_close_item->setTag(kRP_MT_send_close);
		//
		//			send_close_menu = CCMenu::createWithItem(send_close_item);
		//			send_close_menu->setPosition(getContentPosition(kRP_MT_send_close));
		//			back->addChild(send_close_menu);
		//
		//			touched_number = 0;
		//			my_state = kRP_State_send;
		//			is_menu_enable = true;
		//		}
		//		else if(tag == kRP_MT_invite_rank)
		//		{
		//			touched_number = -1;
		//			back->removeFromParent();
		//
		//			back = CCSprite::create("rank_back.png");
		//			back->setPosition(ccp(240,160));
		//			addChild(back, kRP_Z_back);
		//
		//			CCSprite* n_close = CCSprite::create("ui_common_close.png");
		//			CCSprite* s_close = CCSprite::create("ui_common_close.png");
		//			s_close->setColor(ccGRAY);
		//
		//			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(RankPopup::menuAction));
		//			close_item->setTag(kRP_MT_close);
		//
		//			close_menu = CCMenu::createWithItem(close_item);
		//			close_menu->setPosition(getContentPosition(kRP_MT_close));
		//			back->addChild(close_menu);
		//
		//
		//			CCSprite* n_invite = CCSprite::create("rank_invite.png");
		//			CCSprite* s_invite = CCSprite::create("rank_invite.png");
		//			s_invite->setColor(ccGRAY);
		//
		//			CCMenuItem* invite_item = CCMenuItemSprite::create(n_invite, s_invite, this, menu_selector(RankPopup::menuAction));
		//			invite_item->setTag(kRP_MT_invite);
		//
		//			invite_menu = CCMenu::createWithItem(invite_item);
		//			invite_menu->setPosition(getContentPosition(kRP_MT_invite));
		//			back->addChild(invite_menu);
		//
		//
		//			CCSprite* n_send = CCSprite::create("rank_send.png");
		//			CCSprite* s_send = CCSprite::create("rank_send.png");
		//			s_send->setColor(ccGRAY);
		//
		//			CCMenuItem* send_item = CCMenuItemSprite::create(n_send, s_send, this, menu_selector(RankPopup::menuAction));
		//			send_item->setTag(kRP_MT_send);
		//
		//			send_menu = CCMenu::createWithItem(send_item);
		//			send_menu->setPosition(getContentPosition(kRP_MT_send));
		//			back->addChild(send_menu);
		//
		//			touched_number = 0;
		//			my_state = kRP_State_rank;
		//			is_menu_enable = true;
		//		}
		//		else if(tag == kRP_MT_invite_close)
		//		{
		//			(target_close->*delegate_close)();
		//			removeFromParent();
		//		}
		//		else if(tag == kRP_MT_invite_send)
		//		{
		//			touched_number = -1;
		//			back->removeFromParent();
		//
		//			back = CCSprite::create("rank_invite_send_back.png");
		//			back->setPosition(ccp(240,160));
		//			addChild(back, kRP_Z_back);
		//
		//			CCSprite* n_invite_send_close = CCSprite::create("option_short_close.png");
		//			CCSprite* s_invite_send_close = CCSprite::create("option_short_close.png");
		//			s_invite_send_close->setColor(ccGRAY);
		//
		//			CCMenuItem* invite_send_close_item = CCMenuItemSprite::create(n_invite_send_close, s_invite_send_close, this, menu_selector(RankPopup::menuAction));
		//			invite_send_close_item->setTag(kRP_MT_invite_send_close);
		//
		//			invite_send_close_menu = CCMenu::createWithItem(invite_send_close_item);
		//			invite_send_close_menu->setPosition(getContentPosition(kRP_MT_invite_send_close));
		//			back->addChild(invite_send_close_menu);
		//
		//			touched_number = 0;
		//			my_state = kRP_State_invite_send;
		//			is_menu_enable = true;
		//		}
		//		else if(tag == kRP_MT_send_close)
		//		{
		//			touched_number = -1;
		//			back->removeFromParent();
		//
		//			back = CCSprite::create("rank_back.png");
		//			back->setPosition(ccp(240,160));
		//			addChild(back, kRP_Z_back);
		//
		//			CCSprite* n_close = CCSprite::create("ui_common_close.png");
		//			CCSprite* s_close = CCSprite::create("ui_common_close.png");
		//			s_close->setColor(ccGRAY);
		//
		//			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(RankPopup::menuAction));
		//			close_item->setTag(kRP_MT_close);
		//
		//			close_menu = CCMenu::createWithItem(close_item);
		//			close_menu->setPosition(getContentPosition(kRP_MT_close));
		//			back->addChild(close_menu);
		//
		//
		//			CCSprite* n_invite = CCSprite::create("rank_invite.png");
		//			CCSprite* s_invite = CCSprite::create("rank_invite.png");
		//			s_invite->setColor(ccGRAY);
		//
		//			CCMenuItem* invite_item = CCMenuItemSprite::create(n_invite, s_invite, this, menu_selector(RankPopup::menuAction));
		//			invite_item->setTag(kRP_MT_invite);
		//
		//			invite_menu = CCMenu::createWithItem(invite_item);
		//			invite_menu->setPosition(getContentPosition(kRP_MT_invite));
		//			back->addChild(invite_menu);
		//
		//
		//			CCSprite* n_send = CCSprite::create("rank_send.png");
		//			CCSprite* s_send = CCSprite::create("rank_send.png");
		//			s_send->setColor(ccGRAY);
		//
		//			CCMenuItem* send_item = CCMenuItemSprite::create(n_send, s_send, this, menu_selector(RankPopup::menuAction));
		//			send_item->setTag(kRP_MT_send);
		//
		//			send_menu = CCMenu::createWithItem(send_item);
		//			send_menu->setPosition(getContentPosition(kRP_MT_send));
		//			back->addChild(send_menu);
		//
		//			touched_number = 0;
		//			my_state = kRP_State_rank;
		//			is_menu_enable = true;
		//		}
		//		else if(tag == kRP_MT_invite_send_close)
		//		{
		//			touched_number = -1;
		//			back->removeFromParent();
		//
		//			back = CCSprite::create("rank_invite_back.png");
		//			back->setPosition(ccp(240,160));
		//			addChild(back, kRP_Z_back);
		//
		//			CCSprite* n_invite_close = CCSprite::create("ui_common_close.png");
		//			CCSprite* s_invite_close = CCSprite::create("ui_common_close.png");
		//			s_invite_close->setColor(ccGRAY);
		//
		//			CCMenuItem* invite_close_item = CCMenuItemSprite::create(n_invite_close, s_invite_close, this, menu_selector(RankPopup::menuAction));
		//			invite_close_item->setTag(kRP_MT_invite_close);
		//
		//			invite_close_menu = CCMenu::createWithItem(invite_close_item);
		//			invite_close_menu->setPosition(getContentPosition(kRP_MT_invite_close));
		//			back->addChild(invite_close_menu);
		//
		//
		//			CCSprite* n_invite_rank = CCSprite::create("rank_invite_rank.png");
		//			CCSprite* s_invite_rank = CCSprite::create("rank_invite_rank.png");
		//			s_invite_rank->setColor(ccGRAY);
		//
		//			CCMenuItem* invite_rank_item = CCMenuItemSprite::create(n_invite_rank, s_invite_rank, this, menu_selector(RankPopup::menuAction));
		//			invite_rank_item->setTag(kRP_MT_invite_rank);
		//
		//			invite_rank_menu = CCMenu::createWithItem(invite_rank_item);
		//			invite_rank_menu->setPosition(getContentPosition(kRP_MT_invite_rank));
		//			back->addChild(invite_rank_menu);
		//
		//
		//			CCSprite* n_invite_send = CCSprite::create("rank_invite_send.png");
		//			CCSprite* s_invite_send = CCSprite::create("rank_invite_send.png");
		//			s_invite_send->setColor(ccGRAY);
		//
		//			CCMenuItem* invite_send_item = CCMenuItemSprite::create(n_invite_send, s_invite_send, this, menu_selector(RankPopup::menuAction));
		//			invite_send_item->setTag(kRP_MT_invite_send);
		//
		//			invite_send_menu = CCMenu::createWithItem(invite_send_item);
		//			invite_send_menu->setPosition(getContentPosition(kRP_MT_invite_send));
		//			back->addChild(invite_send_menu);
		//
		//			touched_number = 0;
		//			my_state = kRP_State_invite;
		//			is_menu_enable = true;
		//		}
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		return true;
		
		//		if(touched_number != 0)		return true;
		//		if(my_state == kRP_State_rank)
		//		{
		//			if(close_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kRP_MT_close;
		//			else if(invite_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kRP_MT_invite;
		//			else if(send_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kRP_MT_send;
		//		}
		//		else if(my_state == kRP_State_invite)
		//		{
		//			if(invite_close_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kRP_MT_invite_close;
		//			else if(invite_rank_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kRP_MT_invite_rank;
		//			else if(invite_send_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kRP_MT_invite_send;
		//		}
		//		else if(my_state == kRP_State_send)
		//		{
		//			if(send_close_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kRP_MT_send_close;
		//		}
		//		else if(my_state == kRP_State_invite_send)
		//		{
		//			if(invite_send_close_menu->ccTouchBegan(pTouch, pEvent))	touched_number = kRP_MT_invite_send_close;
		//		}
		
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		//		if(touched_number == kRP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
		//		else if(touched_number == kRP_MT_invite)					invite_menu->ccTouchMoved(pTouch, pEvent);
		//		else if(touched_number == kRP_MT_send)						send_menu->ccTouchMoved(pTouch, pEvent);
		//		else if(touched_number == kRP_MT_invite_close)				invite_close_menu->ccTouchMoved(pTouch, pEvent);
		//		else if(touched_number == kRP_MT_invite_rank)				invite_rank_menu->ccTouchMoved(pTouch, pEvent);
		//		else if(touched_number == kRP_MT_invite_send)				invite_send_menu->ccTouchMoved(pTouch, pEvent);
		//		else if(touched_number == kRP_MT_send_close)				send_close_menu->ccTouchMoved(pTouch, pEvent);
		//		else if(touched_number == kRP_MT_invite_send_close)			invite_send_close_menu->ccTouchMoved(pTouch, pEvent);
	}
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		//(target_close->*delegate_close)();
		//removeFromParent();
		
		//		if(touched_number == kRP_MT_close){							close_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite){					invite_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_send){						send_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_close){				invite_close_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_rank){				invite_rank_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_send){				invite_send_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		//		else if(touched_number == kRP_MT_send_close){				send_close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_send_close){		invite_send_close_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
	}
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		//		if(touched_number == kRP_MT_close){							close_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite){					invite_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_send){						send_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_close){				invite_close_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_rank){				invite_rank_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_send){				invite_send_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		//		else if(touched_number == kRP_MT_send_close){				send_close_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_send_close){		invite_send_close_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
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
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	CCControlButton *closeBtn;
	
	CCSprite* m_currentSelectSprite;
	ScrollBar* m_scrollBar;
	
	int m_currentInviteCount;
	CCLabelBMFont* m_inviteCountFnt;
};


