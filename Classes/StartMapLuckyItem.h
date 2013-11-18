//
//  StartMapLuckyItem.h
//  DGproto
//
//  Created by 사원3 on 13. 11. 12..
//
//

#ifndef __DGproto__StartMapLuckyItem__
#define __DGproto__StartMapLuckyItem__

#include "cocos2d.h"
#include "GameData.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum StartMapLuckyItemZorder
{
	kStartMapLuckyItem_Z_img = 1,
	kStartMapLuckyItem_Z_particle,
	kStartMapLuckyItem_Z_fx
};

class StartMapLuckyItem : public CCNode
{
public:
	static StartMapLuckyItem* create(IntPoint t_point)
	{
		StartMapLuckyItem* t_smli = new StartMapLuckyItem();
		t_smli->myInit(t_point);
		t_smli->autorelease();
		return t_smli;
	}
	
	void checkInnerRect()
	{
		if(myGD->mapState[my_point.x][my_point.y] == mapOldget || myGD->mapState[my_point.x][my_point.y] == mapOldline ||
		   myGD->mapState[my_point.x-1][my_point.y] == mapOldget || myGD->mapState[my_point.x-1][my_point.y] == mapOldline ||
		   myGD->mapState[my_point.x-1][my_point.y-1] == mapOldget || myGD->mapState[my_point.x-1][my_point.y-1] == mapOldline ||
		   myGD->mapState[my_point.x-1][my_point.y+1] == mapOldget || myGD->mapState[my_point.x-1][my_point.y+1] == mapOldline ||
		   myGD->mapState[my_point.x][my_point.y-1] == mapOldget || myGD->mapState[my_point.x][my_point.y-1] == mapOldline ||
		   myGD->mapState[my_point.x][my_point.y+1] == mapOldget || myGD->mapState[my_point.x][my_point.y+1] == mapOldline ||
		   myGD->mapState[my_point.x+1][my_point.y-1] == mapOldget || myGD->mapState[my_point.x+1][my_point.y-1] == mapOldline ||
		   myGD->mapState[my_point.x+1][my_point.y] == mapOldget || myGD->mapState[my_point.x+1][my_point.y] == mapOldline ||
		   myGD->mapState[my_point.x+1][my_point.y+1] == mapOldget || myGD->mapState[my_point.x+1][my_point.y+1] == mapOldline)
		{
			speedUpAction();
		}
		else
		{
			startRemoveAction();
		}
	}
	
private:
	IntPoint my_point;
	CCSprite* main_img;
	
	void startMyAction()
	{
		main_img->runAction(CCScaleTo::create(1.f, 0.7f));
	}
	
	void createParticle()
	{
		CCParticleSystemQuad* bomb_particle = CCParticleSystemQuad::createWithTotalParticles(150);
		bomb_particle->setPositionType(kCCPositionTypeRelative);
		bomb_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle2.png"));
		bomb_particle->setEmissionRate(5000);
		bomb_particle->setAngle(90.0);
		bomb_particle->setAngleVar(360.0);
		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
		bomb_particle->setBlendFunc(blendFunc);
		bomb_particle->setDuration(0.01f);
		bomb_particle->setEmitterMode(kCCParticleModeGravity);
		bomb_particle->setStartColor(ccc4f(0.f, 1.f, 1.f, 1.f));
		bomb_particle->setStartColorVar(ccc4f(0,0.16f,0.16f,0.f));
		bomb_particle->setEndColor(ccc4f(0.4f,0.8f,1.f,0.f));
		bomb_particle->setEndColorVar(ccc4f(0, 0.16f, 0.16f, 0.f));
		bomb_particle->setStartSize(0.f);
		bomb_particle->setStartSizeVar(0.f);
		bomb_particle->setEndSize(20.0);
		bomb_particle->setEndSizeVar(5.0);
		bomb_particle->setGravity(ccp(0.f,0.f));
		bomb_particle->setRadialAccel(0.0);
		bomb_particle->setRadialAccelVar(0.0);
		bomb_particle->setSpeed(60);
		bomb_particle->setSpeedVar(30.0);
		bomb_particle->setTangentialAccel(0);
		bomb_particle->setTangentialAccelVar(10);
		bomb_particle->setTotalParticles(150);
		bomb_particle->setLife(0.00);
		bomb_particle->setLifeVar(1.2);
		bomb_particle->setStartSpin(0.0);
		bomb_particle->setStartSpinVar(0.f);
		bomb_particle->setEndSpin(0.0);
		bomb_particle->setEndSpinVar(0.f);
		bomb_particle->setPosVar(ccp(0.f,0.f));
		bomb_particle->setPosition(my_point.convertToCCP());
		bomb_particle->setAutoRemoveOnFinish(true);
		bomb_particle->setScale(0.7);
		addChild(bomb_particle, kStartMapLuckyItem_Z_particle);
	}
	
	void speedUpAction()
	{
		main_img->stopAllActions();
		
		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		CCBReader* reader = new CCBReader(nodeLoader);
		CCSprite* fx_lucky = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("fx_lucky.ccbi",this));
		fx_lucky->setPosition(my_point.convertToCCP());
		fx_lucky->setScale(0.7);
		addChild(fx_lucky, kStartMapLuckyItem_Z_fx);
		
		CCDelayTime* t_delay = CCDelayTime::create(0.7f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::createParticle));
		CCSequence* particle_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		
		CCScaleTo* t_scale1 = CCScaleTo::create(0.5f, 1.f);
		CCFadeTo* t_fade1 = CCFadeTo::create(0.5f, 195);
		CCSpawn* t_spawn1 = CCSpawn::create(t_scale1, t_fade1, NULL);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::speedUp));
		
		CCScaleTo* t_scale2 = CCScaleTo::create(0.5f, 1.3f);
		CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 130);
		CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_scale2, t_fade2);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::speedUp));
		
		CCScaleTo* t_scale3 = CCScaleTo::create(0.5f, 1.6f);
		CCFadeTo* t_fade3 = CCFadeTo::create(0.5f, 65);
		CCSpawn* t_spawn3 = CCSpawn::createWithTwoActions(t_scale3, t_fade3);
		CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::speedUp));
		
		CCScaleTo* t_scale4 = CCScaleTo::create(0.5f, 1.9f);
		CCFadeTo* t_fade4 = CCFadeTo::create(0.5f, 0);
		CCSpawn* t_spawn4 = CCSpawn::createWithTwoActions(t_scale4, t_fade4);
		CCCallFunc* t_call4 = CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::removeFromParent));
		CCSequence* img_seq = CCSequence::create(t_spawn1, t_call1, t_spawn2, t_call2, t_spawn3, t_call3, t_spawn4, t_call4, NULL);
		
		CCSpawn* total_spawn = CCSpawn::createWithTwoActions(particle_seq, img_seq);
		
		main_img->runAction(total_spawn);
	}
	
	void speedUp()
	{
		myGD->communication("Jack_takeSpeedUpItem");
	}
	
	void startRemoveAction()
	{
		main_img->stopAllActions();
		main_img->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(1.f, 0.f), CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::removeFromParent))));
	}
	
	void myInit(IntPoint t_point)
	{
		my_point = IntPoint(t_point.x, t_point.y);
		
		main_img = CCSprite::create("item_lucky.png");
		main_img->setPosition(my_point.convertToCCP());
		main_img->setScale(0.f);
		addChild(main_img, kStartMapLuckyItem_Z_img);
		
		setPosition(CCPointZero);
		startMyAction();
	}
};

#endif /* defined(__DGproto__StartMapLuckyItem__) */
