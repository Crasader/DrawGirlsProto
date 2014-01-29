//
//  TakeCardAnimation.h
//  DGproto
//
//  Created by LitQoo on 2014. 1. 29..
//
//

#ifndef __DGproto__TakeCardAnimation__
#define __DGproto__TakeCardAnimation__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "KSUtil.h"
#include "DataStorageHub.h"

using namespace cocos2d::extension;
using namespace std;

class TakeCardAnimation : public CCLayer{
	CCSprite* gray;
	CCLayer* roundBig;
	CCLayer* roundSmall;
	CCSprite* card;
	CCSprite* cardLight;
	CCSprite* title;
	int touch_priority;
	bool isOpening;
	float kScale;
public:
//	CREATE_FUNC(TakeCardAnimation);
	//CC_SYNTHESIZE(CCNode*, m_contentNode, ContentNode);
	
	static TakeCardAnimation* create(CCSprite* _card, int _touch_priority){
		TakeCardAnimation* ret = new TakeCardAnimation();
		ret->init(_card,_touch_priority);
		ret->autorelease();
		return ret;
	}
	
	bool init(CCSprite* _card,int _touch_priority){
		if(CCLayer::init()==false)return false;
		
		isOpening=true;
		touch_priority=_touch_priority;
		setTouchEnabled(true);
		
		kScale = 141/_card->getContentSize().width;
		
		
		card=_card;
		card->setPosition(ccp(240,155));
		card->setVisible(false);
		card->setScale(kScale*0.7);
		
		this->addChild(card,3);
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		title = CCSprite::create("ending_take_card.png");
		title->setPosition(ccp(240,276));
		title->setScale(0);
		this->addChild(title,2);
		
		gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(240,160));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChild(gray, 1);
		
		auto ret = KS::loadCCBI<CCLayer*>(this, "roundBlue1.ccbi");
		roundBig = ret.first;
		roundBig->setPosition(ccp(0,-95));
		roundBig->setScale(0);
		this->addChild(roundBig,2);
		
		auto ret2 = KS::loadCCBI<CCLayer*>(this, "roundBlue2.ccbi");
		roundSmall = ret2.first;
		roundSmall->setPosition(ccp(0,-95));
		roundSmall->setScale(0);
		this->addChild(roundSmall,2);
		
		cardLight = CCSprite::create("cardLightCoverBlue.png");
		cardLight->setScale(0.7);
		cardLight->setPosition(ccp(240,500));
		
		this->addChild(cardLight,4);

		return true;
	}
	std::function<void(void)> closeFunc;
	
	void setCloseFunc(std::function<void(void)> func){
		closeFunc = func;
	}
	
	void start(){
		isOpening=true;
		gray->runAction(CCSequence::create(
										   CCFadeIn::create(0.5f),
										   CCCallFunc::create(this, callfunc_selector(TakeCardAnimation::step2)),
										   CCDelayTime::create(0.2f)
										   ,NULL
										   )
						);
		

		
		
		
		
	}
	
	void step2(){
		
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(8/30.f, 1, 0.2f),
											   CCDelayTime::create(0.2f),
											   CCCallFunc::create(this, callfunc_selector(TakeCardAnimation::step3))
											   ,NULL
											   )
							);
		
		roundSmall->runAction(CCScaleTo::create(8/60.f, 1, 0.2f));

		
	}
	
	void step3(){
		
		cardLight->runAction(
							 CCSequence::create(
												CCMoveTo::create(7/30.f,ccp(240,150)),
												CCMoveTo::create(7/30.f,ccp(240,155)),
												CCCallFunc::create(this, callfunc_selector(TakeCardAnimation::step4)),
												NULL
												)
							 );
		

	}
	
	void step4(){
		title->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
		
		cardLight->runAction(
							 CCFadeOut::create(16/30.f)
							 );
		card->setVisible(true);
		card->runAction(
						CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f,ccp(0,5)),CCMoveBy::create(0.5f,ccp(0,-5)),NULL))
						);
		
		isOpening=false;
	}
	
	void close(){
		isOpening=true;
		
		title->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3,0)));
		
		card->runAction(CCMoveBy::create(0.2f,ccp(0,300)));
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(0.3,0),
											   CCCallFunc::create(this, callfunc_selector(TakeCardAnimation::closeStep2)),
											   NULL
											   )
							);
		roundSmall->runAction(CCScaleTo::create(0.3f,0));
		
		if(closeFunc)closeFunc();
		
	}
							
	void closeStep2(){
		gray->runAction(
						CCSequence::create(
										   CCFadeOut::create(0.3f),
										   CCCallFunc::create(this, callfunc_selector(TakeCardAnimation::closeStep3)),
										   NULL
										   )
						);
	}
						
	void closeStep3(){
		isOpening=false;
		this->removeFromParent();
	}
	
	void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touch_priority, true);
		
	}
	
	
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
		if(isOpening==false)this->close();
		return true;
	}
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DownCardAnimation : public CCLayer{
	CCSprite* gray;
	CCLayer* roundBig;
	CCLayer* roundSmall;
	CCSprite* card;
	CCSprite* cardLight;
	CCSprite* title;
	int touch_priority;
	bool isOpening;
	float kScale;
public:
	//	CREATE_FUNC(TakeCardAnimation);
	//CC_SYNTHESIZE(CCNode*, m_contentNode, ContentNode);
	
	static DownCardAnimation* create(CCSprite* _card, int _touch_priority){
		DownCardAnimation* ret = new DownCardAnimation();
		ret->init(_card,_touch_priority);
		ret->autorelease();
		return ret;
	}
	
	bool init(CCSprite* _card,int _touch_priority){
		
		if(CCLayer::init()==false)return false;
		isOpening=true;
		touch_priority=_touch_priority;
		setTouchEnabled(true);
		
		kScale = 141/_card->getContentSize().width;
		
		card=_card;
		
		card->setPosition(ccp(240,155));
		card->setVisible(false);
		card->setScale(kScale*0.7);
		this->addChild(card,3);
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		title = CCSprite::create("ending_decrease_durability.png");
		title->setPosition(ccp(240,276));
		title->setScale(0);
		this->addChild(title,2);
		
		gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(240,160));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChild(gray, 1);
		
		auto ret = KS::loadCCBI<CCLayer*>(this, "roundRed1.ccbi");
		roundBig = ret.first;
		roundBig->setPosition(ccp(0,-95));
		roundBig->setScale(0);
		this->addChild(roundBig,2);
		
		auto ret2 = KS::loadCCBI<CCLayer*>(this, "roundRed2.ccbi");
		roundSmall = ret2.first;
		roundSmall->setPosition(ccp(0,-95));
		roundSmall->setScale(0);
		this->addChild(roundSmall,2);
		
		cardLight = CCSprite::create("cardLightCoverRed.png");
		cardLight->setScale(0.7);
		cardLight->setPosition(ccp(240,500));
		
		this->addChild(cardLight,4);
		
		return true;
	}
	std::function<void(void)> closeFunc;
	
	void setCloseFunc(std::function<void(void)> func){
		closeFunc = func;
	}
	void start(){
		isOpening=true;
		gray->runAction(CCSequence::create(
										   CCFadeIn::create(0.5f),
										   CCCallFunc::create(this, callfunc_selector(DownCardAnimation::step2)),
										   CCDelayTime::create(0.2f)
										   ,NULL
										   )
						);
		
		
		
		
		
		
	}
	
	void step2(){
		
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(8/30.f, 1, 0.2f),
											   CCDelayTime::create(0.2f),
											   CCCallFunc::create(this, callfunc_selector(DownCardAnimation::step3))
											   ,NULL
											   )
							);
		
		roundSmall->runAction(CCScaleTo::create(8/30.f, 1, 0.2f));
		
		
	}
	
	void step3(){
		
		cardLight->runAction(
							 CCSequence::create(
												CCMoveTo::create(7/30.f,ccp(240,150)),
												CCMoveTo::create(7/30.f,ccp(240,155)),
												CCCallFunc::create(this, callfunc_selector(DownCardAnimation::step4)),
												NULL
												)
							 );
		
		
	}
	
	void step4(){
		title->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
		
		cardLight->runAction(
							 CCFadeOut::create(16/30.f)
							 );
		card->setVisible(true);
		card->runAction(
						CCRepeatForever::create(CCSequence::create(CCTintTo::create(5/30.f, 255, 100, 100),CCTintTo::create(5/30.f, 255, 255, 255),CCMoveBy::create(0.5f,ccp(0,5)),CCMoveBy::create(0.5f,ccp(0,-5)),NULL))
						);
		
		isOpening=false;
	}
	
	void close(){
		isOpening=true;
		
		title->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3,0)));
		
		card->runAction(CCMoveBy::create(0.2f,ccp(0,300)));
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(0.3,0),
											   CCCallFunc::create(this, callfunc_selector(DownCardAnimation::closeStep2)),
											   NULL
											   )
							);
		roundSmall->runAction(CCScaleTo::create(0.3f,0));
		
		if(closeFunc)closeFunc();
	}
	
	void closeStep2(){
		gray->runAction(
						CCSequence::create(
										   CCFadeOut::create(0.3f),
										   CCCallFunc::create(this, callfunc_selector(DownCardAnimation::closeStep3)),
										   NULL
										   )
						);
	}
	
	void closeStep3(){
		isOpening=false;
		this->removeFromParent();
	}
	
	void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touch_priority, true);
		
	}
	
	
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
		if(isOpening==false)this->close();
		return true;
	}
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RemoveCardAnimation : public CCLayer{
	CCSprite* gray;
	CCLayer* roundBig;
	CCLayer* roundSmall;
	CCSprite* card;
	CCSprite* cardLight;
	CCSprite* title;
	CCSprite* stencil;
	int touch_priority;
	bool isOpening;
	CCParticleSystemQuad* particle;
	float removeSpeed;
	float kScale;
	CCControlButton* skipBtn;
	CCControlButton* repairBtn;
	
public:
	//	CREATE_FUNC(TakeCardAnimation);
	//CC_SYNTHESIZE(CCNode*, m_contentNode, ContentNode);
	
	static RemoveCardAnimation* create(CCSprite* _card, int _touch_priority){
		RemoveCardAnimation* ret = new RemoveCardAnimation();
		ret->init(_card,_touch_priority);
		ret->autorelease();
		return ret;
	}
	
	bool init(CCSprite* _card,int _touch_priority){
		if(CCLayer::init()==false)return false;
		isOpening=true;
		touch_priority=_touch_priority;
		setTouchEnabled(true);
		kScale = 141/_card->getContentSize().width;
		
		
		removeSpeed=0.2f;
		card = _card;
		card->setPosition(ccp(240,155));
		card->setScale(kScale*0.7);
		card->setVisible(false);
		//this->addChild(card,3);
		
		
		stencil = CCSprite::create("back_gray.png");
		stencil->setAnchorPoint(ccp(0.5,0));
		stencil->setPosition(ccp(240,320));
		
		CCClippingNode *cliper = CCClippingNode::create(stencil);
		//you want to hide intersection so we setInverted to true
		cliper->setInverted(true);
		cliper->addChild(card);
		addChild(cliper,3);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		title = CCSprite::create("ending_remove_card.png");
		title->setPosition(ccp(240,276));
		title->setScale(0);
		this->addChild(title,2);
		
		gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(240,160));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChild(gray, 1);
		
		auto ret = KS::loadCCBI<CCLayer*>(this, "roundRed1.ccbi");
		roundBig = ret.first;
		roundBig->setPosition(ccp(0,-95));
		roundBig->setScale(0);
		this->addChild(roundBig,2);
		
		auto ret2 = KS::loadCCBI<CCLayer*>(this, "roundRed2.ccbi");
		roundSmall = ret2.first;
		roundSmall->setPosition(ccp(0,-95));
		roundSmall->setScale(0);
		this->addChild(roundSmall,2);
		
		cardLight = CCSprite::create("cardLightCoverRed.png");
		cardLight->setScale(0.7);
		cardLight->setPosition(ccp(240,500));
		
		this->addChild(cardLight,4);
		
		
		particle = CCParticleSystemQuad::createWithTotalParticles(150);
		particle->setPositionType(kCCPositionTypeRelative);
		//particle->setAutoRemoveOnFinish(true);
		particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("fever_particle.png"));
		particle->setEmissionRate(5000);
		particle->setAngle(0.0);
		particle->setAngleVar(0.0);
		ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
		particle->setBlendFunc(blendFunc);
		particle->setDuration(-1);
		particle->setEmitterMode(kCCParticleModeGravity);
		particle->setStartColor(ccc4f(1.f, 1.f, 1.f, 0.f));
		particle->setStartColorVar(ccc4f(0,0,0,1.f));
		particle->setEndColor(ccc4f(1.f,0.f,0.f,0.f));
		particle->setEndColorVar(ccc4f(0, 0, 0, 1.f));
		particle->setStartSize(8.0);
		particle->setStartSizeVar(5.0);
		particle->setEndSize(20.0);
		particle->setEndSizeVar(10.0);
		particle->setGravity(ccp(240,160));
		particle->setRadialAccel(-671.0);
		particle->setRadialAccelVar(0.0);
		particle->setSpeed(51);
		particle->setSpeedVar(300.0);
		particle->setTangentialAccel(0);
		particle->setTangentialAccelVar(0);
		particle->setTotalParticles(150);
		particle->setLife(0.03);
		particle->setLifeVar(0.1);
		particle->setStartSpin(0.0);
		particle->setStartSpinVar(0.f);
		particle->setEndSpin(360.0);
		particle->setEndSpinVar(0.f);
		particle->setPosVar(ccp(50,0));
		particle->setPosition(ccp(240,220));		//
		particle->setVisible(false);
		addChild(particle, 100);
		
		repairBtn = CCControlButton::create("고치기", "", 20);
		repairBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RemoveCardAnimation::stopRemove), CCControlEventTouchUpInside);
		repairBtn->setPosition(ccp(200,40));
		repairBtn->setTouchPriority(touch_priority-1);
		repairBtn->setScale(0);
		addChild(repairBtn,9);
		
		skipBtn = CCControlButton::create("스킵","",20);
		skipBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RemoveCardAnimation::skip), CCControlEventTouchUpInside);
		skipBtn->setPosition(ccp(280,40));
		skipBtn->setTouchPriority(touch_priority-1);
		skipBtn->setScale(0);
		addChild(skipBtn,9);
		
		skipFunc = NULL;
		repairFunc = NULL;
		closeFunc = NULL;
		
		return true;
	}
	
	std::function<void(void)> skipFunc;
	std::function<void(void)> repairFunc;
	std::function<void(void)> closeFunc;
	
	void setSkipFunc(std::function<void(void)> func){
		skipFunc = func;
	}
	
	void setRepairFunc(std::function<void(void)> func){
		repairFunc = func;
	}
	
	void setCloseFunc(std::function<void(void)> func){
		closeFunc = func;
	}
	
	void repair(){
		
		particle->setVisible(true);
		this->schedule(schedule_selector(RemoveCardAnimation::repairSchedule));
		
	}
	
	void skip(){
		removeSpeed+=4;
		
		
		repairBtn->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,0)));
		skipBtn->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,0)));
		
		if(skipFunc)skipFunc();
	}
	
	
	void stopRemove(){
		this->unschedule(schedule_selector(RemoveCardAnimation::removeSchedule));
		
		
		repairBtn->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,0)));
		skipBtn->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,0)));
		
		
		if(repairFunc)repairFunc();
	}
	
	
	void start(){
		isOpening=true;
		
		gray->runAction(CCSequence::create(
										   CCFadeIn::create(0.5f),
										   CCCallFunc::create(this, callfunc_selector(RemoveCardAnimation::step2)),
										   CCDelayTime::create(0.2f)
										   ,NULL
										   )
						);
		
		
		
		

		
		
	}
	
	void step2(){
		
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(8/30.f, 1, 0.2f),
											   CCDelayTime::create(0.2f),
											   CCCallFunc::create(this, callfunc_selector(RemoveCardAnimation::step3))
											   ,NULL
											   )
							);
		
		roundSmall->runAction(CCScaleTo::create(8/30.f, 1, 0.2f));
		
		
	}
	
	void step3(){
		
		cardLight->runAction(
							 CCSequence::create(
												CCMoveTo::create(7/30.f,ccp(240,150)),
												CCMoveTo::create(7/30.f,ccp(240,155)),
												CCCallFunc::create(this, callfunc_selector(RemoveCardAnimation::step4)),
												NULL
												)
							 );
		
		
	}
	
	void step4(){
		
		title->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
		
		repairBtn->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
		skipBtn->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
		
		cardLight->runAction(
							 CCFadeOut::create(16/30.f)
							 );
		card->setVisible(true);
		card->runAction(
						CCRepeatForever::create(CCSequence::create(CCTintTo::create(5/30.f, 255, 100, 100),CCTintTo::create(5/30.f, 255, 255, 255),CCMoveBy::create(0.5f,ccp(0,5)),CCMoveBy::create(0.5f,ccp(0,-5)),NULL))
						);
		this->runAction(CCSequence::create(CCDelayTime::create(0.1),CCCallFunc::create(this, callfunc_selector(RemoveCardAnimation::step5)),NULL));
	}
	
	void step5(){
		particle->setVisible(true);
		isOpening=false;
		
		this->schedule(schedule_selector(RemoveCardAnimation::removeSchedule));
		
		
	}
	
	void removeSchedule(float dt){
		float y =  particle->getPositionY();
		particle->setPositionY(y-removeSpeed);
		stencil->setPositionY(y-removeSpeed);
		if(y-removeSpeed<card->getPositionY()-(card->getContentSize().height/2*card->getScale())){
			particle->setVisible(false);
			doRemoveCard();
			this->unschedule(schedule_selector(RemoveCardAnimation::removeSchedule));
		}
	}
		
	void repairSchedule(float dt){
		float y =  particle->getPositionY();
		particle->setPositionY(y+1);
		stencil->setPositionY(y+1);
		if(y+1>=card->getPositionY()+(card->getContentSize().height/2*card->getScale())){
			stencil->setPositionY(500);
			particle->setVisible(false);
			doRepair();
			this->unschedule(schedule_selector(RemoveCardAnimation::repairSchedule));
		}
	}
	
	void doRepair(){
		//CCLog("doRepair");
		close();
	}
	
	void doRemoveCard(){
		//CCLog("doRemoveCard");
		close();
	}
	
	void close(){
		isOpening=true;
		
		title->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3,0)));
		
		
		card->runAction(CCMoveBy::create(0.2f,ccp(0,300)));
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(0.3,0),
											   CCCallFunc::create(this, callfunc_selector(RemoveCardAnimation::closeStep2)),
											   NULL
											   )
							);
		roundSmall->runAction(CCScaleTo::create(0.3f,0));
		
	}
	
	void closeStep2(){
		gray->runAction(
						CCSequence::create(
										   CCFadeOut::create(0.3f),
										   CCCallFunc::create(this, callfunc_selector(RemoveCardAnimation::closeStep3)),
										   NULL
										   )
						);
	}
	
	void closeStep3(){
		isOpening=false;
		this->removeFromParent();
		if(closeFunc)closeFunc();
	}
	
	void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touch_priority, true);
		
	}
	
	
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
		//removeSpeed++;
		//stopRemove();
		//if(isOpening==false)this->close();
		return true;
	}
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class StrengthCardAnimation : public CCLayer{
	CCSprite* gray;
	CCLayer* roundBig;
	CCLayer* roundSmall;
	CCSprite* card;
	CCSprite* card2;
	CCSprite* cardLight;
	CCSprite* title;
	CCLabelTTF* optionLbl;
	int touch_priority;
	bool isOpening;
	float kScale;
public:
	//	CREATE_FUNC(TakeCardAnimation);
	//CC_SYNTHESIZE(CCNode*, m_contentNode, ContentNode);
	
	static StrengthCardAnimation* create(CCSprite* _card,CCSprite* _card2, int _touch_priority){
		StrengthCardAnimation* ret = new StrengthCardAnimation();
		ret->init(_card,_card2,_touch_priority);
		ret->autorelease();
		return ret;
	}
	
	bool init(CCSprite* _card,CCSprite* _card2,int _touch_priority){
		if(CCLayer::init()==false)return false;
		
		//141,188
		kScale = 141/_card->getContentSize().width;
		
		isOpening=true;
		touch_priority=_touch_priority;
		setTouchEnabled(true);
		card=_card;
		card->setPosition(ccp(240,155));
		card->setVisible(false);
		card->setScale(kScale*0.7);
		this->addChild(card,3);
		
		optionLbl = CCLabelTTF::create("", "", 15);
		optionLbl->setPosition(ccp(240,40));
		optionLbl->setScale(0);
		this->addChild(optionLbl,4);
		
		card2=_card2;
		card2->setPosition(ccp(240,-410));
		card2->setAnchorPoint(ccp(0.5,0));
		this->addChild(card2,5);
		
		//kScale = card2->getScale();
		card2->setScale(kScale*2);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		title = CCSprite::create("ending_take_card.png");
		title->setPosition(ccp(240,276));
		title->setScale(0);
		this->addChild(title,2);
		
		gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(240,160));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChild(gray, 1);
		
		auto ret = KS::loadCCBI<CCLayer*>(this, "roundBlue1.ccbi");
		roundBig = ret.first;
		roundBig->setPosition(ccp(0,-95));
		roundBig->setScale(0);
		this->addChild(roundBig,2);
		
		auto ret2 = KS::loadCCBI<CCLayer*>(this, "roundBlue2.ccbi");
		roundSmall = ret2.first;
		roundSmall->setPosition(ccp(0,-95));
		roundSmall->setScale(0);
		this->addChild(roundSmall,2);
		
		cardLight = CCSprite::create("cardLightCoverBlue.png");
		cardLight->setScale(0.75);
		cardLight->setPosition(ccp(240,500));
		
		this->addChild(cardLight,4);
		
		return true;
	}
	std::function<void(void)> closeFunc;
	
	void setCloseFunc(std::function<void(void)> func){
		closeFunc = func;
	}
	
	
	void startSuccess(string option){
		isOpening=true;
		optionLbl->setString(option.c_str());
		gray->runAction(CCSequence::create(
										   CCFadeIn::create(0.5f),
										   CCCallFunc::create(this, callfunc_selector(StrengthCardAnimation::step2)),
										   CCDelayTime::create(0.2f)
										   ,NULL
										   )
						);
		
		
		
		
		
		
	}

	void startFail(string option){
		isOpening=true;
		optionLbl->setString(option.c_str());
		gray->runAction(CCSequence::create(
										   CCFadeIn::create(0.5f),
										   CCCallFunc::create(this, callfunc_selector(StrengthCardAnimation::step2)),
										   CCDelayTime::create(0.2f)
										   ,NULL
										   )
						);
		
		
		
		
		
		
	}
	
	void step2(){
		
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(8/60.f, 1, 0.2f),
											   CCDelayTime::create(0.2f),
											   CCCallFunc::create(this, callfunc_selector(StrengthCardAnimation::step3))
											   ,NULL
											   )
							);
		
		roundSmall->runAction(CCScaleTo::create(8/60.f, 1, 0.2f));
		
		
	}
	
	void step3(){
		
		cardLight->runAction(
							 CCSequence::create(
												CCMoveTo::create(7/60.f,ccp(240,150)),
												CCMoveTo::create(7/60.f,ccp(240,155)),
												CCCallFunc::create(this, callfunc_selector(StrengthCardAnimation::step4)),
												NULL
												)
							 );
		
		
	}
	
	void step4(){
		title->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
		cardLight->runAction(
							 CCSequence::create(
												CCFadeOut::create(16/60.f),
												CCDelayTime::create(10/30.f),
												CCCallFunc::create(this, callfunc_selector(StrengthCardAnimation::step5)),
												NULL
												)
							 );
		card->setVisible(true);
//		card->runAction(
//						CCRepeatForever::create(
//												CCSequence::create(
//																   CCMoveBy::create(0.5f,ccp(0,5)),
//																   CCMoveBy::create(0.5f,ccp(0,-5))
//																   ,NULL)
//												)
//						);
		
	}
	
	//재료카드액션
	void step5(){

		
		card2->runAction(
						 CCSequence::create(
											CCSpawn::create(
															CCMoveTo::create(10/30.f, ccp(240,100)),
															CCScaleTo::create(10/30.f, kScale*0.7),
															NULL
															),
											CCSpawn::create(
															CCMoveTo::create(0, ccp(240,-410)),
															CCScaleTo::create(0, kScale*2.f),
															NULL
															),
											CCSpawn::create(
															CCMoveTo::create(10/30.f, ccp(240,100)),
															CCScaleTo::create(10/30.f, kScale*0.7),
															NULL
															),
											CCSpawn::create(
															CCMoveTo::create(0, ccp(240,-410)),
															CCScaleTo::create(0, kScale*2.f),
															NULL
															),
											CCSpawn::create(
															CCMoveTo::create(10/30.f, ccp(240,100)),
															CCScaleTo::create(10/30.f, kScale*0.7),
															NULL
															),
											CCSpawn::create(
															CCMoveTo::create(0, ccp(240,-410)),
															CCScaleTo::create(0, kScale*2.f),
															NULL
															),
											CCDelayTime::create(0.5f),
											CCCallFunc::create(this,callfunc_selector(StrengthCardAnimation::step55)),
						 NULL
						 )
		);
		cardLight->runAction(
							 CCSpawn::create(
											CCFadeIn::create(24/30.f),
											 CCSequence::create(
																CCDelayTime::create(10/30.f),
																CCScaleTo::create(2/30.f,0.8),
																CCScaleTo::create(0,0.75),
																CCDelayTime::create(8/30.f),
																CCScaleTo::create(2/30.f,0.8),
																CCScaleTo::create(0,0.75),
																CCDelayTime::create(8/30.f),
																CCScaleTo::create(2/30.f,0.8),
																CCScaleTo::create(0,0.75),
																NULL
											 ),
											 NULL
											)
							 );
	}
	
	void step55(){
		card->setVisible(false);
		cardLight->setPositionY(155);
		
		cardLight->runAction(
							 CCSequence::create(
												CCOrbitCamera::create(0.1f, 0.15f, 0, 0, 90, 0, 0),
												CCOrbitCamera::create(0.1f, 0.15f, 0, -90, 90, 0, 0),
												CCOrbitCamera::create(0.1f, 0.15f, 0, 0, 90, 0, 0),
												CCOrbitCamera::create(0.1f, 0.15f, 0, -90, 90, 0, 0),
												CCOrbitCamera::create(0.1f, 0.15f, 0, 0, 90, 0, 0),
												CCOrbitCamera::create(0.1f, 0.15f, 0, -90, 90, 0, 0),
												CCCallFunc::create(this,callfunc_selector(StrengthCardAnimation::step6)),
												NULL
												)
							 );
	}
	//카드라이트페이드아웃
	void step6(){
		card->setVisible(true);
		cardLight->runAction(
							 CCSequence::create(
												CCFadeOut::create(24/30.f),
												CCCallFunc::create(this,callfunc_selector(StrengthCardAnimation::step7)),
												NULL)
							 );
							
		optionLbl->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
	}
	
	void step7(){
				card->runAction(
								CCRepeatForever::create(
														CCSequence::create(
																		   CCMoveBy::create(0.5f,ccp(0,5)),
																		   CCMoveBy::create(0.5f,ccp(0,-5))
																		   ,NULL)
														)
								);
		isOpening=false;
	}
	
	void close(){
		isOpening=true;
		
		title->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3,0)));
		optionLbl->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3,0)));
		
		card->runAction(CCMoveBy::create(0.2f,ccp(0,300)));
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(0.3,0),
											   CCCallFunc::create(this, callfunc_selector(StrengthCardAnimation::closeStep2)),
											   NULL
											   )
							);
		roundSmall->runAction(CCScaleTo::create(0.3f,0));
		
		if(closeFunc)closeFunc();
		
	}
	
	void closeStep2(){
		gray->runAction(
						CCSequence::create(
										   CCFadeOut::create(0.3f),
										   CCCallFunc::create(this, callfunc_selector(StrengthCardAnimation::closeStep3)),
										   NULL
										   )
						);
	}
	
	void closeStep3(){
		isOpening=false;
		this->removeFromParent();
	}
	
	void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touch_priority, true);
		
	}
	
	
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
		if(isOpening==false)this->close();
		return true;
	}
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	
};

#endif /* defined(__DGproto__TakeCardAnimation__) */
