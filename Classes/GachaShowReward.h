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
#include "CommonButton.h"
#include "DataStorageHub.h"

USING_NS_CC;


class GachaShowReward : public CCLayer
{
public:
	GachaShowReward();
	virtual ~GachaShowReward();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(const std::function<void(void)>& replayFunction, const std::function<void(void)>& obtainFunction, CCSprite* rewardImg, 
			const std::string& descStr, RewardKind kind, int value, const std::string& againImageName, GachaCategory gc)
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
//				case RewardKind::kSpecialAttack:
//					{
//						int currentValue = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_attack);
//						myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_critical, currentValue + selectedItemValue);
//					}
//					break;
//				case RewardKind::kDash:
//					{
//						int currentValue = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_fast);
//						myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_critical, currentValue + selectedItemValue);
//					}
//					break;
//				case RewardKind::kSlience:
//					{
//						int currentValue = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_silence);
//						myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_critical, currentValue + selectedItemValue);
//					}
//					break;
//				case RewardKind::kSubMonsterOneKill:
//					{
//						int currentValue = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_subOneDie);
//						myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_critical, currentValue + selectedItemValue);
//					}
//					break;
			}
			getParent()->removeFromParent();
		};

		CommonButton* obtainItem = CommonButton::create("닫기", 16, CCSizeMake(80,52), CommonButtonGreen, INT_MIN);	
		obtainItem->setFunction( [=](CCObject*) {
			prevObtain();
			obtainFunction(); // 닫을 때 뒤에 터치를 활성화 하는 것. 이름과는 다름.
		});
		obtainItem->setPosition(ccp(313, 70));
		addChild(obtainItem);
	
		CommonButton* replayItem = CommonButton::create(againImageName.c_str(), 16, CCSizeMake(150,52), CommonButtonBlue, INT_MIN);	
		if(gc == GachaCategory::kGoldGacha)
		{
			replayItem->setPrice(PriceTypeGold, mySGD->getGachaGoldFeeRetry());
			replayItem->setTitle("다시뽑기");
		}
		else if(gc == GachaCategory::kRubyGacha)
		{
			replayItem->setPrice(PriceTypeRuby, mySGD->getGachaRubyFeeRetry());
			replayItem->setTitle("다시뽑기");
		}
		replayItem->setFunction([=](CCObject*){
			prevObtain();
			replayFunction();
		});
		//CCMenuItemLambda* replayItem = CCMenuItemImageLambda::create(againImageName.c_str(), againImageName.c_str(),
				//[=](CCObject*)
				//{
					//prevObtain();
					//replayFunction();
				//});
		addChild(replayItem);
		replayItem->setPosition(ccp(200, 70));
			
		addChild(rewardImg);
		rewardImg->setPosition(ccp(240, 160));
		CCLabelTTF* desc = CCLabelTTF::create(descStr.c_str(), "", 14.f);
		desc->setPosition(ccp(240, 140));
		addChild(desc);

		return true;
	}

	static GachaShowReward* create(const std::function<void(void)>& replayFunction, const std::function<void(void)>& obtainFunction, CCSprite* rewardImg, 
			const std::string& descStr, RewardKind kind, int value, const std::string& againImageName, GachaCategory gc)
	{
		GachaShowReward* t = new GachaShowReward();
		t->init(replayFunction, obtainFunction, rewardImg, descStr, kind, value, againImageName, gc);
		t->autorelease();
		return t;
	}
	virtual void registerWithTouchDispatcher();
};
#endif
