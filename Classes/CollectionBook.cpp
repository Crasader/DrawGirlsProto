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
#include "CardSettingScene.h"
#include "utf8.h"
#include "DiaryZoom.h"

CCScene* CollectionBook::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CollectionBook *layer = CollectionBook::create();
	
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
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
	kCB_MT_zoom,
	kCB_MT_pre,
	kCB_MT_next,
	kCB_MT_second,
	kCB_MT_third,
	kCB_MT_cardBase = 10000
};

bool CollectionBook::onTextFieldInsertText(cocos2d::CCTextFieldTTF *sender, const char *text, int nLen)
{
    string tempString = sender->getString();
	if(tempString == "")
	{
		return false;
	}
	else
	{
		basic_string<wchar_t> result;
		utf8::utf8to16(tempString.begin(), tempString.end(), back_inserter(result));
		
		if(result.length() > 30)
		{
			result = result.substr(0,30);
			string conver;
			utf8::utf16to8(result.begin(), result.end(), back_inserter(conver));
			sender->setString(conver.c_str());
		}
	}
	
	return false;
}

bool CollectionBook::onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *sender)
{
    was_open_text = true;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    CCMoveBy* t_move = CCMoveBy::create(0.3f, ccp(0,105));
    runAction(t_move);
#endif
    return false;
}

bool CollectionBook::onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *sender)
{
    was_open_text = false;
	string tempString = sender->getString();
	if(tempString == "")
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        CCMoveBy* t_move = CCMoveBy::create(0.3f, ccp(0,-105));
        runAction(t_move);
#endif
		return false;
	}
	else
	{
		basic_string<wchar_t> result;
		utf8::utf8to16(tempString.begin(), tempString.end(), back_inserter(result));
		
		if(result.length() > 30)
		{
			result = result.substr(0,30);
			string conver;
			utf8::utf16to8(result.begin(), result.end(), back_inserter(conver));
			sender->setString(conver.c_str());
		}
	}
    
    myDSH->setStringForKey(kDSH_Key_inputTextCard_int1, recent_card_number, sender->getString());
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    CCMoveBy* t_move = CCMoveBy::create(0.3f, ccp(0,-105));
    runAction(t_move);
#endif
    
	return false;
}

void CollectionBook::setRightPage(CCNode *target, int card_number)
{
	CCLabelTTF* r_stage_script = CCLabelTTF::create(mySD->getScriptString(card_number/10, card_number%10 + 1).c_str(), mySGD->getFont().c_str(), 15, CCSizeMake(185, 150), kCCTextAlignmentLeft);
	r_stage_script->setPosition(ccp(27,175));
	r_stage_script->setColor(ccBLACK);
	r_stage_script->setVerticalAlignment(kCCVerticalTextAlignmentTop);
	r_stage_script->setAnchorPoint(ccp(0,1));
	target->addChild(r_stage_script);
	
	
	CCMenuItem* r_close_item = CCMenuItemImage::create("sspl_cancel.png", "sspl_cancel.png", this, menu_selector(CollectionBook::menuAction));
	r_close_item->setTag(kCB_MT_close);
	
	CCMenu* r_close_menu = CCMenu::createWithItem(r_close_item);
	r_close_menu->setPosition(ccp(210, 290));
	target->addChild(r_close_menu, 1, kCB_MT_close);
	
	CCSprite* n_zoom = CCSprite::create("cardsetting_zoom.png");
	CCSprite* s_zoom = CCSprite::create("cardsetting_zoom.png");
	s_zoom->setColor(ccGRAY);
	
	CCMenuItem* zoom_item = CCMenuItemSprite::create(n_zoom, s_zoom, this, menu_selector(CollectionBook::menuAction));
	zoom_item->setTag(kCB_MT_zoom);
	
	CCMenu* zoom_menu = CCMenu::createWithItem(zoom_item);
	zoom_menu->setPosition(ccp(45,35));
	target->addChild(zoom_menu, 1, kCB_MT_zoom);
	
	float mul_value = 0.16f;
    int stage_number = card_number/10;
    int level_number = card_number%10 + 1;
    if(level_number == 1)
    {
        if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, card_number+1) != 0)
        {
            CCSprite* second_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", stage_number, level_number+1)->getCString());
            second_img->setScale(mul_value);
            second_img->setPosition(ccp(44,235));
            target->addChild(second_img);
			
			CCMenuItem* second_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CollectionBook::menuAction));
			second_item->setTag(kCB_MT_cardBase + card_number+1);
			
			CCMenu* second_menu = CCMenu::createWithItem(second_item);
			second_menu->setPosition(second_img->getPosition());
			target->addChild(second_menu, 1, kCB_MT_second);
        }
        
        if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, card_number+2) != 0)
        {
            CCSprite* third_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", stage_number, level_number+2)->getCString());
            third_img->setScale(mul_value);
            third_img->setPosition(ccp(116,235));
            target->addChild(third_img);
            
            if(mySD->isAnimationStage(stage_number))
            {
                CCSize ani_size = mySD->getAnimationCutSize(stage_number);
                CCSprite* ani_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", stage_number, level_number+2)->getCString(), CCRectMake(0, 0, ani_size.width, ani_size.height));
                ani_img->setPosition(mySD->getAnimationPosition(stage_number));
                third_img->addChild(ani_img);
            }
			
			CCMenuItem* third_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CollectionBook::menuAction));
			third_item->setTag(kCB_MT_cardBase + card_number+2);
			
			CCMenu* third_menu = CCMenu::createWithItem(third_item);
			third_menu->setPosition(third_img->getPosition());
			target->addChild(third_menu, 1, kCB_MT_third);
        }
    }
    else if(level_number == 2)
    {
        if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, card_number-1) != 0)
        {
            CCSprite* first_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", stage_number, level_number-1)->getCString());
            first_img->setScale(mul_value);
            first_img->setPosition(ccp(44,235));
            target->addChild(first_img);
			
			CCMenuItem* second_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CollectionBook::menuAction));
			second_item->setTag(kCB_MT_cardBase + card_number-1);
			
			CCMenu* second_menu = CCMenu::createWithItem(second_item);
			second_menu->setPosition(first_img->getPosition());
			target->addChild(second_menu, 1, kCB_MT_second);
        }
        
        if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, card_number+1) != 0)
        {
            CCSprite* third_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", stage_number, level_number+1)->getCString());
            third_img->setScale(mul_value);
            third_img->setPosition(ccp(116,235));
            target->addChild(third_img);
            
            if(mySD->isAnimationStage(stage_number))
            {
                CCSize ani_size = mySD->getAnimationCutSize(stage_number);
                CCSprite* ani_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", stage_number, level_number+1)->getCString(), CCRectMake(0, 0, ani_size.width, ani_size.height));
                ani_img->setPosition(mySD->getAnimationPosition(stage_number));
                third_img->addChild(ani_img);
            }
			
			CCMenuItem* third_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CollectionBook::menuAction));
			third_item->setTag(kCB_MT_cardBase + card_number+1);
			
			CCMenu* third_menu = CCMenu::createWithItem(third_item);
			third_menu->setPosition(third_img->getPosition());
			target->addChild(third_menu, 1, kCB_MT_third);
        }
    }
    else if(level_number == 3)
    {
        if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, card_number-2) != 0)
        {
            CCSprite* first_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", stage_number, level_number-2)->getCString());
            first_img->setScale(mul_value);
            first_img->setPosition(ccp(44,235));
            target->addChild(first_img);
			
			CCMenuItem* second_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CollectionBook::menuAction));
			second_item->setTag(kCB_MT_cardBase + card_number-2);
			
			CCMenu* second_menu = CCMenu::createWithItem(second_item);
			second_menu->setPosition(first_img->getPosition());
			target->addChild(second_menu, 1, kCB_MT_second);
        }
        
        if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, card_number-1) != 0)
        {
            CCSprite* second_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", stage_number, level_number-1)->getCString());
            second_img->setScale(mul_value);
            second_img->setPosition(ccp(116,235));
            target->addChild(second_img);
			
			CCMenuItem* third_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CollectionBook::menuAction));
			third_item->setTag(kCB_MT_cardBase + card_number-1);
			
			CCMenu* third_menu = CCMenu::createWithItem(third_item);
			third_menu->setPosition(second_img->getPosition());
			target->addChild(third_menu, 1, kCB_MT_third);
        }
    }
}

void CollectionBook::setLeftPage(CCNode *target, int card_number)
{
	CCSprite* r_card_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", card_number/10, card_number%10 + 1)->getCString());
	r_card_img->setScale(0.56);
	r_card_img->setPosition(ccp(122,149));
	target->addChild(r_card_img);
	
	int stage_number = card_number / 10;
	int level_number = card_number % 10 + 1;
	
	if(level_number == 3 && mySD->isAnimationStage(stage_number))
	{
		CollectionAnimation* t_ca = CollectionAnimation::create(card_number);
		r_card_img->addChild(t_ca);
	}
	
	CCLabelTTF* r_stage_label = CCLabelTTF::create(CCString::createWithFormat("STAGE %d-%d", stage_number, level_number)->getCString(), mySGD->getFont().c_str(), 8);
	r_stage_label->setPosition(ccp(185, 287));
	r_stage_label->setColor(ccBLACK);
	r_stage_label->setHorizontalAlignment(kCCTextAlignmentCenter);
	r_stage_label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	r_stage_label->setAnchorPoint(ccp(0.5,0.5));
	target->addChild(r_stage_label);
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
    
	was_open_text = false;
	setKeypadEnabled(true);
	
	recent_card_number = mySGD->selected_collectionbook;
	
	int next_number = mySGD->getNextStageCardNumber(recent_card_number);
	int pre_number = mySGD->getPreStageCardNumber(recent_card_number);
	
	if(next_number == -1 || pre_number == -1)
		is_enable_pageturn = false;
	else
		is_enable_pageturn = true;
	
	recent_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
	recent_left_img->setAnchorPoint(ccp(1.f,0.5f));
	recent_left_img->setPosition(ccp(240,160));
	addChild(recent_left_img, kCB_Z_recent);
	
	setLeftPage(recent_left_img, recent_card_number);
	
	
	recent_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
	recent_right_img->setAnchorPoint(ccp(0.f,0.5f));
	recent_right_img->setPosition(ccp(240,160));
	addChild(recent_right_img, kCB_Z_recent);
	
	setRightPage(recent_right_img, recent_card_number);
	
	string input_data = myDSH->getStringForKey(kDSH_Key_inputTextCard_int1, recent_card_number).c_str();
    if(input_data == "")
        input_data = "입력해주세요.";
    
    input_text = CCTextFieldTTF::textFieldWithPlaceHolder(input_data.c_str(), CCSizeMake(190,45), kCCTextAlignmentLeft, mySGD->getFont().c_str(), 15);
    input_text->setPosition(ccp(121,94));
    input_text->setAnchorPoint(ccp(0.5,0.5));
    recent_right_img->addChild(input_text);
    
    if(input_data != "입력해주세요.")
        input_text->setString(input_data.c_str());
    
    input_text->setDelegate(this);
	
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
		
		
		
		after_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
		after_left_img->setAnchorPoint(ccp(1.f,0.5f));
		after_left_img->setPosition(ccp(240,160));
		addChild(after_left_img, kCB_Z_after);
		
		setLeftPage(after_left_img, pre_number);
		
		
		after_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
		after_right_img->setAnchorPoint(ccp(0.f,0.5f));
		after_right_img->setPosition(ccp(240,160));
		addChild(after_right_img, kCB_Z_after);
		
		setRightPage(after_right_img, next_number);
		
		((CCMenu*)after_right_img->getChildByTag(kCB_MT_close))->setEnabled(false);
		((CCMenu*)after_right_img->getChildByTag(kCB_MT_zoom))->setEnabled(false);
		if(after_right_img->getChildByTag(kCB_MT_second))
			((CCMenu*)after_right_img->getChildByTag(kCB_MT_second))->setEnabled(false);
		if(after_right_img->getChildByTag(kCB_MT_third))
			((CCMenu*)after_right_img->getChildByTag(kCB_MT_third))->setEnabled(false);
		
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
		setTouchEnabled(true);
	}
	
	is_menu_enable = true;
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
    return true;
}

bool CollectionBook::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCPoint location = pTouch->getLocationInView();
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(location));
	
    CCRect textFieldRect = CCRectMake(0, 0, input_text->getContentSize().width, input_text->getContentSize().height);
    textFieldRect = CCRectApplyAffineTransform(textFieldRect, input_text->nodeToWorldTransform());
    
    if(!was_open_text && textFieldRect.containsPoint(convertedLocation))
    {
        input_text->attachWithIME();
        touch_direction = 0;
    }
    else
    {
        if(!is_touch_enable)
			return true;
        
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
    }
	
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
	
	if(was_open_text)	return;
	
	CCPoint location = pTouch->getLocationInView();
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(location));
	
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
	((CCMenu*)recent_right_img->getChildByTag(kCB_MT_zoom))->setEnabled(true);
	((CCMenu*)recent_right_img->getChildByTag(kCB_MT_pre))->setEnabled(true);
	((CCMenu*)recent_right_img->getChildByTag(kCB_MT_next))->setEnabled(true);
	
	if(recent_right_img->getChildByTag(kCB_MT_second))
		((CCMenu*)recent_right_img->getChildByTag(kCB_MT_second))->setEnabled(true);
	if(recent_right_img->getChildByTag(kCB_MT_third))
		((CCMenu*)recent_right_img->getChildByTag(kCB_MT_third))->setEnabled(true);
	
	
	covered_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
	covered_left_img->setAnchorPoint(ccp(1.f,0.5f));
	covered_left_img->setPosition(ccp(240,160));
	addChild(covered_left_img, kCB_Z_cover);
	
	int next_number = mySGD->getNextStageCardNumber(recent_card_number);
	recent_card_number = next_number;
	
	string input_data = myDSH->getStringForKey(kDSH_Key_inputTextCard_int1, recent_card_number).c_str();
    if(input_data == "")
        input_data = "입력해주세요.";
    
    input_text = CCTextFieldTTF::textFieldWithPlaceHolder(input_data.c_str(), CCSizeMake(190,45), kCCTextAlignmentLeft, mySGD->getFont().c_str(), 15);
    input_text->setPosition(ccp(121,94));
    input_text->setAnchorPoint(ccp(0.5,0.5));
    recent_right_img->addChild(input_text);
    
    if(input_data != "입력해주세요.")
        input_text->setString(input_data.c_str());
    
    input_text->setDelegate(this);
	
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
	
	int pre_number = mySGD->getPreStageCardNumber(recent_card_number);
	after_left_img->removeFromParent();
	
	after_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
	after_left_img->setAnchorPoint(ccp(1.f,0.5f));
	after_left_img->setPosition(ccp(240,160));
	addChild(after_left_img, kCB_Z_after);
	
	setLeftPage(after_left_img, pre_number);
	
	
	reorderChild(recent_left_img, kCB_Z_recent);
	
	after_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
	after_right_img->setAnchorPoint(ccp(0.f, 0.5f));
	after_right_img->setPosition(ccp(240,160));
	addChild(after_right_img, kCB_Z_after);
	
	setRightPage(after_right_img, mySGD->getNextStageCardNumber(recent_card_number));
	
	((CCMenu*)after_right_img->getChildByTag(kCB_MT_close))->setEnabled(false);
	((CCMenu*)after_right_img->getChildByTag(kCB_MT_zoom))->setEnabled(false);
	
	
	if(after_right_img->getChildByTag(kCB_MT_second))
		((CCMenu*)after_right_img->getChildByTag(kCB_MT_second))->setEnabled(false);
	if(after_right_img->getChildByTag(kCB_MT_third))
		((CCMenu*)after_right_img->getChildByTag(kCB_MT_third))->setEnabled(false);
	
	
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
	
	recent_card_number = mySGD->getPreStageCardNumber(recent_card_number);
	
	covered_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
	covered_right_img->setAnchorPoint(ccp(0.f,0.5f));
	covered_right_img->setPosition(ccp(240,160));
	addChild(covered_right_img, kCB_Z_cover);
	
	setRightPage(covered_right_img, recent_card_number);
	
	string input_data = myDSH->getStringForKey(kDSH_Key_inputTextCard_int1, recent_card_number).c_str();
    if(input_data == "")
        input_data = "입력해주세요.";
    
    input_text = CCTextFieldTTF::textFieldWithPlaceHolder(input_data.c_str(), CCSizeMake(190,45), kCCTextAlignmentLeft, mySGD->getFont().c_str(), 15);
    input_text->setPosition(ccp(121,94));
    input_text->setAnchorPoint(ccp(0.5,0.5));
    covered_right_img->addChild(input_text);
    
    if(input_data != "입력해주세요.")
        input_text->setString(input_data.c_str());
    
    input_text->setDelegate(this);
	
	((CCMenu*)covered_right_img->getChildByTag(kCB_MT_close))->setEnabled(false);
	((CCMenu*)covered_right_img->getChildByTag(kCB_MT_zoom))->setEnabled(false);
	
	if(covered_right_img->getChildByTag(kCB_MT_second))
		((CCMenu*)covered_right_img->getChildByTag(kCB_MT_second))->setEnabled(false);
	if(covered_right_img->getChildByTag(kCB_MT_third))
		((CCMenu*)covered_right_img->getChildByTag(kCB_MT_third))->setEnabled(false);
	
		
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
	((CCMenu*)recent_right_img->getChildByTag(kCB_MT_zoom))->setEnabled(true);
	((CCMenu*)recent_right_img->getChildByTag(kCB_MT_pre))->setEnabled(true);
	((CCMenu*)recent_right_img->getChildByTag(kCB_MT_next))->setEnabled(true);
	
	if(recent_right_img->getChildByTag(kCB_MT_second))
		((CCMenu*)recent_right_img->getChildByTag(kCB_MT_second))->setEnabled(true);
	if(recent_right_img->getChildByTag(kCB_MT_third))
		((CCMenu*)recent_right_img->getChildByTag(kCB_MT_third))->setEnabled(true);
	
	after_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
	after_left_img->setAnchorPoint(ccp(1.f, 0.5f));
	after_left_img->setPosition(ccp(240,160));
	addChild(after_left_img, kCB_Z_after);
	
	int pre_number = mySGD->getPreStageCardNumber(recent_card_number);
	
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
	
	if(was_open_text)	return;
	
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
	
	if(was_open_text)	return;
	
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
		CCDirector::sharedDirector()->replaceScene(CardSettingScene::scene());
	}
	else if(tag == kCB_MT_zoom)
	{
		mySGD->selected_collectionbook = recent_card_number;
		CCDirector::sharedDirector()->replaceScene(DiaryZoom::scene());
	}
	else if(tag == kCB_MT_pre)
	{
		startPrePageFull();
	}
	else if(tag == kCB_MT_next)
	{
		startNextPageFull();
	}
	else if(tag >= kCB_MT_cardBase)
	{
		int t_tag = tag - kCB_MT_cardBase;
		
		mySGD->selected_collectionbook = t_tag;
		CCDirector::sharedDirector()->replaceScene(CollectionBook::scene());
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