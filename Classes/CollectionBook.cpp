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
#include "LogData.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"
#include "AlertEngine.h"

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
	kCB_MT_inputText,
	kCB_MT_cardBase = 10000
};

CCPoint CollectionBook::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kCB_MT_zoom)		return_value = ccp(42, 51);
	else if(t_tag == kCB_MT_pre)	return_value = ccp(120, 51);
	else if(t_tag == kCB_MT_next)	return_value = ccp(170, 51);
	else if(t_tag == kCB_MT_second)	return_value = ccp(93,222);
	else if(t_tag == kCB_MT_third)	return_value = ccp(163,234);
	else if(t_tag == kCB_MT_close)	return_value = ccp(220, 30);
	else if(t_tag == kCB_MT_inputText)	return_value = ccp(115,91);
	
	return return_value;
}

int CollectionBook::getContentRotate(int t_tag)
{
	int return_value;
	
	if(t_tag == kCB_MT_second)			return_value = 5;
	else if(t_tag == kCB_MT_third)		return_value = -11;
	
	return return_value;
}

void CollectionBook::setRightPage(CCNode *target, int card_number)
{
	CCLabelTTF* r_stage_script = CCLabelTTF::create(mySD->getScriptString(NSDS_GI(kSDS_CI_int1_stage_i, card_number), NSDS_GI(kSDS_CI_int1_grade_i, card_number)).c_str(), mySGD->getFont().c_str(), 12, CCSizeMake(180, 60), kCCTextAlignmentLeft);
	r_stage_script->setPosition(ccp(25,175));
	r_stage_script->setColor(ccBLACK);
	r_stage_script->setVerticalAlignment(kCCVerticalTextAlignmentTop);
	r_stage_script->setAnchorPoint(ccp(0,1));
	target->addChild(r_stage_script);
		
	CCMenuItem* r_close_item = CCMenuItemImage::create("sspl_cancel.png", "sspl_cancel.png", this, menu_selector(CollectionBook::menuAction));
	r_close_item->setTag(kCB_MT_close);
	
	CCMenu* r_close_menu = CCMenu::createWithItem(r_close_item);
	r_close_menu->setPosition(getContentPosition(kCB_MT_close));
	target->addChild(r_close_menu, 1, kCB_MT_close);
	
	CCSprite* n_zoom = CCSprite::create("diary_zoom.png");
	CCSprite* s_zoom = CCSprite::create("diary_zoom.png");
	s_zoom->setColor(ccGRAY);
	
	CCMenuItem* zoom_item = CCMenuItemSprite::create(n_zoom, s_zoom, this, menu_selector(CollectionBook::menuAction));
	zoom_item->setTag(kCB_MT_zoom);
	
	CCMenu* zoom_menu = CCMenu::createWithItem(zoom_item);
	zoom_menu->setPosition(getContentPosition(kCB_MT_zoom));
	target->addChild(zoom_menu, 1, kCB_MT_zoom);
	
	float mul_value = 0.88f;
    int stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
    int level_number = NSDS_GI(kSDS_CI_int1_grade_i, card_number);
    if(level_number == 1)
    {
        if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, level_number+1)) != 0)
        {
            CCSprite* second_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_thumbnail.png", stage_number, level_number+1)->getCString());
            second_img->setScale(mul_value);
			second_img->setRotation(getContentRotate(kCB_MT_second));
            second_img->setPosition(getContentPosition(kCB_MT_second));
            target->addChild(second_img);
			
			CCMenuItem* second_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CollectionBook::menuAction));
			second_item->setTag(kCB_MT_cardBase + NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, level_number+1));
			second_item->setRotation(getContentRotate(kCB_MT_second));
			
			CCMenu* second_menu = CCMenu::createWithItem(second_item);
			second_menu->setPosition(second_img->getPosition());
			target->addChild(second_menu, 1, kCB_MT_second);
        }
        
        if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, level_number+2)) != 0)
        {
            CCSprite* third_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_thumbnail.png", stage_number, level_number+2)->getCString());
            third_img->setScale(mul_value);
			third_img->setRotation(getContentRotate(kCB_MT_third));
            third_img->setPosition(getContentPosition(kCB_MT_third));
            target->addChild(third_img);
            
//            if(mySD->isAnimationStage(stage_number))
//            {
//                CCSize ani_size = mySD->getAnimationCutSize(stage_number);
//                CCSprite* ani_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", stage_number, level_number+2)->getCString(), CCRectMake(0, 0, ani_size.width, ani_size.height));
//                ani_img->setPosition(mySD->getAnimationPosition(stage_number));
//                third_img->addChild(ani_img);
//            }
			
			CCMenuItem* third_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CollectionBook::menuAction));
			third_item->setTag(kCB_MT_cardBase + NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, level_number+2));
			third_item->setRotation(getContentRotate(kCB_MT_third));
			
			CCMenu* third_menu = CCMenu::createWithItem(third_item);
			third_menu->setPosition(third_img->getPosition());
			target->addChild(third_menu, 1, kCB_MT_third);
        }
    }
    else if(level_number == 2)
    {
        if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, level_number-1)) != 0)
        {
            CCSprite* first_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_thumbnail.png", stage_number, level_number-1)->getCString());
            first_img->setScale(mul_value);
			first_img->setRotation(getContentRotate(kCB_MT_second));
            first_img->setPosition(getContentPosition(kCB_MT_second));
            target->addChild(first_img);
			
			CCMenuItem* second_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CollectionBook::menuAction));
			second_item->setTag(kCB_MT_cardBase + NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, level_number-1));
			second_item->setRotation(getContentRotate(kCB_MT_second));
			
			CCMenu* second_menu = CCMenu::createWithItem(second_item);
			second_menu->setPosition(first_img->getPosition());
			target->addChild(second_menu, 1, kCB_MT_second);
        }
        
        if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, level_number+1)) != 0)
        {
            CCSprite* third_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_thumbnail.png", stage_number, level_number+1)->getCString());
            third_img->setScale(mul_value);
			third_img->setRotation(getContentRotate(kCB_MT_third));
            third_img->setPosition(getContentPosition(kCB_MT_third));
            target->addChild(third_img);
            
//            if(mySD->isAnimationStage(stage_number))
//            {
//                CCSize ani_size = mySD->getAnimationCutSize(stage_number);
//                CCSprite* ani_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", stage_number, level_number+1)->getCString(), CCRectMake(0, 0, ani_size.width, ani_size.height));
//                ani_img->setPosition(mySD->getAnimationPosition(stage_number));
//                third_img->addChild(ani_img);
//            }
			
			CCMenuItem* third_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CollectionBook::menuAction));
			third_item->setTag(kCB_MT_cardBase + NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, level_number+1));
			third_item->setRotation(getContentRotate(kCB_MT_third));
			
			CCMenu* third_menu = CCMenu::createWithItem(third_item);
			third_menu->setPosition(third_img->getPosition());
			target->addChild(third_menu, 1, kCB_MT_third);
        }
    }
    else if(level_number == 3)
    {
        if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, level_number-2)) != 0)
        {
            CCSprite* first_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_thumbnail.png", stage_number, level_number-2)->getCString());
            first_img->setScale(mul_value);
			first_img->setRotation(getContentRotate(kCB_MT_second));
            first_img->setPosition(getContentPosition(kCB_MT_second));
            target->addChild(first_img);
			
			CCMenuItem* second_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CollectionBook::menuAction));
			second_item->setTag(kCB_MT_cardBase + NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, level_number-2));
			second_item->setRotation(getContentRotate(kCB_MT_second));
			
			CCMenu* second_menu = CCMenu::createWithItem(second_item);
			second_menu->setPosition(first_img->getPosition());
			target->addChild(second_menu, 1, kCB_MT_second);
        }
        
        if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, level_number-1)) != 0)
        {
            CCSprite* second_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_thumbnail.png", stage_number, level_number-1)->getCString());
            second_img->setScale(mul_value);
			second_img->setRotation(getContentRotate(kCB_MT_third));
            second_img->setPosition(getContentPosition(kCB_MT_third));
            target->addChild(second_img);
			
			CCMenuItem* third_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CollectionBook::menuAction));
			third_item->setTag(kCB_MT_cardBase + NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, level_number-1));
			third_item->setRotation(getContentRotate(kCB_MT_third));
			
			CCMenu* third_menu = CCMenu::createWithItem(third_item);
			third_menu->setPosition(second_img->getPosition());
			target->addChild(third_menu, 1, kCB_MT_third);
        }
    }
	
	CCLabelTTF* r_stage_label = CCLabelTTF::create(CCString::createWithFormat("STAGE %d-%d", stage_number, level_number)->getCString(), mySGD->getFont().c_str(), 8);
	r_stage_label->setPosition(ccp(180, 287));
	r_stage_label->setColor(ccBLACK);
	r_stage_label->setHorizontalAlignment(kCCTextAlignmentCenter);
	r_stage_label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	r_stage_label->setAnchorPoint(ccp(0.5,0.5));
	target->addChild(r_stage_label);
}

void CollectionBook::setLeftPage(CCNode *target, int card_number)
{
	CCSprite* r_card_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", NSDS_GI(kSDS_CI_int1_stage_i, card_number), NSDS_GI(kSDS_CI_int1_grade_i, card_number))->getCString());
	r_card_img->setScale(0.57);
	r_card_img->setPosition(ccp(129,161));
	target->addChild(r_card_img);
	
	int stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
	int level_number = NSDS_GI(kSDS_CI_int1_grade_i, card_number);
	
	if(level_number == 3 && mySD->isAnimationStage(stage_number))
	{
//		CollectionAnimation* t_ca = CollectionAnimation::create(card_number);
//		r_card_img->addChild(t_ca);
	}
	
	CCSprite* left_top_paper = CCSprite::create("diary_paper.png");
	left_top_paper->setPosition(ccp(46,277));
	target->addChild(left_top_paper);
	
	CCSprite* right_bottom_paper = CCSprite::create("diary_paper.png");
	right_bottom_paper->setRotation(180);
	right_bottom_paper->setPosition(ccp(213,44));
	target->addChild(right_bottom_paper);
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
    
	input_text = CCEditBox::create(CCSizeMake(185, 50), CCScale9Sprite::create("diary_text_box.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
	input_text->setPosition(getContentPosition(kCB_MT_inputText));
	input_text->setPlaceHolder(input_data.c_str());
	input_text->setReturnType(kKeyboardReturnTypeDone);
	input_text->setFont(mySGD->getFont().c_str(), 12);
	input_text->setFontColor(ccBLACK);
	input_text->setDelegate(this);
    recent_right_img->addChild(input_text);
	
    if(input_data != "입력해주세요.")
        input_text->setText(input_data.c_str());
	
	if(is_enable_pageturn)
	{
		CCSprite* n_pre = CCSprite::create("diary_left.png");
		CCSprite* s_pre = CCSprite::create("diary_left.png");
		s_pre->setColor(ccGRAY);
		
		CCMenuItem* r_pre_item = CCMenuItemSprite::create(n_pre, s_pre, this, menu_selector(CollectionBook::menuAction));
		r_pre_item->setTag(kCB_MT_pre);
		
		CCMenu* r_pre_menu = CCMenu::createWithItem(r_pre_item);
		r_pre_menu->setPosition(getContentPosition(kCB_MT_pre));
		recent_right_img->addChild(r_pre_menu, 1, kCB_MT_pre);
		
		CCSprite* n_next = CCSprite::create("diary_right.png");
		CCSprite* s_next = CCSprite::create("diary_right.png");
		s_next->setColor(ccGRAY);
		
		CCMenuItem* r_next_item = CCMenuItemSprite::create(n_next, s_next, this, menu_selector(CollectionBook::menuAction));
		r_next_item->setTag(kCB_MT_next);
		
		CCMenu* r_next_menu = CCMenu::createWithItem(r_next_item);
		r_next_menu->setPosition(getContentPosition(kCB_MT_next));
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
		
		CCSprite* a_n_pre = CCSprite::create("diary_left.png");
		CCSprite* a_s_pre = CCSprite::create("diary_left.png");
		a_s_pre->setColor(ccGRAY);
		
		CCMenuItem* a_pre_item = CCMenuItemSprite::create(a_n_pre, a_s_pre, this, menu_selector(CollectionBook::menuAction));
		a_pre_item->setTag(kCB_MT_pre);
		
		CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
		a_pre_menu->setPosition(getContentPosition(kCB_MT_pre));
		a_pre_menu->setEnabled(false);
		after_right_img->addChild(a_pre_menu, 1, kCB_MT_pre);
		
		CCSprite* a_n_next = CCSprite::create("diary_right.png");
		CCSprite* a_s_next = CCSprite::create("diary_right.png");
		a_s_next->setColor(ccGRAY);
		
		CCMenuItem* a_next_item = CCMenuItemSprite::create(a_n_next, a_s_next, this, menu_selector(CollectionBook::menuAction));
		a_next_item->setTag(kCB_MT_next);
		
		CCMenu* a_next_menu = CCMenu::createWithItem(a_next_item);
		a_next_menu->setPosition(getContentPosition(kCB_MT_next));
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

void CollectionBook::editBoxEditingDidBegin(CCEditBox* editBox)
{
	CCLog("edit begin");
}
void CollectionBook::editBoxEditingDidEnd(CCEditBox* editBox)
{
	CCLog("edit end");
}
void CollectionBook::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
	CCLog("edit changed : %s", text.c_str());
}
void CollectionBook::editBoxReturn(CCEditBox* editBox)
{
	CCLog("edit return");
}

bool CollectionBook::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCTouch* touch = pTouch;
	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
	location = ccpSub(location, myDSH->ui_touch_convert);
	
	if(!is_touch_enable)
		return true;
	
	begin_point = location;
	
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
	
//	if(was_open_text)	return;
	
	CCTouch* touch = pTouch;
	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
	location = ccpSub(location, myDSH->ui_touch_convert);
	
	float x_distance = begin_point.x - location.x;
	
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
    
	input_text = CCEditBox::create(CCSizeMake(185, 50), CCScale9Sprite::create("diary_text_box.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
	input_text->setPosition(getContentPosition(kCB_MT_inputText));
	input_text->setPlaceHolder(input_data.c_str());
	input_text->setReturnType(kKeyboardReturnTypeDone);
	input_text->setFont(mySGD->getFont().c_str(), 12);
	input_text->setFontColor(ccBLACK);
	input_text->setDelegate(this);
	recent_right_img->addChild(input_text);
    
    if(input_data != "입력해주세요.")
        input_text->setText(input_data.c_str());
	
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
	
	
	CCSprite* a_n_pre = CCSprite::create("diary_left.png");
	CCSprite* a_s_pre = CCSprite::create("diary_left.png");
	a_s_pre->setColor(ccGRAY);
	
	CCMenuItem* a_pre_item = CCMenuItemSprite::create(a_n_pre, a_s_pre, this, menu_selector(CollectionBook::menuAction));
	a_pre_item->setTag(kCB_MT_pre);
	
	CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
	a_pre_menu->setPosition(getContentPosition(kCB_MT_pre));
	a_pre_menu->setEnabled(false);
	after_right_img->addChild(a_pre_menu, 1, kCB_MT_pre);
	
	CCSprite* a_n_next = CCSprite::create("diary_right.png");
	CCSprite* a_s_next = CCSprite::create("diary_right.png");
	a_s_next->setColor(ccGRAY);
	
	CCMenuItem* a_next_item = CCMenuItemSprite::create(a_n_next, a_s_next, this, menu_selector(CollectionBook::menuAction));
	a_next_item->setTag(kCB_MT_next);
	
	CCMenu* a_next_menu = CCMenu::createWithItem(a_next_item);
	a_next_menu->setPosition(getContentPosition(kCB_MT_next));
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
    
	input_text = CCEditBox::create(CCSizeMake(185, 50), CCScale9Sprite::create("diary_text_box.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
	input_text->setPosition(getContentPosition(kCB_MT_inputText));
	input_text->setPlaceHolder(input_data.c_str());
	input_text->setReturnType(kKeyboardReturnTypeDone);
	input_text->setFont(mySGD->getFont().c_str(), 12);
	input_text->setFontColor(ccBLACK);
	input_text->setDelegate(this);
	covered_right_img->addChild(input_text);
    
    if(input_data != "입력해주세요.")
        input_text->setText(input_data.c_str());
	
	((CCMenu*)covered_right_img->getChildByTag(kCB_MT_close))->setEnabled(false);
	((CCMenu*)covered_right_img->getChildByTag(kCB_MT_zoom))->setEnabled(false);
	
	if(covered_right_img->getChildByTag(kCB_MT_second))
		((CCMenu*)covered_right_img->getChildByTag(kCB_MT_second))->setEnabled(false);
	if(covered_right_img->getChildByTag(kCB_MT_third))
		((CCMenu*)covered_right_img->getChildByTag(kCB_MT_third))->setEnabled(false);
	
	CCSprite* a_n_pre = CCSprite::create("diary_left.png");
	CCSprite* a_s_pre = CCSprite::create("diary_left.png");
	a_s_pre->setColor(ccGRAY);
	
	CCMenuItem* a_pre_item = CCMenuItemSprite::create(a_n_pre, a_s_pre, this, menu_selector(CollectionBook::menuAction));
	a_pre_item->setTag(kCB_MT_pre);
	
	CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
	a_pre_menu->setPosition(getContentPosition(kCB_MT_pre));
	a_pre_menu->setEnabled(false);
	covered_right_img->addChild(a_pre_menu, 1, kCB_MT_pre);
	
	CCSprite* a_n_next = CCSprite::create("diary_right.png");
	CCSprite* a_s_next = CCSprite::create("diary_right.png");
	a_s_next->setColor(ccGRAY);
	
	CCMenuItem* a_next_item = CCMenuItemSprite::create(a_n_next, a_s_next, this, menu_selector(CollectionBook::menuAction));
	a_next_item->setTag(kCB_MT_next);
	
	CCMenu* a_next_menu = CCMenu::createWithItem(a_next_item);
	a_next_menu->setPosition(getContentPosition(kCB_MT_next));
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

//CollectionAnimation* CollectionAnimation::create( int t_card_number )
//{
//	CollectionAnimation* t_ca = new CollectionAnimation();
//	t_ca->setAnimation(t_card_number);
//	t_ca->initWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_animation.png", t_ca->stage_number, t_ca->level_number)->getCString()), CCRectMake(0, 0, t_ca->ani_size.width, t_ca->ani_size.height));
//	t_ca->setPosition(mySD->getAnimationPosition(t_ca->stage_number));
//	t_ca->autorelease();
//	t_ca->startAnimation();
//	return t_ca;
//}

//void CollectionAnimation::setAnimation( int t_card_number )
//{
//	stage_number = NSDS_GI(kSDS_CI_int1_stage_i, t_card_number);
//	level_number = NSDS_GI(kSDS_CI_int1_grade_i, t_card_number);
//	ani_size = mySD->getAnimationCutSize(stage_number);
//
//	mySD->setAnimationLoop(stage_number);
//	loop_length = mySD->getAnimationLoopLength(stage_number);
//
//	for(int i=0;i<loop_length;i++)
//		animation_frame.push_back(mySD->getAnimationLoopPoint(i));
//}
//
//void CollectionAnimation::startAnimation()
//{
//	startStageAnimation();
//}
//
//void CollectionAnimation::startStageAnimation()
//{
//	ing_animation_frame = 0;
//
//	int random_value = rand()%16;
//
//	if(random_value >= 2 && random_value <= 4)
//		random_value = 7;
//	else if(random_value >= 5 && random_value <= 10)
//		random_value = 10;
//	else if(random_value >= 11 && random_value <= 13)
//		random_value = 13;
//	else if(random_value == 14 || random_value == 15)
//		random_value = 16;
//	else
//		random_value++;
//
//	CCDelayTime* t_delay = CCDelayTime::create(random_value*0.1f);
//	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(CollectionAnimation::frameAnimation));
//	CCAction* t_seq = CCSequence::create(t_delay, t_call, NULL);
//	runAction(t_seq);
//}
//
//void CollectionAnimation::frameAnimation()
//{
//	int loop_point = animation_frame[ing_animation_frame];
//	setTextureRect(CCRectMake(loop_point*ani_size.width, 0, ani_size.width, ani_size.height));
//
//	ing_animation_frame++;
//	if(ing_animation_frame >= loop_length)
//		startStageAnimation();
//	else
//	{
//		CCDelayTime* t_delay = CCDelayTime::create(0.1f);
//		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(CollectionAnimation::frameAnimation));
//		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
//		runAction(t_seq);
//	}
//}
