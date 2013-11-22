// JackMissile.cpp
//

#include "JackMissile.h"
#include "AudioEngine.h"
#include "KSCumberBase.h"
#define LZZ_INLINE inline
using namespace cocos2d;
using namespace std;
void JackMissile::startMoving ()
        {
		
	}
JM_UpgradeMissile * JM_UpgradeMissile::create (CCNode * t_target, int jm_type, float missile_speed)
        {
		JM_UpgradeMissile* t_um = new JM_UpgradeMissile();
		t_um->myInit(t_target, jm_type, missile_speed);
		t_um->autorelease();
		return t_um;
	}
JM_UpgradeMissile * JM_UpgradeMissile::create (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p)
        {
		JM_UpgradeMissile* t_um = new JM_UpgradeMissile();
		t_um->myInit(t_target, jm_type, missile_speed, s_p);
		t_um->autorelease();
		return t_um;
	}
JM_UpgradeMissile::~ JM_UpgradeMissile ()
        {
//		myGD->communication("EP_subJackAttack");
	}
void JM_UpgradeMissile::startMoving ()
        {
		ingFrame = 0;
		schedule(schedule_selector(JM_UpgradeMissile::moving));
	}
void JM_UpgradeMissile::stopMoving ()
        {
		unschedule(schedule_selector(JM_UpgradeMissile::moving));
		removeFromParentAndCleanup(true);
	}
void JM_UpgradeMissile::moving ()
        {
		bool isEnable = false;
		
		if(targetNode == myGD->getCommunicationNode("CP_getMainCumberPointer"))
			isEnable = true;
		
		if(!isEnable)
		{
			CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
			for(int i=0;i<subCumberArray->count();i++)
			{
				if(targetNode == subCumberArray->objectAtIndex(i))
				{
					isEnable = true;
					break;
				}
			}
		}
		
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
				float target_agi = ((KSCumberBase*)targetNode)->getAgility();
				
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
					
					particlePosition.x += rand()%21 - 10;
					particlePosition.y += rand()%21 - 10;
					
					myGD->communication("MP_explosion", particlePosition, ccc4f(0, 0, 0, 0), -shootImg->getRotation());
					myGD->communication("MP_bombCumber", (CCObject*)targetNode); // with startMoving
					myGD->communication("CP_startDamageReaction", targetNode, damage, -shootImg->getRotation());
					
					myGD->communication("Main_showDamageMissile", particlePosition, int(damage));
					
					int combo_cnt = myGD->getCommunication("UI_getComboCnt");
					combo_cnt++;
					
					int addScore = (100.f + damage)*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d)*combo_cnt;
					
					myGD->communication("Main_startShake", -shootImg->getRotation());
					
					myGD->communication("UI_addScore", addScore);
					myGD->communication("UI_setComboCnt", combo_cnt);
				}
				else
				{
					myGD->communication("Main_showMissMissile", particlePosition);
					
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
void JM_UpgradeMissile::myInit (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p)
        {
		particlePosition = s_p;
		
		realInit(t_target, jm_type, missile_speed);
	}
void JM_UpgradeMissile::myInit (CCNode * t_target, int jm_type, float missile_speed)
        {
		IntPoint jackPoint = myGD->getJackPoint();
		particlePosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
		
		realInit(t_target, jm_type, missile_speed);
	}
void JM_UpgradeMissile::realInit (CCNode * t_target, int jm_type, float missile_speed)
        {
//		myGD->communication("EP_addJackAttack");
		
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
			damage = NSDS_GI(kSDS_CI_int1_missile_power_i, recent_card_number); // * damage_per
			dex = NSDS_GI(kSDS_CI_int1_missile_dex_i, recent_card_number);
			
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
		ccBlendFunc blendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
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
		loadImg->setAnchorPoint(ccp(1,0.5));
		loadImg->setRotation(-loadAngle);
		loadImg->setPosition(particlePosition);
		addChild(loadImg, kZorderJMU_loadImg);
		
		string main_string = type_name + CCString::createWithFormat("%d_main.png", element_level)->getCString();
		
		mainImg = CCSprite::create(main_string.c_str());
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
JM_BasicMissile * JM_BasicMissile::create (CCNode * t_target, int jm_type, float missile_speed)
        {
		JM_BasicMissile* t_bm = new JM_BasicMissile();
		t_bm->myInit(t_target, jm_type, missile_speed);
		t_bm->autorelease();
		return t_bm;
	}
JM_BasicMissile * JM_BasicMissile::create (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p)
        {
		JM_BasicMissile* t_bm = new JM_BasicMissile();
		t_bm->myInit(t_target, jm_type, missile_speed, s_p);
		t_bm->autorelease();
		return t_bm;
	}
JM_BasicMissile::~ JM_BasicMissile ()
        {
//		myGD->communication("EP_subJackAttack");
	}
void JM_BasicMissile::startMoving ()
        {
		schedule(schedule_selector(JM_BasicMissile::moving));
	}
void JM_BasicMissile::moving ()
        {
		bool isEnable = false;
		
		if(targetNode == myGD->getCommunicationNode("CP_getMainCumberPointer"))
			isEnable = true;
		
		if(!isEnable && !myGD->getIsGameover())
		{
			CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
			for(int i=0;i<subCumberArray->count();i++)
			{
				if(targetNode == subCumberArray->objectAtIndex(i))
				{
					isEnable = true;
					break;
				}
			}
		}
		
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
			
			if(is_spin)				mainImg->setRotation(mainImg->getRotation()-6);
			else					mainImg->setRotation((mainImg->getRotation()-(directionAngle-90))/2.f);
			
			particle->setPosition(particlePosition);
			
			CCPoint subDistance = ccpSub(t_p, particlePosition);
			float distance = sqrtf(powf(subDistance.x, 2.f) + powf(subDistance.y, 2.f));
			if(distance <= 1)
			{
				unschedule(schedule_selector(JM_BasicMissile::moving));
				
				float target_agi = ((KSCumberBase*)targetNode)->getAgility();
				
				float agi_rate = (target_agi-dex)/target_agi;
				agi_rate = agi_rate < 0 ? 0 : agi_rate;
				
				agi_rate = agi_rate/100.f*85.f + 0.1f;
				agi_rate *= 100.f;
				
				if(rand()%100 > agi_rate)
				{
					particlePosition.x += rand()%21 - 10;
					particlePosition.y += rand()%21 - 10;
					
					myGD->communication("MP_explosion", particlePosition, ccc4f(0, 0, 0, 0), directionAngle);
					myGD->communication("MP_bombCumber", (CCObject*)targetNode); // with startMoving
					myGD->communication("CP_startDamageReaction", targetNode, damage, directionAngle);
					
					myGD->communication("Main_showDamageMissile", particlePosition, int(damage));
					
					int combo_cnt = myGD->getCommunication("UI_getComboCnt");
					combo_cnt++;
					
					int addScore = (100.f+damage)*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d)*combo_cnt;
					myGD->communication("UI_addScore", addScore);
					myGD->communication("UI_setComboCnt", combo_cnt);
					
					myGD->communication("Main_startShake", directionAngle);
					
					removeFromParentAndCleanup(true);
				}
				else
				{
					myGD->communication("Main_showMissMissile", particlePosition);
					
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
					mainImg->runAction(move2);
					
					CCMoveBy* move1 = CCMoveBy::create(10.f/60.f, miss_position);
					CCCallFunc* call1 = CCCallFunc::create(this, callfunc_selector(JM_BasicMissile::removeFromParent));
					CCSequence* t_seq = CCSequence::createWithTwoActions(move1, call1);
					
					particle->runAction(t_seq);
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
					unschedule(schedule_selector(JM_BasicMissile::moving));
					
					myGD->communication("Main_showMissMissile", particlePosition);
					
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
					mainImg->runAction(move2);
					
					CCMoveBy* move1 = CCMoveBy::create(10.f/60.f, miss_position);
					CCCallFunc* call1 = CCCallFunc::create(this, callfunc_selector(JM_BasicMissile::removeFromParent));
					CCSequence* t_seq = CCSequence::createWithTwoActions(move1, call1);
					
					particle->runAction(t_seq);
				}
			}
		}
		else
		{
			unschedule(schedule_selector(JM_BasicMissile::moving));
			removeFromParentAndCleanup(true);
		}
	}
void JM_BasicMissile::myInit (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p)
        {
		setStartPosition(s_p);
		realInit(t_target, jm_type, missile_speed);
	}
void JM_BasicMissile::setStartPosition (CCPoint s_p)
        {
		particlePosition = s_p;
	}
void JM_BasicMissile::myInit (CCNode * t_target, int jm_type, float missile_speed)
        {
		IntPoint jackPoint = myGD->getJackPoint();
		setStartPosition(ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1));
		realInit(t_target, jm_type, missile_speed);
	}
void JM_BasicMissile::realInit (CCNode * t_target, int jm_type, float missile_speed)
        {
		ing_miss_counting = -1;
		targetNode = t_target;
		particle = new CCParticleSystemQuad();
		
		my_type = (MyElemental)jm_type;
		
		string type_name;
		int element_level;
		
		if(jm_type%10 == kMyElementalNonElemental)			type_name = "jm_empty";
		else if(jm_type%10 == kMyElementalFire)				type_name = "jm_fire";
		else if(jm_type%10 == kMyElementalWater)			type_name = "jm_water";
		else if(jm_type%10 == kMyElementalLife)				type_name = "jm_life";
		
		element_level = jm_type/10 + 1;
		
		jm_type = jm_type%10;
		
		float particle_cnt = 3 + element_level*3;
		string particle_string;
		
		particle_string = type_name + ".png";
		
		int recent_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		if(recent_card_number > 0)
		{
			damage = NSDS_GI(kSDS_CI_int1_missile_power_i, recent_card_number); // * damage_per
			dex = NSDS_GI(kSDS_CI_int1_missile_dex_i, recent_card_number);
			
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
		
		particle->initWithTotalParticles(particle_cnt);
		particle->setPositionType(kCCPositionTypeRelative);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(particle_string.c_str());
		particle->setTexture(texture);
		particle->setEmissionRate(particle_cnt);
		particle->setAngle(0.0);
		particle->setAngleVar(360.0);
		ccBlendFunc blendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
		particle->setBlendFunc(blendFunc);
		particle->setDuration(-1.0);
		particle->setEmitterMode(kCCParticleModeGravity);
		ccColor4F startColor = {1.00,1.00,1.00,1.00}; // 0.76 0.25 0.12
		particle->setStartColor(startColor);
		ccColor4F startColorVar = {0,0,0,0};
		particle->setStartColorVar(startColorVar);
		ccColor4F endColor = {1.00,1.00,1.00,1.00};
		particle->setEndColor(endColor);
		ccColor4F endColorVar = {0,0,0,0};
		particle->setEndColorVar(endColorVar);
		particle->setStartSize(5.0);
		particle->setStartSizeVar(5.0);
		particle->setEndSize(5.0);
		particle->setEndSizeVar(0.0);
		particle->setGravity(ccp(0,0));
		particle->setRadialAccel(0.0);
		particle->setRadialAccelVar(0.0);
		particle->setSpeed(20);
		particle->setSpeedVar(0.0);
		particle->setTangentialAccel(0);
		particle->setTangentialAccelVar(0);
		particle->setTotalParticles(particle_cnt);
		particle->setLife(0.3);
		particle->setLifeVar(0.0);
		particle->setStartSpin(0.0);
		particle->setStartSpinVar(360.f);
		particle->setEndSpin(0.0);
		particle->setEndSpinVar(360.f);
		
		particle->autorelease();
		
		particle->setPosition(particlePosition);
		particle->setPosVar(ccp(0,0));
		
		CCPoint t_p = targetNode->getPosition();
		
		CCPoint directionVector = ccpSub(t_p, particlePosition);
		
		directionAngle = atan2f(directionVector.y, directionVector.x)/M_PI*180.f;
		directionAngle += 180.f + rand()%31 - 15.f;
		if(directionAngle >= 180.f)
			directionAngle -= 360.f;
		addChild(particle);
		
		if(jm_type == kMyElementalLife && element_level >= 8)
		{
			if(element_level == 8)
			{
				CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage(CCString::createWithFormat((type_name + "%d_main.png").c_str(), element_level)->getCString());
				
				mainImg = CCSprite::createWithTexture(t_texture, CCRectMake(0, 0, 27, 27));
				mainImg->setPosition(particlePosition);
				addChild(mainImg);
				
				
				CCAnimation* t_animation = CCAnimation::create();
				t_animation->setDelayPerUnit(0.1);
				t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(0, 0, 27, 27));
				t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(27, 0, 27, 27));
				
				CCAnimate* t_animate = CCAnimate::create(t_animation);
				CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
				
				mainImg->runAction(t_repeat);
			}
			else if(element_level == 9)
			{
				CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage(CCString::createWithFormat((type_name + "%d_main.png").c_str(), element_level)->getCString());
				
				mainImg = CCSprite::createWithTexture(t_texture, CCRectMake(0, 0, 37, 38));
				mainImg->setPosition(particlePosition);
				addChild(mainImg);
				
				
				CCAnimation* t_animation = CCAnimation::create();
				t_animation->setDelayPerUnit(0.1);
				t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(0, 0, 37, 38));
				t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(37, 0, 37, 38));
				
				CCAnimate* t_animate = CCAnimate::create(t_animation);
				CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
				
				mainImg->runAction(t_repeat);
			}
		}
		else
		{
			mainImg = CCSprite::create(CCString::createWithFormat((type_name + "%d_main.png").c_str(), element_level)->getCString());
			mainImg->setPosition(particlePosition);
			addChild(mainImg);
		}
		
		is_spin = false;
		
		if(jm_type == kMyElementalNonElemental && (element_level == 2 || element_level == 3 || element_level == 5 || element_level == 8 || element_level == 9))		is_spin = true;
		else if(jm_type == kMyElementalLife && element_level >= 2 && element_level <= 6)									is_spin = true;
		else if(jm_type == kMyElementalFire && element_level >= 2 && element_level <= 7)									is_spin = true;
		else if(jm_type == kMyElementalWater && (element_level == 2 || element_level == 4))									is_spin = true;
	}
#undef LZZ_INLINE
