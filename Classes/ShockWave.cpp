// ShockWave.cpp
//

#include "ShockWave.h"
#include "ServerDataSave.h"
#include "StarGoldData.h"

#define LZZ_INLINE inline
ShockWave * ShockWave::create (IntPoint t_createPoint)
{
	ShockWave* t_sw = new ShockWave();
	t_sw->myInit(t_createPoint);
	t_sw->autorelease();
	return t_sw;
}
void ShockWave::stopSW ()
{
	unschedule(schedule_selector(ShockWave::ingSW));
	if(!is_removing)
	{
		AudioEngine::sharedInstance()->stopEffect("se_energywave.mp3");
//		AudioEngine::sharedInstance()->stopEffect("sound_bomb_wave.mp3");
		is_removing = true;
		ing_frame = 0;
		schedule(schedule_selector(ShockWave::removeProcess));
	}
}
void ShockWave::removeProcess ()
{
	ing_frame++;
	
	CCArray* my_child = getChildren();
	
	for(int i=0;i<getChildrenCount();i++)
	{
		CCSprite* t_child = (CCSprite*)my_child->objectAtIndex(i);
		t_child->setOpacity(t_child->getOpacity()-7);
	}
	
	if(ing_frame > 35)
	{
		unschedule(schedule_selector(ShockWave::removeProcess));
		AudioEngine::sharedInstance()->stopEffect("se_energywave.mp3");
//		AudioEngine::sharedInstance()->stopEffect("sound_bomb_wave.mp3");
		removeFromParentAndCleanup(true);
	}
}
void ShockWave::startSW ()
{
	ing_frame = 0;
	schedule(schedule_selector(ShockWave::ingSW));
}
void ShockWave::ingSW ()
{
	if(ing_frame%15 == 0 && getChildrenCount() < 3)
	{
		CCSprite* t_sw = CCSprite::create("shock_wave.png");
		t_sw->setColor(ccc3(200, 200, 200));
		t_sw->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		t_sw->setScale(0);
		addChild(t_sw);
	}
	CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
	float spread_speed_value = 0.03f - 0.03f*NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_monsterWave_d, t_history.characterIndex.getV(), t_history.characterLevel.getV())/100.f;
	
	if(myGD->getCommunicationBool("UI_isExchanged"))
		spread_speed_value *= 2.f;
	
	CCArray* my_child = getChildren();
	
	for(int i=0;i<getChildrenCount();i++)
	{
		CCSprite* t_child = (CCSprite*)my_child->objectAtIndex(i);
		t_child->setScale(t_child->getScale()+spread_speed_value);
	}
	radius += 80.f*spread_speed_value;
	
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
	
	CCPoint subPosition = ccpSub(getPosition(), jackPosition);
	float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
	
	if(radius > distance)
	{
		//			if(emotion_target && emotion_delegate)
		//				(emotion_target->*emotion_delegate)(6);
		myGD->communication("Jack_startDieEffect", DieType::kDieType_shockwave);
		myGD->communication("MP_resetTickingTimeBomb");
		stopSW();
	}
	
	ing_frame++;
}
void ShockWave::myInit (IntPoint t_createPoint)
{
	//		emotion_target = t_emotion;
	//		emotion_delegate = d_emotion;
	is_removing = false;
	
	CCSprite* texture_spr = CCSprite::create("shock_wave.png");
	initWithTexture(texture_spr->getTexture(), kDefaultSpriteBatchCapacity);
	radius = 0;
	setPosition(ccp((t_createPoint.x-1)*pixelSize+1,(t_createPoint.y-1)*pixelSize+1));
	setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	startSW();
}
SW_Parent * SW_Parent::create ()
{
	SW_Parent* t_mySW = new SW_Parent();
	t_mySW->myInit();
	t_mySW->autorelease();
	return t_mySW;
}
void SW_Parent::createSW (IntPoint t_create_point)
{
	if(getChildrenCount() == 0)
	{
		AudioEngine::sharedInstance()->playEffect("se_energywave.mp3", true);
//		AudioEngine::sharedInstance()->playEffect("sound_bomb_wave.mp3", true);
		ShockWave* t_sw = ShockWave::create(t_create_point);
		addChild(t_sw);
	}
}
void SW_Parent::createJDSW (IntPoint t_create_point)
{
	is_justDie = true;
	createSW(t_create_point);
}
void SW_Parent::stopAllSW ()
{
	if(!is_justDie)
	{
		CCArray* my_child = getChildren();
		AudioEngine::sharedInstance()->stopEffect("se_energywave.mp3");
//		AudioEngine::sharedInstance()->stopEffect("sound_bomb_wave.mp3");
		for(int i=0;i<getChildrenCount();i++)
		{
			ShockWave* t_sw = (ShockWave*)my_child->objectAtIndex(i);
			t_sw->stopSW();
		}
	}
}
void SW_Parent::myInit ()
{
	is_justDie = false;
	
	
	//		myGD->regSW(this,
	//					callfuncIpOC_selector(SW_Parent::createSW),
	//					callfunc_selector(SW_Parent::stopAllSW),
	//					callfuncIpOC_selector(SW_Parent::createJDSW));
	
	myGD->V_Ip["SW_createSW"] = std::bind(&SW_Parent::createSW, this, _1);
	myGD->V_V["SW_stopAllSW"] = std::bind(&SW_Parent::stopAllSW, this);
	myGD->V_Ip["SW_createJDSW"] = std::bind(&SW_Parent::createJDSW, this, _1);
}
#undef LZZ_INLINE
