//
//  CollectionBook.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 20..
//
//

#include "CollectionBook.h"
#include "ScreenSide.h"
#include "MyLocalization.h"
#include "WorldMapScene.h"
#include "CollectionListScene.h"


CCScene* CollectionBook::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CollectionBook *layer = CollectionBook::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

enum CB_Zorder{
	kCB_Z_after = 1,
	kCB_Z_recent,
	kCB_Z_cover
};

enum CB_MenuTag{
	kCB_MT_close = 1,
	kCB_MT_list,
	kCB_MT_pre,
	kCB_MT_next
};

void CollectionBook::setRightPage(CCNode *target, int card_number)
{
	CCLabelTTF* r_stage_label = CCLabelTTF::create(CCString::createWithFormat("STAGE %d-%d", card_number/10, card_number%10 + 1)->getCString(), mySGD->getFont().c_str(), 16);
	r_stage_label->setPosition(ccp(178, 248));
	r_stage_label->setColor(ccBLACK);
	r_stage_label->setHorizontalAlignment(kCCTextAlignmentCenter);
	r_stage_label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	r_stage_label->setAnchorPoint(ccp(0.5,0.5));
	target->addChild(r_stage_label);
	
	CCLabelTTF* r_stage_script = CCLabelTTF::create(mySD->getScriptString(card_number/10, card_number%10 + 1).c_str(), mySGD->getFont().c_str(), 18, CCSizeMake(185, 140), kCCTextAlignmentLeft);
	r_stage_script->setPosition(ccp(30,193));
	r_stage_script->setColor(ccBLACK);
	r_stage_script->setVerticalAlignment(kCCVerticalTextAlignmentTop);
	r_stage_script->setAnchorPoint(ccp(0,1));
	target->addChild(r_stage_script);
	
	
	CCMenuItem* r_close_item = CCMenuItemImage::create("sspl_cancel.png", "sspl_cancel.png", this, menu_selector(CollectionBook::menuAction));
	r_close_item->setTag(kCB_MT_close);
	
	CCMenu* r_close_menu = CCMenu::createWithItem(r_close_item);
	r_close_menu->setPosition(ccp(210, 290));
	target->addChild(r_close_menu, 1, kCB_MT_close);
	
	CCMenuItem* r_list_item = CCMenuItemImage::create("collectionbook_list.png", "collectionbook_list.png", this, menu_selector(CollectionBook::menuAction));
	r_list_item->setTag(kCB_MT_list);
	
	CCMenu* r_list_menu = CCMenu::createWithItem(r_list_item);
	r_list_menu->setPosition(ccp(40,35));
	target->addChild(r_list_menu, 1, kCB_MT_list);
}

void CollectionBook::setLeftPage(CCNode *target, int card_number)
{
	CCSprite* r_card_img = CCSprite::create(CCString::createWithFormat("stage%d_level%d_visible.png", card_number/10, card_number%10 + 1)->getCString());
	r_card_img->setScale(0.65);
	r_card_img->setPosition(ccp(120,160));
	target->addChild(r_card_img);
	
	int stage_number = card_number / 10;
	int level_number = card_number % 10 + 1;
	
	if(level_number == 3 && mySD->isAnimationStage(stage_number))
	{
		CollectionAnimation* t_ca = CollectionAnimation::create(card_number);
		r_card_img->addChild(t_ca);
	}
}

// on "init" you need to initialize your instance
bool CollectionBook::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	setKeypadEnabled(true);
	
	recent_card_number = mySGD->selected_collectionbook;
	
	int next_number = mySGD->getNextCardNumber(recent_card_number);
	int pre_number = mySGD->getPreCardNumber(recent_card_number);
	
	if(next_number == -1 || pre_number == -1)
		is_enable_pageturn = false;
	else
		is_enable_pageturn = true;
	
	recent_left_img = CCSprite::create("collectionbook_back.png", CCRectMake(0, 0, 240, 320));
	recent_left_img->setAnchorPoint(ccp(1.f,0.5f));
	recent_left_img->setPosition(ccp(240,160));
	addChild(recent_left_img, kCB_Z_recent);
	
	setLeftPage(recent_left_img, recent_card_number);
	
	
	recent_right_img = CCSprite::create("collectionbook_back.png", CCRectMake(240, 0, 240, 320));
	recent_right_img->setAnchorPoint(ccp(0.f,0.5f));
	recent_right_img->setPosition(ccp(240,160));
	addChild(recent_right_img, kCB_Z_recent);
	
	setRightPage(recent_right_img, recent_card_number);
	
	if(is_enable_pageturn)
	{
		CCMenuItem* r_pre_item = CCMenuItemImage::create("collectionbook_pre.png", "collectionbook_pre.png", this, menu_selector(CollectionBook::menuAction));
		r_pre_item->setTag(kCB_MT_pre);
		
		CCMenu* r_pre_menu = CCMenu::createWithItem(r_pre_item);
		r_pre_menu->setPosition(ccp(150, 35));
		recent_right_img->addChild(r_pre_menu, 1, kCB_MT_pre);
		
		CCMenuItem* r_next_item = CCMenuItemImage::create("collectionbook_next.png", "collectionbook_next.png", this, menu_selector(CollectionBook::menuAction));
		r_next_item->setTag(kCB_MT_next);
		
		CCMenu* r_next_menu = CCMenu::createWithItem(r_next_item);
		r_next_menu->setPosition(ccp(200,35));
		recent_right_img->addChild(r_next_menu, 1, kCB_MT_next);
		
		
		
		after_left_img = CCSprite::create("collectionbook_back.png", CCRectMake(0, 0, 240, 320));
		after_left_img->setAnchorPoint(ccp(1.f,0.5f));
		after_left_img->setPosition(ccp(240,160));
		addChild(after_left_img, kCB_Z_after);
		
		setLeftPage(after_left_img, pre_number);
		
		
		after_right_img = CCSprite::create("collectionbook_back.png", CCRectMake(240, 0, 240, 320));
		after_right_img->setAnchorPoint(ccp(0.f,0.5f));
		after_right_img->setPosition(ccp(240,160));
		addChild(after_right_img, kCB_Z_after);
		
		setRightPage(after_right_img, next_number);
		
		((CCMenu*)after_right_img->getChildByTag(kCB_MT_close))->setEnabled(false);
		((CCMenu*)after_right_img->getChildByTag(kCB_MT_list))->setEnabled(false);
		
		CCMenuItem* a_pre_item = CCMenuItemImage::create("collectionbook_pre.png", "collectionbook_pre.png", this, menu_selector(CollectionBook::menuAction));
		a_pre_item->setTag(kCB_MT_pre);
		
		CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
		a_pre_menu->setPosition(ccp(150, 35));
		a_pre_menu->setEnabled(false);
		after_right_img->addChild(a_pre_menu, 1, kCB_MT_pre);
		
		CCMenuItem* a_next_item = CCMenuItemImage::create("collectionbook_next.png", "collectionbook_next.png", this, menu_selector(CollectionBook::menuAction));
		a_next_item->setTag(kCB_MT_next);
		
		CCMenu* a_next_menu = CCMenu::createWithItem(a_next_item);
		a_next_menu->setPosition(ccp(200,35));
		a_next_menu->setEnabled(false);
		after_right_img->addChild(a_next_menu, 1, kCB_MT_next);
		
		is_touch_enable = true;
		setTouchEnabled(true);
	}
	else
	{
		is_touch_enable = false;
		setTouchEnabled(false);
	}
	
	is_menu_enable = true;
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
    return true;
}

bool CollectionBook::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touch_enable)
		return true;
	
	CCPoint location = pTouch->getLocationInView();
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);
	
	begin_point = convertedLocation;
	
	if(begin_point.x > 240)
	{
		touch_direction = 1;
	}
	else
	{
		touch_direction = -1;
	}
	
	is_menu_enable = false;
	
	return true;
}

void CollectionBook::startNextPageFull()
{
	setTouchEnabled(false);
	is_touch_enable = false;
	animation_angle = 0;
	animation_img = recent_right_img;
	touch_direction = 1;
	touch_end_direction = -1;
	end_animation_delegate = callfunc_selector(CollectionBook::startNextPage);
	
	schedule(schedule_selector(CollectionBook::ingPageFull));
}

void CollectionBook::startPrePageFull()
{
	setTouchEnabled(false);
	is_touch_enable = false;
	animation_angle = 0;
	animation_img = recent_left_img;
	touch_direction = -1;
	touch_end_direction = -1;
	end_animation_delegate = callfunc_selector(CollectionBook::startPrePage);
	
	schedule(schedule_selector(CollectionBook::ingPageFull));
}

void CollectionBook::ingPageFull()
{
	animation_angle += 3.f;
	
	animation_img->setSkewY(-touch_direction*touch_end_direction*animation_angle/4.f);
	animation_img->setScaleX(cosf(animation_angle/180.f*M_PI));
	
	if(animation_angle >= 90.f)
	{
		unschedule(schedule_selector(CollectionBook::ingPageFull));
		animation_img = NULL;
		(this->*end_animation_delegate)();
	}
}

void CollectionBook::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touch_enable)
		return;
	
	CCPoint location = pTouch->getLocationInView();
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);
	
	float x_distance = begin_point.x - convertedLocation.x;
	
	if(touch_direction == 1)
	{
		if(x_distance < 0)
			return;
		
		animation_angle = 9.f/10.f*x_distance;
		
		if(animation_angle > 90)
		{
			setTouchEnabled(false);
			is_touch_enable = false;
			startNextPage();
			return;
		}
		
		float skewY_value = animation_angle / 4.f;
		float scaleX_value = cosf(animation_angle/180.f*M_PI);
		
		recent_right_img->setSkewY(skewY_value);
		recent_right_img->setScaleX(scaleX_value);
		
	}
	else if(touch_direction == -1)
	{
		if(x_distance > 0)
			return;
		x_distance *= -1.f;
		
		animation_angle = 9.f/10.f*x_distance;
		
		if(animation_angle > 90)
		{
			setTouchEnabled(false);
			is_touch_enable = false;
			startPrePage();
			return;
		}
		
		float skewY_value = -animation_angle / 4.f;
		float scaleX_value = cosf(animation_angle/180.f*M_PI);
		
		recent_left_img->setSkewY(skewY_value);
		recent_left_img->setScaleX(scaleX_value);
	}
}

void CollectionBook::startNextPage()
{
	recent_right_img->removeFromParent();
	recent_right_img = after_right_img;
	after_right_img = NULL;
	
	reorderChild(recent_right_img, kCB_Z_recent);
	((CCMenu*)recent_right_img->getChildByTag(kCB_MT_close))->setEnabled(true);
	((CCMenu*)recent_right_img->getChildByTag(kCB_MT_list))->setEnabled(true);
	((CCMenu*)recent_right_img->getChildByTag(kCB_MT_pre))->setEnabled(true);
	((CCMenu*)recent_right_img->getChildByTag(kCB_MT_next))->setEnabled(true);
	
	covered_left_img = CCSprite::create("collectionbook_back.png", CCRectMake(0, 0, 240, 320));
	covered_left_img->setAnchorPoint(ccp(1.f,0.5f));
	covered_left_img->setPosition(ccp(240,160));
	addChild(covered_left_img, kCB_Z_cover);
	
	int next_number = mySGD->getNextCardNumber(recent_card_number);
	recent_card_number = next_number;
	
	setLeftPage(covered_left_img, next_number);
	
	
	animation_angle = 90.f;
	
	covered_left_img->setSkewY(animation_angle/4.f);
	covered_left_img->setScaleX(cosf(animation_angle/180.f*M_PI));
	
	animation_img = covered_left_img;
	end_animation_delegate = callfunc_selector(CollectionBook::endNextPage);
	
	touch_end_direction = 1;
	
	schedule(schedule_selector(CollectionBook::ingPage));
}

void CollectionBook::ingPage()
{
	animation_angle -= 3.f;
	
	if(animation_angle < 0.f)
		animation_angle = 0.f;
	
	animation_img->setSkewY(-touch_direction*touch_end_direction*animation_angle/4.f);
	animation_img->setScaleX(cosf(animation_angle/180.f*M_PI));
	
	if(animation_angle <= 0.f)
	{
		unschedule(schedule_selector(CollectionBook::ingPage));
		animation_img = NULL;
		(this->*end_animation_delegate)();
	}
}

void CollectionBook::endNextPage()
{
	after_left_img->removeFromParent();
	after_left_img = recent_left_img;
	recent_left_img = covered_left_img;
	covered_left_img = NULL;
	
	reorderChild(recent_left_img, kCB_Z_recent);
	
	after_right_img = CCSprite::create("collectionbook_back.png", CCRectMake(240, 0, 240, 320));
	after_right_img->setAnchorPoint(ccp(0.f, 0.5f));
	after_right_img->setPosition(ccp(240,160));
	addChild(after_right_img, kCB_Z_after);
	
	setRightPage(after_right_img, mySGD->getNextCardNumber(recent_card_number));
	
	((CCMenu*)after_right_img->getChildByTag(kCB_MT_close))->setEnabled(false);
	((CCMenu*)after_right_img->getChildByTag(kCB_MT_list))->setEnabled(false);
	
	
	CCMenuItem* a_pre_item = CCMenuItemImage::create("collectionbook_pre.png", "collectionbook_pre.png", this, menu_selector(CollectionBook::menuAction));
	a_pre_item->setTag(kCB_MT_pre);
	
	CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
	a_pre_menu->setPosition(ccp(150, 35));
	a_pre_menu->setEnabled(false);
	after_right_img->addChild(a_pre_menu, 1, kCB_MT_pre);
	
	CCMenuItem* a_next_item = CCMenuItemImage::create("collectionbook_next.png", "collectionbook_next.png", this, menu_selector(CollectionBook::menuAction));
	a_next_item->setTag(kCB_MT_next);
	
	CCMenu* a_next_menu = CCMenu::createWithItem(a_next_item);
	a_next_menu->setPosition(ccp(200,35));
	a_next_menu->setEnabled(false);
	after_right_img->addChild(a_next_menu, 1, kCB_MT_next);
	
	endPage();
}

void CollectionBook::startPrePage()
{
	recent_left_img->removeFromParent();
	recent_left_img = after_left_img;
	after_left_img = NULL;
	
	reorderChild(recent_left_img, kCB_Z_recent);
	
	recent_card_number = mySGD->getPreCardNumber(recent_card_number);
	
	covered_right_img = CCSprite::create("collectionbook_back.png", CCRectMake(240, 0, 240, 320));
	covered_right_img->setAnchorPoint(ccp(0.f,0.5f));
	covered_right_img->setPosition(ccp(240,160));
	addChild(covered_right_img, kCB_Z_cover);
	
	setRightPage(covered_right_img, recent_card_number);
	
	((CCMenu*)covered_right_img->getChildByTag(kCB_MT_close))->setEnabled(false);
	((CCMenu*)covered_right_img->getChildByTag(kCB_MT_list))->setEnabled(false);
	
		
	CCMenuItem* a_pre_item = CCMenuItemImage::create("collectionbook_pre.png", "collectionbook_pre.png", this, menu_selector(CollectionBook::menuAction));
	a_pre_item->setTag(kCB_MT_pre);
	
	CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
	a_pre_menu->setPosition(ccp(150, 35));
	a_pre_menu->setEnabled(false);
	covered_right_img->addChild(a_pre_menu, 1, kCB_MT_pre);
	
	CCMenuItem* a_next_item = CCMenuItemImage::create("collectionbook_next.png", "collectionbook_next.png", this, menu_selector(CollectionBook::menuAction));
	a_next_item->setTag(kCB_MT_next);
	
	CCMenu* a_next_menu = CCMenu::createWithItem(a_next_item);
	a_next_menu->setPosition(ccp(200,35));
	a_next_menu->setEnabled(false);
	covered_right_img->addChild(a_next_menu, 1, kCB_MT_next);
	
	
	animation_angle = 90.f;
	
	covered_right_img->setSkewY(animation_angle/4.f);
	covered_right_img->setScaleX(cosf(animation_angle/180.f*M_PI));
	
	animation_img = covered_right_img;
	end_animation_delegate = callfunc_selector(CollectionBook::endPrePage);
	
	touch_end_direction = 1;
	
	schedule(schedule_selector(CollectionBook::ingPage));
}

void CollectionBook::endPrePage()
{
	after_right_img->removeFromParent();
	after_right_img = recent_right_img;
	recent_right_img = covered_right_img;
	covered_right_img = NULL;
	
	reorderChild(recent_right_img, kCB_Z_recent);
	((CCMenu*)recent_right_img->getChildByTag(kCB_MT_close))->setEnabled(true);
	((CCMenu*)recent_right_img->getChildByTag(kCB_MT_list))->setEnabled(true);
	((CCMenu*)recent_right_img->getChildByTag(kCB_MT_pre))->setEnabled(true);
	((CCMenu*)recent_right_img->getChildByTag(kCB_MT_next))->setEnabled(true);
	
	after_left_img = CCSprite::create("collectionbook_back.png", CCRectMake(0, 0, 240, 320));
	after_left_img->setAnchorPoint(ccp(1.f, 0.5f));
	after_left_img->setPosition(ccp(240,160));
	addChild(after_left_img, kCB_Z_after);
	
	int pre_number = mySGD->getPreCardNumber(recent_card_number);
	
	setLeftPage(after_left_img, pre_number);
	
	endPage();
}

void CollectionBook::endPage()
{
	is_touch_enable = true;
	setTouchEnabled(true);
	is_menu_enable = true;
}

void CollectionBook::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touch_enable)
		return;
	
	is_touch_enable = false;
	setTouchEnabled(false);
	
	if(touch_direction == 1)
		animation_img = recent_right_img;
	else if(touch_direction == -1)
		animation_img = recent_left_img;
	
	end_animation_delegate = callfunc_selector(CollectionBook::endPage);
	
	touch_end_direction = -1;
	
	schedule(schedule_selector(CollectionBook::ingPage));
}
void CollectionBook::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touch_enable)
		return;
	
	is_touch_enable = false;
	setTouchEnabled(false);
	
	if(touch_direction == 1)
		animation_img = recent_right_img;
	else if(touch_direction == -1)
		animation_img = recent_left_img;
	
	end_animation_delegate = callfunc_selector(CollectionBook::endPage);
	
	touch_end_direction = -1;
	
	schedule(schedule_selector(CollectionBook::ingPage));
}

void CollectionBook::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority+1, false);
}

void CollectionBook::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kCB_MT_close)
	{
		CCDirector::sharedDirector()->replaceScene(WorldMapScene::scene());
	}
	else if(tag == kCB_MT_list)
	{
		CCDirector::sharedDirector()->replaceScene(CollectionListScene::scene());
	}
	else if(tag == kCB_MT_pre)
	{
		startPrePageFull();
	}
	else if(tag == kCB_MT_next)
	{
		startNextPageFull();
	}
}

void CollectionBook::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void CollectionBook::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(CollectionBook::alertAction));
}