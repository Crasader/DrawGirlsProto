//
//  PlayUI.h
//  DrawingJack
//
//  Created by 사원3 on 12. 12. 3..
//
//

#ifndef DrawingJack_PlayUI_h
#define DrawingJack_PlayUI_h

#include "cocos2d.h"
#include "SelectedMapData.h"
#include "StarGoldData.h"
#include "EnumDefine.h"
#include "DataStorageHub.h"
#include "AudioEngine.h"
#include "AlertEngine.h"
#include "MyLocalization.h"
#include "PausePopupLayer.h"
#include "CountingBMLabel.h"
#include "BossLifeGage.h"
#include "AreaGage.h"
#include "StartingScene.h"
#include "ContinuePopup.h"
//#include "WorldMapScene.h"
#include "PuzzleMapScene.h"
#include "ConditionPopup.h"
#include "ServerDataSave.h"

using namespace cocos2d;
using namespace std;

#define JM_CONDITION	0.05

#define MY_GRAVITY	-0.5

class GoldLabel : public CCLabelBMFont
{
public:
	static GoldLabel* create()
	{
		GoldLabel* t_gl = new GoldLabel();
		t_gl->myInit();
		t_gl->autorelease();
		return t_gl;
	}
	
	virtual void setString(const char* after_gold)
	{
		stopAllActions();
		if(is_incresing)
			stopIncreasing();
		startIncreasing();
	}
	
private:
	bool is_incresing;
	float keep_gold;
	float base_gold;
	float decrease_gold;
	float increase_gold;
	string keep_gold_string;
	
	void startIncreasing()
	{
		is_incresing = true;
		CCScaleTo* t_scale1 = CCScaleTo::create(0.2f, 1.5f);
		CCScaleTo* t_scale2 = CCScaleTo::create(0.5f, 1.f);
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale1, t_scale2);
		runAction(t_seq);
		
		int stageGold = mySGD->getStageGold();
		
		keep_gold_string = CCString::createWithFormat("%d", stageGold)->getCString();
		base_gold = atof(getString()); // 원래 가지고 있던 골드
		keep_gold = stageGold - base_gold; // 이번에 얻은 골드
		decrease_gold = keep_gold;
		increase_gold = 0.f;
		schedule(schedule_selector(GoldLabel::increasing));
	}
	
	void increasing(float dt)
	{
		if(decrease_gold > 0)
		{
			float decreaseUnit = keep_gold / 1.f * dt;
			
			if(decrease_gold < decreaseUnit)
			{
				increase_gold += decrease_gold;
				decrease_gold = 0;
			}
			else {
				if(decreaseUnit <= 0.01)
				{
					increase_gold += decrease_gold;
					decrease_gold = 0;
				}
				else {
					decrease_gold -= decreaseUnit;
					increase_gold += decreaseUnit;
				}
			}
			CCLabelBMFont::setString(CCString::createWithFormat("%.0f",base_gold+increase_gold)->getCString());
		}
		else
			stopIncreasing();
	}
	
	void stopIncreasing()
	{
		unschedule(schedule_selector(GoldLabel::increasing));
		is_incresing = false;
		CCLabelBMFont::setString(keep_gold_string.c_str());
	}
	
	void myInit()
	{
		is_incresing = false;
		CCLabelBMFont::initWithString(CCString::createWithFormat("%d", mySGD->getStageGold())->getCString(), "etc_font.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentRight, CCPointZero);
		stopIncreasing();
		setAnchorPoint(ccp(1,0.5));
		setPosition(ccp(112,464));
		mySGD->setGoldLabel(this);
	}
};

class MyGold : public CCSprite
{
public:
	static MyGold* create()
	{
		MyGold* t_mg = new MyGold();
		t_mg->myInit();
		t_mg->autorelease();
		return t_mg;
	}
	
	void startMoving()
	{
		getParent()->reorderChild(this, z_order);
		ing_frame = 0;
		r_dy = i_dy;
		schedule(schedule_selector(MyGold::moving));
	}
	
private:
	float i_dy;
	float r_dy;
	
	CCPoint t_dv;
	float dscale;
	int z_order;
	
	CCPoint init_position;
	int ing_frame;
	CCPoint after_position;
	
	void moving()
	{
		ing_frame++;
		
		setScale(getScale()*dscale);
		setOpacity(getOpacity() - 4);
		after_position = ccpAdd(getPosition(), ccp(0, r_dy));
		r_dy += MY_GRAVITY;
		
		if(after_position.y < init_position.y)
		{
			r_dy = i_dy/2.f;
			i_dy /= 2.f;
			
			after_position.y = init_position.y + r_dy;
		}
		
		after_position = ccpAdd(after_position, t_dv);
		init_position = ccpAdd(init_position, t_dv);
		
		setPosition(after_position);
		
		if(ing_frame > 60)
		{
			unschedule(schedule_selector(MyGold::moving));
			removeFromParentAndCleanup(true);
		}
	}
	
	void myInit()
	{
		initWithFile("gold.png");
		init_position = CCPointZero;
		setPosition(init_position);
		
		i_dy = rand()%3+6;
		
		float t_angle = rand()%360-180.f;
		t_dv.x = 1;
		t_dv.y = tanf(t_angle/180.f*M_PI);
		
		if(t_angle >= 90.f || t_angle < -90.f)
			t_dv = ccpMult(t_dv, -1.f);
		
		float div_value = sqrtf(powf(t_dv.x, 2.f) + powf(t_dv.y, 2.f));
		t_dv = ccpMult(t_dv, 1.f/div_value);
		
		float t_distance = rand()%40 + 40.f;
		t_dv = ccpMult(t_dv, t_distance/60.f);
		
		dscale = 1.f - t_dv.y/1.3f*0.01f; // -0.1 ~ 0.1
		
		z_order = fabs(t_angle-90);
		if(z_order > 180)	z_order = 360-z_order;
	}
};

class GetGold : public CCNode
{
public:
	static GetGold* create(CCPoint t_sp, int t_duration_frame)
	{
		GetGold* t_gg = new GetGold();
		t_gg->myInit(t_sp, t_duration_frame);
		t_gg->autorelease();
		return t_gg;
	}
	
	void startMyAction()
	{
		ing_frame = 0;
		schedule(schedule_selector(GetGold::myAction));
	}
	
private:
	CCSpriteBatchNode* batch_node;
	int duration_frame;
	int ing_frame;
	int create_frame;
	
	void myAction()
	{
		ing_frame++;
		
		if(ing_frame%create_frame == 0)
		{
			MyGold* t_mg = MyGold::create();
			batch_node->addChild(t_mg);
			t_mg->startMoving();
		}
		
		if(ing_frame%5 == 0)
		{
			AudioEngine::sharedInstance()->playEffect("sound_get_coin.mp3", false);
		}
		
		if(ing_frame >= duration_frame)
		{
			unschedule(schedule_selector(GetGold::myAction));
			schedule(schedule_selector(GetGold::selfRemove));
		}
	}
	
	void selfRemove()
	{
		if(batch_node->getChildrenCount() <= 0)
		{
			unschedule(schedule_selector(GetGold::selfRemove));
			removeFromParentAndCleanup(true);
		}
	}
	
	void myInit(CCPoint t_sp, int t_duration_frame)
	{
		AudioEngine::sharedInstance()->playEffect("sound_get_coin.mp3", false);
		duration_frame = t_duration_frame;
		mySGD->setGold(mySGD->getGold() + duration_frame);
		
		create_frame = duration_frame/60 + 1;
		
		batch_node = CCSpriteBatchNode::create("gold.png");
		batch_node->setPosition(t_sp);
		addChild(batch_node);
	}
};

enum ZorderGetPercentage{
	kZorderGetPercentage_backImg = 1,
	kZorderGetPercentage_label
};

class GetPercentage : public CCNode
{
public:
	static GetPercentage* create(float t_gp, bool is_item)
	{
		GetPercentage* t_g = new GetPercentage();
		t_g->myInit(t_gp, is_item);
		t_g->autorelease();
		return t_g;
	}
	
private:
	CCLabelBMFont* my_label;
	CCSprite* backImg;
	
	void startFadeOut()
	{
		CCFadeOut* t_fadeout1 = CCFadeOut::create(1.f);
		
		backImg->runAction(t_fadeout1);
		
		CCFadeOut* t_fadeout2 = CCFadeOut::create(1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GetPercentage::selfRemove));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fadeout2, t_call);
		
		my_label->runAction(t_seq);
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void myInit(float t_gp, bool is_item)
	{
		my_label = CCLabelBMFont::create(CCString::createWithFormat("%.1f", t_gp)->getCString(), "font_get_percentage.fnt");
		my_label->setAlignment(kCCTextAlignmentRight);
		addChild(my_label, kZorderGetPercentage_label);
		
		if(is_item)
		{
			CCSprite* t_texture = CCSprite::create("get_percentage.png");
			backImg = CCSprite::createWithTexture(t_texture->getTexture(), CCRectMake(0, 24, 52.5, 24));
			addChild(backImg, kZorderGetPercentage_backImg);
			
			CCAnimation* t_animation = CCAnimation::create();
			t_animation->setDelayPerUnit(0.2);
			for(int i=1;i<=3;i++)
				t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(0, i*24, 52.5, 24));
			CCAnimate* t_animate = CCAnimate::create(t_animation);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GetPercentage::startFadeOut));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_animate, t_call);
			
			backImg->runAction(t_seq);
		}
		else
		{
			backImg = CCSprite::create("get_percentage.png", CCRectMake(0, 0, 52.5, 24));
			addChild(backImg, kZorderGetPercentage_backImg);
			startFadeOut();
		}
	}
};

class Warning : public CCSprite
{
public:
	static Warning* create(int t1)
	{
		Warning* t_w = new Warning();
		t_w->myInit(t1);
		t_w->autorelease();
		return t_w;
	}
	
	void startAction()
	{
		CCMoveTo* t_move1 = CCMoveTo::create(0.4f, ccp(240,myDSH->ui_center_y));
		CCHide* t_hide = CCHide::create();
		CCDelayTime* t_delay1 = CCDelayTime::create(0.05f);
		CCShow* t_show = CCShow::create();
		CCDelayTime* t_delay2 = CCDelayTime::create(0.1f);
		CCRepeat* t_repeat = CCRepeat::create(CCSequence::create(t_hide, t_delay1, t_show, t_delay2, NULL), 4);
		CCMoveTo* t_move2 = CCMoveTo::create(0.4f, ccp(-160,myDSH->ui_center_y));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Warning::selfRemove));
		
		runAction(CCSequence::create(t_move1, t_repeat, t_move2, t_call, NULL));
	}
	
private:
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void myInit(int t1)
	{
		initWithFile("attack_warning.png");
		
		if(t1 == 1)
		{
			setColor(ccWHITE);
		}
		else if(t1 == 2)
		{
			setColor(ccRED);
		}
		setPosition(ccp(640,myDSH->ui_center_y));
	}
};

class TakeCoin : public CCSprite
{
public:
	static TakeCoin* create()
	{
		TakeCoin* t_w = new TakeCoin();
		t_w->myInit();
		t_w->autorelease();
		return t_w;
	}
	
	void startAction()
	{
		CCMoveTo* t_move1 = CCMoveTo::create(0.4f, ccp(240,myDSH->ui_center_y));
		CCHide* t_hide = CCHide::create();
		CCDelayTime* t_delay1 = CCDelayTime::create(0.05f);
		CCShow* t_show = CCShow::create();
		CCDelayTime* t_delay2 = CCDelayTime::create(0.1f);
		CCRepeat* t_repeat = CCRepeat::create(CCSequence::create(t_hide, t_delay1, t_show, t_delay2, NULL), 4);
		CCMoveTo* t_move2 = CCMoveTo::create(0.4f, ccp(-160,myDSH->ui_center_y));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(TakeCoin::selfRemove));
		
		runAction(CCSequence::create(t_move1, t_repeat, t_move2, t_call, NULL));
	}
	
private:
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void myInit()
	{
		initWithFile("show_take_change.png");
		
		setPosition(ccp(640,myDSH->ui_center_y));
	}
};

class ChangeCard : public CCSprite
{
public:
	static ChangeCard* create()
	{
		ChangeCard* t_w = new ChangeCard();
		t_w->myInit();
		t_w->autorelease();
		return t_w;
	}
	
	void startAction()
	{
		CCMoveTo* t_move1 = CCMoveTo::create(0.4f, ccp(240,myDSH->ui_center_y));
		CCHide* t_hide = CCHide::create();
		CCDelayTime* t_delay1 = CCDelayTime::create(0.05f);
		CCShow* t_show = CCShow::create();
		CCDelayTime* t_delay2 = CCDelayTime::create(0.1f);
		CCRepeat* t_repeat = CCRepeat::create(CCSequence::create(t_hide, t_delay1, t_show, t_delay2, NULL), 4);
		CCMoveTo* t_move2 = CCMoveTo::create(0.4f, ccp(-160,myDSH->ui_center_y));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ChangeCard::selfRemove));
		
		runAction(CCSequence::create(t_move1, t_repeat, t_move2, t_call, NULL));
	}
	
private:
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void myInit()
	{
		initWithFile("card_change.png");
		
		setPosition(ccp(640,myDSH->ui_center_y));
	}
};

enum MenuTagUI{
	kMenuTagUI_home = 1,
	kMenuTagUI_condition
};

enum AlertTagUI{
	kAlertTagUI_home = 1
};

enum ChildTagInPlayUI{
	kCT_UI_clrCdtLabel = 100,
	kCT_UI_clrCdtIcon,
	kCT_UI_clrCdtPopup
};

#define t_tta	0xD9

class PlayUI : public CCNode
{
public:
	static PlayUI* create()
	{
		PlayUI* t_ui = new PlayUI();
		t_ui->myInit();
		t_ui->autorelease();
		return t_ui;
	}
	
	virtual ~PlayUI()
	{
		jack_array->removeAllObjects();
		jack_array->release();
		exchange_dic->removeAllObjects();
		exchange_dic->release();
	}
	
	void addScore(int t_score)
	{
		score_label->setString(CCString::createWithFormat("%d", atoi(score_label->getString()) + t_score)->getCString());
	}
	
	void decreasePercentage()
	{
		percentage_decrease_cnt++;
		if(percentage_decrease_cnt >= mySD->must_cnt/1000.f)
		{
			beforePercentage ^= t_tta;
			beforePercentage -= 1;
			percentageLabel->setString(CCString::createWithFormat("%.1f", beforePercentage/10.f)->getCString());
			m_areaGage->setPercentage(beforePercentage/1000.f);
			beforePercentage ^= t_tta;
			percentage_decrease_cnt = 0;
		}
	}
	
	float getScore()
	{
		return atoi(score_label->getString());
	}
	
	float getPercentage()
	{
		return (beforePercentage^t_tta)/1000.f;
	}
	
	void setPercentage(float t_p)
	{
		if(isFirst)
		{
			isFirst = false;
			beforePercentage = (int(t_p*1000))^t_tta;
		}
		else
		{
//			t_p = 0.99f;
			myGD->communication("CP_changeMaxSize", t_p);
			
			AudioEngine::sharedInstance()->playEffect("sound_jack_basic_missile_shoot.mp3", false);
			float t_beforePercentage = (beforePercentage^t_tta)/1000.f;
			
			int item_value = mySGD->getSmallAreaValue();
			
			if(clr_cdt_type == kCLEAR_bigArea && !is_cleared_cdt && t_p - t_beforePercentage >= clr_cdt_per-item_value/100.f)
				takeBigArea();
			
			if(t_p >= t_beforePercentage + JM_CONDITION)
			{
				int cmCnt = (t_p - t_beforePercentage)/JM_CONDITION;
				int missile_type = myDSH->getIntegerForKey(kDSH_Key_lastSelectedElement);
				
				IntPoint jackPoint = myGD->getJackPoint();
				CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize + 1, (jackPoint.y-1)*pixelSize + 1);
				
				myGD->communication("Main_goldGettingEffect", jackPosition, int((t_p - t_beforePercentage)/JM_CONDITION*myDSH->getGoldGetRate()));
				myGD->communication("Main_percentageGettingEffect", (t_p-t_beforePercentage)*100.f, true, jackPosition);
				
				int rmCnt = cmCnt/2 + 1;
				float damage_per = cmCnt*0.8f / rmCnt;
				
				myGD->communication("MP_createJackMissile", missile_type, rmCnt, damage_per);
			}
			
			if(!is_exchanged && !is_show_exchange_coin && !isGameover && t_p < clearPercentage)
			{
				if(t_p >= t_beforePercentage + 0.2f) // 0.2
				{
					is_show_exchange_coin = true;
					taked_coin_cnt = 0;
					myGD->communication("Main_showCoin");
					myGD->communication("Main_showTakeCoin");
				}
				else if(t_p >= t_beforePercentage + 0.15f)
				{
					if(rand()%10 < 7) // 70%
					{
						is_show_exchange_coin = true;
						taked_coin_cnt = 0;
						myGD->communication("Main_showCoin");
						myGD->communication("Main_showTakeCoin");
					}
				}
				else if(t_p >= t_beforePercentage + 0.1f)
				{
					if(rand()%2 == 0) // 50%
					{
						is_show_exchange_coin = true;
						taked_coin_cnt = 0;
						myGD->communication("Main_showCoin");
						myGD->communication("Main_showTakeCoin");
					}
				}
				else if(t_p >= t_beforePercentage + 0.08f)
				{
					if(rand()%20 < 7) // 35%
					{
						is_show_exchange_coin = true;
						taked_coin_cnt = 0;
						myGD->communication("Main_showCoin");
						myGD->communication("Main_showTakeCoin");
					}
				}
				else if(t_p >= t_beforePercentage + 0.06f)
				{
					if(rand()%5 == 0) // 20%
					{
						is_show_exchange_coin = true;
						taked_coin_cnt = 0;
						myGD->communication("Main_showCoin");
						myGD->communication("Main_showTakeCoin");
					}
				}
			}
			
			beforePercentage = (int(t_p*1000))^t_tta;
		}
		
		if(t_p > 0.5f && !is_show_exchange_coin && t_p < clearPercentage)
		{
			is_show_exchange_coin = true;
			taked_coin_cnt = 0;
			myGD->communication("Main_showCoin");
			myGD->communication("Main_showTakeCoin");
		}
		
		percentageLabel->setString(CCString::createWithFormat("%.1f", floorf(t_p*10000.f)/10000.f*100.f)->getCString());
		
		int item_value = mySGD->getWidePerfectValue();
		
		if(clr_cdt_type == kCLEAR_perfect && !isGameover && !is_cleared_cdt && atof(percentageLabel->getString()) >= (clr_cdt_per-item_value/200.f)*100.f && atof(percentageLabel->getString()) <= (clr_cdt_per+clr_cdt_range+item_value/200.f)*100.f)
			conditionClear();
		
		m_areaGage->setPercentage(t_p);
		percentage_decrease_cnt = 0;
		
		if(!isGameover && t_p > clearPercentage) // clear 80%
		{
			if(clr_cdt_type == kCLEAR_timeLimit)
			{
				if(playtime_limit - countingCnt >= ing_cdt_cnt)
					conditionClear();
				else
					conditionFail();
			}
			
			if(is_cleared_cdt)
			{
				myGD->communication("MP_bombCumber", myGD->getCommunicationNode("CP_getMainCumberPointer"));
				isGameover = true;
				myGD->setIsGameover(true);
				myGD->communication("CP_setGameover");
				stopCounting();
				myGD->communication("Main_allStopSchedule");
				myGD->communication("Main_startMoveToBossPosition");
				myGD->communication("CP_startDieAnimation");
				AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
				result_sprite = CCSprite::create("game_clear.png");
				result_sprite->setRotation(-25);
				result_sprite->setPosition(ccp(240,myDSH->ui_center_y));
				addChild(result_sprite);
				
				int star_cnt = 1;
				if(t_p >= clearPercentage + 0.1f || countingCnt <= 50)				star_cnt = 3;
				else if(t_p >= clearPercentage + 0.05f || countingCnt <= 100)		star_cnt = 2;
				
				mySGD->gameClear(star_cnt, atoi(score_label->getString()), (beforePercentage^t_tta)/1000.f, countingCnt);
				
				endGame();
			}
			else
			{
				conditionFail();
				
				mySGD->fail_code = kFC_missionfail;
				
				stopCounting();
				// timeover
				isGameover = true;
				myGD->communication("Main_allStopSchedule");
				AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
				result_sprite = CCSprite::create("game_fail.png");
				result_sprite->setRotation(-25);
				result_sprite->setPosition(ccp(240,myDSH->ui_center_y));
				addChild(result_sprite);
				endGame();
			}
		}
	}
	
	void conditionClear()
	{
		removeChildByTag(kCT_UI_clrCdtLabel);
		is_cleared_cdt = true;
		((CCMenu*)getChildByTag(kCT_UI_clrCdtIcon))->setEnabled(false);
		
		CCSprite* condition_clear = CCSprite::create("condition_clear.png");
		condition_clear->setPosition(getChildByTag(kCT_UI_clrCdtIcon)->getPosition());
		addChild(condition_clear);
	}
	
	void conditionFail()
	{
		((CCMenu*)getChildByTag(kCT_UI_clrCdtIcon))->setEnabled(false);
		
		CCSprite* condition_fail = CCSprite::create("condition_fail.png");
		condition_fail->setPosition(getChildByTag(kCT_UI_clrCdtIcon)->getPosition());
		addChild(condition_fail);
	}
	
	void takeExchangeCoin(int t_coin_number)
	{
		if(clr_cdt_type == kCLEAR_sequenceChange && !isGameover)
		{
			if(!mySGD->isUsingItem(kIC_randomChange) && t_coin_number != ing_cdt_cnt)
			{
				conditionFail();
				
				mySGD->fail_code = kFC_missionfail;
				
				stopCounting();
				// timeover
				isGameover = true;
				myGD->communication("Main_allStopSchedule");
				AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
				result_sprite = CCSprite::create("game_fail.png");
				result_sprite->setRotation(-25);
				result_sprite->setPosition(ccp(240,myDSH->ui_center_y));
				addChild(result_sprite);
				endGame();
				return;
			}
			else
			{
				ing_cdt_cnt++;
				
				if(mySGD->isUsingItem(kIC_randomChange))
				{
					((CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel))->setString(CCString::createWithFormat("%d/%d", ing_cdt_cnt-1, 6)->getCString());
				}
				else
				{
					removeChildByTag(kCT_UI_clrCdtLabel);
					if(ing_cdt_cnt <= 6)
					{
						CCSprite* clr_cdt_img = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", ing_cdt_cnt)->getCString());
						clr_cdt_img->setPosition(ccpAdd(getChildByTag(kCT_UI_clrCdtIcon)->getPosition(), ccp(0,-5)));
						addChild(clr_cdt_img, 0, kCT_UI_clrCdtLabel);
					}
				}
			}
		}
		
		taked_coin_cnt++;
		
		CCSprite* t_coin_spr = (CCSprite*)exchange_dic->objectForKey(t_coin_number);
		t_coin_spr->removeFromParentAndCleanup(true);
		exchange_dic->removeObjectForKey(t_coin_number);
		
		CCSprite* new_coin_spr = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", t_coin_number)->getCString());
		if(myGD->gamescreen_type == kGT_leftUI)			new_coin_spr->setPosition(ccp(260-32*3-16+t_coin_number*32,25));
		else if(myGD->gamescreen_type == kGT_rightUI)		new_coin_spr->setPosition(ccp(220-32*3-16+t_coin_number*32,25));
		else									new_coin_spr->setPosition(ccp(260-32*3-16+t_coin_number*32,25));
		addChild(new_coin_spr);
		
		exchange_dic->setObject(new_coin_spr, t_coin_number);
		
		if(taked_coin_cnt >= 6 && !isGameover && getPercentage() < clearPercentage)
		{
			if(clr_cdt_type == kCLEAR_sequenceChange)
			{
				conditionClear();
			}
			
			isFirst = true;
			is_exchanged = true;
			myGD->communication("Main_startExchange");
			myGD->communication("Main_showChangeCard");
			myGD->communication("Jack_positionRefresh");
		}
	}
	
	void subBossLife(float t_life)
	{
		//##
		// gamedata 로부터 boss pointer 받아서 해결.
		if(clr_cdt_type != kCLEAR_bossLifeZero || is_cleared_cdt || isGameover)
			return;
		
//		t_life = MissileDamageData::getCorrelationDamage(t_life, main_cumber_element);
		
//		if(bossLife < t_life)
//			bossLife = 0;
//		else
//			bossLife -= t_life;
		
//		m_bossLifeGage->setPercentage(bossLife/maxBossLife);
		KSCumberBase* cb = dynamic_cast<KSCumberBase*>(myGD->getCommunicationNode("CP_getMainCumberPointer"));
		if(!is_cleared_cdt)
			((CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel))->setString(CCString::createWithFormat("%.1f%%",
						cb->getLife()/cb->getTotalLife()*100.f)->getCString());
		
		if(cb->getLife() <= 0.f && !is_cleared_cdt)
		{
			conditionClear();
		}
	}
	
	void setMaxBossLife(float t_life)
	{
		maxBossLife = t_life;
		bossLife = maxBossLife*(100.f - mySGD->getBossLittleEnergyValue())/100.f;
	}
	
	void setClearPercentage(float t_p)
	{
		clearPercentage = t_p;
	}
	
	void startCounting()
	{
		schedule(schedule_selector(PlayUI::counting), 1.f);
	}
	
	void resumeCounting()
	{
		schedule(schedule_selector(PlayUI::counting), 1.f);
	}
	
	void stopCounting()
	{
		unschedule(schedule_selector(PlayUI::counting));
	}
	
	void keepBossLife()
	{
		keepLife = bossLife;
	}
	
	void checkBossLife()
	{
//		if((keepLife - bossLife)/maxBossLife > 0.1)
//		{
//			myGD->communication("CP_mainCumberShowEmotion", 2);
//		}
//		else if((keepLife - bossLife)/maxBossLife < 0.02)
//		{
//			myGD->communication("CP_mainCumberShowEmotion", 3);
//		}
//		else
//		{
//			myGD->communication("CP_mainCumberShowEmotion", 1);
//		}
	}
	
	int getGameTime()
	{
		return countingCnt;
	}
	
	void setControlTD(CCObject* t_main, SEL_CallFunc d_gesture, SEL_CallFunc d_button, SEL_CallFunc d_joystick, SEL_CallFunc d_startControl)
	{
		target_main = t_main;
		delegate_gesture = d_gesture;
		delegate_button = d_button;
		delegate_joystick = d_joystick;
		delegate_startControl = d_startControl;
	}
	
	void showPause()
	{
		mySGD->is_paused = true;
		PausePopupLayer* t_ppl = PausePopupLayer::create(this, callfunc_selector(PlayUI::goHome), this, callfunc_selector(PlayUI::cancelHome), target_main, delegate_gesture, delegate_button, delegate_joystick);
		addChild(t_ppl);
	}
	
	void showContinuePopup(CCObject* t_end, SEL_CallFunc d_end, CCObject* t_continue, SEL_CallFunc d_continue)
	{
		target_continue = t_continue;
		delegate_continue = d_continue;
		
		ContinuePopup* t_cpl = ContinuePopup::create(t_end, d_end, this, callfunc_selector(PlayUI::continueAction));
		addChild(t_cpl);
	}
	
	void addGameTime30Sec()
	{
		if(countingCnt >= 50 && countingCnt < 100)
		{
			countingLabel->setColor(ccWHITE);
		}
		else if(countingCnt >= 100 && countingCnt < 130)
		{
			countingLabel->setColor(ccYELLOW);
		}
		else if(countingCnt >= 130)
		{
			AudioEngine::sharedInstance()->stopEffect("sound_time_noti.mp3");
			countingLabel->setColor(ccORANGE);
		}
		
		countingCnt -= 30;
		if(mySGD->isUsingItem(kIC_longTime))
		{
			if(countingCnt < -mySGD->getLongTimeValue())
				countingCnt = -mySGD->getLongTimeValue();
		}
		else
		{
			if(countingCnt < 0)
				countingCnt = 0;
		}
		countingLabel->setString(CCString::createWithFormat("%d", playtime_limit-countingCnt)->getCString());
	}
	
	bool beRevivedJack()
	{
		if(jack_life > 0)
		{
			addGameTime30Sec();
			
			jack_life--;
			removeChild((CCNode*)jack_array->lastObject(), true);
			jack_array->removeLastObject();
			return true;
		}
		else
		{
			return false;
		}
	}
	
	void takeAddTimeItem()
	{
		countingCnt -= SDS_GI(kSDF_stageInfo, mySD->getSilType(), "itemOption_addTime_sec");
		
		if(countingCnt < -mySGD->getLongTimeValue())
			countingCnt = -mySGD->getLongTimeValue();
		
		countingLabel->setString(CCString::createWithFormat("%d", playtime_limit-countingCnt)->getCString());
	}
	
	bool getIsExchanged()
	{
		return is_exchanged;
	}
	
private:
	int percentage_decrease_cnt;
	
	CCObject* target_main;
	SEL_CallFunc delegate_gesture;
	SEL_CallFunc delegate_button;
	SEL_CallFunc delegate_joystick;
	SEL_CallFunc delegate_startControl;
	
	CCObject* target_continue;
	SEL_CallFunc delegate_continue;
	
	float bossLife;
	float keepLife;
	float maxBossLife;
	float clearPercentage;
	
	ElementCode main_cumber_element;
	
	CCSprite* ui_case;
	CCSprite* sand_clock;
	
	BossLifeGage* m_bossLifeGage;
	AreaGage* m_areaGage;
	
//	CCSprite* bossLifeGage;
	
	GoldLabel* gold_label;
	CCLabelBMFont* score_label;
	CCLabelBMFont* percentageLabel;
	CCLabelBMFont* countingLabel;
	
	CCSprite* result_sprite;
	
	CCDictionary* exchange_dic;
	bool is_exchanged;
	bool is_show_exchange_coin;
	int taked_coin_cnt;
	
	bool isFirst;
	bool isGameover;
	bool is_hard;
	int beforePercentage;
	
	int playtime_limit;
	int countingCnt;
	
	int jack_life;
	CCArray* jack_array;
	
	bool is_cleared_cdt; // cdt => condition
	CLEAR_CONDITION clr_cdt_type;
	bool is_show_condition;
	int clr_cdt_cnt;
	int ing_cdt_cnt;
	float clr_cdt_per;
	float clr_cdt_range;
	
	void counting()
	{
		countingCnt++;
		
		countingLabel->setString(CCString::createWithFormat("%d", playtime_limit-countingCnt)->getCString());
		CCRotateBy* t_rotate = CCRotateBy::create(0.5, -180);
		sand_clock->runAction(t_rotate);
		
		if(countingCnt == 50)
		{
			AudioEngine::sharedInstance()->playEffect("sound_time_noti.mp3", false);
			countingLabel->setColor(ccYELLOW);
			CCScaleTo* t_scale1 = CCScaleTo::create(0.2f, 2.5f);
			CCScaleTo* t_scale2 = CCScaleTo::create(0.6f, 1.f);
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale1, t_scale2);
			countingLabel->runAction(t_seq);
		}
		else if(countingCnt == 100)
		{
			AudioEngine::sharedInstance()->playEffect("sound_time_noti.mp3", false);
			countingLabel->setColor(ccORANGE);
			CCScaleTo* t_scale1 = CCScaleTo::create(0.2f, 2.5f);
			CCScaleTo* t_scale2 = CCScaleTo::create(0.6f, 1.3f);
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale1, t_scale2);
			countingLabel->runAction(t_seq);
		}
		else if(countingCnt == 130)
		{
			AudioEngine::sharedInstance()->playEffect("sound_time_noti.mp3", true);
			countingLabel->setColor(ccRED);
			CCScaleTo* t_scale1 = CCScaleTo::create(0.2f, 2.5f);
			CCScaleTo* t_scale2 = CCScaleTo::create(0.6f, 1.6f);
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale1, t_scale2);
			countingLabel->runAction(t_seq);
		}
		
		if(countingCnt >= playtime_limit)
		{
			stopCounting();
			// timeover
			
			mySGD->fail_code = kFC_timeover;
			
			myGD->communication("Main_allStopSchedule");
			AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
			result_sprite = CCSprite::create("game_timeover.png");
			result_sprite->setRotation(-25);
			result_sprite->setPosition(ccp(240,myDSH->ui_center_y));
			addChild(result_sprite);
			
			endGame();
		}
	}
	
	void endGame()
	{
		AudioEngine::sharedInstance()->stopEffect("sound_time_noti.mp3");
//		myGD->communication("CP_setGameover");
		CCDelayTime* n_d = CCDelayTime::create(3.f);
		CCCallFunc* nextScene = CCCallFunc::create(this, callfunc_selector(PlayUI::nextScene));
		
		CCSequence* sequence = CCSequence::createWithTwoActions(n_d, nextScene);
		
		runAction(sequence);
	}
	
	void nextScene()
	{
		result_sprite->setVisible(false);
		myGD->communication("Main_gameover");
	}
	
	void catchSubCumber()
	{
		if(is_cleared_cdt || clr_cdt_type != kCLEAR_subCumberCatch || isGameover)
			return;
		
		ing_cdt_cnt++;
		
		((CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel))->setString(CCString::createWithFormat("%d/%d", ing_cdt_cnt, clr_cdt_cnt)->getCString());
		if(ing_cdt_cnt >= clr_cdt_cnt)		conditionClear();
	}
	
	void takeBigArea()
	{
		if(is_cleared_cdt || clr_cdt_type != kCLEAR_bigArea || isGameover)
			return;
		
		ing_cdt_cnt++;
		
		int item_value = mySGD->getSmallAreaValue();
		
		((CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel))->setString(CCString::createWithFormat("%2.0f%%:%d/%d", (clr_cdt_per-item_value/100.f)*100.f, ing_cdt_cnt, clr_cdt_cnt)->getCString());
		if(ing_cdt_cnt >= clr_cdt_cnt)		conditionClear();
	}
	
	void takeItemCollect()
	{
		if(is_cleared_cdt || clr_cdt_type != kCLEAR_itemCollect || isGameover)
			return;
		
		ing_cdt_cnt++;
		((CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel))->setString(CCString::createWithFormat("%d/%d", ing_cdt_cnt, clr_cdt_cnt)->getCString());
		if(ing_cdt_cnt >= clr_cdt_cnt)		conditionClear();
	}
	
	void myInit()
	{
		isGameover = false;
		
		int re_chapter_number = SelectedMapData::sharedInstance()->getViewChapterNumber();
		
		if(re_chapter_number == 1 && SelectedMapData::sharedInstance()->getSelectedStage() == 1)
			main_cumber_element = kElementCode_empty;
		else if(re_chapter_number <= 2)			main_cumber_element = kElementCode_life;
		else if(re_chapter_number <= 4)			main_cumber_element = kElementCode_fire;
		else if(re_chapter_number <= 6)			main_cumber_element = kElementCode_water;
		else if(re_chapter_number <= 8)			main_cumber_element = kElementCode_water;
		else if(re_chapter_number <= 10)		main_cumber_element = kElementCode_fire;
		else if(re_chapter_number == 11)		main_cumber_element = kElementCode_empty;
		else if(re_chapter_number == 12)		main_cumber_element = kElementCode_water;
		else if(re_chapter_number == 13)		main_cumber_element = kElementCode_life;
		else if(re_chapter_number == 14)		main_cumber_element = kElementCode_life;
		else if(re_chapter_number == 15)		main_cumber_element = kElementCode_fire;
		else if(re_chapter_number == 16)		main_cumber_element = kElementCode_water;
		
		percentage_decrease_cnt = 0;
		
		clearPercentage = 1;
		
		ui_case = CCSprite::create("ui_back.png");
		if(myGD->gamescreen_type == kGT_leftUI)			ui_case->setPosition(ccp(25,myDSH->ui_center_y));
		else if(myGD->gamescreen_type == kGT_rightUI)		ui_case->setPosition(ccp(480-25,myDSH->ui_center_y));
		else
		{
			ui_case->setPosition(ccp(25,myDSH->ui_center_y));
			ui_case->setOpacity(0);
		}
		addChild(ui_case);
		
		gold_label = GoldLabel::create();
		addChild(gold_label);
		
		score_label = CountingBMLabel::create("0", "etc_font.fnt", 2.f);
		score_label->setAnchorPoint(ccp(1.0,0.5));
		score_label->setPosition(ccp(288,464));
		addChild(score_label);
		
		m_areaGage = AreaGage::create();
		m_areaGage->setPosition(ccp(228,441));
		addChild(m_areaGage);
		
		CCSprite* percentage_p = CCSprite::create("maingame_percentage.png");
		percentage_p->setPosition(ccp(250,440));
		addChild(percentage_p);
		
		percentageLabel = CCLabelBMFont::create("0", "etc_font.fnt");
		percentageLabel->setAnchorPoint(ccp(1.0, 0.5));
		if(myGD->gamescreen_type == kGT_leftUI)			percentageLabel->setPosition(ccp(36,myDSH->ui_center_y));
		else if(myGD->gamescreen_type == kGT_rightUI)		percentageLabel->setPosition(ccp(480-50+36,myDSH->ui_center_y));
		else									percentageLabel->setPosition(ccp(470,myDSH->ui_top-20));
		
		addChild(percentageLabel);
		
		
//		CCSprite* counting_tiem = CCSprite::create("maingame_time.png");
//		counting_tiem->setPosition(ccp(140, 410));
//		addChild(counting_tiem);
		
		sand_clock = CCSprite::create("maingame_ui_sandclock.png");
		sand_clock->setPosition(ccp(147,463));
		addChild(sand_clock);
		
		countingCnt = -mySGD->getLongTimeValue();
		playtime_limit = mySDS->getIntegerForKey(kSDF_stageInfo, mySD->getSilType(), "playtime");
		
		countingLabel = CCLabelBMFont::create(CCString::createWithFormat("%d", playtime_limit-countingCnt)->getCString(), "etc_font.fnt");
		countingLabel->setScale(1.3);
		if(myGD->gamescreen_type == kGT_leftUI)			countingLabel->setPosition(ccp((480-50-myGD->boarder_value*2)/2.f+50+myGD->boarder_value,myDSH->ui_top-20));
		else if(myGD->gamescreen_type == kGT_rightUI)	countingLabel->setPosition(ccp((480-50-myGD->boarder_value*2)/2.f+myGD->boarder_value,myDSH->ui_top-20));
		else											countingLabel->setPosition(ccp(240,myDSH->ui_top-20));
		addChild(countingLabel);
		
		isFirst = true;
//		beforePercentage = 0;

		
		m_bossLifeGage = BossLifeGage::create();
		m_bossLifeGage->setPosition(ccp(108,441));
		addChild(m_bossLifeGage);
		
		
		CCSprite* n_home = CCSprite::create("ui_stop.png");
		CCSprite* s_home = CCSprite::create("ui_stop.png");
		s_home->setColor(ccGRAY);
		
		CCMenuItem* home_item = CCMenuItemSprite::create(n_home, s_home, this, menu_selector(PlayUI::menuAction));
		home_item->setTag(kMenuTagUI_home);
		
		CCMenu* home_menu = CCMenu::createWithItem(home_item);
		if(myGD->gamescreen_type == kGT_leftUI)			home_menu->setPosition(ccp(25,myDSH->ui_top-25));
		else if(myGD->gamescreen_type == kGT_rightUI)		home_menu->setPosition(ccp(480-25,myDSH->ui_top-25));
		else									home_menu->setPosition(ccp(25,myDSH->ui_top-25));
		addChild(home_menu);
		
		
		jack_array = new CCArray(1);
		
		jack_life = 3;
		for(int i=0;i<jack_life;i++)
		{
			CCSprite* jack_img = CCSprite::create("jack2.png", CCRectMake(0, 0, 23, 23));
			if(myGD->gamescreen_type == kGT_leftUI)			jack_img->setPosition(ccp(25, myDSH->ui_center_y-30-i*20));
			else if(myGD->gamescreen_type == kGT_rightUI)		jack_img->setPosition(ccp(480-25,myDSH->ui_center_y-30-i*20));
			else									jack_img->setPosition(ccp(100+i*20, myDSH->ui_top-20));
			addChild(jack_img);
			
			jack_array->addObject(jack_img);
		}
		
		is_exchanged = false;
		is_show_exchange_coin = false;
		exchange_dic = new CCDictionary();
		
		for(int i=1;i<=6;i++)
		{
			CCSprite* exchange_spr = CCSprite::create(CCString::createWithFormat("exchange_%d_unact.png", i)->getCString());
			if(myGD->gamescreen_type == kGT_leftUI)			exchange_spr->setPosition(ccp(260-32*3-16+i*32,25));
			else if(myGD->gamescreen_type == kGT_rightUI)		exchange_spr->setPosition(ccp(220-32*3-16+i*32,25));
			else									exchange_spr->setPosition(ccp(260-32*3-16+i*32,25));
			addChild(exchange_spr);
			
			exchange_dic->setObject(exchange_spr, i);
		}
		
		is_show_condition = false;
		clr_cdt_type = mySD->getClearCondition();
		
		CCPoint icon_menu_position;
		if(myGD->gamescreen_type == kGT_leftUI)			icon_menu_position = ccp(25,myDSH->ui_center_y+43);
		else if(myGD->gamescreen_type == kGT_rightUI)		icon_menu_position = ccp(480-25,myDSH->ui_center_y+43);
		else									icon_menu_position = ccp(390,myDSH->ui_top-25);
		
		if(clr_cdt_type == kCLEAR_bossLifeZero)
		{
			is_cleared_cdt = false;
			
			CCSprite* n_icon = CCSprite::create("condition1_menu.png");
			CCSprite* s_icon = CCSprite::create("condition1_menu.png");
			s_icon->setColor(ccGRAY);
			
			CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
			icon_item->setTag(kMenuTagUI_condition);
			
			CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
			icon_menu->setPosition(icon_menu_position);
			addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
			
			int start_percentage = 100 - mySGD->getBossLittleEnergyValue();
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d%%", start_percentage)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
		}
		else if(clr_cdt_type == kCLEAR_subCumberCatch)
		{
			is_cleared_cdt = false;
			
			CCSprite* n_icon = CCSprite::create("condition2_menu.png");
			CCSprite* s_icon = CCSprite::create("condition2_menu.png");
			s_icon->setColor(ccGRAY);
			
			CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
			icon_item->setTag(kMenuTagUI_condition);
			
			CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
			icon_menu->setPosition(icon_menu_position);
			addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
			
			clr_cdt_cnt = mySD->getClearConditionCatchSubCumber();
			ing_cdt_cnt = 0;
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", ing_cdt_cnt, clr_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
		}
		else if(clr_cdt_type == kCLEAR_bigArea)
		{
			is_cleared_cdt = false;
			
			CCSprite* n_icon = CCSprite::create("condition3_menu.png");
			CCSprite* s_icon = CCSprite::create("condition3_menu.png");
			s_icon->setColor(ccGRAY);
			
			CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
			icon_item->setTag(kMenuTagUI_condition);
			
			CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
			icon_menu->setPosition(icon_menu_position);
			addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
			
			clr_cdt_per = mySD->getClearConditionBigAreaPer();
			clr_cdt_cnt = mySD->getClearConditionBigAreaCnt();
			ing_cdt_cnt = 0;
			
			int item_value = mySGD->getSmallAreaValue();
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%2.0f%%:%d/%d", (clr_cdt_per-item_value/100.f)*100.f, ing_cdt_cnt, clr_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
		}
		else if(clr_cdt_type == kCLEAR_itemCollect)
		{
			is_cleared_cdt = false;
			
			CCSprite* n_icon = CCSprite::create("condition4_menu.png");
			CCSprite* s_icon = CCSprite::create("condition4_menu.png");
			s_icon->setColor(ccGRAY);
			
			CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
			icon_item->setTag(kMenuTagUI_condition);
			
			CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
			icon_menu->setPosition(icon_menu_position);
			addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
			
			clr_cdt_cnt = mySD->getClearConditionItemCollect();
			ing_cdt_cnt = 0;
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", ing_cdt_cnt, clr_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
		}
		else if(clr_cdt_type == kCLEAR_perfect)
		{
			is_cleared_cdt = false;
			
			CCSprite* n_icon = CCSprite::create("condition5_menu.png");
			CCSprite* s_icon = CCSprite::create("condition5_menu.png");
			s_icon->setColor(ccGRAY);
			
			CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
			icon_item->setTag(kMenuTagUI_condition);
			
			CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
			icon_menu->setPosition(icon_menu_position);
			addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
			
			clr_cdt_per = mySD->getClearConditionPerfectBase();
			clr_cdt_range = mySD->getClearConditionPerfectRange();
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", clr_cdt_per*100.f)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
		}
		else if(clr_cdt_type == kCLEAR_sequenceChange)
		{
			is_cleared_cdt = false;
			
			CCSprite* n_icon = CCSprite::create("condition6_menu.png");
			CCSprite* s_icon = CCSprite::create("condition6_menu.png");
			s_icon->setColor(ccGRAY);
			
			CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
			icon_item->setTag(kMenuTagUI_condition);
			
			CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
			icon_menu->setPosition(icon_menu_position);
			addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
			
			ing_cdt_cnt = 1;
			
			if(mySGD->isUsingItem(kIC_randomChange))
			{
				CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", ing_cdt_cnt-1, 6)->getCString(), mySGD->getFont().c_str(), 12);
				clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
				addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
			}
			else
			{
				CCSprite* clr_cdt_img = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", ing_cdt_cnt)->getCString());
				clr_cdt_img->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
				addChild(clr_cdt_img, 0, kCT_UI_clrCdtLabel);
			}
		}
		else if(clr_cdt_type == kCLEAR_timeLimit)
		{
			is_cleared_cdt = false;
			
			CCSprite* n_icon = CCSprite::create("condition7_menu.png");
			CCSprite* s_icon = CCSprite::create("condition7_menu.png");
			s_icon->setColor(ccGRAY);
			
			CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
			icon_item->setTag(kMenuTagUI_condition);
			
			CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
			icon_menu->setPosition(icon_menu_position);
			addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
			
			ing_cdt_cnt = mySD->getClearConditionTimeLimit();
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", ing_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
		}
		else if(clr_cdt_type == kCLEAR_default)
		{
			is_cleared_cdt = true;
			
			CCSprite* icon_img = CCSprite::create("condition0_menu.png");
			icon_img->setPosition(icon_menu_position);
			addChild(icon_img, 0, kCT_UI_clrCdtIcon);
		}
		
		myGD->V_I["UI_addScore"] = std::bind(&PlayUI::addScore, this, _1);
		myGD->V_F["UI_setPercentage"] = std::bind(&PlayUI::setPercentage, this, _1);
		myGD->V_F["UI_subBossLife"] = std::bind(&PlayUI::subBossLife, this, _1);
		myGD->V_V["UI_decreasePercentage"] = std::bind(&PlayUI::decreasePercentage, this);
		myGD->B_V["UI_beRevivedJack"] = std::bind(&PlayUI::beRevivedJack, this);
		myGD->V_TDTD["UI_showContinuePopup"] = std::bind(&PlayUI::showContinuePopup, this, _1, _2, _3, _4);
		myGD->V_V["UI_catchSubCumber"] = std::bind(&PlayUI::catchSubCumber, this);
		myGD->V_V["UI_takeItemCollect"] = std::bind(&PlayUI::takeItemCollect, this);
		myGD->V_V["UI_takeAddTimeItem"] = std::bind(&PlayUI::takeAddTimeItem, this);
		myGD->V_V["UI_stopCounting"] = std::bind(&PlayUI::stopCounting, this);
		myGD->V_V["UI_resumeCounting"] = std::bind(&PlayUI::resumeCounting, this);
		myGD->F_V["UI_getMapPercentage"] = std::bind(&PlayUI::getPercentage, this);
	}
	
	void continueAction()
	{
		addGameTime30Sec();
		jack_life = 3;
		for(int i=0;i<jack_life;i++)
		{
			CCSprite* jack_img = CCSprite::create("jack2.png", CCRectMake(0, 0, 23, 23));
			if(myGD->gamescreen_type == kGT_leftUI)			jack_img->setPosition(ccp(25, myDSH->ui_center_y-30-i*20));
			else if(myGD->gamescreen_type == kGT_rightUI)		jack_img->setPosition(ccp(480-25,myDSH->ui_center_y-30-i*20));
			else									jack_img->setPosition(ccp(100+i*20,myDSH->ui_top-20));
			addChild(jack_img);
			
			jack_array->addObject(jack_img);
		}
		
		(target_continue->*delegate_continue)();
	}
	
	void menuAction(CCObject* sender)
	{
		AudioEngine::sharedInstance()->playEffect("sound_buttonClick_Low.mp3", false);
		int tag = ((CCNode*)sender)->getTag();
		if(tag == kMenuTagUI_home && !isGameover)
		{
			showPause();
		}
		else if(tag == kMenuTagUI_condition && !isGameover)
		{
			showCondition();
		}
	}
	
	void showCondition()
	{
		if(is_show_condition)
		{
			((ConditionPopup*)getChildByTag(kCT_UI_clrCdtPopup))->holdingPopup();
		}
		else
		{
			is_show_condition = true;
			ConditionPopup* t_cdt = ConditionPopup::create(this, callfunc_selector(PlayUI::closeCondition));
			addChild(t_cdt, 0, kCT_UI_clrCdtPopup);
		}
	}
	
	void closeCondition()
	{
		is_show_condition = false;
	}
	
	void closeShutter()
	{
		mySGD->is_paused = false;
		AudioEngine::sharedInstance()->setAppFore();
		CCDirector::sharedDirector()->resume();
		
		endCloseShutter();
	}
	
	void endCloseShutter()
	{
		
		mySGD->gameOver(0, 0, 0);
		mySGD->resetLabels();
		myGD->resetGameData();
		CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
	}
	
	void goHome()
	{
		closeShutter();
	}
	
	void cancelHome()
	{
		(target_main->*delegate_startControl)();
		mySGD->is_paused = false;
		AudioEngine::sharedInstance()->setAppFore();
		CCDirector::sharedDirector()->resume();
	}
	
	void alertAction(int t1, int t2)
	{
		
	}
};

#endif
