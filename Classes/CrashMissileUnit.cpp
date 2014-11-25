//
//  CrashMissileUnit.cpp
//  DGproto
//
//  Created by ksoo k on 2014. 11. 18..
//
//

#include "MissileUnit.h"

#include "StarGoldData.h"
#include "KSUtil.h"

#include "ProbSelector.h"
#include "MissileDamageData.h"

#include "KSUtil.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "KSUtil.h"
#include "GameData.h"
#include "DataStorageHub.h"
#include "Jack.h"
#include "EffectSprite.h"
#include "EasingAction.h"

using namespace std;
using namespace placeholders;

ThrowObject * ThrowObject::create (CCPoint t_sp, int t_type, float t_speed, float t_angle, IntSize t_mSize)
{
	ThrowObject* t_to = new ThrowObject();
	t_to->myInit(t_sp, t_type, t_speed, t_angle, t_mSize);
	t_to->autorelease();
	return t_to;
}
void ThrowObject::startMyAction ()
{
	AudioEngine::sharedInstance()->playEffect("sound_throw_obj_shot.mp3", false);
	b_c_p = getPosition();
	schedule(schedule_selector(ThrowObject::myAction));
}
void ThrowObject::jackDie ()
{
	unschedule(schedule_selector(ThrowObject::myAction));
	removeEffect();
}
void ThrowObject::lineDie (IntPoint t_p)
{
	unschedule(schedule_selector(ThrowObject::myAction));
	myGD->communication("Main_showLineDiePosition", t_p);
	removeEffect();
}
void ThrowObject::removeEffect ()
{
	//		objImg->unscheduleAllSelectors();
	objImg->stopAllActions();
	CCScaleTo* t_fade = CCScaleTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ThrowObject::selfRemove));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	
	objImg->runAction(t_seq);
}
void ThrowObject::selfRemove ()
{
	removeFromParentAndCleanup(true);
	myGD->communication("MS_resetRects", false);
}
void ThrowObject::stopMyAction ()
{
	CCLOG("crashCount  %d ", crashCount);
	if(crashCount <= 60)
	{
		myGD->toScratch();
	}
	unschedule(schedule_selector(ThrowObject::myAction));
	removeFromParentAndCleanup(true);
}
void ThrowObject::myAction ()
{
	//		objImg->setRotation(objImg->getRotation() + random_spin);
	objects->setPosition(ccpAdd(objects->getPosition(), dv));
	
	CCPoint myPosition = objects->getPosition();
	CCPoint subPosition = ccpSub(myPosition, b_c_p);
	subPosition.x = fabsf(subPosition.x);
	subPosition.y = fabsf(subPosition.y);
	
	if(subPosition.x > mSize.width || subPosition.y > mSize.height)
	{
		b_c_p = myPosition;
		IntPoint myPoint = IntPoint((myPosition.x-1)/pixelSize + 1 - mSize.width/2, (myPosition.y-1)/pixelSize + 1 - mSize.height/2);
		for(int i=0;i<mSize.height;i++)
		{
			for(int j=0;j<mSize.width;j++)
			{
				auto result = crashMapForIntPoint(IntPoint(myPoint.x+j,myPoint.y+i));
				if(result)
				{
					crashCount++;
				}
			}
		}
	}
	
	if(myPosition.x < -30 || myPosition.x > 350 || myPosition.y < -30 || myPosition.y > 460)
	{
		stopMyAction();
	}
}
void ThrowObject::myInit (CCPoint t_sp, int t_type, float t_speed, float t_angle, IntSize t_mSize)
{
	// t_sp 자리에서 부터 t_angle 각도로 던짐.
	//
	
	objects = CCNode::create();
	addChild(objects);
	objects->setPosition(t_sp);
	type = t_type;
	mSize = t_mSize;
	pair<CCSprite*, CCBAnimationManager*> alreadyWarning, alreadyWarning2;
	if(type == 11) // 하나 짜리 던지기.
	{
		//		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		//		CCBReader* reader = new CCBReader(nodeLoader);
		//		objImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("pattern_saw1.ccbi",this));
		//KS::setBlendFunc(objImg, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		//		reader->release();
		objImg = KS::loadCCBI<CCSprite*>(this, "pattern_saw1.ccbi").first;
		objects->addChild(objImg);
		objImg->setVisible(false);
		
		CCNode* guideNode = CCNode::create();
		guideNode->setRotation(-t_angle);
		addChild(guideNode);
		guideNode->setPosition(t_sp);
		alreadyWarning = KS::loadCCBI<CCSprite*>(this, "signal1_1.ccbi");
		alreadyWarning2 = KS::loadCCBI<CCSprite*>(this, "signal1_2.ccbi");
		guideNode->addChild(alreadyWarning.first);
		guideNode->addChild(alreadyWarning2.first);
		alreadyWarning2.first->setPosition(ccp(55.f / 2.f, 0));
	}
	else if(type == 14) // 여러개 던지기.
	{
		//		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		//		CCBReader* reader = new CCBReader(nodeLoader);
		//		objImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("pattern_saw4.ccbi",this));
		//		//KS::setBlendFunc(objImg, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		//		reader->release();
		objImg = KS::loadCCBI<CCSprite*>(this, "pattern_saw4.ccbi").first;
		objects->addChild(objImg);
		objImg->setVisible(false);
		
		CCNode* guideNode = CCNode::create();
		guideNode->setRotation(-t_angle);
		addChild(guideNode);
		guideNode->setPosition(t_sp);
		alreadyWarning = KS::loadCCBI<CCSprite*>(this, "signal2_1.ccbi");
		alreadyWarning2 = KS::loadCCBI<CCSprite*>(this, "signal2_2.ccbi");
		guideNode->addChild(alreadyWarning.first);
		guideNode->addChild(alreadyWarning2.first);
		alreadyWarning2.first->setPosition(ccp(35.f / 2.f, 0));
	}
	
	dv.x = 1;
	dv.y = tanf(t_angle/180.f*M_PI);
	
	if((t_angle > 90 && t_angle < 270) || t_angle < -90)
		dv = ccpMult(dv, -1.f);
	
	float div_value = sqrtf(powf(dv.x, 2.f) + powf(dv.y, 2.f));
	dv = ccpMult(dv, 1.f/div_value);
	
	dv = ccpMult(dv, t_speed);
	
	addChild(KSTimer::create(0.7f, [=]()
													 {
														 AudioEngine::sharedInstance()->playEffect("sound_throw_obj_shot.mp3", false);
														 schedule(schedule_selector(ThrowObject::myAction));
														 objImg->setVisible(true);
														 addChild(KSGradualValue<float>::create(255, 0, 0.3f, [=](float t)
																																		{
																																			KS::setOpacity(alreadyWarning.first, t);
																																			KS::setOpacity(alreadyWarning2.first, t);
																																			alreadyWarning2.first->setOpacity(t);
																																		}, nullptr));
													 }));
}

SatelliteBeam * SatelliteBeam::create (CCPoint t_sp, int t_type, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect)
{
	SatelliteBeam* t_sb = new SatelliteBeam();
	t_sb->myInit(t_sp, t_type, t_removeEffect, d_removeEffect);
	t_sb->autorelease();
	return t_sb;
}
void SatelliteBeam::beamSetPosition (CCPoint t_p)
{
	setPosition(t_p);
	
	{
		IntPoint beamPoint = IntPoint((t_p.x-1)/pixelSize+1, (t_p.y-1)/pixelSize+1); // center
		
		int crashSize = 1;
		crashMapForIntRect(IntRect(beamPoint.x - crashSize, beamPoint.y - crashSize, crashSize * 2.f + 1, crashSize * 2.f + 1));
	}
	
	//	for(int x=beamPoint.x-crashSize; x<=beamPoint.x+crashSize; x++)
	//	{
	//		for(int y=beamPoint.y-crashSize; y<=beamPoint.y+crashSize; y++)
	//		{
	//			crashMapForIntPoint(IntPoint(x, y));
	//
	//		}
	//	}
	
	//		for(int i=beamPoint.y-10;i<=beamPoint.y+10;i++)
	//		{
	//			int up_down_value;
	//			if(i == beamPoint.y-10 || i == beamPoint.y+10)			up_down_value = 6;
	//			else if(i == beamPoint.y-9 || i == beamPoint.y+9)		up_down_value = 11;
	//			else if(i == beamPoint.y-8 || i == beamPoint.y+8)		up_down_value = 13;
	//			else if(i == beamPoint.y-7 || i == beamPoint.y+7)		up_down_value = 15;
	//			else if(i == beamPoint.y-6 || i == beamPoint.y+6)		up_down_value = 17;
	//			else if(i == beamPoint.y-5 || i == beamPoint.y+5)		up_down_value = 18;
	//			else if(i == beamPoint.y-4 || i == beamPoint.y+4 || i == beamPoint.y-3 || i == beamPoint.y+3)		up_down_value = 19;
	//			else if(i >= beamPoint.y-2 && i <= beamPoint.y+2)		up_down_value = 20;
	//
	//			for(int j=beamPoint.x-up_down_value;j<=beamPoint.x+up_down_value;j++)
	//			{
	//				crashMapForIntPoint(IntPoint(j,i));
	//			}
	//		}
}
void SatelliteBeam::jackDie ()
{
	unschedule(schedule_selector(SatelliteBeam::fallingStar));
	(target_removeEffect->*delegate_removeEffect)();
}
void SatelliteBeam::lineDie (IntPoint t_p)
{
	unschedule(schedule_selector(SatelliteBeam::fallingStar));
	myGD->communication("Main_showLineDiePosition", t_p);
	(target_removeEffect->*delegate_removeEffect)();
}
void SatelliteBeam::startFallingStar ()
{
	AudioEngine::sharedInstance()->playEffect("sound_satellitebeam.mp3", false);
	schedule(schedule_selector(SatelliteBeam::fallingStar));
}
void SatelliteBeam::fallingStar ()
{
	if(rand()%2)
	{
		SB_FallStar* t_fs = SB_FallStar::create(type);
		addChild(t_fs);
		t_fs->startFalling();
	}
}
void SatelliteBeam::myInit (CCPoint t_sp, int t_type, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect)
{
	target_removeEffect = t_removeEffect;
	delegate_removeEffect = d_removeEffect;
	
	type = t_type;
	setPosition(t_sp);
	auto ret = KS::loadCCBI<CCSprite*>(this, "pattern_lightning.ccbi");
	KS::setBlendFunc(ret.first, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	beam_main = ret.first;
	//		beam_main->setAnchorPoint(ccp(0.5,0.036));
	addChild(beam_main);
	{
		auto ret2 = KS::loadCCBI<CCSprite*>(this, "pattern_lightning_targeting.ccbi");
		//		KS::setBlendFunc(ret2.first, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		beam_main->addChild(ret2.first, -1);
	}
	//		beam_main->setVisible(false);
	
	startFallingStar();
}
FM_Targeting * FM_Targeting::create (string imgFilename, CCPoint t_sp, int t_aniFrame, float t_sSize,
																		 float t_fSize, float t_sAngle, float t_fAngle, float inDegree, bool enable)
{
	FM_Targeting* t_fmt = new FM_Targeting();
	t_fmt->myInit(imgFilename, t_sp, t_aniFrame, t_sSize, t_fSize, t_sAngle, t_fAngle, inDegree, enable);
	t_fmt->autorelease();
	return t_fmt;
}
void FM_Targeting::startAction ()
{
	CCScaleTo* t_scaleAction = CCScaleTo::create(duration, fSize);
	
	CCRotateBy* t_rotateAction = CCRotateBy::create(duration, rotateValue);
	
	CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_scaleAction, t_rotateAction);
	
	targetingImg->runAction(t_spawn);
}
void FM_Targeting::myInit (string imgFilename, CCPoint t_sp, int t_aniFrame, float t_sSize, float t_fSize,
													 float t_sAngle, float t_fAngle, float inDegree, bool enable)
{
	duration = t_aniFrame/60.f;
	fSize = t_fSize;
	rotateValue = t_fAngle - t_sAngle;
	
	//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	//	CCBReader* reader = new CCBReader(nodeLoader);
	//	targetingImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("bomb_8_6_1.ccbi",this));//"pattern_meteor1_targeting.ccbi",this));
	//	reader->release();
	if(enable)
	{
		targetingImg = KS::loadCCBI<CCSprite*>(this, "bomb_8_6_1.ccbi").first;
		CCSprite* guideLine = KS::loadCCBI<CCSprite*>(this, "target4.ccbi").first;
		guideLine->setPosition(t_sp);
		guideLine->setRotation(-inDegree);
		addChild(guideLine);
	}
	else
	{
		targetingImg = KS::loadCCBI<CCSprite*>(this, "bomb_8_6_1_2.ccbi").first;
		CCSprite* guideLine = KS::loadCCBI<CCSprite*>(this, "target4_1.ccbi").first;
		guideLine->setPosition(t_sp);
		guideLine->setRotation(-inDegree);
		addChild(guideLine);
	}
	targetingImg->setPosition(t_sp);
	
	targetingImg->setScale(1.f);
	targetingImg->setRotation(t_sAngle);
	
	addChild(targetingImg);
	
	
	AudioEngine::sharedInstance()->playEffect("se_meteo_1.mp3");
}
FallMeteor * FallMeteor::create (string t_imgFilename, int imgFrameCnt, CCSize imgFrameSize, CCPoint t_sp, CCPoint t_fp, int t_fallFrame, int t_explosionFrame,
																 IntSize t_mSize, float enableP, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect,
																 std::function<void(int)> accum)
{
	FallMeteor* t_fm = new FallMeteor();
	t_fm->myInit(t_imgFilename, imgFrameCnt, imgFrameSize, t_sp, t_fp, t_fallFrame,
							 t_explosionFrame, t_mSize, enableP, t_removeEffect, d_removeEffect, accum);
	t_fm->autorelease();
	return t_fm;
}
void FallMeteor::removeEffect ()
{
	fadeFromToDuration.init(255, 0, 0.3f);
	schedule(schedule_selector(FallMeteor::hidingAnimation));
}
void FallMeteor::hidingAnimation (float dt)
{
	if(fadeFromToDuration.step(1.f/60.f) == false)
	{
		selfRemove();
	}
	else
	{
		//		KS::setColor(this, ccc3(fadeFromToDuration.getValue(), fadeFromToDuration.getValue(), 255));
		KS::setOpacity(this, fadeFromToDuration.getValue());
	}
}
void FallMeteor::jackDie ()
{
	AudioEngine::sharedInstance()->playEffect("sound_meteor.mp3", false);
	stopFall();
}
void FallMeteor::lineDie (IntPoint t_p)
{
	myGD->communication("Main_showLineDiePosition", t_p);
	//		unschedule(schedule_selector(FallMeteor::fall));
	AudioEngine::sharedInstance()->playEffect("sound_meteor.mp3", false);
	stopFall();
}
void FallMeteor::finalCrash ()
{
	IntPoint mapPoint = IntPoint(round((fp.x-1)/pixelSize+1),round((fp.y-1)/pixelSize+1));
	
	for(int i=-20;i<=20;i++)
	{
		int addValue;
		if(i == -20 || i == 20)									addValue = 4;
		else if(i == -19 || i == 19)							addValue = 8;
		else if(i == -18 || i == 18)							addValue = 9;
		else if(i == -17 || i == 17)							addValue = 11;
		else if(i == -16 || i == 16)							addValue = 12;
		else if(i == -15 || i == 15)							addValue = 14;
		else if(i == -14 || i == 14 || i == -13 || i == 13)		addValue = 15;
		else if(i == -12 || i == 12)							addValue = 16;
		else if(i == -11 || i == 11 || i == -10 || i == 10)		addValue = 17;
		else if(i == -9 || i == 9)								addValue = 18;
		else if((i >= -8 && i <= -5) || (i >= 5 && i <= 8))		addValue = 19;
		else													addValue = 20;
		for(int j=-addValue;j<=addValue;j++)
		{
			crashMapForIntPoint(IntPoint(mapPoint.x+i, mapPoint.y+j));
		}
	}
}
void FallMeteor::stopFall ()
{
	unschedule(schedule_selector(FallMeteor::fall));
	initParticle();
	if(meteor)
		meteor->removeFromParentAndCleanup(true);
	meteor = nullptr;
	//finalCrash();
	
	CCDelayTime* t_delay = CCDelayTime::create(0.5);
	CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(FallMeteor::selfRemove));
	
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call2);
	runAction(t_seq);
	
	accumer(crashCount);
}
void FallMeteor::fall ()
{
	ingFrame++;
	bool notFinish = meteorChanger.step(1/60.f);
	
	//		meteor->setPosition(ccpAdd(meteor->getPosition(),fall_dv));
	meteor->setPosition(meteorChanger.getValue());
	
	IntPoint rightUpPoint = IntPoint((meteor->getPositionX()-1)/pixelSize+1,(meteor->getPositionY()-1)/pixelSize+1); // right up
	IntPoint leftDownPoint = IntPoint(rightUpPoint.x-mSize.width,rightUpPoint.y-mSize.height);		// left down point
	
	IntSize size = IntSize(rightUpPoint.x - leftDownPoint.x + 1, rightUpPoint.y - leftDownPoint.y + 1); // size
	
	crashMapForIntRect(IntRect(leftDownPoint.x, leftDownPoint.y, size.width, size.height));
	
	//	for(int i = leftDownPoint.x;i<leftDownPoint.x+size.width;i++)
	//	{
	//		for(int j = leftDownPoint.y;j<leftDownPoint.y+size.height;j++)
	//		{
	//			crashMapForIntPoint(IntPoint(i, j));
	//		}
	//	}
	if(!notFinish)
	{
		AudioEngine::sharedInstance()->playEffect("se_meteo_3.mp3");
		//		AudioEngine::sharedInstance()->playEffect("sound_meteor.mp3", false);
		for(int i=0;i<11;i++)
		{
			crashMapForIntPoint(IntPoint(leftDownPoint.x-1, leftDownPoint.y+2+i));
			crashMapForIntPoint(IntPoint(leftDownPoint.x-2, leftDownPoint.y+2+i));
		}
		for(int i=0;i<7;i++)
		{
			crashMapForIntPoint(IntPoint(leftDownPoint.x-3, leftDownPoint.y+4+i));
			crashMapForIntPoint(IntPoint(leftDownPoint.x-4, leftDownPoint.y+4+i));
		}
		
		for(int i=0;i<11;i++)
		{
			crashMapForIntPoint(IntPoint(leftDownPoint.x+2+i, leftDownPoint.y-1));
			crashMapForIntPoint(IntPoint(leftDownPoint.x+2+i, leftDownPoint.y-2));
		}
		for(int i=0;i<7;i++)
		{
			crashMapForIntPoint(IntPoint(leftDownPoint.x+4+i, leftDownPoint.y-3));
			crashMapForIntPoint(IntPoint(leftDownPoint.x+4+i, leftDownPoint.y-4));
		}
		
		stopFall();
	}
	
	//		if(ingFrame >= fallFrame)
	//		{
	//			for(int i=0;i<11;i++)
	//			{
	//				crashMapForIntPoint(IntPoint(leftDownPoint.x-1, leftDownPoint.y+2+i));
	//			}
	//			for(int i=0;i<7;i++)
	//			{
	//				crashMapForIntPoint(IntPoint(leftDownPoint.x-2, leftDownPoint.y+4+i));
	//			}
	//			crashMapForIntPoint(IntPoint(leftDownPoint.x-3, leftDownPoint.y+7));
	//
	//			for(int i=0;i<11;i++)
	//			{
	//				crashMapForIntPoint(IntPoint(leftDownPoint.x+2+i, leftDownPoint.y-1));
	//			}
	//			for(int i=0;i<7;i++)
	//			{
	//				crashMapForIntPoint(IntPoint(leftDownPoint.x+4+i, leftDownPoint.y-2));
	//			}
	//			crashMapForIntPoint(IntPoint(leftDownPoint.x+7, leftDownPoint.y-3));
	//
	//			stopFall();
	//		}
}
void FallMeteor::selfRemove ()
{
	removeFromParentAndCleanup(true);
	myGD->communication("MS_resetRects", false);
}
void FallMeteor::initParticle ()
{
	auto ret = KS::loadCCBI<CCSprite*>(this, "bomb_8_4.ccbi");//"fx_bomb5.ccbi");
	KS::setBlendFunc(ret.first, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	CCSprite* particle = ret.first;
	
	if(meteor)
		particle->setPosition(meteor->getPosition());
	//		particle->setPosVar(CCPointZero);
	addChild(particle);
}
void FallMeteor::myInit (string t_imgFilename, int imgFrameCnt, CCSize imgFrameSize, CCPoint t_sp,
												 CCPoint t_fp, int t_fallFrame, int t_explosionFrame, IntSize t_mSize,
												 float enableP,
												 CCObject * t_removeEffect, SEL_CallFunc d_removeEffect, std::function<void(int)> accum)
{
	crashCount = 0;
	enableProb = enableP;
	accumer = accum;
	target_removeEffect = t_removeEffect;
	delegate_removeEffect = d_removeEffect;
	ProbSelector ps{enableP, 1.f - enableP};
	enable = (ps.getResult() == 0);
	
	imgFilename = t_imgFilename;
	fp = t_fp;
	fallFrame = t_fallFrame;
	explosionFrame = t_explosionFrame;
	mSize = t_mSize;
	
	meteorChanger.init(t_sp, fp, 1.f);//0.75f);
	//		fall_dv = ccpSub(fp, t_sp);
	////		fall_dv = (fall_dv / ccpLength(fall_dv)) * 10.f;
	//		fall_dv = ccpMult(fall_dv, 1.f/fallFrame);
	
	//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	//	CCBReader* reader = new CCBReader(nodeLoader);
	//	meteor = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("pattern_meteor3.ccbi",this));
	meteor = KS::loadCCBI<CCSprite*>(this, "pattern_meteor3.ccbi").first;
	meteor->setRotation(90 + 45);
	//meteor->setScale(2.5f);
	//	reader->release();
	
	
	//		 = CCSprite::create(("meteor_stone_test" + imgFilename).c_str(), CCRectMake(0, 0, imgFrameSize.width, imgFrameSize.height));
	meteor->setPosition(t_sp);
	addChild(meteor, 1);
	CCParticleSystemQuad* meteorBack = KS::loadCCBI<CCParticleSystemQuad*>(this, "pattern_meteor3_back.ccbi").first;
	meteorBack->setPositionType(kCCPositionTypeRelative);
	meteor->addChild(meteorBack, 0);
	
	int random_sign;
	if(rand()%2)
		random_sign = 360;
	else
		random_sign = -360;
	m_targetSprite = FM_Targeting::create(imgFilename, t_fp, t_fallFrame, 3.0, 0.7, 0,
																				random_sign, rad2Deg(atan2f(t_sp.y - t_fp.y, t_sp.x - t_fp.x)), enable);
	
	
	addChild(m_targetSprite);
	
	//m_targetSprite->startAction();
	
	if(enable)
	{
		ingFrame = 0;
		addChild(KSGradualValue<float>::create(255, 0, fallFrame / 60.f, [=](int t)
																					 {
																						 KS::setOpacity(m_targetSprite, MIN(255, t + 150) );
																						 //					KS::setColor(m_targetSprite, ccc3(t, t, 255));
																					 },
																					 [=](float t)
																					 {
																						 AudioEngine::sharedInstance()->playEffect("se_meteo_2.mp3");
																						 schedule(schedule_selector(FallMeteor::fall));
																					 }));
		
	}
	else
	{
		addChild(KSGradualValue<float>::create(255, 0, fallFrame / 60.f, [=](int t)
																					 {
																						 KS::setOpacity(m_targetSprite, MIN(255, t + 150) );
																						 //					KS::setColor(m_targetSprite, ccc3(t, t, 255));
																					 },
																					 [=](float t)
																					 {
																						 this->removeFromParent();
																					 }));
		
		
	}
}

Lazer_Ring * Lazer_Ring::create (float t_ring_angle, CCPoint t_ring_sP, CCPoint t_ring_fP, float t_ring_sS, float t_ring_fS, int t_frame, ccColor3B t_color)
{
	Lazer_Ring* t_lr = new Lazer_Ring();
	t_lr->myInit(t_ring_angle, t_ring_sP, t_ring_fP, t_ring_sS, t_ring_fS, t_frame, t_color);
	t_lr->autorelease();
	return t_lr;
}
void Lazer_Ring::startMyAction ()
{
	ingFrame = 0;
	schedule(schedule_selector(Lazer_Ring::myAction));
}
void Lazer_Ring::stopMyAction ()
{
	unschedule(schedule_selector(Lazer_Ring::myAction));
	removeFromParentAndCleanup(true);
}
void Lazer_Ring::myAction ()
{
	ingFrame++;
	
	ringImg->setPosition(ccpAdd(ringImg->getPosition(), dv));
	ringImg->setScale(ringImg->getScale()+ds);
	
	if(ingFrame >= frame)
	{
		stopMyAction();
	}
}
void Lazer_Ring::myInit (float t_ring_angle, CCPoint t_ring_sP, CCPoint t_ring_fP, float t_ring_sS, float t_ring_fS, int t_frame, ccColor3B t_color)
{
	frame = t_frame;
	ringImg = CCSprite::create("lazer_ring.png");
	ringImg->setPosition(t_ring_sP);
	ringImg->setScale(t_ring_sS);
	ringImg->setRotation(-t_ring_angle);
	
	ringImg->setOpacity(rand()%100+100);
	ringImg->setColor(t_color);
	
	addChild(ringImg);
	
	dv = ccpSub(t_ring_fP, t_ring_sP);
	dv = ccpMult(dv, 1.f/frame);
	
	ds = (t_ring_fS - t_ring_sS)/frame;
	
	startMyAction();
}
ThreeCushion * ThreeCushion::create (CCPoint t_sp, float t_speed, int t_cushion_cnt, bool t_is_big_bomb, int t_crashArea, float enableProb, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect)
{
	ThreeCushion* t_tc = new ThreeCushion();
	t_tc->myInit(t_sp, t_speed, t_cushion_cnt, t_is_big_bomb, t_crashArea, enableProb, t_removeEffect, d_removeEffect);
	t_tc->autorelease();
	return t_tc;
}
void ThreeCushion::removeEffect ()
{
	unschedule(schedule_selector(ThreeCushion::myAction));
	CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(ThreeCushion::selfRemove));
	
	if(baseNode)
	{
		baseNode->stopAllActions();
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade1, t_call2);
		baseNode->runAction(t_seq);
	}
	
	//		if(colorControl)
	//		{
	//			CCFadeTo* t_fade2 = CCFadeTo::create(1.f, 0);
	//			colorControl->runAction(t_fade2);
	//		}
	
	if(cntLabel)
	{
		CCFadeTo* t_fade3 = CCFadeTo::create(1.f, 0);
		cntLabel->runAction(t_fade3);
	}
}
void ThreeCushion::startMyAction ()
{
	AudioEngine::sharedInstance()->playEffect("sound_throw_obj_shot.mp3", false);
	schedule(schedule_selector(ThreeCushion::myAction));
}
void ThreeCushion::stopMyAction ()
{
	AudioEngine::sharedInstance()->playEffect("sound_threecusion_bomb.mp3", false);
	unschedule(schedule_selector(ThreeCushion::myAction));
	initParticle();
	
	ProbSelector ps = {enableProb, 1.f - enableProb};
	if(ps.getResult() == 0)
	{
		crashMap();
	}
	baseNode->removeFromParentAndCleanup(true);
	baseNode = NULL;
	//		colorControl = NULL;
	cntLabel = NULL;
	
	CCDelayTime* t_delay = CCDelayTime::create(0.5);
	CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(ThreeCushion::selfRemove));
	
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call2);
	runAction(t_seq);
}
void ThreeCushion::myAction ()
{
	int side_cnt = 0;
	IntPoint check_point;
	IntVector t_v;
	
	IntPoint myPoint = IntPoint(round((baseNode->getPositionX()-1)/pixelSize+1),round((baseNode->getPositionY()-1)/pixelSize+1));
	
	t_v = IntVector::directionVector(directionLeft);
	check_point = IntPoint(myPoint.x + t_v.dx , myPoint.y + t_v.dy);
	if(!check_point.isInnerMap() || (myGD->mapState[check_point.x][check_point.y] == mapOldget || myGD->mapState[check_point.x][check_point.y] == mapOldline))		side_cnt++;
	
	t_v = IntVector::directionVector(directionRight);
	check_point = IntPoint(myPoint.x + t_v.dx , myPoint.y + t_v.dy);
	if(!check_point.isInnerMap() || (myGD->mapState[check_point.x][check_point.y] == mapOldget || myGD->mapState[check_point.x][check_point.y] == mapOldline))		side_cnt++;
	
	t_v = IntVector::directionVector(directionDown);
	check_point = IntPoint(myPoint.x + t_v.dx , myPoint.y + t_v.dy);
	if(!check_point.isInnerMap() || (myGD->mapState[check_point.x][check_point.y] == mapOldget || myGD->mapState[check_point.x][check_point.y] == mapOldline))		side_cnt++;
	
	t_v = IntVector::directionVector(directionUp);
	check_point = IntPoint(myPoint.x + t_v.dx , myPoint.y + t_v.dy);
	if(!check_point.isInnerMap() || (myGD->mapState[check_point.x][check_point.y] == mapOldget || myGD->mapState[check_point.x][check_point.y] == mapOldline))		side_cnt++;
	
	if(side_cnt == 4)
	{
		unschedule(schedule_selector(ThreeCushion::myAction));
		AudioEngine::sharedInstance()->playEffect("sound_jack_basic_missile_shoot.mp3", false);
		
		int rmCnt = 3;
		
		string missile_code;
		missile_code = NSDS_GS(kSDS_CI_int1_missile_type_s, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
		int missile_type = MissileDamageData::getMissileType(missile_code.c_str());
		
		//				myGD->communication("Main_goldGettingEffect", jackPosition, int((t_p - t_beforePercentage)/JM_CONDITION*myDSH->getGoldGetRate()));
		float missile_speed = NSDS_GD(kSDS_CI_int1_missile_speed_d, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
		
		myGD->communication("MP_createJackMissile", missile_type, rmCnt, missile_speed, getPosition());
		
		removeFromParentAndCleanup(true);
		return;
	}
	
	CCPoint after_position = getAfterPosition(baseNode->getPosition(), angle);
	IntPoint after_point = IntPoint(round((after_position.x-1)/pixelSize+1),round((after_position.y-1)/pixelSize+1));
	if(!after_point.isInnerMap() || myGD->mapState[after_point.x][after_point.y] != mapEmpty)
	{
		cushionCnt--;
		if(cushionCnt <= 0)
		{
			stopMyAction(); // bomb
			return;
		}
		//			colorControl->setColor(getCushionColor(cushionCnt));
		cntLabel->setString(CCString::createWithFormat("%d", cushionCnt)->getCString());
		
		after_position = judgeAnglePoint(baseNode->getPosition());
		if(after_position.x == -1 && after_position.y == -1)
		{
			return;
		}
	}
	
	baseNode->setPosition(after_position);
}
void ThreeCushion::selfRemove ()
{
	removeFromParentAndCleanup(true);
	myGD->communication("MS_resetRects", false);
}
void ThreeCushion::jackDie ()
{
	(target_removeEffect->*delegate_removeEffect)();
}
void ThreeCushion::lineDie (IntPoint t_p)
{
	myGD->communication("Main_showLineDiePosition", t_p);
	(target_removeEffect->*delegate_removeEffect)();
}
void ThreeCushion::crashMap ()
{
	IntPoint centerPoint = IntPoint(round((baseNode->getPositionX()-1)/pixelSize+1),round((baseNode->getPositionY()-1)/pixelSize+1));
	
	float radius = crashArea / 2.f;
	for(int y=-radius; y<=radius; y++)
	{
		for(int x=-radius; x<=radius; x++)
		{
			float distance = (IntPoint(x, y)).length();
			if(distance <= radius)
			{
				crashMapForIntPoint(centerPoint + IntPoint(x, y));
			}
		}
	}
	//	if(!is_big_bomb)
	//	{
	//		for(int i=-8;i<=8;i++)
	//		{
	//			int addValue;
	//			if(i == -8 || i == 8)									addValue = 3;
	//			else if(i == -7 || i == 7)								addValue = 5;
	//			else if(i == -6 || i == 6)								addValue = 6;
	//			else if(i == -5 || i == 5 || i == -4 || i == 4)			addValue = 7;
	//			else													addValue = 8;
	//			for(int j=-addValue;j<=addValue;j++)
	//			{
	//				crashMapForIntPoint(IntPoint(centerPoint.x+i, centerPoint.y+j));
	//			}
	//		}
	//	}
	//	else
	//	{
	//		for(int i=-14;i<=14;i++)
	//		{
	//			int addValue;
	//			if(i == -14 || i == 14)									addValue = 3;
	//			else if(i == -13 || i == 13)							addValue = 6;
	//			else if(i == -12 || i == 12)							addValue = 8;
	//			else if(i == -11 || i == 11)							addValue = 9;
	//			else if(i == -10 || i == 10)							addValue = 10;
	//			else if(i == -9 || i == 9)								addValue = 11;
	//			else if(i == -8 || i == 8 || i == -7 || i == 7)			addValue = 12;
	//			else if((i >= -6 && i <= -4) || (i >= 4 && i <= 6))		addValue = 13;
	//			else													addValue = 14;
	//			for(int j=-addValue;j<=addValue;j++)
	//			{
	//				crashMapForIntPoint(IntPoint(centerPoint.x+i, centerPoint.y+j));
	//			}
	//		}
	//	}
}
CCPoint ThreeCushion::getAfterPosition (CCPoint b_p, int t_angle)
{
	CCPoint dv;
	dv.x = 1;
	dv.y = tanf(t_angle/180.f*M_PI);
	
	if((t_angle >= 90 && t_angle < 270) || (t_angle < -90 && t_angle >= -270))
	{
		dv = ccpMult(dv, -1.f);
	}
	
	float div_value = sqrtf(powf(dv.x, 2.f) + powf(dv.y, 2.f));
	
	dv = ccpMult(dv, speed/div_value);
	
	CCPoint after_position = ccpAdd(b_p, dv);
	return after_position;
}
CCPoint ThreeCushion::judgeAnglePoint (CCPoint b_p)
{
	CCPoint returnPoint;
	int a_angle;
	
	if((angle >= -45 && angle < 0) || (angle >= 0 && angle < 45) || (angle >= 135 && angle < 180) || (angle >= -180 && angle < -135))
	{
		// 180 - x, -x, reverse
		a_angle = 180 - angle;
		if(a_angle < -180)		a_angle += 360;
		if(a_angle >= 180)		a_angle -= 360;
		
		CCPoint after_position = getAfterPosition(b_p, a_angle);
		IntPoint after_point = IntPoint(round((after_position.x-1)/pixelSize+1),round((after_position.y-1)/pixelSize+1));
		if(!after_point.isInnerMap() || myGD->mapState[after_point.x][after_point.y] != mapEmpty)
		{
			a_angle = -angle;
			if(a_angle < -180)		a_angle += 360;
			if(a_angle >= 180)		a_angle -= 360;
		}
		else
		{
			angle = a_angle;
			return after_position;
		}
		
		after_position = getAfterPosition(b_p, a_angle);
		after_point = IntPoint(round((after_position.x-1)/pixelSize+1),round((after_position.y-1)/pixelSize+1));
		if(!after_point.isInnerMap() || myGD->mapState[after_point.x][after_point.y] != mapEmpty)
		{
			a_angle = angle + 180;
			if(a_angle < -180)		a_angle += 360;
			if(a_angle >= 180)		a_angle -= 360;
		}
		else
		{
			angle = a_angle;
			return after_position;
		}
		
		after_position = getAfterPosition(b_p, a_angle);
		angle = a_angle;
		return after_position;
	}
	else if((angle >= 45 && angle < 90) || (angle >= 90 && angle < 135) || (angle >= -135 && angle < -90) || (angle >= -90 && angle < -45))
	{
		// -x, 180 - x, reverse
		a_angle = -angle;
		if(a_angle < -180)		a_angle += 360;
		if(a_angle >= 180)		a_angle -= 360;
		
		CCPoint after_position = getAfterPosition(b_p, a_angle);
		IntPoint after_point = IntPoint(round((after_position.x-1)/pixelSize+1),round((after_position.y-1)/pixelSize+1));
		if(!after_point.isInnerMap() || myGD->mapState[after_point.x][after_point.y] != mapEmpty)
		{
			a_angle = 180 - angle;
			if(a_angle < -180)		a_angle += 360;
			if(a_angle >= 180)		a_angle -= 360;
		}
		else
		{
			angle = a_angle;
			return after_position;
		}
		
		after_position = getAfterPosition(b_p, a_angle);
		after_point = IntPoint(round((after_position.x-1)/pixelSize+1),round((after_position.y-1)/pixelSize+1));
		if(!after_point.isInnerMap() || myGD->mapState[after_point.x][after_point.y] != mapEmpty)
		{
			a_angle = angle + 180;
			if(a_angle < -180)		a_angle += 360;
			if(a_angle >= 180)		a_angle -= 360;
		}
		else
		{
			angle = a_angle;
			return after_position;
		}
		
		after_position = getAfterPosition(b_p, a_angle);
		angle = a_angle;
		return after_position;
	}
	else
	{
		// ?!
	}
	stopMyAction();
	return ccp(-1,-1);
}
void ThreeCushion::myInit (CCPoint t_sp, float t_speed, int t_cushion_cnt, bool t_is_big_bomb, int t_crashArea, float enableP, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect)
{
	target_removeEffect = t_removeEffect;
	delegate_removeEffect = d_removeEffect;
	crashArea = t_crashArea;
	enableProb = enableP;
	cushionCnt = t_cushion_cnt;
	speed = t_speed;
	angle = 0;
	is_big_bomb = t_is_big_bomb;
	while (angle%45 == 0)
	{
		angle = rand()%360 - 180;
	}
	
	//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	//	CCBReader* reader = new CCBReader(nodeLoader);
	//	baseNode = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("pattern_bumbball.ccbi",this));
	baseNode = KS::loadCCBI<CCSprite*>(this, "pattern_bumbball.ccbi").first;
	//		baseNode = CCSprite::create("threeCushion_main.png");
	baseNode->setPosition(t_sp);
	addChild(baseNode);
	//	reader->release();
	
	//		colorControl = CCSprite::create("threeCushion_color.png");
	//		colorControl->setPosition(ccp(12,12));
	//		colorControl->setColor(getCushionColor(cushionCnt));
	//		baseNode->addChild(colorControl);
	
	cntLabel = CCLabelAtlas::create(CCString::createWithFormat("%d", cushionCnt)->getCString(), "threeCushion_number.png", 13, 13, '1');
	cntLabel->setAnchorPoint(ccp(0.5,0.5));
	cntLabel->setPosition(ccp(baseNode->getContentSize().width/2.f,baseNode->getContentSize().height/2.f));
	baseNode->addChild(cntLabel);
	
	startMyAction();
}
void ThreeCushion::initParticle ()
{
	//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	//	CCBReader* reader = new CCBReader(nodeLoader);
	//	CCSprite* particle = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("bomb_8_8.ccbi",this));
	CCSprite* particle = KS::loadCCBI<CCSprite*>(this, "bomb_8_8.ccbi").first;
	KS::setBlendFunc(particle, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	//	reader->release();
	particle->setPosition(baseNode->getPosition());
	particle->setRotation(rand()%360);
	addChild(particle);
	
	//		CCParticleSystemQuad* particle = CCParticleSystemQuad::createWithTotalParticles(50);
	//		particle->setPositionType(kCCPositionTypeRelative);
	//		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("threeCushion_bomb.png");
	//		particle->setTexture(texture);
	//		particle->setEmissionRate(250.00);
	//		particle->setAngle(90.0);
	//		particle->setAngleVar(360.0);
	//		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
	//		particle->setBlendFunc(blendFunc);
	//		particle->setDuration(0.20);
	//		particle->setEmitterMode(kCCParticleModeGravity);
	//		ccColor4F startColor = {0.87,0.81,0.12,1.00}; // 0.76 0.25 0.12
	//		particle->setStartColor(startColor);
	//		ccColor4F startColorVar = {0,0,0,0};
	//		particle->setStartColorVar(startColorVar);
	//		ccColor4F endColor = {0.68,0.16,0.00,1.00};
	//		particle->setEndColor(endColor);
	//		ccColor4F endColorVar = {0,0,0,0};
	//		particle->setEndColorVar(endColorVar);
	//		particle->setStartSize(20.00);
	//		particle->setStartSizeVar(10.0);
	//		particle->setEndSize(40.0);
	//		particle->setEndSizeVar(10.0);
	//		particle->setGravity(ccp(0,0));
	//		particle->setRadialAccel(0.0);
	//		particle->setRadialAccelVar(0.0);
	//		particle->setSpeed(170);
	//		particle->setSpeedVar(60.0);
	//		particle->setTangentialAccel(0);
	//		particle->setTangentialAccelVar(0);
	//		particle->setTotalParticles(50);
	//		particle->setLife(0.20);
	//		particle->setLifeVar(0.0);
	//		particle->setStartSpin(0.0);
	//		particle->setStartSpinVar(0.0);
	//		particle->setEndSpin(0.0);
	//		particle->setEndSpinVar(0.0);
	//		particle->setPosition(baseNode->getPosition());
	//		particle->setPosVar(CCPointZero);
	//		addChild(particle);
}
TickingTimeBomb * TickingTimeBomb::create (IntPoint t_setPoint, int t_bombFrameOneTime, int t_bombTimes,
																					 int t_crashArea, int t_rangeCode, CCArray * t_tickingArray, CCObject * t_resetTickingTimeBomb, SEL_CallFunc d_resetTickingTimeBomb)
{
	TickingTimeBomb* t_ttb = new TickingTimeBomb();
	t_ttb->myInit(t_setPoint, t_bombFrameOneTime, t_bombTimes, t_crashArea, t_rangeCode, t_tickingArray, t_resetTickingTimeBomb, d_resetTickingTimeBomb);
	t_ttb->autorelease();
	return t_ttb;
}
IntPoint TickingTimeBomb::getSettedPoint ()
{
	return setPoint;
}
void TickingTimeBomb::startMyAction ()
{
	ingFrame = 0;
	schedule(schedule_selector(TickingTimeBomb::myAction));
}
void TickingTimeBomb::myAction ()
{
	ingFrame++;
	
	if(ingFrame == bombFrameOneTime)
	{
		ingFrame = 0;
		bombTimes--;
		ticking_label->setString(CCString::createWithFormat("%d",bombTimes)->getCString());
	}
	
	int surroundCnt = 0;
	IntPoint checkPoint = IntPoint(setPoint.x-1, setPoint.y);
	if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
	checkPoint = IntPoint(setPoint.x+1, setPoint.y);
	if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
	checkPoint = IntPoint(setPoint.x, setPoint.y-1);
	if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
	checkPoint = IntPoint(setPoint.x, setPoint.y+1);
	if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
	
	if(surroundCnt == 0)
	{
		stopMyAction();
		selfRemove();
		return;
	}
	
	if(ingFrame == bombFrameOneTime-1 && bombTimes <= 1)
	{
		crashMap();
		ticking_main->removeFromParentAndCleanup(true);
		
		CCDelayTime* t_delay = CCDelayTime::create(1.2f);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(TickingTimeBomb::selfRemove));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call2);
		runAction(t_seq);
		stopMyAction();
	}
}
void TickingTimeBomb::crashMap ()
{
	initParticle();
	AudioEngine::sharedInstance()->playEffect("sound_tickingTimeBomb.mp3", false);
	float radius = crashArea / 2.f;
	for(int y=-radius; y<=radius; y++)
	{
		for(int x=-radius; x<=radius; x++)
		{
			float distance = (IntPoint(x, y)).length();
			if(distance <= radius)
			{
				crashMapForIntPoint(setPoint + IntPoint(x, y));
			}
		}
	}
	
	//	if(rangeCode == 1)
	//	{
	//		for(int i=-40;i<=40;i++)
	//		{
	//			int addValue;
	//			if(i == -40 || i == -39 || i == 40 || i == 39)			addValue = 8;
	//			else if(i == -38 || i == -37 || i == 38 || i == 37)		addValue = 16;
	//			else if(i == -36 || i == -35 || i == 36 || i == 35)		addValue = 19;
	//			else if(i == -34 || i == -33 || i == 34 || i == 33)		addValue = 22;
	//			else if(i == -32 || i == -31 || i == 32 || i == 31)		addValue = 24;
	//			else if(i == -30 || i == -29 || i == 30 || i == 29)		addValue = 28;
	//			else if((i >= -28 && i <= -25) || (i <= 28 && i >= 25))	addValue = 30;
	//			else if(i == -24 || i == -23 || i == 24 || i == 23)		addValue = 32;
	//			else if((i >= -22 && i <= -19) || (i <= 22 && i >= 19))	addValue = 34;
	//			else if(i == -18 || i == -17 || i == 18 || i == 17)		addValue = 36;
	//			else if((i >= -16 && i <= -9) || (i >= 9 && i <= 16))	addValue = 38;
	//			else													addValue = 40;
	//			for(int j=-addValue;j<=addValue;j++)
	//			{
	//				crashMapForIntPoint(IntPoint(setPoint.x+i, setPoint.y+j));
	//			}
	//		}
	//	}
	//	else if(rangeCode == 2)
	//	{
	//		myGD->communication("SW_createJDSW", setPoint);
	//	}
}
void TickingTimeBomb::selfRemove ()
{
	tickingArray->removeObject(this);
	removeFromParentAndCleanup(true);
	myGD->communication("MS_resetRects", false);
}
void TickingTimeBomb::stopMyAction ()
{
	unschedule(schedule_selector(TickingTimeBomb::myAction));
}
void TickingTimeBomb::jackDie ()
{
	unschedule(schedule_selector(TickingTimeBomb::myAction));
	(target_resetTickingTimeBomb->*delegate_resetTickingTimeBomb)();
}
void TickingTimeBomb::lineDie (IntPoint t_p)
{
	unschedule(schedule_selector(TickingTimeBomb::myAction));
	myGD->communication("Main_showLineDiePosition", t_p);
	(target_resetTickingTimeBomb->*delegate_resetTickingTimeBomb)();
}
void TickingTimeBomb::myInit (IntPoint t_setPoint, int t_bombFrameOneTime, int t_bombTimes, int t_crashArea, int t_rangeCode, CCArray * t_tickingArray, CCObject * t_resetTickingTimeBomb, SEL_CallFunc d_resetTickingTimeBomb)
{
	target_resetTickingTimeBomb = t_resetTickingTimeBomb;
	delegate_resetTickingTimeBomb = d_resetTickingTimeBomb;
	
	tickingArray = t_tickingArray;
	setPoint = t_setPoint;
	bombFrameOneTime = t_bombFrameOneTime;
	bombTimes = t_bombTimes;
	rangeCode = t_rangeCode;
	crashArea = t_crashArea;
	
	CCPoint myPosition = ccp((setPoint.x-1)*pixelSize+1, (setPoint.y-1)*pixelSize+1);
	setPosition(myPosition);
	
	ticking_main = KS::loadCCBI<CCSprite*>(this, "pattern_timebomb1.ccbi").first;
	ticking_main->setScale(0.50f);
	//		ticking_main->setVisible(false);
	ticking_main->setAnchorPoint(ccp(0.5f, 0.5f));
	addChild(ticking_main);
	
	if(rangeCode == 1)
	{
		//			CCSprite* ticking_pad = CCSprite::create("tickingTimeBomb_pad.png");
		////			ticking_pad->setPosition(ccp(17,12));
		//			ticking_main->addChild(ticking_pad);
		
		ticking_label = CCLabelAtlas::create(CCString::createWithFormat("%d",bombTimes)->getCString(), "threeCushion_number.png", 13, 13, '1');
		ticking_label->setAnchorPoint(ccp(0.5f, 0.5f));
		ticking_label->setPosition(ccp(0, 0));
		ticking_label->setScale(0.7f);
		addChild(ticking_label);
	}
	else if(rangeCode == 2)
	{
		ticking_label = CCLabelAtlas::create(CCString::createWithFormat("%d",bombTimes)->getCString(), "threeCushion_number.png", 13, 13, '1');
		ticking_label->setPosition(ccp(13,13));
		ticking_main->addChild(ticking_label);
		ticking_label->setScale(0.5f);
	}
	
	startMyAction();
}
void TickingTimeBomb::initParticle ()
{
	auto bomb = KS::loadCCBI<CCSprite*>(this, "bomb_8_8.ccbi");
	KS::setBlendFunc(bomb.first, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	bomb.first->setPosition(CCPointZero);
	addChild(bomb.first);
	//	addChild(KSTimer::create(2.0f, [=](){
	//		bomb.first->removeFromParent();})
	//					 ); // 1.3 초 후에 사라짐.
}
BurnFragment * BurnFragment::create (IntPoint t_sp, int t_life, CCNode * t_parent, int t_mType, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect)
{
	BurnFragment* t_bf = new BurnFragment();
	t_bf->myInit(t_sp, t_life, t_parent, t_mType, t_removeEffect, d_removeEffect);
	t_bf->autorelease();
	return t_bf;
}
void BurnFragment::jackDie ()
{
	unschedule(schedule_selector(BurnFragment::myAction));
	CCLOG("call %x", target_removeEffect);
	(target_removeEffect->*delegate_removeEffect)();
	removeFromParentAndCleanup(true);
	CCLOG("%x dest", this);
}
void BurnFragment::lineDie (IntPoint t_p)
{
	unschedule(schedule_selector(BurnFragment::myAction));
	myGD->communication("Main_showLineDiePosition", t_p);
	(target_removeEffect->*delegate_removeEffect)();
	removeFromParentAndCleanup(true);
}
void BurnFragment::startMyAction ()
{
	ingFrame = 0;
	schedule(schedule_selector(BurnFragment::myAction));
}
void BurnFragment::myAction ()
{
	ingFrame++;
	
	int emptyCnt = 0;
	if(myGD->mapState[myPoint.x-1][myPoint.y] == mapEmpty)	emptyCnt++;
	if(myGD->mapState[myPoint.x+1][myPoint.y] == mapEmpty)	emptyCnt++;
	if(myGD->mapState[myPoint.x][myPoint.y-1] == mapEmpty)	emptyCnt++;
	if(myGD->mapState[myPoint.x][myPoint.y+1] == mapEmpty)	emptyCnt++;
	
	if(emptyCnt == 0)
	{
		stopMyAction();
		return;
	}
	
	IntPoint jackPoint = myGD->getJackPoint();
	if(jackPoint.x == myPoint.x && jackPoint.y == myPoint.y)
	{
		myGD->communication("CP_jackCrashDie");
		myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
		jackDie();
		return;
	}
	
	if(ingFrame <= 30)
	{
		fragmentImg.first->setScale(fragmentImg.first->getScale() + 1.f/30.f);
		if(ingFrame == 30)
		{
			crashMapForIntPoint(myPoint);
			// diffusion
			int random_value = rand()%2;
			if(random_value == 0) // 2 direction
			{
				int first_rd = rand()%4;
				diffusionBF(first_rd);
				int other_rd = first_rd + rand()%3+1;
				other_rd %= 4;
				diffusionBF(other_rd);
			}
			else // 3 direction
			{
				int first_rd = rand()%4;
				diffusionBF(first_rd);
				int second_rd = first_rd + rand()%3+1;
				second_rd %= 4;
				diffusionBF(second_rd);
				int other_rd = second_rd;
				while(other_rd == second_rd || other_rd == first_rd)
				{
					other_rd = rand()%4;
				}
				diffusionBF(other_rd);
			}
		}
	}
	else if(ingFrame <= 60)
	{
		fragmentImg.first->setScale(fragmentImg.first->getScale() - 1.f/30.f);
	}
	
	
	if(ingFrame >= 60)
	{
		stopMyAction();
	}
}
void BurnFragment::diffusionBF (int t_direction)
{
	if(myLife <= 1)
		return;
	if(t_direction == 0) // left
	{
		if(myGD->mapState[myPoint.x-1][myPoint.y] == mapOldget || myGD->mapState[myPoint.x-1][myPoint.y] == mapOldline)
		{
			BurnFragment* t_bf = BurnFragment::create(IntPoint(myPoint.x-1, myPoint.y), myLife-1, myParent, mType, target_removeEffect, delegate_removeEffect);
			myParent->addChild(t_bf);
		}
	}
	else if(t_direction == 1) // right
	{
		if(myGD->mapState[myPoint.x+1][myPoint.y] == mapOldget || myGD->mapState[myPoint.x+1][myPoint.y] == mapOldline)
		{
			BurnFragment* t_bf = BurnFragment::create(IntPoint(myPoint.x+1, myPoint.y), myLife-1, myParent, mType, target_removeEffect, delegate_removeEffect);
			myParent->addChild(t_bf);
		}
	}
	else if(t_direction == 2) // down
	{
		if(myGD->mapState[myPoint.x][myPoint.y-1] == mapOldget || myGD->mapState[myPoint.x][myPoint.y-1] == mapOldline)
		{
			BurnFragment* t_bf = BurnFragment::create(IntPoint(myPoint.x, myPoint.y-1), myLife-1, myParent, mType, target_removeEffect, delegate_removeEffect);
			myParent->addChild(t_bf);
		}
	}
	else // up
	{
		if(myGD->mapState[myPoint.x][myPoint.y+1] == mapOldget || myGD->mapState[myPoint.x][myPoint.y+1] == mapOldline)
		{
			BurnFragment* t_bf = BurnFragment::create(IntPoint(myPoint.x, myPoint.y+1), myLife-1, myParent, mType, target_removeEffect, delegate_removeEffect);
			myParent->addChild(t_bf);
		}
	}
}
void BurnFragment::stopMyAction ()
{
	unschedule(schedule_selector(BurnFragment::myAction));
	removeFromParentAndCleanup(true);
}
void BurnFragment::myInit (IntPoint t_sp, int t_life, CCNode * t_parent, int t_mType, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect)
{
	target_removeEffect = t_removeEffect;
	delegate_removeEffect = d_removeEffect;
	
	setPosition(ccp((t_sp.x-1)*pixelSize+1, (t_sp.y-1)*pixelSize+1));
	mType = t_mType;
	myParent = t_parent;
	
	myPoint = t_sp;
	myLife = t_life;
	
	//	if(mType == 1)
	//	{
	fragmentImg = KS::loadCCBI<CCSprite*>(this, "fx_pollution1.ccbi");
	fragmentImg.first->setScale(0);
	addChild(fragmentImg.first);
	//	}
	//	else
	//	{
	//		fragmentImg = CCSprite::create("iceFragment.png");
	//		fragmentImg->setScale(0);
	//		addChild(fragmentImg);
	//	}
	
	startMyAction();
}

Firework::~ Firework ()
{
	CCLOG("exit firework");
}
Firework * Firework::create (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern)
{
	Firework* t_bf = new Firework();
	t_bf->myInit(cumberPosition, jackPosition, pattern);
	t_bf->autorelease();
	return t_bf;
}
void Firework::crashMapForPoint (IntPoint point, int radius)
{
	for(int y = - radius; y <= radius; y++)
	{
		for(int x = - radius; x <= radius; x++)
		{
			if(sqrt(x*x + y*y) <= radius)
				crashMapForIntPoint(IntPoint(point.x + x, point.y + y));
		}
	}
}
void Firework::myInit (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern)
{
	m_numberAtOnce = pattern.get("frameinterval", 15).asInt();
	m_frameInterval = pattern.get("numberatonce", 18).asInt();
	m_1TO2 = false;
	m_step = 1;
	m_bombFrame = 200;
	this->m_pattern = pattern;
	m_parentMissile = KS::loadCCBI<CCSprite*>(this, "pattern_flame1.ccbi").first;
	//		m_parentMissile = CCParticleSystemQuad::create("pm.plist");
	//		m_parentMissile->setPositionType(kCCPositionTypeRelative);
	m_parentMissile->setPosition(cumberPosition);
	addChild(m_parentMissile);
	
	//		CCPoint jackPosition = ip2ccp(myGD->getJackPoint());
	CCPoint subtract = jackPosition - cumberPosition;
	m_parentMissileGoal.init(cumberPosition, jackPosition, 0.01f * ccpLength(subtract));
	scheduleUpdate();
	
	int m_color = 1;
	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);
	
	addChild(batchNode);
	
}
void Firework::setTwoStep ()
{
	
	
	
	//		m_parentMissile->setStartColor(ccc4f(0, 0, 0, 0));
	//		m_parentMissile->setEndColor(ccc4f(0, 0, 0, 0));
	m_1TO2 = true;
	myGD->communication("MS_resetRects", false);
	addChild(KSGradualValue<float>::create(1, 0, 2.f,
																				 [=](float t)
																				 {
																					 CCLOG("scale = %f", t);
																					 m_parentMissile->setScale(t);
																				 },
																				 [=](float t)
																				 {
																					 CCLOG("scale finish");
																					 m_step = 2;
																					 m_1TO2 = false;
																					 m_frame = 0;
																					 m_sourcePosition = m_parentMissile->getPosition();
																					 
																					 m_parentMissile->removeFromParent();
																					 
																					 CCSprite* frame2 = KS::loadCCBI<CCSprite*>(this, "pattern_flame2.ccbi").first;
																					 frame2->setPosition(m_sourcePosition);
																					 addChild(frame2);
																					 addChild(KSTimer::create(0.7, [=]()
																																		{
																																			frame2->removeFromParent();
																																		}));
																				 }));
	
	
	schedule(schedule_selector(ThisClassType::selfRemove));
}
void Firework::selfRemove (float dt)
{
	if(batchNode->getChildrenCount() == 0 && getChildrenCount() == 1)
	{
		removeFromParentAndCleanup(true);
	}
}
void Firework::jackDie ()
{
	//		unscheduleUpdate();
	if(m_step == 1 && m_1TO2 == false)
	{
		setTwoStep();
	}
}
void Firework::lineDie (IntPoint t_p)
{
	//		unscheduleUpdate();
	myGD->communication("Main_showLineDiePosition", t_p);
	//		(target_removeEffect->*delegate_removeEffect)();
	if(m_step == 1 && m_1TO2 == false)
	{
		setTwoStep();
	}
}
void Firework::update (float dt)
{
	//		CCLOG("pokjuk %d", m_frame);
	
	bool r = m_parentMissileGoal.step(1.f / 60.f);
	if(m_step == 1)
	{
		m_frame++;
		m_parentMissile->setPosition(m_parentMissileGoal.getValue());
		
		// 깎는 코드.
		//if(m_frame % 5 == 0)
		//crashMapForPoint(ccp2ip(m_parentMissile->getPosition()), 10);
	}
	
	if(!r && m_step == 1 && m_1TO2 == false)
	{
		setTwoStep();
		
	}
	
	if(m_step == 2)
	{
		m_frame++;
		
		if(m_frame % m_frameInterval == 0)
		{
			float bulletSpeed = 2.f;
			int m_color = 1;
			std::string imgFileName;
			std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
			if(KS::isExistFile(fileName))
				imgFileName = fileName;
			else
				imgFileName = "cumber_missile1.png";
			CCSize t_mSize = CCSize(4.f, 4.f);
			
			for(int i=0; i<=360; i+=360.f / m_numberAtOnce)
			{
				MissileUnit* t_mu = MissileUnit::create(m_sourcePosition, i, bulletSpeed,
																								imgFileName.c_str(), t_mSize,0, 0);
				batchNode->addChild(t_mu);
			}
			
		}
		
		
		if(m_frame == m_bombFrame)
		{
			
			m_step = 3;
		}
		
	}
}
MovingSunflower * MovingSunflower::create (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern)
{
	CCLOG("%f %f", cumberPosition.x, cumberPosition.y);
	MovingSunflower* t_bf = new MovingSunflower();
	t_bf->myInit(cumberPosition, jackPosition, pattern);
	t_bf->autorelease();
	return t_bf;
}
void MovingSunflower::crashMapForPoint (IntPoint point, int radius)
{
	for(int y = - radius; y <= radius; y++)
	{
		for(int x = - radius; x <= radius; x++)
		{
			if(sqrt(x*x + y*y) <= radius)
				crashMapForIntPoint(IntPoint(point.x + x, point.y + y));
		}
	}
}
void MovingSunflower::myInit (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern)
{
	m_step = 1;
	m_bombFrame = 300;
	m_parentMissile = KS::loadCCBI<CCSprite*>(this, "pattern_flame3.ccbi");
	KS::setBlendFunc(m_parentMissile.first, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	m_pattern = pattern;
	
	m_parentMissile.first->setPosition(cumberPosition);
	addChild(m_parentMissile.first);
	
	//		CCPoint jackPosition = ip2ccp(myGD->getJackPoint());
	
	auto mat = CCAffineTransformMakeIdentity();
	auto mat2 = CCAffineTransformRotate(mat, m_well512.GetFloatValue(-15 * M_PI / 180.f, +15 * M_PI / 180.f));
	jackPosition = CCPointApplyAffineTransform(jackPosition, mat2);
	CCPoint subtract = jackPosition - cumberPosition;
	
	m_parentMissileGoal.init(cumberPosition, jackPosition, 0.01f * ccpLength(subtract) * 100.f / pattern.get("parentspeed", 100.f).asFloat());
	scheduleUpdate();
	
	int m_color = 1;
	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);
	
	addChild(batchNode);
	
}
void MovingSunflower::setTwoStep ()
{
	schedule(schedule_selector(ThisClassType::selfRemove));
	myGD->communication("MS_resetRects", false);
	m_step = 2;
	m_frame = 0;
	m_sourcePosition = m_parentMissile.first->getPosition();
	//	m_parentMissile->setStartColor(ccc4f(0, 0, 0, 0));
	//	m_parentMissile->setEndColor(ccc4f(0, 0, 0, 0));
	addChild(KSGradualValue<float>::create(1, 0, 1.0f, [=](float t){
		m_parentMissile.first->setScale(t);
	},
																				 [=](float t)
																				 {
																					 m_parentMissile.first->removeFromParent();
																				 }
																				 ));
}
void MovingSunflower::selfRemove (float dt)
{
	if(batchNode->getChildrenCount() == 0 && getChildrenCount() == 1)
	{
		removeFromParentAndCleanup(true);
	}
}
void MovingSunflower::jackDie ()
{
	//		unscheduleUpdate();
	if(m_step == 1)
	{
		setTwoStep();
	}
}
void MovingSunflower::lineDie (IntPoint t_p)
{
	//		unscheduleUpdate();
	myGD->communication("Main_showLineDiePosition", t_p);
	//		(target_removeEffect->*delegate_removeEffect)();
	if(m_step == 1)
	{
		setTwoStep();
	}
}
void MovingSunflower::update (float dt)
{
	//		CCLOG("pokjuk %d", m_frame);
	
	bool r = m_parentMissileGoal.step(1.f / 60.f);
	if(m_step == 1)
	{
		m_frame++;
		m_parentMissile.first->setPosition(m_parentMissileGoal.getValue());
		if(m_frame % 5 == 0)
			crashMapForPoint(ccp2ip(m_parentMissile.first->getPosition()), 10);
		
		if(m_frame % m_pattern.get("bulletterm", 15).asInt() == 0)
		{
			float bulletSpeed = m_pattern.get("childspeed", 100.f).asFloat() / 100.f;
			int m_color = 1;
			std::string imgFileName;
			std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
			if(KS::isExistFile(fileName))
				imgFileName = fileName;
			else
				imgFileName = "cumber_missile1.png";
			CCSize t_mSize = CCSize(4.f, 4.f);
			
			for(int i=0; i<=360; i+= 360 / m_pattern.get("childnumber", 36).asInt())
			{
				MissileUnit* t_mu = MissileUnit::create(m_parentMissileGoal.getValue(), i, bulletSpeed,
																								imgFileName.c_str(), t_mSize,0, 0);
				batchNode->addChild(t_mu);
			}
		}
		
	}
	
	if(!r && m_step == 1)
	{
		setTwoStep();
		
	}
	
	if(m_step == 2)
	{
		
	}
}

ThrowBomb * ThrowBomb::create (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern)
{
	ThrowBomb* t_bf = new ThrowBomb();
	t_bf->myInit(cumberPosition, jackPosition, pattern);
	t_bf->autorelease();
	return t_bf;
}
void ThrowBomb::crashMapForPoint (IntPoint point, int radius)
{
	for(int y = - radius; y <= radius; y++)
	{
		for(int x = - radius; x <= radius; x++)
		{
			if(sqrt(x*x + y*y) <= radius)
				crashMapForIntPoint(IntPoint(point.x + x, point.y + y));
		}
	}
}
void ThrowBomb::selfRemove (float dt)
{
	if(getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
		myGD->communication("MS_resetRects", false);
	}
}
void ThrowBomb::jackDie ()
{
	//		unscheduleUpdate();
	//		if(m_step == 1)
	//		{
	//			setTwoStep();
	//		}
}
void ThrowBomb::lineDie (IntPoint t_p)
{
	//		unscheduleUpdate();
	myGD->communication("Main_showLineDiePosition", t_p);
	//		(target_removeEffect->*delegate_removeEffect)();
	//		if(m_step == 1)
	//		{
	//			m_step
	//		}
}
void ThrowBomb::myInit (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern)
{
	m_step = 1;
	m_pattern = pattern;
	m_angle = atan2(jackPosition.y - cumberPosition.y, jackPosition.x - cumberPosition.x);
	m_angle += m_well512.GetFloatValue(-10 * M_PI/180.f, +10 * M_PI/180.f);
	//		m_parentMissile = CCParticleSystemQuad::create("throwbomb.plist");
	//		m_parentMissile->setPositionType(kCCPositionTypeRelative);
	m_parentMissile = KS::loadCCBI<CCSprite*>(this, "pattern_meteor4.ccbi").first;
	
	m_parentMissile->setPosition(cumberPosition);
	addChild(m_parentMissile);
	
	scheduleUpdate();
}
void ThrowBomb::update (float dt)
{
	//		CCLOG("pokjuk %d", m_frame);
	if(m_step == 1)
	{
		m_frame++;
		CCPoint delta = ccp(cos(m_angle) * 3.5f, sin(m_angle) * 3.5f) * m_pattern.get("speed", 100.f).asFloat() / 100.f;
		
		CCPoint missilePosition = m_parentMissile->getPosition();
		m_parentMissile->setPosition(missilePosition + delta);
		
		IntPoint mapPoint = ccp2ip(m_parentMissile->getPosition());
		float half_distance = 10;
		bool crash = false;
		for(int i=mapPoint.x-half_distance/2;i<=mapPoint.x+half_distance/2 && !crash;i++)
		{
			for(int j=mapPoint.y-half_distance/2;j<=mapPoint.y+half_distance/2 && !crash;j++)
			{
				float calc_distance = sqrtf(powf((mapPoint.x - i)*pixelSize,2) + powf((mapPoint.y - j)*pixelSize, 2));
				if(calc_distance < half_distance)
				{
					IntPoint check_position = IntPoint(i,j);
					if(!check_position.isInnerMap() || myGD->mapState[check_position.x][check_position.y] != mapEmpty)
					{
						crash = true;
					}
				}
			}
		}
		
		if(crash)
		{
			AudioEngine::sharedInstance()->playEffect("se_meteo_3.mp3");
			crashMapForPoint(ccp2ip(m_parentMissile->getPosition()), m_pattern.get("area", 15).asInt());
			m_step = 2;
		}
	}
	
	if(m_step == 2) // 폭발.
	{
		auto bomb = KS::loadCCBI<CCSprite*>(this, "bomb_8_7.ccbi");//"fx_bomb5.ccbi");
		KS::setBlendFunc(bomb.first, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		bomb.first->setPosition(m_parentMissile->getPosition());
		addChild(bomb.first);
		addChild(KSTimer::create(1.3f, [=](){bomb.first->removeFromParent();})); // 1.3 초 후에 사라짐.
		
		m_step = 3;
		m_parentMissile->removeFromParent();
		schedule(schedule_selector(ThisClassType::selfRemove));
		
	}
}
namespace std
{
	size_t hash <IntPoint>::operator () (IntPoint const & ip) const
	{
		int x = ip.x;
		int y = ip.y;
		return hash<int>()(x) ^ hash<int>()(y);
		//			return x + y;
	}
}
ReaverScarab * ReaverScarab::create (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern)
{
	ReaverScarab* t_bf = new ReaverScarab();
	t_bf->myInit(cumberPosition, jackPosition, pattern);
	t_bf->autorelease();
	return t_bf;
}
void ReaverScarab::crashMapForPoint (IntPoint point, int radius)
{
	for(int y = - radius; y <= radius; y++)
	{
		for(int x = - radius; x <= radius; x++)
		{
			if(sqrt(x*x + y*y) <= radius)
				crashMapForIntPoint(IntPoint(point.x + x, point.y + y));
		}
	}
}
void ReaverScarab::selfRemove (float dt)
{
	if(getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
		myGD->communication("MS_resetRects", false);
	}
}
void ReaverScarab::jackDie ()
{
	//		unscheduleUpdate();
	//		if(m_step == 1)
	//		{
	//			setTwoStep();
	//		}
}
void ReaverScarab::lineDie (IntPoint t_p)
{
	//		unscheduleUpdate();
	myGD->communication("Main_showLineDiePosition", t_p);
	//		(target_removeEffect->*delegate_removeEffect)();
	//		if(m_step == 1)
	//		{
	//			m_step
	//		}
}
void ReaverScarab::myInit (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern)
{
	m_pattern = pattern;
	m_crashArea = m_pattern.get("area", 15).asInt();
	m_insertCount = 0;
	m_step = 1;
	m_jackPoint = ccp2ip(jackPosition);
	CCLOG("init %d %d", m_jackPoint.x, m_jackPoint.y);
	m_parentMissile = CCParticleSystemQuad::create("throwbomb.plist");
	m_parentMissile->setPositionType(kCCPositionTypeRelative);
	
	m_parentMissile->setPosition(cumberPosition);
	addChild(m_parentMissile);
	
	scheduleUpdate();
	
	aStar(m_jackPoint);
	m_targetSprite = KS::loadCCBI<CCSprite*>(this, "target3.ccbi").first;
	addChild(m_targetSprite);
}
int ReaverScarab::lengthToEnd (IntPoint point)
{
	return (abs(m_jackPoint.x - point.x) + abs(m_jackPoint.y - point.y)) * 10;
}
bool ReaverScarab::processObject (IntPoint pointFrom, IntPoint pointTo, int distance)
{
	if(pointTo.x < mapLoopRange::mapWidthInnerBegin || pointTo.x >= mapLoopRange::mapWidthInnerEnd ||
		 pointTo.y < mapLoopRange::mapHeightInnerBegin || pointTo.y >= mapLoopRange::mapHeightInnerEnd)
		return true;
	// 장애물이면 패스.
	
	if(!(myGD->getJackPoint() == pointTo))
	{
		if(myGD->mapState[pointTo.x][pointTo.y] == mapType::mapOldline ||
			 myGD->mapState[pointTo.x][pointTo.y] == mapType::mapOutline)
			return true;
		if(myGD->mapState[pointTo.x][pointFrom.y] == mapType::mapOldline ||
			 myGD->mapState[pointTo.x][pointFrom.y] == mapType::mapOutline )
			return true;
		if(myGD->mapState[pointFrom.x][pointTo.y] == mapType::mapOldline ||
			 myGD->mapState[pointFrom.x][pointTo.y] == mapType::mapOutline)
			return true;
	}
	
	//	bool found = false;
	auto openedIter = m_openList.find(CoordAndCellInfo(pointTo.x, pointTo.y, 0, 0, 0, 0));
	
	auto closedIter = m_closeListMap.find(pointTo);
	// 닫힌 쪽에 있지 않고 열린 쪽이라면
	if(openedIter != m_openList.end() && closedIter == m_closeListMap.end() )
	{
		m_openList.erase(openedIter);
		auto openedValue = *openedIter;
		//		auto smart = ProbSelector({1,50}).getResult();
		//		smart = 1;
		if(openedValue.g > m_closeListMap[pointFrom].g + distance)
		{
			openedValue.dx = pointFrom.x - openedValue.x;
			openedValue.dy = pointFrom.y - openedValue.y;
			openedValue.g = m_closeListMap[pointFrom].g + distance;
		}
		
		m_openList.insert(openedValue);
		return true;
	}
	
	// 닫힌쪽도 아니고 열린쪽도 아니면!
	if(closedIter == m_closeListMap.end() && openedIter == m_openList.end())
	{
		CellInfo pointToObj(0, 0, 0, 0);
		//		CellInfo pointToObj = {g:0, h:0, dx:0, dy:0};
		pointToObj.g = m_closeListMap[pointFrom].g + distance;
		pointToObj.h = lengthToEnd(pointTo);
		pointToObj.dx = pointFrom.x - pointTo.x; // 부모 가르키기.
		pointToObj.dy = pointFrom.y - pointTo.y; // 부모 가르키기.
		m_insertCount++;
		
		auto obj = CoordAndCellInfo(pointTo.x, pointTo.y, pointToObj.dx, pointToObj.dy, pointToObj.g, pointToObj.h);
		obj.order = m_insertCount;
		m_openList.insert(obj);
		//		m_openList[pointTo] = pointToObj;
	}
	return true;
	
}
void ReaverScarab::aStar (IntPoint endPt)
{
	chrono::time_point<chrono::system_clock> start, end;
	chrono::duration<double> elapsed_seconds;
	start = chrono::system_clock::now();
	
	
	
	
	
	IntPoint startPoint = ccp2ip(m_parentMissile->getPosition());
	
	m_openList = decltype(m_openList)();
	m_closeListMap.clear();
	
	m_bulletReversePath.clear();
	auto ret = CoordAndCellInfo(startPoint.x, startPoint.y, 0, 0, 0, lengthToEnd(startPoint));
	m_openList.insert(ret);
	//	m_openList[startPoint] = CellInfo(0, 0, 0, lengthToEnd(startPoint));
	
	bool foundSolution = false;
	int counter = 0;
	
	
	while(m_openList.size() > 0)
	{
		counter++;
		if(counter >= 1000)
			break;
		auto minElement = *m_openList.begin();
		
		
		IntPoint fminIndex = IntPoint(minElement.x, minElement.y);
		CellInfo fminElement = CellInfo(minElement.dx, minElement.dy, minElement.g, minElement.h);
		
		m_closeListMap[fminIndex] = fminElement;
		
		m_openList.erase(m_openList.begin());
		//		m_openList.erase(fminIndex);
		
		//		CCLOG("%d %d -> %d %d", fminIndex.x, fminIndex.y, endPoint.x, endPoint.y);
		if(fminIndex == endPt)
			break;
		
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x - 1, fminIndex.y + 1), 14);
		if(foundSolution)
			break;
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x, fminIndex.y + 1), 10);
		if(foundSolution)
			break;
		
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x + 1, fminIndex.y + 1), 14);
		if(foundSolution)
			break;
		
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x - 1, fminIndex.y), 10);
		if(foundSolution)
			break;
		
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x + 1, fminIndex.y), 10);
		if(foundSolution)
			break;
		
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x - 1, fminIndex.y - 1), 14);
		if(foundSolution)
			break;
		
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x, fminIndex.y - 1), 10);
		if(foundSolution)
			break;
		
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x + 1, fminIndex.y - 1), 14);
		if(foundSolution)
			break;
	}
	
	end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds1 = end-start;
	KS::KSLog("process step 1 / time : %, %", elapsed_seconds1.count(), counter);
	
	
	IntPoint coordNearEndPoint = m_jackPoint;
	int minDistance = INT_MAX;
	for(auto obj : m_closeListMap)
	{
		if(minDistance > (obj.first - m_jackPoint).length())
		{
			minDistance = (obj.first - m_jackPoint).length();
			coordNearEndPoint = obj.first;
		}
	}
	
	
	
	if(m_closeListMap.find(coordNearEndPoint) != m_closeListMap.end())
	{
		IntPoint currentPoint = coordNearEndPoint;
		IntPoint beforePoint = coordNearEndPoint;
		while(!(currentPoint == startPoint))
		{
			IntPoint coord = currentPoint;
			m_bulletReversePath.push_back(ip2ccp(currentPoint));
			beforePoint = currentPoint;
			currentPoint.x += m_closeListMap[coord].dx;
			currentPoint.y += m_closeListMap[coord].dy;
		}
		m_bulletIter = m_bulletReversePath.rbegin();
		while(0);
	}
	
	//	int t = ;
#if 0 // 탐색하는 과정 보임.
	ccColor3B c3 = ccc3(m_well512.GetValue(0, 255),m_well512.GetValue(0, 255),m_well512.GetValue(0, 255));
	for(auto cl : m_closeListMap)
	{
		CCSprite* rt = CCSprite::create();
		rt->setTextureRect(CCRectMake(0, 0, 2, 2));
		rt->setColor(c3);
		rt->setPosition(ip2ccp(cl.first));
		//		rt->setOpacity(100);
		addChild(rt);
		rt->runAction(CCSequence::create(CCDelayTime::create(4.f), CCRemoveSelf::create(), 0));
	}
#endif
	// alert("complete!!");
}
void ReaverScarab::update (float dt)
{
	
	if(m_step == 1)
	{
		m_frame++;
		if(m_bulletReversePath.empty() == false && m_bulletIter != m_bulletReversePath.rend())
		{
			m_parentMissile->setPosition((*m_bulletIter));
			
			++m_bulletIter;
		}
		else
		{
			m_step = 2;
			crashMapForPoint(ccp2ip(m_parentMissile->getPosition()), m_crashArea);
		}
		//		aStar(m_jackPoint);
		
		
		if(m_frame >= m_pattern.get("totalframe", 240).asInt())
		{
			m_step = 2;
			crashMapForPoint(ccp2ip(m_parentMissile->getPosition()), m_crashArea);
		}
		m_targetSprite->setPosition(ip2ccp(myGD->getJackPoint()));
	}
	if(m_step == 2) // 폭발.
	{
		m_parentMissile->removeFromParent();
		schedule(schedule_selector(ThisClassType::selfRemove));
		
		auto bomb = KS::loadCCBI<CCSprite*>(this, "bomb_8_7.ccbi");
		KS::setBlendFunc(bomb.first, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		addChild(bomb.first);
		m_targetSprite->removeFromParent();
		addChild(KSTimer::create(1.3f, [=](){
			bomb.first->removeFromParent();
		})); // 1.3 초 후에 사라짐.
		
		bomb.first->setPosition(m_parentMissile->getPosition());
		m_step = 3;
	}
	
}
ReaverScarab::CellInfo::CellInfo (int _dx, int _dy, int _g, int _h)
: dx (_dx), dy (_dy), g (_g), h (_h)
{}
ReaverScarab::CellInfo::CellInfo ()
{}
bool ReaverScarab::CellInfo::operator < (CellInfo const & ci) const
{
	return g+h < ci.g + ci.h;
}
ReaverScarab::CoordAndCellInfo::CoordAndCellInfo (int _x, int _y, int _dx, int _dy, int _g, int _h)
: x (_x), y (_y), dx (_dx), dy (_dy), g (_g), h (_h)
{
	order = 0;
}
ReaverScarab::CoordAndCellInfo::CoordAndCellInfo ()
{
	order = 0;
}
bool ReaverScarab::CoordAndCellInfo::operator < (CoordAndCellInfo const & ci) const
{
	return g+h - order< ci.g + ci.h - ci.order;
}
bool ReaverScarab::CoordAndCellInfo::operator == (CoordAndCellInfo const & ci) const
{
	return x == ci.x && y == ci.y;
}

Burn * Burn::create (CCPoint t_sp, float t_baseDistance, float t_shootAngle, int t_mType, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect)
{
	Burn* t_b = new Burn();
	t_b->myInit(t_sp, t_baseDistance, t_shootAngle, t_mType, t_removeEffect, d_removeEffect);
	t_b->autorelease();
	return t_b;
}
void Burn::startMyAction ()
{
	ingFrame = 0;
	schedule(schedule_selector(Burn::myAction));
}
void Burn::removeEffect ()
{
	unschedule(schedule_selector(Burn::myAction));
	(target_removeEffect->*delegate_removeEffect)();
	removeFromParentAndCleanup(true);
	CCLOG("%x destroy", this);
}
void Burn::myAction ()
{
	ingFrame++;
	
	if(!positionSetted)
	{
		CCPoint afterPosition = ccpAdd(getPosition(), movingDv);
		setPosition(afterPosition);
		
		IntPoint mapPoint = IntPoint((afterPosition.x-1)/pixelSize + 1, (afterPosition.y-1)/pixelSize + 1);
		if(mapPoint.isInnerMap() && myGD->mapState[mapPoint.x][mapPoint.y] == mapOldline)
		{
			positionSetted = true;
			BurnFragment* t_bf = BurnFragment::create(mapPoint, 5, getParent(), mType, target_removeEffect, delegate_removeEffect);
			getParent()->addChild(t_bf);
			stopMyAction();
		}
		else if(mapPoint.isInnerMap() && myGD->mapState[mapPoint.x][mapPoint.y] == mapOldget)
		{
			IntMoveState searchFirstMoveState = IntMoveState(mapPoint.x, mapPoint.y, directionStop);
			searchAndMoveOldline(searchFirstMoveState);
			stopMyAction();
		}
	}
	
	
	if(ingFrame > 60 && !positionSetted)
	{
		stopMyAction();
	}
}
void Burn::searchAndMoveOldline (IntMoveState searchFirstMoveState)
{
	queue<IntMoveState> bfsArray;
	bfsArray.push(searchFirstMoveState);
	
	bool isFinded = false;
	IntPoint findedPoint;
	
	while(!bfsArray.empty() && !isFinded)
	{
		IntMoveState t_ms = bfsArray.front();
		bfsArray.pop();
		if(myGD->mapState[t_ms.origin.x][t_ms.origin.y] == mapOldline)
		{
			isFinded = true;
			findedPoint = t_ms.origin;
		}
		else
		{
			if(t_ms.direction == directionStop)
			{
				for(int i = directionLeftUp;i <= directionUp; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionLeftUp)
			{
				for(int i = directionLeftUp;i <= directionLeft; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);
				}
				
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionLeft)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x-1, t_ms.origin.y, directionLeft);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionLeftDown)
			{
				for(int i = directionLeft;i <= directionDown; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionDown)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y-1, directionDown);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionRightDown)
			{
				for(int i = directionDown;i <= directionRight; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionRight)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionRightUp)
			{
				for(int i = directionRight;i <= directionUp; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionUp)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);}
			}
		}
	}
	
	if(isFinded)
	{
		positionSetted = true;
		BurnFragment* t_bf = BurnFragment::create(findedPoint, 10, getParent(), mType, target_removeEffect, delegate_removeEffect);
		getParent()->addChild(t_bf);
	}
	else // escape point not found
	{
		CCLOG("escape point not found!");
	}
}
void Burn::stopMyAction ()
{
	unschedule(schedule_selector(Burn::myAction));
	removeFromParentAndCleanup(true);
}
void Burn::myInit (CCPoint t_sp, float t_baseDistance, float t_shootAngle, int t_mType, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect)
{
	target_removeEffect = t_removeEffect;
	delegate_removeEffect = d_removeEffect;
	
	positionSetted = false;
	
	mType = t_mType;
	
	setPosition(t_sp);
	
	movingDv.x = 1;
	movingDv.y = tanf(t_shootAngle/180.f*M_PI);
	
	if((t_shootAngle > 90 && t_shootAngle < 270) || t_shootAngle < -90)
	{
		movingDv = ccpMult(movingDv, -1.f);
	}
	
	float div_value = sqrtf(powf(movingDv.x, 2.f) + powf(movingDv.y, 2.f));
	movingDv = ccpMult(movingDv, 1.f/div_value);
	movingDv = ccpMult(movingDv, t_baseDistance/60.f);
	
	startMyAction();
}


MeteorStorm * MeteorStorm::create (CCPoint t_sp, CCPoint t_fp, int t_moving_frame)
{
	MeteorStorm* t_ms = new MeteorStorm();
	t_ms->myInit(t_sp, t_fp, t_moving_frame);
	t_ms->autorelease();
	return t_ms;
}
void MeteorStorm::myInit (CCPoint t_sp, CCPoint t_fp, int t_moving_frame)
{
	
	
	inner_fire_particle = CCParticleSystemQuad::create("meteor_storm.plist");
	inner_fire_particle->setPositionType(kCCPositionTypeRelative);
	addChild(inner_fire_particle);
	
	outer_fire_particle	= CCParticleSystemQuad::createWithTotalParticles(100);
	outer_fire_particle->setPositionType(kCCPositionTypeRelative);
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("meteor_storm_particle.png");
	outer_fire_particle->setTexture(texture);
	outer_fire_particle->setEmissionRate(100.00);
	outer_fire_particle->setAngle(45.0);
	outer_fire_particle->setAngleVar(0.0);
	ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
	outer_fire_particle->setBlendFunc(blendFunc);
	outer_fire_particle->setDuration(-1.0);
	outer_fire_particle->setEmitterMode(kCCParticleModeGravity);
	ccColor4F startColor = {1.00,0.98,0.40,1.00}; // 0.76 0.25 0.12
	outer_fire_particle->setStartColor(startColor);
	ccColor4F startColorVar = {0,0,0,0};
	outer_fire_particle->setStartColorVar(startColorVar);
	ccColor4F endColor = {1.00,0.07,0.03,1.00};
	outer_fire_particle->setEndColor(endColor);
	ccColor4F endColorVar = {0,0,0,0};
	outer_fire_particle->setEndColorVar(endColorVar);
	outer_fire_particle->setStartSize(60.00);
	outer_fire_particle->setStartSizeVar(0.0);
	outer_fire_particle->setEndSize(80.0);
	outer_fire_particle->setEndSizeVar(0.0);
	outer_fire_particle->setGravity(ccp(-400,400));
	outer_fire_particle->setRadialAccel(0.0);
	outer_fire_particle->setRadialAccelVar(0.0);
	outer_fire_particle->setSpeed(0);
	outer_fire_particle->setSpeedVar(200.0);
	outer_fire_particle->setTangentialAccel(0);
	outer_fire_particle->setTangentialAccelVar(0);
	outer_fire_particle->setTotalParticles(100);
	outer_fire_particle->setLife(0.0);
	outer_fire_particle->setLifeVar(1.00);
	outer_fire_particle->setStartSpin(0.0);
	outer_fire_particle->setStartSpinVar(0.0);
	outer_fire_particle->setEndSpin(0.0);
	outer_fire_particle->setEndSpinVar(0.0);
	outer_fire_particle->setPosition(ccp(20,-20));
	outer_fire_particle->setPosVar(ccp(0,0));
	addChild(outer_fire_particle);
	
}


RunDownSaw * RunDownSaw::create (CCPoint t_sp, float t_speed, float t_angle, IntSize t_mSize, int runDown)
{
	RunDownSaw* t_to = new RunDownSaw();
	t_to->myInit(t_sp, t_speed, t_angle, t_mSize, runDown);
	t_to->autorelease();
	return t_to;
}
void RunDownSaw::startMyAction ()
{
	AudioEngine::sharedInstance()->playEffect("sound_throw_obj_shot.mp3", false);
	schedule(schedule_selector(RunDownSaw::myAction));
}
void RunDownSaw::jackDie ()
{
	unschedule(schedule_selector(RunDownSaw::myAction));
	removeEffect();
}
void RunDownSaw::lineDie (IntPoint t_p)
{
	unschedule(schedule_selector(RunDownSaw::myAction));
	myGD->communication("Main_showLineDiePosition", t_p);
	removeEffect();
}
void RunDownSaw::removeEffect ()
{
	//		objImg->unscheduleAllSelectors();
	m_objImg->stopAllActions();
	CCScaleTo* t_fade = CCScaleTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(RunDownSaw::selfRemove));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	
	m_objImg->runAction(t_seq);
}
void RunDownSaw::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void RunDownSaw::stopMyAction ()
{
	unschedule(schedule_selector(RunDownSaw::myAction));
	myGD->communication("MS_resetRects", false);
	addChild(KSTimer::create(0.5f, [=](){
		addChild(KSGradualValue<float>::create(1, 0, 0.5f, [=](float t){
			m_objImg->setScale(t);
		}, [=](float t){
			
			removeFromParentAndCleanup(true);
			
			if(m_crashCount <= 40)
			{
				myGD->toScratch();
			}
		}));
	}));
}
void RunDownSaw::myAction (float dt)
{
	//		objImg->setRotation(objImg->getRotation() + random_spin);
	float dx = cos(m_angleDegree * M_PI / 180.f) * m_speed;
	float dy = sin(m_angleDegree * M_PI / 180.f) * m_speed;
	m_objImg->setPosition(m_objImg->getPosition() + ccp(dx, dy));
	
	CCPoint myPosition = m_objImg->getPosition();
	CCPoint deltaPosition = myPosition - m_lastSawPosition;
	//CCPoint subPosition = ccpSub(myPosition, b_c_p);
	deltaPosition.x = fabsf(deltaPosition.x);
	deltaPosition.y = fabsf(deltaPosition.y);
	
	if(deltaPosition.x > m_size.width || deltaPosition.y > m_size.height)
	{
		m_lastSawPosition = m_objImg->getPosition();
		IntPoint myPoint = IntPoint((myPosition.x-1)/pixelSize + 1 - m_size.width/2, (myPosition.y-1)/pixelSize + 1 - m_size.height/2);
		for(int i=0;i<m_size.height;i++)
		{
			for(int j=0;j<m_size.width;j++)
			{
				if(IntPoint(myPoint.x + j, myPoint.y + i).isInnerMap() && (myGD->mapState[myPoint.x + j][myPoint.y + i] == mapType::mapOldget ||
																																	 myGD->mapState[myPoint.x + j][myPoint.y + i] == mapType::mapOldline))
				{
					m_runDown--;
				}
				
				if(crashMapForIntPoint(IntPoint(myPoint.x+j,myPoint.y+i)))
				{
					m_crashCount++;
				}
			}
		}
	}
	
	if(myPosition.x < -30 || myPosition.x > 350 || myPosition.y < -30 || myPosition.y > 460 ||
		 m_runDown <= 0)
	{
		stopMyAction();
	}
}
void RunDownSaw::myInit (CCPoint t_sp, float t_speed, float t_angle, IntSize t_mSize, int runDown)
{
	// t_sp 자리에서 부터 t_angle 각도로 던짐.
	//
	m_crashCount = 0;
	m_speed = t_speed;
	m_size = t_mSize;
	m_runDown = runDown;
	m_angleDegree = t_angle;
	pair<CCSprite*, CCBAnimationManager*> alreadyWarning, alreadyWarning2;
	
	{
		//		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		//		CCBReader* reader = new CCBReader(nodeLoader);
		//		m_objImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("pattern_saw1.ccbi",this));
		m_objImg = KS::loadCCBI<CCSprite*>(this, "pattern_saw1.ccbi").first;
		m_objImg->setPosition(t_sp);
		m_lastSawPosition = m_objImg->getPosition();
		//KS::setBlendFunc(objImg, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		//		reader->release();
		addChild(m_objImg);
		m_objImg->setVisible(false);
		
		CCNode* guideNode = CCNode::create();
		guideNode->setRotation(-t_angle);
		addChild(guideNode);
		guideNode->setPosition(t_sp);
		alreadyWarning = KS::loadCCBI<CCSprite*>(this, "signal1_1.ccbi");
		alreadyWarning2 = KS::loadCCBI<CCSprite*>(this, "signal1_2.ccbi");
		guideNode->addChild(alreadyWarning.first);
		guideNode->addChild(alreadyWarning2.first);
		alreadyWarning2.first->setPosition(ccp(55.f / 2.f, 0));
	}
	
	addChild(KSTimer::create(0.7f, [=]()
													 {
														 AudioEngine::sharedInstance()->playEffect("sound_throw_obj_shot.mp3", false);
														 schedule(schedule_selector(RunDownSaw::myAction));
														 m_objImg->setVisible(true);
														 addChild(KSGradualValue<float>::create(255, 0, 0.3f, [=](float t)
																																		{
																																			KS::setOpacity(alreadyWarning.first, t);
																																			KS::setOpacity(alreadyWarning2.first, t);
																																			alreadyWarning2.first->setOpacity(t);
																																		}, nullptr));
													 }));
}

