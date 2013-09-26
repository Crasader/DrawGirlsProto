//
//  CardFullPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 19..
//
//

#ifndef __DGproto__CardFullPopup__
#define __DGproto__CardFullPopup__

#include "cocos2d.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"
#include "StageImgLoader.h"

USING_NS_CC;
using namespace std;

enum CFP_MenuTag{
	kCFP_MT_discard = 1,
	kCFP_MT_selectBase
};

enum CFP_Zorder{
	kCFP_Z_back = 1,
	kCFP_Z_selected,
	kCFP_Z_content
};

class CardFullPopup : public CCLayer
{
public:
	static CardFullPopup* create(CCObject* t_discard, SEL_CallFunc d_discard)
	{
		CardFullPopup* t_cfp = new CardFullPopup();
		t_cfp->myInit(t_discard, d_discard);
		t_cfp->autorelease();
		return t_cfp;
	}
	
private:
	bool is_menu_enable;
	
	int selected_number;
	CCSprite* selected_img;
	
	int touched_number;
	CCMenu* discard_menu;
	
	CCObject* target_discard;
	SEL_CallFunc delegate_discard;
	
	void myInit(CCObject* t_discard, SEL_CallFunc d_discard)
	{
		target_discard = t_discard;
		delegate_discard = d_discard;
		
		CCSprite* back = CCSprite::create("cardfull_back.png");
		back->setPosition(ccp(240,160));
		addChild(back, kCFP_Z_back);
		
		int stage_number = mySD->getSilType();
		int take_level;
		if(mySGD->is_exchanged && mySGD->is_showtime)
			take_level = 3;
		else if(mySGD->is_exchanged || mySGD->is_showtime)
			take_level = 2;
		else
			take_level = 1;
		
		CCSprite* take_card = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", stage_number, take_level)->getCString());
		take_card->setScale(0.18);
		take_card->setPosition(ccp(58, 143));
		addChild(take_card, kCFP_Z_content);
		
		if(take_level == 3 && mySD->isAnimationStage())
		{
			CCSize ani_size = mySD->getAnimationCutSize();
			CCSprite* take_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", stage_number, take_level)->getCString(),
												  CCRectMake(0, 0, ani_size.width, ani_size.height));
			take_ani->setPosition(mySD->getAnimationPosition());
			take_card->addChild(take_ani);
		}
		
		CCLabelTTF* take_durability = CCLabelTTF::create(CCString::createWithFormat("%d/%d", mySD->getCardDurability(stage_number, take_level), mySD->getCardDurability(stage_number, take_level))->getCString(), mySGD->getFont().c_str(), 20);
		take_durability->setColor(ccBLACK);
		take_durability->setHorizontalAlignment(kCCTextAlignmentLeft);
		take_durability->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		take_durability->setPosition(ccp(10,10));
		take_card->addChild(take_durability);
		
		CCMenuItem* take_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CardFullPopup::menuAction));
		take_item->setTag(kCFP_MT_selectBase);
		
		CCMenu* take_menu = CCMenu::createWithItem(take_item);
		take_menu->setPosition(ccp(58,143));
		addChild(take_menu, kCFP_Z_content, kCFP_MT_selectBase);
		
		selected_img = CCSprite::create("card_selected.png");
		selected_img->setPosition(ccp(58,143));
		addChild(selected_img, kCFP_Z_selected);
		
		selected_number = 0;
		
		
		for(int i=1;i<=5;i++)
		{
			int card_number = myDSH->getIntegerForKey(kDSH_Key_haveCardNumber_int1, i);
			int card_stage = card_number/10;
			int card_level = card_number%10 + 1;
			
			CCSprite* have_card = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", card_stage, card_level)->getCString());
			have_card->setScale(0.18);
			have_card->setPosition(ccp(58+i*73, 143));
			addChild(have_card, kCFP_Z_content);
			
			if(card_level == 3 && mySD->isAnimationStage(card_stage))
			{
				CCSize ani_size = mySD->getAnimationCutSize(card_stage);
				CCSprite* card_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", card_stage, card_level)->getCString(),
													  CCRectMake(0, 0, ani_size.width, ani_size.height));
				card_ani->setPosition(mySD->getAnimationPosition(card_stage));
				have_card->addChild(card_ani);
			}
			
			CCLabelTTF* card_durability = CCLabelTTF::create(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_haveCardDurability_int1, i),
																						mySD->getCardDurability(card_stage, card_level))->getCString(), mySGD->getFont().c_str(), 20);
			card_durability->setColor(ccBLACK);
			card_durability->setHorizontalAlignment(kCCTextAlignmentLeft);
			card_durability->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
			card_durability->setPosition(ccp(10,10));
			have_card->addChild(card_durability);
			
			CCMenuItem* card_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CardFullPopup::menuAction));
			card_item->setTag(kCFP_MT_selectBase+i);
			
			CCMenu* card_menu = CCMenu::createWithItem(card_item);
			card_menu->setPosition(ccp(58+i*73,143));
			addChild(card_menu, kCFP_Z_content, kCFP_MT_selectBase+i);
		}
		
		
		CCMenuItem* discard_item = CCMenuItemImage::create("cardfull_discard.png", "cardfull_discard.png", this, menu_selector(CardFullPopup::menuAction));
		discard_item->setTag(kCFP_MT_discard);
		
		discard_menu = CCMenu::createWithItem(discard_item);
		discard_menu->setPosition(ccp(240,50));
		addChild(discard_menu, kCFP_Z_content);
		
		is_menu_enable = true;
		
		touched_number = 0;
		
		setTouchEnabled(true);
	}
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kCFP_MT_discard)
		{
			if(selected_number > 0)
			{
				int stage_number = mySD->getSilType();
				int take_level;
				if(mySGD->is_exchanged && mySGD->is_showtime)
					take_level = 3;
				else if(mySGD->is_exchanged || mySGD->is_showtime)
					take_level = 2;
				else
					take_level = 1;
				
				myDSH->setIntegerForKey(kDSH_Key_haveCardNumber_int1, selected_number, stage_number*10+take_level-1);
				myDSH->setIntegerForKey(kDSH_Key_haveCardDurability_int1, selected_number, mySD->getCardDurability(stage_number, take_level));
				myDSH->setIntegerForKey(kDSH_Key_selectedCard, stage_number*10+take_level-1);
			}
			
			(target_discard->*delegate_discard)();
			removeFromParent();
		}
		else if(tag >= kCFP_MT_selectBase)
		{
			if(selected_number != tag-kCFP_MT_selectBase)
			{
				selected_number = tag-kCFP_MT_selectBase;
				
				selected_img->setPosition(ccp(58+selected_number*73, 143));
			}
			is_menu_enable = true;
		}
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number != 0)		return true;
		if(discard_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kCFP_MT_discard;
		else
		{
			for(int i=0;i<=5 && touched_number == 0;i++)
			{
				if(((CCMenu*)getChildByTag(kCFP_MT_selectBase+i))->ccTouchBegan(pTouch, pEvent))
					touched_number = kCFP_MT_selectBase+i;
			}
		}
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kCFP_MT_discard)						discard_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number >= kCFP_MT_selectBase)				((CCMenu*)getChildByTag(touched_number))->ccTouchMoved(pTouch, pEvent);
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kCFP_MT_discard){			discard_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number >= kCFP_MT_selectBase){	((CCMenu*)getChildByTag(touched_number))->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kCFP_MT_discard){			discard_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number >= kCFP_MT_selectBase){	((CCMenu*)getChildByTag(touched_number))->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
};

#endif /* defined(__DGproto__CardFullPopup__) */
