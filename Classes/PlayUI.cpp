// PlayUI.cpp
//

#include "PlayUI.h"
#include "KSCumberBase.h"
#include "StartSettingScene.h"
#include "AchieveNoti.h"
#include "RollingButton.h"
#include "KSLabelTTF.h"
#include "EnumDefine.h"
#include "GraySprite.h"
#include "OnePercentGame.h"
#include "FlagSelector.h"
#include "MyLocalization.h"
#include "KSJuniorBase.h"
#include "OnePercentTutorial.h"
#include "CommonAnimation.h"
#include "ConvexGraph.h"
#include "Jack.h"
#define LZZ_INLINE inline
using namespace cocos2d;
using namespace std;
using namespace std::placeholders;
ComboView * ComboView::create (int combo)
{
	ComboView* t_cv = new ComboView();
	t_cv->myInit(combo);
	t_cv->autorelease();
	return t_cv;
}
void ComboView::changeCombo (int combo)
{
	combo_label->setString(CCString::createWithFormat("%s%d", myLoc->getLocalForKey(LK::kMyLocalKey_combo), combo)->getCString());
	shadow_label->setString(CCString::createWithFormat("%s%d", myLoc->getLocalForKey(LK::kMyLocalKey_combo), combo)->getCString());
	if(!mySGD->is_endless_mode)
	{
		shadow_label->setPosition(ccp(combo_label->getContentSize().width, combo_label->getContentSize().height/2.f) + ccp(0, -4));
	}
	else
	{
		shadow_label->setPosition(ccpFromSize(combo_label->getContentSize()/2.f) + ccp(0,-4));
	}
//	if(!mySGD->is_endless_mode)
//		combo_front->setPosition(ccp(combo_label->getPositionX()-combo_label->getContentSize().width-5,0));
}
void ComboView::setPercentage (float t_percent)
{
//	combo_timer->setPercentage(t_percent*100);
}
void ComboView::myInit (int combo)
{
//	combo_timer = CCProgressTimer::create(CCSprite::create("combo_front.png"));
//	combo_timer->setType(kCCProgressTimerTypeBar);
//	combo_timer->setMidpoint(ccp(0,0));
//	combo_timer->setBarChangeRate(ccp(1,0));
//	combo_timer->setPercentage(100);
//	combo_timer->setPosition(ccp(getContentSize().width/2.f-5, getContentSize().height/2.f));
//	addChild(combo_timer);
	
	if(mySGD->is_endless_mode)
	{
		combo_label = KSLabelTTF::create(ccsf("%s%d", myLoc->getLocalForKey(LK::kMyLocalKey_combo), combo), mySGD->getFont().c_str(), 30);
		combo_label->enableOuterStroke(ccc3(0, 45, 10), 2.5f, 255, true);
		combo_label->setGradientColor(ccc4(240, 255, 10, 255), ccc4(110, 190, 5, 255), ccp(0,-1));
		shadow_label = CommonAnimation::applyBigShadow(combo_label, combo_label->getFontSize());
		combo_label->setPosition(ccp(0,0));
		addChild(combo_label);
//		combo_label = CCLabelBMFont::create(CCString::createWithFormat("%d", combo)->getCString(), "combo.fnt");
//		combo_label->setAnchorPoint(ccp(0.5f,0.5f));
//		combo_label->setPosition(ccp(0,-3));
//		addChild(combo_label);
//		
//		combo_front = CCSprite::create("combo_front.png");
//		combo_front->setAnchorPoint(ccp(0.5f,0.5f));
//		combo_front->setPosition(ccp(0,-10));
//		combo_front->setScale(0.5f);
//		addChild(combo_front);
	}
	else
	{
		combo_label = KSLabelTTF::create(ccsf("%s%d", myLoc->getLocalForKey(LK::kMyLocalKey_combo), combo), mySGD->getFont().c_str(), 30);
		combo_label->enableOuterStroke(ccc3(0, 45, 10), 2.5f, 255, true);
		combo_label->setGradientColor(ccc4(240, 255, 10, 255), ccc4(110, 190, 5, 255), ccp(0,-1));
		shadow_label = CommonAnimation::applyBigShadow(combo_label, combo_label->getFontSize());
		shadow_label->setAnchorPoint(ccp(1,0.5f));
		shadow_label->setPosition(ccp(combo_label->getContentSize().width, combo_label->getContentSize().height/2.f) + ccp(0, -4));
		combo_label->setAnchorPoint(ccp(1,0.5f));
		combo_label->setPosition(ccp(0,0));
		addChild(combo_label);
		
//		combo_label = CCLabelBMFont::create(CCString::createWithFormat("%d", combo)->getCString(), "combo.fnt");
//		combo_label->setAnchorPoint(ccp(1,0.5f));
//		combo_label->setPosition(ccp(-22,-17));
//		addChild(combo_label);
//		
//		combo_front = CCSprite::create("combo_front.png");
//		combo_front->setAnchorPoint(ccp(1,0.5f));
//		combo_front->setPosition(ccp(combo_label->getPositionX()-combo_label->getContentSize().width-5,0));
//		addChild(combo_front);
	}
}
ComboParent * ComboParent::create (CCNode* t_score_label)
{
	ComboParent* t_cp = new ComboParent();
	t_cp->myInit(t_score_label);
	t_cp->autorelease();
	return t_cp;
}
void ComboParent::showCombo (int t_combo)
{
	if(getChildrenCount() > 0)
	{
		ComboView* t_cv = (ComboView*)getChildren()->objectAtIndex(0);// randomObject();
		t_cv->changeCombo(t_combo);
	}
	else
	{
		ComboView* t_cv = ComboView::create(t_combo);
		t_cv->setScale(1.f/1.5f);
		
		if(mySGD->is_endless_mode)
			t_cv->setPosition(ccp(40,myDSH->ui_center_y+60));
		else if(mySGD->is_hell_mode)
			t_cv->setPosition(ccp(480-8,myDSH->ui_top-46));
		else
			t_cv->setPosition(ccp(480-8,myDSH->ui_top-65));
		addChild(t_cv,0,1);// 1 : ComboView
	}
	
	keeping_frame = 300;
	if(!is_keeping)
		startKeep();
}
void ComboParent::stopKeep ()
{
	unschedule(schedule_selector(ComboParent::keeping));
	removeChildByTag(1);
	myGD->communication("UI_setComboCnt", 0);
	is_keeping = false;
//	score_label->setVisible(true);
}
void ComboParent::startKeep ()
{
//	score_label->setVisible(false);
	is_keeping = true;
	schedule(schedule_selector(ComboParent::keeping));
}
void ComboParent::keeping ()
{
//	if(myGD->getJackState() == jackState::jackStateDrawing)
//	{
//		if(!is_holding)
//		{
//			is_holding = true;
//			hold_frame = 1;
//		}
//		else
//		{
//			hold_frame++;
//		}
//	}
//	else
//	{
//		if(is_holding)
//		{
//			is_holding = false;
//			hold_frame = 0;
////			keeping_frame -= hold_frame;
//		}
	
		keeping_frame--;
		//	((ComboView*)getChildByTag(1))->setPercentage(keeping_frame/500.f);
		
		if(keeping_frame <= 0)
		{
			stopKeep();
		}
//	}
}
void ComboParent::myInit (CCNode* t_score_label)
{
	is_keeping = false;
	score_label = t_score_label;
	is_holding = false;
	hold_frame = 0;
}
FeverParent * FeverParent::create (CCNode* t_counting_label)
{
	FeverParent* t_fp = new FeverParent();
	t_fp->myInit(t_counting_label);
	t_fp->autorelease();
	return t_fp;
}
void FeverParent::addFeverGage (int count)
{
	if(ing_fever)	return;
	
	keeping_count = 300;
//	if(!is_keeping) 피버게이지를 일정 시간 지나면 초기화 하는거 대한 처리
//		startKeep();
	
//	recent_count += count;
//	if(recent_count >= 15)
//	{
//		if(((CCLabelTTF*)counting_label)->getColor().g > 150)
//			counting_label->setVisible(false);
//		
//		fever_back->setVisible(true);
//		fever_top->setVisible(true);
//		
//		ing_fever = true;
//		entered_fever_cnt++;
//		int total_fever_cnt = myDSH->getIntegerForKey(kDSH_Key_achieve_totalFeverCnt)+1;
//		myDSH->setIntegerForKey(kDSH_Key_achieve_totalFeverCnt, total_fever_cnt);
//		
//		AchieveConditionReward* shared_acr = AchieveConditionReward::sharedInstance();
//		
//		if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_feverMania1) == 0 &&
//		   entered_fever_cnt >= shared_acr->getCondition(kAchievementCode_feverMania1))
//		{
//			myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_feverMania1, 1);
//			AchieveNoti* t_noti = AchieveNoti::create(kAchievementCode_feverMania1);
//			CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
//		}
//		
//		for(int i=kAchievementCode_feverMania2;i<=kAchievementCode_feverMania3;i++)
//		{
//			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) == 0 &&
//			   total_fever_cnt == shared_acr->getCondition((AchievementCode)i))
//			{
//				myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, i, 1);
//				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
//				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
//			}
//		}
//		
//		
//		recent_count = 15;
//		
//		fever_top->setPercentage(100.f);
//		
//		myLog->addLog(kLOG_show_fever, myGD->getCommunication("UI_getUseTime"));
//		myGD->communication("GIM_startFever");
//		
//		myGD->setAlphaSpeed(myGD->getAlphaSpeed() + 1.5f);
//		
//		fever_particle = CCParticleSystemQuad::createWithTotalParticles(100);
//		fever_particle->setPositionType(kCCPositionTypeFree);
//		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("fever_particle.png");
//		fever_particle->setTexture(texture);
//		fever_particle->setEmissionRate(100);
//		fever_particle->setAngle(90.0);
//		fever_particle->setAngleVar(40.0);
//		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
//		fever_particle->setBlendFunc(blendFunc);
//		fever_particle->setDuration(-1.0);
//		fever_particle->setEmitterMode(kCCParticleModeGravity);
//		ccColor4F startColor = {1.0,0.8,0.4,1.0}; // 0.76 0.25 0.12
//		fever_particle->setStartColor(startColor);
//		ccColor4F startColorVar = {0,0,0,0.3};
//		fever_particle->setStartColorVar(startColorVar);
//		ccColor4F endColor = {0.0,0.0,0.0,1.00};
//		fever_particle->setEndColor(endColor);
//		ccColor4F endColorVar = {0,0,0,0.3};
//		fever_particle->setEndColorVar(endColorVar);
//		fever_particle->setStartSize(10.0);
//		fever_particle->setStartSizeVar(10.0);
//		fever_particle->setEndSize(5.0);
//		fever_particle->setEndSizeVar(10.0);
//		fever_particle->setGravity(ccp(0,-400));
//		fever_particle->setRadialAccel(0.0);
//		fever_particle->setRadialAccelVar(0.0);
//		fever_particle->setSpeed(200);
//		fever_particle->setSpeedVar(50.0);
//		fever_particle->setTangentialAccel(0);
//		fever_particle->setTangentialAccelVar(0);
//		fever_particle->setTotalParticles(100);
//		fever_particle->setLife(1.0);
//		fever_particle->setLifeVar(0.0);
//		fever_particle->setStartSpin(0.0);
//		fever_particle->setStartSpinVar(360.f);
//		fever_particle->setEndSpin(0.0);
//		fever_particle->setEndSpinVar(360.f);
//		fever_particle->setPosition(ccp(240,140));
//		fever_particle->setPosVar(ccp(240,160));
//		addChild(fever_particle);
//		
//		unschedule(schedule_selector(FeverParent::keeping));
//		is_keeping = false;
//		
//		CCDelayTime* t_delay = CCDelayTime::create(NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_feverTime_i, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1));
//		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(FeverParent::endFever));
//		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
//		runAction(t_seq);
//		
//		CCProgressFromTo* progress_to = CCProgressFromTo::create(NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_feverTime_i, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1), 100.f, 0.f);
//		fever_top->runAction(progress_to);
//	}
//	else
//	{
//		CCProgressTo* progress_to = CCProgressTo::create(0.3f, recent_count/15.f*100.f);
//		fever_top->runAction(progress_to);
//	}
}
void FeverParent::endFever ()
{
	if(ing_fever)
	{
		counting_label->setVisible(true);
		fever_back->setVisible(false);
		fever_top->setVisible(false);
		
		ing_fever = false;
		recent_count = 0;
		
		myGD->communication("GIM_stopFever");
		
		myGD->setAlphaSpeed(myGD->getAlphaSpeed() - 1.5f);
		
		fever_particle->setDuration(0.f);
		fever_particle->setAutoRemoveOnFinish(true);
		
		fever_top->stopAllActions();
		fever_top->setPercentage(0.f);
		CCProgressTo* progress_to = CCProgressTo::create(0.3f, recent_count/20.f*100.f);
		fever_top->runAction(progress_to);
	}
}
void FeverParent::startKeep ()
{
	is_keeping = true;
	schedule(schedule_selector(FeverParent::keeping));
}
void FeverParent::keeping ()
{
	keeping_count--;
	
	if(keeping_count <= 0)
		stopKeep();
}
void FeverParent::stopKeep ()
{
	unschedule(schedule_selector(FeverParent::keeping));
	is_keeping = false;
	recent_count = 0;
	CCProgressTo* progress_to = CCProgressTo::create(0.3f, recent_count/10.f*100.f);
	fever_top->runAction(progress_to);
}
void FeverParent::myInit (CCNode* t_counting_label)
{
	counting_label = t_counting_label;
	ing_fever = false;
	keeping_count = 0;
	is_keeping = false;
	entered_fever_cnt = 0;
	
	fever_back = CCSprite::create("fever_gage_back.png");
	fever_back->setPosition(ccpAdd(counting_label->getPosition(), ccp(0,-20)));
	addChild(fever_back);
	fever_back->setVisible(false);
	
	fever_top = CCProgressTimer::create(CCSprite::create("fever_gage_top.png"));
	fever_top->getSprite()->setColor(ccc3(150, 0, 255));
	fever_top->setType(kCCProgressTimerTypeBar);
	fever_top->setMidpoint(ccp(0,0));
	fever_top->setBarChangeRate(ccp(1,0));
	fever_top->setPosition(fever_back->getPosition());
	fever_top->setPercentage(0.f);
	addChild(fever_top);
	fever_top->setVisible(false);
	
	recent_count = 0;
	
	fever_particle = NULL;
}
GoldLabel * GoldLabel::create ()
{
	GoldLabel* t_gl = new GoldLabel();
	t_gl->myInit();
	t_gl->autorelease();
	return t_gl;
}
void GoldLabel::setString (char const * after_gold)
{
//	stopAllActions();
	if(is_incresing)
		stopIncreasing();
	startIncreasing();
}
void GoldLabel::startIncreasing ()
{
	is_incresing = true;
//	CCScaleTo* t_scale1 = CCScaleTo::create(0.2f, 1.5f);
//	CCScaleTo* t_scale2 = CCScaleTo::create(0.5f, 1.f);
//	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale1, t_scale2);
//	runAction(t_seq);
	
	int stageGold = mySGD->getStageGold();
	myGD->communication("UI_checkStageGoldMission", stageGold);
	
	int stageBaseGold = mySGD->getStageBaseGold();
	keep_gold_string = CCString::createWithFormat("%d", stageBaseGold)->getCString();
	base_gold = atof(getString()); // 원래 가지고 있던 골드
	keep_gold = stageBaseGold - base_gold; // 이번에 얻은 골드
	decrease_gold = keep_gold;
	increase_gold = 0.f;
	schedule(schedule_selector(GoldLabel::increasing));
}
void GoldLabel::increasing (float dt)
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
void GoldLabel::stopIncreasing ()
{
	unschedule(schedule_selector(GoldLabel::increasing));
	is_incresing = false;
	CCLabelBMFont::setString(keep_gold_string.c_str());
}
void GoldLabel::myInit ()
{
	is_incresing = false;
	CCLabelBMFont::initWithString(CCString::createWithFormat("%d", mySGD->getStageBaseGold())->getCString(), "goldfont.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentRight, CCPointZero);
	stopIncreasing();
	setAnchorPoint(ccp(1.f,0.5));
	
	if(mySGD->is_endless_mode || mySGD->is_hell_mode)
	{
		setPosition(ccp(480-8, myDSH->ui_top-25));
	}
	else
	{
		setPosition(ccp(480-8, myDSH->ui_top-44));
	}
//	if(myGD->gamescreen_type == kGT_leftUI)			setPosition(ccp((480-50-myGD->boarder_value*2)*1.1f/4.f+50+myGD->boarder_value,myDSH->ui_top-15));
//	else if(myGD->gamescreen_type == kGT_rightUI)	setPosition(ccp((480-50-myGD->boarder_value*2)*1.1f/4.f+myGD->boarder_value,myDSH->ui_top-15));
//	else											setPosition(ccp((480-myGD->boarder_value*2)*1.1f/4.f,myDSH->ui_top-15));
}
MyGold * MyGold::create ()
{
	MyGold* t_mg = new MyGold();
	t_mg->myInit();
	t_mg->autorelease();
	return t_mg;
}
void MyGold::startMoving ()
{
	getParent()->reorderChild(this, z_order);
	ing_frame = 0;
	r_dy = i_dy;
	schedule(schedule_selector(MyGold::moving));
}
void MyGold::moving ()
{
	ing_frame++;
	
	setScale(getScale()*dscale);
	
	if(ing_frame > 30)
	{
		setOpacity(getOpacity() - 8);
	}
	
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
void MyGold::myInit ()
{
	int start_cut = rand()%6;
	CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("fever_coin_bronze.png");
	initWithTexture(t_texture, CCRectMake(start_cut*30, 0, 30, 30));
	CCAnimation* t_animation = CCAnimation::create();
	t_animation->setDelayPerUnit(0.1f);
	int add_count = 0;
	for(int i=start_cut;add_count < 6;i=(i+1)%6)
	{
		add_count++;
		t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*30, 0, 30, 30));
	}
	CCAnimate* t_animate = CCAnimate::create(t_animation);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
	runAction(t_repeat);
	
//	initWithFile("gold.png");
	init_position = CCPointZero;
	setScale(0.3f);
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
GetGold * GetGold::create (CCPoint t_sp, int t_duration_frame)
{
	GetGold* t_gg = new GetGold();
	t_gg->myInit(t_sp, t_duration_frame);
	t_gg->autorelease();
	return t_gg;
}
void GetGold::startMyAction ()
{
	ing_frame = 0;
	schedule(schedule_selector(GetGold::myAction));
}
void GetGold::myAction ()
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
void GetGold::selfRemove ()
{
	if(batch_node->getChildrenCount() <= 0)
	{
		unschedule(schedule_selector(GetGold::selfRemove));
		removeFromParentAndCleanup(true);
	}
}
void GetGold::myInit (CCPoint t_sp, int t_duration_frame)
{
	AudioEngine::sharedInstance()->playEffect("sound_get_coin.mp3", false);
	duration_frame = t_duration_frame;
	mySGD->addChangeGoodsIngameGold(duration_frame, 0);
	
	create_frame = duration_frame/60 + 1;
	
	batch_node = CCSpriteBatchNode::create("fever_coin_bronze.png");//"gold.png");
	batch_node->setPosition(t_sp);
	addChild(batch_node);
}
GetPercentage * GetPercentage::create (float t_gp, bool is_item)
{
	GetPercentage* t_g = new GetPercentage();
	t_g->myInit(t_gp, is_item);
	t_g->autorelease();
	return t_g;
}
void GetPercentage::startFadeOut ()
{
//	CCFadeOut* t_fadeout1 = CCFadeOut::create(1.f);
//	
//	backImg->runAction(t_fadeout1);
	
	t_value+=3;
	if(t_value >= 255)
		removeFromParent();
	else
		KS::setOpacity(my_label, 255-t_value);
}
void GetPercentage::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void GetPercentage::myInit (float t_gp, bool is_item)
{
	my_label = KSLabelTTF::create(CCString::createWithFormat("+%.1f%%", t_gp < 0.01f ? 0.f : t_gp)->getCString(), mySGD->getFont().c_str(), 12);
	// CCLabelBMFont::create(CCString::createWithFormat("%.1f%%", t_gp < 0.01f ? 0.f : t_gp)->getCString(), "gain.fnt");
	my_label->setColor(ccYELLOW);
	my_label->enableOuterStroke(ccBLACK, 1.f);
	addChild(my_label, kZorderGetPercentage_label);
	
	t_value = 0;
	
//	if(is_item)
//	{
//		CCSprite* t_texture = CCSprite::create("get_percentage.png");
//		backImg = CCSprite::createWithTexture(t_texture->getTexture(), CCRectMake(0, 24, 52.5, 24));
//		addChild(backImg, kZorderGetPercentage_backImg);
//		
//		CCAnimation* t_animation = CCAnimation::create();
//		t_animation->setDelayPerUnit(0.2);
//		for(int i=1;i<=3;i++)
//			t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(0, i*24, 52.5, 24));
//		CCAnimate* t_animate = CCAnimate::create(t_animation);
//		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GetPercentage::startFadeOut));
//		CCSequence* t_seq = CCSequence::createWithTwoActions(t_animate, t_call);
//		
//		backImg->runAction(t_seq);
//	}
//	else
//	{
//		backImg = CCSprite::create("get_percentage.png", CCRectMake(0, 0, 52.5, 24));
//		addChild(backImg, kZorderGetPercentage_backImg);
	schedule(schedule_selector(GetPercentage::startFadeOut));
//		startFadeOut();
//	}
}
TakeSpeedUp * TakeSpeedUp::create (int t_step, std::function<void()> t_end_func)
{
	TakeSpeedUp* t_g = new TakeSpeedUp();
	t_g->myInit(t_step, t_end_func);
	t_g->autorelease();
	return t_g;
}
void TakeSpeedUp::startFadeOut ()
{
	addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
								  {
									  KS::setOpacity(this, (1.f-t)*255);
								  }, [=](float t)
								  {
									  KS::setOpacity(this, 0);
									  selfRemove();
								  }));
}
void TakeSpeedUp::selfRemove ()
{
	end_function();
	removeFromParentAndCleanup(true);
}
void TakeSpeedUp::myInit (int t_step, std::function<void()> t_end_func)
{
	end_function = t_end_func;
	
	setPosition(CCPointZero);
	
	KSLabelTTF* speed_label;
	KSLabelTTF* shadow;
	
	CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
	
	if(NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_speed_d, t_history.characterIndex.getV(), t_history.characterLevel.getV()) + t_step*0.1f >= 2.f)
	{
		int i = kAchievementCode_hidden_speedMania;
		
		if(!myAchieve->isCompleted(AchievementCode(i)) && !myAchieve->isAchieve(AchievementCode(i)))
		{
			if(!myAchieve->isNoti(AchievementCode(i)))
			{
				myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition(AchievementCode(i)));
				AchieveNoti* t_noti = AchieveNoti::create(AchievementCode(i));
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
		
		speed_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_maxSpeed), mySGD->getFont().c_str(), 30);
		speed_label->enableOuterStroke(ccc3(65, 5, 35), 2.5f, 255, true);
		speed_label->setGradientColor(ccc4(255, 115, 250, 255), ccc4(215, 60, 130, 255), ccp(0,-1));
		shadow = CommonAnimation::applyBigShadow(speed_label, speed_label->getFontSize());
		speed_label->setPosition(ccp(0,0));
//		CCSprite* speed_label = CCSprite::create("speed_max.png");
		addChild(speed_label);
	}
	else
	{
		speed_label = KSLabelTTF::create(ccsf("%s%d", myLoc->getLocalForKey(LK::kMyLocalKey_speed), t_step), mySGD->getFont().c_str(), 30);
		speed_label->enableOuterStroke(ccc3(0, 25, 45), 2.5f, 255, true);
		speed_label->setGradientColor(ccc4(95, 255, 255, 255), ccc4(50, 155, 255, 255), ccp(0,-1));
		shadow = CommonAnimation::applyBigShadow(speed_label, speed_label->getFontSize());
//		CCSprite* speed_label = CCSprite::create("speed_front.png");
		addChild(speed_label);
		
		speed_label->setPosition(ccp(0, 0));
	}
	
	addChild(KSTimer::create(0.4f, [=]()
							 {
								 addChild(KSGradualValue<float>::create(0.f, 1.f, 0.6f, [=](float t)
																		{
																			speed_label->setOpacity((1.f-t)*255);
																			shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																		}, [=](float t)
																		{
																			speed_label->setOpacity(0);
																			shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																			selfRemove();
																		}));
							 }));
	
	setScale(1.f/myGD->game_scale);
	
//	startFadeOut();
}

DetailWarning * DetailWarning::create (const std::string& fileName)
{
	DetailWarning* t_w = new DetailWarning();
	t_w->myInit(fileName);
	t_w->autorelease();
	return t_w;
}
void DetailWarning::startAction ()
{
	//CCMoveTo* t_move1 = CCMoveTo::create(0.4f, ccp(240,myDSH->ui_center_y));
	//CCHide* t_hide = CCHide::create();
	//CCDelayTime* t_delay1 = CCDelayTime::create(0.05f);
	//CCShow* t_show = CCShow::create();
	//CCDelayTime* t_delay2 = CCDelayTime::create(0.1f);
	//CCRepeat* t_repeat = CCRepeat::create(CCSequence::create(t_hide, t_delay1, t_show, t_delay2, NULL), 4);
	//CCMoveTo* t_move2 = CCMoveTo::create(0.4f, ccp(-160,myDSH->ui_center_y));
	//CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(DetailWarning::selfRemove));

	//runAction(CCSequence::create(t_move1, t_repeat, t_move2, t_call, NULL));
}
void DetailWarning::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void DetailWarning::myInit (const std::string& fileName)
{
	CCSprite* warningSprite = KS::loadCCBI<CCSprite*>(this, fileName).first;
	if(!warningSprite)
	{
		warningSprite = CCSprite::create("attack_warning.png");
	}
	warningSprite->setPosition(ccp(240, myDSH->ui_center_y));
	addChild(warningSprite);
	addChild(KSTimer::create(3.f, [=]()
				{
					removeFromParentAndCleanup(true);
				}));
	//initWithFile("attack_warning.png");

	//if(t1 == 1)
	//{
		//setColor(ccWHITE);
	//}
	//else if(t1 == 2)
	//{
		//setColor(ccRED);
	//}
	//else if(t1 == 3)
	//{
		//setColor(ccGREEN);
	//}
	//setPosition(ccp(640,myDSH->ui_center_y));
}

Warning * Warning::create (int t1)
{
	Warning* t_w = new Warning();
	t_w->myInit(t1);
	t_w->autorelease();
	return t_w;
}
void Warning::startAction ()
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
void Warning::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void Warning::myInit (int t1)
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
	else if(t1 == 3)
	{
		setColor(ccGREEN);
	}
	setPosition(ccp(640,myDSH->ui_center_y));
}
TakeCoin * TakeCoin::create ()
{
	TakeCoin* t_w = new TakeCoin();
	t_w->myInit();
	t_w->autorelease();
	return t_w;
}
void TakeCoin::startMyAction()
{
	unschedule(schedule_selector(TakeCoin::startMyAction));
	
	CCNode* take_coin_node = CCNode::create();
	addChild(take_coin_node);
	
	KSLabelTTF* ment = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_collectChange), mySGD->getFont().c_str(), 35);
	ment->enableOuterStroke(ccc3(0, 25, 45), 2.5f, 255, true);
	ment->setGradientColor(ccc4(95, 255, 255, 255), ccc4(50, 155, 255, 255), ccp(0,-1));
	KSLabelTTF* shadow = CommonAnimation::applyBigShadow(ment, ment->getFontSize());
	ment->setBlendFunc({GL_ONE, GL_ONE_MINUS_SRC_ALPHA});
	ment->setPosition(ccp(0,30));
	ment->setScale(1.5f);
	ment->setOpacity(0);
	shadow->setOpacityOuterStroke(0);
	take_coin_node->addChild(ment);
	
	ment->addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
												 {
													 ment->setPosition(ccp(0,30-10*t));
													 ment->setScale(1.5f-0.5f*t);
													 ment->setOpacity(255*t);
													 shadow->setOpacityOuterStroke(255 * 0.3f*(t));
												 }, [=](float t)
												 {
													 ment->setPosition(ccp(0,30-10*t));
													 ment->setScale(1.5f-0.5f*t);
													 ment->setOpacity(255*t);
													 shadow->setOpacityOuterStroke(255 * 0.3f*(t));
													 
													 ment->addChild(KSTimer::create(1.f, [=]()
																					{
																						ment->addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
																																	 {
																																		 ment->setPosition(ccp(0,20+10*t));
																																		 ment->setScale(1.f+0.5f*t);
																																		 ment->setOpacity(255-255*t);
																																		 shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																	 }, [=](float t)
																																	 {
																																		 ment->setPosition(ccp(0,20+10*t));
																																		 ment->setScale(1.f+0.5f*t);
																																		 ment->setOpacity(255-255*t);
																																		 shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																		 removeFromParent();
																																	 }));
																					}));
												 }));
	
	KSLabelTTF* light = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_collectChange), mySGD->getFont().c_str(), 35);
	light->enableOuterStroke(ccc3(0, 25, 45), 2.5f, 255, true);
	light->setGradientColor(ccc4(95, 255, 255, 255), ccc4(50, 155, 255, 255), ccp(0,-1));
//	CommonAnimation::applyBigShadow(light, light->getFontSize());
	light->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
	light->setPosition(ccpFromSize(ment->getContentSize()/2.f));
	light->setOpacity(0);
	ment->addChild(light);
	
	function<void(function<void()>)> lighting_func = [=](function<void()> end_func)
	{
		light->addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
													  {
														  light->setOpacity(255*t);
													  }, [=](float t)
													  {
														  light->setOpacity(255);
														  light->addChild(KSTimer::create(2.f/30.f, [=]()
																						  {
																							  end_func();
																						  }));
													  }));
	};
	
	light->addChild(KSTimer::create(8.f/30.f, [=]()
									{
										lighting_func([=]()
													  {
														  lighting_func([=]()
																		{
																			lighting_func([=]()
																						  {
																							  lighting_func([=]()
																											{
																												lighting_func([=]()
																															  {
																																  lighting_func([=]()
																																				{
																																					light->removeFromParent();
																																				});
																															  });
																											});
																						  });
																		});
													  });
									}));
	
	function<void(CCSprite* t_target)> show_ani = [=](CCSprite* t_target)
	{
		t_target->addChild(KSGradualValue<float>::create(0.f, 1.f, 7.f/30.f, [=](float t)
														 {
															 t_target->setScale(1.6f-0.6f*t);
															 t_target->setOpacity(255*t);
														 }, [=](float t)
														 {
															 t_target->setScale(1.6f-0.6f*t);
															 t_target->setOpacity(255*t);
														 }));
	};
	
	function<void(CCSprite* t_target)> pumping_ani = [=](CCSprite* t_target)
	{
		t_target->addChild(KSGradualValue<float>::create(0.f, 1.f, 5.f/30.f, [=](float t)
														 {
															 t_target->setScale(1.f+0.4f*t);
														 }, [=](float t)
														 {
															 t_target->setScale(1.4f);
															 t_target->addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
																											  {
																												  t_target->setScale(1.4f-0.4f*t);
																											  }, [=](float t)
																											  {
																												  t_target->setScale(1.f);
																											  }));
														 }));
	};
	
	function<void(CCSprite* t_target)> final_ani = [=](CCSprite* t_target)
	{
		t_target->addChild(KSGradualValue<float>::create(0.f, 1.f, 6.f/30.f, [=](float t)
														 {
															 t_target->setScale(1.f+0.4f*t);
															 t_target->setOpacity(255-255*t);
														 }, [=](float t)
														 {
															 t_target->setScale(1.f+0.4f*t);
															 t_target->setOpacity(255-255*t);
															 t_target->removeFromParent();
														 }));
	};
	
	function<void(CCSprite* t_target, float t_del)> change_ani = [=](CCSprite* t_target, float t_del)
	{
		show_ani(t_target);
		
		t_target->addChild(KSTimer::create(t_del, [=]()
										   {
											   pumping_ani(t_target);
										   }));
		
		t_target->addChild(KSTimer::create(31.f/30.f, [=]()
										   {
											   final_ani(t_target);
										   }));
	};
	
	CCSprite* change_c = CCSprite::create("ui_game_change_c.png");
	change_c->setPosition(ccp(-85,-16));
	change_c->setScale(1.6f);
	change_c->setOpacity(0);
	take_coin_node->addChild(change_c);
	
	change_ani(change_c, 9.f/30.f);
	
	
	CCSprite* change_h = CCSprite::create("ui_game_change_h.png");
	change_h->setPosition(ccp(-51,-16));
	change_h->setScale(1.6f);
	change_h->setOpacity(0);
	take_coin_node->addChild(change_h);
	
	change_ani(change_h, 12.f/30.f);
	
	
	CCSprite* change_a = CCSprite::create("ui_game_change_a.png");
	change_a->setPosition(ccp(-17,-16));
	change_a->setScale(1.6f);
	change_a->setOpacity(0);
	take_coin_node->addChild(change_a);
	
	change_ani(change_a, 15.f/30.f);
	
	
	CCSprite* change_n = CCSprite::create("ui_game_change_n.png");
	change_n->setPosition(ccp(17,-16));
	change_n->setScale(1.6f);
	change_n->setOpacity(0);
	take_coin_node->addChild(change_n);
	
	change_ani(change_n, 18.f/30.f);
	
	
	CCSprite* change_g = CCSprite::create("ui_game_change_g.png");
	change_g->setPosition(ccp(51,-16));
	change_g->setScale(1.6f);
	change_g->setOpacity(0);
	take_coin_node->addChild(change_g);
	
	change_ani(change_g, 21.f/30.f);
	
	
	CCSprite* change_e = CCSprite::create("ui_game_change_e.png");
	change_e->setPosition(ccp(85,-16));
	change_e->setScale(1.6f);
	change_e->setOpacity(0);
	take_coin_node->addChild(change_e);
	
	change_ani(change_e, 24.f/30.f);
	
	
	
//	CCSprite* take_coin = KS::loadCCBI<CCSprite*>(this, CCString::createWithFormat("ui_change_%s.ccbi", myLoc->getLocalCode()->getCString())->getCString()).first;
//	addChild(take_coin);
//	
//	CCDelayTime* t_delay = CCDelayTime::create(3.f);
//	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(CCNode::removeFromParent));
//	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
//	runAction(t_seq);
}
void TakeCoin::myInit ()
{
	setPosition(ccp(240,myDSH->ui_center_y));
	
	schedule(schedule_selector(TakeCoin::startMyAction));
}
AreaScroll * AreaScroll::create ()
{
	AreaScroll* t_w = new AreaScroll();
	t_w->myInit();
	t_w->autorelease();
	return t_w;
}
void AreaScroll::startAction ()
{
	KSLabelTTF* main_view = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_unlockedAreaScroll), mySGD->getFont().c_str(), 25);
	main_view->enableOuterStroke(ccc3(0, 45, 10), 2.5f, 255, true);
	main_view->setGradientColor(ccc4(240, 255, 10, 255), ccc4(110, 190, 5, 255), ccp(0,-1));
	CommonAnimation::applyBigShadow(main_view, main_view->getFontSize());
	main_view->setPosition(ccp(640,myDSH->ui_center_y));
	addChild(main_view);
	
//	CCSprite* main_view = CCSprite::create(CCString::createWithFormat("show_area_scroll_%s.png", myLoc->getLocalCode()->getCString())->getCString());
//	main_view->setPosition(ccp(640,myDSH->ui_center_y));
//	addChild(main_view);
	
	CCMoveTo* t_move1 = CCMoveTo::create(0.4f, ccp(240,myDSH->ui_center_y));
	CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(AreaScroll::showArrow));
	CCHide* t_hide = CCHide::create();
	CCDelayTime* t_delay1 = CCDelayTime::create(0.05f);
	CCShow* t_show = CCShow::create();
	CCDelayTime* t_delay2 = CCDelayTime::create(0.1f);
	CCRepeat* t_repeat = CCRepeat::create(CCSequence::create(t_hide, t_delay1, t_show, t_delay2, NULL), 4);
	CCMoveTo* t_move2 = CCMoveTo::create(0.4f, ccp(-160,myDSH->ui_center_y));
	CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(AreaScroll::selfRemove));
	
	main_view->runAction(CCSequence::create(t_move1, t_call1, t_repeat, t_move2, t_call2, NULL));
}
void AreaScroll::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void AreaScroll::showArrow ()
{
	for(int i=0;i<8;i++)
	{
		CCSprite* t_down = CCSprite::create("area_scroll_down.png");
		t_down->setPosition(ccp(65+i*50, myDSH->ui_center_y-30));
		t_down->setOpacity(0);
		addChild(t_down);
		
		CCFadeTo* down_fade = CCFadeTo::create(0.5f, 255);
		CCMoveBy* down_move = CCMoveBy::create(0.7f, ccp(0,-250));
		CCSequence* down_seq = CCSequence::createWithTwoActions(down_fade, down_move);
		
		t_down->runAction(down_seq);
		
		
		CCSprite* t_up = CCSprite::create("area_scroll_up.png");
		t_up->setPosition(ccp(65+i*50, myDSH->ui_center_y+30));
		t_up->setOpacity(0);
		addChild(t_up);
		
		CCFadeTo* up_fade = CCFadeTo::create(0.5f, 255);
		CCMoveBy* up_move = CCMoveBy::create(0.7f, ccp(0,250));
		CCSequence* up_seq = CCSequence::createWithTwoActions(up_fade, up_move);
		
		t_up->runAction(up_seq);
	}
}
void AreaScroll::myInit ()
{
}
ChangeCard * ChangeCard::create ()
{
	ChangeCard* t_w = new ChangeCard();
	t_w->myInit();
	t_w->autorelease();
	return t_w;
}
void ChangeCard::startMyAction()
{
	unschedule(schedule_selector(ChangeCard::startMyAction));
	
	KSLabelTTF* card_change_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_cardChange), mySGD->getFont().c_str(), 45);
	card_change_label->enableOuterStroke(ccBLACK, 2.5f, int(255*0.3), true);
	card_change_label->setGradientColor(ccc4(255, 155, 255, 255), ccc4(145, 45, 215, 255), ccp(0,-1));
	KSLabelTTF* shadow = CommonAnimation::applyBigShadow(card_change_label, card_change_label->getFontSize());
	card_change_label->setPosition(ccp(0,30));
	card_change_label->setScale(1.5f);
	card_change_label->setOpacity(0);
	shadow->setOpacityOuterStroke(0);
	addChild(card_change_label);
	
	card_change_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
															  {
																  card_change_label->setPosition(ccp(0,30-30*t));
																  card_change_label->setScale(1.5f-0.5f*t);
																  card_change_label->setOpacity(255*t);
																  shadow->setOpacityOuterStroke(255 * 0.3f*(t));
															  }, [=](float t)
															  {
																  card_change_label->setPosition(ccp(0,20));
																  card_change_label->setScale(1.f);
																  card_change_label->setOpacity(255);
																  shadow->setOpacityOuterStroke(255 * 0.3f*(t));
																  
																  card_change_label->addChild(KSTimer::create(22.f/30.f, [=]()
																											  {
																												  card_change_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 5.f/30.f, [=](float t)
																																											{
																																												card_change_label->setPosition(ccp(0,30*t));
																																												card_change_label->setScale(1.f+0.6f*t);
																																												card_change_label->setOpacity(255-255*t);
																																												shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																											}, [=](float t)
																																											{
																																												card_change_label->setPosition(ccp(0,30*t));
																																												card_change_label->setScale(1.f+0.6f*t);
																																												card_change_label->setOpacity(255-255*t);
																																												shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																												removeFromParent();
																																											}));
																											  }));
															  }));
	
	KSLabelTTF* light = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_cardChange), mySGD->getFont().c_str(), 45);
	light->enableOuterStroke(ccBLACK, 2.5f, int(255*0.3), true);
	light->setGradientColor(ccc4(255, 155, 255, 255), ccc4(145, 45, 215, 255), ccp(0,-1));
//	CommonAnimation::applyBigShadow(card_change_label, card_change_label->getFontSize());
	light->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
	light->setOpacity(0);
	light->setPosition(ccpFromSize(card_change_label->getContentSize()/2.f));
	card_change_label->addChild(light);
	
	function<void(function<void()>)> lighting_func = [=](function<void()> end_func)
	{
		light->addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
													  {
														  light->setOpacity(255*t);
													  }, [=](float t)
													  {
														  light->setOpacity(255*t);
														  light->addChild(KSTimer::create(2.f/30.f, [=]()
																						  {
																							  end_func();
																						  }));
													  }));
	};
	
	lighting_func([=]()
				  {
					  lighting_func([=]()
									{
										lighting_func([=]()
													  {
														  lighting_func([=]()
																		{
																			light->removeFromParent();
																		});
													  });
									});
				  });
	
	
//	CCSprite* change_card = KS::loadCCBI<CCSprite*>(this, "ui_cardchange.ccbi").first;
//	addChild(change_card);
//	
//	CCDelayTime* t_delay = CCDelayTime::create(1.2f);
//	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(CCNode::removeFromParent));
//	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
//	runAction(t_seq);
}
void ChangeCard::myInit ()
{
	setPosition(ccp(240,myDSH->ui_center_y));
	
	schedule(schedule_selector(ChangeCard::startMyAction));
}
const float UI_IN_TIME = 0.5f;
const float UI_OUT_DISTANCE = 100.f;
PlayUI * PlayUI::create ()
{
	PlayUI* t_ui = new PlayUI();
	t_ui->myInit();
	t_ui->autorelease();
	return t_ui;
}
PlayUI::~ PlayUI ()
{
	if(jack_array)
	{
		jack_array->removeAllObjects();
		jack_array->release();
		jack_array = NULL;
	}
	if(exchange_dic)
	{
		exchange_dic->removeAllObjects();
		exchange_dic->release();
		exchange_dic = NULL;
	}
}
void PlayUI::addScore (int t_score, int t_sub_score)
{
	score_value = score_value.getV() + t_score;
	sub_score_value = sub_score_value.getV() + t_sub_score;
//	CCLOG("damaged_score : %d / score_value : %.0f", damaged_score.getV(), score_value.getV());
	score_label->setString(CCString::createWithFormat("%d", damaged_score.getV() + int(score_value.getV()))->getCString());
	if(sub_score_value.getV() > 0)
		sub_score_label->setString(ccsf("+%s", KS::insert_separator(sub_score_value.getV()).c_str()));
	
	if(clr_cdt_type == kCLEAR_score && !is_cleared_cdt)
	{
		if(getScore() >= clr_cdt_cnt.getV())
		{
			conditionClear();
		}
	}
//		checkScoreMission();
	
	if(mySGD->is_write_replay)
	{
		if(mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreTime)].size() > 0)
			myGD->communication("UI_checkScoreTimeVector");
		else
			myGD->communication("UI_writeScore");
	}
}

void PlayUI::decreasePercentages(int t_cnt)
{
	for(int i=0;i<t_cnt;i++)
		decreasePercentage();
//	percentage_decrease_cnt+=t_cnt;
//	if(percentage_decrease_cnt >= mySD->must_cnt/1000.f)
//	{
//		int d_decrease = floor(percentage_decrease_cnt/(mySD->must_cnt/1000.f));
//		
//		beforePercentage ^= t_tta;
//		beforePercentage -= d_decrease;
//		percentageLabel->setString(CCString::createWithFormat("%.1f%%", beforePercentage/10.f)->getCString());
//		if(m_areaGage)
//			m_areaGage->setPercentage(beforePercentage/1000.f);
//		beforePercentage ^= t_tta;
//		percentage_decrease_cnt -= floor(d_decrease*(mySD->must_cnt/1000.f));
//	}
}

void PlayUI::decreasePercentage ()
{
	percentage_decrease_cnt++;
	if(percentage_decrease_cnt >= mySD->must_cnt/1000.f)
	{
		beforePercentage ^= t_tta;
		beforePercentage -= 1;
		percentageLabel->setString(CCString::createWithFormat("%.1f%%", beforePercentage/10.f)->getCString());
		if(m_areaGage)
			m_areaGage->setPercentage(beforePercentage/1000.f);
		beforePercentage ^= t_tta;
		percentage_decrease_cnt = 0;
	}
}
float PlayUI::getScore ()
{
	return score_value.getV() + sub_score_value.getV();
}
float PlayUI::getPercentage ()
{
	return (beforePercentage^t_tta)/1000.f;
}
void PlayUI::setPercentage (float t_p, bool t_b)
{
	float last_get_percentage = 0;
	float t_t_beforePercentage = 0;
	if(isFirst)
	{
		isFirst = false;
		beforePercentage = (int(t_p*1000))^t_tta;
	}
	else
	{
		//			t_p = 0.99f;
		myGD->communication("CP_changeMaxSize", t_p);
		float t_beforePercentage = (beforePercentage^t_tta)/1000.f;
		t_t_beforePercentage = t_beforePercentage;
		
		bool is_five_percent = false;
		
		if(t_b)
		{
//			AudioEngine::sharedInstance()->playEffect("sound_jack_basic_missile_shoot.mp3", false);
			myLog->addLog(kLOG_getPercent_f, myGD->getCommunication("UI_getUseTime"), t_p-t_beforePercentage);
			
			if(t_p >= t_beforePercentage + 0.001f)
			{
				IntPoint jackPoint = myGD->getJackPoint();
				CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize + 1, (jackPoint.y-1)*pixelSize + 1);
				
//				myGD->communication("Main_goldGettingEffect", jackPosition, int(floorf((t_p-t_beforePercentage)*200.f)));
				
				last_get_percentage = t_p-t_beforePercentage;
				
				if(clr_cdt_type == kCLEAR_bigArea || clr_cdt_type == kCLEAR_perfect)
				{
					myGD->communication("Main_percentageGettingEffect", floorf((t_p-t_beforePercentage)*10000.f)/10000.f*100.f, true, jackPosition);
				}
			}
			
			if(t_p >= t_beforePercentage + 0.05f)
			{
				is_five_percent = true;
				int random_value = rand()%9 + 1;
				AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("groan%d.mp3", random_value)->getCString(), false);
			}
			else
			{
				AudioEngine::sharedInstance()->playEffect("se_area.mp3", false, true);
			}
		}
		
		{
			int i = kAchievementCode_luckySeven1;
			
			if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
			   int(round(t_p*1000)) == int(round(t_beforePercentage*1000)) + myAchieve->getCondition((AchievementCode)i))
			{
				myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
		{
			int i = kAchievementCode_luckySeven2;
			
			if(myAchieve->isCompleted(kAchievementCode_luckySeven1) && !myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
			   int(round(t_p*1000)) == int(round(t_beforePercentage*1000)) + myAchieve->getCondition((AchievementCode)i))
			{
				myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
		{
			int i = kAchievementCode_luckySeven3;
			
			if(myAchieve->isCompleted(kAchievementCode_luckySeven1) && myAchieve->isCompleted(kAchievementCode_luckySeven2) && !myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
			   int(round(t_p*1000)) == int(round(t_beforePercentage*1000)) + myAchieve->getCondition((AchievementCode)i))
			{
				myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
		
		for(int i=kAchievementCode_hidden_bigHand1;i<=kAchievementCode_hidden_bigHand2;i++)
		{
			if(!myAchieve->isCompleted(AchievementCode(i)) && !myAchieve->isAchieve(AchievementCode(i)))
			{
				if(!myAchieve->isNoti(AchievementCode(i)) && t_p >= t_beforePercentage + myAchieve->getCondition(AchievementCode(i))*0.01f)
				{
					myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition(AchievementCode(i)));
					AchieveNoti* t_noti = AchieveNoti::create(AchievementCode(i));
					CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
				}
			}
		}
		
		if(clr_cdt_type == kCLEAR_bigArea && !is_cleared_cdt && t_p - t_beforePercentage >= clr_cdt_per)
			takeBigArea();
		
		mySGD->is_draw_button_tutorial = false;
		myGD->communication("Main_offDrawButtonTutorial");
		
		if(t_p >= t_beforePercentage + 0.01f && t_p < clearPercentage.getV())
		{
			int up_count = (t_p - t_beforePercentage)/0.01f;
			my_fp->addFeverGage(up_count);
		}
		
		if(clr_cdt_type == kCLEAR_littlePercent && t_p*100.f >= t_beforePercentage*100.f + clr_cdt_cnt.getV())
		{
			if(t_b && t_p >= t_t_beforePercentage + 0.001f)
				myGD->communication("GIM_showPercentFloatingCoin", last_get_percentage);
			
			conditionFail();
			
			mySGD->fail_code = kFC_missionfail;
			
			stopCounting();
			int boss_count = myGD->getMainCumberCount();
			for(int i=0;i<boss_count;i++)
			{
				myGD->communication("MP_bombCumber", myGD->getMainCumberCCNodeVector()[i]);
			}
			// timeover
			isGameover = true;
			myGD->communication("CP_setGameover");
			myGD->removeAllPattern();
			myGD->communication("Main_allStopSchedule");
			AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
			
			addResultCCB("ui_missonfail.ccbi");
			AudioEngine::sharedInstance()->playEffect("ment_mission_fail.mp3", false, true);
			
			endGame(false);
			
			myGD->setIsGameover(true); // ks 가 추가함. 타이밍을 뒤로 ...
			return;
		}
		
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		if(t_p >= t_beforePercentage + NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_percent_d, t_history.characterIndex.getV(), t_history.characterLevel.getV())/100.f)
		{
			if(!is_five_percent)
				AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("ment_attack%d.mp3", rand()%4+1)->getCString(), false, true);
			
			float cmCnt = (t_p - t_beforePercentage)/(NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_percent_d, t_history.characterIndex.getV(), t_history.characterLevel.getV())/100.f);
			Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
			int weapon_type = mInfo.get("type", 0).asInt();
			int weapon_level = mySGD->getUserdataCharLevel();
			
//			int weapon_rank = (weapon_level-1)/5 + 1;
//			weapon_level = (weapon_level-1)%5 + 1;
			
			double power_rate = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_power_d, t_history.characterIndex.getV(), t_history.characterLevel.getV());
			if(power_rate < 1.0)
				power_rate = 1.0;
			
			myGD->createJackMissileWithStoneFunctor((StoneType)weapon_type, weapon_level, cmCnt * 2, myGD->getJackPoint().convertToCCP(), mySGD->getUserdataMissileInfoPower(),
													int((power_rate-1.0)*mySGD->getUserdataMissileInfoPower()));
		}
		
		if(!is_exchanged && !is_show_exchange_coin && !isGameover && t_p < clearPercentage.getV())
		{
			if(t_p >= t_beforePercentage + 0.15f) // 0.2
			{
				takeCoinModeOn();
			}
			else if(t_p >= t_beforePercentage + 0.1f)
			{
				if(rand()%10 < 7) // 70%
					takeCoinModeOn();
			}
			else if(t_p >= t_beforePercentage + 0.08f)
			{
				if(rand()%2 == 0) // 50%
					takeCoinModeOn();
			}
			else if(t_p >= t_beforePercentage + 0.06f)
			{
				if(rand()%20 < 7) // 35%
					takeCoinModeOn();
			}
			else if(t_p >= t_beforePercentage + 0.04f)
			{
				if(rand()%5 == 0) // 20%
					takeCoinModeOn();
			}
		}
		
		beforePercentage = (int(t_p*1000))^t_tta;
	}
	
	if(t_p > 0.35f && !is_show_exchange_coin && t_p < clearPercentage.getV())
	{
		takeCoinModeOn();
	}
	
	percentageLabel->setString(CCString::createWithFormat("%.1f%%", floorf(t_p*1000)/10.f)->getCString());
	
	if(clr_cdt_type == kCLEAR_perfect && !isGameover && !is_cleared_cdt && floorf(t_p*10000.f)/10000.f*100.f >= clr_cdt_per*100.f &&
	   floorf(t_p*10000.f)/10000.f*100.f <= (clr_cdt_per+clr_cdt_range)*100.f)
		conditionClear();
	if(clr_cdt_type == kCLEAR_percentage && !isGameover && !is_cleared_cdt && t_p >= clearPercentage.getV())
		conditionClear();
	
	if(m_areaGage)
		m_areaGage->setPercentage(t_p);
	percentage_decrease_cnt = 0;
	
//	CCLog("isTimeEventClear : %d, is_on_clear_time_event : %d, isGameover : %d, clearPercentage : %f, timeEventValue : %f, t_p : %f", mySGD->isTimeEvent(kTimeEventType_clear), is_on_clear_time_event, isGameover, clearPercentage.getV(), mySGD->getTimeEventFloatValue(kTimeEventType_clear)/100.f, t_p);
	
	if(clr_cdt_type != kCLEAR_percentage && mySGD->isTimeEvent(kTimeEventType_clear) && !is_on_clear_time_event && !isGameover && clearPercentage.getV() == mySGD->getTimeEventFloatValue(kTimeEventType_clear)/100.f && t_p > mySGD->getTimeEventFloatValue(kTimeEventType_clear)/100.f && t_p <= 0.85f)
	{
		is_on_clear_time_event = true;
		clear_time_event_func([=]()
							  {
								  // no
								  clearPercentage = 0.85f;
								  if(t_b && t_p >= t_t_beforePercentage + 0.001f)
									  myGD->communication("GIM_showPercentFloatingCoin", last_get_percentage);
							  }, [=]()
							  {
								  // yes
								  if(!is_exchanged && is_show_exchange_coin && myGD->getCommunicationBool("MS_isCheckBossLocked") && myGD->getCommunicationBool("GIM_isChangeAllInner"))
								  {
									  taked_coin_cnt = 6;
									  
									  for(int i=1;i<=6;i++)
									  {
										  CCSprite* t_coin_spr = (CCSprite*)exchange_dic->objectForKey(i);
										  CCPoint after_position = t_coin_spr->getPosition();
										  t_coin_spr->removeFromParentAndCleanup(true);
										  exchange_dic->removeObjectForKey(i);
										  
										  CCSprite* new_coin_spr = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", i)->getCString());
										  new_coin_spr->setPosition(after_position);
										  top_center_node->addChild(new_coin_spr);
										  
										  
										  exchange_dic->setObject(new_coin_spr, i);
									  }
									  
									  if(clr_cdt_type == kCLEAR_sequenceChange)
									  {
										  conditionClear();
									  }
									  
									  isFirst = true;
									  is_exchanged = true;
									  my_fp->addFeverGage(20);
									  
									  myGD->communication("Main_startExchange");
									  myGD->communication("Main_showChangeCard");
									  myGD->communication("Jack_positionRefresh");
									  
									  m_areaGage->onChange();
									  
									  return;
								  }
								  
								  myGD->communication("GIM_stopCoin");
								  
								  if(clr_cdt_type == kCLEAR_timeLimit)
								  {
									  //			if(playtime_limit.getV() - countingCnt.getV() >= ing_cdt_cnt.getV())
									  conditionClear();
									  //			else
									  //				conditionFail();
								  }
								  else if(clr_cdt_type == kCLEAR_default)
									  conditionClear();
								  
								  else if(clr_cdt_type == kCLEAR_score)
									{
										checkScoreMission();
									}
								  else if(clr_cdt_type == kCLEAR_turns || clr_cdt_type == kCLEAR_littlePercent)
									  conditionClear();
								  
								  if(is_cleared_cdt)
								  {
									  int boss_count = myGD->getMainCumberCount();
									  for(int i=0;i<boss_count;i++)
									  {
										  myGD->communication("MP_bombCumber", myGD->getMainCumberCCNodeVector()[i]);
									  }
									  
									  isGameover = true;
									  myGD->setIsGameover(true);
									  myGD->communication("CP_setGameover");
										myGD->removeAllPattern();
									  stopCounting();
									  myGD->communication("Main_allStopSchedule");
									  myGD->communication("Main_startMoveToBossPosition");
									  myGD->communication("CP_startDieAnimation");
									  AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
									  
									  CCDelayTime* t_delay = CCDelayTime::create(1.5f);
									  CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PlayUI::addResultClearCCB));
									  CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
									  runAction(t_seq);
									  
									  endGame(t_p < 1.f && t_p > 0.99f);
									  
									  for(int i=kAchievementCode_fastClear1;i<=kAchievementCode_fastClear3;i++)
									  {
										  if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) &&
											 use_time <= myAchieve->getCondition((AchievementCode)i))
										  {
											  myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition(AchievementCode(i)));
											  AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
											  CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
										  }
									  }
									  
									  for(int i=kAchievementCode_hidden_breathtaking1;i<=kAchievementCode_hidden_breathtaking2;i++)
									  {
										  if(!myAchieve->isCompleted(AchievementCode(i)) && !myAchieve->isAchieve(AchievementCode(i)))
										  {
											  if(!myAchieve->isNoti(AchievementCode(i)) && playtime_limit.getV() - countingCnt.getV() <= myAchieve->getCondition(AchievementCode(i)))
											  {
												  myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition(AchievementCode(i)));
												  AchieveNoti* t_noti = AchieveNoti::create(AchievementCode(i));
												  CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
											  }
										  }
									  }
									  
									  myGD->communication("Main_startClearFloatingCoin", last_get_percentage);
								  }
								  else
								  {
									  if(t_b && t_p >= t_t_beforePercentage + 0.001f)
										  myGD->communication("GIM_showPercentFloatingCoin", last_get_percentage);
									  
									  conditionFail();
									  
									  mySGD->fail_code = kFC_missionfail;
									  
									  stopCounting();
									  // timeover
									  isGameover = true;
									  myGD->communication("CP_setGameover");
										myGD->removeAllPattern();
									  myGD->communication("Main_allStopSchedule");
									  AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
									  addResultCCB("ui_missonfail.ccbi");
									  AudioEngine::sharedInstance()->playEffect("ment_mission_fail.mp3", false, true);
									  
									  endGame(false);
								  }
							  });
	}
	else if(!isGameover && t_p > clearPercentage.getV()) // clear 80%
	{
		if(!is_exchanged && is_show_exchange_coin && myGD->getCommunicationBool("MS_isCheckBossLocked") && myGD->getCommunicationBool("GIM_isChangeAllInner"))
		{
			taked_coin_cnt = 6;
			
			for(int i=1;i<=6;i++)
			{
				CCSprite* t_coin_spr = (CCSprite*)exchange_dic->objectForKey(i);
				CCPoint after_position = t_coin_spr->getPosition();
				t_coin_spr->removeFromParentAndCleanup(true);
				exchange_dic->removeObjectForKey(i);
				
				CCSprite* new_coin_spr = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", i)->getCString());
				new_coin_spr->setPosition(after_position);
				top_center_node->addChild(new_coin_spr);
				
				
				exchange_dic->setObject(new_coin_spr, i);
			}
			
			if(clr_cdt_type == kCLEAR_sequenceChange)
			{
				conditionClear();
			}
				
			isFirst = true;
			is_exchanged = true;
			my_fp->addFeverGage(20);
			
			myGD->communication("Main_startExchange");
			myGD->communication("Main_showChangeCard");
			myGD->communication("Jack_positionRefresh");
				
			m_areaGage->onChange();
			
			return;
		}
		
		myGD->communication("GIM_stopCoin");
		
		if(clr_cdt_type == kCLEAR_timeLimit)
		{
//			if(playtime_limit.getV() - countingCnt.getV() >= ing_cdt_cnt.getV())
				conditionClear();
//			else
//				conditionFail();
		}
		else if(clr_cdt_type == kCLEAR_default)
			conditionClear();
		else if(clr_cdt_type == kCLEAR_score)
		{
			checkScoreMission();
		}
		else if(clr_cdt_type == kCLEAR_turns || clr_cdt_type == kCLEAR_littlePercent)
			conditionClear();
		
		if(is_cleared_cdt)
		{
			int boss_count = myGD->getMainCumberCount();
			for(int i=0;i<boss_count;i++)
			{
				myGD->communication("MP_bombCumber", myGD->getMainCumberCCNodeVector()[i]);
			}
			
			isGameover = true;
			myGD->setIsGameover(true);
			myGD->communication("CP_setGameover");
			myGD->removeAllPattern();
			stopCounting();
			myGD->communication("Main_allStopSchedule");
			myGD->communication("Main_startMoveToBossPosition");
			myGD->communication("CP_startDieAnimation");
			AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
			
			CCDelayTime* t_delay = CCDelayTime::create(1.5f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PlayUI::addResultClearCCB));
			CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
			runAction(t_seq);
			
			endGame(t_p < 1.f && t_p > 0.99f);
			
			for(int i=kAchievementCode_fastClear1;i<=kAchievementCode_fastClear3;i++)
			{
				if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) &&
				   use_time <= myAchieve->getCondition((AchievementCode)i))
				{
					myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition(AchievementCode(i)));
					AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
					CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
				}
			}
			
			for(int i=kAchievementCode_hidden_breathtaking1;i<=kAchievementCode_hidden_breathtaking2;i++)
			{
				if(!myAchieve->isCompleted(AchievementCode(i)) && !myAchieve->isAchieve(AchievementCode(i)))
				{
					if(!myAchieve->isNoti(AchievementCode(i)) && playtime_limit.getV() - countingCnt.getV() <= myAchieve->getCondition(AchievementCode(i)))
					{
						myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition(AchievementCode(i)));
						AchieveNoti* t_noti = AchieveNoti::create(AchievementCode(i));
						CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
					}
				}
			}
			
			myGD->communication("Main_startClearFloatingCoin", last_get_percentage);
		}
		else
		{
			if(t_b && t_p >= t_t_beforePercentage + 0.001f)
				myGD->communication("GIM_showPercentFloatingCoin", last_get_percentage);
			
			conditionFail();
			
			mySGD->fail_code = kFC_missionfail;
			
			stopCounting();
			int boss_count = myGD->getMainCumberCount();
			for(int i=0;i<boss_count;i++)
			{
				myGD->communication("MP_bombCumber", myGD->getMainCumberCCNodeVector()[i]);
			}
			// timeover
			isGameover = true;
			myGD->communication("CP_setGameover");
			myGD->removeAllPattern();
			myGD->communication("Main_allStopSchedule");
			AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
			
			addResultCCB("ui_missonfail.ccbi");
			AudioEngine::sharedInstance()->playEffect("ment_mission_fail.mp3", false, true);
			
			endGame(false);
			
			myGD->setIsGameover(true); // ks 가 추가함. 타이밍을 뒤로 ...
		}
	}
	else
	{
		if(t_b && t_p >= t_t_beforePercentage + 0.001f)
			myGD->communication("GIM_showPercentFloatingCoin", last_get_percentage);
	}
}

void PlayUI::hellModeResult()
{
	if(is_cleared_cdt)
	{
		myGD->communication("GIM_stopCoin");
		
		int boss_count = myGD->getMainCumberCount();
		for(int i=0;i<boss_count;i++)
		{
			myGD->communication("MP_bombCumber", myGD->getMainCumberCCNodeVector()[i]);
		}
		
		isGameover = true;
		myGD->setIsGameover(true);
		myGD->communication("CP_setGameover");
		myGD->removeAllPattern();
		stopCounting();
		myGD->communication("Main_allStopSchedule");
		myGD->communication("Main_startMoveToBossPosition");
		myGD->communication("CP_startDieAnimation");
//		AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
		
//		addResultCCB("ui_gameover.ccbi");
//		AudioEngine::sharedInstance()->playEffect("ment_gameover3.mp3", false, true);
		
		endGame(false);
		
//		for(int i=kAchievementCode_fastClear1;i<=kAchievementCode_fastClear3;i++)
//		{
//			if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) &&
//			   use_time <= myAchieve->getCondition((AchievementCode)i))
//			{
//				myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition(AchievementCode(i)));
//				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
//				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
//			}
//		}
		
//		for(int i=kAchievementCode_hidden_breathtaking1;i<=kAchievementCode_hidden_breathtaking2;i++)
//		{
//			if(!myAchieve->isCompleted(AchievementCode(i)) && !myAchieve->isAchieve(AchievementCode(i)))
//			{
//				if(!myAchieve->isNoti(AchievementCode(i)) && playtime_limit.getV() - countingCnt.getV() <= myAchieve->getCondition(AchievementCode(i)))
//				{
//					myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition(AchievementCode(i)));
//					AchieveNoti* t_noti = AchieveNoti::create(AchievementCode(i));
//					CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
//				}
//			}
//		}
		
//		myGD->communication("Main_startClearFloatingCoin", last_get_percentage);
	}
	else
	{
		conditionFail();
		
		mySGD->fail_code = kFC_missionfail;
		
		stopCounting();
		// timeover
		isGameover = true;
		myGD->communication("CP_setGameover");
		myGD->removeAllPattern();
		myGD->communication("Main_allStopSchedule");
//		AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
		
//		addResultCCB("ui_missonfail.ccbi");
//		AudioEngine::sharedInstance()->playEffect("ment_mission_fail.mp3", false, true);
		
		endGame(false);
	}
}

void PlayUI::addResultClearCCB()
{
	addResultCCB("ui_stageclear.ccbi");
	
	
	AudioEngine::sharedInstance()->stopSound();
	
	//AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("ment_gameover%d.mp3", rand()%3+1)->getCString(), false, true);
	
	AudioEngine::sharedInstance()->playEffect("bgm_gameclear.mp3");
	//AudioEngine::sharedInstance()->playEffect("ment_stageclear1.mp3", false, true);
}

void PlayUI::addResultCCB(string ccb_filename)
{
	if(ccb_filename == "ui_missonfail.ccbi")
	{
		KSLabelTTF* mission_fail_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_failTitleMissionfail), mySGD->getFont().c_str(), 45);
		mission_fail_label->setGradientColor(ccc4(255, 115, 250, 255), ccc4(215, 60, 130, 255), ccp(0,-1));
		mission_fail_label->enableOuterStroke(ccc3(65, 5, 35), 2.5f, 255, true);
		mission_fail_label->setPosition(ccp(240,myDSH->ui_center_y+93));
		mission_fail_label->setOpacity(0);
		addChild(mission_fail_label);
        
        StyledLabelTTF* context_label = NULL;
        
        if(clr_cdt_type == CLEAR_CONDITION::kCLEAR_combo)
        {
            context_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionFailContextCombo), clr_cdt_cnt.getV() - high_combo_cnt), mySGD->getFont().c_str(), 25, 999, StyledAlignment::kCenterAlignment);
            context_label->setAnchorPoint(ccp(0.5f,0.5f));
            context_label->setPosition(ccpFromSize(mission_fail_label->getContentSize()/2.f) + ccp(0,-43));
            mission_fail_label->addChild(context_label);
        }
        else if(clr_cdt_type == CLEAR_CONDITION::kCLEAR_gold)
        {
            context_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionFailContextGold), clr_cdt_cnt.getV() - mySGD->getStageGold()), mySGD->getFont().c_str(), 25, 999, StyledAlignment::kCenterAlignment);
            context_label->setAnchorPoint(ccp(0.5f,0.5f));
            context_label->setPosition(ccpFromSize(mission_fail_label->getContentSize()/2.f) + ccp(0,-43));
            mission_fail_label->addChild(context_label);
        }
        else if(clr_cdt_type == CLEAR_CONDITION::kCLEAR_itemCollect)
        {
            context_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionFailContextItemCollect), clr_cdt_cnt.getV() - ing_cdt_cnt.getV()), mySGD->getFont().c_str(), 25, 999, StyledAlignment::kCenterAlignment);
            context_label->setAnchorPoint(ccp(0.5f,0.5f));
            context_label->setPosition(ccpFromSize(mission_fail_label->getContentSize()/2.f) + ccp(0,-43));
            mission_fail_label->addChild(context_label);
        }
        else if(clr_cdt_type == CLEAR_CONDITION::kCLEAR_score)
        {
            int total_score;
            
            int grade_value = 1;
            
            if(is_exchanged && (beforePercentage^t_tta)/1000.f >= 1.f)		grade_value = 4;
            else if(is_exchanged)											grade_value = 3;
            else if((beforePercentage^t_tta)/1000.f >= 1.f)					grade_value = 2;
            
            int recent_score = getScore();
            
            float t_game_time = countingCnt.getV();
            float play_limit_time = NSDS_GI(mySD->getSilType(), kSDS_SI_playtime_i);
            if(clr_cdt_type == kCLEAR_timeLimit)
            {
                play_limit_time -= mySD->getClearConditionTimeLimit();
            }
            
            int time_score = ((play_limit_time-t_game_time)*500*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d));
            
            total_score = recent_score + time_score;
            total_score = total_score*grade_value;
            
            context_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionFailContextScore), clr_cdt_cnt.getV() - total_score), mySGD->getFont().c_str(), 25, 999, StyledAlignment::kCenterAlignment);
            context_label->setAnchorPoint(ccp(0.5f,0.5f));
            context_label->setPosition(ccpFromSize(mission_fail_label->getContentSize()/2.f) + ccp(0,-43));
            mission_fail_label->addChild(context_label);
        }
        else if(clr_cdt_type == CLEAR_CONDITION::kCLEAR_subCumberCatch)
        {
            context_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionFailContextSubCumberCatch), clr_cdt_cnt.getV() - ing_cdt_cnt.getV()), mySGD->getFont().c_str(), 25, 999, StyledAlignment::kCenterAlignment);
            context_label->setAnchorPoint(ccp(0.5f,0.5f));
            context_label->setPosition(ccpFromSize(mission_fail_label->getContentSize()/2.f) + ccp(0,-43));
            mission_fail_label->addChild(context_label);
        }
        else if(clr_cdt_type == CLEAR_CONDITION::kCLEAR_turns)
        {
            context_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionFailContextTurns), clr_cdt_cnt.getV()), mySGD->getFont().c_str(), 25, 999, StyledAlignment::kCenterAlignment);
            context_label->setAnchorPoint(ccp(0.5f,0.5f));
            context_label->setPosition(ccpFromSize(mission_fail_label->getContentSize()/2.f) + ccp(0,-43));
            mission_fail_label->addChild(context_label);
        }
		else if(clr_cdt_type == CLEAR_CONDITION::kCLEAR_casting)
		{
			context_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionFailContextCasting), clr_cdt_cnt.getV() - casting_cancel_cnt.getV()), mySGD->getFont().c_str(), 25, 999, StyledAlignment::kCenterAlignment);
			context_label->setAnchorPoint(ccp(0.5f,0.5f));
			context_label->setPosition(ccpFromSize(mission_fail_label->getContentSize()/2.f) + ccp(0,-43));
			mission_fail_label->addChild(context_label);
		}
		else if(clr_cdt_type == CLEAR_CONDITION::kCLEAR_littlePercent)
		{
			context_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionFailContextLittlePercent), clr_cdt_cnt.getV()), mySGD->getFont().c_str(), 25, 999, StyledAlignment::kCenterAlignment);
			context_label->setAnchorPoint(ccp(0.5f,0.5f));
			context_label->setPosition(ccpFromSize(mission_fail_label->getContentSize()/2.f) + ccp(0,-43));
			mission_fail_label->addChild(context_label);
		}
		
		KSLabelTTF* shadow = CommonAnimation::applyBigShadow(mission_fail_label, mission_fail_label->getFontSize());
		shadow->setOpacityOuterStroke(0);
		
		mission_fail_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 13.f/30.f, [=](float t)
																   {
																	   float convert_t;
																	   if (t < 1 / 2.75)
																	   {
																		   t = 7.5625f * t * t;
																	   } else if (t < 2 / 2.75)
																	   {
																		   t -= 1.5f / 2.75f;
																		   t = 7.5625f * t * t + 0.75f;
																	   } else if(t < 2.5 / 2.75)
																	   {
																		   t -= 2.25f / 2.75f;
																		   t = 7.5625f * t * t + 0.9375f;
																	   }
																	   
																	   t -= 2.625f / 2.75f;
																	   convert_t = 7.5625f * t * t + 0.984375f;
																	   
																	   mission_fail_label->setPosition(ccp(240,myDSH->ui_center_y+93-93*convert_t));
																	   mission_fail_label->setOpacity(t*255);
																	   shadow->setOpacityOuterStroke(255 * 0.3f*(t));
																   }, [=](float t)
																   {
																	   mission_fail_label->setPosition(ccp(240,myDSH->ui_center_y));
																	   mission_fail_label->setOpacity(255);
																	   shadow->setOpacityOuterStroke(255 * 0.3f*(t));
																	   
																	   mission_fail_label->addChild(KSTimer::create(32.f/30.f, [=]()
																													{
																														mission_fail_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 5.f/30.f, [=](float t)
																																												   {
																																													   mission_fail_label->setScale(1.f+t*0.6f);
																																													   mission_fail_label->setOpacity(255-t*255);
																																													   shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																												   }, [=](float t)
																																												   {
																																													   mission_fail_label->setScale(1.6f);
																																													   mission_fail_label->setOpacity(0);
																																													   shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																													   
																																													   mission_fail_label->removeFromParent();
																																												   }));
																													}));
																   }));
	}
	else if(ccb_filename == "ui_gameover.ccbi")
	{
		KSLabelTTF* mission_fail_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_failTitleGameover), mySGD->getFont().c_str(), 45);
		mission_fail_label->setGradientColor(ccc4(255, 115, 250, 255), ccc4(215, 60, 130, 255), ccp(0,-1));
		mission_fail_label->enableOuterStroke(ccc3(65, 5, 35), 2.5f, 255, true);
		mission_fail_label->setPosition(ccp(240,myDSH->ui_center_y+93));
		mission_fail_label->setOpacity(0);
		addChild(mission_fail_label);
		
		KSLabelTTF* shadow = CommonAnimation::applyBigShadow(mission_fail_label, mission_fail_label->getFontSize());
		shadow->setOpacityOuterStroke(0);
		
		mission_fail_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 13.f/30.f, [=](float t)
																   {
																	   float convert_t;
																	   if (t < 1 / 2.75)
																	   {
																		   t = 7.5625f * t * t;
																	   } else if (t < 2 / 2.75)
																	   {
																		   t -= 1.5f / 2.75f;
																		   t = 7.5625f * t * t + 0.75f;
																	   } else if(t < 2.5 / 2.75)
																	   {
																		   t -= 2.25f / 2.75f;
																		   t = 7.5625f * t * t + 0.9375f;
																	   }
																	   
																	   t -= 2.625f / 2.75f;
																	   convert_t = 7.5625f * t * t + 0.984375f;
																	   
																	   mission_fail_label->setPosition(ccp(240,myDSH->ui_center_y+93-93*convert_t));
																	   mission_fail_label->setOpacity(t*255);
																	   shadow->setOpacityOuterStroke(255 * 0.3f*(t));
																   }, [=](float t)
																   {
																	   mission_fail_label->setPosition(ccp(240,myDSH->ui_center_y));
																	   mission_fail_label->setOpacity(255);
																	   shadow->setOpacityOuterStroke(255 * 0.3f*(t));
																	   
																	   mission_fail_label->addChild(KSTimer::create(32.f/30.f, [=]()
																													{
																														mission_fail_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 5.f/30.f, [=](float t)
																																												   {
																																													   mission_fail_label->setScale(1.f+t*0.6f);
																																													   mission_fail_label->setOpacity(255-t*255);
																																													   shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																												   }, [=](float t)
																																												   {
																																													   mission_fail_label->setScale(1.6f);
																																													   mission_fail_label->setOpacity(0);
																																													   shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																													   
																																													   mission_fail_label->removeFromParent();
																																												   }));
																													}));
																   }));
	}
	else if(ccb_filename == "ui_stageclear.ccbi")
	{
		KSLabelTTF* stage_clear_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_clearTitle), mySGD->getFont().c_str(), 45);
		stage_clear_label->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
		stage_clear_label->enableOuterStroke(ccc3(60, 20, 0), 2.5f, 255, true);
		stage_clear_label->setPosition(ccp(240,myDSH->ui_center_y));
		stage_clear_label->setScale(1.8f);
		stage_clear_label->setOpacity(0);
		addChild(stage_clear_label);
		
		KSLabelTTF* shadow = CommonAnimation::applyBigShadow(stage_clear_label, stage_clear_label->getFontSize());
		shadow->setOpacityOuterStroke(0);
		
		stage_clear_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
																   {
																	   stage_clear_label->setScale(1.8f-0.8f*t);
																	   stage_clear_label->setOpacity(t*255);
																	   shadow->setOpacityOuterStroke(255 * 0.3f*(t));
																   }, [=](float t)
																   {
																	   stage_clear_label->setScale(1.8f-0.8f*t);
																	   stage_clear_label->setOpacity(255);
																	   shadow->setOpacityOuterStroke(255 * 0.3f*(t));
																	   
																	   stage_clear_label->addChild(KSTimer::create(42.f/30.f, [=]()
																													{
																														stage_clear_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 10.f/30.f, [=](float t)
																																												   {
																																													   stage_clear_label->setScale(1.f+t*0.6f);
																																													   stage_clear_label->setOpacity(255-t*255);
																																													   shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																												   }, [=](float t)
																																												   {
																																													   stage_clear_label->setScale(1.6f);
																																													   stage_clear_label->setOpacity(0);
																																													   shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																													   
																																													   stage_clear_label->removeFromParent();
																																												   }));
																													}));
																   }));
		
		CCParticleSystemQuad* t_particle = CCParticleSystemQuad::createWithTotalParticles(20);
		t_particle->setPositionType(kCCPositionTypeRelative);
		t_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
		t_particle->setEmissionRate(50.00);
		t_particle->setAngle(90);						//
		t_particle->setAngleVar(10.0);
		t_particle->setBlendFunc(ccBlendFunc{GL_ONE, GL_ONE});
		ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
		t_particle->setBlendFunc(blendFunc);
		t_particle->setDuration(-1.0);
		t_particle->setEmitterMode(kCCParticleModeGravity);
		t_particle->setStartColor(ccc4f(1.f, 0.8353f, 0.188f, 0.5f));
		t_particle->setStartColorVar(ccc4f(0, 0, 0, 0));
		t_particle->setEndColor(ccc4f(0, 0, 0, 0));
		t_particle->setEndColorVar(ccc4f(0, 0, 0, 0));
		t_particle->setStartSize(15.00);
		t_particle->setStartSizeVar(5.0);
		t_particle->setEndSize(0.0);
		t_particle->setEndSizeVar(0.0);
		t_particle->setGravity(ccp(0,0));
		t_particle->setRadialAccel(0.0);
		t_particle->setRadialAccelVar(0.0);
		t_particle->setSpeed(0);
		t_particle->setSpeedVar(0.0);
		t_particle->setTangentialAccel(0);
		t_particle->setTangentialAccelVar(0);
		t_particle->setTotalParticles(20);
		t_particle->setLife(0.50);
		t_particle->setLifeVar(1.0);
		t_particle->setStartSpin(0.0);
		t_particle->setStartSpinVar(40.0);
		t_particle->setEndSpin(0.0);
		t_particle->setEndSpinVar(40.0);
		t_particle->setPosition(ccpFromSize(stage_clear_label->getContentSize()/2.f));
		t_particle->setPosVar(ccp(140,20));
		stage_clear_label->addChild(t_particle);
	}
}

void PlayUI::conditionClear ()
{
	if(is_cleared_cdt)
		return;
//	removeChildByTag(kCT_UI_clrCdtLabel);
	is_cleared_cdt = true;
	mission_button->doClose();
	mission_button->isSuccessed(true);
	
	if(mission_back)
	{
		mission_back->removeAllChildren();
		CCSprite* t_success_img = CCSprite::create(CCString::createWithFormat("ui_mission_clear_%s.png", myLoc->getSupportLocalCode())->getCString());
		t_success_img->setPosition(ccpFromSize(mission_back->getContentSize()/2.f));
		mission_back->addChild(t_success_img);
	}
	
	for(int i=0;i<mission_clear_remove_nodes.size();i++)
	{
		mission_clear_remove_nodes[i]->removeFromParent();
	}
	mission_clear_remove_nodes.clear();
	
//	((CCMenu*)getChildByTag(kCT_UI_clrCdtIcon))->setEnabled(false);
	
//	CCSprite* condition_clear = CCSprite::create("condition_clear.png");
//	condition_clear->setPosition(getChildByTag(kCT_UI_clrCdtIcon)->getPosition());
//	addChild(condition_clear);
	
	AudioEngine::sharedInstance()->playEffect("ment_mission_success.mp3", false, true);
	
	KSLabelTTF* mission_complete_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_missionComplete), mySGD->getFont().c_str(), 40);
	mission_complete_label->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
	mission_complete_label->enableOuterStroke(ccc3(60, 20, 0), 2.5f, 255, true);
	mission_complete_label->setPosition(ccp(240-200,myDSH->ui_center_y));
	mission_complete_label->setOpacity(0);
	addChild(mission_complete_label);
	
	KSLabelTTF* shadow = CommonAnimation::applyBigShadow(mission_complete_label, mission_complete_label->getFontSize());
	
	mission_complete_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 9.f/30.f, [=](float t)
															  {
																  mission_complete_label->setPosition(ccp(240-200+200*t,myDSH->ui_center_y));
																  mission_complete_label->setOpacity(t*255);
																  shadow->setOpacityOuterStroke(255 * 0.3f*(t));
															  }, [=](float t)
															  {
																  mission_complete_label->setPosition(ccp(240,myDSH->ui_center_y));
																  mission_complete_label->setOpacity(255);
																  shadow->setOpacityOuterStroke(255 * 0.3f*(t));
																  
																  mission_complete_label->addChild(KSTimer::create(29.f/30.f, [=]()
																											  {
																												  mission_complete_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 7.f/30.f, [=](float t)
																																											{
																																												mission_complete_label->setPosition(ccp(240+200*t,myDSH->ui_center_y));
																																												mission_complete_label->setOpacity(255-t*255);
																																												shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																											}, [=](float t)
																																											{
																																												mission_complete_label->setPosition(ccp(240+200,myDSH->ui_center_y));
																																												mission_complete_label->setOpacity(0);
																																												shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																												
																																												mission_complete_label->removeFromParent();
																																											}));
																											  }));
															  }));
	
//	CCParticleSystemQuad* t_particle = CCParticleSystemQuad::createWithTotalParticles(20);
//	t_particle->setPositionType(kCCPositionTypeRelative);
//	t_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
//	t_particle->setEmissionRate(50.00);
//	t_particle->setAngle(90);						//
//	t_particle->setAngleVar(10.0);
//	t_particle->setBlendFunc(ccBlendFunc{GL_ONE, GL_ONE});
//	ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
//	t_particle->setBlendFunc(blendFunc);
//	t_particle->setDuration(-1.0);
//	t_particle->setEmitterMode(kCCParticleModeGravity);
//	t_particle->setStartColor(ccc4f(1.f, 0.8353f, 0.188f, 0.5f));
//	t_particle->setStartColorVar(ccc4f(0, 0, 0, 0));
//	t_particle->setEndColor(ccc4f(0, 0, 0, 0));
//	t_particle->setEndColorVar(ccc4f(0, 0, 0, 0));
//	t_particle->setStartSize(15.00);
//	t_particle->setStartSizeVar(5.0);
//	t_particle->setEndSize(0.0);
//	t_particle->setEndSizeVar(0.0);
//	t_particle->setGravity(ccp(0,0));
//	t_particle->setRadialAccel(0.0);
//	t_particle->setRadialAccelVar(0.0);
//	t_particle->setSpeed(0);
//	t_particle->setSpeedVar(0.0);
//	t_particle->setTangentialAccel(0);
//	t_particle->setTangentialAccelVar(0);
//	t_particle->setTotalParticles(20);
//	t_particle->setLife(0.50);
//	t_particle->setLifeVar(1.0);
//	t_particle->setStartSpin(0.0);
//	t_particle->setStartSpinVar(40.0);
//	t_particle->setEndSpin(0.0);
//	t_particle->setEndSpinVar(40.0);
//	t_particle->setPosition(ccpFromSize(stage_clear_label->getContentSize()/2.f));
//	t_particle->setPosVar(ccp(140,20));
//	stage_clear_label->addChild(t_particle);
}
void PlayUI::conditionFail ()
{
//	((CCMenu*)getChildByTag(kCT_UI_clrCdtIcon))->setEnabled(false);
//
//	CCSprite* condition_fail = CCSprite::create("condition_fail.png");
//	condition_fail->setPosition(getChildByTag(kCT_UI_clrCdtIcon)->getPosition());
//	addChild(condition_fail);
}
void PlayUI::takeExchangeCoin (CCPoint t_start_position, int t_coin_number)
{
	if(isGameover)
		return;
	
	if(clr_cdt_type == kCLEAR_sequenceChange && !isGameover)
	{
		if(t_coin_number != ing_cdt_cnt.getV())
		{
			conditionFail();
			
			mySGD->fail_code = kFC_missionfail;
			
			stopCounting();
			// timeover
			isGameover = true;
			
			
			AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
			
			addResultCCB("ui_missonfail.ccbi");
			AudioEngine::sharedInstance()->playEffect("ment_mission_fail.mp3", false, true);
			
			endGame(false);
			return;
		}
		else
		{
			ing_cdt_cnt = ing_cdt_cnt.getV()+1;
			
			mission_button->setTextAtIndex(CCString::createWithFormat("%d/%d", ing_cdt_cnt.getV()-1, 6)->getCString(), 1);
//			removeChildByTag(kCT_UI_clrCdtLabel);
//			if(ing_cdt_cnt <= 6)
//			{
//				CCSprite* clr_cdt_img = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", ing_cdt_cnt)->getCString());
//				clr_cdt_img->setPosition(ccpAdd(getChildByTag(kCT_UI_clrCdtIcon)->getPosition(), ccp(0,-5)));
//				addChild(clr_cdt_img, 0, kCT_UI_clrCdtLabel);
//			}
		}
	}
	
	taked_coin_cnt++;
	
	CCSprite* t_coin_spr = (CCSprite*)exchange_dic->objectForKey(t_coin_number);
	CCPoint after_position = t_coin_spr->getPosition();
	t_coin_spr->removeFromParentAndCleanup(true);
	exchange_dic->removeObjectForKey(t_coin_number);
	
	
	CCParticleSystemQuad* coin_particle = CCParticleSystemQuad::createWithTotalParticles(30);
	coin_particle->setPositionType(kCCPositionTypeRelative);
	coin_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("fever_particle.png"));
	coin_particle->setEmissionRate(60);
	coin_particle->setAngle(90.0);
	coin_particle->setAngleVar(30.0);
	ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
	coin_particle->setBlendFunc(blendFunc);
	coin_particle->setDuration(0.7f);
	coin_particle->setEmitterMode(kCCParticleModeGravity);
	coin_particle->setStartColor(ccc4f(1.f, 0.8f, 0.4f, 1.f));
	coin_particle->setStartColorVar(ccc4f(0,0,0,0));
	coin_particle->setEndColor(ccc4f(1.f,1.f,1.f,0.f));
	coin_particle->setEndColorVar(ccc4f(0, 0, 0, 0));
	coin_particle->setStartSize(13.0);
	coin_particle->setStartSizeVar(5.0);
	coin_particle->setEndSize(-1.0);
	coin_particle->setEndSizeVar(0.0);
	coin_particle->setGravity(ccp(0,80));
	coin_particle->setRadialAccel(0.0);
	coin_particle->setRadialAccelVar(0.0);
	coin_particle->setSpeed(140);
	coin_particle->setSpeedVar(60.0);
	coin_particle->setTangentialAccel(0);
	coin_particle->setTangentialAccelVar(0);
	coin_particle->setTotalParticles(30);
	coin_particle->setLife(0.5);
	coin_particle->setLifeVar(0.3);
	coin_particle->setStartSpin(0.0);
	coin_particle->setStartSpinVar(180.f);
	coin_particle->setEndSpin(0.0);
	coin_particle->setEndSpinVar(180.f);
	coin_particle->setPosVar(ccp(12,12));
	coin_particle->setPosition(t_start_position);
	coin_particle->setAutoRemoveOnFinish(true);
	addChild(coin_particle);
	
	
	CCSprite* new_coin_spr = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", t_coin_number)->getCString());
	new_coin_spr->setPosition(t_start_position);
	top_center_node->addChild(new_coin_spr);
	
	CCDelayTime* t_delay = CCDelayTime::create(0.7f);
	CCMoveTo* t_move = CCMoveTo::create(0.5f, after_position);
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_move);
	new_coin_spr->runAction(t_seq);
	
	
	exchange_dic->setObject(new_coin_spr, t_coin_number);
	
	if(taked_coin_cnt >= 6 && !isGameover && getPercentage() < clearPercentage.getV() && !is_exchanged)
	{
		if(clr_cdt_type == kCLEAR_sequenceChange)
		{
			conditionClear();
		}
		
		isFirst = true;
		is_exchanged = true;
		my_fp->addFeverGage(20);
		
		for(int i=kAchievementCode_changeMania1;i<=kAchievementCode_changeMania3;i++)
		{
			if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) &&
			   mySGD->getUserdataAchieveChangeMania() + 1 >= myAchieve->getCondition((AchievementCode)i))
			{
				myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
		
		myGD->communication("Main_startExchange");
		myGD->communication("Main_showChangeCard");
		myGD->communication("Jack_positionRefresh");
		
		m_areaGage->onChange();
		
//		if(NSDS_GI(mySD->getSilType(), kSDS_SI_missionType_i) == kCLEAR_score && !is_cleared_cdt)
//			checkScoreMission();
	}
}
void PlayUI::subBossLife (float t_life)
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
	KSCumberBase* cb = dynamic_cast<KSCumberBase*>(myGD->getMainCumberCCNodeVector()[0]);
	if(!is_cleared_cdt)
		mission_button->setTextAtIndex(CCString::createWithFormat("%.1f%%", cb->getLife()/cb->getTotalLife()*100.f)->getCString(), 1);
//		((CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel))->setString(CCString::createWithFormat("%.1f%%", cb->getLife()/cb->getTotalLife()*100.f)->getCString());
	
	if(cb->getLife() <= 0.f && !is_cleared_cdt)
	{
		conditionClear();
	}
}
void PlayUI::setMaxBossLife (float t_life)
{
	maxBossLife = t_life;
	bossLife = maxBossLife;
}
void PlayUI::setClearPercentage (float t_p)
{
	if(clr_cdt_type == kCLEAR_percentage)
	{
		map<int, MissionOper>::iterator t_iter = mission_oper_list.find((int)clr_cdt_type);
		if(t_iter != mission_oper_list.end())
		{
			float dis_value = t_p - 0.85f;
			
			if(t_iter->second.oper == "-")
				dis_value -= t_iter->second.value;
			else if(t_iter->second.oper == "*")
				dis_value *= t_iter->second.value;
			
			myGD->communication("Jack_showMissionEffect", int(clr_cdt_type));
			
			if(dis_value < 0)
				dis_value = 0;
			
			t_p -= dis_value;
		}
	}
	
	clearPercentage = t_p;
	m_areaGage = AreaGage::create(clearPercentage.getV());//0.85f);//clearPercentage.getV());
	m_areaGage->setPosition(ccp(240,myDSH->ui_top-29));
	top_center_node->addChild(m_areaGage);
	m_areaGage->setPercentage(getPercentage());
	if(clr_cdt_type == kCLEAR_hellMode)
		m_areaGage->setVisible(false);
}
void PlayUI::startCounting ()
{
	schedule(schedule_selector(PlayUI::counting));
}
void PlayUI::resumeCounting ()
{
	schedule(schedule_selector(PlayUI::counting));
}
void PlayUI::stopCounting ()
{
	unschedule(schedule_selector(PlayUI::counting));
}
void PlayUI::keepBossLife ()
{
	keepLife = bossLife;
}
void PlayUI::checkBossLife ()
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
int PlayUI::getGameTime ()
{
	return countingCnt.getV();
}
//void PlayUI::setControlTD (CCObject * t_main, SEL_CallFunc d_gesture, SEL_CallFunc d_button, SEL_CallFunc d_joystick, SEL_CallFunc d_startControl)
//{
//	target_main = t_main;
//	delegate_gesture = d_gesture;
//	delegate_button = d_button;
//	delegate_joystick = d_joystick;
//	delegate_startControl = d_startControl;
//}
void PlayUI::showPause ()
{
	AudioEngine::sharedInstance()->setAppBack();
	mySGD->is_paused = true;
	
	myGD->communication("Main_showPause");
//	PausePopupLayer* t_ppl = PausePopupLayer::create(this, callfunc_selector(PlayUI::goHome), this, callfunc_selector(PlayUI::cancelHome), target_main, delegate_gesture, delegate_button, delegate_joystick, this, callfunc_selector(PlayUI::goReplay));
//	addChild(t_ppl);
}
void PlayUI::showContinuePopup (CCObject * t_end, SEL_CallFunc d_end, CCObject * t_continue, SEL_CallFunc d_continue)
{
	target_continue = t_continue;
	delegate_continue = d_continue;
	AudioEngine::sharedInstance()->setAppBack();
	mySGD->is_paused = true;
	
	myGD->communication("Main_showContinue", t_end, d_end, this, callfunc_selector(PlayUI::continueAction));
}
void PlayUI::addGameTime30Sec ()
{
	if(countingCnt >= 130)
	{
		AudioEngine::sharedInstance()->stopEffect("se_clock.mp3");
//		AudioEngine::sharedInstance()->stopEffect("sound_time_noti.mp3");
	}
	
	countingCnt -= 30;
	total_time += 30;
//	if(mySGD->isUsingItem(kIC_longTime))
//	{
//		if(countingCnt < -mySGD->getLongTimeValue())
//			countingCnt = -mySGD->getLongTimeValue();
//	}
//	else
//	{
		if(countingCnt < 0)
			countingCnt = 0;
//	}
}
bool PlayUI::beRevivedJack ()
{
	if(jack_life > 0)
	{
		addGameTime30Sec();
		
		jack_life--;
		jack_life_node->removeChild((CCNode*)jack_array->lastObject(), true);
		jack_array->removeLastObject();
		
//		CCMoveTo* t_move = CCMoveTo::create(0.5f, ccp(240,myDSH->ui_bottom+20));
//		jack_life_node->runAction(t_move);
		jack_life_hide_count = 0;
		
		if(jack_life <= 0)
		{
			myGD->showDetailMessage("warning_over_02.ccbi", "h");
			
//			CCSprite* last_life_ccb = KS::loadCCBI<CCSprite*>(this, "warning_over_02.ccbi").first;
//			last_life_ccb->setPosition(ccp(240,myDSH->ui_center_y));
//			addChild(last_life_ccb);
//			
//			last_life_ccb->addChild(KSTimer::create(1.5f, [=]()
//			{
//				last_life_ccb->removeFromParent();
//			}));
		}
		
		AudioEngine::sharedInstance()->stopEffect("se_clock.mp3");
//		AudioEngine::sharedInstance()->stopEffect("sound_time_noti.mp3");
		
		return true;
	}
	else
	{
		if(!is_used_heartUpItem && mySGD->isUsingItem(kIC_heartUp))
		{
			using_item_sprites[kIC_heartUp]->setGray(true);
			
//			CCLabelTTF* item_bonus_label = CCLabelTTF::create("부활 아이템!!!", mySGD->getFont().c_str(), 20);
//			item_bonus_label->setPosition(ccp(240,myDSH->ui_center_y));
//			addChild(item_bonus_label);
//			
//			CCFadeTo* t_fade = CCFadeTo::create(1.5f, 0);
//			CCCallFunc* t_call = CCCallFunc::create(item_bonus_label, callfunc_selector(CCLabelTTF::removeFromParent));
//			CCSequence* t_seq = CCSequence::create(t_fade, t_call, NULL);
//			item_bonus_label->runAction(t_seq);
			
			is_used_heartUpItem = true;
			
			AudioEngine::sharedInstance()->stopEffect("se_clock.mp3");
//			AudioEngine::sharedInstance()->stopEffect("sound_time_noti.mp3");
			
			return true;
		}
		else
			return false;
	}
}

void PlayUI::takeSilenceItem()
{
//	CCLabelTTF* item_bonus_label = CCLabelTTF::create("침묵 아이템!!!", mySGD->getFont().c_str(), 20);
//	item_bonus_label->setPosition(ccp(240,myDSH->ui_center_y));
//	addChild(item_bonus_label);
//	
//	CCFadeTo* t_fade = CCFadeTo::create(1.5f, 0);
//	CCCallFunc* t_call = CCCallFunc::create(item_bonus_label, callfunc_selector(CCLabelTTF::removeFromParent));
//	CCSequence* t_seq = CCSequence::create(t_fade, t_call, NULL);
//	item_bonus_label->runAction(t_seq);
}

void PlayUI::takeAddTimeItem ()
{
	if(mySGD->is_god_of_death)
		return;
	
	int change_time = NSDS_GI(mySD->getSilType(), kSDS_SI_itemOptionAddTimeSec_i);
	mySGD->add_time_value = mySGD->add_time_value.getV() + change_time;
	countingCnt -= change_time;
	total_time += change_time;
	
	if(countingCnt-1 < playtime_limit-30)
		AudioEngine::sharedInstance()->stopEffect("se_clock.mp3");
	
	CCLOG("addtime value : %d", change_time);
//	if(mySGD->isUsingItem(kIC_longTime))
//	{
//		if(countingCnt < -mySGD->getLongTimeValue())
//			countingCnt = -mySGD->getLongTimeValue();
//	}
//	else
//	{
		if(countingCnt < 0)
			countingCnt = 0;
//	}
}
bool PlayUI::getIsExchanged ()
{
	return is_exchanged;
}

void PlayUI::addScoreAttack(int t_damage)
{
//	ccColor4F t_color;
//	if(score_attack_damage.getV() < 3000)
//	{
//		t_color = ccc4f(1.f, 1.f, 1.f, 1.f);
//	}
//	else if(score_attack_damage.getV() < 6000)
//	{
//		t_color = ccc4f(1.f, 1.f, 0.f, 1.f);
//	}
//	else if(score_attack_damage.getV() < 9000)
//	{
//		t_color = ccc4f(1.f, 0.5f, 0.f, 1.f);
//	}
//	else
//	{
//		t_color = ccc4f(1.f, 0.f, 0.f, 1.f);
//	}
//	
//	CCParticleSystemQuad* particle2 = CCParticleSystemQuad::createWithTotalParticles(100);
//	particle2->setPositionType(kCCPositionTypeRelative);
//	particle2->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
//	particle2->setEmissionRate(200);
//	particle2->setAngle(180.0);
//	particle2->setAngleVar(180.0);
//	ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
//	particle2->setBlendFunc(blendFunc);
//	particle2->setDuration(0.7);
//	particle2->setEmitterMode(kCCParticleModeRadius);
//	particle2->setStartColor(t_color);
//	particle2->setStartColorVar(ccc4f(0,0,0,0.f));
//	particle2->setEndColor(t_color);
//	particle2->setEndColorVar(ccc4f(0, 0, 0, 0.f));
//	particle2->setStartSize(10.0);
//	particle2->setStartSizeVar(5.0);
//	particle2->setEndSize(10.0);
//	particle2->setEndSizeVar(2.0);
//	particle2->setRotatePerSecond(40);
//	particle2->setRotatePerSecondVar(10);
//	particle2->setStartRadius(30);
//	particle2->setStartRadiusVar(10);
//	particle2->setEndRadius(2);
//	particle2->setTotalParticles(100);
//	particle2->setLife(0.5);
//	particle2->setLifeVar(0.3);
//	particle2->setStartSpin(0.0);
//	particle2->setStartSpinVar(50.f);
//	particle2->setEndSpin(0.0);
//	particle2->setEndSpinVar(60.f);
//	particle2->setPosVar(ccp(0,0));
//	particle2->setPosition(ccp(40, myDSH->ui_center_y));
//	particle2->setAutoRemoveOnFinish(true);
//	addChild(particle2);
	
	score_attack_damage = score_attack_damage.getV() + t_damage;
	score_attack_keep_frame = 0;
	schedule(schedule_selector(PlayUI::scoreAttackKeep));
}

void PlayUI::scoreAttackKeep()
{
	score_attack_keep_frame++;
	
	if(score_attack_keep_frame >= 60)
	{
		unschedule(schedule_selector(PlayUI::scoreAttackKeep));

		CCPoint recent_position = bomb_img->getPosition();
		CCPoint after_position = ccp(0,-54);
		CCPoint sub_position = recent_position - after_position;
		bomb_img->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
														   {
															   bomb_img->setPosition(recent_position - sub_position*t);
														   }, [=](float t)
														   {
															   bomb_img->setPosition(after_position);
														   }));
		
		ing_bomb_value = 0;
		
		myGD->communication("Main_scoreAttackMissile", score_attack_damage.getV());
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_scoreAttackDamage)] = score_attack_damage.getV();
		mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_scoreAttackedValue)] = mySGD->replay_playing_info.get(mySGD->getReplayKey(kReplayKey_scoreAttackedValue), Json::Value()).asInt() + score_attack_damage.getV();
		
		score_attack_damage = 0;
	}
}

void PlayUI::scoreAttackMissile(int t_damage)
{
	if(t_damage <= 0)
		return;
	
	int cnt = t_damage/3000;
	
	KSLabelTTF* combo_label = KSLabelTTF::create(ccsf("%s%d", myLoc->getLocalForKey(LK::kMyLocalKey_combo), cnt*5), mySGD->getFont().c_str(), 30);
	combo_label->enableOuterStroke(ccc3(0, 45, 10), 2.5f, 255, true);
	combo_label->setGradientColor(ccc4(240, 255, 10, 255), ccc4(110, 190, 5, 255), ccp(0,-1));
	KSLabelTTF* shadow = CommonAnimation::applyBigShadow(combo_label, combo_label->getFontSize());
	combo_label->setPosition(ccp(440,myDSH->ui_center_y+60));
	combo_label->setScale(1.f/1.5f);
	addChild(combo_label);
	
//	CCLabelBMFont* combo_label = CCLabelBMFont::create(CCString::createWithFormat("%d", cnt*5)->getCString(), "combo.fnt");
//	combo_label->setAnchorPoint(ccp(0.5f,0.5f));
//	combo_label->setPosition(ccp(0,0));
//	t_node->addChild(combo_label);
//	
//	CCSprite* combo_front = CCSprite::create("combo_front.png");
//	combo_front->setAnchorPoint(ccp(0.5f,0.5f));
//	combo_front->setPosition(ccp(0,-10));
//	combo_front->setScale(0.5f);
//	t_node->addChild(combo_front);
	
	addChild(KSTimer::create(2.f, [=]()
	{
		addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
											   {
												   combo_label->setOpacity(255-t*255);
												   shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
											   }, [=](float t)
											   {
												   combo_label->setOpacity(0);
												   shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
												   combo_label->removeFromParent();
											   }));
	}));
	
//
//	for(int i=0;i<cnt;i++)
//	{
//		addChild(KSTimer::create(0.2f*i, [=]()
//								 {
									 myGD->communication("Main_readyBomb");
//								 }));
//	}
	
	
	addChild(KSTimer::create(1.f, [=]()
							 {
								 myGD->communication("Main_stopBomb");
								 
								 CCPoint origin_position = ccp(440, myDSH->ui_center_y+60);
								 CCPoint final_position = ccp(40, myDSH->ui_center_y-10);
								 
								 CCSprite* t_missile = KS::loadCCBI<CCSprite*>(this, "endless_missile_you.ccbi").first;
								 t_missile->setPosition(origin_position);
								 addChild(t_missile);
								 
								 float alpha_value = 150.f/180.f*M_PI;
								 float v_zero = (final_position.x-origin_position.x)/60.f/cosf(alpha_value);
								 float v_x_zero = v_zero*cosf(alpha_value);
								 float v_y_zero = v_zero*sinf(alpha_value);
								 float gravity = (-final_position.y + origin_position.y + v_zero*sinf(alpha_value)*60.f)/3600.f*2.f;
								 
								 addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
																		{
																			CCPoint before_position = t_missile->getPosition();
																			t_missile->setPosition(ccp(origin_position.x + v_x_zero*t*60, origin_position.y + v_y_zero*t*60 - gravity*t*t*60*60/2));
																			
																			t_missile->setRotation(-(t_missile->getPosition() - before_position).getAngle()/M_PI*180.f + 180);
																		}, [=](float t)
																		{
																			CCPoint before_position = t_missile->getPosition();
																			t_missile->setPosition(ccp(origin_position.x + v_x_zero*1.f*60, origin_position.y + v_y_zero*1.f*60 - gravity*1.f*1.f*60*60/2));
																			
																			t_missile->setRotation(-(t_missile->getPosition() - before_position).getAngle()/M_PI*180.f + 180);
																			
																			t_missile->removeFromParent();
																			
																			CCSprite* t_bomb_img = KS::loadCCBI<CCSprite*>(this, "bossbomb2.ccbi").first;
																			t_bomb_img->setPosition(ccp(40, myDSH->ui_center_y-10));
																			addChild(t_bomb_img);
																			
																			t_bomb_img->addChild(KSTimer::create(24.f/30.f, [=](){t_bomb_img->removeFromParent();}));
																			
																			KSLabelTTF* t_label = KSLabelTTF::create(CCString::createWithFormat("%d", -t_damage)->getCString(), mySGD->getFont().c_str(), 12);
																			t_label->setColor(ccRED);
																			t_label->enableOuterStroke(ccBLACK, 1.f);
																			t_label->setPosition(ccp(40, myDSH->ui_center_y-10));
																			t_label->setScale(0.7f);
																			addChild(t_label);
																			
																			addChild(KSGradualValue<float>::create(0.f, 1.f, 5.f/30.f, [=](float t)
																												   {
																													   t_label->setOpacity(t*255);
																													   t_label->setScale(0.7f+t);
																												   }, [=](float t)
																												   {
																													   t_label->setOpacity(255);
																													   t_label->setScale(1.7f);
																													   addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
																																							  {
																																								  t_label->setScale(1.7f-t*0.5f);
																																							  }, [=](float t)
																																							  {
																																								  t_label->setScale(1.2f);
																																								  addChild(KSTimer::create(8.f/30.f, [=]()
																																														   {
																																															   addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
																																																									  {
																																																										  t_label->setPosition(ccp(40, myDSH->ui_center_y-35*t));
																																																										  t_label->setScale(1.2f-0.3f*t);
																																																										  t_label->setOpacity(255-t*255);
																																																									  }, [=](float t)
																																																									  {
																																																										  t_label->setPosition(ccp(40, myDSH->ui_center_y-35));
																																																										  t_label->setScale(0.9f);
																																																										  t_label->setOpacity(0);
																																																										  t_label->removeFromParent();
																																																									  }));
																																														   }));
																																							  }));
																												   }));
																			
																			float before_score = score_value.getV();
																			damaged_score = damaged_score.getV() - t_damage;
//																			CCLOG("damaged_score : %d / score_value : %.0f", damaged_score.getV(), score_value.getV());
																			score_label->setString(CCString::createWithFormat("%.0f", damaged_score.getV() + before_score)->getCString());
																			
																		}));
								 
								 
								 
								 
//								 CCPoint base_position = ccp(440-70, myDSH->ui_center_y+70+40);
//								 CCPoint random_position = ccp(rand()%41-20, rand()%31-15);
//								 CCPoint sum_position = base_position + random_position;
//								 CCPoint sub_position = sum_position - origin_position;
//
//								 float angle = atan2(sum_position.y - origin_position.y, sum_position.x - origin_position.x)/M_PI*180.f;
//								 t_missile->setRotation(-angle+180);
//
//								 addChild(KSGradualValue<float>::create(0.f, 1.f, 20.f/30.f, [=](float t)
//																		{
//																			t_missile->setPosition(origin_position + sub_position*t);
//																		}, [=](float t)
//																		{
//																			t_missile->setPosition(origin_position + sub_position);
//																			CCPoint final_position = ccp(40,myDSH->ui_center_y);
//																			CCPoint t_sub_position = final_position - sum_position;
//																			
//																			float angle2 = atan2(final_position.y - sum_position.y, final_position.x - sum_position.x)/M_PI*180.f;
//																			t_missile->setRotation(-angle2+180);
//																			
//																			addChild(KSGradualValue<float>::create(0.f, 1.f, 10.f/30.f, [=](float t)
//																												   {
//																													   t_missile->setPosition(sum_position + t_sub_position*t);
//																												   }, [=](float t)
//																												   {
//																													   t_missile->setPosition(sum_position + t_sub_position);
//																													   t_missile->removeFromParent();
//																													   
//																													   CCSprite* bomb_img = KS::loadCCBI<CCSprite*>(this, "bossbomb2.ccbi").first;
//																													   bomb_img->setPosition(ccp(40, myDSH->ui_center_y));
//																													   addChild(bomb_img);
//																													   
//																													   bomb_img->addChild(KSTimer::create(24.f/30.f, [=](){bomb_img->removeFromParent();}));
//																													   
//																													   KSLabelTTF* t_label = KSLabelTTF::create(CCString::createWithFormat("%d", -t_damage)->getCString(), mySGD->getFont().c_str(), 12);
//																													   t_label->setColor(ccRED);
//																													   t_label->enableOuterStroke(ccBLACK, 1.f);
//																													   t_label->setPosition(ccp(40, myDSH->ui_center_y));
//																													   t_label->setScale(0.7f);
//																													   addChild(t_label);
//																													   
//																													   addChild(KSGradualValue<float>::create(0.f, 1.f, 5.f/30.f, [=](float t)
//																																							  {
//																																								  t_label->setOpacity(t*255);
//																																								  t_label->setScale(0.7f+t);
//																																							  }, [=](float t)
//																																							  {
//																																								  t_label->setOpacity(255);
//																																								  t_label->setScale(1.7f);
//																																								  addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
//																																																		 {
//																																																			 t_label->setScale(1.7f-t*0.5f);
//																																																		 }, [=](float t)
//																																																		 {
//																																																			 t_label->setScale(1.2f);
//																																																			 addChild(KSTimer::create(8.f/30.f, [=]()
//																																																									  {
//																																																										  addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
//																																																																				 {
//																																																																					 t_label->setPosition(ccp(40, myDSH->ui_center_y-25*t));
//																																																																					 t_label->setScale(1.2f-0.3f*t);
//																																																																					 t_label->setOpacity(255-t*255);
//																																																																				 }, [=](float t)
//																																																																				 {
//																																																																					 t_label->setPosition(ccp(40, myDSH->ui_center_y-25));
//																																																																					 t_label->setScale(0.9f);
//																																																																					 t_label->setOpacity(0);
//																																																																					 t_label->removeFromParent();
//																																																																				 }));
//																																																									  }));
//																																																		 }));
//																																							  }));
//																													   
//																													   float before_score = score_value.getV();
//																													   damaged_score = damaged_score.getV() - t_damage;
//																													   CCLOG("damaged_score : %d / score_value : %.0f", damaged_score.getV(), score_value.getV());
//																													   score_label->setString(CCString::createWithFormat("%.0f", damaged_score.getV() + before_score)->getCString());
//																													   
//																												   }));
//																		}));
								 
								 
//								 CCSprite* t_missile = CCSprite::create("blind_drop.png");
//								 t_missile->setPosition(ccp(440, myDSH->ui_center_y));
//								 addChild(t_missile);
//								 
//								 t_missile->setScale(0.3f);
//								 
//								 addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
//																		{
//																			t_missile->setScale(0.3f+t*0.7f);
//																			t_missile->setPosition(ccp(440-t*400, myDSH->ui_center_y));
//																		}, [=](float t)
//																		{
//																			t_missile->setScale(1.f);
//																			t_missile->setPosition(ccp(40, myDSH->ui_center_y));
//																			myGD->communication("Main_showScoreMissileEffect", ccp(40, myDSH->ui_center_y));
//																			t_missile->removeFromParent();
//																			
//																			addChild(KSTimer::create(0.1f, [=]()
//																									 {
//																										 KSLabelTTF* t_label = KSLabelTTF::create(CCString::createWithFormat("%d", -t_damage)->getCString(), mySGD->getFont().c_str(), 12);
//																										 t_label->enableOuterStroke(ccBLACK, 1.f);
//																										 t_label->setPosition(ccp(40, myDSH->ui_center_y));
//																										 addChild(t_label);
//																										 
//																										 addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
//																																				{
//																																					t_label->setOpacity(255-t*255);
//																																				}, [=](float t)
//																																				{
//																																					t_label->setOpacity(0);
//																																					t_label->removeFromParent();
//																																				}));
//																										 
//																										 float before_score = score_value.getV();
//																										 damaged_score = damaged_score.getV() - t_damage;
//																										 CCLOG("damaged_score : %d / score_value : %.0f", damaged_score.getV(), score_value.getV());
//																										 score_label->setString(CCString::createWithFormat("%.0f", damaged_score.getV() + before_score)->getCString());
//																									 }));
//																		}));
							 }));
	
	mySGD->damaged_score = mySGD->damaged_score.getV() + t_damage;
}

int PlayUI::getComboCnt ()
{
	return combo_cnt.getV();
}
void PlayUI::setComboCnt (int t_combo)
{
	int before_combo = combo_cnt.getV();
	combo_cnt = t_combo;
	if(high_combo_cnt < combo_cnt.getV())
	{
		high_combo_cnt = combo_cnt.getV();
		
		if(!is_cleared_cdt && clr_cdt_type == kCLEAR_combo && high_combo_cnt >= clr_cdt_cnt.getV())
		{
			conditionClear();
		}
	}
	
	if(before_combo < combo_cnt.getV())
	{
		if(mySGD->is_endless_mode)
		{
			if(combo_cnt.getV()%5 == 0)
			{
				ing_bomb_value = 5;
				addScoreAttack(3000);
			}
			else if(ing_bomb_value < combo_cnt.getV()%5)
			{
				ing_bomb_value = combo_cnt.getV()%5;
			}
			
			CCPoint recent_position = bomb_img->getPosition();
			CCPoint after_position = ccp(0,-54+54.f/5.f*ing_bomb_value);
			CCPoint sub_position = recent_position - after_position;
			
			bomb_img->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
															   {
																   bomb_img->setPosition(recent_position - sub_position*t);
															   }, [=](float t)
															   {
																   bomb_img->setPosition(after_position);
															   }));
		}
		
		my_combo->showCombo(t_combo);
		
		for(int i=kAchievementCode_comboMania1;i<=kAchievementCode_comboMania3;i++)
		{
			if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
			   combo_cnt.getV() == myAchieve->getCondition((AchievementCode)i))
			{
				myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
	}
	else
	{
		if(mySGD->is_endless_mode)
		{
			ing_bomb_value = combo_cnt.getV();
			if(ing_bomb_value > 5)
				ing_bomb_value = 5;
			
			CCPoint recent_position = bomb_img->getPosition();
			CCPoint after_position = ccp(0,-54+54.f/5.f*ing_bomb_value);
			CCPoint sub_position = recent_position - after_position;
			
			bomb_img->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
															   {
																   bomb_img->setPosition(recent_position - sub_position*t);
															   }, [=](float t)
															   {
																   bomb_img->setPosition(after_position);
															   }));
		}
		myLog->addLog(kLOG_endCombo_i, myGD->getCommunication("UI_getUseTime"), before_combo);
	}
}
int PlayUI::getUseTime ()
{
	return use_time;
}
void PlayUI::takeCoinModeOn ()
{
//	top_center_node->setPosition(ccpAdd(top_center_node->getPosition(), ccp(0,8)));
	is_show_exchange_coin = true;
	
	if(!mySGD->is_hell_mode)
	{
		taked_coin_cnt = 0;
		for(int i=1;i<=6;i++)
		{
			CCSprite* t_gray_coin = (CCSprite*)exchange_dic->objectForKey(i);
			t_gray_coin->setVisible(true);
		}
		myGD->communication("Main_showCoin");
		myGD->communication("Main_showTakeCoin");
	}
}

void PlayUI::checkMapTimeVector()
{
	Json::Value map_time_list = mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_mapTime)];
	int list_size = map_time_list.size();
	
	if(list_size > 0 && map_time_list[list_size-1].asInt() < use_time)
		writeMap();
	else
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_isChangedMap)] = true;
}

void PlayUI::writeMap()
{
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_mapIndex)] = mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_mapTime)].size();
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_mapTime)].append(use_time);
	
	Json::Value writing_map;
	int y, x, record, t_count, i, j;
	for(y = 0;y<43;++y)
	{
		record = 0;
		for(x = 0;x<32;++x)
		{
			t_count = 0;
			for(i = mapWidthInnerBegin+x*5;i<mapWidthInnerBegin+(x+1)*5;++i)
				for(j = mapHeightInnerBegin+y*5;j<mapHeightInnerBegin+(y+1)*5;++j)
					if(myGD->mapState[i][j] == mapOldget || myGD->mapState[i][j] == mapOldline)
						t_count++;
			
			record = record << 1;
			if(t_count >= 13)
			{
				++record;
			}
		}
		writing_map.append(record);
	}
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_mapData)].append(writing_map);
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_isChangedMap)] = false;
}

void PlayUI::checkScoreTimeVector()
{
	Json::Value score_time_list = mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreTime)];
	int list_size = score_time_list.size();
	
	if(list_size > 0 && score_time_list[list_size-1].asInt() < use_time)
		writeScore();
	else
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_isChangedScore)] = true;
}

void PlayUI::writeScore()
{
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_scoreIndex)] = mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreTime)].size();
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreTime)].append(use_time);
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreData)].append(int(getScore()));
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_isChangedScore)] = false;
}

void PlayUI::writePosition()
{
	CCPoint jack_position = myGD->getJackPoint().convertToCCP();
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_characterPositionX)] = jack_position.x;
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_characterPositionY)] = jack_position.y;
	
	
	vector<CCNode*> bossArray = myGD->getMainCumberCCNodeVector();
	int loop_cnt1 = bossArray.size();
	for(int i=0;i<loop_cnt1;i++)
	{
		CCNode* t_boss = (CCNode*)bossArray[i];
		CCPoint t_position = t_boss->getPosition();
		
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_boss)][i][mySGD->getReplayKey(kReplayKey_timeStamp_boss_x)] = t_position.x;
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_boss)][i][mySGD->getReplayKey(kReplayKey_timeStamp_boss_y)] = t_position.y;
	}
	
//	CCNode* boss_node = myGD->getCommunicationNode("CP_getMainCumberPointer");
//	CCPoint boss_position = boss_node->getPosition();
//	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_boss)][i] = boss_position.x;
//	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_boss)][i] = boss_position.y;
	
	vector<CCNode*> subCumberArray = myGD->getSubCumberCCNodeVector();
	int loop_cnt2 = subCumberArray.size();
	for(int i=0;i<loop_cnt2;i++)
	{
		CCNode* t_subCumber = (CCNode*)subCumberArray[i];
		CCPoint t_position = t_subCumber->getPosition();
		
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_monster)][i][mySGD->getReplayKey(kReplayKey_timeStamp_monster_x)] = t_position.x;
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_monster)][i][mySGD->getReplayKey(kReplayKey_timeStamp_monster_y)] = t_position.y;
	}
}

void PlayUI::writeDie()
{
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_isDie)] = true;
}

void PlayUI::writeImageChange()
{
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_isImageChange)] = true;
}

void PlayUI::writeGameOver(int t_i)
{
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_gameInfo)] = t_i;
}

void PlayUI::writeContinue()
{
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_isContinue)] = true;
}

void PlayUI::counting ()
{
	if(countingCnt-1 >= playtime_limit-30)
		is_urgent = true;
	else
		is_urgent = false;
	
	detail_counting_cnt++;
	bool t_is_die = false;
	if(detail_counting_cnt >= 60)
	{
		detail_counting_cnt = 0;
		countingCnt++;
		use_time++;
		if(mySGD->is_hell_mode && use_time%5 == 0)
		{
			vector<CCNode*> t_vector = myGD->getMainCumberCCNodeVector();
			for(int i=0;i<t_vector.size();i++)
			{
				KSCumberBase* t_boss = (KSCumberBase*)t_vector[i];
				t_boss->applyHellBalance(use_time);
			}
		}
		
		if(jack_life_hide_count != -1)
		{
			jack_life_hide_count++;
			if(jack_life_hide_count > 3)
			{
				CCMoveTo* t_move = CCMoveTo::create(1.f, ccp(240, myDSH->ui_bottom-70));
				jack_life_node->runAction(t_move);
				jack_life_hide_count = -1;
			}
		}
		
		if(mySGD->is_draw_button_tutorial && mySGD->draw_button_tutorial_ing > 0)
		{
			mySGD->draw_button_tutorial_ing++;
			if(mySGD->draw_button_tutorial_ing >= draw_button_tutorial_show)
			{
				myGD->communication("Main_showDrawButtonTutorial");
				mySGD->draw_button_tutorial_ing = 0;
				draw_button_tutorial_show += 10;
			}
		}
		
		if(mySGD->is_write_replay)
		{
			if(mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_isChangedMap)].asBool())
				writeMap();
			
			if(mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_isChangedScore)].asBool())
				writeScore();
			
			writePosition();
		}
		
		if(mySGD->is_play_replay)
		{
			if(mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapTime)].size() > 0)
			{
				myGD->communication("Main_refreshReplayThumb", use_time);
				myGD->communication("Main_refreshReplayPosition", use_time);
			}
			
			if(mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_scoreTime)].size() > 0)
				myGD->communication("Main_refreshReplayScore", use_time);
			
			scoreAttackMissile(mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time].get(mySGD->getReplayKey(kReplayKey_timeStamp_scoreAttackDamage), Json::Value()).asInt());
		}
		
		
		if(countingCnt-1 == playtime_limit/3 && !mySGD->is_hell_mode)
		{
			AudioEngine::sharedInstance()->playEffect("se_clock.mp3", false);
//			AudioEngine::sharedInstance()->playEffect("sound_time_noti.mp3", false);
			if(myGD->game_step == kGS_limited)
				myGD->communication("Main_setUnlimitMap");
		}
		else if(countingCnt-1 == playtime_limit-50 && !mySGD->is_hell_mode)
		{
			AudioEngine::sharedInstance()->playEffect("se_clock.mp3", false);
//			AudioEngine::sharedInstance()->playEffect("sound_time_noti.mp3", false);
		}
		else if(countingCnt-1 == playtime_limit-30 && !mySGD->is_hell_mode)
		{
			AudioEngine::sharedInstance()->playEffect("se_clock.mp3", true);
			AudioEngine::sharedInstance()->playEffect("ment_30second.mp3", false, true);
//			AudioEngine::sharedInstance()->playEffect("sound_time_noti.mp3", true);
		}
		
		if(countingCnt.getV()-1 >= playtime_limit.getV() && !mySGD->is_hell_mode)
		{
			if(!is_used_longTimeItem && mySGD->isUsingItem(kIC_longTime))
			{
				is_used_longTimeItem = true;
				countingCnt -= mySGD->getLongTimeValue();
				
				using_item_sprites[kIC_longTime]->setGray(true);
				
				CCLabelTTF* item_bonus_label = CCLabelTTF::create("시간 아이템!!!", mySGD->getFont().c_str(), 20);
				item_bonus_label->setPosition(ccp(240,myDSH->ui_center_y));
				addChild(item_bonus_label);
				
				CCFadeTo* t_fade = CCFadeTo::create(1.5f, 0);
				CCCallFunc* t_call = CCCallFunc::create(item_bonus_label, callfunc_selector(CCLabelTTF::removeFromParent));
				CCSequence* t_seq = CCSequence::create(t_fade, t_call, NULL);
				item_bonus_label->runAction(t_seq);
			}
			else
			{
				AudioEngine::sharedInstance()->playEffect("ment_timeover.mp3", false, true);
				
				t_is_die = true;
				
				mySGD->is_god_of_death = true;
				Json::Value pd;
				pd["speed"] = 3.f;
				myGD->attachGodOfDeath(myGD->getMainCumberVector()[0], pd);
				
//				myGD->communication("Jack_startDieEffect", DieType::kDieType_timeover);
//				
//				KSLabelTTF* time_over_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_failTitleTimeover), mySGD->getFont().c_str(), 45);
//				time_over_label->setGradientColor(ccc4(255, 115, 250, 255), ccc4(215, 60, 130, 255), ccp(0,-1));
//				time_over_label->enableOuterStroke(ccc3(65, 5, 35), 2.5f, 255, true);
//				time_over_label->setPosition(ccp(240,myDSH->ui_center_y+93));
//				time_over_label->setOpacity(0);
//				addChild(time_over_label);
//				
//				KSLabelTTF* shadow = CommonAnimation::applyBigShadow(time_over_label, time_over_label->getFontSize());
//				shadow->setOpacityOuterStroke(0);
//				
//				time_over_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 13.f/30.f, [=](float t)
//																		{
//																			float convert_t;
//																			if (t < 1 / 2.75)
//																			{
//																				convert_t = 7.5625f * t * t;
//																			} else if (t < 2 / 2.75)
//																			{
//																				t -= 1.5f / 2.75f;
//																				convert_t = 7.5625f * t * t + 0.75f;
//																			} else if(t < 2.5 / 2.75)
//																			{
//																				t -= 2.25f / 2.75f;
//																				convert_t = 7.5625f * t * t + 0.9375f;
//																			}
//																			
//																			t -= 2.625f / 2.75f;
//																			convert_t = 7.5625f * t * t + 0.984375f;
//																			
//																			time_over_label->setPosition(ccp(240,myDSH->ui_center_y+93-93*convert_t));
//																			time_over_label->setOpacity(t*255);
//																			shadow->setOpacityOuterStroke(255 * 0.3f*(t));
//																		}, [=](float t)
//																		{
//																			time_over_label->setPosition(ccp(240,myDSH->ui_center_y));
//																			time_over_label->setOpacity(255);
//																			shadow->setOpacityOuterStroke(255 * 0.3f*(t));
//																			
//																			time_over_label->addChild(KSTimer::create(32.f/30.f, [=]()
//																													  {
//																														  time_over_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 5.f/30.f, [=](float t)
//																																												  {
//																																													  time_over_label->setScale(1.f+t*0.6f);
//																																													  time_over_label->setOpacity(255-t*255);
//																																													  shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
//																																												  }, [=](float t)
//																																												  {
//																																													  time_over_label->setScale(1.6f);
//																																													  time_over_label->setOpacity(0);
//																																													  shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
//																																													  
//																																													  time_over_label->removeFromParent();
//																																												  }));
//																													  }));
//																		}));
			}
		}
	}
	
	int label_value;
	if(clr_cdt_type == kCLEAR_hellMode)
		label_value = use_time;
	else
		label_value = playtime_limit.getV()-countingCnt.getV();
	if(label_value < 0)
		label_value = 0;
	
	if(is_urgent)
	{
		countingLabel->setVisible(true);
		
		if(detail_counting_cnt%20 == 0)
		{
			if(countingLabel->getColor().b > 50)
				countingLabel->setColor(ccRED);
			else
				countingLabel->setColor(ccYELLOW);
		}
		
		
//		countingLabel->setOpacity(100);
//		countingLabel->setScale(4.f);
//		countingLabel->setPosition(ccp(240,myDSH->ui_center_y));
		if(!t_is_die)
		{
			if(clr_cdt_type == kCLEAR_hellMode)
			{
				countingLabel->setString(CCString::createWithFormat("%d.%d", label_value, detail_counting_cnt/6)->getCString());
				float percentage_value = 100.f*(float(label_value) + (detail_counting_cnt/6)/10.f)/(playtime_limit.getV()+1);
				progress_timer->setPercentage(percentage_value);
				if(percentage_value >= 100.f)
				{
					if(progress_timer->isVisible())
						conditionClear();
					else
						is_cleared_cdt = true;
				}
				
				mySGD->hell_play_time = float(label_value) + (detail_counting_cnt/6)/10.f;
			}
			else
				countingLabel->setString(CCString::createWithFormat("%d.%d", label_value, 9 - detail_counting_cnt/6)->getCString());
		}
		else
		{
			countingLabel->setString(CCString::createWithFormat("%d.%d", 0, 0)->getCString());
			stopCounting();
		}
	}
	else
	{
		if(countingLabel->getOpacity() == 100 && myGD->getCommunicationBool("Main_isFever"))
		{
			countingLabel->setVisible(true);
		}
		
		countingLabel->setColor(ccYELLOW);
		countingLabel->setOpacity(255);
		countingLabel->setScale(1.f);
		countingLabel->setPosition(ccp(240,32));
		if(clr_cdt_type == kCLEAR_hellMode)
		{
			countingLabel->setString(CCString::createWithFormat("%d.%d", label_value, detail_counting_cnt/6)->getCString());
			float percentage_value = 100.f*(float(label_value) + (detail_counting_cnt/6)/10.f)/(playtime_limit.getV()+1);
			progress_timer->setPercentage(percentage_value);
			if(percentage_value >= 100.f)
			{
				if(progress_timer->isVisible())
					conditionClear();
				else
					is_cleared_cdt = true;
			}
			mySGD->hell_play_time = float(label_value) + (detail_counting_cnt/6)/10.f;
		}
		else
			countingLabel->setString(CCString::createWithFormat("%d.%d", label_value, 9 - detail_counting_cnt/6)->getCString());
	}
	
}

void PlayUI::showTimeover()
{
	KSLabelTTF* time_over_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_failTitleTimeover), mySGD->getFont().c_str(), 45);
	time_over_label->setGradientColor(ccc4(255, 115, 250, 255), ccc4(215, 60, 130, 255), ccp(0,-1));
	time_over_label->enableOuterStroke(ccc3(65, 5, 35), 2.5f, 255, true);
	time_over_label->setPosition(ccp(240,myDSH->ui_center_y+93));
	time_over_label->setOpacity(0);
	addChild(time_over_label);
	
	KSLabelTTF* shadow = CommonAnimation::applyBigShadow(time_over_label, time_over_label->getFontSize());
	shadow->setOpacityOuterStroke(0);
	
	time_over_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 13.f/30.f, [=](float t)
															{
																float convert_t;
																if (t < 1 / 2.75)
																{
																	convert_t = 7.5625f * t * t;
																} else if (t < 2 / 2.75)
																{
																	t -= 1.5f / 2.75f;
																	convert_t = 7.5625f * t * t + 0.75f;
																} else if(t < 2.5 / 2.75)
																{
																	t -= 2.25f / 2.75f;
																	convert_t = 7.5625f * t * t + 0.9375f;
																}
																
																t -= 2.625f / 2.75f;
																convert_t = 7.5625f * t * t + 0.984375f;
																
																time_over_label->setPosition(ccp(240,myDSH->ui_center_y+93-93*convert_t));
																time_over_label->setOpacity(t*255);
																shadow->setOpacityOuterStroke(255 * 0.3f*(t));
															}, [=](float t)
															{
																time_over_label->setPosition(ccp(240,myDSH->ui_center_y));
																time_over_label->setOpacity(255);
																shadow->setOpacityOuterStroke(255 * 0.3f*(t));
																
																time_over_label->addChild(KSTimer::create(32.f/30.f, [=]()
																										  {
																											  time_over_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 5.f/30.f, [=](float t)
																																									  {
																																										  time_over_label->setScale(1.f+t*0.6f);
																																										  time_over_label->setOpacity(255-t*255);
																																										  shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																									  }, [=](float t)
																																									  {
																																										  time_over_label->setScale(1.6f);
																																										  time_over_label->setOpacity(0);
																																										  shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																										  
																																										  time_over_label->removeFromParent();
																																									  }));
																										  }));
															}));
}

void PlayUI::lifeBonus ()
{
	if(jack_life > 0)
	{
		jack_life--;
//		CCSprite* t_jack = (CCSprite*)jack_array->lastObject();
//		jack_array->removeObject(t_jack);
//		
//		CCParticleSystemQuad* t_particle = CCParticleSystemQuad::createWithTotalParticles(100);
//		t_particle->setPositionType(kCCPositionTypeRelative);
//		t_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("bonus_score_particle.png"));
//		t_particle->setEmissionRate(100);
//		t_particle->setAngle(90.0);
//		t_particle->setAngleVar(180.0);
//		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
//		t_particle->setBlendFunc(blendFunc);
//		t_particle->setDuration(-1.f);
//		t_particle->setEmitterMode(kCCParticleModeGravity);
//		t_particle->setStartColor(ccc4f(1.f, 0.8f, 0.4f, 1.f));
//		t_particle->setStartColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
//		t_particle->setEndColor(ccc4f(1.f, 1.f, 1.f, 0.f));
//		t_particle->setEndColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
//		t_particle->setStartSize(30.0);
//		t_particle->setStartSizeVar(10.0);
//		t_particle->setEndSize(-1.0);
//		t_particle->setEndSizeVar(0.0);
//		t_particle->setGravity(ccp(0,0));
//		t_particle->setRadialAccel(0.0);
//		t_particle->setRadialAccelVar(0.0);
//		t_particle->setSpeed(40);
//		t_particle->setSpeedVar(20.0);
//		t_particle->setTangentialAccel(0);
//		t_particle->setTangentialAccelVar(0);
//		t_particle->setTotalParticles(100);
//		t_particle->setLife(1.0);
//		t_particle->setLifeVar(0.25);
//		t_particle->setStartSpin(0);
//		t_particle->setStartSpinVar(0);
//		t_particle->setEndSpin(0);
//		t_particle->setEndSpinVar(0);
//		t_particle->setPosVar(ccp(10,10));
//		t_particle->setPosition(ccpAdd(t_jack->getPosition(), t_jack->getParent()->getPosition()));
//		t_particle->setAutoRemoveOnFinish(true);
//		addChild(t_particle);
//		
//		CCMoveTo* particle_move = CCMoveTo::create(0.5f, score_label->getPosition());
//		CCCallFuncO* particle_remove = CCCallFuncO::create(this, callfuncO_selector(PlayUI::removeParticle), t_particle);
//		CCSequence* particle_seq = CCSequence::createWithTwoActions(particle_move, particle_remove);
//		t_particle->runAction(particle_seq);
//		
//		CCFadeTo* jack_fade = CCFadeTo::create(0.5f, 0);
//		CCCallFunc* jack_remove = CCCallFunc::create(t_jack, callfunc_selector(CCSprite::removeFromParent));
//		CCSequence* jack_seq = CCSequence::createWithTwoActions(jack_fade, jack_remove);
//		t_jack->runAction(jack_seq);
		removeParticle(NULL);
	}
	else
	{
		CCLOG("keep percentage2 : %.4f", keep_percentage.getV());
		int grade_value = 1;
		if(is_exchanged && keep_percentage.getV() >= 1.f)	grade_value = 4;
		else if(is_exchanged)								grade_value = 3;
		else if(keep_percentage.getV() >= 1.f)				grade_value = 2;
		
		mySGD->gameClear(grade_value, int(getScore()), keep_percentage.getV(), countingCnt.getV(), use_time, total_time);
		CCDelayTime* n_d = CCDelayTime::create(2.5f);
		CCCallFunc* nextScene = CCCallFunc::create(this, callfunc_selector(PlayUI::nextScene));
		
		CCSequence* sequence = CCSequence::createWithTwoActions(n_d, nextScene);
		
		runAction(sequence);
	}
}
void PlayUI::removeParticle (CCObject * sender)
{
	if(sender)
		((CCParticleSystemQuad*)sender)->setDuration(0);
	
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_lifeBonusCnt)] = mySGD->replay_write_info.get(mySGD->getReplayKey(kReplayKey_lifeBonusCnt), Json::Value()).asInt() + 1;
	
	addScore(30000*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d), 0);
	lifeBonus();
}
void PlayUI::createBonusScore ()
{
	my_combo->stopKeep();
//	CCSprite* bonus_score = CCSprite::create("bonus_score.png");
//	bonus_score->setOpacity(0);
//	bonus_score->setPosition(ccp(240,myDSH->ui_center_y+50));
//	addChild(bonus_score);
//	
//	CCMoveTo* t_move = CCMoveTo::create(0.5f, ccp(240,myDSH->ui_bottom+20));
//	jack_life_node->runAction(t_move);
//	
//	CCFadeTo* t_fade = CCFadeTo::create(1.f, 255);
//	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PlayUI::lifeBonus));
//	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
//	bonus_score->runAction(t_seq);
	lifeBonus();
}
void PlayUI::endGame (bool is_show_reason)
{
	myGD->communication("GIM_stopCounting");
	
	AudioEngine::sharedInstance()->stopEffect("se_clock.mp3");
//	AudioEngine::sharedInstance()->stopEffect("sound_time_noti.mp3");
	//		myGD->communication("CP_setGameover");
	if(myGD->getIsGameover())
	{
		if(!is_show_reason)
		{
			keep_percentage = getPercentage();
			
			if(jack_life > 0 && !mySGD->is_hell_mode)
			{
				CCDelayTime* t_delay = CCDelayTime::create(4.f);
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PlayUI::createBonusScore));
				CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
				runAction(t_seq);
			}
			else
			{
				int grade_value = 1;
				
				if(is_exchanged && (beforePercentage^t_tta)/1000.f >= 1.f)		grade_value = 4;
				else if(is_exchanged)											grade_value = 3;
				else if((beforePercentage^t_tta)/1000.f >= 1.f)					grade_value = 2;
				
				if(mySGD->is_hell_mode)
					grade_value = 1;
				
				mySGD->gameClear(grade_value, int(getScore()), (beforePercentage^t_tta)/1000.f, countingCnt.getV(), use_time, total_time);
				CCDelayTime* n_d = CCDelayTime::create(4.5f);
				CCCallFunc* nextScene = CCCallFunc::create(this, callfunc_selector(PlayUI::nextScene));
				
				CCSequence* sequence = CCSequence::createWithTwoActions(n_d, nextScene);
				
				runAction(sequence);
			}
		}
		else
		{
			if(mySGD->is_hell_mode)
			{
				int grade_value = 1;
				
				mySGD->gameClear(grade_value, int(getScore()), (beforePercentage^t_tta)/1000.f, countingCnt.getV(), use_time, total_time);
				CCDelayTime* n_d = CCDelayTime::create(4.5f);
				CCCallFunc* nextScene = CCCallFunc::create(this, callfunc_selector(PlayUI::nextScene));
				
				CCSequence* sequence = CCSequence::createWithTwoActions(n_d, nextScene);
				
				runAction(sequence);
			}
			else
			{
				keep_percentage = getPercentage();
				
				CCDelayTime* n_d1 = CCDelayTime::create(4.5f);
				CCCallFunc* nextScene1 = CCCallFunc::create(this, callfunc_selector(PlayUI::searchEmptyPosition));
				CCDelayTime* n_d2 = CCDelayTime::create(2.f);
				CCFiniteTimeAction* nextScene2;
				if(mySGD->is_endless_mode)
					nextScene2 = CCCallFunc::create(this, callfunc_selector(PlayUI::cancelOnePercentGacha));
				else
					nextScene2 = CCCallFunc::create(this, callfunc_selector(PlayUI::showGachaOnePercent));
				
				CCSequence* sequence = CCSequence::create(n_d1, nextScene1, n_d2, nextScene2, NULL);
				
				runAction(sequence);
			}
		}
	}
	else
	{
		CCDelayTime* t_delay = CCDelayTime::create(2.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PlayUI::nextScene));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		runAction(t_seq);
	}
	
	myGD->communication("Main_hideThumb");
}
void PlayUI::showGachaOnePercent ()
{
	//OnePercentGacha* t_popup = OnePercentGacha::create(this, callfunc_selector(PlayUI::cancelOnePercentGacha), this, callfuncF_selector(PlayUI::gachaOnOnePercent), keep_percentage.getV());
//	OnePercentGame* t_popup = OnePercentGame::create(keep_percentage.getV(), bind(&PlayUI::cancelOnePercentGacha, this), bind(&PlayUI::gachaOnOnePercent, this, _1) );
//
//	getParent()->addChild(t_popup, 9999);
	auto tt = myDSH->getIntegerForKey(kSDH_Key_isShowOnePercentTutorial);
	if(myDSH->getIntegerForKey(kSDH_Key_isShowOnePercentTutorial))
	{
		OnePercentGame* t_popup = OnePercentGame::create(keep_percentage.getV(), bind(&PlayUI::cancelOnePercentGacha, this), bind(&PlayUI::gachaOnOnePercent, this, _1), true );
		
		getParent()->addChild(t_popup, 9999);
	
	}
	else
	{
		OnePercentTutorial* t_popup = OnePercentTutorial::create(keep_percentage.getV(), bind(&PlayUI::cancelOnePercentGacha, this), bind(&PlayUI::gachaOnOnePercent, this, _1) );
		getParent()->addChild(t_popup, 9999);
	}

}
void PlayUI::gachaOnOnePercent (float t_percent)
{
	myGD->communication("MS_removeEmptyPoint");
	
	vector<SaveUserData_Key> save_userdata_list;
	
	save_userdata_list.push_back(kSaveUserData_Key_star);
	
//	myDSH->saveUserData(save_userdata_list, nullptr);
	
	beforePercentage = (int(t_percent*1000))^t_tta;
	m_areaGage->setPercentage(t_percent);
	
	if(jack_life > 0)
	{
		CCLOG("keep percentage : %.4f", keep_percentage.getV());
		keep_percentage = t_percent;
		createBonusScore();
	}
	else
	{
		int grade_value = 1;
		
		if(is_exchanged && t_percent >= 1.f)		grade_value = 4;
		else if(is_exchanged)						grade_value = 3;
		else if(t_percent >= 1.f)					grade_value = 2;
		
		mySGD->gameClear(grade_value, int(getScore()), t_percent, countingCnt.getV(), use_time, total_time);
		nextScene();
	}
}
void PlayUI::searchEmptyPosition ()
{
	AudioEngine::sharedInstance()->playEffect("ment_99percent.mp3", false, true);
	
	CCPoint found_empty_position;
	bool break_flag = false;
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd && !break_flag;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd && !break_flag;j++)
		{
			if(myGD->mapState[i][j] == mapEmpty && mySD->silData[i][j])
			{
				break_flag = true;
				found_empty_position = IntPoint(i,j).convertToCCP();
			}
		}
	}
	
	myGD->communication("MS_showEmptyPoint", found_empty_position);
	myGD->communication("Main_startMoveToCCPoint", found_empty_position);
}
void PlayUI::cancelOnePercentGacha ()
{
	myGD->communication("MS_removeEmptyPoint");
	
	if(jack_life > 0)
	{
		createBonusScore();
	}
	else
	{
		int grade_value = 1;
		
		if(is_exchanged && keep_percentage.getV() >= 1.f)		grade_value = 4;
		else if(is_exchanged)									grade_value = 3;
		else if(keep_percentage.getV() >= 1.f)					grade_value = 2;
		
		mySGD->gameClear(grade_value, int(getScore()), keep_percentage.getV(), countingCnt.getV(), use_time, total_time);
		nextScene();
	}
}
void PlayUI::nextScene ()
{
	if(mySGD->is_write_replay)
	{
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_scoreIndex)] = mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreTime)].size();
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreTime)].append(use_time);
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreData)].append(int(mySGD->getScore()));
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_isChangedScore)] = false;
	}
	if(result_sprite)
		result_sprite->setVisible(false);
	myGD->communication("Main_gameover");
}
void PlayUI::catchSubCumber ()
{
	if(is_cleared_cdt || clr_cdt_type != kCLEAR_subCumberCatch || isGameover)
		return;
	
	ing_cdt_cnt = ing_cdt_cnt.getV()+1;
	
	mission_button->setTextAtIndex(CCString::createWithFormat("%d/%d", ing_cdt_cnt.getV(), clr_cdt_cnt.getV())->getCString(), 1);
	((CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel))->setString(CCString::createWithFormat("%d", ing_cdt_cnt.getV())->getCString());
	if(ing_cdt_cnt.getV() >= clr_cdt_cnt.getV())		conditionClear();
}
void PlayUI::takeBigArea ()
{
	if(is_cleared_cdt || clr_cdt_type != kCLEAR_bigArea || isGameover)
		return;
	
	ing_cdt_cnt = ing_cdt_cnt.getV()+1;
	
	mission_button->setTextAtIndex(CCString::createWithFormat("%2.0f%%:%d/%d", clr_cdt_per*100.f, ing_cdt_cnt.getV(), clr_cdt_cnt.getV())->getCString(), 1);
//	((CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel))->setString(CCString::createWithFormat("%2.0f%%:%d/%d", (clr_cdt_per-item_value/100.f)*100.f, ing_cdt_cnt, clr_cdt_cnt)->getCString());
	if(ing_cdt_cnt.getV() >= clr_cdt_cnt.getV())		conditionClear();
}
void PlayUI::takeItemCollect ()
{
	if(is_cleared_cdt || clr_cdt_type != kCLEAR_itemCollect || isGameover)
		return;
	
	ing_cdt_cnt = ing_cdt_cnt.getV()+1;
	
	mission_button->setTextAtIndex(CCString::createWithFormat("%d/%d", ing_cdt_cnt.getV(), clr_cdt_cnt.getV())->getCString(), 1);
	((CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel))->setString(CCString::createWithFormat("%d", ing_cdt_cnt.getV())->getCString());
	if(ing_cdt_cnt.getV() >= clr_cdt_cnt.getV())		conditionClear();
}

void PlayUI::myInit ()
{
	mySGD->is_god_of_death = false;
	mission_oper_list.clear();
	
	CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
	string t_missionInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missionInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
	Json::Value t_json;
	Json::Reader t_reader;
	t_reader.parse(t_missionInfo, t_json);
	for(int i=0;i<t_json.size();i++)
	{
		Json::Value t_mi = t_json[i];
		MissionOper t_mo;
		t_mo.m_type = t_mi["type"].asInt();
		t_mo.prop = t_mi["prop"].asString();
		t_mo.oper = t_mi["oper"].asString();
		t_mo.value = t_mi["value"].asDouble();
		mission_oper_list[t_mo.m_type] = t_mo;
	}
	
	isGameover = false;
	
	is_on_clear_time_event = false;
	
	result_sprite = NULL;
	
	jack_array = NULL;
	exchange_dic = NULL;
	
	bomb_img = NULL;
	ing_bomb_value = 0;
	
	score_value = 0;
	sub_score_value = 0;
	damaged_score = 0;
	
	percentage_decrease_cnt = 0;
	combo_cnt = 0;
	high_combo_cnt = 0;
	
	turn_cnt = 0;
	casting_cancel_cnt = 0;
	
	score_attack_damage = 0;
	
	clearPercentage = 1;
	
	if(mySD->getSilType() == 1 && !myDSH->getBoolForKey(kDSH_Key_isDisableDrawButton))
	{
		mySGD->is_draw_button_tutorial = true;
		mySGD->draw_button_tutorial_ing = 1;
		draw_button_tutorial_show = 11;
	}
	else
	{
		mySGD->is_draw_button_tutorial = false;
		mySGD->draw_button_tutorial_ing = 0;
	}
	
	gold_label = GoldLabel::create();
	addChild(gold_label);
	
	KSLabelTTF* t_label = KSLabelTTF::create("", mySGD->getFont().c_str(), 7);
	t_label->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
	t_label->enableOuterStroke(ccc3(60, 20, 0), 0.5f, 255, true);
	t_label->setAnchorPoint(ccp(1,0.5f));
	t_label->setPosition(gold_label->getPosition() + ccp(0,-8));
	addChild(t_label);
	
	mySGD->setIngameGoldLabel(gold_label, t_label);
	
	myGD->V_I["UI_checkStageGoldMission"] = std::bind(&PlayUI::checkStageGoldMission, this, _1);
	
	gold_label->setString("0");
	
	CCPoint gold_origin_position = gold_label->getPosition();
	
	addChild(KSGradualValue<float>::create(gold_origin_position.y+UI_OUT_DISTANCE, gold_origin_position.y, UI_IN_TIME, [=](float t){gold_label->setPositionY(t);}, [=](float t){gold_label->setPositionY(gold_origin_position.y);}));
	
	CCSprite* gold_img = CCSprite::create("ui_gold_img.png");
	gold_img->setPosition(ccpAdd(gold_label->getPosition(), ccp(-50,0)));
	addChild(gold_img);
	
	CCPoint gold_img_origin_position = gold_img->getPosition();
	
	addChild(KSGradualValue<float>::create(gold_img_origin_position.y+UI_OUT_DISTANCE, gold_img_origin_position.y, UI_IN_TIME, [=](float t){gold_img->setPositionY(t);}, [=](float t){gold_img->setPositionY(gold_img_origin_position.y);}));
	
	thumb_node = CCNode::create();
	addChild(thumb_node);
	
	
	
	if(mySGD->is_endless_mode)
	{
		score_label = CountingBMLabel::create("0", "endless_score.fnt", 2.f, "%.0f");
		((CountingBMLabel*)score_label)->onChangeScale(false);
		
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
		
		score_label->setAnchorPoint(ccp(0.5f,0.5f));
		score_label->setPosition(ccp(40-UI_OUT_DISTANCE,myDSH->ui_center_y-10) + ccp(0,-215.f*0.17f+10));
		
		addChild(KSGradualValue<float>::create(40-UI_OUT_DISTANCE, 40, UI_IN_TIME, [=](float t){score_label->setPositionX(t);}, [=](float t){score_label->setPositionX(40);}));
		
		string flag = myDSH->getStringForKey(kDSH_Key_flag);
		CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
		selectedFlagSpr->setPosition(ccp(40-UI_OUT_DISTANCE,myDSH->ui_center_y-10) + ccp(-20,215.f*0.17f+8));
		selectedFlagSpr->setScale(0.5f);
		thumb_node->addChild(selectedFlagSpr);
		
		addChild(KSGradualValue<float>::create(40-20-UI_OUT_DISTANCE, 40-20, UI_IN_TIME, [=](float t){selectedFlagSpr->setPositionX(t);}, [=](float t){selectedFlagSpr->setPositionX(40-20);}));
		
		KSLabelTTF* nick_label = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 10);
		nick_label->setAnchorPoint(ccp(0,0.5f));
		nick_label->setPosition(ccp(40-UI_OUT_DISTANCE,myDSH->ui_center_y-10) + ccp(-20,215.f*0.17f+8) + ccp(selectedFlagSpr->getContentSize().width/2.f*selectedFlagSpr->getScale()+2, 0));
		thumb_node->addChild(nick_label);
		
		addChild(KSGradualValue<float>::create(40-20+selectedFlagSpr->getContentSize().width/2.f*selectedFlagSpr->getScale()+2-UI_OUT_DISTANCE, 40-20+selectedFlagSpr->getContentSize().width/2.f*selectedFlagSpr->getScale()+2,
											   UI_IN_TIME, [=](float t){nick_label->setPositionX(t);}, [=](float t)
		{
			nick_label->setPositionX(40-20+selectedFlagSpr->getContentSize().width/2.f*selectedFlagSpr->getScale()+2);
			
			endless_me_bomb_node = CCNode::create();
			endless_me_bomb_node->setPosition(ccp(40, myDSH->ui_center_y+60));
			addChild(endless_me_bomb_node, -1);
			
			CCClippingNode* me_bomb_clipping = CCClippingNode::create(CCSprite::create("endless_bomb_mask.png"));
			me_bomb_clipping->setAlphaThreshold(0.1f);
			me_bomb_clipping->setPosition(ccp(0, 0));
			endless_me_bomb_node->addChild(me_bomb_clipping, -1);
			
			CCSprite* me_bomb_img = CCSprite::create("endless_bomb.png");
			me_bomb_img->setPosition(ccp(0, 0));
			endless_me_bomb_node->addChild(me_bomb_img);
			
			auto temp = KS::loadCCBI<CCSprite*>(this, "endless_bomb_me.ccbi");
			bomb_img = temp.first;
			bomb_img->setPosition(ccp(0, -54));
			me_bomb_clipping->addChild(bomb_img);
		}));
		thumb_node->addChild(score_label);
		
		sub_score_label = KSLabelTTF::create("", mySGD->getFont().c_str(), 8);
		sub_score_label->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
		sub_score_label->enableOuterStroke(ccc3(60, 20, 0), 0.5f, 255, true);
		sub_score_label->setAnchorPoint(ccp(0,0.5f));
		sub_score_label->setPosition(ccp(40,myDSH->ui_center_y-10) + ccp(0,-215.f*0.17f+10) + ccp(-3,-7));
		thumb_node->addChild(sub_score_label);
	}
	else
	{
		score_label = CountingBMLabel::create("0", "scorefont.fnt", 2.f, "%d");
		((CountingBMLabel*)score_label)->onChangeScale(false);
		
		score_label->setAnchorPoint(ccp(1,1));
		score_label->setPosition(ccp(480-8,myDSH->ui_top-14+UI_OUT_DISTANCE));
		
		addChild(KSGradualValue<float>::create(myDSH->ui_top-14+UI_OUT_DISTANCE, myDSH->ui_top-14, UI_IN_TIME, [=](float t){score_label->setPositionY(t);}, [=](float t){score_label->setPositionY(myDSH->ui_top-14);}));
		addChild(score_label);
		
		sub_score_label = KSLabelTTF::create("", mySGD->getFont().c_str(), 10);
		sub_score_label->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
		sub_score_label->enableOuterStroke(ccc3(60, 20, 0), 0.7f, 255, true);
		sub_score_label->setAnchorPoint(ccp(1,0.5f));
		sub_score_label->setPosition(ccp(480-8,myDSH->ui_top-14) + ccp(3,-17));
		addChild(sub_score_label);
	}
	
	top_center_node = CCNode::create();
	
	addChild(KSGradualValue<float>::create(4+UI_OUT_DISTANCE, 4, UI_IN_TIME, [=](float t){top_center_node->setPositionY(t);}, [=](float t){top_center_node->setPositionY(4);}));
	
	addChild(top_center_node, 1);
	
	m_areaGage = NULL;
	
//	percentageLabel = KSLabelTTF::create("0%", mySGD->getFont().c_str(), 14);// CCLabelTTF::create("0%%", mySGD->getFont().c_str(), 14);
	percentageLabel = CCLabelBMFont::create("0%", "star_gage_font.fnt");
	percentageLabel->setAnchorPoint(ccp(0.5, 0.5));
//	percentageLabel->enableOuterStroke(ccBLACK, 1.f);
	percentageLabel->setPosition(ccp(182,myDSH->ui_top-29));//29
	
//	if(myGD->gamescreen_type == kGT_leftUI)			percentageLabel->setPosition(ccp(36,myDSH->ui_center_y));
//	else if(myGD->gamescreen_type == kGT_rightUI)		percentageLabel->setPosition(ccp(480-50+36,myDSH->ui_center_y));
//	else									percentageLabel->setPosition(ccp(470,myDSH->ui_top-60));
	top_center_node->addChild(percentageLabel, 1);
	
	if(mySD->getClearCondition() == kCLEAR_hellMode)
		percentageLabel->setVisible(false);
	
//	CCSprite* percentage_gain = CCSprite::create("ui_gain.png");
//	percentage_gain->setAnchorPoint(ccp(0,0.5));
//	percentage_gain->setPosition(ccp(205,myDSH->ui_top-35));
//	addChild(percentage_gain);
//	
//	CCSprite* percentage_p = CCSprite::create("maingame_percentage.png");
//	percentage_p->setAnchorPoint(ccp(0,0.5));
//	percentage_p->setPosition(ccpAdd(percentageLabel->getPosition(), ccp(2,0)));
//	addChild(percentage_p);
	
	
	//		CCSprite* counting_tiem = CCSprite::create("maingame_time.png");
	//		counting_tiem->setPosition(ccp(140, 410));
	//		addChild(counting_tiem);
	
	countingCnt = 0;//-mySGD->getLongTimeValue();
	detail_counting_cnt = 0;
	is_urgent = false;
	use_time = 0;
	playtime_limit = mySDS->getIntegerForKey(kSDF_stageInfo, mySD->getSilType(), "playtime")-1;
	total_time = playtime_limit.getV();
	
	clr_cdt_type = mySD->getClearCondition();
	
	if(clr_cdt_type == kCLEAR_timeLimit)
	{
		int time_limit_value = mySD->getClearConditionTimeLimit();
		map<int, MissionOper>::iterator t_iter = mission_oper_list.find((int)clr_cdt_type);
		if(t_iter != mission_oper_list.end())
		{
			if(t_iter->second.oper == "-")
				time_limit_value -= t_iter->second.value;
			else if(t_iter->second.oper == "*")
				time_limit_value *= t_iter->second.value;
			
			myGD->communication("Jack_showMissionEffect", int(clr_cdt_type));
			
			if(time_limit_value < 0)
				time_limit_value = 0;
		}
		
		playtime_limit = playtime_limit.getV() - time_limit_value;
		total_time = total_time - time_limit_value;
	}
	
	
	if(clr_cdt_type == kCLEAR_hellMode)
	{
		score_label->setVisible(false);
	}
	
//	CCSprite* time_back = CCSprite::create("ui_time_back.png");
//	time_back->setPosition(ccp(240-25,myDSH->ui_top-25));
//	if(myGD->gamescreen_type == kGT_leftUI)			time_back->setPosition(ccp((480-50-myGD->boarder_value*2)*3.1f/4.f+50+myGD->boarder_value,myDSH->ui_top-25));
//	else if(myGD->gamescreen_type == kGT_rightUI)	time_back->setPosition(ccp((480-50-myGD->boarder_value*2)*3.1f/4.f+myGD->boarder_value,myDSH->ui_top-25));
//	else											time_back->setPosition(ccp(480.f*3.1f/4.f,myDSH->ui_top-25));
//	addChild(time_back);
	
	int counting_label_init_value;
	if(clr_cdt_type == kCLEAR_hellMode)
		counting_label_init_value = 0;
	else
		counting_label_init_value = playtime_limit.getV()-countingCnt.getV()+1;
	
	countingLabel = CCLabelBMFont::create(CCString::createWithFormat("%d", counting_label_init_value)->getCString(), "timefont.fnt");
	countingLabel->setAlignment(kCCTextAlignmentCenter);
	countingLabel->setAnchorPoint(ccp(0.5f,0.5f));
	countingLabel->setPosition(ccp(240,32-UI_OUT_DISTANCE));
	countingLabel->setColor(ccYELLOW);
	addChild(countingLabel);
	
	addChild(KSGradualValue<float>::create(32-UI_OUT_DISTANCE, 32, UI_IN_TIME, [=](float t){countingLabel->setPositionY(t);}, [=](float t){countingLabel->setPositionY(32);}));
	
	isFirst = true;
	//		beforePercentage = 0;
	
	
	CCSprite* n_home = CCSprite::create("ui_stop.png");
	CCSprite* s_home = CCSprite::create("ui_stop.png");
	s_home->setColor(ccGRAY);
	
	CCMenuItem* home_item = CCMenuItemSprite::create(n_home, s_home, this, menu_selector(PlayUI::menuAction));
	home_item->setTag(kMenuTagUI_home);
	home_item->setStringData("backkey");
	home_menu = CCMenu::createWithItem(home_item);
	home_menu->setPosition(ccp(25, myDSH->ui_top-25+UI_OUT_DISTANCE));
	home_menu->setEnabled(false);
	
	addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){home_menu->setPositionY(t);}, [=](float t){home_menu->setPositionY(myDSH->ui_top-25);home_menu->setEnabled(true);}));
	
//	if(myGD->gamescreen_type == kGT_leftUI)				home_menu->setPosition(ccp(25,myDSH->ui_top-25));
//	else if(myGD->gamescreen_type == kGT_rightUI)		home_menu->setPosition(ccp(480-25,myDSH->ui_top-25));
//	else												home_menu->setPosition(ccp(25,myDSH->ui_top-25));
	addChild(home_menu);
	
	
	jack_array = new CCArray(1);
	jack_life_hide_count = 0;
	
	jack_life = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_int2_life_i, t_history.characterIndex.getV(), t_history.characterLevel.getV())-1;//NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_life_i, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1)-1;
	
	is_used_heartUpItem = false;
	is_used_longTimeItem = false;
	
//	if(mySGD->isUsingItem(kIC_heartUp))
//		jack_life++;
	
	jack_life_node = CCNode::create();
	jack_life_node->setPosition(ccp(240,myDSH->ui_bottom-70-UI_OUT_DISTANCE));
	
	addChild(KSGradualValue<float>::create(myDSH->ui_bottom-70-UI_OUT_DISTANCE, myDSH->ui_bottom-70, UI_IN_TIME, [=](float t){jack_life_node->setPositionY(t);}, [=](float t){jack_life_node->setPositionY(myDSH->ui_bottom-70);}));
	
	addChild(jack_life_node);
	
	CCNode* black_life_node = CCNode::create();
	black_life_node->setPosition(CCPointZero);
	jack_life_node->addChild(black_life_node,-1);
	
	CCPoint life_base_position = ccpMult(ccp(-50,0), (jack_life-1)/2.f);
	
	for(int i=0;i<jack_life.getV();i++)
	{
		CCSprite* black_img = CCSprite::create("ingame_life.png");
		black_img->setColor(ccBLACK);
		black_img->setPosition(ccpAdd(life_base_position, ccp(i*50, 0)));
		black_life_node->addChild(black_img);
		
		CCSprite* jack_img = CCSprite::create("ingame_life.png");
		jack_img->setPosition(ccpAdd(life_base_position, ccp(i*50, 0)));
		jack_life_node->addChild(jack_img);
		
		jack_array->addObject(jack_img);
	}
	
	is_exchanged = false;
	is_show_exchange_coin = false;
	exchange_dic = new CCDictionary();
	
	for(int i=1;i<=6;i++)
	{
		CCSprite* exchange_spr = CCSprite::create(CCString::createWithFormat("exchange_%d_unact.png", i)->getCString());
		if(myGD->gamescreen_type == kGT_leftUI)			exchange_spr->setPosition(ccp(240-20*3.5f+i*20,myDSH->ui_top-50));
		else if(myGD->gamescreen_type == kGT_rightUI)		exchange_spr->setPosition(ccp(240-20*3.5f+i*20,myDSH->ui_top-50));
		else									exchange_spr->setPosition(ccp(240-20*3.5f+i*20,myDSH->ui_top-50));
		top_center_node->addChild(exchange_spr);
		
		exchange_spr->setVisible(false);
		
		exchange_dic->setObject(exchange_spr, i);
	}
	
	is_show_condition = false;
	
	mission_button = RollingButton::create("");
	mission_button->setPosition(ccp(68, myDSH->ui_top-22+UI_OUT_DISTANCE));
	mission_button->setVisible(false); // 원래 주석
	addChild(KSGradualValue<float>::create(myDSH->ui_top-22+UI_OUT_DISTANCE, myDSH->ui_top-22, UI_IN_TIME, [=](float t){mission_button->setPositionY(t);}, [=](float t){mission_button->setPositionY(myDSH->ui_top-22);}));
	
	addChild(mission_button,2);
	
	mission_button->startMarquee();
	
	mission_button->setOpenFunction([&](){
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
		mission_button->runAction(CCMoveBy::create(0.3,ccp(174,0)));
//		top_center_node->setVisible(false);
	});
	
	mission_button->setCloseFunction([&](){
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
		mission_button->runAction(CCMoveBy::create(0.3,ccp(-174,0)));
//		top_center_node->setVisible(true);
	});
	
//	CCPoint icon_menu_position;
//	if(myGD->gamescreen_type == kGT_leftUI)				icon_menu_position = ccp(25,myDSH->ui_center_y+43);
//	else if(myGD->gamescreen_type == kGT_rightUI)		icon_menu_position = ccp(480-25,myDSH->ui_center_y+43);
//	else												icon_menu_position = ccp(440,myDSH->ui_top-25);
//	
//	CCSprite* condition_back = CCSprite::create("ui_condition_back.png");
//	condition_back->setPosition(icon_menu_position);
//	addChild(condition_back);
	
	progress_timer = NULL;
	
	mission_back = NULL;
	mission_clear_remove_nodes.clear();
	
	if(clr_cdt_type == kCLEAR_bossLifeZero)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
//		CCSprite* n_icon = CCSprite::create("condition1_menu.png");
//		CCSprite* s_icon = CCSprite::create("condition1_menu.png");
//		s_icon->setColor(ccGRAY);
//		
//		CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
//		icon_item->setTag(kMenuTagUI_condition);
//		
//		CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
//		icon_menu->setPosition(icon_menu_position);
//		addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
		
		int start_percentage = 100;
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		mission_button->addText(CCString::createWithFormat("%d%%", start_percentage)->getCString());
		
//		CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d%%", start_percentage)->getCString(), mySGD->getFont().c_str(), 12);
//		clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
//		addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
	}
	else if(clr_cdt_type == kCLEAR_subCumberCatch)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
//		CCSprite* n_icon = CCSprite::create("condition2_menu.png");
//		CCSprite* s_icon = CCSprite::create("condition2_menu.png");
//		s_icon->setColor(ccGRAY);
//		
//		CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
//		icon_item->setTag(kMenuTagUI_condition);
//		
//		CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
//		icon_menu->setPosition(icon_menu_position);
//		addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
		
		int catch_sub_cumber_value = mySD->getClearConditionCatchSubCumber();
		map<int, MissionOper>::iterator t_iter = mission_oper_list.find((int)clr_cdt_type);
		if(t_iter != mission_oper_list.end())
		{
			if(t_iter->second.oper == "-")
				catch_sub_cumber_value -= t_iter->second.value;
			else if(t_iter->second.oper == "*")
				catch_sub_cumber_value *= t_iter->second.value;
			
			myGD->communication("Jack_showMissionEffect", int(clr_cdt_type));
			
			if(catch_sub_cumber_value < 0)
				catch_sub_cumber_value = 0;
		}
		
		clr_cdt_cnt = catch_sub_cumber_value;
		ing_cdt_cnt = 0;
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		mission_button->addText(CCString::createWithFormat("%d/%d", ing_cdt_cnt.getV(), clr_cdt_cnt.getV())->getCString());
		
		mission_back = CCSprite::create("ui_mission_button_open.png");
		mission_back->setPosition(ccp(80, myDSH->ui_top-25+UI_OUT_DISTANCE));
		addChild(mission_back, 2);
		mission_back->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){mission_back->setPositionY(t);}, [=](float t){mission_back->setPositionY(myDSH->ui_top-25);}));
		
		CCNode* junior_node = CCNode::create();
		junior_node->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(-18.5f,0));
		mission_back->addChild(junior_node);
		
		std::string juniorInfo = mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "junior");
		
		Json::Reader reader;
		Json::Value juniorJson;
		reader.parse(juniorInfo, juniorJson);
		
		int juniorIndex = 0;
		juniorJson = juniorJson[juniorIndex];
		{
			std::string juniorType = juniorJson["type"].asString();
			std::string ccbiName = juniorType;
			std::string ccbiname2 = ccbiName;
			
			if(ccbiname2.substr(0,1)=="1") {
				ccbiname2="bear";
			}
			////////////////////////////////////////////
			
			//		m_directionAngleDegree = m_well512.GetValue(0, 360);
			
			//		CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			//		ccNodeLoaderLibrary->registerCCNodeLoader("CircleBossCCB", CircleLoader::loader());
			
			//		cocos2d::extension::CCBReader* reader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
			
			
			std::string _ccbiName = (ccbiname2 + ".ccbi").c_str();
			CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader* reader = new cocos2d::extension::CCBReader(nodeLoader);
			CCNode* p = reader->readNodeGraphFromFileForFullPath((mySIL->getDocumentPath()+_ccbiName).c_str(), this);
			
			CCSprite* m_juniorSprite = dynamic_cast<CCSprite*>(p);
			m_juniorSprite->addChild(reader->getAnimationManager());
//			m_juniorAnimation = reader->getAnimationManager();
//			m_juniorAnimation->setDelegate(this);
			reader->release();
			
			if(m_juniorSprite != NULL)
			{
				junior_node->addChild(m_juniorSprite, 1001);
				m_juniorSprite->setScale(0.5f);
				m_juniorSprite->setPosition(CCPointZero);
			}
		}
		
		CCLabelTTF* t_condition_label = CCLabelTTF::create(CCString::createWithFormat("/%d", clr_cdt_cnt.getV())->getCString(), mySGD->getFont().c_str(), 12);
		t_condition_label->setAnchorPoint(ccp(0,0.5f));
		t_condition_label->setPosition(mission_back->getPosition() + ccp(10,0));
		addChild(t_condition_label, 2);
		t_condition_label->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){t_condition_label->setPositionY(t);}, [=](float t){t_condition_label->setPositionY(myDSH->ui_top-25);}));
		mission_clear_remove_nodes.push_back(t_condition_label);
		
		CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", ing_cdt_cnt.getV())->getCString(), mySGD->getFont().c_str(), 16);
		clr_cdt_label->setColor(ccc3(255, 170, 20));
		clr_cdt_label->setAnchorPoint(ccp(1,0.5f));
		clr_cdt_label->setPosition(mission_back->getPosition() + ccp(10,0));
		addChild(clr_cdt_label, 2, kCT_UI_clrCdtLabel);
		clr_cdt_label->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){clr_cdt_label->setPositionY(t);}, [=](float t){clr_cdt_label->setPositionY(myDSH->ui_top-25);}));
		mission_clear_remove_nodes.push_back(clr_cdt_label);
		
		if(clr_cdt_cnt.getV() <= ing_cdt_cnt.getV())
			conditionClear();
	}
	else if(clr_cdt_type == kCLEAR_bigArea)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
//		CCSprite* n_icon = CCSprite::create("condition3_menu.png");
//		CCSprite* s_icon = CCSprite::create("condition3_menu.png");
//		s_icon->setColor(ccGRAY);
//		
//		CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
//		icon_item->setTag(kMenuTagUI_condition);
//		
//		CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
//		icon_menu->setPosition(icon_menu_position);
//		addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
		
		clr_cdt_per = mySD->getClearConditionBigAreaPer();
		clr_cdt_cnt = mySD->getClearConditionBigAreaCnt();
		ing_cdt_cnt = 0;
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		mission_button->addText(CCString::createWithFormat("%2.0f%%:%d/%d", clr_cdt_per*100.f, ing_cdt_cnt.getV(), clr_cdt_cnt.getV())->getCString());
//		
//		CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%2.0f%%:%d/%d", (clr_cdt_per-item_value/100.f)*100.f, ing_cdt_cnt, clr_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
//		clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
//		addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
	}
	else if(clr_cdt_type == kCLEAR_itemCollect)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
//		CCSprite* n_icon = CCSprite::create("condition4_menu.png");
//		CCSprite* s_icon = CCSprite::create("condition4_menu.png");
//		s_icon->setColor(ccGRAY);
//		
//		CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
//		icon_item->setTag(kMenuTagUI_condition);
//		
//		CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
//		icon_menu->setPosition(icon_menu_position);
//		addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
		
		int item_collect_value = mySD->getClearConditionItemCollect();
		map<int, MissionOper>::iterator t_iter = mission_oper_list.find((int)clr_cdt_type);
		if(t_iter != mission_oper_list.end())
		{
			if(t_iter->second.oper == "-")
				item_collect_value -= t_iter->second.value;
			else if(t_iter->second.oper == "*")
				item_collect_value *= t_iter->second.value;
			
			myGD->communication("Jack_showMissionEffect", int(clr_cdt_type));
			
			if(item_collect_value < 0)
				item_collect_value = 0;
		}
		
		clr_cdt_cnt = item_collect_value;
		ing_cdt_cnt = 0;
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		mission_button->addText(CCString::createWithFormat("%d/%d", ing_cdt_cnt.getV(), clr_cdt_cnt.getV())->getCString());
		
		
		mission_back = CCSprite::create("ui_mission_button_open.png");
		mission_back->setPosition(ccp(80, myDSH->ui_top-25+UI_OUT_DISTANCE));
		addChild(mission_back, 2);
		mission_back->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){mission_back->setPositionY(t);}, [=](float t){mission_back->setPositionY(myDSH->ui_top-25);}));
		
		CCSprite* item_img = CCSprite::create("mission_item.png");
		item_img->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(-21,0));
		mission_back->addChild(item_img);
		
		
		CCLabelTTF* t_condition_label = CCLabelTTF::create(CCString::createWithFormat("/%d", clr_cdt_cnt.getV())->getCString(), mySGD->getFont().c_str(), 12);
		t_condition_label->setAnchorPoint(ccp(0,0.5f));
		t_condition_label->setPosition(mission_back->getPosition() + ccp(10,-1));
		addChild(t_condition_label, 2);
		t_condition_label->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){t_condition_label->setPositionY(t);}, [=](float t){t_condition_label->setPositionY(myDSH->ui_top-25);}));
		mission_clear_remove_nodes.push_back(t_condition_label);
		
		CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", ing_cdt_cnt.getV())->getCString(), mySGD->getFont().c_str(), 16);
		clr_cdt_label->setColor(ccc3(255, 170, 20));
		clr_cdt_label->setAnchorPoint(ccp(1,0.5f));
		clr_cdt_label->setPosition(mission_back->getPosition() + ccp(10,0));
		addChild(clr_cdt_label, 2, kCT_UI_clrCdtLabel);
		clr_cdt_label->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){clr_cdt_label->setPositionY(t);}, [=](float t){clr_cdt_label->setPositionY(myDSH->ui_top-25);}));
		mission_clear_remove_nodes.push_back(clr_cdt_label);
		
		
		if(clr_cdt_cnt.getV() <= ing_cdt_cnt.getV())
			conditionClear();
	}
	else if(clr_cdt_type == kCLEAR_perfect)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
//		CCSprite* n_icon = CCSprite::create("condition5_menu.png");
//		CCSprite* s_icon = CCSprite::create("condition5_menu.png");
//		s_icon->setColor(ccGRAY);
//		
//		CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
//		icon_item->setTag(kMenuTagUI_condition);
//		
//		CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
//		icon_menu->setPosition(icon_menu_position);
//		addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
		
		clr_cdt_per = mySD->getClearConditionPerfectBase();
		clr_cdt_range = mySD->getClearConditionPerfectRange();
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		mission_button->addText(CCString::createWithFormat("%.0f", clr_cdt_per*100.f)->getCString());
		
//		CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", clr_cdt_per*100.f)->getCString(), mySGD->getFont().c_str(), 12);
//		clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
//		addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
	}
	else if(clr_cdt_type == kCLEAR_sequenceChange)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
//		CCSprite* n_icon = CCSprite::create("condition6_menu.png");
//		CCSprite* s_icon = CCSprite::create("condition6_menu.png");
//		s_icon->setColor(ccGRAY);
//		
//		CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
//		icon_item->setTag(kMenuTagUI_condition);
//		
//		CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
//		icon_menu->setPosition(icon_menu_position);
//		addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
		
		ing_cdt_cnt = 1;
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		mission_button->addText(CCString::createWithFormat("exchange_%d_act.png", ing_cdt_cnt.getV())->getCString());
		
//		CCSprite* clr_cdt_img = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", ing_cdt_cnt)->getCString());
//		clr_cdt_img->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
//		addChild(clr_cdt_img, 0, kCT_UI_clrCdtLabel);
	}
	else if(clr_cdt_type == kCLEAR_timeLimit)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
//		CCSprite* n_icon = CCSprite::create("condition7_menu.png");
//		CCSprite* s_icon = CCSprite::create("condition7_menu.png");
//		s_icon->setColor(ccGRAY);
//		
//		CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
//		icon_item->setTag(kMenuTagUI_condition);
//		
//		CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
//		icon_menu->setPosition(icon_menu_position);
//		addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		mission_button->addText(CCString::createWithFormat("%d", playtime_limit.getV())->getCString());
	}
	else if(clr_cdt_type == kCLEAR_hellMode)
	{
		is_cleared_cdt = false;
		
		progress_timer = ConvexGraph::create("loading_progress_front2.png", CCRectMake(0, 0, 13, 13), CCRectMake(6, 6, 1, 1), CCSizeMake(201, 13), ConvexGraphType::width);
		progress_timer->setPosition(ccp(240,myDSH->ui_top-25));
		addChild(progress_timer);
		
		progress_timer->setCover("loading_progress_front1.png", "loading_progress_mask.png");
		progress_timer->setBack("loading_progress_back.png");
		
		CCSprite* star_img = CCSprite::create("star_gold.png");
		star_img->setScale(0.5f);
		star_img->setPosition(ccp(110,2));
		progress_timer->addChild(star_img);
		
		int hell_cnt = NSDS_GI(kSDS_GI_hellMode_listCount_i);
		bool is_found = false;
		for(int i=0;!is_found && i<hell_cnt;i++)
		{
			if(mySD->getSilType() == NSDS_GI(kSDS_GI_hellMode_int1_pieceNo_i, i+1))
			{
				is_found = true;
				int cha_no = NSDS_GI(kSDS_GI_hellMode_int1_characterNo_i, i+1);
				
				int history_size = mySGD->getCharacterHistorySize();
				bool is_found2 = false;
				for(int j=0;!is_found2 && j<history_size;j++)
				{
					CharacterHistory t_history = mySGD->getCharacterHistory(j);
					if(t_history.characterNo == cha_no)
					{
						is_found2 = true;
						if(mySGD->isClearPiece(mySD->getSilType()))
						{
							progress_timer->setVisible(false);
						}
					}
				}
			}
		}
	}
	else if(clr_cdt_type == kCLEAR_percentage)
	{
		is_cleared_cdt = false;
	}
	else if(clr_cdt_type == kCLEAR_score)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
		mission_back = CCSprite::create("ui_mission_button_open.png");
		mission_back->setPosition(ccp(80, myDSH->ui_top-25+UI_OUT_DISTANCE));
		addChild(mission_back, 2);
		mission_back->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){mission_back->setPositionY(t);}, [=](float t){mission_back->setPositionY(myDSH->ui_top-25);}));
		
		int t_score_value = NSDS_GI(mySD->getSilType(), kSDS_SI_missionOptionCount_i);
		map<int, MissionOper>::iterator t_iter = mission_oper_list.find((int)clr_cdt_type);
		if(t_iter != mission_oper_list.end())
		{
			if(t_iter->second.oper == "-")
				t_score_value -= t_iter->second.value;
			else if(t_iter->second.oper == "*")
				t_score_value *= t_iter->second.value;
			
			myGD->communication("Jack_showMissionEffect", int(clr_cdt_type));
			
			if(t_score_value < 0)
				t_score_value = 0;
		}
		
		clr_cdt_cnt = t_score_value;
		
		CCLabelTTF* t_condition_label = CCLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_mission10Label), KS::insert_separator(t_score_value).c_str()), mySGD->getFont().c_str(), 12);
		t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
		t_condition_label->setPosition(mission_back->getPosition() + ccp(0,-1));
		addChild(t_condition_label, 2);
		t_condition_label->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){t_condition_label->setPositionY(t);}, [=](float t){t_condition_label->setPositionY(myDSH->ui_top-25);}));
		mission_clear_remove_nodes.push_back(t_condition_label);
		
		if(t_score_value <= 0)
			conditionClear();
	}
	else if(clr_cdt_type == kCLEAR_combo)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
		mission_back = CCSprite::create("ui_mission_button_open.png");
		mission_back->setPosition(ccp(80, myDSH->ui_top-25+UI_OUT_DISTANCE));
		addChild(mission_back, 2);
		mission_back->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){mission_back->setPositionY(t);}, [=](float t){mission_back->setPositionY(myDSH->ui_top-25);}));
		
		int t_combo_value = NSDS_GI(mySD->getSilType(), kSDS_SI_missionOptionCount_i);
		map<int, MissionOper>::iterator t_iter = mission_oper_list.find((int)clr_cdt_type);
		if(t_iter != mission_oper_list.end())
		{
			if(t_iter->second.oper == "-")
				t_combo_value -= t_iter->second.value;
			else if(t_iter->second.oper == "*")
				t_combo_value *= t_iter->second.value;
			
			myGD->communication("Jack_showMissionEffect", int(clr_cdt_type));
			
			if(t_combo_value < 0)
				t_combo_value = 0;
		}
		
		clr_cdt_cnt = t_combo_value;
		
		CCLabelTTF* t_condition_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_mission11Label), t_combo_value)->getCString(), mySGD->getFont().c_str(), 12);
		t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
		t_condition_label->setPosition(mission_back->getPosition() + ccp(0,-1));
		addChild(t_condition_label, 2);
		t_condition_label->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){t_condition_label->setPositionY(t);}, [=](float t){t_condition_label->setPositionY(myDSH->ui_top-25);}));
		mission_clear_remove_nodes.push_back(t_condition_label);
		
		if(t_combo_value <= 0)
			conditionClear();
	}
	else if(clr_cdt_type == kCLEAR_gold)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
		mission_back = CCSprite::create("ui_mission_button_open.png");
		mission_back->setPosition(ccp(80, myDSH->ui_top-25+UI_OUT_DISTANCE));
		addChild(mission_back, 2);
		mission_back->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){mission_back->setPositionY(t);}, [=](float t){mission_back->setPositionY(myDSH->ui_top-25);}));
		
		int t_gold_value = NSDS_GI(mySD->getSilType(), kSDS_SI_missionOptionCount_i);
		map<int, MissionOper>::iterator t_iter = mission_oper_list.find((int)clr_cdt_type);
		if(t_iter != mission_oper_list.end())
		{
			if(t_iter->second.oper == "-")
				t_gold_value -= t_iter->second.value;
			else if(t_iter->second.oper == "*")
				t_gold_value *= t_iter->second.value;
			
			myGD->communication("Jack_showMissionEffect", int(clr_cdt_type));
			
			if(t_gold_value < 0)
				t_gold_value = 0;
		}
		
		clr_cdt_cnt = t_gold_value;
		
		CCLabelTTF* t_condition_label = CCLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_mission12Label), KS::insert_separator(t_gold_value).c_str()), mySGD->getFont().c_str(), 12);
		t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
		t_condition_label->setPosition(mission_back->getPosition() + ccp(0,-1));
		addChild(t_condition_label, 2);
		t_condition_label->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){t_condition_label->setPositionY(t);}, [=](float t){t_condition_label->setPositionY(myDSH->ui_top-25);}));
		mission_clear_remove_nodes.push_back(t_condition_label);
		
		if(t_gold_value <= 0)
			conditionClear();
	}
	else if(clr_cdt_type == kCLEAR_turns)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
		mission_back = CCSprite::create("ui_mission_button_open.png");
		mission_back->setPosition(ccp(80, myDSH->ui_top-25+UI_OUT_DISTANCE));
		addChild(mission_back, 2);
		mission_back->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){mission_back->setPositionY(t);}, [=](float t){mission_back->setPositionY(myDSH->ui_top-25);}));
		
		int t_turns_value = NSDS_GI(mySD->getSilType(), kSDS_SI_missionOptionCount_i);
		map<int, MissionOper>::iterator t_iter = mission_oper_list.find((int)clr_cdt_type);
		if(t_iter != mission_oper_list.end())
		{
			if(t_iter->second.oper == "+")
				t_turns_value += t_iter->second.value;
			else if(t_iter->second.oper == "*")
				t_turns_value *= t_iter->second.value;
			
			myGD->communication("Jack_showMissionEffect", int(clr_cdt_type));
		}
		
		clr_cdt_cnt = t_turns_value;
		
		CCLabelTTF* t_condition_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_mission13Label), t_turns_value)->getCString(), mySGD->getFont().c_str(), 12);
		t_condition_label->setAnchorPoint(ccp(0.f,0.5f));
		addChild(t_condition_label, 2);
		
		CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", turn_cnt.getV())->getCString(), mySGD->getFont().c_str(), 16);
		clr_cdt_label->setColor(ccc3(255, 170, 20));
		clr_cdt_label->setAnchorPoint(ccp(1,0.5f));
		t_condition_label->setPosition(mission_back->getPosition() + ccp(clr_cdt_label->getContentSize().width/2.f - t_condition_label->getContentSize().width/2.f, -1));
		clr_cdt_label->setPosition(mission_back->getPosition() + ccp(-t_condition_label->getContentSize().width/2.f + clr_cdt_label->getContentSize().width/2.f, -1));
		addChild(clr_cdt_label, 2, kCT_UI_clrCdtLabel);
		
		t_condition_label->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){t_condition_label->setPositionY(t);}, [=](float t){t_condition_label->setPositionY(myDSH->ui_top-25);}));
		mission_clear_remove_nodes.push_back(t_condition_label);
		
		clr_cdt_label->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){clr_cdt_label->setPositionY(t);}, [=](float t){clr_cdt_label->setPositionY(myDSH->ui_top-25);}));
		mission_clear_remove_nodes.push_back(clr_cdt_label);
	}
	else if(clr_cdt_type == kCLEAR_casting)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
		mission_back = CCSprite::create("ui_mission_button_open.png");
		mission_back->setPosition(ccp(80, myDSH->ui_top-25+UI_OUT_DISTANCE));
		addChild(mission_back, 2);
		mission_back->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){mission_back->setPositionY(t);}, [=](float t){mission_back->setPositionY(myDSH->ui_top-25);}));
		
		int t_casting_value = NSDS_GI(mySD->getSilType(), kSDS_SI_missionOptionCount_i);
		map<int, MissionOper>::iterator t_iter = mission_oper_list.find((int)clr_cdt_type);
		if(t_iter != mission_oper_list.end())
		{
			if(t_iter->second.oper == "-")
				t_casting_value -= t_iter->second.value;
			else if(t_iter->second.oper == "*")
				t_casting_value *= t_iter->second.value;
			
			myGD->communication("Jack_showMissionEffect", int(clr_cdt_type));
		}
		
		clr_cdt_cnt = t_casting_value;
		
		CCLabelTTF* t_condition_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_mission14Label), t_casting_value)->getCString(), mySGD->getFont().c_str(), 12);
		t_condition_label->setAnchorPoint(ccp(0.f,0.5f));
		addChild(t_condition_label, 2);
		
		CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", casting_cancel_cnt.getV())->getCString(), mySGD->getFont().c_str(), 16);
		clr_cdt_label->setColor(ccc3(255, 170, 20));
		clr_cdt_label->setAnchorPoint(ccp(1,0.5f));
		t_condition_label->setPosition(mission_back->getPosition() + ccp(clr_cdt_label->getContentSize().width/2.f - t_condition_label->getContentSize().width/2.f, -1));
		clr_cdt_label->setPosition(mission_back->getPosition() + ccp(-t_condition_label->getContentSize().width/2.f + clr_cdt_label->getContentSize().width/2.f, -1));
		addChild(clr_cdt_label, 2, kCT_UI_clrCdtLabel);
		
		t_condition_label->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){t_condition_label->setPositionY(t);}, [=](float t){t_condition_label->setPositionY(myDSH->ui_top-25);}));
		mission_clear_remove_nodes.push_back(t_condition_label);
		
		clr_cdt_label->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){clr_cdt_label->setPositionY(t);}, [=](float t){clr_cdt_label->setPositionY(myDSH->ui_top-25);}));
		mission_clear_remove_nodes.push_back(clr_cdt_label);
		
		if(t_casting_value <= 0)
			conditionClear();
	}
	else if(clr_cdt_type == kCLEAR_littlePercent)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
		mission_back = CCSprite::create("ui_mission_button_open.png");
		mission_back->setPosition(ccp(80, myDSH->ui_top-25+UI_OUT_DISTANCE));
		addChild(mission_back, 2);
		mission_back->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){mission_back->setPositionY(t);}, [=](float t){mission_back->setPositionY(myDSH->ui_top-25);}));
		
		int t_percent_value = NSDS_GI(mySD->getSilType(), kSDS_SI_missionOptionPercent_i);
		map<int, MissionOper>::iterator t_iter = mission_oper_list.find((int)clr_cdt_type);
		if(t_iter != mission_oper_list.end())
		{
			if(t_iter->second.oper == "+")
				t_percent_value += t_iter->second.value;
			else if(t_iter->second.oper == "*")
				t_percent_value *= t_iter->second.value;
			
			myGD->communication("Jack_showMissionEffect", int(clr_cdt_type));
		}
		
		clr_cdt_cnt = t_percent_value;
		
		CCLabelTTF* t_condition_label = CCLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_mission15Label), t_percent_value), mySGD->getFont().c_str(), 12);
		t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
		t_condition_label->setPosition(mission_back->getPosition() + ccp(0,-1));
		addChild(t_condition_label, 2);
		t_condition_label->addChild(KSGradualValue<float>::create(myDSH->ui_top-25+UI_OUT_DISTANCE, myDSH->ui_top-25, UI_IN_TIME, [=](float t){t_condition_label->setPositionY(t);}, [=](float t){t_condition_label->setPositionY(myDSH->ui_top-25);}));
		mission_clear_remove_nodes.push_back(t_condition_label);
	}
	else if(clr_cdt_type == kCLEAR_default)
	{
		if(mySD->getSilType() != 1)
		{
			mission_button->doClose();
		}
		else
		{
			CCNode* t_node = CCNode::create();
			mission_button->addChild(t_node);
			CCDelayTime* t_delay = CCDelayTime::create(4.f);
			CCCallFunc* t_call1 = CCCallFunc::create(mission_button, callfunc_selector(RollingButton::doClose));
			CCCallFunc* t_call2 = CCCallFunc::create(t_node, callfunc_selector(CCNode::removeFromParent));
			CCSequence* t_seq = CCSequence::create(t_delay, t_call1, t_call2, NULL);
			t_node->runAction(t_seq);
			
			mission_button->doOpen();
		}
		
		is_cleared_cdt = true;
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		
//		CCSprite* icon_img = CCSprite::create("condition0_menu.png");
//		icon_img->setPosition(icon_menu_position);
//		addChild(icon_img, 0, kCT_UI_clrCdtIcon);
	}
	
	casting_cancel_node = CCNode::create();
	casting_cancel_node->setPosition(ccp(240,12));
	addChild(casting_cancel_node);
	
	casting_cancel_gage = ConvexGraph::create("boss_cancelgage_front.png", CCRectMake(0, 0, 13, 13), CCRectMake(6, 6, 1, 1), CCSizeMake(85, 13), ConvexGraphType::width);
	casting_cancel_gage->setPosition(ccp(0,0));
	casting_cancel_node->addChild(casting_cancel_gage);
	
	casting_cancel_gage->setBack("boss_cancelgage_back.png");
	casting_cancel_gage->setPercentage(0);
	
	casting_cancel_chance_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_castingCancelChance), mySGD->getFont().c_str(), 9.5f);
	casting_cancel_chance_label->enableOuterStroke(ccc3(115, 0, 145), 1, 255, true);
	casting_cancel_chance_label->setPosition(ccp(0,0));
	casting_cancel_node->addChild(casting_cancel_chance_label);
	
	casting_cancel_chance_label->setVisible(false);
//	casting_cancel_node->setVisible(false);
	
	my_combo = ComboParent::create(score_label);
	my_combo->setPosition(CCPointZero);
	addChild(my_combo);
	
	my_fp = FeverParent::create(countingLabel);
	my_fp->setPosition(CCPointZero);
	addChild(my_fp);
	
	using_item_sprites.clear();
	int using_item_cnt = 0;
	
	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
	{
		if(mySGD->isUsingItem((ITEM_CODE)i))
		{
			using_item_cnt++;
		}
	}
	
	vector<GraySprite*> using_item_vectors;
	using_item_vectors.clear();
	
	float item_scale = 0.4f;
	CCPoint item_base_position = ccpAdd(ccp(460, myDSH->ui_center_y), ccpMult(ccp(0,25), (using_item_cnt-1)/2.f));
	CCPoint distance_position = ccp(0,-25);
	CCNode* add_target = this;
	if(mySGD->is_endless_mode)
	{
		item_scale = 0.18f;
		item_base_position = ccp(40, myDSH->ui_center_y-10) + ccpMult(ccp(-160,-215), 0.17f) + ccp(7,-7);
		distance_position = ccp(14,0);
		add_target = thumb_node;
	}
	
	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
	{
		if(mySGD->isUsingItem((ITEM_CODE)i))
		{
			GraySprite* item_img = GraySprite::create(CCString::createWithFormat("item%d.png", i)->getCString());
			item_img->setScale(item_scale);
			item_img->setOpacity(150);
			add_target->addChild(item_img);
			
			CCSprite* item_case = CCSprite::create("ingame_itembox.png");
			item_case->setPosition(ccp(item_img->getContentSize().width/2.f, item_img->getContentSize().height/2.f));
			item_case->setScale(1.f/0.4f);
			item_case->setOpacity(150);
			item_img->addChild(item_case, -1);
			
			using_item_sprites[i] = item_img;
			using_item_vectors.push_back(item_img);
		}
	}
	
	float signed_distance = UI_OUT_DISTANCE;
	if(mySGD->is_endless_mode)
		signed_distance *= -1.f;
	
	for(int i=0;i<using_item_vectors.size();i++)
	{
		using_item_vectors[i]->setPosition(ccpAdd(item_base_position, ccpMult(distance_position, i)));
		
		using_item_vectors[i]->setPositionX(using_item_vectors[i]->getPositionX()+signed_distance);
		addChild(KSGradualValue<float>::create(ccpAdd(item_base_position, ccpMult(distance_position, i)).x+signed_distance, ccpAdd(item_base_position, ccpMult(distance_position, i)).x, UI_IN_TIME,
											   [=](float t){using_item_vectors[i]->setPositionX(t);},
											   [=](float t){using_item_vectors[i]->setPositionX(ccpAdd(item_base_position, ccpMult(distance_position, i)).x);}));
	}
	
	is_casting = false;
	is_tinting_casting_gage = false;
	schedule(schedule_selector(PlayUI::castingCheck));
	
	myGD->V_II["UI_addScore"] = std::bind(&PlayUI::addScore, this, _1, _2);
	myGD->V_FB["UI_setPercentage"] = std::bind(&PlayUI::setPercentage, this, _1, _2);
	myGD->V_F["UI_subBossLife"] = std::bind(&PlayUI::subBossLife, this, _1);
	myGD->V_V["UI_decreasePercentage"] = std::bind(&PlayUI::decreasePercentage, this);
	myGD->V_I["UI_decreasePercentages"] = std::bind(&PlayUI::decreasePercentages, this, _1);
	myGD->B_V["UI_beRevivedJack"] = std::bind(&PlayUI::beRevivedJack, this);
	myGD->V_TDTD["UI_showContinuePopup"] = std::bind(&PlayUI::showContinuePopup, this, _1, _2, _3, _4);
	myGD->V_V["UI_catchSubCumber"] = std::bind(&PlayUI::catchSubCumber, this);
	myGD->V_V["UI_takeItemCollect"] = std::bind(&PlayUI::takeItemCollect, this);
	myGD->V_V["UI_takeAddTimeItem"] = std::bind(&PlayUI::takeAddTimeItem, this);
	myGD->V_V["UI_stopCounting"] = std::bind(&PlayUI::stopCounting, this);
	myGD->V_V["UI_resumeCounting"] = std::bind(&PlayUI::resumeCounting, this);
	myGD->F_V["UI_getMapPercentage"] = std::bind(&PlayUI::getPercentage, this);
	myGD->I_V["UI_getComboCnt"] = std::bind(&PlayUI::getComboCnt, this);
	myGD->V_I["UI_setComboCnt"] = std::bind(&PlayUI::setComboCnt, this, _1);
	myGD->I_V["UI_getUseTime"] = std::bind(&PlayUI::getUseTime, this);
	myGD->V_V["UI_endFever"] = std::bind(&FeverParent::endFever, my_fp);
	myGD->V_V["UI_stopCombo"] = std::bind(&ComboParent::stopKeep, my_combo);
	myGD->B_V["UI_isExchanged"] = std::bind(&PlayUI::isExchanged, this);
	myGD->V_V["UI_addGameTime30Sec"] = std::bind(&PlayUI::addGameTime30Sec, this);
	myGD->V_V["UI_writeMap"] = std::bind(&PlayUI::writeMap, this);
	myGD->V_V["UI_checkMapTimeVector"] = std::bind(&PlayUI::checkMapTimeVector, this);
	myGD->V_V["UI_writeScore"] = std::bind(&PlayUI::writeScore, this);
	myGD->V_V["UI_checkScoreTimeVector"] = std::bind(&PlayUI::checkScoreTimeVector, this);
	myGD->V_V["UI_writeDie"] = std::bind(&PlayUI::writeDie, this);
	myGD->V_V["UI_writeImageChange"] = std::bind(&PlayUI::writeImageChange, this);
	myGD->V_I["UI_writeGameOver"] = std::bind(&PlayUI::writeGameOver, this, _1);
	myGD->V_V["UI_writeContinue"] = std::bind(&PlayUI::writeContinue, this);
	myGD->V_V["UI_takeSilenceItem"] = std::bind(&PlayUI::takeSilenceItem, this);
	myGD->V_V["UI_addTurnCnt"] = std::bind(&PlayUI::addTurnCnt, this);
	myGD->V_V["UI_checkScoreMission"] = std::bind(&PlayUI::checkScoreMission, this);
	myGD->V_V["UI_hellModeResult"] = std::bind(&PlayUI::hellModeResult, this);
	myGD->V_V["UI_showTimeover"] = std::bind(&PlayUI::showTimeover, this);
	myGD->V_F["UI_changeCastingGage"] = [=](float t_f)
	{
		casting_cancel_gage->ks_animator_node->removeAllChildren();
		
//		casting_cancel_chance_label->stopAllActions();
		if(t_f >= 1.f)
		{
			t_f = 1.f;
			casting_cancel_chance_label->setVisible(true);
//			CCSequence* t_action = CCSequence::create(CCShow::create(), CCDelayTime::create(0.3f), CCHide::create(), CCDelayTime::create(0.3f), NULL);
//			CCRepeatForever* t_repeat = CCRepeatForever::create(t_action);
//			casting_cancel_chance_label->runAction(t_repeat);
		}
		else
		{
			casting_cancel_gage->front_img->setColor(ccWHITE);
			casting_cancel_chance_label->setVisible(false);
		}
		
		casting_cancel_gage->ks_animator_node->addChild(KSGradualValue<float>::create(casting_cancel_gage->getPercentage(), t_f*100.f, 0.3f, [=](float t_f)
									  {
										  casting_cancel_gage->setPercentage(t_f);
									  }, [=](float t_f)
									  {
										  casting_cancel_gage->setPercentage(t_f);
									  }));
	};
	myGD->V_B["UI_setIsCasting"] = [=](bool t_b)
	{
		if(!is_casting && t_b)
		{
			bool keep_visible = casting_cancel_chance_label->isVisible();
			casting_cancel_chance_label->setString(getLocal(LK::kMyLocalKey_attackRightNow));
			casting_cancel_chance_label->setVisible(keep_visible);
		}
		else if(is_casting && !t_b)
		{
			bool keep_visible = casting_cancel_chance_label->isVisible();
			casting_cancel_chance_label->setString(getLocal(LK::kMyLocalKey_castingCancelChance));
			casting_cancel_chance_label->setVisible(keep_visible);
		}
		
		is_casting = t_b;
	};
	myGD->V_V["UI_castingCancel"] = [=]()
	{
		casting_cancel_cnt = casting_cancel_cnt.getV() + 1;
		if(!is_cleared_cdt && clr_cdt_type == kCLEAR_casting)
		{
			CCLabelTTF* t_label = (CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel);
			t_label->setString(ccsf("%d", casting_cancel_cnt.getV()));
			
			if(casting_cancel_cnt.getV() >= clr_cdt_cnt.getV())
			{
				conditionClear();
			}
		}
	};
}

void PlayUI::castingCheck()
{
	if(is_casting)
	{
		if(!is_tinting_casting_gage)
		{
			is_tinting_casting_gage = true;
			CCScale9Sprite* gage_sprite = casting_cancel_gage->front_img;
			
			CCSequence* t_action = CCSequence::create(CCTintTo::create(0.3f, 255, 0, 0), CCDelayTime::create(0.1f), CCTintTo::create(0.3f, 255, 255, 255), CCDelayTime::create(0.1f), NULL);
			CCRepeatForever* t_repeat = CCRepeatForever::create(t_action);
			gage_sprite->runAction(t_repeat);
		}
	}
	else
	{
		if(is_tinting_casting_gage)
		{
			casting_cancel_gage->front_img->stopAllActions();
			casting_cancel_gage->front_img->setColor(ccWHITE);
			is_tinting_casting_gage = false;
		}
	}
}

void PlayUI::hideThumb()
{
	thumb_node->setVisible(false);
	if(mySGD->is_endless_mode)
	{
		endless_me_bomb_node->setVisible(false);
//		bomb_img->setVisible(false);
	}
}

bool PlayUI::isExchanged ()
{
	return is_exchanged;
}
void PlayUI::continueAction ()
{
	vector<SaveUserData_Key> save_userdata_list;
	
	save_userdata_list.push_back(kSaveUserData_Key_gold);
	
//	myDSH->saveUserData(save_userdata_list, nullptr);
	
	AudioEngine::sharedInstance()->stopEffect("se_clock.mp3");
//	AudioEngine::sharedInstance()->stopEffect("sound_time_noti.mp3");
	
	total_time += countingCnt.getV();
	
//	if(mySGD->isUsingItem(kIC_longTime))
//	{
//		countingCnt = -mySGD->getLongTimeValue();
//	}
//	else
//	{
		countingCnt = 0;
//	}
	
	CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
	jack_life = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_int2_life_i, t_history.characterIndex.getV(), t_history.characterLevel.getV())-1;//NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_life_i, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1)-1;
	
	CCPoint life_base_position = ccpMult(ccp(-50,0), (jack_life-1)/2.f);
	
	for(int i=0;i<jack_life.getV();i++)
	{
		CCSprite* jack_img = CCSprite::create("ingame_life.png");
		jack_img->setPosition(ccpAdd(life_base_position, ccp(i*50, 0)));
//		if(myGD->gamescreen_type == kGT_leftUI)			jack_img->setPosition(ccp(25, myDSH->ui_center_y-30-i*20));
//		else if(myGD->gamescreen_type == kGT_rightUI)		jack_img->setPosition(ccp(480-25,myDSH->ui_center_y-30-i*20));
//		else									jack_img->setPosition(ccp(80+i*20,myDSH->ui_top-35));
		jack_life_node->addChild(jack_img);
		
		jack_array->addObject(jack_img);
	}
	
//	CCMoveTo* t_move = CCMoveTo::create(0.5f, ccp(240,myDSH->ui_bottom+20));
//	jack_life_node->runAction(t_move);
	
	jack_life_hide_count = 0;
	
	
	writeContinue();
	
	(target_continue->*delegate_continue)();
}
void PlayUI::menuAction (CCObject * sender)
{
//	AudioEngine::sharedInstance()->playEffect("sound_buttonClick_Low.mp3", false);
	int tag = ((CCNode*)sender)->getTag();
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	if(tag == kMenuTagUI_home && !isGameover)
	{
		showPause();
	}
	else if(tag == kMenuTagUI_condition && !isGameover)
	{
		showCondition();
	}
}
void PlayUI::showCondition ()
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
void PlayUI::closeCondition ()
{
	is_show_condition = false;
}
void PlayUI::closeShutter ()
{
	mySGD->is_paused = false;
	AudioEngine::sharedInstance()->setAppFore();
	CCDirector::sharedDirector()->resume();
	
	endCloseShutter();
}
void PlayUI::endCloseShutter ()
{
	myGD->communication("Main_gameover");
}
//void PlayUI::goHome ()
//{
//	myLog->addLog(kLOG_getCoin_i, -1, mySGD->getStageGold());
//	
//	myLog->sendLog(CCString::createWithFormat("home_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStage))->getCString());
//	AudioEngine::sharedInstance()->stopAllEffects();
//	AudioEngine::sharedInstance()->stopSound();
//	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_fail);
//	closeShutter();
//}
//void PlayUI::goReplay ()
//{
//	myDSH->setIntegerForKey(kDSH_Key_achieve_seqNoFailCnt, 0);
//	myLog->addLog(kLOG_getCoin_i, -1, mySGD->getStageGold());
//	
//	myLog->sendLog(CCString::createWithFormat("replay_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStage))->getCString());
//	AudioEngine::sharedInstance()->stopAllEffects();
//	AudioEngine::sharedInstance()->stopSound();
//	
//	mySGD->is_paused = false;
//	AudioEngine::sharedInstance()->setAppFore();
//	CCDirector::sharedDirector()->resume();
//	mySGD->gameOver(0, 0, 0);
//	mySGD->resetLabels();
//	myGD->resetGameData();
//	
//	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stageSetting);
////	CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
//	CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
//}
//void PlayUI::cancelHome ()
//{
//	(target_main->*delegate_startControl)();
//	mySGD->is_paused = false;
//	AudioEngine::sharedInstance()->setAppFore();
//	CCDirector::sharedDirector()->resume();
//}

void PlayUI::checkStageGoldMission(int t_gold)
{
	if(clr_cdt_type == kCLEAR_gold && t_gold >= clr_cdt_cnt.getV())
	{
		conditionClear();
	}
}

void PlayUI::addTurnCnt()
{
	turn_cnt = turn_cnt.getV() + 1;
	
	if(clr_cdt_type == kCLEAR_turns)
	{
		if(mission_clear_remove_nodes.size() >= 2)
		{
			CCLabelTTF* t_condition_label = (CCLabelTTF*)mission_clear_remove_nodes[0];
		
			CCLabelTTF* clr_cdt_label = (CCLabelTTF*)mission_clear_remove_nodes[1];
			clr_cdt_label->setString(ccsf("%d", turn_cnt.getV()));
			
			t_condition_label->setPosition(mission_back->getPosition() + ccp(clr_cdt_label->getContentSize().width/2.f - t_condition_label->getContentSize().width/2.f, -1));
			clr_cdt_label->setPosition(mission_back->getPosition() + ccp(-t_condition_label->getContentSize().width/2.f + clr_cdt_label->getContentSize().width/2.f, -1));
		}
	}
	
	if(!is_cleared_cdt && clr_cdt_type == kCLEAR_turns && turn_cnt.getV() >= clr_cdt_cnt.getV())
	{
		conditionFail();
		
		mySGD->fail_code = kFC_missionfail;
		
		stopCounting();
		// timeover
		isGameover = true;
		myGD->communication("CP_setGameover");
		myGD->removeAllPattern();
		myGD->communication("Main_allStopSchedule");
		AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
		
		addResultCCB("ui_missonfail.ccbi");
		AudioEngine::sharedInstance()->playEffect("ment_mission_fail.mp3", false, true);
		
		endGame(false);
	}
}

void PlayUI::checkScoreMission()
{
	int total_score;
	
	int grade_value = 1;
	
	if(is_exchanged && (beforePercentage^t_tta)/1000.f >= 1.f)		grade_value = 4;
	else if(is_exchanged)											grade_value = 3;
	else if((beforePercentage^t_tta)/1000.f >= 1.f)					grade_value = 2;
	
	int recent_score = getScore();
	
	float t_game_time = countingCnt.getV();
	float play_limit_time = NSDS_GI(mySD->getSilType(), kSDS_SI_playtime_i);
	if(mySD->getClearCondition() == kCLEAR_timeLimit)
	{
		play_limit_time -= mySD->getClearConditionTimeLimit();
	}
	
	int time_score = ((play_limit_time-t_game_time)*500*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d));
	
	total_score = recent_score + time_score;
	total_score = total_score*grade_value;
	
	if(total_score >= clr_cdt_cnt.getV())
	{
		conditionClear();
	}
}

void PlayUI::alertAction (int t1, int t2)
{
	
}
#undef LZZ_INLINE
