//
//  TakeCardPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 11. 14..
//
//

#ifndef __DGproto__TakeCardPopup__
#define __DGproto__TakeCardPopup__

#include "cocos2d.h"
#include "CardCase.h"
#include "StageImgLoader.h"

USING_NS_CC;
using namespace std;

enum TakeCardPopupZorder{
	kTakeCardPopup_Z_gray = 1,
	kTakeCardPopup_Z_img,
	kTakeCardPopup_Z_particle
};

class TakeCardPopup : public CCLayer
{
public:
	static TakeCardPopup* create(int t_stage, int t_grade)
	{
		TakeCardPopup* t_cp = new TakeCardPopup();
		t_cp->myInit(t_stage, t_grade);
		t_cp->autorelease();
		return t_cp;
	}
	
private:
	bool is_touch_enable;
	CCParticleSystemQuad* star_particle;
	CCSprite* gray;
	CCSprite* take_label;
	CCSprite* take_card;
	CCSprite* card_back;
	
	void myInit(int t_stage, int t_grade)
	{
		gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160));
		gray->setOpacity(0);
		addChild(gray, kTakeCardPopup_Z_gray);
		
		gray->runAction(CCFadeTo::create(0.4f, 255));
		
		take_label = CCSprite::create("ending_take_card.png");
		take_label->setPosition(ccp(240,280));
		take_label->setOpacity(0);
		addChild(take_label, kTakeCardPopup_Z_img);
		
		take_label->runAction(CCFadeTo::create(0.4f, 255));
		
		take_card = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", t_stage, t_grade)->getCString());
		take_card->setScale(0.5f);
		take_card->setPosition(ccp(600,130));
		addChild(take_card, kTakeCardPopup_Z_img);
		
		if(t_grade == 3 && mySD->isAnimationStage())
		{
			CCSize ani_size = mySD->getAnimationCutSize();
			CCSprite* take_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", t_stage, t_grade)->getCString(), CCRectMake(0, 0, ani_size.width, ani_size.height));
			take_ani->setPosition(mySD->getAnimationPosition());
			take_card->addChild(take_ani);
		}
		
		CardCase* t_case = CardCase::create(t_stage, t_grade);
		t_case->setPosition(CCPointZero);
		take_card->addChild(t_case);
		
		CCLabelTTF* recent_durability_label = t_case->getRecentDurabilityLabel();
		recent_durability_label->setString(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_durability_i, NSDS_GI(t_stage, kSDS_SI_level_int1_card_i, t_grade)))->getCString());
		recent_durability_label->setPosition(ccpAdd(recent_durability_label->getPosition(), ccp(6,-1)));
		recent_durability_label->setFontSize(recent_durability_label->getFontSize()+3);
		
		t_case->getTotalDurabilityLabel()->removeFromParent();
		
		card_back = CCSprite::create("ending_take_card_back.png");
		card_back->setVisible(false);
		card_back->setScale(take_card->getScale());
		card_back->setPosition(take_card->getPosition());
		addChild(card_back, kTakeCardPopup_Z_img);
		
		is_touch_enable = false;
		
		setTouchEnabled(true);
		
		runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.2f), CCCallFunc::create(this, callfunc_selector(TakeCardPopup::openingAction))));
	}
	
	void openingAction()
	{
		CCMoveTo* t_move = CCMoveTo::create(0.8f, ccp(240,130));
		
		CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.1f, 0.2f, 0, 0, 90, 0, 0);// float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::changeBack));
		CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.1f, 0.2f, 0, -90, 90, 0, 0);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::changeTakeCard));
		CCSequence* t_seq = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
		CCRepeat* t_repeat = CCRepeat::create(t_seq, 4);
		CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::touchOn));
		CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_repeat, t_call3);
		CCSpawn* t_spawn1 = CCSpawn::create(t_move, t_seq2, NULL);
		take_card->runAction(t_spawn1);
		
		
		CCMoveTo* t_move2 = CCMoveTo::create(0.8f, ccp(240,130));
		CCOrbitCamera* t_orbit3 = CCOrbitCamera::create(0.1f, 0.2f, 0, 0, 90, 0, 0);
		CCOrbitCamera* t_orbit4 = CCOrbitCamera::create(0.1f, 0.2f, 0, -90, 90, 0, 0);
		CCSequence* t_seq3 = CCSequence::create(t_orbit3, t_orbit4, NULL);
		CCRepeat* t_repeat2 = CCRepeat::create(t_seq3, 4);
		CCSpawn* t_spawn2 = CCSpawn::create(t_move2, t_repeat2, NULL);
		card_back->runAction(t_spawn2);
	}
	
	void changeBack()
	{
		card_back->setVisible(true);
		take_card->setVisible(false);
	}
	
	void changeTakeCard()
	{
		take_card->setVisible(true);
		card_back->setVisible(false);
	}
	
	void touchOn()
	{
		is_touch_enable = true;
		
		star_particle = CCParticleSystemQuad::createWithTotalParticles(10);
		star_particle->setPositionType(kCCPositionTypeRelative);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("ending_take_particle.png");
		star_particle->setTexture(texture);
		star_particle->setEmissionRate(13);
		star_particle->setAngle(90.0);
		star_particle->setAngleVar(5.0);
		ccBlendFunc blendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
		star_particle->setBlendFunc(blendFunc);
		star_particle->setDuration(-1.0);
		star_particle->setEmitterMode(kCCParticleModeGravity);
		star_particle->setStartColor(ccc4f(1.f, 1.f, 1.f, 1.f));
		star_particle->setStartColorVar(ccc4f(0, 0, 0, 0));
		star_particle->setEndColor(ccc4f(1.f, 1.f, 1.f, 0));
		star_particle->setEndColorVar(ccc4f(0, 0, 0, 0));
		star_particle->setStartSize(30.0);
		star_particle->setStartSizeVar(10.0);
		star_particle->setEndSize(-1.0);
		star_particle->setEndSizeVar(0.0);
		star_particle->setGravity(ccp(0,0));
		star_particle->setRadialAccel(0.0);
		star_particle->setRadialAccelVar(0.0);
		star_particle->setSpeed(60);
		star_particle->setSpeedVar(20.0);
		star_particle->setTangentialAccel(0);
		star_particle->setTangentialAccelVar(0);
		star_particle->setLife(1.f);
		star_particle->setLifeVar(0.5f);
		star_particle->setStartSpin(0.0);
		star_particle->setStartSpinVar(180.f);
		star_particle->setEndSpin(0.0);
		star_particle->setEndSpinVar(0.f);
		star_particle->setPosition(ccp(240,130));
		star_particle->setPosVar(ccp(60,60));
		star_particle->setAutoRemoveOnFinish(true);
		
		addChild(star_particle, kTakeCardPopup_Z_particle);
	}
	
	void fadeGrayTitle()
	{
		gray->runAction(CCFadeTo::create(0.4f, 0));
		take_label->runAction(CCFadeTo::create(0.4f, 0));
	}
	
	void closingAction()
	{
		CCMoveTo* t_move = CCMoveTo::create(0.8f, ccp(-120,130));
		
		CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.1f, 0.2f, 0, 0, 90, 0, 0);// float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::changeBack));
		CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.1f, 0.2f, 0, -90, 90, 0, 0);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::changeTakeCard));
		CCSequence* t_seq = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
		CCRepeat* t_repeat = CCRepeat::create(t_seq, 2);
		
		CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::fadeGrayTitle));
		
		CCOrbitCamera* t_orbit5 = CCOrbitCamera::create(0.1f, 0.2f, 0, 0, 90, 0, 0);// float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX
		CCCallFunc* t_call4 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::changeBack));
		CCOrbitCamera* t_orbit6 = CCOrbitCamera::create(0.1f, 0.2f, 0, -90, 90, 0, 0);
		CCCallFunc* t_call5 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::changeTakeCard));
		CCSequence* t_seq4 = CCSequence::create(t_orbit5, t_call4, t_orbit6, t_call5, NULL);
		CCRepeat* t_repeat3 = CCRepeat::create(t_seq4, 2);
		
		CCCallFunc* t_call6 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::removeFromParent));
		CCSequence* t_seq2 = CCSequence::create(t_repeat, t_call3, t_repeat3, t_call6, NULL);
		CCSpawn* t_spawn1 = CCSpawn::create(t_move, t_seq2, NULL);
		take_card->runAction(t_spawn1);
		
		
		CCMoveTo* t_move2 = CCMoveTo::create(0.8f, ccp(-120,130));
		CCOrbitCamera* t_orbit3 = CCOrbitCamera::create(0.1f, 0.2f, 0, 0, 90, 0, 0);
		CCOrbitCamera* t_orbit4 = CCOrbitCamera::create(0.1f, 0.2f, 0, -90, 90, 0, 0);
		CCSequence* t_seq3 = CCSequence::create(t_orbit3, t_orbit4, NULL);
		CCRepeat* t_repeat2 = CCRepeat::create(t_seq3, 4);
		CCSpawn* t_spawn2 = CCSpawn::create(t_move2, t_repeat2, NULL);
		card_back->runAction(t_spawn2);
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(is_touch_enable)
		{
			closingAction();
			star_particle->setDuration(0);
			is_touch_enable = false;
		}
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
};

#endif /* defined(__DGproto__TakeCardPopup__) */
