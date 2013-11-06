//
//  MailPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 30..
//
//

#ifndef __DGproto__MailPopup__
#define __DGproto__MailPopup__

#include "cocos2d.h"
#include "hspConnector.h"
#include "cocos-ext.h"
#include "DataStorageHub.h"

USING_NS_CC;

using namespace cocos2d::extension;
using namespace std;

enum MailPopupZorder{
	kMP_Z_gray = 1,
	kMP_Z_back,
	kMP_Z_close,
	kMP_Z_content,
	kMP_Z_mailTable
	
};

enum MailTableViewTag{
	kMP_MT_title = 1,
	kMP_MT_score,
	kMP_MT_rank,
	kMP_MT_send
};

enum MailPopupMenuTag{
	kMP_MT_close = 1,
	kMP_MT_invite,
	kMP_MT_send_close,
	kMP_MT_invite_close,
	kMP_MT_invite_rank,
	kMP_MT_invite_send,
	kMP_MT_invite_send_close
};

enum MailPopupState{
	kMP_State_rank = 1,
	kMP_State_send,
	kMP_State_invite,
	kMP_State_invite_send
};

class MailPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static MailPopup* create(CCObject* t_close, SEL_CallFunc d_close)
	{
		MailPopup* t_rp = new MailPopup();
		t_rp->myInit(t_close, d_close);
		t_rp->autorelease();
		return t_rp;
	}
	
	void finishedOpen(){
		loadMail();
	}
	
	void finishedClose(){
		
		(target_close->*delegate_close)();
		removeFromParent();
	}
	
	CCSprite* gray;
private:
	
	CCTableView* mailTableView;
	 
	bool is_menu_enable;
	
	int touched_number;
//	CCMenu* close_menu;
//	CCMenu* invite_menu;
//	CCMenu* send_menu;
//	CCMenu* send_close_menu;
//	CCMenu* invite_close_menu;
//	CCMenu* invite_rank_menu;
//	CCMenu* invite_send_menu;
//	CCMenu* invite_send_close_menu;
	
	
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	
	MailPopupState my_state;
	
	
	CCControlButton *closeBtn;
	
	void myInit(CCObject* t_close, SEL_CallFunc d_close)
	{
		target_close = t_close;
		delegate_close = d_close;
		
		gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160+400));
		gray->setContentSize(CCSizeMake(600, 400));
		addChild(gray, kMP_Z_gray);
		
		CCSprite* back = CCSprite::create("ui_common_popup_back.png");
		back->setPosition(ccp(240,160));
		addChild(back, kMP_Z_back);
		
		CCLabelTTF *btnTitle = CCLabelTTF::create("확인", "Helvetica", 15, CCSizeMake(80, 30), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
		
		CCScale9Sprite *btnBg = CCScale9Sprite::create("ui_common_9_button_red.png",CCRectMake(0, 0, 38, 38),CCRectMake(10, 10, 18, 18));
		
		closeBtn = CCControlButton::create(btnTitle, btnBg);
		closeBtn->setPosition(ccp(240,35));
		closeBtn->setTag(kMP_MT_send);
		closeBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailPopup::closePopup), CCControlEventTouchUpInside);
		closeBtn->setTouchPriority(-201);
		addChild(closeBtn, kMP_Z_close);
		closeBtn->setVisible(false);
		
		
		this->setPositionY(this->getPositionY()-400);
		
		
		this->runAction(CCSequence::create(CCMoveBy::create(0.5, ccp(0,400)),CCCallFunc::create(this, callfunc_selector(MailPopup::finishedOpen)),NULL));
		gray->runAction(CCSpawn::create(CCFadeIn::create(0.5),CCMoveBy::create(0.5,CCPoint(0,-400)),NULL));
		
//
//		CCSprite* n_close = CCSprite::create("ui_common_close.png");
//		CCSprite* s_close = CCSprite::create("ui_common_close.png");
//		s_close->setColor(ccGRAY);
//		
//		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(MailPopup::menuAction));
//		close_item->setTag(kRP_MT_close);
//		
//		close_menu = CCMenu::createWithItem(close_item);
//		close_menu->setPosition(getContentPosition(kRP_MT_close));
//		back->addChild(close_menu);
//		
//		
//		CCSprite* n_invite = CCSprite::create("rank_invite.png");
//		CCSprite* s_invite = CCSprite::create("rank_invite.png");
//		s_invite->setColor(ccGRAY);
//		
//		CCMenuItem* invite_item = CCMenuItemSprite::create(n_invite, s_invite, this, menu_selector(MailPopup::menuAction));
//		invite_item->setTag(kRP_MT_invite);
//		
//		invite_menu = CCMenu::createWithItem(invite_item);
//		invite_menu->setPosition(getContentPosition(kRP_MT_invite));
//		back->addChild(invite_menu);
//		
//		
//		CCSprite* n_send = CCSprite::create("rank_send.png");
//		CCSprite* s_send = CCSprite::create("rank_send.png");
//		s_send->setColor(ccGRAY);
//		
//		CCMenuItem* send_item = CCMenuItemSprite::create(n_send, s_send, this, menu_selector(MailPopup::menuAction));
//		send_item->setTag(kRP_MT_send);
//		
//		send_menu = CCMenu::createWithItem(send_item);
//		send_menu->setPosition(getContentPosition(kRP_MT_send));
//		back->addChild(send_menu);
		
		
		my_state = kMP_State_rank;
		
		is_menu_enable = true;
		touched_number = 0;
		
		setTouchEnabled(true);
	}
	
	
	
	void loadMail()
	{
		Json::Value p;
		//88899626759589034
		p["receiverMemberNo"]=hspConnector::get()->getHSPMemberNo();
		p["gameNo"]=hspConnector::get()->hspNo;
		p["contentType"]=0;
		p["basisYmdt"]="";
		p["pageNo"]=1;
		p["pageSize"]=10;
		
		hspConnector::get()->command("GetReceivedGameMails2",p,[this](Json::Value r)
		 {
			 this->drawMail(r);
			
		 });
	}
	
	
	void drawMail(Json::Value obj){
		
		hspConnector::get()->mailData = obj;
		
		//테이블 뷰 생성 시작 /////////////////////////////////////////////////////////////////////////////////////////
		
		//320x320 테이블 뷰 생성
		mailTableView = CCTableView::create(this, CCSizeMake(400, 225));
		
		mailTableView->setAnchorPoint(CCPointZero);
		
		//kCCScrollViewDirectionVertical : 세로 스크롤, kCCScrollViewDirectionHorizontal : 가로 스크롤
		mailTableView->setDirection(kCCScrollViewDirectionVertical);
		
		//추가시 정렬 기준 설정 kCCTableViewFillTopDown : 아래부분으로 추가됨, kCCTableViewFillBottomUp : 위에서 부터 추가됨.
		mailTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
		
		//기준점 0,0
		mailTableView->setPosition(ccp(40, 65));
		
		//데이터를 가져오고나 터치 이벤트를 반환해줄 대리자를 이 클래스로 설정.
		mailTableView->setDelegate(this);
		this->addChild(mailTableView, kMP_Z_mailTable);
		mailTableView->setTouchPriority(-200);
		//테이블 뷰 생성 끝/////////////////////////////////////////////////////////////////////////////////////////
		
		
		
		closeBtn->setVisible(true);
	}

	
	////////////////////////////////////////////////////////
	// touch button ////////////////////////////////////////
	////////////////////////////////////////////////////////
	
   	void closePopup(CCControlButton *obj, CCControlEvent event){
		
		gray->runAction(CCSpawn::create(CCFadeOut::create(0.5),CCMoveBy::create(0.5,CCPoint(0,400)),NULL));
		this->runAction(CCSequence::create(CCMoveBy::create(0.5, CCPoint(0,-400)),CCCallFunc::create(this, callfunc_selector(MailPopup::finishedClose)),NULL));
	}
	

	
	void removeMail(CCControlButton *obj, CCControlEvent event){
		Json::Value* mail = (Json::Value *)obj->getUserData();
		
		Json::Value p;
		int mailNo =(*mail)["mailNo"].asInt();
		
		p["mailNo"]=mailNo;
		p["targetOption"]="send,recv";
		p["senderMemberNo"]=(*mail)["sender"]["memberNo"].asInt64();
		p["receiverMemberNo"]=(*mail)["receiver"]["memberNo"].asInt64();
		
		hspConnector::get()->command("DeleteGameMail2",p,[this,obj,mailNo](Json::Value r)
		{
			Json::Value newMailList;
			
			if(r["header"].get("status","1").asInt()==0){
			
				
				for(int i=0;i<hspConnector::get()->mailData["mailList"].size();i++){
					if(hspConnector::get()->mailData["mailList"][i]["mailNo"].asInt()!=mailNo){
						newMailList.append(hspConnector::get()->mailData["mailList"][i]);
					}
				}
				
				hspConnector::get()->mailData["mailList"]=newMailList;
				
				if(myDSH->getIntegerForKey(kDSH_Key_heartCnt)<5){
					myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+1);
				}
				this->mailTableView->reloadData();
			}
		});
	
	}
	
	
	
	////////////////////////////////////////////////////////
	// tableview	////////////////////////////////////////
	////////////////////////////////////////////////////////
	
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx){
		CCTableViewCell *cell = table->dequeueCell();
		CCLabelTTF* title;
		CCControlButton* sendBtn;
		CCLabelTTF* score;
		CCLabelTTF* rank;
		Json::Value* mail = hspConnector::get()->getMailByIndex(idx);
		
		if(!cell){
			cell = new CCTableViewCell();
			
			cell->autorelease();
			
			CCScale9Sprite* bg = CCScale9Sprite::create("ui_rank_cell_back.png",CCRectMake(0, 0, 238, 45),CCRectMake(80, 10, 78, 25));
			bg->setContentSize(CCSizeMake(400, 45));
			bg->setPosition(CCPointZero);
			bg->setAnchorPoint(CCPointZero);
			cell->addChild(bg,1);
			
			
			CCLabelTTF *btnTitle = CCLabelTTF::create("get", "Helvetica", 10, CCSizeMake(30, 30), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
			//btnTitle->setContentSize(CCSizeMake(40, 40));
			CCScale9Sprite *btnBg = CCScale9Sprite::create("ui_common_9_button_brown.png",CCRectMake(0, 0, 38, 38),CCRectMake(10, 10, 18, 18));
			
			sendBtn = CCControlButton::create(btnTitle, btnBg);
			sendBtn->setPosition(ccp(370,22));
			sendBtn->setTag(kMP_MT_send);
			sendBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailPopup::removeMail), CCControlEventTouchUpInside);
			sendBtn->setTouchPriority(-201);
			
			cell->addChild(sendBtn,2);
			
			title = CCLabelTTF::create("","Helvetica",12);
			title->setPosition(ccp(90,28));
			title->setAnchorPoint(CCPointZero);
			title->setTag(kMP_MT_title);
			cell->addChild(title,2);
			
			
			score = CCLabelTTF::create("","Helvetica",20);
			score->setPosition(ccp(90,5));
			score->setAnchorPoint(CCPointZero);
			score->setTag(kMP_MT_score);
			cell->addChild(score,2);
			
			rank = CCLabelTTF::create("","Helvetica",25);
			rank->setPosition(ccp(10,10));
			rank->setAnchorPoint(CCPointZero);
			rank->setTag(kMP_MT_rank);
			cell->addChild(rank,2);
			
		}else{
			title=(CCLabelTTF*)cell->getChildByTag(kMP_MT_title);
			score=(CCLabelTTF*)cell->getChildByTag(kMP_MT_score);
			rank=(CCLabelTTF*)cell->getChildByTag(kMP_MT_rank);
			sendBtn=(CCControlButton *)cell->getChildByTag(kMP_MT_send);
		}
		
		
		sendBtn->setUserData((void *)mail);
		
		title->setString((*mail)["sentTimestamp"].asString().c_str());
		score->setString((*mail)["sender"]["nickname"].asString().c_str());
		rank->setString((*mail)["contentType"].asString().c_str());
		
		return cell;
	}
	
	virtual void scrollViewDidScroll(CCScrollView* view) {
		
	}
	
    virtual void scrollViewDidZoom(CCScrollView* view) {
		
	}
    
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell){
		
		CCLog("touch!!");
		
	}
    virtual CCSize cellSizeForTable(CCTableView *table){
		return CCSizeMake(400, 47);
	}
	
    virtual unsigned int numberOfCellsInTableView(CCTableView *table){
		return hspConnector::get()->mailData["mailList"].size();
	}
	
	
	void touchFriend(CCObject*, CCControlEvent){
		
		
	}
		
	CCPoint getContentPosition(int t_tag)
	{
		CCPoint return_value;
		
		if(t_tag == kMP_MT_invite)					return_value = ccp(240,43);
		else if(t_tag == kMP_MT_close)				return_value = ccp(345,283);
		else if(t_tag == kMP_MT_send)				return_value = ccp(240,230);
		else if(t_tag == kMP_MT_send_close)			return_value = ccp(193,59);
		else if(t_tag == kMP_MT_invite_close)		return_value = ccp(345,283);
		else if(t_tag == kMP_MT_invite_rank)		return_value = ccp(240,43);
		else if(t_tag == kMP_MT_invite_send)		return_value = ccp(240,230);
		else if(t_tag == kMP_MT_invite_send_close)	return_value = ccp(193,59);
		
		return return_value;
	}
	
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
////			CCMenuItem* invite_close_item = CCMenuItemSprite::create(n_invite_close, s_invite_close, this, menu_selector(MailPopup::menuAction));
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
////			CCMenuItem* invite_rank_item = CCMenuItemSprite::create(n_invite_rank, s_invite_rank, this, menu_selector(MailPopup::menuAction));
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
////			CCMenuItem* invite_send_item = CCMenuItemSprite::create(n_invite_send, s_invite_send, this, menu_selector(MailPopup::menuAction));
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
//			CCMenuItem* send_close_item = CCMenuItemSprite::create(n_send_close, s_send_close, this, menu_selector(MailPopup::menuAction));
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
//			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(MailPopup::menuAction));
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
//			CCMenuItem* invite_item = CCMenuItemSprite::create(n_invite, s_invite, this, menu_selector(MailPopup::menuAction));
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
//			CCMenuItem* send_item = CCMenuItemSprite::create(n_send, s_send, this, menu_selector(MailPopup::menuAction));
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
//			CCMenuItem* invite_send_close_item = CCMenuItemSprite::create(n_invite_send_close, s_invite_send_close, this, menu_selector(MailPopup::menuAction));
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
//			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(MailPopup::menuAction));
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
//			CCMenuItem* invite_item = CCMenuItemSprite::create(n_invite, s_invite, this, menu_selector(MailPopup::menuAction));
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
//			CCMenuItem* send_item = CCMenuItemSprite::create(n_send, s_send, this, menu_selector(MailPopup::menuAction));
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
//			CCMenuItem* invite_close_item = CCMenuItemSprite::create(n_invite_close, s_invite_close, this, menu_selector(MailPopup::menuAction));
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
//			CCMenuItem* invite_rank_item = CCMenuItemSprite::create(n_invite_rank, s_invite_rank, this, menu_selector(MailPopup::menuAction));
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
//			CCMenuItem* invite_send_item = CCMenuItemSprite::create(n_invite_send, s_invite_send, this, menu_selector(MailPopup::menuAction));
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
};

#endif /* defined(__DGproto__MailPopup__) */
