//
//  CardCase.h
//  DGproto
//
//  Created by 사원3 on 13. 11. 12..
//
//

#ifndef __DGproto__CardCase__
#define __DGproto__CardCase__

#include "cocos2d.h"
#include "ServerDataSave.h"
#include "StarGoldData.h"
#include "MissileDamageData.h"
#include "CountingBMLabel.h"

USING_NS_CC;

enum ZorderSUM{
	kZorderSUM_afterImg = 1,
	kZorderSUM_beforeImg,
	kZorderSUM_particle,
	kZorderSUM_main
};

enum ChildTagSUM{
	kChildTagSUM_before = 1,
	kChildTagSUM_after
};

#define dAngle2		144.0
#define myRadius2	20.2

class SpinUpgradeMissile : public CCNode
{
public:
	static SpinUpgradeMissile* create(string t_type_name, int elemental_level, CCPoint t_center_position, bool is_popup);
	
private:
	
	string type_name;
	CCSprite* missile_main;
	CCParticleSystemQuad* missile_particle;
	CCSprite* edge;
	
	CCPoint center_position;
	CCPoint before_position;
	CCPoint after_position;
	
	CCPoint turn_position;
	
	CCPoint move_dv;
	
	float angle;
	float rotation_angle;
	
	float distance_turnToAfter;
	
	float mRadius;
	
	bool is_self_spin;
	
	
	void startSpin();
	
	void spining();
	
	void myInit(string t_type_name, int elemental_level, CCPoint t_center_position, bool is_popup);
};

#define dAngle1		6.0
#define myRadius1	15.0

class SpinBasicMissile : public CCNode
{
public:
	static SpinBasicMissile* create(string type_name, int elemental_level, CCPoint t_center_position, bool is_popup);
	
private:
	
	CCSprite* missile_main;
	CCParticleSystemQuad* missile_particle;
	
	CCPoint center_position;
	CCPoint side_position;
	float angle;
	
	bool is_self_spin;
	
	float mRadius;
	
	void startSpin();
	
	void spining();
	
	void myInit(string type_name, int elemental_level, CCPoint t_center_position, bool is_popup);
};



enum CARDCASE_Zorder{
	kCARDCASE_Z_sideCase = 1,
	kCARDCASE_Z_innerCase,
	kCARDCASE_Z_data
};

enum CARDCASE_Tag{
	kCARDCASE_Tag_passive = 1,
	kCARDCASE_Tag_passive_content,
	kCARDCASE_Tag_mounted_top = 10,
	kCARDCASE_Tag_mounted_bottom,
	kCARDCASE_Tag_mounted_left,
	kCARDCASE_Tag_mounted_right
};

class CardCase : public CCNode
{
public:
	static CardCase* create(int t_selected_card_number);
	
	static CardCase* create(int t_card_stage, int t_card_level);
	
	CCLabelTTF* getRecentDurabilityLabel();
	CCLabelTTF* getTotalDurabilityLabel();
	
	void startDecreaseDurability(CCObject* t_end, SEL_CallFunc d_end);
	
private:
	CCLabelTTF* recent_durability_label;
	CCLabelTTF* total_durability_label;
	CCSprite* durability_case;
	int selected_card_number;
	string passive_content;
	bool is_show_passive_content;
	
	void changeRecentDurabilityLabel(CCObject* sender);
	
	void myInit(int t_selected_card_number);
	
	void menuAction(CCObject* sender);
};

#endif /* defined(__DGproto__CardCase__) */
