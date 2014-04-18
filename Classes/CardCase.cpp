//
//  CardCase.cpp
//  DGproto
//
//  Created by 사원3 on 13. 11. 12..
//
//

#include "CardCase.h"
#include "ServerDataSave.h"
#include "StarGoldData.h"
#include "CountingBMLabel.h"
#include "DataStorageHub.h"

SpinUpgradeMissile* SpinUpgradeMissile::create( string t_type_name, int elemental_level, CCPoint t_center_position, bool is_popup )
{
	SpinUpgradeMissile* t_sum = new SpinUpgradeMissile();
	t_sum->myInit(t_type_name, elemental_level, t_center_position, is_popup);
	t_sum->autorelease();
	return t_sum;
}

void SpinUpgradeMissile::startSpin()
{
	schedule(schedule_selector(SpinUpgradeMissile::spining));
}

void SpinUpgradeMissile::spining()
{
	distance_turnToAfter = sqrtf(powf(turn_position.x-after_position.x, 2.f) + powf(turn_position.y-after_position.y, 2.f));
	if(distance_turnToAfter < 1) // reflection
	{
		CCSprite* before = (CCSprite*)getChildByTag(kChildTagSUM_after);
		before->setScaleX(before->getScaleX() - 1.f);
		before->setTag(kChildTagSUM_before);
		reorderChild(before, kZorderSUM_beforeImg);

		string edge_string = "jm_" + type_name + "_edge.png";
		edge = CCSprite::create(edge_string.c_str());
		edge->setPosition(ccp(before->getContentSize().width,before->getContentSize().height/2.f));
		edge->setScaleX(1.f/before->getScaleX());
		before->addChild(edge);

		before_position = after_position;

		angle += dAngle2;
		if(angle >= 180)		angle-=360;
		if(angle < -180)		angle+=360;

		after_position = ccp(1,tanf(angle/180.f*M_PI));
		after_position = ccpMult(after_position, mRadius/sqrtf(powf(after_position.x, 2.f) + powf(after_position.y, 2.f)));
		if((angle > 90 && angle <= 270) || (angle <= -90 && angle > -270))
			after_position = ccpMult(after_position, -1);
		after_position = ccpAdd(center_position, after_position);

		move_dv = ccpSub(after_position, before_position);
		move_dv = ccpMult(move_dv, 1.f/10.f);

		turn_position = ccpAdd(before_position, move_dv);

		rotation_angle = atan2f(move_dv.y, move_dv.x)/M_PI*180.f;
		string temp_string = "jm_" + type_name + ".png";

		CCSprite* tag1_spr = CCSprite::create(temp_string.c_str());
		tag1_spr->setAnchorPoint(ccp(1,0.5));
		tag1_spr->setPosition(turn_position);
		tag1_spr->setRotation(-rotation_angle);
		addChild(tag1_spr, kZorderSUM_afterImg, kChildTagSUM_after);

		if(!is_self_spin)
			missile_main->setRotation(-rotation_angle+90);
	}
	else
	{
		turn_position = ccpAdd(turn_position, move_dv);
		missile_main->setPosition(turn_position);
		missile_particle->setPosition(turn_position);

		CCSprite* after = (CCSprite*)getChildByTag(kChildTagSUM_after);
		after->setPosition(turn_position);
		if(after->getScaleX() < 7)
			after->setScaleX(after->getScaleX() + 1.f);

		CCSprite* before = (CCSprite*)getChildByTag(kChildTagSUM_before);
		if(before)
		{
			if(before->getScaleX() > 1)
			{
				before->setScaleX(before->getScaleX() - 1.f);
				edge->setScaleX(1.f/before->getScaleX());
			}
			else
				before->removeFromParentAndCleanup(true);
		}
	}
}

void SpinUpgradeMissile::myInit( string t_type_name, int elemental_level, CCPoint t_center_position, bool is_popup )
{
	mRadius = (is_popup ? myRadius2 : myRadius2-5);

	type_name = t_type_name;
	center_position = t_center_position;
	angle = 90.2;

	before_position = ccp(1,tanf(angle/180.f*M_PI));
	before_position = ccpMult(before_position, mRadius/sqrtf(powf(before_position.x, 2.f) + powf(before_position.y, 2.f)));
	if((angle > 90 && angle <= 270) || (angle <= -90 && angle > -270))
		before_position = ccpMult(before_position, -1);
	before_position = ccpAdd(center_position, before_position);

	angle += dAngle2;
	if(angle >= 180)		angle-=360;
	if(angle < -180)		angle+=360;

	after_position = ccp(1,tanf(angle/180.f*M_PI));
	after_position = ccpMult(after_position, mRadius/sqrtf(powf(after_position.x, 2.f) + powf(after_position.y, 2.f)));
	if((angle > 90 && angle <= 270) || (angle <= -90 && angle > -270))
		after_position = ccpMult(after_position, -1);
	after_position = ccpAdd(center_position, after_position);

	move_dv = ccpSub(after_position, before_position);
	move_dv = ccpMult(move_dv, 1.f/10.f);

	turn_position = ccpAdd(before_position, move_dv);

	string particle_string;
	if(elemental_level >= 1 && elemental_level <= 5)			particle_string = "jm_" + type_name + "1_particle.png";
	else														particle_string = "jm_" + type_name + "2_particle.png";

	float particle_cnt = 3 + elemental_level*3;

	missile_particle = new CCParticleSystemQuad();
	missile_particle->initWithTotalParticles(particle_cnt);
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(particle_string.c_str());
	missile_particle->setTexture(texture);
	missile_particle->setEmissionRate(particle_cnt*2);
	missile_particle->setAngle(0.0);
	missile_particle->setAngleVar(360.0);
	ccBlendFunc blendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
	missile_particle->setBlendFunc(blendFunc);
	missile_particle->setDuration(-1.0);
	missile_particle->setEmitterMode(kCCParticleModeGravity);
	ccColor4F startColor = {1.00,1.00,0.00,1.00}; // 0.76 0.25 0.12
	missile_particle->setStartColor(startColor);
	ccColor4F startColorVar = {0,0,0,0};
	missile_particle->setStartColorVar(startColorVar);
	ccColor4F endColor = {1.00,0.00,0.00,1.00};
	missile_particle->setEndColor(endColor);
	ccColor4F endColorVar = {0,0,0,0};
	missile_particle->setEndColorVar(endColorVar);
	missile_particle->setStartSize(20.0);
	missile_particle->setStartSizeVar(5.0);
	missile_particle->setEndSize(10.0);
	missile_particle->setEndSizeVar(5.0);
	missile_particle->setGravity(ccp(0,0));
	missile_particle->setRadialAccel(0.0);
	missile_particle->setRadialAccelVar(0.0);
	missile_particle->setSpeed(50);
	missile_particle->setSpeedVar(0.0);
	missile_particle->setTangentialAccel(0);
	missile_particle->setTangentialAccelVar(0);
	missile_particle->setTotalParticles(particle_cnt);
	missile_particle->setLife(0.20);
	missile_particle->setLifeVar(0.0);
	missile_particle->setStartSpin(0);
	missile_particle->setStartSpinVar(360);
	missile_particle->setEndSpin(0);
	missile_particle->setEndSpinVar(360);
	missile_particle->autorelease();
	missile_particle->setPosition(turn_position);
	missile_particle->setPosVar(ccp(0,0));
	addChild(missile_particle, kZorderSUM_particle);


	rotation_angle = atan2f(move_dv.y, move_dv.x)/M_PI*180.f;
	string temp_string = "jm_" + type_name + ".png";

	CCSprite* tag1_spr = CCSprite::create(temp_string.c_str());
	tag1_spr->setAnchorPoint(ccp(1,0.5));
	tag1_spr->setPosition(turn_position);
	tag1_spr->setRotation(-rotation_angle);
	addChild(tag1_spr, kZorderSUM_afterImg, kChildTagSUM_after);


	string main_string = "jm_" + type_name + CCString::createWithFormat("%d_main.png", elemental_level)->getCString();

	missile_main = CCSprite::create(main_string.c_str());
	missile_main->setPosition(turn_position);
	addChild(missile_main, kZorderSUM_main);

	if(elemental_level >= 8)
	{
		is_self_spin = true;
		int random_value = rand()%200;
		random_value = (random_value >= 100) ? random_value+100 : random_value-300;
		CCRotateBy* t_rotate = CCRotateBy::create(1.f, random_value);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_rotate);
		missile_main->runAction(t_repeat);
	}
	else
	{
		is_self_spin = false;
		missile_main->setRotation(-rotation_angle+90);
	}

	startSpin();
}

SpinBasicMissile* SpinBasicMissile::create( string type_name, int elemental_level, CCPoint t_center_position, bool is_popup )
{
	SpinBasicMissile* t_sbm = new SpinBasicMissile();
	t_sbm->myInit(type_name, elemental_level, t_center_position, is_popup);
	t_sbm->autorelease();
	return t_sbm;
}

void SpinBasicMissile::startSpin()
{
	schedule(schedule_selector(SpinBasicMissile::spining));
}

void SpinBasicMissile::spining()
{
	angle += dAngle1;
	if(angle >= 180)		angle-=360;
	if(angle < -180)		angle+=360;

	side_position = ccp(1,tan(angle/180.f*M_PI));
	if((angle > 90 && angle <= 270) || (angle <= -90 && angle > -270))
		side_position = ccpMult(side_position, -1);
	side_position = ccpMult(side_position, mRadius/sqrtf(powf(side_position.x, 2.f) + powf(side_position.y, 2.f)));
	side_position = ccpAdd(center_position, side_position);

	if(is_self_spin)			missile_main->setRotation(missile_main->getRotation()+dAngle1*2);
	else						missile_main->setRotation(-angle);
	missile_main->setPosition(side_position);
	missile_particle->setPosition(side_position);
}

void SpinBasicMissile::myInit( string type_name, int elemental_level, CCPoint t_center_position, bool is_popup )
{
	mRadius = (is_popup ? myRadius1 : myRadius1-6);

	center_position = t_center_position;
	angle = 179.8;

//	float particle_cnt = elemental_level*5;

	string particle_filename;
	if(type_name == "life")
		particle_filename = "jm_particle%d_life.plist";
	else if(type_name == "fire")
		particle_filename = "jm_particle%d_fire.plist";
	else if(type_name == "water")
		particle_filename = "jm_particle%d_water.plist";
	else if(type_name == "empty")
		particle_filename = "jm_particle%d_empty.plist";
		
	
	missile_particle = CCParticleSystemQuad::create(CCString::createWithFormat(particle_filename.c_str(), (elemental_level-1)/2+1)->getCString());
	
//	missile_particle = new CCParticleSystemQuad();
//	missile_particle->initWithTotalParticles(particle_cnt);
//	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(particle_string.c_str());
//	missile_particle->setTexture(texture);
//	missile_particle->setEmissionRate(particle_cnt);
//	missile_particle->setAngle(0.0);
//	missile_particle->setAngleVar(360.0);
//	ccBlendFunc blendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
//	missile_particle->setBlendFunc(blendFunc);
//	missile_particle->setDuration(-1.0);
//	missile_particle->setEmitterMode(kCCParticleModeGravity);
//	ccColor4F startColor = {1.00,1.00,1.00,1.00}; // 0.76 0.25 0.12
//	missile_particle->setStartColor(startColor);
//	ccColor4F startColorVar = {0,0,0,0};
//	missile_particle->setStartColorVar(startColorVar);
//	ccColor4F endColor = {1.00,1.00,1.00,1.00};
//	missile_particle->setEndColor(endColor);
//	ccColor4F endColorVar = {0,0,0,0};
//	missile_particle->setEndColorVar(endColorVar);
//	missile_particle->setStartSize(5.0);
//	missile_particle->setStartSizeVar(5.0);
//	missile_particle->setEndSize(5.0);
//	missile_particle->setEndSizeVar(0.0);
//	missile_particle->setGravity(ccp(0,0));
//	missile_particle->setRadialAccel(0.0);
//	missile_particle->setRadialAccelVar(0.0);
//	missile_particle->setSpeed(20);
//	missile_particle->setSpeedVar(0.0);
//	missile_particle->setTangentialAccel(0);
//	missile_particle->setTangentialAccelVar(0);
//	missile_particle->setTotalParticles(particle_cnt);
//	missile_particle->setLife(0.3);
//	missile_particle->setLifeVar(0.0);
//	missile_particle->setStartSpin(0.0);
//	missile_particle->setStartSpinVar(360.f);
//	missile_particle->setEndSpin(0.0);
//	missile_particle->setEndSpinVar(360.f);
//	missile_particle->autorelease();

	side_position = ccp(1,tan(angle/180.f*M_PI));
	if((angle > 90 && angle <= 270) || (angle <= -90 && angle > -270))
		side_position = ccpMult(side_position, -1);
	side_position = ccpMult(side_position, mRadius/sqrtf(powf(side_position.x, 2.f) + powf(side_position.y, 2.f)));
	side_position = ccpAdd(center_position, side_position);

	missile_particle->setPosition(side_position);
//	missile_particle->setPosVar(ccp(0,0));
	addChild(missile_particle);

	int animation_cnt = 0;
	
	if(type_name == "life")
	{
		if(elemental_level == 8)
			animation_cnt = 3;
		else
			animation_cnt = 2;
	}
	else if(type_name == "fire")
	{
		animation_cnt = 2;
	}
	else if(type_name == "water")
	{
		if(elemental_level == 6 || elemental_level == 9)
			animation_cnt = 4;
		else if(elemental_level == 8)
			animation_cnt = 6;
		else
			animation_cnt = 3;
	}
	else if(type_name == "empty")
	{
		if(elemental_level == 1)
			animation_cnt = 2;
		else if(elemental_level == 4)
			animation_cnt = 4;
		else
			animation_cnt = 3;
	}
	
	CCTexture2D* missile_main_texture = CCTextureCache::sharedTextureCache()->addImage(CCString::createWithFormat(("jm_" + type_name + "%d_main.png").c_str(), elemental_level)->getCString());
	
	CCSize animation_cut_size = CCSizeMake(missile_main_texture->getContentSize().width/animation_cnt, missile_main_texture->getContentSize().height);
	
	missile_main = CCSprite::createWithTexture(missile_main_texture, CCRectMake(0, 0, animation_cut_size.width, animation_cut_size.height));
	missile_main->setPosition(side_position);
	addChild(missile_main);
	
	CCAnimation* main_animation = CCAnimation::create();
	main_animation->setDelayPerUnit(0.1f);
	for(int i=0;i<animation_cnt;i++)
		main_animation->addSpriteFrameWithTexture(missile_main_texture, CCRectMake(i*animation_cut_size.width, 0, animation_cut_size.width, animation_cut_size.height));
	
	CCAnimate* main_animate = CCAnimate::create(main_animation);
	CCRepeatForever* main_repeat = CCRepeatForever::create(main_animate);
	
	missile_main->runAction(main_repeat);
	

	is_self_spin = false;

	if(!is_self_spin)
		missile_main->setRotation(-angle);

	startSpin();
}

CardCase* CardCase::create( int t_selected_card_number )
{
	CardCase* t_cc = new CardCase();
	t_cc->myInit(t_selected_card_number, -1, "-1");
	t_cc->autorelease();
	return t_cc;
}

//CardCase* CardCase::create( int t_card_stage, int t_card_grade )
//{
//	CardCase* t_cc = new CardCase();
//	t_cc->myInit(NSDS_GI(t_card_stage, kSDS_SI_level_int1_card_i, t_card_grade), -1, "-1");
//	t_cc->autorelease();
//	return t_cc;
//}

CardCase* CardCase::create(int t_card_number, int t_level, string t_passive)
{
	CardCase* t_cc = new CardCase();
	t_cc->myInit(t_card_number, t_level, t_passive);
	t_cc->autorelease();
	return t_cc;
}

CCLabelTTF* CardCase::getRecentDurabilityLabel()
{
	return recent_durability_label;
}

void CardCase::myInit(int t_selected_card_number, int t_level, string t_passive)
{
	card_level = t_level;
	original_passive_string = t_passive;
	myInit(t_selected_card_number);
}

void CardCase::myInit( int t_selected_card_number )
{
	is_show_passive_content = false;
	selected_card_number = t_selected_card_number;
	
	int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, selected_card_number);
	string grade_filename;
	if(card_grade == 2)
		grade_filename = "silver";
	else if(card_grade == 3)
		grade_filename = "gold";
	else
		grade_filename = "bronze";
	
	CCSprite* top_case = CCSprite::create(("card_case_" + grade_filename + "_top.png").c_str());
//	CCSprite* top_case = CCSprite::create("card_case_top.png");
	top_case->setPosition(ccp(160,430));
	addChild(top_case, kCARDCASE_Z_sideCase);

	CCSprite* bottom_case = CCSprite::create(("card_case_" + grade_filename + "_bottom.png").c_str());
//	CCSprite* bottom_case = CCSprite::create("card_case_bottom.png");
	bottom_case->setPosition(ccp(160,0));
	addChild(bottom_case, kCARDCASE_Z_sideCase);

	CCSprite* left_case = CCSprite::create(("card_case_" + grade_filename + "_left.png").c_str());
//	CCSprite* left_case = CCSprite::create("card_case_left.png");
	left_case->setPosition(ccp(0,215));
	addChild(left_case, kCARDCASE_Z_sideCase);

	CCSprite* right_case = CCSprite::create(("card_case_" + grade_filename + "_right.png").c_str());
//	CCSprite* right_case = CCSprite::create("card_case_right.png");
	right_case->setPosition(ccp(320,215));
	addChild(right_case, kCARDCASE_Z_sideCase);

//	CCSprite* rank_case = CCSprite::create("card_case_rank.png");
//	rank_case->setPosition(ccp(30,402));
//	addChild(rank_case, kCARDCASE_Z_innerCase);
//
//	CCLabelTTF* rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, t_selected_card_number))->getCString(), mySGD->getFont().c_str(), 30);
//	rank_label->setAnchorPoint(ccp(0.5,0.5));
//	rank_label->setPosition(ccp(rank_case->getContentSize().width/2.f, rank_case->getContentSize().height/2.f-3));
//	rank_case->addChild(rank_label, kCARDCASE_Z_data);
//
//	durability_case = CCSprite::create("card_case_durability.png");
//	durability_case->setPosition(ccp(287,417));
//	addChild(durability_case, kCARDCASE_Z_innerCase);
//
//	total_durability_label = CCLabelTTF::create(CCString::createWithFormat("/%d", myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, t_selected_card_number))->getCString(), mySGD->getFont().c_str(), 10);
//	total_durability_label->setColor(ccBLACK);
//	total_durability_label->setAnchorPoint(ccp(1.f,0.5));
//	total_durability_label->setPosition(ccp(42,21));
//	durability_case->addChild(total_durability_label, kCARDCASE_Z_data);

	recent_durability_label = CCLabelTTF::create(CCString::createWithFormat(""/*%d", myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, t_selected_card_number)*/)->getCString(), mySGD->getFont().c_str(), 20);
	recent_durability_label->setAnchorPoint(ccp(0.5,0.5));
	recent_durability_label->setPosition(ccp(293,413));
	addChild(recent_durability_label, kCARDCASE_Z_data);

//	if(card_level == -1)
//		card_level = myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, selected_card_number);
	
	CCSprite* option_case = CCSprite::create("card_case_option.png");
	option_case->setPosition(ccp(265,80));
	option_case->setScale(1.5f);
	addChild(option_case, kCARDCASE_Z_innerCase);

	CountingBMLabel* pow_label = CountingBMLabel::create("0", "allfont.fnt", 0.5f, "%d");
	pow_label->setPosition(ccp(75,15));//145,58));
	pow_label->setColor(ccRED);
	pow_label->setScale(1.8f);
	addChild(pow_label, kCARDCASE_Z_data);

	pow_label->setString(CCString::createWithFormat("%d", int(NSDS_GI(kSDS_CI_int1_missile_power_i, t_selected_card_number)*((card_level-1)*0.1f+1.f)))->getCString());

	CountingBMLabel* dex_label = CountingBMLabel::create("0", "allfont.fnt", 0.5f, "%d");
	dex_label->setPosition(ccp(275,15));//145,26));
	dex_label->setColor(ccGREEN);
	dex_label->setScale(1.8f);
	addChild(dex_label, kCARDCASE_Z_data);

	dex_label->setString(CCString::createWithFormat("%d", int(NSDS_GI(kSDS_CI_int1_missile_dex_i, t_selected_card_number)*((card_level-1)*0.1f+1.f)))->getCString());

	CountingBMLabel* spd_label = CountingBMLabel::create("0.0", "allfont.fnt", 0.5f, "%.1f");
	spd_label->setPosition(ccp(175,15));//180,42));
	spd_label->setColor(ccBLUE);
	spd_label->setScale(1.8f);
	addChild(spd_label, kCARDCASE_Z_data);

	spd_label->setString(CCString::createWithFormat("%.1f", NSDS_GD(kSDS_CI_int1_missile_speed_d, t_selected_card_number))->getCString());

	//		CCProgressTimer* pow_progress = CCProgressTimer::create(CCSprite::create("card_damage_bar.png"));
	//		pow_progress->setType(kCCProgressTimerTypeBar);
	//		pow_progress->setMidpoint(ccp(0,0));
	//		pow_progress->setBarChangeRate(ccp(1,0));
	//		pow_progress->setPosition(ccp(82,27));
	//		pow_progress->setPercentage(100.f);
	//		option_case->addChild(pow_progress);
	//		
	//		CCProgressFromTo* pow_action = CCProgressFromTo::create(1.f, 0.f, 100.f);
	//		pow_progress->runAction(pow_action);
	//		
	//		CCProgressTimer* spd_progress = CCProgressTimer::create(CCSprite::create("card_speed_bar.png"));
	//		spd_progress->setType(kCCProgressTimerTypeBar);
	//		spd_progress->setMidpoint(ccp(0,0));
	//		spd_progress->setBarChangeRate(ccp(1,0));
	//		spd_progress->setPosition(ccp(82,17));
	//		spd_progress->setPercentage(100.f);
	//		option_case->addChild(spd_progress);
	//		
	//		CCProgressFromTo* spd_action = CCProgressFromTo::create(1.f, 0.f, 100.f);
	//		spd_progress->runAction(spd_action);
	//		
	//		CCProgressTimer* dex_progress = CCProgressTimer::create(CCSprite::create("card_dex_bar.png"));
	//		dex_progress->setType(kCCProgressTimerTypeBar);
	//		dex_progress->setMidpoint(ccp(0,0));
	//		dex_progress->setBarChangeRate(ccp(1,0));
	//		dex_progress->setPosition(ccp(82,7));
	//		dex_progress->setPercentage(100.f);
	//		option_case->addChild(dex_progress);
	//		
	//		CCProgressFromTo* dex_action = CCProgressFromTo::create(1.f, 0.f, 100.f);
	//		dex_progress->runAction(dex_action);
	
	CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("%d", card_level)->getCString(), mySGD->getFont().c_str(), 20);
	t_card_level_label->setPosition(ccp(113,400));
	addChild(t_card_level_label, kCARDCASE_Z_data);

//	if(original_passive_string == "-1")
//		original_passive_string = myDSH->getStringForKey(kDSH_Key_cardPassive_int1, t_selected_card_number).c_str();

	if(original_passive_string != "")
	{
		Json::Reader reader;
		Json::Value passive_data;
		reader.parse(original_passive_string, passive_data);

		string operator_string = passive_data["operator"].asString();
		double ai_value = passive_data["ai"].asDouble();
		double speed_value = passive_data["speed"].asDouble();
		double scale_value = passive_data["scale"].asDouble();
		double attackpercent_value = passive_data["attackpercent"].asDouble();
		double hp_value = passive_data["hp"].asDouble();
		double agi_value = passive_data["agi"].asDouble();

		passive_content = "몬스터 능력치 감소";
		if(ai_value != 0.0)
			passive_content += "\nai : " + operator_string + CCString::createWithFormat("%.0f", ai_value)->getCString();
		if(speed_value != 0.0)
			passive_content += "\nspeed : " + operator_string + CCString::createWithFormat("%.2f", speed_value)->getCString();
		if(scale_value != 0.0)
			passive_content += "\nscale : " + operator_string + CCString::createWithFormat("%.2f", scale_value)->getCString();
		if(attackpercent_value != 0.0)
			passive_content += "\nattack percent : " + operator_string + CCString::createWithFormat("%.2f", attackpercent_value)->getCString();
		if(hp_value != 0.0)
			passive_content += "\nhp : " + operator_string + CCString::createWithFormat("%.0f", hp_value)->getCString();
		if(agi_value != 0.0)
			passive_content += "\nagi : " + operator_string + CCString::createWithFormat("%.0f", agi_value)->getCString();

		CCSprite* n_passive = CCSprite::create("card_passive_button.png");
		CCSprite* s_passive = CCSprite::create("card_passive_button.png");
		s_passive->setColor(ccGRAY);

		CCMenuItem* passive_item = CCMenuItemSprite::create(n_passive, s_passive, this, menu_selector(CardCase::menuAction));
		passive_item->setTag(kCARDCASE_Tag_passive);

		CCMenu* passive_menu = CCMenu::createWithItem(passive_item);
		passive_menu->setPosition(ccp(275,32));
		addChild(passive_menu, kCARDCASE_Z_data, kCARDCASE_Tag_passive);
		
		passive_menu->setTouchPriority(kCCMenuHandlerPriority-1);
	}


	string missile_type_code = NSDS_GS(kSDS_CI_int1_missile_type_s, t_selected_card_number).c_str();
	int missile_type_number = MissileDamageData::getMissileType(missile_type_code.c_str());

	int elemental_number = missile_type_number%10;
	int level_number = missile_type_number/10 + 1;

	string type_name;

	if(elemental_number == kElementCode_empty)
		type_name = "empty";
	else if(elemental_number == kElementCode_life)
		type_name = "life";
	else if(elemental_number == kElementCode_fire)
		type_name = "fire";
	else if(elemental_number == kElementCode_water)
		type_name = "water";
	else if(elemental_number == kElementCode_wind)
		type_name = "wind";
	else if(elemental_number == kElementCode_lightning)
		type_name = "lightning";
	else if(elemental_number == kElementCode_plasma)
		type_name = "plasma";

	if(elemental_number == kElementCode_empty || elemental_number == kElementCode_fire || elemental_number == kElementCode_life || elemental_number == kElementCode_water)
	{
		SpinBasicMissile* missile_img = SpinBasicMissile::create(type_name.c_str(), level_number, ccp(28,28), false);
		option_case->addChild(missile_img, kCARDCASE_Z_data);
	}
	else
	{
		SpinUpgradeMissile* missile_img = SpinUpgradeMissile::create(type_name.c_str(), level_number, ccp(28,28), false);
		option_case->addChild(missile_img, kCARDCASE_Z_data);
	}
}

void CardCase::menuAction( CCObject* sender )
{
	int tag = ((CCNode*)sender)->getTag();

	if(tag == kCARDCASE_Tag_passive)
	{
		//			CCMenu* passive_menu = (CCMenu*)getChildByTag(kCARDCASE_Tag_passive);
		//			passive_menu->setEnabled(false);

		if(!is_show_passive_content)
		{
			is_show_passive_content = true;
			CCSprite* n_passive_content = CCSprite::create("card_passive_case.png");
			CCLabelTTF* n_content = CCLabelTTF::create(passive_content.c_str(), mySGD->getFont().c_str(), 15);
			n_content->setHorizontalAlignment(kCCTextAlignmentCenter);
			n_content->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
			n_content->setPosition(ccp(n_passive_content->getContentSize().width/2.f, n_passive_content->getContentSize().height/2.f));
			n_passive_content->addChild(n_content);

			CCSprite* s_passive_content = CCSprite::create("card_passive_case.png");
			s_passive_content->setColor(ccGRAY);
			CCLabelTTF* s_content = CCLabelTTF::create(passive_content.c_str(), mySGD->getFont().c_str(), 15);
			s_content->setHorizontalAlignment(kCCTextAlignmentCenter);
			s_content->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
			s_content->setPosition(ccp(s_passive_content->getContentSize().width/2.f, s_passive_content->getContentSize().height/2.f));
			s_passive_content->addChild(s_content);

			CCMenuItem* passive_content_item = CCMenuItemSprite::create(n_passive_content, s_passive_content, this, menu_selector(CardCase::menuAction));
			passive_content_item->setTag(kCARDCASE_Tag_passive_content);

			CCMenu* passive_content_menu = CCMenu::createWithItem(passive_content_item);
			passive_content_menu->setPosition(ccp(160, 100));
			addChild(passive_content_menu, kCARDCASE_Z_data, kCARDCASE_Tag_passive_content);
		}
		else
		{
			is_show_passive_content = false;
			removeChildByTag(kCARDCASE_Tag_passive_content);

			//				CCMenu* passive_menu = (CCMenu*)getChildByTag(kCARDCASE_Tag_passive);
			//				passive_menu->setEnabled(true);
		}
	}
	else if(tag == kCARDCASE_Tag_passive_content)
	{
		is_show_passive_content = false;
		removeChildByTag(kCARDCASE_Tag_passive_content);

		//			CCMenu* passive_menu = (CCMenu*)getChildByTag(kCARDCASE_Tag_passive);
		//			passive_menu->setEnabled(true);
	}
}
