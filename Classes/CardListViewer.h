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
#include "ServerDataSave.h"

USING_NS_CC;
using namespace std;

enum CSS_Zorder{
	kCSS_Z_back = 1,
	kCSS_Z_selectedImg,
	kCSS_Z_content,
	kCSS_Z_select,
	kCSS_Z_check,
	kCSS_Z_alignList,
	kCSS_Z_alignButton
};

enum CSS_MenuTag{
	kCSS_MT_close = 1,
	kCSS_MT_align = 2,
	kCSS_MT_diary = 3,
	kCSS_MT_selectedCard = 4,
	kCSS_MT_selectedCheck = 5,
	kCSS_MT_checkMark = 6,
	kCSS_MT_alignClose = 7,
	kCSS_MT_alignTake = 8,
	kCSS_MT_alignGradeUp = 9,
	kCSS_MT_alignGradeDown = 10,
	kCSS_MT_alignDefault = 11,
	kCSS_MT_cardBase = 1000000,
	kCSS_MT_cardMenuBase = 2000000,
	kCSS_MT_noCardBase = 3000000
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
	
	static CLV_Node* create(int t_card_number, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect)
	{
		CLV_Node* t_n = new CLV_Node();
		t_n->myInit(t_card_number, t_menu, d_menu, t_position, t_rect);
		t_n->autorelease();
		return t_n;
	}
	
	void viewCheck()
	{
		CCPoint parent_position = getParent()->getPosition();
		CCRect tt_rect = CCRectMake(parent_position.x+my_position.x-my_size.width/2.f - 2, parent_position.y+my_position.y-my_size.height/2.f - 2, my_size.width + 4, my_size.height + 4);
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
	int card_number;
	
	bool is_card_number_setted;
	
	CCObject* target_menu;
	SEL_MenuHandler delegate_menu;
	CCPoint my_position;
	CCRect parent_view_rect;
	CCSize my_size;
	
	bool is_setted;
	
	int my_tag;
	
	void setChild()
	{
		if(is_card_number_setted)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, card_number) != 0)
			{
				bool is_color = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0;
				
				GraySprite* t_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png",
																											  NSDS_GI(kSDS_CI_int1_stage_i, card_number), NSDS_GI(kSDS_CI_int1_grade_i, card_number))->getCString()));
				t_card->setScale(0.92f);
				t_card->setPosition(CCPointZero);
				addChild(t_card, kCSS_Z_content, kCSS_MT_cardBase+card_number);
				
				if(is_color)		t_card->setGray(false);
				else				t_card->setGray(true);
				
				CCSprite* t_no = CCSprite::create("cardsetting_noimg.png");
				t_no->setPosition(CCPointZero);
				addChild(t_no, kCSS_Z_content);
				
				CCLabelTTF* t_durability = CCLabelTTF::create(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number),
																						 mySD->getCardDurability(NSDS_GI(kSDS_CI_int1_stage_i, card_number), NSDS_GI(kSDS_CI_int1_grade_i, card_number)))->getCString(),
															  mySGD->getFont().c_str(), 10);
				t_durability->setAnchorPoint(ccp(0.5f,0.5f));
				t_durability->setColor(ccBLACK);
				t_durability->setHorizontalAlignment(kCCTextAlignmentLeft);
				t_durability->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
				t_durability->setPosition(ccp(t_card->getContentSize().width/2.f+15, 7));
				t_card->addChild(t_durability);
				
				CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
				mini_rank->setPosition(ccp(9,9));
				t_card->addChild(mini_rank);
				
				CCLabelTTF* t_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, card_number))->getCString(), mySGD->getFont().c_str(), 8);
				t_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f, mini_rank->getContentSize().height/2.f-1));
				mini_rank->addChild(t_rank);
				
				CCMenuItem* t_card_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", target_menu, delegate_menu);
				t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
				
				CCMenu* t_card_menu = CCMenu::createWithItem(t_card_item);
				t_card_menu->setTouchEnabled(false);
				t_card_menu->setPosition(CCPointZero);
				addChild(t_card_menu, kCSS_Z_content, kCSS_MT_cardMenuBase+card_number);
			}
			else
			{
				CCSprite* t_no = CCSprite::create("cardsetting_noimg.png");
				t_no->setPosition(CCPointZero);
				addChild(t_no, kCSS_Z_content);
				
				CCMenuItem* t_card_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", target_menu, delegate_menu);
				t_card_item->setTag(kCSS_MT_noCardBase+card_number);
				
				CCMenu* t_card_menu = CCMenu::createWithItem(t_card_item);
				t_card_menu->setTouchEnabled(false);
				t_card_menu->setPosition(CCPointZero);
				addChild(t_card_menu, kCSS_Z_content, kCSS_MT_noCardBase+card_number);
			}
		}
		else
		{
			if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level)) != 0)
			{
				bool is_color = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level)) > 0;
				
				GraySprite* t_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", card_stage, card_level)->getCString()));
				t_card->setScale(0.92f);
				t_card->setPosition(CCPointZero);
				addChild(t_card, kCSS_Z_content, kCSS_MT_cardBase+NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level));
				
				if(is_color)		t_card->setGray(false);
				else				t_card->setGray(true);
				
				CCSprite* t_no = CCSprite::create("cardsetting_noimg.png");
				t_no->setPosition(CCPointZero);
				addChild(t_no, kCSS_Z_content);
				
				CCLabelTTF* t_durability = CCLabelTTF::create(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level)),
																						 mySD->getCardDurability(card_stage, card_level))->getCString(),
															  mySGD->getFont().c_str(), 10);
				t_durability->setAnchorPoint(ccp(0.5f,0.5f));
				t_durability->setColor(ccBLACK);
				t_durability->setHorizontalAlignment(kCCTextAlignmentLeft);
				t_durability->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
				t_durability->setPosition(ccp(t_card->getContentSize().width/2.f+15, 7));
				t_card->addChild(t_durability);
				
				CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
				mini_rank->setPosition(ccp(9,9));
				t_card->addChild(mini_rank);
				
				CCLabelTTF* t_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level)))->getCString(), mySGD->getFont().c_str(), 8);
				t_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f, mini_rank->getContentSize().height/2.f-1));
				mini_rank->addChild(t_rank);
				
				CCMenuItem* t_card_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", target_menu, delegate_menu);
				t_card_item->setTag(kCSS_MT_cardMenuBase+NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level));
				
				CCMenu* t_card_menu = CCMenu::createWithItem(t_card_item);
				t_card_menu->setTouchEnabled(false);
				t_card_menu->setPosition(CCPointZero);
				addChild(t_card_menu, kCSS_Z_content, kCSS_MT_cardMenuBase+NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level));
			}
			else
			{
				CCSprite* t_no = CCSprite::create("cardsetting_noimg.png");
				t_no->setPosition(CCPointZero);
				addChild(t_no, kCSS_Z_content);
				
				CCMenuItem* t_card_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", target_menu, delegate_menu);
				t_card_item->setTag(kCSS_MT_noCardBase+NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level));
				
				CCMenu* t_card_menu = CCMenu::createWithItem(t_card_item);
				t_card_menu->setTouchEnabled(false);
				t_card_menu->setPosition(CCPointZero);
				addChild(t_card_menu, kCSS_Z_content, kCSS_MT_noCardBase+NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level));
			}
		}
		is_setted = true;;
	}
	
	void myInit(int t_card_stage, int t_card_level, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect)
	{
		is_card_number_setted = false;
		card_stage = t_card_stage;
		card_level = t_card_level;
		
		dataSet(t_menu, d_menu, t_position, t_rect);
		
		setPosition(my_position);
		
		my_size = CCSizeMake(60, 78);
		is_setted = false;
		
		if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level)) != 0)
			my_tag = kCSS_MT_cardMenuBase+NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level);
		else
			my_tag = kCSS_MT_noCardBase+NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level); // if (not loaded stage info) then my_tag = 30000 -> error
			
		CCRect tt_rect = CCRectMake(my_position.x-my_size.width/2.f, my_position.y-my_size.height/2.f, my_size.width, my_size.height);
		if(parent_view_rect.intersectsRect(tt_rect))
		{
			setChild();
		}
	}
	
	void myInit(int t_card_number, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect)
	{
		is_card_number_setted = true;
		card_number = t_card_number;
		
		dataSet(t_menu, d_menu, t_position, t_rect);
		
		setPosition(my_position);
		
		my_size = CCSizeMake(60, 78);
		is_setted = false;
		
		if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, card_number) != 0)
			my_tag = kCSS_MT_cardMenuBase+card_number;
		else
			my_tag = kCSS_MT_noCardBase+card_number; // if (not loaded stage info) then my_tag = 30000 -> error
		
		CCRect tt_rect = CCRectMake(my_position.x-my_size.width/2.f, my_position.y-my_size.height/2.f, my_size.width, my_size.height);
		if(parent_view_rect.intersectsRect(tt_rect))
		{
			setChild();
		}
	}
	
	void dataSet(CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect)
	{
		target_menu = t_menu;
		delegate_menu = d_menu;
		my_position = t_position;
		parent_view_rect = t_rect;
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
	void setPercentage(float t_p)
	{
		setPosition(ccp(getPositionX(),max_positionY*t_p));
		for(int i=0;i<getChildrenCount();i++)
		{
			CCNode* t_child = (CCNode*)getChildren()->objectAtIndex(i);
			int tag = t_child->getTag();
			if(tag == kCSS_MT_selectedCheck || tag == kCSS_MT_checkMark)		continue;
			((CLV_Node*)t_child)->viewCheck();
		}
	}
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
