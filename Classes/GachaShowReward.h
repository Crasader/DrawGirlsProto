#ifndef GACHASHOWREWARD_H
#define GACHASHOWREWARD_H

#include "cocos2d.h"
#include <functional>
USING_NS_CC;
#include "CCMenuLambda.h"
#include "KSAlertView.h"
#include "KSUtil.h"
#include <vector>
#include "Well512.h"
#include "ProbSelector.h"
#include "FromTo.h"
#include "KSUtil.h"
#include "StarGoldData.h"
#include "GachaPurchase.h"
#include "GachaReward.h"
USING_NS_CC;


class GachaShowReward : public CCLayer
{
public:
	GachaShowReward();
	virtual ~GachaShowReward();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(const std::function<void(void)>& replayFunction, const std::function<void(void)>& obtainFunction, CCSprite* rewardImg, 
			const std::string& descStr, RewardKind kind, int value)
	{
		CCLayer::init();

		CCSprite* dimed = CCSprite::create();
		dimed->setTextureRect(CCRectMake(0, 0, 520, 320));
		dimed->setColor(ccc3(0, 0, 0));
		dimed->setOpacity(180);
		dimed->setPosition(ccp(240, 160));
		addChild(dimed);
		CCSprite* back = CCSprite::create("gacha_result.png");
		back->setPosition(ccp(240, 160));
		addChild(back);

		CCMenuLambda* _menu = CCMenuLambda::create();
		_menu->setTouchPriority(INT_MIN);
		_menu->setPosition(ccp(0, 0));
		addChild(_menu);

		auto prevObtain = [=](){
			int selectedItemValue = value;
			switch(kind) {
				case RewardKind::kRuby:
					mySGD->setStar(mySGD->getStar() + selectedItemValue);
					myDSH->saveUserData({kSaveUserData_Key_star}, [=](Json::Value v) {

					});
					break;
				case RewardKind::kGold:
					mySGD->setGold(mySGD->getGold() + selectedItemValue);
					myDSH->saveUserData({kSaveUserData_Key_gold}, [=](Json::Value v) {

					});
					break;
				case RewardKind::kSpecialAttack:
					{
						int currentValue = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_attack);
						myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_critical, currentValue + selectedItemValue);
					}
					break;
				case RewardKind::kDash:
					{
						int currentValue = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_fast);
						myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_critical, currentValue + selectedItemValue);
					}
					break;
				case RewardKind::kSlience:
					{
						int currentValue = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_silence);
						myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_critical, currentValue + selectedItemValue);
					}
					break;
				case RewardKind::kRentCard:
					{
						int currentValue = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_rentCard);
						myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_critical, currentValue + selectedItemValue);
					}
					break;
				case RewardKind::kSubMonsterOneKill:
					{
						int currentValue = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_subOneDie);
						myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_critical, currentValue + selectedItemValue);
					}
					break;
			}
			getParent()->removeFromParent();
		};

		CCMenuItemLambda* obtainItem = CCMenuItemImageLambda::create("gacha_popup_off.png", "gacha_popup_off.png",
				[=](CCObject*)
				{
					prevObtain();
					obtainFunction(); // 닫을 때 뒤에 터치를 활성화 하는 것. 이름과는 다름.
				});
		obtainItem->setPosition(ccp(320, 60));
		_menu->addChild(obtainItem);
	

		CCMenuItemLambda* replayItem = CCMenuItemImageLambda::create("gacha_popup_again.png", "gacha_popup_again.png",
				[=](CCObject*)
				{
					prevObtain();
					replayFunction();
				});
		replayItem->setPosition(ccp(207, 60));
		_menu->addChild(replayItem);
			
		addChild(rewardImg);
		rewardImg->setPosition(ccp(240, 160));
		CCLabelTTF* desc = CCLabelTTF::create(descStr.c_str(), "", 14.f);
		desc->setPosition(ccp(240, 140));
		addChild(desc);

		return true;
	}

	static GachaShowReward* create(const std::function<void(void)>& replayFunction, const std::function<void(void)>& obtainFunction, CCSprite* rewardImg, 
			const std::string& descStr, RewardKind kind, int value)
	{
		GachaShowReward* t = new GachaShowReward();
		t->init(replayFunction, obtainFunction, rewardImg, descStr, kind, value);
		t->autorelease();
		return t;
	}
	virtual void registerWithTouchDispatcher();
};
#endif
