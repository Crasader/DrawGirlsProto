//
//  GachaReward.h
//  DGproto
//
//  Created by ksoo k on 2013. 12. 12..
//
//

#ifndef DGproto_GachaReward_h
#define DGproto_GachaReward_h
#include "cocos2d.h"
USING_NS_CC;


enum class RewardKind
{
	kGold,
	kRuby,
	kSpecialAttack,
	kDash,
	kSlience,
	kSubMonsterOneKill,
	kPower,
	kMobLess,
	kAddTime,
	kDecreaseBossHp,
	kDecreaseMobScale,
	kPerfectLess, // 완벽주의자
	kChangeOrderForce // 체인지 순서대로.
};


class RewardSprite : public CCSprite
{
public:
	virtual ~RewardSprite(){}
	static RewardSprite* create(RewardKind kind, int value, const std::string& imgFile, int weight)
	{
		RewardSprite* rs = new RewardSprite();
		rs->initWithFile(imgFile.c_str());
		rs->autorelease();
		
		rs->m_kind = kind;
		rs->m_value = value;
		rs->m_spriteStr = imgFile;
		rs->m_weight = weight;
		return rs;
	}
	RewardKind m_kind;
	int m_value; // 가치.
	std::string m_spriteStr;
	int m_weight; // 가중치, 높을 수록 잘 걸림.
};



#endif
