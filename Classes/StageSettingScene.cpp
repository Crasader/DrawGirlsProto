//
//  StageSettingScene.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 18..
//
//

#include "StageSettingScene.h"
#include "ScreenSide.h"
#include "MyLocalization.h"
//#include "WorldMapScene.h"
#include "PuzzleMapScene.h"
#include "MaingameScene.h"
#include "ItemBuyPopup.h"
#include "CardSettingScene.h"
#include "ListViewerScroll.h"
#include "ChallengePopup.h"
#include "GachaPopup.h"
#include "DurabilityNoti.h"

CCScene* StageSettingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StageSettingScene *layer = StageSettingScene::create();
	
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

enum SSS_Zorder{
	kSSS_Z_back = 1,
	kSSS_Z_selectedImg,
	kSSS_Z_content,
	kSSS_Z_top,
	kSSS_Z_popup
};

enum SSS_MenuTag{
	kSSS_MT_start = 1,
	kSSS_MT_back = 2,
	kSSS_MT_changeCard = 3,
	kSSS_MT_challenge = 4,
	kSSS_MT_gacha = 5,
	kSSS_MT_itemBase = 100,
	kSSS_MT_selectedBase = 200,
	kSSS_MT_itemCntBase = 300,
	kSSS_MT_noti = 9999
};

// on "init" you need to initialize your instance
bool StageSettingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	setKeypadEnabled(true);
	
	selected_stage = mySD->getSilType();
	
	CCSprite* stagesetting_back = CCSprite::create("stagesetting_back.png");
	stagesetting_back->setPosition(ccp(240,160));
	addChild(stagesetting_back, kSSS_Z_back);
	
	CCLabelTTF* stage_label = CCLabelTTF::create(CCString::createWithFormat("%d", selected_stage)->getCString(), mySGD->getFont().c_str(), 25);
	stage_label->setColor(ccBLACK);
	stage_label->setPosition(ccp(227,235));
	addChild(stage_label, kSSS_Z_content);
	
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard); // 1, 2, 3 / 11, 12, 13 / 14, ...
	if(selected_card_number > 0 && myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number) > 0)
	{
		int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, selected_card_number);
		int card_level = NSDS_GI(kSDS_CI_int1_rank_i, selected_card_number);
		
		CCSprite* card_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", card_stage, card_level)->getCString());
		card_img->setScale(0.45);
		card_img->setPosition(getContentPosition(kSSS_MT_changeCard));
		addChild(card_img, kSSS_Z_content);
		
		if(card_level == 3 && mySD->isAnimationStage(card_stage))
		{
			CCSize ani_size = mySD->getAnimationCutSize(card_stage);
			CCSprite* card_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", card_stage, card_level)->getCString(),
												  CCRectMake(0, 0, ani_size.width, ani_size.height));
			card_ani->setPosition(mySD->getAnimationPosition(card_stage));
			card_img->addChild(card_ani);
		}
	}
	else
	{
		myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
	}
	
	mySD->setCardOptions(card_options, selected_card_number);
	
	CCSprite* n_gacha = CCSprite::create("stagesetting_item_gacha_on.png");
	CCSprite* s_gacha = CCSprite::create("stagesetting_item_gacha_on.png");
	s_gacha->setColor(ccGRAY);
	
	CCMenuItem* gacha_item = CCMenuItemSprite::create(n_gacha, s_gacha, this, menu_selector(StageSettingScene::menuAction));
	gacha_item->setTag(kSSS_MT_gacha);
	
	CCMenu* gacha_menu = CCMenu::createWithItem(gacha_item);
	gacha_menu->setPosition(getContentPosition(kSSS_MT_gacha));
	addChild(gacha_menu, kSSS_Z_content);
	
	
	my_ilv = ItemListViewer::create();
	addChild(my_ilv, kSSS_Z_content);
	
	item_list = mySD->getStageItemList(selected_stage);
	itemSetting();
	
	my_ilv->setMaxPositionY();
	
	ListViewerScroll* t_lvs = ListViewerScroll::create(CCRectMake(432, 60, 27, 156), my_ilv, "stagesetting_scroll.png", ccp(443,80), ccp(443,195));
	t_lvs->setTouchEnabled(true);
	addChild(t_lvs, kSSS_Z_content);
	
	my_ilv->setScroll(t_lvs);
	my_ilv->setTouchEnabled(true);
	
	CCMenuItem* changeCard_item = CCMenuItemImage::create("stagesetting_card_button.png", "stagesetting_card_button.png", this, menu_selector(StageSettingScene::menuAction));
	changeCard_item->setTag(kSSS_MT_changeCard);
	
	CCMenu* change_menu = CCMenu::createWithItem(changeCard_item);
	change_menu->setPosition(getContentPosition(kSSS_MT_changeCard));
	addChild(change_menu, kSSS_Z_content);
	
	
	CCSprite* n_backward = CCSprite::create("stagesetting_backward.png");
	CCSprite* s_backward = CCSprite::create("stagesetting_backward.png");
	s_backward->setColor(ccGRAY);
	
	CCMenuItem* backward_item = CCMenuItemSprite::create(n_backward, s_backward, this, menu_selector(StageSettingScene::menuAction));
	backward_item->setTag(kSSS_MT_back);
	
	CCMenu* backward_menu = CCMenu::createWithItem(backward_item);
	backward_menu->setPosition(getContentPosition(kSSS_MT_back));
	addChild(backward_menu, kSSS_Z_content);
	
	
	CCSprite* n_challenge = CCSprite::create("stagesetting_challenge.png");
	CCSprite* s_challenge = CCSprite::create("stagesetting_challenge.png");
	s_challenge->setColor(ccGRAY);
	
	CCMenuItem* challenge_item = CCMenuItemSprite::create(n_challenge, s_challenge, this, menu_selector(StageSettingScene::menuAction));
	challenge_item->setTag(kSSS_MT_challenge);
	
	CCMenu* challenge_menu = CCMenu::createWithItem(challenge_item);
	challenge_menu->setPosition(getContentPosition(kSSS_MT_challenge));
	addChild(challenge_menu, kSSS_Z_content);
	
	
	CCSprite* n_start = CCSprite::create("stagesetting_start.png");
	CCSprite* s_start = CCSprite::create("stagesetting_start.png");
	s_start->setColor(ccGRAY);
	
	CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(StageSettingScene::menuAction));
	start_item->setTag(kSSS_MT_start);
	
	CCMenu* start_menu = CCMenu::createWithItem(start_item);
	start_menu->setPosition(getContentPosition(kSSS_MT_start));
	addChild(start_menu, kSSS_Z_content);
	
	
	CCSprite* n_temp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 30, 30));
	n_temp->setOpacity(0);
	CCSprite* s_temp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 30, 30));
	s_temp->setOpacity(0);
	
	CCMenuItemSprite* temp_item = CCMenuItemSprite::create(n_temp, s_temp, this, menu_selector(StageSettingScene::tempAction));
	temp_item->setTag(1);
	CCMenu* temp_menu = CCMenu::createWithItem(temp_item);
	temp_menu->setPosition(ccp(15,305));
	addChild(temp_menu, kSSS_Z_content);
	
	
	
	CCSprite* top_case = CCSprite::create("test_ui_top.png");
	top_case->setAnchorPoint(ccp(0.5f,1.f));
	top_case->setPosition(ccp(240,320.f));//(myDSH->puzzle_ui_top-320.f)/2.f + 320.f));
	addChild(top_case, kSSS_Z_top);
	
	CountingBMLabel* gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGold())->getCString(), "etc_font.fnt", 0.3f);
	gold_label->setPosition(ccp(225,top_case->getContentSize().height/2.f));
	top_case->addChild(gold_label);
	
	mySGD->setGoldLabel(gold_label);
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(295,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_time);
	
	is_menu_enable = true;
	
	srand(time(NULL));
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
    return true;
}

void StageSettingScene::tempAction(CCObject* sender)
{
	int tag = ((CCNode*)sender)->getTag();
	if(tag == 1)
		myDSH->setIntegerForKey(kDSH_Key_heartCnt, 5);
	else if(tag == 2)
	{
		removeChildByTag(kSSS_MT_noti);
		realStartAction();
	}
	else if(tag == 3)
	{
		removeChildByTag(kSSS_MT_noti);
		is_menu_enable = true;
	}
}

CCPoint StageSettingScene::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kSSS_MT_changeCard)				return_value = ccp(103,161);
	else if(t_tag == kSSS_MT_back)				return_value = ccp(70,27);
	else if(t_tag == kSSS_MT_challenge)			return_value = ccp(240,27);
	else if(t_tag == kSSS_MT_start)				return_value = ccp(387,27);
	else if(t_tag == kSSS_MT_gacha)				return_value = ccp(365,246);
	else if(t_tag == kSSS_MT_itemBase)			return_value = ccp(314,190);
	else if(t_tag == kSSS_MT_selectedBase)		return_value = ccp(95,1);
	
	return return_value;
}

void StageSettingScene::itemSetting()
{
	for(int i=0;i<item_list.size();i++)
	{
		ITEM_CODE t_ic = item_list[i];
		
		deque<int>::iterator iter = find(card_options.begin(), card_options.end(), t_ic);
		if(iter == card_options.end()) // not same option card // enable item
		{
			CCNode* item_parent = CCNode::create();
			item_parent->setPosition(ccpAdd(getContentPosition(kSSS_MT_itemBase), ccpMult(ccp(0,-43), i)));
			my_ilv->addChild(item_parent, kSSS_Z_content, kSSS_MT_itemBase+i);
			
			CCSprite* n_item = CCSprite::create("stagesetting_item_normal_back.png");
			CCSprite* s_item = CCSprite::create("stagesetting_item_normal_back.png");
			s_item->setColor(ccGRAY);
			
			CCMenuItem* item_item = CCMenuItemSprite::create(n_item, s_item, this, menu_selector(StageSettingScene::menuAction));
			item_item->setTag(kSSS_MT_itemBase+i);
			
			CCMenu* item_menu = CCMenu::createWithItem(item_item);
			item_menu->setPosition(CCPointZero);
			item_parent->addChild(item_menu, kSSS_Z_content, kSSS_MT_itemBase+i);
			
			CCSprite* item_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
			item_img->setScale(0.53);
			item_img->setPosition(ccp(-91,1));
			item_parent->addChild(item_img, kSSS_Z_content);
			
			int price_value = mySD->getItemPrice(t_ic);
			CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("%d", price_value)->getCString(), mySGD->getFont().c_str(), 18);
			price_label->setColor(ccBLACK);
			price_label->setAnchorPoint(ccp(0,0.5));
			price_label->setPosition(ccp(-65,5));
			item_parent->addChild(price_label, kSSS_Z_content);
			
			CCLabelTTF* option_label = CCLabelTTF::create(mySD->getItemScript(t_ic).c_str(), mySGD->getFont().c_str(), 8);
			option_label->setColor(ccBLACK);
			option_label->setAnchorPoint(ccp(0,0.5));
			option_label->setPosition(ccp(-67,-11));
			item_parent->addChild(option_label, kSSS_Z_content);
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);
			if(item_cnt > 0)
			{
				CCSprite* cnt_img = CCSprite::create("stagesetting_item_count.png");
				cnt_img->setPosition(ccp(-100,16));
				item_parent->addChild(cnt_img, kSSS_Z_content, kSSS_MT_itemCntBase + i);
				
				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 12);
				cnt_label->setPosition(ccp(cnt_img->getContentSize().width/2.f, cnt_img->getContentSize().height/2.f));
				cnt_img->addChild(cnt_label);
				
//				if(mySGD->isBeforeUseItem(item_list[i]))
//				{
//					is_selected_item.push_back(true);
//					CCSprite* selected_img = CCSprite::create("button_item_selected.png");
//					selected_img->setPosition(item_menu->getPosition());
//					addChild(selected_img, kSSS_Z_selectedImg, kSSS_MT_selectedBase+i);
//				}
//				else
			}
			
			CCSprite* selected_img = CCSprite::create("stagesetting_item_unselected.png");
			selected_img->setPosition(getContentPosition(kSSS_MT_selectedBase));
			item_parent->addChild(selected_img, kSSS_Z_content, kSSS_MT_selectedBase+i);
			
			is_selected_item.push_back(false);
		}
		else
		{
			is_selected_item.push_back(false);
		}
	}
}

void StageSettingScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kSSS_MT_start)
	{
		int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		int durability;
		if(selected_card_number > 0)
		{
			durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number)-1;
		}
		
		if(durability > 0 && heart_time->isStartable())
		{
			if(heart_time->startGame())
			{
				realStartAction();
			}
			else
			{
				is_menu_enable = true;
			}
		}
		else if(heart_time->isStartable())
		{
			DurabilityNoti* t_popup = DurabilityNoti::create(this, menu_selector(StageSettingScene::tempAction), this, menu_selector(StageSettingScene::tempAction));
			addChild(t_popup, kSSS_Z_popup, kSSS_MT_noti);
		}
	}
	else if(tag == kSSS_MT_back)
	{
		mySGD->resetLabels();
		CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
	}
	else if(tag == kSSS_MT_changeCard)
	{
		mySGD->resetLabels();
		mySGD->before_cardsetting = kSceneCode_StageSetting;
		CCDirector::sharedDirector()->replaceScene(CardSettingScene::scene());
	}
	else if(tag == kSSS_MT_challenge)
	{
		is_menu_enable = true;
//		ChallengePopup* t_cp = ChallengePopup::create(this, callfunc_selector(StageSettingScene::popupClose));
//		addChild(t_cp, kSSS_Z_popup);
	}
	else if(tag == kSSS_MT_gacha)
	{
		is_menu_enable = false;
		GachaPopup* t_gp = GachaPopup::create(this, callfunc_selector(StageSettingScene::popupClose));
		addChild(t_gp, kSSS_Z_popup);
	}
	else if(tag >= kSSS_MT_itemBase && tag < kSSS_MT_selectedBase)
	{
		int clicked_item_number = tag-kSSS_MT_itemBase;
		
		if(myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[clicked_item_number]) > 0)
		{
			if(is_selected_item[clicked_item_number])
			{
				is_selected_item[clicked_item_number] = false;
				
				CCNode* t_item = my_ilv->getChildByTag(kSSS_MT_itemBase+clicked_item_number);
				t_item->removeChildByTag(kSSS_MT_selectedBase+clicked_item_number);
				
				CCSprite* selected_img = CCSprite::create("stagesetting_item_unselected.png");
				selected_img->setPosition(getContentPosition(kSSS_MT_selectedBase));
				t_item->addChild(selected_img, kSSS_Z_content, kSSS_MT_selectedBase+clicked_item_number);
			}
			else
			{
				is_selected_item[clicked_item_number] = true;
				
				CCNode* t_item = my_ilv->getChildByTag(kSSS_MT_itemBase+clicked_item_number);
				t_item->removeChildByTag(kSSS_MT_selectedBase+clicked_item_number);
				
				CCSprite* selected_img = CCSprite::create("stagesetting_item_selected.png");
				selected_img->setPosition(getContentPosition(kSSS_MT_selectedBase));
				t_item->addChild(selected_img, kSSS_Z_content, kSSS_MT_selectedBase+clicked_item_number);
			}
		}
		else
		{
			ItemBuyPopup* t_ibp = ItemBuyPopup::create(item_list[clicked_item_number], clicked_item_number, this, callfuncII_selector(StageSettingScene::buySuccessItem));
			addChild(t_ibp, kSSS_Z_popup);
		}
		
		is_menu_enable = true;
	}
}

void StageSettingScene::realStartAction()
{
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	if(selected_card_number > 0)
	{
		int durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number) - 1;
		myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number, durability);
	}
	
	Json::Value param;
	param["key"] = CCSTR_CWF("stage_start_%d", mySD->getSilType())->getCString();
	
	hspConnector::get()->command("increaseStats", param, NULL);
	
	mySGD->resetLabels();
	myGD->resetGameData();
	
	deque<bool> is_using_item;
	for(int i=kIC_attack;i<=kIC_randomChange;i++)
		is_using_item.push_back(false);
	
	for(int i=0;i<is_selected_item.size();i++)
	{
		if(is_selected_item[i])
		{
			myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i], myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i])-1);
			myLog->addLog(kLOG_useItem_s, -1, convertToItemCodeToItemName(item_list[i]).c_str());
			is_using_item[item_list[i]] = true;
		}
	}
	
	for(int i=kIC_attack;i<=kIC_randomChange;i++)
		mySGD->setIsUsingItem(ITEM_CODE(i), is_using_item[i]);
	
	mySGD->setGameStart();
	CCDirector::sharedDirector()->replaceScene(Maingame::scene());
}

void StageSettingScene::popupClose()
{
	is_menu_enable = true;
}

void StageSettingScene::buySuccessItem(int t_clicked_item_number, int cnt)
{
	myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_number], myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_number])+cnt);
	int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_number]);
	is_selected_item[t_clicked_item_number] = true;
	
	myLog->addLog(kLOG_buyItem_s, -1, convertToItemCodeToItemName(item_list[t_clicked_item_number]).c_str());
	
	CCNode* item_parent = my_ilv->getChildByTag(kSSS_MT_itemBase+t_clicked_item_number);
	
	CCSprite* cnt_img = CCSprite::create("stagesetting_item_count.png");
	cnt_img->setPosition(ccp(-100,16));
	item_parent->addChild(cnt_img, kSSS_Z_content, kSSS_MT_itemCntBase + t_clicked_item_number);
	
	CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 12);
	cnt_label->setPosition(ccp(cnt_img->getContentSize().width/2.f, cnt_img->getContentSize().height/2.f));
	cnt_img->addChild(cnt_label);
	
	CCSprite* selected_img = CCSprite::create("stagesetting_item_selected.png");
	selected_img->setPosition(getContentPosition(kSSS_MT_selectedBase));
	item_parent->addChild(selected_img, kSSS_Z_content, kSSS_MT_selectedBase+t_clicked_item_number);
}

string StageSettingScene::convertToItemCodeToItemName(ITEM_CODE t_code)
{
	string return_value;
	if(t_code == kIC_attack)				return_value = "attack";
	else if(t_code == kIC_speedUp)			return_value = "speedUp";
	else if(t_code == kIC_addTime)			return_value = "addTime";
	else if(t_code == kIC_fast)				return_value = "fast";
	else if(t_code == kIC_critical)			return_value = "critical";
	else if(t_code == kIC_subOneDie)		return_value = "subOneDie";
	else if(t_code == kIC_doubleItem)		return_value = "doubleItem";
	else if(t_code == kIC_silence)			return_value = "silence";
	else if(t_code == kIC_subNothing)		return_value = "subNothing";
	else if(t_code == kIC_longTime)			return_value = "longTime";
	else if(t_code == kIC_bossLittleEnergy)	return_value = "bossLittleEnergy";
	else if(t_code == kIC_subSmallSize)		return_value = "subSmallSize";
	else if(t_code == kIC_smallArea)		return_value = "smallArea";
	else if(t_code == kIC_widePerfect)		return_value = "widePerfect";
	else if(t_code == kIC_randomChange)		return_value = "randomChange";
	
	return return_value.c_str();
}

void StageSettingScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void StageSettingScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(StageSettingScene::alertAction));
}