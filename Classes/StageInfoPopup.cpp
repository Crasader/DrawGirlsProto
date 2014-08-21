//
//  StageInfoPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 21..
//
//

#include "StageInfoPopup.h"
#include "ServerDataSave.h"
#include "StarGoldData.h"
#include "SilhouetteData.h"
#include "CumberShowWindow.h"
#include "DataStorageHub.h"

StageInfoPopup* StageInfoPopup::create( CCObject* t_close, SEL_CallFunc d_close, int t_stage_number )
{
	StageInfoPopup* t_tnp = new StageInfoPopup();
	t_tnp->myInit(t_close, d_close, t_stage_number);
	t_tnp->autorelease();
	return t_tnp;
}

void StageInfoPopup::myInit( CCObject* t_close, SEL_CallFunc d_close, int t_stage_number )
{
	target_close = t_close;
	delegate_close = d_close;
	stage_number = t_stage_number;
	
	back_img = NULL;
	close_menu = NULL;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kSIP_Z_gray);
	
	setBack();
	
	touched_number = 0;
	is_menu_enable = false;
	setTouchEnabled(true);
}

void StageInfoPopup::onEnter()
{
	CCLayer::onEnter();
	showPopup();
}

void StageInfoPopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.3f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* back_move = CCMoveTo::create(0.5f, ccp(240, 160));
	CCCallFunc* back_call = CCCallFunc::create(this, callfunc_selector(StageInfoPopup::endShowPopup));
	CCSequence* back_seq = CCSequence::createWithTwoActions(back_move, back_call);
	back_img->runAction(back_seq);
}

void StageInfoPopup::endShowPopup()
{
	is_menu_enable = true;
}

void StageInfoPopup::hidePopup()
{
	is_menu_enable = false;
	CCFadeTo* gray_fade = CCFadeTo::create(0.3f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* back_move = CCMoveTo::create(0.5f, ccp(240, -160));
	CCCallFunc* back_call = CCCallFunc::create(this, callfunc_selector(StageInfoPopup::endHidePopup));
	CCSequence* back_seq = CCSequence::createWithTwoActions(back_move, back_call);
	back_img->runAction(back_seq);
}

void StageInfoPopup::endHidePopup()
{
	(target_close->*delegate_close)();
	removeFromParent();
}

void StageInfoPopup::setBack()
{
	back_img = CCSprite::create("stageinfo_back.png");
	back_img->setPosition(ccp(240,-160));
	addChild(back_img, kSIP_Z_back);
	
	CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
	CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(StageInfoPopup::menuAction));
	close_item->setTag(kSIP_MT_close);
	close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(getContentPosition(kSIP_MT_close));
	back_img->addChild(close_menu, kSIP_Z_content);
	
	CCLabelTTF* stage_label = CCLabelTTF::create(CCString::createWithFormat("%d - %d", NSDS_GI(stage_number, kSDS_SI_puzzle_i), stage_number)->getCString(), mySGD->getFont().c_str(), 16);
	stage_label->setPosition(getContentPosition(kSIP_MT_stageLabel));
	back_img->addChild(stage_label, kSIP_Z_content);
	
	int found_puzzle_number = -1;
	int puzzle_cnt = NSDS_GI(kSDS_GI_puzzleListCount_i);
	for(int i=1;i<=puzzle_cnt && found_puzzle_number == -1;i++)
	{
		int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i);
		int start_stage = NSDS_GI(puzzle_number, kSDS_PZ_startStage_i);
		int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
		
		if(stage_number >= start_stage && stage_number < start_stage+stage_count)
			found_puzzle_number = puzzle_number;
	}
	
	CCLabelTTF* stage_level = CCLabelTTF::create(CCString::createWithFormat("난이도 %d", NSDS_GI(found_puzzle_number, kSDS_PZ_stage_int1_level_i, stage_number))->getCString(), mySGD->getFont().c_str(), 16);
	stage_level->setPosition(ccpAdd(getContentPosition(kSIP_MT_stageLabel), ccp(175,0)));
	back_img->addChild(stage_level, kSIP_Z_content);
	
	
	int step1_card = NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, 1);
	int step2_card = NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, 2);
	int step3_card = NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, 3);
	
	CCLabelTTF* step1_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, step1_card))->getCString(), mySGD->getFont().c_str(), 17);
	step1_rank->setColor(ccBLACK);
	step1_rank->setPosition(getContentPosition(kSIP_MT_step1Rank));
	back_img->addChild(step1_rank, kSIP_Z_content);
	
	CCLabelTTF* step2_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, step2_card))->getCString(), mySGD->getFont().c_str(), 17);
	step2_rank->setColor(ccBLACK);
	step2_rank->setPosition(getContentPosition(kSIP_MT_step2Rank));
	back_img->addChild(step2_rank, kSIP_Z_content);
	
	CCLabelTTF* step3_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, step3_card))->getCString(), mySGD->getFont().c_str(), 17);
	step3_rank->setColor(ccBLACK);
	step3_rank->setPosition(getContentPosition(kSIP_MT_step3Rank));
	back_img->addChild(step3_rank, kSIP_Z_content);
	
	
//	CCLabelTTF* step1_reward = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_reward_i, step1_card))->getCString(), mySGD->getFont().c_str(), 17);
//	step1_reward->setPosition(getContentPosition(kSIP_MT_step1Reward));
//	back_img->addChild(step1_reward, kSIP_Z_content);
//	
//	CCLabelTTF* step2_reward = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_reward_i, step2_card))->getCString(), mySGD->getFont().c_str(), 17);
//	step2_reward->setPosition(getContentPosition(kSIP_MT_step2Reward));
//	back_img->addChild(step2_reward, kSIP_Z_content);
//	
//	CCLabelTTF* step3_reward = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_reward_i, step3_card))->getCString(), mySGD->getFont().c_str(), 17);
//	step3_reward->setPosition(getContentPosition(kSIP_MT_step3Reward));
//	back_img->addChild(step3_reward, kSIP_Z_content);
	
	
	if(mySGD->isHasGottenCards(step1_card) > 0)
	{
		CCSprite* t_complete = CCSprite::create("stageinfo_complete.png");
		t_complete->setPosition(ccpAdd(getContentPosition(kSIP_MT_step1Reward), ccp(-15, 5)));
		back_img->addChild(t_complete, kSIP_Z_content);
	}
	
	if(mySGD->isHasGottenCards(step2_card) > 0)
	{
		CCSprite* t_complete = CCSprite::create("stageinfo_complete.png");
		t_complete->setPosition(ccpAdd(getContentPosition(kSIP_MT_step2Reward), ccp(-15, 5)));
		back_img->addChild(t_complete, kSIP_Z_content);
	}
	
	if(mySGD->isHasGottenCards(step3_card) > 0)
	{
		CCSprite* t_complete = CCSprite::create("stageinfo_complete.png");
		t_complete->setPosition(ccpAdd(getContentPosition(kSIP_MT_step3Reward), ccp(-15, 5)));
		back_img->addChild(t_complete, kSIP_Z_content);
	}
	
	CumberShowWindow* monster_img = CumberShowWindow::create(stage_number);
	monster_img->setPosition(getContentPosition(kSIP_MT_monster));
	back_img->addChild(monster_img, kSIP_Z_content);
	
	CCLabelTTF* mission_label = CCLabelTTF::create(mySD->getConditionContent(stage_number).c_str(), mySGD->getFont().c_str(), 17);
	mission_label->setPosition(getContentPosition(kSIP_MT_mission));
	back_img->addChild(mission_label, kSIP_Z_content);
}

void StageInfoPopup::menuAction(CCObject *sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kSIP_MT_close)
	{
		hidePopup();
	}
}

CCPoint StageInfoPopup::getContentPosition( int t_tag )
{
	CCPoint return_value;
	
	if(t_tag == kSIP_MT_close)
		return_value = ccp(345, 253);
	else if(t_tag == kSIP_MT_stageLabel)
		return_value = ccp(60, 248);
	else if(t_tag == kSIP_MT_step1Rank)
		return_value = ccp(47, 168);
	else if(t_tag == kSIP_MT_step2Rank)
		return_value = ccp(47, 111);
	else if(t_tag == kSIP_MT_step3Rank)
		return_value = ccp(47, 53);
	else if(t_tag == kSIP_MT_step1Reward)
		return_value = ccp(130, 163);
	else if(t_tag == kSIP_MT_step2Reward)
		return_value = ccp(130, 106);
	else if(t_tag == kSIP_MT_step3Reward)
		return_value = ccp(130, 48);
	else if(t_tag == kSIP_MT_monster)
		return_value = ccp(258,165);
	else if(t_tag == kSIP_MT_mission)
		return_value = ccp(260, 42);
	
	return return_value;
}

bool StageInfoPopup::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number != 0)		return true;
	if(close_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kSIP_MT_close;
	return true;
}

void StageInfoPopup::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kSIP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
}

void StageInfoPopup::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kSIP_MT_close){						close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
}

void StageInfoPopup::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kSIP_MT_close){						close_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
}

void StageInfoPopup::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}