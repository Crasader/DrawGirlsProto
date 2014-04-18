//
//  JackMissileShowShow.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 1. 23..
//
//

#include "JackMissileShow.h"
#include "AudioEngine.h"
#include "ASMotionStreak.h"

using namespace cocos2d;
using namespace std;

void JackMissileShow::startMoving ()
{
	
}
JM_UpgradeMissileShow * JM_UpgradeMissileShow::create (CCNode * t_target, int jm_type, float missile_speed, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile)
{
	JM_UpgradeMissileShow* t_um = new JM_UpgradeMissileShow();
	t_um->myInit(t_target, jm_type, missile_speed, t_boss_agi, t_popup, t_explosion, t_showDamageMissile, t_showMissMissile);
	t_um->autorelease();
	return t_um;
}
JM_UpgradeMissileShow * JM_UpgradeMissileShow::create (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile)
{
	JM_UpgradeMissileShow* t_um = new JM_UpgradeMissileShow();
	t_um->myInit(t_target, jm_type, missile_speed, s_p, t_boss_agi, t_popup, t_explosion, t_showDamageMissile, t_showMissMissile);
	t_um->autorelease();
	return t_um;
}
JM_UpgradeMissileShow::~ JM_UpgradeMissileShow ()
{
	//		myGD->communication("EP_subJackAttack");
}
void JM_UpgradeMissileShow::startMoving ()
{
	ingFrame = 0;
	schedule(schedule_selector(JM_UpgradeMissileShow::moving));
}
void JM_UpgradeMissileShow::stopMoving ()
{
	unschedule(schedule_selector(JM_UpgradeMissileShow::moving));
	removeFromParentAndCleanup(true);
}
void JM_UpgradeMissileShow::moving ()
{
	bool isEnable = true;
	
	if(isEnable)
	{
		ingFrame++;
		
		if(ingFrame < 20)
		{
			particlePosition = ccpAdd(loadImg->getPosition(),load_dv);
			loadImg->setPosition(particlePosition);
			particle->setPosition(particlePosition);
			mainImg->setPosition(particlePosition);
			
			if(loadImg->getScaleX() < 10.f)
				loadImg->setScaleX(loadImg->getScaleX() + 1.f);
		}
		else if(ingFrame == 20)
		{
			AudioEngine::sharedInstance()->playEffect("sound_jack_special_missile.mp3", false);
			particlePosition = ccpAdd(loadImg->getPosition(),load_dv);
			loadImg->setPosition(particlePosition);
			particle->setPosition(particlePosition);
			
			if(loadImg->getScaleX() < 10.f)
				loadImg->setScaleX(loadImg->getScaleX() + 1.f);
			
			edge = CCSprite::create((type_name + "_edge.png").c_str());
			
			ccBlendFunc t_blend1;
			t_blend1.src = GL_SRC_ALPHA;
			t_blend1.dst = GL_ONE;
			edge->setBlendFunc(t_blend1);
			
			edge->setPosition(ccp(loadImg->getContentSize().width,loadImg->getContentSize().height/2.f));
			edge->setScaleX(1.f/loadImg->getScaleX());
			loadImg->addChild(edge);
			
			load_removing = true;
			
			CCPoint subPosition = ccpSub(targetNode->getPosition(), loadImg->getPosition());
			float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
			float shootAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
			
			shoot_dv.x = 1;
			shoot_dv.y = tanf(shootAngle/180.f*M_PI);
			
			float div_value = sqrtf(powf(shoot_dv.x, 2.f) + powf(shoot_dv.y, 2.f));
			shoot_dv = ccpMult(shoot_dv, 1.f/div_value);
			shoot_dv = ccpMult(shoot_dv, distance/10); // img width 4
			
			if((shootAngle > 90.f && shootAngle < 270.f) || shootAngle < -90.f)
				shoot_dv = ccpMult(shoot_dv, -1.f);
			
			shoot_dv_distance = sqrtf(powf(shoot_dv.x, 2.f) + powf(shoot_dv.y, 2.f))/4.f;
			
			string temp_string = type_name + ".png";
			
			particle->setStartSpin(shootAngle+90);
			particle->setEndSpin(shootAngle+90);
			
			shootImg = CCSprite::create(temp_string.c_str());
			
			ccBlendFunc t_blend;
			t_blend.src = GL_SRC_ALPHA;
			t_blend.dst = GL_ONE;
			shootImg->setBlendFunc(t_blend);
			
			shootImg->setAnchorPoint(ccp(1,0.5));
			shootImg->setRotation(-shootAngle);
			shootImg->setPosition(loadImg->getPosition());
			
			mainImg->setPosition(particlePosition);
			
			addChild(shootImg, kZorderJMU_shootImg);
			
			if(!is_spin)
			{
				float t_angle = atan2f(shoot_dv.y, shoot_dv.x)/M_PI*180.f-90.f;
				mainImg->setRotation(-t_angle);
			}
		}
		else if(ingFrame < 20 + 10)
		{
			particlePosition = ccpAdd(shootImg->getPosition(),shoot_dv);
			shootImg->setPosition(particlePosition);
			particle->setPosition(particlePosition);
			mainImg->setPosition(particlePosition);
			
			if(shootImg->getScaleX() < 30.f)
			{
				shootImg->setScaleX(shootImg->getScaleX() + shoot_dv_distance);
			}
		}
		else if(ingFrame == 20 + 10)
		{
			float target_agi = boss_agi;
			
			float agi_rate = (target_agi-dex)/target_agi;
			agi_rate = agi_rate < 0 ? 0 : agi_rate;
			
			agi_rate = agi_rate/100.f*85.f + 0.1f;
			agi_rate *= 100.f;
			
			if(rand()%100 > agi_rate)
			{
				particlePosition = ccpAdd(shootImg->getPosition(),shoot_dv);
				shootImg->setPosition(particlePosition);
				particle->setPosition(particlePosition);
				mainImg->setPosition(particlePosition);
				
				if(shootImg->getScaleX() < 30.f)
					shootImg->setScaleX(shootImg->getScaleX() + shoot_dv_distance);
				shoot_removing = true;
				
				// bomb
				particlePosition = shootImg->getPosition();
				mainImg->removeFromParentAndCleanup(true);
				
				particlePosition.x += rand()%31 - 15;
				particlePosition.y += rand()%31 - 15;
				
				(target_popup->*target_explosion)(particlePosition, int(-shootImg->getRotation()));
				(target_popup->*target_showDamageMissile)(particlePosition, int(damage));
			}
			else
			{
				(target_popup->*target_showMissMissile)(particlePosition, 0);
				
				shoot_removing = true;
				mainImg->removeFromParentAndCleanup(true);
			}
		}
		
		if(load_removing)
		{
			if(loadImg->getScaleX() > 1.f)
			{
				loadImg->setScaleX(loadImg->getScaleX() - 1.f);
				edge->setScaleX(1.f/loadImg->getScaleX());
			}
			else
			{
				loadImg->removeFromParentAndCleanup(true);
				load_removing = false;
			}
		}
		
		if(shoot_removing)
		{
			if(shootImg->getScaleX() > shoot_dv_distance)
				shootImg->setScaleX(shootImg->getScaleX() - shoot_dv_distance);
			else
			{
				shootImg->removeFromParentAndCleanup(true);
				shoot_removing = false;
			}
		}
		
		
		if(ingFrame >= 20 + 10 && !load_removing && !shoot_removing)
		{
			stopMoving();
		}
	}
	else
	{
		stopMoving();
	}
}
void JM_UpgradeMissileShow::myInit (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile)
{
	particlePosition = s_p;
	
	realInit(t_target, jm_type, missile_speed, t_boss_agi, t_popup, t_explosion, t_showDamageMissile, t_showMissMissile);
}
void JM_UpgradeMissileShow::myInit (CCNode * t_target, int jm_type, float missile_speed, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile)
{
	IntPoint jackPoint = myGD->getJackPoint();
	particlePosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
	
	realInit(t_target, jm_type, missile_speed, t_boss_agi, t_popup, t_explosion, t_showDamageMissile, t_showMissMissile);
}
void JM_UpgradeMissileShow::realInit (CCNode * t_target, int jm_type, float missile_speed, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile)
{
	//		myGD->communication("EP_addJackAttack");
	boss_agi = t_boss_agi;
	target_popup = t_popup;
	target_explosion = t_explosion;
	target_showDamageMissile = t_showDamageMissile;
	target_showMissMissile = t_showMissMissile;
	targetNode = t_target;
	my_type = (MyElemental)(jm_type%10);
	
	int element_level;
	
	if(jm_type%10 == kMyElementalPlasma)				type_name = "jm_plasma";
	else if(jm_type%10 == kMyElementalLightning)		type_name = "jm_lightning";
	else if(jm_type%10 == kMyElementalWind)				type_name = "jm_wind";
	
	element_level = jm_type/10 + 1;
	
	int recent_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	if(recent_card_number > 0)
	{
		float base_missile_damage;
//		base_missile_damage = NSDS_GI(kSDS_CI_int1_missile_power_i, recent_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, recent_card_number)-1)*0.1f+1.f);
		
		damage = base_missile_damage; // * damage_per
		
		int base_missile_dex;
//		base_missile_dex = NSDS_GI(kSDS_CI_int1_missile_dex_i, recent_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, recent_card_number)-1)*0.1f+1.f);
		
		dex = base_missile_dex;
		
		damage = damage < 1 ? 1 : damage;
		dex = dex < 1 ? 1 : dex;
	}
	else
	{
		damage = 1;
		dex = 1;
	}
	
	float damage_variation = damage*0.1f;
	
	random_device rd;
	default_random_engine e1(rd());
	uniform_real_distribution<float> uniform_dist(0, damage_variation);
	
	damage += uniform_dist(e1) - damage_variation/2.f;
	if(damage < 1.f)
		damage = 1.f;
	
	load_removing = false;
	shoot_removing = false;
	
	CCPoint subPosition = ccpSub(targetNode->getPosition(), particlePosition);
	float loadAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
	
	float rate = (rand()%11 + 5)/10.f;
	
	if(rand()%2)
		loadAngle += 90.f + 30.f*rate;
	else
		loadAngle -= 90.f + 30.f*rate;
	
	load_dv.x = 1;
	load_dv.y = tanf(loadAngle/180.f*M_PI);
	
	float div_value = sqrtf(powf(load_dv.x, 2.f) + powf(load_dv.y, 2.f));
	load_dv = ccpMult(load_dv, 1.f/div_value);
	load_dv = ccpMult(load_dv, 4.f); // img width 4
	
	if((loadAngle > 90.f && loadAngle < 270.f) || loadAngle < -90.f)
		load_dv = ccpMult(load_dv, -1.f);
	
	string particle_string;
	if(element_level >= 1 && element_level <= 5)			particle_string = type_name + "1_particle.png";
	else													particle_string = type_name + "2_particle.png";
	
	float particle_cnt = 3 + element_level*3;
	
	particle = new CCParticleSystemQuad();
	particle->initWithTotalParticles(particle_cnt);
	particle->setPositionType(kCCPositionTypeRelative);
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(particle_string.c_str());
	particle->setTexture(texture);
	particle->setEmissionRate(particle_cnt*2);
	particle->setAngle(0.0);
	particle->setAngleVar(360.0);
//	ccBlendFunc blendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
	ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
	particle->setBlendFunc(blendFunc);
	particle->setDuration(-1.0);
	particle->setEmitterMode(kCCParticleModeGravity);
	ccColor4F startColor = {1.00,1.00,0.00,1.00}; // 0.76 0.25 0.12
	particle->setStartColor(startColor);
	ccColor4F startColorVar = {0,0,0,0};
	particle->setStartColorVar(startColorVar);
	ccColor4F endColor = {1.00,0.00,0.00,1.00};
	particle->setEndColor(endColor);
	ccColor4F endColorVar = {0,0,0,0};
	particle->setEndColorVar(endColorVar);
	particle->setStartSize(20.0);
	particle->setStartSizeVar(5.0);
	particle->setEndSize(10.0);
	particle->setEndSizeVar(5.0);
	particle->setGravity(ccp(0,0));
	particle->setRadialAccel(0.0);
	particle->setRadialAccelVar(0.0);
	particle->setSpeed(50);
	particle->setSpeedVar(0.0);
	particle->setTangentialAccel(0);
	particle->setTangentialAccelVar(0);
	particle->setTotalParticles(particle_cnt);
	particle->setLife(0.20);
	particle->setLifeVar(0.0);
	particle->setStartSpin(0);
	particle->setStartSpinVar(360);
	particle->setEndSpin(0);
	particle->setEndSpinVar(360);
	particle->autorelease();
	
	particle->setPosition(particlePosition);
	particle->setPosVar(ccp(0,0));
	
	addChild(particle, kZorderJMU_particle);
	
	string temp_string = type_name + ".png";
	
	loadImg = CCSprite::create(temp_string.c_str());
	
	ccBlendFunc t_blend1;
	t_blend1.src = GL_SRC_ALPHA;
	t_blend1.dst = GL_ONE;
	loadImg->setBlendFunc(t_blend1);
	
	loadImg->setAnchorPoint(ccp(1,0.5));
	loadImg->setRotation(-loadAngle);
	loadImg->setPosition(particlePosition);
	addChild(loadImg, kZorderJMU_loadImg);
	
	string main_string = type_name + CCString::createWithFormat("%d_main.png", element_level)->getCString();
	
	mainImg = CCSprite::create(main_string.c_str());
	
	ccBlendFunc t_blend;
	t_blend.src = GL_SRC_ALPHA;
	t_blend.dst = GL_ONE;
	mainImg->setBlendFunc(t_blend);
	
	mainImg->setScale(1.f);
	mainImg->setPosition(particlePosition);
	addChild(mainImg, kZorderJMU_mainImg);
	
	if(element_level >= 8)
	{
		is_spin = true;
		int random_value = rand()%200;
		random_value = (random_value >= 100) ? random_value+100 : random_value-300;
		CCRotateBy* t_rotate = CCRotateBy::create(1.f, random_value);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_rotate);
		mainImg->runAction(t_repeat);
	}
	else
	{
		is_spin = false;
		float t_angle = atan2f(load_dv.y, load_dv.x)/M_PI*180.f-90.f;
		mainImg->setRotation(-t_angle);
	}
}
JM_BasicMissileShow * JM_BasicMissileShow::create (CCNode * t_target, int jm_type, float missile_speed, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile)
{
	JM_BasicMissileShow* t_bm = new JM_BasicMissileShow();
	t_bm->myInit(t_target, jm_type, missile_speed, t_boss_agi, t_popup, t_explosion, t_showDamageMissile, t_showMissMissile);
	t_bm->autorelease();
	return t_bm;
}
JM_BasicMissileShow * JM_BasicMissileShow::create (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile)
{
	JM_BasicMissileShow* t_bm = new JM_BasicMissileShow();
	t_bm->myInit(t_target, jm_type, missile_speed, s_p, t_boss_agi, t_popup, t_explosion, t_showDamageMissile, t_showMissMissile);
	t_bm->autorelease();
	return t_bm;
}
JM_BasicMissileShow::~ JM_BasicMissileShow ()
{
	//		myGD->communication("EP_subJackAttack");
}
void JM_BasicMissileShow::startMoving ()
{
	schedule(schedule_selector(JM_BasicMissileShow::moving));
}
void JM_BasicMissileShow::moving ()
{
	bool isEnable = true;
	
	if(isEnable)
	{
		CCPoint t_p = targetNode->getPosition();
		CCPoint directionVector = ccpSub(t_p, particlePosition);
		float immediatelyDirectionAngle = atan2f(directionVector.y, directionVector.x)/M_PI*180.f;
		float reverseDirectionAngle = immediatelyDirectionAngle + 180.f;
		if(reverseDirectionAngle >= 180.f)
		{
			reverseDirectionAngle -= 360.f;
		}
		
		float changeDirection;
		
		if(immediatelyDirectionAngle >= 0.f)
		{
			if(directionAngle >= 0.f)
			{
				if(directionAngle >= immediatelyDirectionAngle)
				{
					changeDirection = -1*myJM_CHANGE_DIRECTION_VAL;
					if(directionAngle + changeDirection >= immediatelyDirectionAngle)
					{
						directionAngle += changeDirection;
					}
					else
					{
						directionAngle = immediatelyDirectionAngle;
					}
				}
				else
				{
					changeDirection = myJM_CHANGE_DIRECTION_VAL;
					if(directionAngle + changeDirection <= immediatelyDirectionAngle)
					{
						directionAngle += changeDirection;
					}
					else
					{
						directionAngle = immediatelyDirectionAngle;
					}
				}
			}
			else
			{
				if(directionAngle >= reverseDirectionAngle)
				{
					changeDirection = myJM_CHANGE_DIRECTION_VAL;
					if(directionAngle + changeDirection <= immediatelyDirectionAngle)
					{
						directionAngle += changeDirection;
					}
					else
					{
						directionAngle = immediatelyDirectionAngle;
					}
				}
				else
				{
					changeDirection = -1*myJM_CHANGE_DIRECTION_VAL;
					float tempDirection = directionAngle + changeDirection;
					if(tempDirection < -180.f)
					{
						tempDirection += 360.f;
						if(tempDirection >= immediatelyDirectionAngle)
						{
							directionAngle = tempDirection;
						}
						else
						{
							directionAngle = immediatelyDirectionAngle;
						}
					}
					else
					{
						directionAngle = tempDirection;
					}
				}
			}
		}
		else
		{
			if(directionAngle >= 0.f)
			{
				if(directionAngle >= reverseDirectionAngle)
				{
					changeDirection = myJM_CHANGE_DIRECTION_VAL;
					float tempDirection = directionAngle + changeDirection;
					if(tempDirection >= 180.f)
					{
						tempDirection -= 360.f;
						if(tempDirection <= immediatelyDirectionAngle)
						{
							directionAngle = tempDirection;
						}
						else
						{
							directionAngle = immediatelyDirectionAngle;
						}
					}
					else
					{
						directionAngle = tempDirection;
					}
				}
				else
				{
					changeDirection = -1*myJM_CHANGE_DIRECTION_VAL;
					if(directionAngle + changeDirection >= immediatelyDirectionAngle)
					{
						directionAngle += changeDirection;
					}
					else
					{
						directionAngle = immediatelyDirectionAngle;
					}
				}
			}
			else
			{
				if(directionAngle >= immediatelyDirectionAngle)
				{
					changeDirection = -1*myJM_CHANGE_DIRECTION_VAL;
					if(directionAngle + changeDirection <= immediatelyDirectionAngle)
					{
						directionAngle += changeDirection;
					}
					else
					{
						directionAngle = immediatelyDirectionAngle;
					}
				}
				else
				{
					changeDirection = myJM_CHANGE_DIRECTION_VAL;
					if(directionAngle + changeDirection >= immediatelyDirectionAngle)
					{
						directionAngle += changeDirection;
					}
					else
					{
						directionAngle = immediatelyDirectionAngle;
					}
				}
			}
		}
		
		CCPoint moveVector;
		moveVector.x = 1.f;
		moveVector.y = tanf(directionAngle/180.f*M_PI);
		
		float t_distance = 1.f/sqrtf(powf(moveVector.x, 2.f) + powf(moveVector.y, 2.f));
		moveVector = ccpMult(moveVector, t_distance);
		
		if((directionAngle >= 90.f && directionAngle < 270.f) || (directionAngle < -90.f && directionAngle >= -270.f))
		{
			moveVector = ccpMult(moveVector, -1.f);
		}
		
		
		float recentDistance = sqrtf(powf(directionVector.x, 2.f) + powf(directionVector.y, 2.f));
		
		if(recentDistance < myJM_SPEED)
		{
			moveVector = ccpMult(moveVector, recentDistance);
		}
		else
		{
			moveVector = ccpMult(moveVector, myJM_SPEED);
		}
		
		particlePosition = ccpAdd(particlePosition, moveVector);
		
		mainImg->setPosition(particlePosition);
		streak_img->setPosition(particlePosition);
		
		if(is_spin)				mainImg->setRotation(mainImg->getRotation()-6);
		else					mainImg->setRotation((mainImg->getRotation()-(directionAngle-90))/2.f);
		
		particle->setPosition(particlePosition);
		
		CCPoint subDistance = ccpSub(t_p, particlePosition);
		float distance = sqrtf(powf(subDistance.x, 2.f) + powf(subDistance.y, 2.f));
		if(distance <= 1)
		{
			unschedule(schedule_selector(JM_BasicMissileShow::moving));
			
			float target_agi = boss_agi;
			
			float agi_rate = (target_agi-dex)/target_agi;
			agi_rate = agi_rate < 0 ? 0 : agi_rate;
			
			agi_rate = agi_rate/100.f*85.f + 0.1f;
			agi_rate *= 100.f;
			
			if(rand()%100 > agi_rate)
			{
				particlePosition.x += rand()%31 - 15;
				particlePosition.y += rand()%31 - 15;
				
				(target_popup->*target_explosion)(particlePosition, directionAngle);
				(target_popup->*target_showDamageMissile)(particlePosition, int(damage));
				
				removeFromParentAndCleanup(true);
			}
			else
			{
				(target_popup->*target_showMissMissile)(particlePosition, 0);
				
				int random_angle = directionAngle + rand()%31 - 15;
				
				if(is_spin)				mainImg->setRotation(mainImg->getRotation()-6);
				else					mainImg->setRotation((mainImg->getRotation()-(random_angle-90))/2.f);
				
				CCPoint miss_position;
				miss_position.x = 1.f;
				miss_position.y = tanf(random_angle/180.f*M_PI);
				
				if(random_angle >= 90 && random_angle <= 270)
					miss_position = ccpMult(miss_position, -1.f);
				
				miss_position = ccpMult(miss_position, 10.f*myJM_SPEED/sqrtf(powf(miss_position.x, 2.f) + powf(miss_position.y, 2.f)));
				
				CCMoveBy* move2 = CCMoveBy::create(10.f/60.f, miss_position);
				CCCallFunc* call2 = CCCallFunc::create(this, callfunc_selector(JM_BasicMissileShow::removeFromParent));
				CCSequence* t_seq2 = CCSequence::createWithTwoActions(move2, call2);
				mainImg->runAction(t_seq2);
				
				CCMoveBy* move1 = CCMoveBy::create(10.f/60.f, miss_position);
				particle->runAction(move1);
			}
		}
		if(ing_miss_counting < 0)
		{
			if(distance <= 5)
				ing_miss_counting = 30;
		}
		else
		{
			ing_miss_counting--;
			
			if(ing_miss_counting == 0)
			{
				unschedule(schedule_selector(JM_BasicMissileShow::moving));
				
				(target_popup->*target_showMissMissile)(particlePosition, 0);
				
				int random_angle = directionAngle + rand()%21 - 10;
				
				if(is_spin)				mainImg->setRotation(mainImg->getRotation()-6);
				else					mainImg->setRotation((mainImg->getRotation()-(random_angle-90))/2.f);
				
				CCPoint miss_position;
				miss_position.x = 1.f;
				miss_position.y = tanf(random_angle/180.f*M_PI);
				
				if(random_angle >= 90 && random_angle <= 270)
					miss_position = ccpMult(miss_position, -1.f);
				
				miss_position = ccpMult(miss_position, 10.f*myJM_SPEED/sqrtf(powf(miss_position.x, 2.f) + powf(miss_position.y, 2.f)));
				
				CCMoveBy* move2 = CCMoveBy::create(10.f/60.f, miss_position);
				CCCallFunc* call2 = CCCallFunc::create(this, callfunc_selector(JM_BasicMissileShow::removeFromParent));
				CCSequence* t_seq2 = CCSequence::createWithTwoActions(move2, call2);
				mainImg->runAction(t_seq2);
				
				CCMoveBy* move1 = CCMoveBy::create(10.f/60.f, miss_position);
				particle->runAction(move1);
			}
		}
	}
	else
	{
		unschedule(schedule_selector(JM_BasicMissileShow::moving));
		removeFromParentAndCleanup(true);
	}
}
void JM_BasicMissileShow::myInit (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile)
{
	setStartPosition(s_p);
	realInit(t_target, jm_type, missile_speed, t_boss_agi, t_popup, t_explosion, t_showDamageMissile, t_showMissMissile);
}
void JM_BasicMissileShow::setStartPosition (CCPoint s_p)
{
	particlePosition = s_p;
}
void JM_BasicMissileShow::myInit (CCNode * t_target, int jm_type, float missile_speed, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile)
{
	IntPoint jackPoint = myGD->getJackPoint();
	setStartPosition(ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1));
	realInit(t_target, jm_type, missile_speed, t_boss_agi, t_popup, t_explosion, t_showDamageMissile, t_showMissMissile);
}
void JM_BasicMissileShow::realInit (CCNode * t_target, int jm_type, float missile_speed, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile)
{
	boss_agi = t_boss_agi;
	target_popup = t_popup;
	target_explosion = t_explosion;
	target_showDamageMissile = t_showDamageMissile;
	target_showMissMissile = t_showMissMissile;
	ing_miss_counting = -1;
	targetNode = t_target;
	//	particle = new CCParticleSystemQuad();
	
	my_type = (MyElemental)jm_type;
	
	string type_name;
	int element_level;
	
	if(jm_type%10 == kMyElementalNonElemental)			type_name = "jm_empty";
	else if(jm_type%10 == kMyElementalFire)				type_name = "jm_fire";
	else if(jm_type%10 == kMyElementalWater)			type_name = "jm_water";
	else if(jm_type%10 == kMyElementalLife)				type_name = "jm_life";
	
	element_level = jm_type/10 + 1;
	
	jm_type = jm_type%10;
	
//	float particle_cnt = 3 + element_level*3;
	string particle_string;
	
	particle_string = type_name + ".png";
	
	int recent_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	if(recent_card_number > 0)
	{
		float base_missile_damage;
//		base_missile_damage = NSDS_GI(kSDS_CI_int1_missile_power_i, recent_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, recent_card_number)-1)*0.1f+1.f);
		
		damage = base_missile_damage; // * damage_per
		
		int base_missile_dex;
//		base_missile_dex = NSDS_GI(kSDS_CI_int1_missile_dex_i, recent_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, recent_card_number)-1)*0.1f+1.f);
		
		dex = base_missile_dex;
		
		damage = damage < 1 ? 1 : damage;
		dex = dex < 1 ? 1 : dex;
	}
	else
	{
		damage = 1;
		dex = 1;
	}
	
	float damage_variation = damage*0.1f;
	
	random_device rd;
	default_random_engine e1(rd());
	uniform_real_distribution<float> uniform_dist(0, damage_variation);
	
	damage += uniform_dist(e1) - damage_variation/2.f;
	if(damage < 1.f)
		damage = 1.f;
	
	
	if(missile_speed < 2.f)
		missile_speed = 2.f;
	else if(missile_speed > 9.f)
		missile_speed = 9.f;
	
	myJM_SPEED = missile_speed * ((rand()%11 - 5)/10.f + 1.f);
	myJM_CHANGE_DIRECTION_VAL = JM_CHANGE_DIRECTION_VAL;
	
	string particle_filename;
	if(jm_type == kElementCode_empty)
		particle_filename = "jm_particle%d_empty.plist";
	else if(jm_type == kElementCode_fire)
		particle_filename = "jm_particle%d_fire.plist";
	else if(jm_type == kElementCode_life)
		particle_filename = "jm_particle%d_life.plist";
	else if(jm_type == kElementCode_water)
		particle_filename = "jm_particle%d_water.plist";
	
	particle = CCParticleSystemQuad::create(CCString::createWithFormat(particle_filename.c_str(), (element_level-1)/2+1)->getCString());
	particle->setPositionType(kCCPositionTypeRelative);
	//	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(particle_string.c_str());
	//	particle->setTexture(texture);
	//	particle->setEmissionRate(particle_cnt);
	//	particle->setAngle(0.0);
	//	particle->setAngleVar(360.0);
	//	ccBlendFunc blendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
	//	particle->setBlendFunc(blendFunc);
	//	particle->setDuration(-1.0);
	//	particle->setEmitterMode(kCCParticleModeGravity);
	//	ccColor4F startColor = {1.00,1.00,1.00,1.00}; // 0.76 0.25 0.12
	//	particle->setStartColor(startColor);
	//	ccColor4F startColorVar = {0,0,0,0};
	//	particle->setStartColorVar(startColorVar);
	//	ccColor4F endColor = {1.00,1.00,1.00,1.00};
	//	particle->setEndColor(endColor);
	//	ccColor4F endColorVar = {0,0,0,0};
	//	particle->setEndColorVar(endColorVar);
	particle->setStartSize(particle->getStartSize());
	particle->setStartSizeVar(particle->getStartSizeVar());
	//	particle->setEndSize(5.0);
	//	particle->setEndSizeVar(0.0);
	//	particle->setGravity(ccp(0,0));
	//	particle->setRadialAccel(0.0);
	//	particle->setRadialAccelVar(0.0);
	particle->setSpeed(particle->getSpeed());
	particle->setSpeedVar(particle->getSpeedVar());
	//	particle->setTangentialAccel(0);
	//	particle->setTangentialAccelVar(0);
	//	particle->setTotalParticles(particle_cnt);
	//	particle->setLife(0.3);
	//	particle->setLifeVar(0.0);
	//	particle->setStartSpin(0.0);
	//	particle->setStartSpinVar(360.f);
	//	particle->setEndSpin(0.0);
	//	particle->setEndSpinVar(360.f);
	particle->setPosition(particlePosition);
	//	particle->setPosVar(ccp(0,0));
	
	CCPoint t_p = targetNode->getPosition();
	
	CCPoint directionVector = ccpSub(t_p, particlePosition);
	
	directionAngle = atan2f(directionVector.y, directionVector.x)/M_PI*180.f;
	directionAngle += 180.f + rand()%31 - 15.f;
	if(directionAngle >= 180.f)
		directionAngle -= 360.f;
	
	streak_img = ASMotionStreak::create(0.4f, 2, 12, ccWHITE, "streak_temp.png");
	streak_img->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	streak_img->setPosition(particlePosition);
	addChild(streak_img);
	
	addChild(particle);
	
	int animation_cnt = 0;
	
	if(jm_type == kElementCode_life)
	{
		if(element_level == 8)
			animation_cnt = 3;
		else
			animation_cnt = 2;
	}
	else if(jm_type == kElementCode_fire)
	{
		animation_cnt = 2;
	}
	else if(jm_type == kElementCode_water)
	{
		if(element_level == 6 || element_level == 9)
			animation_cnt = 4;
		else if(element_level == 8)
			animation_cnt = 6;
		else
			animation_cnt = 3;
	}
	else if(jm_type == kElementCode_empty)
	{
		if(element_level == 1)
			animation_cnt = 2;
		else if(element_level == 4)
			animation_cnt = 4;
		else
			animation_cnt = 3;
	}
	
	CCTexture2D* missile_main_texture = CCTextureCache::sharedTextureCache()->addImage(CCString::createWithFormat((type_name + "%d_main.png").c_str(), element_level)->getCString());
	
	CCSize animation_cut_size = CCSizeMake(missile_main_texture->getContentSize().width/animation_cnt, missile_main_texture->getContentSize().height);
	
	mainImg = CCSprite::createWithTexture(missile_main_texture, CCRectMake(0, 0, animation_cut_size.width, animation_cut_size.height));
	
	ccBlendFunc t_blend;
	t_blend.src = GL_SRC_ALPHA;
	t_blend.dst = GL_ONE;
	mainImg->setBlendFunc(t_blend);
	
	mainImg->setPosition(particlePosition);
	mainImg->setScale(1.f);
	addChild(mainImg);
	
	CCAnimation* main_animation = CCAnimation::create();
	main_animation->setDelayPerUnit(0.1f);
	for(int i=0;i<animation_cnt;i++)
		main_animation->addSpriteFrameWithTexture(missile_main_texture, CCRectMake(i*animation_cut_size.width, 0, animation_cut_size.width, animation_cut_size.height));
	
	CCAnimate* main_animate = CCAnimate::create(main_animation);
	CCRepeatForever* main_repeat = CCRepeatForever::create(main_animate);
	
	mainImg->runAction(main_repeat);
	
	is_spin = false;
}

UM_creatorShow* UM_creatorShow::create( int t_um_tcnt, int t_create_type, float t_missile_speed, CCNode* t_target, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile )
{
	UM_creatorShow* t_c = new UM_creatorShow();
	t_c->myInit(t_um_tcnt, t_create_type, t_missile_speed, t_target, t_boss_agi, t_popup, t_explosion, t_showDamageMissile, t_showMissMissile);
	t_c->autorelease();
	return t_c;
}

UM_creatorShow* UM_creatorShow::create( int t_um_tcnt, int t_create_type, float t_missile_speed, CCPoint s_p, CCNode* t_target, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile )
{
	UM_creatorShow* t_c = new UM_creatorShow();
	t_c->myInit(t_um_tcnt, t_create_type, t_missile_speed, s_p, t_target, t_boss_agi, t_popup, t_explosion, t_showDamageMissile, t_showMissMissile);
	t_c->autorelease();
	return t_c;
}

void UM_creatorShow::startCreate()
{
	ing_frame = 0;
	ing_um_cnt = 0;
	schedule(schedule_selector(UM_creatorShow::creating));
}

void UM_creatorShow::creating()
{
	ing_frame++;
	
	if(ing_frame%shoot_frame == 0)
	{
		JackMissileShow* t_jm = JM_UpgradeMissileShow::create(target_node, create_type, missile_speed, start_position, boss_agi, target_popup, target_explosion, target_showDamageMissile, target_showMissMissile);// ::create(target_node, create_type, missile_speed, start_position, boss_agi, target_popup, target_explosion, target_showDamageMissile, target_showMissMissile);
		getParent()->addChild(t_jm, getZOrder());
		t_jm->startMoving();
		
		ing_um_cnt++;
	}
	
	if(ing_um_cnt >= um_tcnt)
	{
		stopCreate();
	}
}

void UM_creatorShow::stopCreate()
{
	unschedule(schedule_selector(UM_creatorShow::creating));
	removeFromParentAndCleanup(true);
}

void UM_creatorShow::myInit(int t_um_tcnt, int t_create_type, float t_missile_speed, CCNode *t_target, float t_boss_agi, CCObject *t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile)
{
	target_node = t_target;
	boss_agi = t_boss_agi;
	target_popup = t_popup;
	target_explosion = t_explosion;
	target_showDamageMissile = t_showDamageMissile;
	target_showMissMissile = t_showMissMissile;
	
	if(t_missile_speed < 2.f)
		missile_speed = 2.f;
	else if(t_missile_speed > 9.f)
		missile_speed = 9.f;
	else
		missile_speed = t_missile_speed;
	shoot_frame = missile_speed*3;
	um_tcnt = t_um_tcnt;
	create_type = t_create_type;
}

void UM_creatorShow::myInit(int t_um_tcnt, int t_create_type, float t_missile_speed, CCPoint s_p, CCNode *t_target, float t_boss_agi, CCObject *t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile)
{
	start_position = s_p;
	myInit(t_um_tcnt, t_create_type, t_missile_speed, t_target, t_boss_agi, t_popup, t_explosion, t_showDamageMissile, t_showMissMissile);
}