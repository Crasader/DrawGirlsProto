//
//  MiniGamePopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 30..
//
//

#include "MiniGamePopup.h"
#include "DataStorageHub.h"
#include "CountingGame.h"
#include "Dodge.h"
#include "SlidingPuzzle.h"
#include "CardMatching.h"
#include "GoldClicker.h"
#include "CountingBMLabel.h"

enum MiniGameZorder{
	kMiniGameZorder_game = 1,
	kMiniGameZorder_curtain,
	kMiniGameZorder_title,
	kMiniGameZorder_content,
	kMiniGameZorder_menu
};
using namespace placeholders;
MiniGamePopup* MiniGamePopup::create(MiniGameCode t_code, function<void(void)> t_end_func)
{
	MiniGamePopup* t_mgp = new MiniGamePopup();
	if(t_mgp && t_mgp->init(t_code, t_end_func))
	{
		t_mgp->autorelease();
		return t_mgp;
	}
	CC_SAFE_DELETE(t_mgp);
	return NULL;
}

bool MiniGamePopup::init(MiniGameCode t_code, function<void(void)> t_end_func)
{
	if(!CCLayer::init())
	{
		return false;
	}
	
	game_code = t_code;
	end_function = t_end_func;
	
	is_menu_enable = false;
	setTouchEnabled(true);
	
	left_curtain = CCSprite::create("curtain_left.png");
	left_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
	left_curtain->setAnchorPoint(ccp(1.f, 0.5f));
	left_curtain->setPosition(ccp(-80, 160));
	addChild(left_curtain, kMiniGameZorder_curtain);
	
	right_curtain = CCSprite::create("curtain_left.png");
	right_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
	right_curtain->setFlipX(true);
	right_curtain->setAnchorPoint(ccp(0.f, 0.5f));
	right_curtain->setPosition(ccp(560,160));
	addChild(right_curtain, kMiniGameZorder_curtain);
	
	title_bonusgame = CCSprite::create("bonusgame_title.png");
	title_bonusgame->setPosition(ccp(240, 500));
	addChild(title_bonusgame, kMiniGameZorder_title);
	
	content_back = CCSprite::create("bonusgame_tipbox.png");
	content_back->setPosition(ccp(650,150));
	addChild(content_back, kMiniGameZorder_content);
	CCSprite* n_start = CCSprite::create("bonusgame_start.png");
	CCSprite* s_start = CCSprite::create("bonusgame_start.png");
	s_start->setColor(ccGRAY);
	
	CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(MiniGamePopup::menuAction));
	
	start_menu = CCMenu::createWithItem(start_item);
	start_menu->setPosition(ccp(240, -200));
	addChild(start_menu, kMiniGameZorder_menu);
	
	start_menu->setTouchPriority(-171);
	
	string title_filename;
	string content_filename;
	
	if(game_code == kMiniGameCode_touchtouch)
	{
		title_filename = "minigame_start_touch.png";
		content_filename = "minigame_start_tip.png";
	}
	
	CCSprite* title_gamename = CCSprite::create(title_filename.c_str());
	title_gamename->setPosition(ccp(title_bonusgame->getContentSize().width/2.f, -title_gamename->getContentSize().height/2.f-7));
	title_bonusgame->addChild(title_gamename);
	
	CCSprite* content_front = CCSprite::create(content_filename.c_str());
	content_front->setPosition(ccp(content_back->getContentSize().width/2.f, content_back->getContentSize().height/2.f-10));
	content_back->addChild(content_front);
	
	
	return true;
}

void MiniGamePopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
}

void MiniGamePopup::showPopup()
{
	CCMoveTo* left_move = CCMoveTo::create(0.3f, ccp(240,160));
	left_curtain->runAction(left_move);
	
	CCMoveTo* right_move = CCMoveTo::create(0.3f, ccp(240,160));
	right_curtain->runAction(right_move);
	
	CCMoveTo* title_move = CCMoveTo::create(0.3f, ccp(240, 270));
	title_bonusgame->runAction(title_move);
	
	CCMoveTo* content_move = CCMoveTo::create(0.3f, ccp(240, 150));
	content_back->runAction(content_move);
	
	CCMoveTo* menu_move = CCMoveTo::create(0.3f, ccp(240, 65));
	CCCallFunc* menu_call = CCCallFunc::create(this, callfunc_selector(MiniGamePopup::endShowPopup));
	CCSequence* menu_seq = CCSequence::createWithTwoActions(menu_move, menu_call);
	start_menu->runAction(menu_seq);
}

void MiniGamePopup::endShowPopup()
{
	is_menu_enable = true;
}

void MiniGamePopup::hidePopup(CCObject* pSelectorTarget, SEL_CallFunc selector, int ruby, int gold, int social)
{
	left_curtain->setVisible(true);
	right_curtain->setVisible(true);
	
	keep_gold = gold;
	keep_ruby = ruby;
	keep_social = social;
	
	CCMoveTo* left_move1 = CCMoveTo::create(0.3f, ccp(240,160));
//	CCDelayTime* left_delay = CCDelayTime::create(1.f);
//	CCMoveTo* left_move = CCMoveTo::create(0.3f, ccp(-80,160));
//	CCSequence* left_seq = CCSequence::create(left_move1, left_delay, left_move, NULL);
	left_curtain->runAction(left_move1);
	
	CCMoveTo* right_move1 = CCMoveTo::create(0.3f, ccp(240,160));
	CCCallFunc* right_end = CCCallFunc::create(this, callfunc_selector(MiniGamePopup::startCalcAction));
	CCCallFunc* remove_minigame = CCCallFunc::create(pSelectorTarget, selector);
	CCSequence* right_seq = CCSequence::create(right_move1, right_end, remove_minigame, NULL);
//	CCDelayTime* right_delay = CCDelayTime::create(1.f);
//	CCMoveTo* right_move = CCMoveTo::create(0.3f, ccp(560,160));
//	CCCallFunc* right_call = CCCallFunc::create(this, callfunc_selector(MiniGamePopup::endHidePopup));
//	CCCallFunc* middle_call = CCCallFunc::create(pSelectorTarget, selector);
//	CCSequence* right_seq = CCSequence::create(right_move1, right_delay, middle_call, right_move, right_call, NULL);
	right_curtain->runAction(right_seq);
}

void MiniGamePopup::startCalcAction()
{
	reward_title = CCSprite::create("minigame_reward_title.png");
	reward_title->setPosition(ccp(240,240+200));
	addChild(reward_title, kMiniGameZorder_title);
	
	CCMoveTo* title_move = CCMoveTo::create(0.3f, ccp(240,240));
	reward_title->runAction(title_move);
	
	reward_case = CCSprite::create("minigame_reward_box.png");
	reward_case->setPosition(ccp(240-480, 150));
	addChild(reward_case, kMiniGameZorder_content);
	
	reward_gold = CountingBMLabel::create("0", "etc_font.fnt", 1.f, "%d");
	reward_gold->setPosition(ccp(210,44+25));
	reward_case->addChild(reward_gold);
	
	reward_ruby = CountingBMLabel::create("0", "etc_font.fnt", 1.f, "%d");
	reward_ruby->setPosition(ccp(210,42.5f));
	reward_case->addChild(reward_ruby);
	
	reward_social = CountingBMLabel::create("0", "etc_font.fnt", 1.f, "%d");
	reward_social->setPosition(ccp(210,41-25));
	reward_case->addChild(reward_social);
	
	CCMoveTo* case_move = CCMoveTo::create(0.3f, ccp(240,150));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MiniGamePopup::setCalcInfo));
	CCSequence* t_seq = CCSequence::createWithTwoActions(case_move, t_call);
	reward_case->runAction(t_seq);
}

void MiniGamePopup::setCalcInfo()
{
	reward_gold->setString(CCString::createWithFormat("%d", keep_gold)->getCString());
	reward_ruby->setString(CCString::createWithFormat("%d", keep_ruby)->getCString());
	reward_social->setString(CCString::createWithFormat("%d", keep_social)->getCString());
	
	
	CCSprite* n_ok = CCSprite::create("minigame_reward_conform.png");
	CCSprite* s_ok = CCSprite::create("minigame_reward_conform.png");
	s_ok->setColor(ccGRAY);
	
	CCMenuItemLambda* ok_item = CCMenuItemSpriteLambda::create(n_ok, s_ok, [=](CCObject* sender)
															   {
																   startOutAction();
															   });
	ok_menu = CCMenuLambda::createWithItem(ok_item);
	ok_menu->setPosition(ccp(240,70));
	addChild(ok_menu, kMiniGameZorder_menu);
	ok_menu->setTouchPriority(-180);
	
	ok_menu->setVisible(false);
	
	CCDelayTime* t_delay = CCDelayTime::create(1.3f);
	CCShow* t_show = CCShow::create();
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_show);
	
	ok_menu->runAction(t_seq);
}

void MiniGamePopup::startOutAction()
{
	ok_menu->setVisible(false);
	reward_title->runAction(CCMoveTo::create(0.3f, ccp(240,240+200)));
	reward_case->runAction(CCSequence::create(CCMoveTo::create(0.3f, ccp(240+480, 150)), CCCallFunc::create(this, callfunc_selector(MiniGamePopup::endHidePopup)), NULL));
	
	left_curtain->runAction(CCMoveTo::create(0.3f, ccp(-80,160)));
	right_curtain->runAction(CCMoveTo::create(0.3f, ccp(560,160)));
}

void MiniGamePopup::endHidePopup()
{
	end_function();
	removeFromParent();
}

void MiniGamePopup::menuAction(CCObject *pSender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	miniGameStart = nullptr;

	GoldClicker* goldClicker = GoldClicker::create(-180, bind(&MiniGamePopup::hidePopup, this, _1, _2, _3, _4, _5));
	miniGameStart = bind(&GoldClicker::startSchedule, goldClicker);
	addChild(goldClicker, kMiniGameZorder_game);

	//if(game_code == kMiniGameCode_counting)
	//{
		//CountingGame* game = CountingGame::create(-180, bind(&MiniGamePopup::hidePopup, this, _1, _2));
		//miniGameStart = bind(&CountingGame::startSchedule, game);
		//addChild(game, kMiniGameZorder_game);
	//}
	//else if(game_code == kMiniGameCode_slidingPuzzle)
	//{
		//SlidingPuzzle* slidingPuzzle = SlidingPuzzle::create(-180, bind(&MiniGamePopup::hidePopup, this, _1, _2));
		//miniGameStart = bind(&SlidingPuzzle::startSchedule, slidingPuzzle);
		//addChild(slidingPuzzle, kMiniGameZorder_game);
	//}
	//else if(game_code == kMiniGameCode_cardMatch)
	//{
		//CardMatching* card = CardMatching::create(-180, bind(&MiniGamePopup::hidePopup, this, _1, _2));
		//miniGameStart = bind(&CardMatching::scheduleUpdate, card);
		//addChild(card, kMiniGameZorder_game);
	//}
	//else if(game_code == kMiniGameCode_dodge)
	//{
		//Dodge* dodge = Dodge::create(-180, bind(&MiniGamePopup::hidePopup, this, _1, _2));
		//addChild(dodge, kMiniGameZorder_game);
	//}
	
	CCMoveTo* left_move = CCMoveTo::create(0.3f, ccp(-80,160));
	left_curtain->runAction(left_move);
	
	CCMoveTo* right_move = CCMoveTo::create(0.3f, ccp(560,160));
	right_curtain->runAction(right_move);
	
	CCMoveTo* title_move = CCMoveTo::create(0.3f, ccp(240, 500));
	title_bonusgame->runAction(title_move);
	
	CCMoveTo* content_move = CCMoveTo::create(0.3f, ccp(-170, 150));
	content_back->runAction(content_move);
	
	CCMoveTo* menu_move = CCMoveTo::create(0.3f, ccp(240, -200));
	CCCallFunc* menu_call = CCCallFunc::create(this, callfunc_selector(MiniGamePopup::startGame));
	CCSequence* menu_seq = CCSequence::createWithTwoActions(menu_move, menu_call);
	start_menu->runAction(menu_seq);
}

void MiniGamePopup::startGame()
{
	left_curtain->setVisible(false);
	right_curtain->setVisible(false);
	title_bonusgame->removeFromParent();
	content_back->removeFromParent();
	if(miniGameStart)
		miniGameStart();
	// mini_game->startGame();
}

bool MiniGamePopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	return true;
}
void MiniGamePopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void MiniGamePopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void MiniGamePopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void MiniGamePopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}








