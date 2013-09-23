//
//  CardSettingScene.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 18..
//
//

#include "CardSettingScene.h"
#include "ScreenSide.h"
#include "MyLocalization.h"
#include "StageSettingScene.h"

CCScene* CardSettingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CardSettingScene *layer = CardSettingScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

enum CSS_Zorder{
	kCSS_Z_back = 1,
	kCSS_Z_selectedImg,
	kCSS_Z_content
};

enum CSS_MenuTag{
	kCSS_MT_ok = 1,
	kCSS_MT_removeCard = 2,
	kCSS_MT_selectedCard = 3,
	kCSS_MT_cardBase = 100,
	kCSS_MT_cardMenuBase = 200
};

// on "init" you need to initialize your instance
bool CardSettingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	setKeypadEnabled(true);
	
	CCSprite* cardsetting_back = CCSprite::create("cardsetting_back.png");
	cardsetting_back->setPosition(ccp(240,160));
	addChild(cardsetting_back, kCSS_Z_back);
	
	CCSprite* cardsetting_title = CCSprite::create("cardsetting_title.png");
	cardsetting_title->setPosition(ccp(240,298));
	addChild(cardsetting_title, kCSS_Z_content);
	
	CCSprite* n_remove_card = CCSprite::create("cardsetting_remove.png");
	CCSprite* s_remove_card = CCSprite::create("cardsetting_remove.png");
	s_remove_card->setColor(ccGRAY);
	CCSprite* d_remove_card = CCSprite::create("cardsetting_remove.png");
	d_remove_card->setColor(ccBLACK);
	
	remove_card_item = CCMenuItemSprite::create(n_remove_card, s_remove_card, d_remove_card, this, menu_selector(CardSettingScene::menuAction));
	remove_card_item->setTag(kCSS_MT_removeCard);
	
	CCMenu* remove_card_menu = CCMenu::createWithItem(remove_card_item);
	remove_card_menu->setPosition(ccp(242+2*84, 218-98));
	addChild(remove_card_menu, kCSS_Z_content);
	
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	if(selected_card_number > 0)
	{
		int card_stage = selected_card_number/10;
		int card_level = selected_card_number%10 + 1;
		
		int loop_cnt = myDSH->getIntegerForKey(kDSH_Key_haveCardCnt);
		selected_img_number = 0;
		for(int i=1;i<=loop_cnt;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_haveCardNumber_int1, i) == selected_card_number)
			{
				selected_img_number = i;
				break;
			}
		}
		
		mountingCard(card_stage, card_level);
	}
	else
	{
		selected_card_img = NULL;
		selected_img = NULL;
		star_parent = NULL;
		card_option_case = NULL;
		card_option_script = NULL;
		selected_card_menu = NULL;
		
		selected_img_number = 0;
		
		remove_card_item->setEnabled(false);
	}
	
	int loop_cnt = myDSH->getIntegerForKey(kDSH_Key_haveCardCnt);
	for(int i=1;i<=loop_cnt;i++)
	{
		int card_number = myDSH->getIntegerForKey(kDSH_Key_haveCardNumber_int1, i);
		int card_stage = card_number/10;
		int card_level = card_number%10+1;
		
		CCSprite* t_card = CCSprite::create(CCString::createWithFormat("stage%d_level%d_visible.png", card_stage, card_level)->getCString());
		t_card->setScale(0.18);
		t_card->setPosition(ccp(242+((i-1)%3)*84, 218-((i-1)/3)*98));
		addChild(t_card, kCSS_Z_content, kCSS_MT_cardBase+i);
		
		if(card_level == 3 && mySD->isAnimationStage(card_stage))
		{
			CCSize ani_size = mySD->getAnimationCutSize(card_stage);
			CCSprite* t_ani = CCSprite::create(CCString::createWithFormat("stage%d_level%d_animation.png", card_stage, card_level)->getCString(),
											   CCRectMake(0, 0, ani_size.width, ani_size.height));
			t_ani->setPosition(mySD->getAnimationPosition(card_stage));
			t_card->addChild(t_ani);
		}
		
		CCLabelTTF* t_durability = CCLabelTTF::create(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_haveCardDurability_int1, i),
																				 mySD->getCardDurability(card_stage, card_level))->getCString(),
													  mySGD->getFont().c_str(), 50);
		t_durability->setColor(ccBLACK);
		t_durability->setHorizontalAlignment(kCCTextAlignmentLeft);
		t_durability->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		t_durability->setPosition(ccp(70,30));
		t_card->addChild(t_durability);
		
		CCMenuItem* t_card_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CardSettingScene::menuAction));
		t_card_item->setTag(kCSS_MT_cardMenuBase+i);
		
		CCMenu* t_card_menu = CCMenu::createWithItem(t_card_item);
		t_card_menu->setPosition(t_card->getPosition());
		addChild(t_card_menu, kCSS_Z_content, kCSS_MT_cardMenuBase+i);
	}
	
	CCMenuItem* ok_item = CCMenuItemImage::create("cardsetting_ok.png", "cardsetting_ok.png", this, menu_selector(CardSettingScene::menuAction));
	ok_item->setTag(kCSS_MT_ok);
	
	CCMenu* ok_menu = CCMenu::createWithItem(ok_item);
	ok_menu->setPosition(ccp(360, 30));
	addChild(ok_menu, kCSS_Z_content);
	
	is_menu_enable = true;
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
    return true;
}

void CardSettingScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kCSS_MT_ok)
	{
		CCDirector::sharedDirector()->replaceScene(StageSettingScene::scene());
	}
	else if(tag == kCSS_MT_selectedCard)
	{
		removeMountingCard();
		
		myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
		
		is_menu_enable = true;
	}
	else if(tag == kCSS_MT_removeCard)
	{
		int have_card_cnt = myDSH->getIntegerForKey(kDSH_Key_haveCardCnt);
		
		CCSprite* remove_card = (CCSprite*)getChildByTag(kCSS_MT_cardBase+selected_img_number);
		
		for(int i=selected_img_number+1;i<=have_card_cnt;i++)
		{
			myDSH->setIntegerForKey(kDSH_Key_haveCardNumber_int1, i-1, myDSH->getIntegerForKey(kDSH_Key_haveCardNumber_int1, i));
			myDSH->setIntegerForKey(kDSH_Key_haveCardDurability_int1, i-1, myDSH->getIntegerForKey(kDSH_Key_haveCardDurability_int1, i));
		}
		
		for(int i=have_card_cnt;i>selected_img_number;i--)
		{
			CCSprite* a_card = (CCSprite*)getChildByTag(kCSS_MT_cardBase+i);
			CCSprite* b_card = (CCSprite*)getChildByTag(kCSS_MT_cardBase+i-1);
			
			a_card->setPosition(b_card->getPosition());
			a_card->setTag(kCSS_MT_cardBase+i-1);
		}
		
		myDSH->setIntegerForKey(kDSH_Key_haveCardNumber_int1, have_card_cnt, 0);
		myDSH->setIntegerForKey(kDSH_Key_haveCardDurability_int1, have_card_cnt, 0);
		myDSH->setIntegerForKey(kDSH_Key_haveCardCnt, have_card_cnt-1);
		
		remove_card->removeFromParent();
		removeChildByTag(kCSS_MT_cardMenuBase+have_card_cnt);
		
		removeMountingCard();
		
		myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
		
		is_menu_enable = true;
	}
	else if(tag >= kCSS_MT_cardMenuBase)
	{
		int clicked_card_number = tag-kCSS_MT_cardMenuBase;
		
		if(clicked_card_number != selected_img_number)
		{
			removeMountingCard();
			selected_img_number = clicked_card_number;
			int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_haveCardNumber_int1, selected_img_number);
			int card_stage = selected_card_number/10;
			int card_level = selected_card_number%10 + 1;
			mountingCard(card_stage, card_level);
			myDSH->setIntegerForKey(kDSH_Key_selectedCard, selected_card_number);
			remove_card_item->setEnabled(true);
		}
		
		is_menu_enable = true;
	}
}

void CardSettingScene::removeMountingCard()
{
	if(selected_card_img)		selected_card_img->removeFromParent();
	selected_card_img = NULL;
	if(star_parent)				star_parent->removeFromParent();
	star_parent = NULL;
	
	if(card_option_case)		card_option_case->removeFromParent();
	card_option_case = NULL;
	if(card_option_script)		card_option_script->removeFromParent();
	card_option_script = NULL;
	
	if(selected_card_menu)		selected_card_menu->removeFromParent();
	selected_card_menu = NULL;
	
	if(selected_img)			selected_img->removeFromParent();
	selected_img = NULL;
	
	remove_card_item->setEnabled(false);
	
	selected_img_number = 0;
}

void CardSettingScene::mountingCard(int card_stage, int card_level)
{
	selected_card_img = CCSprite::create(CCString::createWithFormat("stage%d_level%d_visible.png", card_stage, card_level)->getCString());
	selected_card_img->setScale(0.45);
	selected_card_img->setPosition(ccp(113,168));
	addChild(selected_card_img, kCSS_Z_content);
	
	if(card_level == 3 && mySD->isAnimationStage(card_stage))
	{
		CCSize ani_size = mySD->getAnimationCutSize(card_stage);
		CCSprite* t_ani = CCSprite::create(CCString::createWithFormat("stage%d_level%d_animation.png", card_stage, card_level)->getCString(),
										   CCRectMake(0, 0, ani_size.width, ani_size.height));
		t_ani->setPosition(mySD->getAnimationPosition(card_stage));
		selected_card_img->addChild(t_ani);
	}
	
	star_parent = CCNode::create();
	star_parent->setPosition(ccp(165,247));
	addChild(star_parent, kCSS_Z_content);
	
	for(int i=card_level-1;i>=0;i--)
	{
		CCSprite* card_star = CCSprite::create("card_star.png");
		card_star->setPosition(ccp(i*(-20),0));
		star_parent->addChild(card_star);
	}
	
	card_option_case = CCSprite::create("card_option_case.png");
	card_option_case->setPosition(ccpAdd(selected_card_img->getPosition(), ccp(0,-80)));
	addChild(card_option_case, kCSS_Z_content);
	
	card_option_script = CCLabelTTF::create(CCString::createWithFormat("%d/%d | %s",
																	   myDSH->getIntegerForKey(kDSH_Key_haveCardDurability_int1, selected_img_number),
																	   mySD->getCardDurability(card_stage, card_level),
																	   mySD->getCardOptionScript(card_stage, card_level).c_str())->getCString(),
											mySGD->getFont().c_str(), 15);
	card_option_script->setColor(ccBLACK);
	card_option_script->setHorizontalAlignment(kCCTextAlignmentCenter);
	card_option_script->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	card_option_script->setPosition(card_option_case->getPosition());
	addChild(card_option_script, kCSS_Z_content);
	
	
	CCMenuItem* selected_card_item = CCMenuItemImage::create("cardsetting_cardmenu_big.png", "cardsetting_cardmenu_big.png", this, menu_selector(CardSettingScene::menuAction));
	selected_card_item->setTag(kCSS_MT_selectedCard);
	
	selected_card_menu = CCMenu::createWithItem(selected_card_item);
	selected_card_menu->setPosition(selected_card_img->getPosition());
	addChild(selected_card_menu, kCSS_Z_content);
	
	selected_img = CCSprite::create("card_selected.png");
	selected_img->setPosition(ccp(242+((selected_img_number-1)%3)*84, 218-((selected_img_number-1)/3)*98));
	addChild(selected_img, kCSS_Z_selectedImg);
}

void CardSettingScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void CardSettingScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(CardSettingScene::alertAction));
}