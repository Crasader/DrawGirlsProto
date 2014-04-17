//
//  CardListViewer.cpp
//  DGproto
//
//  Created by 사원3 on 13. 10. 2..
//
//

#include "CardListViewer.h"

#include "GraySprite.h"
#include "ServerDataSave.h"

#include "DataStorageHub.h"
#include "StageImgLoader.h"

#include "SilhouetteData.h"
#include "StarGoldData.h"


void CardListViewer::visit()
{
	glEnable(GL_SCISSOR_TEST);
	
	CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	CCSize rSize = myDSH->getDesignResolutionSize(); // getSize
	
	float wScale = frame_size.width / rSize.width;
	float hScale = frame_size.height / rSize.height;
	
	float xMargine = 0;
	float yMargine = 0;

	if(wScale >= hScale)
	{
		wScale = hScale;
		xMargine = (frame_size.width - rSize.width*wScale)/2.f;
	}
	else
	{
		hScale = wScale;
		yMargine = (frame_size.height - rSize.height*hScale)/2.f;
	}
	
	float x = (view_rect.origin.x+getParent()->getPositionX())*wScale + xMargine;
	float y = (view_rect.origin.y+getParent()->getPositionY())*hScale + yMargine;
	float w = view_rect.size.width*wScale;
	float h = view_rect.size.height*hScale;
	
	glScissor(x,y,w,h);
	
	CCLayer::visit();
	
	glDisable(GL_SCISSOR_TEST);
}

void CardListViewer::setPositionY(float t_y)
{
	if(t_y < 0)
	{
		t_y = 0;
		unschedule(schedule_selector(CardListViewer::moveAnimation));
	}
	else if(t_y > max_positionY)
	{
		t_y = max_positionY;
		unschedule(schedule_selector(CardListViewer::moveAnimation));
	}
	
	CCLayer::setPositionY(t_y);
	
	for(int i=0;i<getChildrenCount();i++)
	{
		CCNode* t_child = (CCNode*)getChildren()->objectAtIndex(i);
//		int tag = t_child->getTag();
//		if(tag == kCSS_MT_selectedCheck || tag == kCSS_MT_checkMark)		continue;
		((CLV_Node*)t_child)->viewCheck();
	}
	
	if(target_link)
		target_link->setPercentage(t_y / max_positionY);
}

void CardListViewer::setMaxPositionY()
{
	int cnt = getChildrenCount()/3;
	
	float t_y = cnt*92 + 5;
	t_y -= view_rect.size.height;
	
	if(t_y > 0)
		max_positionY = t_y;
	else
		max_positionY = 0;
}

void CardListViewer::setScroll(ScrollingObject* t_link)
{
	target_link = t_link;
	setPositionY(getPositionY());
}

bool CardListViewer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCTouch* touch = pTouch;
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = myDSH->wideWidthFixTouch(location);
	
	bool return_value = false;
	
	if(isVisible() && view_rect.containsPoint(location))
	{
		touch_begin_point = location;
		first_touch_point = touch_begin_point;
		is_touching = true;
		
		touchy=location.y;
		
		timeval time;
		gettimeofday(&time, NULL);
		touchStartTime = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
		touchStartY=location.y;
		
		for(int i=0;i<getChildrenCount() && touched_index == 0;i++)
		{
			CCNode* item_parent = (CCNode*)getChildren()->objectAtIndex(i);
			int tag = item_parent->getTag();
//			if(tag == kCSS_MT_selectedCheck || tag == kCSS_MT_checkMark || tag == kCSS_MT_noCardBase)	continue; // kCSS_selectedCheck || kCSS_checkMark
			if(!((CLV_Node*)item_parent)->isSetted())		continue;
			CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(tag);
			if(item_menu->ccTouchBegan(pTouch, pEvent))
				touched_index = tag;
		}
		
		return_value = true;
	}
	
	unschedule(schedule_selector(CardListViewer::moveAnimation));
	isAnimated=false;
	
	return return_value;
}

void CardListViewer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touching)
	{
		return;
	}
	
	CCTouch* touch = pTouch;
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = myDSH->wideWidthFixTouch(location);
	
	if(isVisible() && view_rect.containsPoint(location))
	{
		float distance = location.y - touch_begin_point.y;
		setPositionY(getPositionY() + distance);
		touch_begin_point = location;
		
		if(fabsf(first_touch_point.y - location.y) > 5.f)
		{
			if(touched_index != 0)
			{
				CCNode* item_parent = getChildByTag(touched_index);
				CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(touched_index);
				item_menu->ccTouchCancelled(pTouch, pEvent);
				touched_index = 0;
			}
		}
	}
	else
	{
		timeval time;
		gettimeofday(&time, NULL);
		long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
		float _y = location.y-touchStartY;
		float _spd = _y/_time*10000;
		
		
		if(isAnimated == false && abs((int)_spd)>2){
			if(_spd > 0)	moveSpeed = ((int)_spd*10)/10.f-2;
			else			moveSpeed = ((int)_spd*10)/10.f+2;
			schedule(schedule_selector(CardListViewer::moveAnimation));
		}
		
		is_touching = false;
		
		if(touched_index != 0)
		{
			CCNode* item_parent = getChildByTag(touched_index);
			CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(touched_index);
			item_menu->ccTouchCancelled(pTouch, pEvent);
			touched_index = 0;
		}
	}
}

void CardListViewer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touching)
	{
		return;
	}
	
	CCTouch* touch = pTouch;
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = myDSH->wideWidthFixTouch(location);
	
	if(isVisible() && view_rect.containsPoint(location))
	{
		float distance = location.y - touch_begin_point.y;
		setPositionY(getPositionY() + distance);
		touch_begin_point = location;
		
		timeval time;
		gettimeofday(&time, NULL);
		long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
		float _y = location.y-touchStartY;
		float _spd = _y/_time*10000;
		
		
		if(isAnimated == false && abs((int)_spd)>2){
			if(_spd > 0)	moveSpeed = ((int)_spd*10)/10.f-2;
			else			moveSpeed = ((int)_spd*10)/10.f+2;
			schedule(schedule_selector(CardListViewer::moveAnimation));
		}
	}
	
	if(touched_index != 0)
	{
		CCNode* item_parent = getChildByTag(touched_index);
		CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(touched_index);
		item_menu->ccTouchEnded(pTouch, pEvent);
		touched_index = 0;
	}
	
	is_touching = false;
}

void CardListViewer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touching)
	{
		return;
	}
	
	CCTouch* touch = pTouch;
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = myDSH->wideWidthFixTouch(location);
	
	if(isVisible() && view_rect.containsPoint(location))
	{
		float distance = location.y - touch_begin_point.y;
		setPositionY(getPositionY() + distance);
		touch_begin_point = location;
		
		timeval time;
		gettimeofday(&time, NULL);
		long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
		float _y = location.y-touchStartY;
		float _spd = _y/_time*10000;
		
		
		if(isAnimated == false && abs((int)_spd)>2){
			if(_spd > 0)	moveSpeed = ((int)_spd*10)/10.f-2;
			else			moveSpeed = ((int)_spd*10)/10.f+2;
			schedule(schedule_selector(CardListViewer::moveAnimation));
		}
	}
	
	if(touched_index != 0)
	{
		CCNode* item_parent = getChildByTag(touched_index);
		CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(touched_index);
		item_menu->ccTouchCancelled(pTouch, pEvent);
		touched_index = 0;
	}
	
	is_touching = false;
}

void CardListViewer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority-1, false);
}

void CardListViewer::moveList(float dy)
{
	if(dy > 10)			dy = 10;
	else if(dy < -10)	dy = -10;
	
	setPositionY(getPositionY()+dy);//after_y);
}

void CardListViewer::moveAnimation()
{
	isAnimated=true;
	
	if(moveSpeed > 0)		moveSpeed -= 0.1;
	else if(moveSpeed < 0)	moveSpeed += 0.1;
	
	if((moveSpeed < 0.1 && moveSpeed > -0.1) || isAnimated == false)
	{
		unschedule(schedule_selector(CardListViewer::moveAnimation));
		moveSpeed = 0;
		isAnimated = false;
	}
	else
		moveList(moveSpeed);
}

void CardListViewer::myInit()
{
	view_rect = CCRectMake(210, 25, 215, 236);//CCRectMake(237, 22, 189, 203)
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 215, 236));
//	temp_back->setOpacity(150);
//	temp_back->setPosition(ccp(210+107.5, 25+118));
//	addChild(temp_back);
	
	setPosition(CCPointZero);
	
	is_touching = false;
	touched_index = 0;
	
	CCLayer::init();
}

CardListViewer* CardListViewer::create()
{
	CardListViewer* t_clv = new CardListViewer();
	t_clv->myInit();
	t_clv->autorelease();
	return t_clv;
}

void CardListViewer::setPercentage( float t_p )
{
	setPosition(ccp(getPositionX(),max_positionY*t_p));
	for(int i=0;i<getChildrenCount();i++)
	{
		CCNode* t_child = (CCNode*)getChildren()->objectAtIndex(i);
//		int tag = t_child->getTag();
//		if(tag == kCSS_MT_selectedCheck || tag == kCSS_MT_checkMark)		continue;
		((CLV_Node*)t_child)->viewCheck();
	}
}

CCRect CardListViewer::getViewRect()
{
	return view_rect;
}




CLV_Node* CLV_Node::create( int t_card_stage, int t_card_level, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect )
{
	CLV_Node* t_n = new CLV_Node();
	t_n->myInit(t_card_stage, t_card_level, t_menu, d_menu, t_position, t_rect);
	t_n->autorelease();
	return t_n;
}

CLV_Node* CLV_Node::create( int t_card_number, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect )
{
	CLV_Node* t_n = new CLV_Node();
	t_n->myInit(t_card_number, t_menu, d_menu, t_position, t_rect);
	t_n->autorelease();
	return t_n;
}

void CLV_Node::viewCheck()
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

bool CLV_Node::isSetted()
{
	return is_setted;
}

int CLV_Node::getMyTag()
{
	return my_tag;
}

void CLV_Node::setChild()
{
	if(is_card_number_setted)
	{
		if(mySGD->isHasGottenCards(card_number) != 0)
		{
//			bool is_color = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0;

			GraySprite* t_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
				card_number)->getCString()));
			t_card->setScale(0.92f);
			t_card->setPosition(CCPointZero);
			addChild(t_card, kCSS_Z_content, kCSS_MT_cardBase+card_number);

//			if(is_color)		t_card->setGray(false);
//			else				t_card->setGray(true);

			CCSprite* t_no = CCSprite::create("cardsetting_noimg.png");
			t_no->setPosition(CCPointZero);
			addChild(t_no, kCSS_Z_content);

//			CCLabelTTF* t_durability = CCLabelTTF::create(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number),
//				NSDS_GI(kSDS_CI_int1_durability_i, card_number))->getCString(),
//				mySGD->getFont().c_str(), 10);
//			t_durability->setAnchorPoint(ccp(0.5f,0.5f));
//			t_durability->setColor(ccBLACK);
//			t_durability->setHorizontalAlignment(kCCTextAlignmentLeft);
//			t_durability->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
//			t_durability->setPosition(ccp(t_card->getContentSize().width/2.f+15, 7));
//			t_card->addChild(t_durability);

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
		int t_card_number = NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level);
		if(mySGD->isHasGottenCards(t_card_number) != 0)
		{
//			bool is_color = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, t_card_number) > 0;

			GraySprite* t_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png", t_card_number)->getCString()));
			t_card->setScale(0.92f);
			t_card->setPosition(CCPointZero);
			addChild(t_card, kCSS_Z_content, kCSS_MT_cardBase+t_card_number);

//			if(is_color)		t_card->setGray(false);
//			else				t_card->setGray(true);

			CCSprite* t_no = CCSprite::create("cardsetting_noimg.png");
			t_no->setPosition(CCPointZero);
			addChild(t_no, kCSS_Z_content);

//			CCLabelTTF* t_durability = CCLabelTTF::create(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, t_card_number),
//				NSDS_GI(kSDS_CI_int1_durability_i, t_card_number))->getCString(),
//				mySGD->getFont().c_str(), 10);
//			t_durability->setAnchorPoint(ccp(0.5f,0.5f));
//			t_durability->setColor(ccBLACK);
//			t_durability->setHorizontalAlignment(kCCTextAlignmentLeft);
//			t_durability->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
//			t_durability->setPosition(ccp(t_card->getContentSize().width/2.f+15, 7));
//			t_card->addChild(t_durability);

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

void CLV_Node::myInit( int t_card_stage, int t_card_level, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect )
{
	is_card_number_setted = false;
	card_stage = t_card_stage;
	card_level = t_card_level;

	dataSet(t_menu, d_menu, t_position, t_rect);

	setPosition(my_position);

	my_size = CCSizeMake(60, 78);
	is_setted = false;

	if(mySGD->isHasGottenCards(card_stage, card_level) != 0)
		my_tag = kCSS_MT_cardMenuBase+NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level);
	else
		my_tag = kCSS_MT_noCardBase+NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level); // if (not loaded stage info) then my_tag = 30000 -> error

	CCRect tt_rect = CCRectMake(my_position.x-my_size.width/2.f, my_position.y-my_size.height/2.f, my_size.width, my_size.height);
	if(parent_view_rect.intersectsRect(tt_rect))
	{
		setChild();
	}
}

void CLV_Node::myInit( int t_card_number, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect )
{
	is_card_number_setted = true;
	card_number = t_card_number;

	dataSet(t_menu, d_menu, t_position, t_rect);

	setPosition(my_position);

	my_size = CCSizeMake(60, 78);
	is_setted = false;

	if(mySGD->isHasGottenCards(card_number) != 0)
		my_tag = kCSS_MT_cardMenuBase+card_number;
	else
		my_tag = kCSS_MT_noCardBase+card_number; // if (not loaded stage info) then my_tag = 30000 -> error

	CCRect tt_rect = CCRectMake(my_position.x-my_size.width/2.f, my_position.y-my_size.height/2.f, my_size.width, my_size.height);
	if(parent_view_rect.intersectsRect(tt_rect))
	{
		setChild();
	}
}

void CLV_Node::dataSet( CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect )
{
	target_menu = t_menu;
	delegate_menu = d_menu;
	my_position = t_position;
	parent_view_rect = t_rect;
}
