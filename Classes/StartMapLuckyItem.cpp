// StartMapLuckyItem.cpp
//

#include "StartMapLuckyItem.h"
#include "KSUtil.h"
#include "AchieveNoti.h"
#define LZZ_INLINE inline
StartMapLuckyItem * StartMapLuckyItem::create (IntPoint t_point)
{
	StartMapLuckyItem* t_smli = new StartMapLuckyItem();
	t_smli->myInit(t_point);
	t_smli->autorelease();
	return t_smli;
}
void StartMapLuckyItem::checkInnerRect ()
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
		
		AchievementCode i = kAchievementCode_luckyGuy;
		
		int after_value = myAchieve->getRecentValue((AchievementCode)i) + 1;
		
		myAchieve->changeIngCount(AchievementCode(i), after_value);
		
		if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
		   after_value >= myAchieve->getCondition((AchievementCode)i))
		{
			myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
			AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
			CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
		}
	}
	else
	{
		startRemoveAction();
	}
}
void StartMapLuckyItem::startMyAction ()
{
	main_img->addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
													 {
														 main_img->setScale(t*0.5f);
													 }, [](float t){}));
	
//	main_img->runAction(CCScaleTo::create(1.f, 0.7f));
}
void StartMapLuckyItem::createParticle ()
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

//struct GradualType
//{
//	float a, float b, float c;
//};
void StartMapLuckyItem::speedUpAction ()
{
//	main_img->stopAllActions();
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	CCSprite* fx_lucky = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("fx_lucky.ccbi",this));
	CCSprite* fx_lucky = KS::loadCCBI<CCSprite*>(this, "fx_lucky.ccbi").first;
	fx_lucky->setPosition(my_point.convertToCCP());
	fx_lucky->setScale(0.7);
	addChild(fx_lucky, kStartMapLuckyItem_Z_fx);
	
	KS::setBlendFunc(fx_lucky, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
//	reader->release();
	
//	CCDelayTime* t_delay = CCDelayTime::create(0.7f);
//	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::createParticle));
//	CCSequence* particle_seq = CCSequence::createWithTwoActions(t_delay, t_call);
//	
//	
//	CCScaleTo* t_scale1 = CCScaleTo::create(0.5f, 1.f);
//	CCFadeTo* t_fade1 = CCFadeTo::create(0.5f, 195);
//	CCSpawn* t_spawn1 = CCSpawn::create(t_scale1, t_fade1, NULL);
//	CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::speedUp));
//	
//	CCScaleTo* t_scale2 = CCScaleTo::create(0.5f, 1.3f);
//	CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 130);
//	CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_scale2, t_fade2);
//	CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::speedUp));
//	
//	CCScaleTo* t_scale3 = CCScaleTo::create(0.5f, 1.6f);
//	CCFadeTo* t_fade3 = CCFadeTo::create(0.5f, 65);
//	CCSpawn* t_spawn3 = CCSpawn::createWithTwoActions(t_scale3, t_fade3);
//	CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::speedUp));
//	
//	CCScaleTo* t_scale4 = CCScaleTo::create(0.5f, 1.9f);
//	CCFadeTo* t_fade4 = CCFadeTo::create(0.5f, 0);
//	CCSpawn* t_spawn4 = CCSpawn::createWithTwoActions(t_scale4, t_fade4);
//	CCCallFunc* t_call4 = CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::removeFromParent));
//	CCSequence* img_seq = CCSequence::create(t_spawn1, t_call1, t_spawn2, t_call2, t_spawn3, t_call3, t_spawn4, t_call4, NULL);
//	
//	CCSpawn* total_spawn = CCSpawn::createWithTwoActions(particle_seq, img_seq);
	
	
	main_img->setVisible(false);
	
	main_img->addChild(KSTimer::create(0.7f, [=]()
					{
						createParticle();
					}));
	main_img->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f,
	[=](float t)
	{
//		main_img->setScale(t*(1.f-0.5f*0.7f) + 0.5f*0.7f);
//		KS::setOpacity(main_img, 255 - t*(255-195));
	},
	[=](float t)
	{
		speedUp();
		main_img->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f,
		[=](float t)
		{
//			main_img->setScale(t*(1.3f-1.f) + 1.f);
//			KS::setOpacity(main_img, 195 - t*(195-130));
		},
		[=](float t)
		{
			
			speedUp();
			main_img->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f,
															 [=](float t)
															 {
																 //					main_img->setScale(t*(1.9f-1.6f) + 1.6f);
																 //					KS::setOpacity(main_img, 65 - t*(65-0));
															 },
															 [=](float t)
															 {
																 removeFromParent();
															 }));
		}));
	}));
	
//	main_img->runAction(total_spawn);
}
void StartMapLuckyItem::speedUp ()
{
	myGD->communication("Jack_takeSpeedUpItem");
}
void StartMapLuckyItem::startRemoveAction ()
{
	main_img->stopAllActions();
	main_img->addChild(KSGradualValue<float>::create(1.f, 0.f, 1.f,
													 [=](float t)
													 {
														 main_img->setScale(t);
														 //					main_img->setScale(t*(1.9f-1.6f) + 1.6f);
														 //					KS::setOpacity(main_img, 65 - t*(65-0));
													 },
													 [=](float t)
													 {
														 removeFromParent();
													 }));
}
void StartMapLuckyItem::myInit (IntPoint t_point)
{
	my_point = IntPoint(t_point.x, t_point.y);
	
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//
//	main_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ingame_item_lucky.ccbi",this));
	main_img = KS::loadCCBI<CCSprite*>(this, "ingame_item_lucky.ccbi").first;
	main_img->setPosition(my_point.convertToCCP());
	main_img->setScale(0.f);
	addChild(main_img, kStartMapLuckyItem_Z_img);
//	reader->release();
	
	setPosition(CCPointZero);
	startMyAction();
}
#undef LZZ_INLINE
