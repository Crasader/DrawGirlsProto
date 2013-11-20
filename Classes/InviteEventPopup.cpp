//
//  InviteEventPopup.cpp
//  DGproto
//
//  Created by ksoo k on 2013. 11. 20..
//
//

#include "InviteEventPopup.h"

#include "RankPopup.h"
void InviteEventPopup::myInit(CCObject* t_close, SEL_CallFunc d_close)
{
	setTouchEnabled(true);
	target_close = t_close;
	delegate_close = d_close;
	
	m_currentInviteCount = myDSH->getUserIntForStr("invitecount", 0);
	m_inviteCountFnt = CCLabelBMFont::create(CCString::createWithFormat("%d", m_currentInviteCount)->getCString(),
																					 "combo.fnt");
	m_inviteCountFnt->setPosition(ccp(160, 220));
	addChild(m_inviteCountFnt, kInvite_Z_count);
	
	
	
	m_currentSelectSprite = NULL;
	
	used_card_img = NULL;
	loading_card_number = 0;
	after_loading_card_number = 0;
	last_selected_card_number = 0;
	
	//		gray = CCSprite::create("back_gray.png");
	//		gray->setPosition(ccp(240,160));
	//		gray->setContentSize(CCSizeMake(600, 400));
	//		addChild(gray, kRP_Z_gray);
	
	CCSprite* back = CCSprite::create("invite_back.png");
	back->setPosition(ccp(240,160));
	addChild(back, kInvite_Z_back);
	
	CCMenuLambda* _menu = CCMenuLambda::create();
	_menu->setTouchPriority(-200);
	back->addChild(_menu);
	_menu->setPosition(ccp(0, 0));
	
	CCMenuItemLambda* closeBtn = CCMenuItemImageLambda::create(
																														 "cardsetting_close.png", "cardsetting_close.png",
																														 [=](CCObject*){
																															 (target_close->*delegate_close)();
																															 removeFromParent();
																															 
																														 });
	closeBtn->setPosition(ccp(440, 290));
	_menu->addChild(closeBtn);
	
	CCMenuItemLambda* rankBtn = CCMenuItemImageLambda::create(
																														"rank_friend_rank.png", "rank_friend_rank.png",
																														[=](CCObject*){
																															//																																 (target_close->*delegate_close)();
																															RankPopup* t_rp = RankPopup::create(t_close, d_close);
																															getParent()->addChild(t_rp, this->getZOrder());
																															removeFromParent();
																															
																														});
	rankBtn->setPosition(ccp(380, 290));
	
	_menu->addChild(rankBtn);
	
	
	loadRank();
}
