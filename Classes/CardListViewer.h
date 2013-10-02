//
//  CardListViewer.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 2..
//
//

#ifndef __DGproto__CardListViewer__
#define __DGproto__CardListViewer__

#include "cocos2d.h"
#include "ListViewerScroll.h"
#include "StageImgLoader.h"
#include "DataStorageHub.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"
#include "GraySprite.h"

USING_NS_CC;
using namespace std;

enum CSS_Zorder{
	kCSS_Z_back = 1,
	kCSS_Z_selectedImg,
	kCSS_Z_content,
	kCSS_Z_check
};

enum CSS_MenuTag{
	kCSS_MT_close = 1,
	kCSS_MT_align = 2,
	kCSS_MT_diary = 3,
	kCSS_MT_selectedCard = 4,
	kCSS_MT_selectedCheck = 5,
	kCSS_MT_checkMark = 6,
	kCSS_MT_cardBase = 1000,
	kCSS_MT_cardMenuBase = 2000,
	kCSS_MT_noCardBase = 3000
};

class CLV_Node : public CCNode
{
public:
	static CLV_Node* create(int t_card_stage, int t_card_level, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect)
	{
		CLV_Node* t_n = new CLV_Node();
		t_n->myInit(t_card_stage, t_card_level, t_menu, d_menu, t_position, t_rect);
		t_n->autorelease();
		return t_n;
	}
	
	void viewCheck()
	{
		CCPoint parent_position = getParent()->getPosition();
		CCRect tt_rect = CCRectMake(parent_position.x+my_position.x-my_size.width/2.f, parent_position.y+my_position.y-my_size.height/2.f, my_size.width, my_size.height);
		if(parent_view_rect.intersectsRect(tt_rect))
		{
			if(!is_setted)
				setChild();
		}
		else
		{
			is_setted = false;
			removeAllChildren();
		}
	}
	
	bool isSetted(){	return is_setted;	}
	
	int getMyTag(){		return my_tag;	}
	
private:
	
	int card_stage;
	int card_level;
	CCObject* target_menu;
	SEL_MenuHandler delegate_menu;
	CCPoint my_position;
	CCRect parent_view_rect;
	CCSize my_size;
	
	bool is_setted;
	
	int my_tag;
	
	void setChild()
	{
		if(myDSH->getBoolForKey(kDSH_Key_hasGottenCard_int1, card_stage*10 + card_level-1))
		{
			bool is_color = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_stage*10 + card_level-1) > 0;
			
			GraySprite* t_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_visible.png", card_stage, card_level)->getCString()));
			t_card->setScale(0.15);
			t_card->setPosition(CCPointZero);
			addChild(t_card, kCSS_Z_content, kCSS_MT_cardBase+card_stage*10+card_level-1);
			
			if(is_color)		t_card->setGray(false);
			else				t_card->setGray(true);
			
			if(card_level == 3 && mySD->isAnimationStage(card_stage))
			{
				CCSize ani_size = mySD->getAnimationCutSize(card_stage);
				GraySprite* t_ani = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_animation.png", card_stage, card_level)->getCString()),
													  CCRectMake(0, 0, ani_size.width, ani_size.height));
				t_ani->setPosition(mySD->getAnimationPosition(card_stage));
				t_card->addChild(t_ani);
				
				if(is_color)			t_ani->setGray(false);
				else					t_ani->setGray(true);
			}
			
			CCLabelTTF* t_durability = CCLabelTTF::create(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_stage*10+card_level-1),
																					 mySD->getCardDurability(card_stage, card_level))->getCString(),
														  mySGD->getFont().c_str(), 50);
			t_durability->setColor(ccBLACK);
			t_durability->setHorizontalAlignment(kCCTextAlignmentLeft);
			t_durability->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
			t_durability->setPosition(ccp(70,30));
			t_card->addChild(t_durability);
			
			CCMenuItem* t_card_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", target_menu, delegate_menu);
			t_card_item->setTag(kCSS_MT_cardMenuBase+card_stage*10+card_level-1);
			
			CCMenu* t_card_menu = CCMenu::createWithItem(t_card_item);
			t_card_menu->setPosition(CCPointZero);
			addChild(t_card_menu, kCSS_Z_content, kCSS_MT_cardMenuBase+card_stage*10+card_level-1);
		}
		else
		{
			CCSprite* t_no = CCSprite::create("cardsetting_noimg.png");
			t_no->setPosition(CCPointZero);
			addChild(t_no, kCSS_Z_content);
			
			CCMenuItem* t_card_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", target_menu, delegate_menu);
			t_card_item->setTag(kCSS_MT_noCardBase+card_stage*10+card_level-1);
			
			CCMenu* t_card_menu = CCMenu::createWithItem(t_card_item);
			t_card_menu->setPosition(CCPointZero);
			addChild(t_card_menu, kCSS_Z_content, kCSS_MT_noCardBase+card_stage*10+card_level-1);
		}
		is_setted = true;;
	}
	
	void myInit(int t_card_stage, int t_card_level, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect)
	{
		card_stage = t_card_stage;
		card_level = t_card_level;
		target_menu = t_menu;
		delegate_menu = d_menu;
		my_position = t_position;
		parent_view_rect = t_rect;
		
		setPosition(my_position);
		
		my_size = CCSizeMake(60, 78);
		is_setted = false;
		
		if(myDSH->getBoolForKey(kDSH_Key_hasGottenCard_int1, card_stage*10 + card_level-1))
			my_tag = kCSS_MT_cardMenuBase+card_stage*10+card_level-1;
		else
			my_tag = kCSS_MT_noCardBase+card_stage*10+card_level-1;
			
		CCRect tt_rect = CCRectMake(my_position.x-my_size.width/2.f, my_position.y-my_size.height/2.f, my_size.width, my_size.height);
		if(parent_view_rect.intersectsRect(tt_rect))
		{
			setChild();
		}
	}
};

class CardListViewer : public CCLayer, public ScrollingObject
{
public:
	static CardListViewer* create()
	{
		CardListViewer* t_clv = new CardListViewer();
		t_clv->myInit();
		t_clv->autorelease();
		return t_clv;
	}
	
	virtual void visit();
	virtual void setPositionY(float t_y);
	void setPercentage(float t_p){		setPosition(ccp(getPositionX(),max_positionY*t_p));	}
	void setMaxPositionY();
	void setScroll(ScrollingObject* t_link);
	
	CCRect getViewRect(){	return view_rect;	}
	
private:
	
	CCRect view_rect;
	bool is_touching;
	int touched_index;
	CCPoint touch_begin_point;
	CCPoint first_touch_point;
	
	float max_positionY;
	
	float touchy;
	long touchStartTime;
    float touchStartY;
    float moveSpeed;
	bool isAnimated;
	
	ScrollingObject* target_link;
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
	
	void myInit();
	void moveList(float dy);
	void moveAnimation();
};

#endif /* defined(__DGproto__CardListViewer__) */
