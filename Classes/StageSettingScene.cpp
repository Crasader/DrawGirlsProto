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
#include "WorldMapScene.h"
#include "MaingameScene.h"
#include "ItemBuyPopup.h"
#include "CardSettingScene.h"

CCScene* StageSettingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StageSettingScene *layer = StageSettingScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

enum SSS_Zorder{
	kSSS_Z_back = 1,
	kSSS_Z_selectedImg,
	kSSS_Z_content,
	kSSS_Z_popup
};

enum SSS_MenuTag{
	kSSS_MT_start = 1,
	kSSS_MT_back = 2,
	kSSS_MT_changeCard = 3,
	kSSS_MT_itemBase = 100,
	kSSS_MT_selectedBase = 200,
	kSSS_MT_itemCntBase = 300
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
	stage_label->setPosition(ccp(300,290));
	addChild(stage_label, kSSS_Z_content);
	
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard); // 1, 2, 3 / 11, 12, 13 / 14, ...
	if(selected_card_number > 0)
	{
		int card_stage = selected_card_number/10;
		int card_level = selected_card_number%10 + 1;
		
		CCSprite* card_img = CCSprite::create(CCString::createWithFormat("stage%d_level%d_visible.png", card_stage, card_level)->getCString());
		card_img->setScale(0.29);
		card_img->setPosition(ccp(110, 188));
		addChild(card_img, kSSS_Z_content);
		
		if(card_level == 3 && mySD->isAnimationStage(card_stage))
		{
			CCSize ani_size = mySD->getAnimationCutSize(card_stage);
			CCSprite* card_ani = CCSprite::create(CCString::createWithFormat("stage%d_level%d_animation.png", card_stage, card_level)->getCString(),
												  CCRectMake(0, 0, ani_size.width, ani_size.height));
			card_ani->setPosition(mySD->getAnimationPosition(card_stage));
			card_img->addChild(card_ani);
		}
	}
	
	mySD->setCardOptions(card_options, selected_card_number);
	
	item_list = mySD->getStageItemList(selected_stage);
	itemSetting();
	
	CCSprite* n_changeCard = CCSprite::create("change_card.png");
	CCSprite* s_changeCard = CCSprite::create("change_card.png");
	s_changeCard->setColor(ccGRAY);
	
	CCMenuItem* changeCard_item = CCMenuItemSprite::create(n_changeCard, s_changeCard, this, menu_selector(StageSettingScene::menuAction));
	changeCard_item->setTag(kSSS_MT_changeCard);
	
	CCMenu* change_menu = CCMenu::createWithItem(changeCard_item);
	change_menu->setPosition(ccp(110,96));
	addChild(change_menu, kSSS_Z_content);
	
	
	CCMenuItem* backward_item = CCMenuItemImage::create("stagesetting_backward.png", "stagesetting_backward.png", this, menu_selector(StageSettingScene::menuAction));
	backward_item->setTag(kSSS_MT_back);
	
	CCMenu* backward_menu = CCMenu::createWithItem(backward_item);
	backward_menu->setPosition(ccp(90,37));
	addChild(backward_menu, kSSS_Z_content);
	
	
	CCSprite* n_start = CCSprite::create("sspl_start.png");
	CCSprite* s_start = CCSprite::create("sspl_start.png");
	s_start->setColor(ccGRAY);
	
	CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(StageSettingScene::menuAction));
	start_item->setTag(kSSS_MT_start);
	
	CCMenu* start_menu = CCMenu::createWithItem(start_item);
	start_menu->setPosition(ccp(347,37));
	addChild(start_menu, kSSS_Z_content);
	
	is_menu_enable = true;
	
	srand(time(NULL));
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
    return true;
}

void StageSettingScene::itemSetting()
{
	for(int i=0;i<item_list.size();i++)
	{
		ITEM_CODE t_ic = item_list[i];
		
		deque<int>::iterator iter = find(card_options.begin(), card_options.end(), t_ic);
		if(iter == card_options.end())
		{
			CCSprite* n_item = CCSprite::create("button_item_normal.png");
			CCSprite* s_item = CCSprite::create("button_item_normal.png");
			s_item->setColor(ccGRAY);
			
			CCMenuItem* item_item = CCMenuItemSprite::create(n_item, s_item, this, menu_selector(StageSettingScene::menuAction));
			item_item->setTag(kSSS_MT_itemBase+i);
			
			CCMenu* item_menu = CCMenu::createWithItem(item_item);
			item_menu->setPosition(ccp(270+(i%2)*104, 226-(i/2)*70));
			addChild(item_menu, kSSS_Z_content, kSSS_MT_itemBase+i);
			
			CCSprite* item_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
			item_img->setPosition(item_menu->getPosition());
			addChild(item_img, kSSS_Z_content);
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);
			if(item_cnt > 0)
			{
				CCSprite* cnt_img = CCSprite::create("item_cnt_label.png");
				cnt_img->setPosition(ccpAdd(item_menu->getPosition(), ccp(-25,17)));
				addChild(cnt_img, kSSS_Z_content, kSSS_MT_itemCntBase+i);
				
				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 12);
				cnt_label->setPosition(ccp(cnt_img->getContentSize().width/2.f, cnt_img->getContentSize().height/2.f));
				cnt_img->addChild(cnt_label);
				
				if(mySGD->isBeforeUseItem(item_list[i]))
				{
					is_selected_item.push_back(true);
					CCSprite* selected_img = CCSprite::create("button_item_selected.png");
					selected_img->setPosition(item_menu->getPosition());
					addChild(selected_img, kSSS_Z_selectedImg, kSSS_MT_selectedBase+i);
				}
				else
					is_selected_item.push_back(false);
			}
			else
			{
				CCSprite* cnt_img = CCSprite::create("item_cnt_plus.png");
				cnt_img->setPosition(ccpAdd(item_menu->getPosition(), ccp(25,-17)));
				addChild(cnt_img, kSSS_Z_content, kSSS_MT_itemCntBase+i);
				
				is_selected_item.push_back(false);
			}
		}
		else
		{
			CCSprite* item_button = CCSprite::create("button_item_normal.png");
			item_button->setColor(ccGRAY);
			item_button->setPosition(ccp(270+(i%2)*104, 226-(i/2)*70));
			addChild(item_button, kSSS_Z_content);
			
			CCSprite* item_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
			item_img->setColor(ccGRAY);
			item_img->setPosition(item_button->getPosition());
			addChild(item_img, kSSS_Z_content);
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);
			if(item_cnt > 0)
			{
				CCSprite* cnt_img = CCSprite::create("item_cnt_label.png");
				cnt_img->setPosition(ccpAdd(item_button->getPosition(), ccp(-25,17)));
				addChild(cnt_img, kSSS_Z_content, kSSS_MT_itemCntBase+i);
				
				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 12);
				cnt_label->setPosition(ccp(cnt_img->getContentSize().width/2.f, cnt_img->getContentSize().height/2.f));
				cnt_img->addChild(cnt_label);
			}
			else
			{
				CCSprite* cnt_img = CCSprite::create("item_cnt_plus.png");
				cnt_img->setPosition(ccpAdd(item_button->getPosition(), ccp(25,-17)));
				addChild(cnt_img, kSSS_Z_content, kSSS_MT_itemCntBase+i);
			}
			
			is_selected_item.push_back(false);
		}
	}
	
	if(item_list.size() > 0)
	{
		item_script = CCLabelTTF::create(mySD->getItemScript(ITEM_CODE(-1)).c_str(), mySGD->getFont().c_str(), 12, CCSizeMake(170, 60), kCCTextAlignmentCenter);
		item_script->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		item_script->setColor(ccBLACK);
		item_script->setPosition(ccp(322,96));
		addChild(item_script, kSSS_Z_content);
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
		if(myDSH->getIntegerForKey(kDSH_Key_selectedCard) > 0)
		{
			int loop_cnt = myDSH->getIntegerForKey(kDSH_Key_haveCardCnt);
			int found_number = 1;
			for(int i=1;i<=loop_cnt;i++)
			{
				int search_number = myDSH->getIntegerForKey(kDSH_Key_haveCardNumber_int1, i);
				if(search_number == myDSH->getIntegerForKey(kDSH_Key_selectedCard))
				{
					found_number = i;
					break;
				}
			}
			
			int durability = myDSH->getIntegerForKey(kDSH_Key_haveCardDurability_int1, found_number) - 1;
			myDSH->setIntegerForKey(kDSH_Key_haveCardDurability_int1, found_number, durability);
		}
		
		myGD->resetGameData();
		
		deque<bool> is_using_item;
		for(int i=kIC_attack;i<=kIC_randomChange;i++)
			is_using_item.push_back(false);
		
		for(int i=0;i<is_selected_item.size();i++)
		{
			if(is_selected_item[i])
			{
				myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i], myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i])-1);
				is_using_item[item_list[i]] = true;
			}
		}
		
		for(int i=kIC_attack;i<=kIC_randomChange;i++)
			mySGD->setIsUsingItem(ITEM_CODE(i), is_using_item[i]);
		
		CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
		pEGLView->setDesignResolutionSize(480, 320, kResolutionFixedWidth);
		
		mySGD->setGameStart();
		CCDirector::sharedDirector()->replaceScene(Maingame::scene());
	}
	else if(tag == kSSS_MT_back)
	{
		CCDirector::sharedDirector()->replaceScene(WorldMapScene::scene());
	}
	else if(tag == kSSS_MT_changeCard)
	{
		CCDirector::sharedDirector()->replaceScene(CardSettingScene::scene());
	}
	else if(tag >= kSSS_MT_itemBase)
	{
		int clicked_item_number = tag-kSSS_MT_itemBase;
		
		if(myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[clicked_item_number]) > 0)
		{
			if(is_selected_item[clicked_item_number])
			{
				is_selected_item[clicked_item_number] = false;
				removeChildByTag(kSSS_MT_selectedBase+clicked_item_number);
			}
			else
			{
				is_selected_item[clicked_item_number] = true;
				CCSprite* selected_img = CCSprite::create("button_item_selected.png");
				selected_img->setPosition(ccp(270+(clicked_item_number%2)*104, 226-(clicked_item_number/2)*70));
				addChild(selected_img, kSSS_Z_selectedImg, kSSS_MT_selectedBase+clicked_item_number);
			}
		}
		else
		{
			ItemBuyPopup* t_ibp = ItemBuyPopup::create(item_list[clicked_item_number], clicked_item_number, this, callfuncII_selector(StageSettingScene::buySuccessItem));
			addChild(t_ibp, kSSS_Z_popup);
		}
		
		item_script->setString(mySD->getItemScript(item_list[clicked_item_number]).c_str());
		
		is_menu_enable = true;
	}
}

void StageSettingScene::buySuccessItem(int t_clicked_item_number, int cnt)
{
	removeChildByTag(kSSS_MT_itemCntBase+t_clicked_item_number);
	myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_number], myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_number])+cnt);
	
	int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_number]);
	
	CCSprite* cnt_img = CCSprite::create("item_cnt_label.png");
	cnt_img->setPosition(ccpAdd(ccp(270+(t_clicked_item_number%2)*104, 226-(t_clicked_item_number/2)*70), ccp(-25,17)));
	addChild(cnt_img, kSSS_Z_content, kSSS_MT_itemCntBase+t_clicked_item_number);
	
	CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 12);
	cnt_label->setPosition(ccp(cnt_img->getContentSize().width/2.f, cnt_img->getContentSize().height/2.f));
	cnt_img->addChild(cnt_label);
	
	is_selected_item[t_clicked_item_number] = true;
	CCSprite* selected_img = CCSprite::create("button_item_selected.png");
	selected_img->setPosition(ccp(270+(t_clicked_item_number%2)*104, 226-(t_clicked_item_number/2)*70));
	addChild(selected_img, kSSS_Z_selectedImg, kSSS_MT_selectedBase+t_clicked_item_number);
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