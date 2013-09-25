//
//  ClearScene.cpp
//  galsprototype
//
//  Created by 사원3 on 13. 8. 23..
//
//

#include "ClearScene.h"
//#include "StartingScene.h"
#include "WorldMapScene.h"
#include "ScreenSide.h"
#include "CardFullPopup.h"

typedef enum tMenuTagClearScene{
	kMT_CS_ok = 1
}MenuTagClearScene;

typedef enum tZorderClearScene{
	kZ_CS_back = 1,
	kZ_CS_img,
	kZ_CS_menu,
	kZ_CS_popup
}ZorderClearScene;

CCScene* ClearScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    ClearScene *layer = ClearScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ClearScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	setKeypadEnabled(true);
	
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
		
		int durability = myDSH->getIntegerForKey(kDSH_Key_haveCardDurability_int1, found_number) + 1;
		if(durability > 0)
			myDSH->setIntegerForKey(kDSH_Key_haveCardDurability_int1, found_number, durability);
		else
		{
			for(int i=loop_cnt;i>found_number;i--)
			{
				myDSH->setIntegerForKey(kDSH_Key_haveCardDurability_int1, i-1, myDSH->getIntegerForKey(kDSH_Key_haveCardDurability_int1, i));
				myDSH->setIntegerForKey(kDSH_Key_haveCardNumber_int1, i-1, myDSH->getIntegerForKey(kDSH_Key_haveCardNumber_int1, i));
			}
			myDSH->setIntegerForKey(kDSH_Key_haveCardCnt, loop_cnt-1);
			myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
		}
	}
    
	CCSprite* clear_back = CCSprite::create("ending_back.png");
	clear_back->setPosition(ccp(240,160));
	addChild(clear_back, kZ_CS_back);
	
	CCSprite* title = CCSprite::create("ending_clear.png");
	title->setPosition(ccp(360,280));
	addChild(title, kZ_CS_img);
	
	int stage_number = mySD->getSilType();
	int take_level;
	if(mySGD->is_exchanged && mySGD->is_showtime)
		take_level = 3;
	else if(mySGD->is_exchanged || mySGD->is_showtime)
		take_level = 2;
	else
		take_level = 1;
	
	CCSprite* take_card = CCSprite::create(CCString::createWithFormat("stage%d_level%d_visible.png", stage_number, take_level)->getCString());
	take_card->setScale(0.65);
	take_card->setPosition(ccp(130,160));
	addChild(take_card, kZ_CS_img);
	
	if(take_level == 3 && mySD->isAnimationStage())
	{
		CCSize ani_size = mySD->getAnimationCutSize();
		CCSprite* take_ani = CCSprite::create(CCString::createWithFormat("stage%d_level%d_animation.png", stage_number, take_level)->getCString(), CCRectMake(0, 0, ani_size.width, ani_size.height));
		take_ani->setPosition(mySD->getAnimationPosition());
		take_card->addChild(take_ani);
	}
	
	
	for(int i=0;i<take_level;i++)
	{
		CCSprite* star = CCSprite::create("ending_star.png");
		star->setPosition(ccp(308+i*46,228));
		addChild(star, kZ_CS_img);
	}
	
	score_label = CCLabelBMFont::create("0", "bb_white_font.fnt");
	score_label->setAnchorPoint(ccp(1.0,0.5));
	score_label->setPosition(ccp(450,171));
	score_label->setAlignment(kCCTextAlignmentRight);
	addChild(score_label, kZ_CS_img);
	
	
	percentage_label = CCLabelBMFont::create("0.0", "bb_white_font.fnt");
	percentage_label->setAnchorPoint(ccp(1.0,0.5));
	percentage_label->setPosition(ccp(450,138));
	percentage_label->setAlignment(kCCTextAlignmentRight);
	addChild(percentage_label, kZ_CS_img);
	
	
	time_label = CCLabelBMFont::create("0", "bb_white_font.fnt");
	time_label->setAnchorPoint(ccp(1.0,0.5));
	time_label->setPosition(ccp(450,105));
	time_label->setAlignment(kCCTextAlignmentRight);
	addChild(time_label, kZ_CS_img);
	
	
	CCSprite* n_ok = CCSprite::create("ending_ok.png");
	CCSprite* s_ok = CCSprite::create("ending_ok.png");
	s_ok->setColor(ccGRAY);
	
	CCMenuItem* ok_item = CCMenuItemSprite::create(n_ok, s_ok, this, menu_selector(ClearScene::menuAction));
	ok_item->setTag(kMT_CS_ok);
	
	CCMenu* ok_menu = CCMenu::createWithItem(ok_item);
	ok_menu->setPosition(ccp(360,50));
	addChild(ok_menu, kZ_CS_menu);
	
	is_menu_enable = true;
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
    return true;
}

void ClearScene::onEnter()
{
	CCLayer::onEnter();
	
	startCalcAnimation();
}

void ClearScene::startCalcAnimation()
{
	AudioEngine::sharedInstance()->playEffect("sound_calc.mp3", true);
	startScoreAnimation();
}

void ClearScene::startScoreAnimation()
{
	keep_score = mySGD->getScore();
	decrease_score = keep_score;
	increase_score = 0.f;
	schedule(schedule_selector(ClearScene::scoreAnimation));
}

void ClearScene::scoreAnimation(float dt)
{
	if(decrease_score > 0)
	{
		int decreaseUnit = keep_score / 1.f * dt;
		
		if(decrease_score < decreaseUnit)
		{
			increase_score += decrease_score;
			decrease_score = 0;
		}
		else {
			if(decreaseUnit <= 0)
			{
				increase_score += decrease_score;
				decrease_score = 0;
			}
			else {
				decrease_score -= decreaseUnit;
				increase_score += decreaseUnit;
			}
		}
		score_label->setString(CCString::createWithFormat("%.0f",increase_score)->getCString());
	}
	else
		stopScoreAnimation();
}

void ClearScene::stopScoreAnimation()
{
	unschedule(schedule_selector(ClearScene::scoreAnimation));
	score_label->setString(CCString::createWithFormat("%.0f", mySGD->getScore())->getCString());
	startPercentageAnimation();
}

void ClearScene::startPercentageAnimation()
{
	keep_percentage = mySGD->getPercentage()*100.f;
	decrease_percentage = keep_percentage;
	increase_percentage = 0.f;
	schedule(schedule_selector(ClearScene::percentageAnimation));
}

void ClearScene::percentageAnimation(float dt)
{
	if(decrease_percentage > 0)
	{
		int decreaseUnit = keep_percentage / 0.5f * dt;
		
		if(decrease_percentage < decreaseUnit)
		{
			increase_percentage += decrease_percentage;
			decrease_percentage = 0;
		}
		else {
			if(decreaseUnit <= 0)
			{
				increase_percentage	+= decrease_percentage;
				decrease_percentage = 0;
			}
			else {
				decrease_percentage -= decreaseUnit;
				increase_percentage	+= decreaseUnit;
			}
		}
		percentage_label->setString(CCString::createWithFormat("%.1f",increase_percentage)->getCString());
	}
	else
		stopPercentageAnimation();
}

void ClearScene::stopPercentageAnimation()
{
	unschedule(schedule_selector(ClearScene::percentageAnimation));
	percentage_label->setString(CCString::createWithFormat("%.1f", mySGD->getPercentage()*100.f)->getCString());
	startTimeAnimation();
}

void ClearScene::startTimeAnimation()
{
	keep_time = mySGD->getGameTime();
	decrease_time = keep_time;
	increase_time = 0.f;
	schedule(schedule_selector(ClearScene::timeAnimation));
}

void ClearScene::timeAnimation(float dt)
{
	if(decrease_time > 0)
	{
		int decreaseUnit = keep_time / 0.5f * dt;
		
		if(decrease_time < decreaseUnit)
		{
			increase_time += decrease_time;
			decrease_time = 0;
		}
		else {
			if(decreaseUnit <= 0)
			{
				increase_time += decrease_time;
				decrease_time = 0;
			}
			else {
				decrease_time -= decreaseUnit;
				increase_time	+= decreaseUnit;
			}
		}
		time_label->setString(CCString::createWithFormat("%.0f",increase_time)->getCString());
	}
	else
		stopTimeAnimation();
}

void ClearScene::stopTimeAnimation()
{
	unschedule(schedule_selector(ClearScene::timeAnimation));
	time_label->setString(CCString::createWithFormat("%d", mySGD->getGameTime())->getCString());
	AudioEngine::sharedInstance()->stopAllEffects();
}

void ClearScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	is_menu_enable = false;
	AudioEngine::sharedInstance()->stopAllEffects();
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kMT_CS_ok)
	{
		int take_level;
		if(mySGD->is_exchanged && mySGD->is_showtime)		take_level = 3;
		else if(mySGD->is_exchanged || mySGD->is_showtime)	take_level = 2;
		else												take_level = 1;
		
		if(!myDSH->getBoolForKey(kDSH_Key_hasGottenCard_int1, mySD->getSilType()*10+take_level-1))
		{
			myDSH->setBoolForKey(kDSH_Key_hasGottenCard_int1, mySD->getSilType()*10+take_level-1, true);
			mySGD->addHasGottenCardNumber(mySD->getSilType()*10+take_level-1);
		}
		
		bool is_have_kind_card = false;
		int loop_cnt = myDSH->getIntegerForKey(kDSH_Key_haveCardCnt);
		for(int i=1;i<loop_cnt;i++)
		{
			int card_number = myDSH->getIntegerForKey(kDSH_Key_haveCardNumber_int1, i);
			int card_stage = card_number/10;
			int card_level = card_number%10 + 1;
			if(mySD->getSilType() == card_stage)
			{
				if(card_level > take_level)
				{
					int durability = myDSH->getIntegerForKey(kDSH_Key_haveCardDurability_int1, i);
					int max_durability = mySD->getCardDurability(mySD->getSilType(), card_level);
					durability += card_level-take_level;
					if(durability > max_durability)
						durability = max_durability;
					myDSH->setIntegerForKey(kDSH_Key_haveCardDurability_int1, i, durability);
				}
				else if(card_level == take_level)
				{
					myDSH->setIntegerForKey(kDSH_Key_haveCardDurability_int1, i, mySD->getCardDurability(mySD->getSilType(), take_level));
				}
				else // card_level < take_level
				{
					myDSH->setIntegerForKey(kDSH_Key_haveCardNumber_int1, i, mySD->getSilType()*10+take_level-1);
					myDSH->setIntegerForKey(kDSH_Key_haveCardDurability_int1, i, mySD->getCardDurability(mySD->getSilType(), take_level));
					
					int selected_card_stage = myDSH->getIntegerForKey(kDSH_Key_selectedCard) / 10;
					if(mySD->getSilType() == selected_card_stage)
					{
						myDSH->setIntegerForKey(kDSH_Key_selectedCard, mySD->getSilType()*10+take_level-1);
					}
				}
				is_have_kind_card = true;
				break;
			}
		}
		
		if(!is_have_kind_card)
		{
			if(loop_cnt >= 5) // card full
			{
				CardFullPopup* t_cfp = CardFullPopup::create(this, callfunc_selector(ClearScene::realEnd));
				addChild(t_cfp, kZ_CS_popup);
			}
			else
			{
				myDSH->setIntegerForKey(kDSH_Key_haveCardCnt, loop_cnt+1);
				myDSH->setIntegerForKey(kDSH_Key_haveCardNumber_int1, loop_cnt+1, mySD->getSilType()*10+take_level-1);
				myDSH->setIntegerForKey(kDSH_Key_haveCardDurability_int1, loop_cnt+1, mySD->getCardDurability(mySD->getSilType(), take_level));
				realEnd();
			}
		}
		else
			realEnd();
	}
}

void ClearScene::realEnd()
{
	CCDirector::sharedDirector()->replaceScene(WorldMapScene::scene());
}

void ClearScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void ClearScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(ClearScene::alertAction));
}