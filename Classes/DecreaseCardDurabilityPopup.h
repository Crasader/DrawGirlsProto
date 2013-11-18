//
//  DecreaseCardDurabilityPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 11. 14..
//
//

#ifndef __DGproto__DecreaseCardDurabilityPopup__
#define __DGproto__DecreaseCardDurabilityPopup__

#include "cocos2d.h"
#include "CardCase.h"
#include "StageImgLoader.h"

USING_NS_CC;
using namespace std;

enum DecreaseCardDurabilityPopupZorder{
	kDecreaseCardDurabilityPopup_Z_gray = 1,
	kDecreaseCardDurabilityPopup_Z_img,
	kDecreaseCardDurabilityPopup_Z_particle
};

class RemoveCardSprite : public CCSprite
{
public:
	static RemoveCardSprite* create(CCTexture2D* t_texture)
	{
		RemoveCardSprite* t_rcs = new RemoveCardSprite();
		t_rcs->myInit(t_texture);
		t_rcs->autorelease();
		return t_rcs;
	}
	
	virtual void visit()
	{
		glEnable(GL_SCISSOR_TEST);
		
		glScissor(view_rect.origin.x,view_rect.origin.y,view_rect.size.width,view_rect.size.height);
		
		CCSprite::visit();
		
		glDisable(GL_SCISSOR_TEST);
	}
	
	void startRemoveCard()
	{
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
		
		view_rect.origin.x = (getPositionX() - getContentSize().width/2.f*getScale() - 5.f)*wScale + xMargine;
		view_rect.origin.y = (getPositionY() - getContentSize().height/2.f*getScale() - 5.f)*hScale + yMargine;
		view_rect.size.width = (getContentSize().width*getScale() + 10.f)*wScale;
		view_rect.size.height = (getContentSize().height*getScale() + 10.f)*hScale;
		
		decrease_value = view_rect.size.height/85.f;
		
		ing_frame = 0;
		schedule(schedule_selector(RemoveCardSprite::removingCard));
	}
	
private:
	CCRect view_rect;
	CCSize frame_size;
	int ing_frame;
	float decrease_value;
	
	void removingCard()
	{
		ing_frame++;
		
		view_rect.size.height -= decrease_value;
		
		if(ing_frame >= 85)
		{
			unschedule(schedule_selector(RemoveCardSprite::removingCard));
		}
	}
	
	void myInit(CCTexture2D* t_texture)
	{
		initWithTexture(t_texture);
		
		view_rect.origin = ccp(0,0);
		view_rect.size = frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	}
};

class DecreaseCardDurabilityPopup : public CCLayer
{
public:
	static DecreaseCardDurabilityPopup* create(int t_stage, int t_grade)
	{
		DecreaseCardDurabilityPopup* t_cp = new DecreaseCardDurabilityPopup();
		t_cp->myInit(t_stage, t_grade);
		t_cp->autorelease();
		return t_cp;
	}
	
private:
	bool is_touch_enable;
	CCParticleSystemQuad* arrow_particle;
	CCSprite* gray;
	CCSprite* decrease_durability_label;
	RemoveCardSprite* selected_card;
	CardCase* t_case;
	
	int stage_number;
	int grade_number;
	
	void myInit(int t_stage, int t_grade)
	{
		stage_number = t_stage;
		grade_number = t_grade;
		gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160));
		gray->setOpacity(0);
		addChild(gray, kDecreaseCardDurabilityPopup_Z_gray);
		
		gray->runAction(CCFadeTo::create(0.4f, 255));
		
		string title_filename;
		
		int durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, grade_number));
		if(durability <= 0)
			title_filename = "ending_remove_card.png";
		else
			title_filename = "ending_decrease_durability.png";
		
		decrease_durability_label = CCSprite::create(title_filename.c_str());
		decrease_durability_label->setPosition(ccp(240,280));
		decrease_durability_label->setOpacity(0);
		addChild(decrease_durability_label, kDecreaseCardDurabilityPopup_Z_img);
		
		decrease_durability_label->runAction(CCFadeTo::create(0.4f, 255));
		
		selected_card = RemoveCardSprite::create(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_visible.png", t_stage, t_grade)->getCString()));
		selected_card->setScale(0.5f);
		selected_card->setPosition(ccp(240,450));
		addChild(selected_card, kDecreaseCardDurabilityPopup_Z_img);
		
		if(t_grade == 3 && mySD->isAnimationStage())
		{
			CCSize ani_size = mySD->getAnimationCutSize();
			CCSprite* take_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", t_stage, t_grade)->getCString(), CCRectMake(0, 0, ani_size.width, ani_size.height));
			take_ani->setPosition(mySD->getAnimationPosition());
			selected_card->addChild(take_ani);
		}
		
		t_case = CardCase::create(t_stage, t_grade);
		t_case->setPosition(CCPointZero);
		selected_card->addChild(t_case);
		
		(t_case->getRecentDurabilityLabel())->setString(CCString::createWithFormat("%d", myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(t_stage, kSDS_SI_level_int1_card_i, t_grade))+1)->getCString());
		
		is_touch_enable = false;
		
		setTouchEnabled(true);
		
		runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.2f), CCCallFunc::create(this, callfunc_selector(DecreaseCardDurabilityPopup::openingAction))));
	}
	
	void openingAction()
	{
		CCMoveTo* t_move = CCMoveTo::create(0.7f, ccp(240,130));
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(DecreaseCardDurabilityPopup::startEffect));
		CCSequence* t_seq1 = CCSequence::create(t_move, t_call1, NULL);
		selected_card->runAction(t_seq1);
	}
	
	void startEffect()
	{
		arrow_particle = CCParticleSystemQuad::createWithTotalParticles(10);
		arrow_particle->setPositionType(kCCPositionTypeRelative);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("ending_decrease_particle.png");
		arrow_particle->setTexture(texture);
		arrow_particle->setEmissionRate(13);
		arrow_particle->setAngle(90.0);
		arrow_particle->setAngleVar(0.0);
		ccBlendFunc blendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
		arrow_particle->setBlendFunc(blendFunc);
		arrow_particle->setDuration(-1.0);
		arrow_particle->setEmitterMode(kCCParticleModeGravity);
		arrow_particle->setStartColor(ccc4f(1.f, 1.f, 1.f, 1.f));
		arrow_particle->setStartColorVar(ccc4f(0, 0, 0, 0));
		arrow_particle->setEndColor(ccc4f(1.f, 1.f, 1.f, 0));
		arrow_particle->setEndColorVar(ccc4f(0, 0, 0, 0));
		arrow_particle->setStartSize(30.0);
		arrow_particle->setStartSizeVar(10.0);
		arrow_particle->setEndSize(-1.0);
		arrow_particle->setEndSizeVar(0.0);
		arrow_particle->setGravity(ccp(0,-150));
		arrow_particle->setRadialAccel(0.0);
		arrow_particle->setRadialAccelVar(0.0);
		arrow_particle->setSpeed(0);
		arrow_particle->setSpeedVar(0.0);
		arrow_particle->setTangentialAccel(0);
		arrow_particle->setTangentialAccelVar(0);
		arrow_particle->setLife(1.f);
		arrow_particle->setLifeVar(0.5f);
		arrow_particle->setStartSpin(0.0);
		arrow_particle->setStartSpinVar(0.f);
		arrow_particle->setEndSpin(0.0);
		arrow_particle->setEndSpinVar(0.f);
		arrow_particle->setPosition(ccp(240,240));
		arrow_particle->setPosVar(ccp(60,60));
		arrow_particle->setAutoRemoveOnFinish(true);
		
		addChild(arrow_particle, kDecreaseCardDurabilityPopup_Z_particle);
		
		t_case->startDecreaseDurability(this, callfunc_selector(DecreaseCardDurabilityPopup::touchOn));
	}
	
	CCParticleSystemQuad* remove_particle;
	
	void removeParticle()
	{
		remove_particle->setDuration(0);
	}
	
	void touchOn()
	{
		int durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, grade_number));
		
		if(durability <= 0)
		{
			is_touch_enable = false;
			arrow_particle->setDuration(0);
			
			remove_particle = CCParticleSystemQuad::createWithTotalParticles(150);
			remove_particle->setPositionType(kCCPositionTypeRelative);
			remove_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("fever_particle.png"));
			remove_particle->setEmissionRate(5000);
			remove_particle->setAngle(0.0);
			remove_particle->setAngleVar(0.0);
			ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
			remove_particle->setBlendFunc(blendFunc);
			remove_particle->setDuration(-1.f);
			remove_particle->setEmitterMode(kCCParticleModeGravity);
			remove_particle->setStartColor(ccc4f(1.f, 1.f, 1.f, 0.f));
			remove_particle->setStartColorVar(ccc4f(0,0,0,1.f));
			remove_particle->setEndColor(ccc4f(0.f,1.f,1.f,0.f));
			remove_particle->setEndColorVar(ccc4f(0, 0, 0, 1.f));
			remove_particle->setStartSize(8.0);
			remove_particle->setStartSizeVar(5.0);
			remove_particle->setEndSize(20.0);
			remove_particle->setEndSizeVar(10.0);
			remove_particle->setGravity(ccp(39.47f,118.4f));
			remove_particle->setRadialAccel(-671.0);
			remove_particle->setRadialAccelVar(0.0);
			remove_particle->setSpeed(51);
			remove_particle->setSpeedVar(300.0);
			remove_particle->setTangentialAccel(0);
			remove_particle->setTangentialAccelVar(0);
			remove_particle->setTotalParticles(150);
			remove_particle->setLife(0.03);
			remove_particle->setLifeVar(0.1);
			remove_particle->setStartSpin(0.0);
			remove_particle->setStartSpinVar(0.f);
			remove_particle->setEndSpin(360.0);
			remove_particle->setEndSpinVar(0.f);
			remove_particle->setPosVar(ccp(selected_card->getContentSize().width/2.f*selected_card->getScale() + 5.f,0));
			remove_particle->setPosition(ccp(selected_card->getPositionX(), selected_card->getPositionY()+selected_card->getContentSize().height/2.f*selected_card->getScale()+5.f));
			remove_particle->setAutoRemoveOnFinish(true);
			addChild(remove_particle, kDecreaseCardDurabilityPopup_Z_particle);
			CCMoveBy* particle_move = CCMoveBy::create(85.f/60.f, ccp(0,-selected_card->getContentSize().height*selected_card->getScale()-10.f));
			CCCallFunc* particle_call = CCCallFunc::create(this, callfunc_selector(DecreaseCardDurabilityPopup::removeParticle));
			CCSequence* particle_seq = CCSequence::createWithTwoActions(particle_move, particle_call);
			remove_particle->runAction(particle_seq);
			
			selected_card->startRemoveCard();
			
			CCDelayTime* t_delay1 = CCDelayTime::create(1.f);
			CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(DecreaseCardDurabilityPopup::fadeGrayTitle));
			CCDelayTime* t_delay2 = CCDelayTime::create(0.5f);
			CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(DecreaseCardDurabilityPopup::removeFromParent));
			CCSequence* t_seq1 = CCSequence::create(t_delay1, t_call1, t_delay2, t_call2, NULL);
			selected_card->runAction(t_seq1);
		}
		else
		{
			is_touch_enable = true;
		}
	}
	
	void fadeGrayTitle()
	{
		gray->runAction(CCFadeTo::create(0.4f, 0));
		decrease_durability_label->runAction(CCFadeTo::create(0.4f, 0));
	}
	
	void closingAction()
	{
		arrow_particle->setDuration(0);
		
		CCMoveTo* t_action = CCMoveTo::create(0.7f, ccp(240,-130));
		CCDelayTime* t_delay1 = CCDelayTime::create(0.4f);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(DecreaseCardDurabilityPopup::fadeGrayTitle));
		CCDelayTime* t_delay2 = CCDelayTime::create(0.4f);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(DecreaseCardDurabilityPopup::removeFromParent));
		CCSequence* t_seq1 = CCSequence::create(t_delay1, t_call1, t_delay2, t_call2, NULL);
		CCSpawn* t_spawn1 = CCSpawn::create(t_action, t_seq1, NULL);
		selected_card->runAction(t_spawn1);
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(is_touch_enable)
		{
			closingAction();
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

#endif /* defined(__DGproto__DecreaseCardDurabilityPopup__) */
