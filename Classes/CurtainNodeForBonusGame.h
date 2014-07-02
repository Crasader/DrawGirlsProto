#ifndef CURTAINNODEFORBONUSGAME_H
#define CURTAINNODEFORBONUSGAME_H

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
#include "KSLabelTTF.h"
#include "StyledLabelTTF.h"
#include "CommonButton.h"
#include "DataStorageHub.h"
#include "KSUtil.h"
#include "FormSetter.h"
#include "CommonAnimation.h"
#include "KsLocal.h"


//#include "GaBaBo.h"
USING_NS_CC_EXT;

enum BonusGameZorder{
	kBonusGameZorder_game = 1,
	kBonusGameZorder_curtain,
	kBonusGameZorder_title,
	kBonusGameZorder_content,
	kBonusGameZorder_menu
};
enum BonusGameCode{
	kBonusGameCode_gababo = 0,
	kBonusGameCode_touchtouch = 1
};

struct BonusGameReward
{
	std::string spriteName;
	std::string desc;
	float scale;
};


class CurtainNodeForBonusGame : public CCLayer
{
public:
	CurtainNodeForBonusGame();
	virtual ~CurtainNodeForBonusGame();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	//static CurtainNodeForBonusGame* create()
	//{
		//CurtainNodeForBonusGame* t = new CurtainNodeForBonusGame();
		//t->init();
		//t->autorelease();
		//return t;
	//}

	static CurtainNodeForBonusGame* create(BonusGameCode gameCode, int tPrior, std::function<void(void)> onPressStartButton)
	{
		CurtainNodeForBonusGame* t = new CurtainNodeForBonusGame();
		t->init(gameCode, tPrior, onPressStartButton);
		t->autorelease();
		return t;
	}
	bool init(BonusGameCode gameCode, int tPrior, std::function<void(void)> onPressStartButton)
	{
		CCLayer::init();
		startFormSetter(this);
		setFormSetterGuide("_0514_start_1.png");
		
		
		m_touchPriority = tPrior;
		m_gameCode = gameCode;
		m_onPressStartButton = onPressStartButton;
		setTouchEnabled(true);
		m_leftCurtain = CCSprite::create("curtain_left.png");
		m_leftCurtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
		m_leftCurtain->setAnchorPoint(ccp(1.f, 0.5f));
		m_leftCurtain->setPosition(ccp(-80, 160));
		addChild(m_leftCurtain, kBonusGameZorder_curtain);

		m_rightCurtain = CCSprite::create("curtain_left.png");
		m_rightCurtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
		m_rightCurtain->setFlipX(true);
		m_rightCurtain->setAnchorPoint(ccp(0.f, 0.5f));
		m_rightCurtain->setPosition(ccp(560,160));
		addChild(m_rightCurtain, kBonusGameZorder_curtain);

		m_titleBonusGame = CCSprite::create("gababo_bonus.png");
		m_titleBonusGame->setPosition(ccp(240, 500));
		addChild(m_titleBonusGame, kBonusGameZorder_title);
		//m_titleBonusGame->setVisible(false);
		m_titleStr = KSLabelTTF::create("보너스 게임", mySGD->getFont().c_str(), 13.f);
		m_titleStr->setPosition(ccp(240, 500));
		m_titleStr->setColor(ccc3(29, 10, 0));
		addChild(m_titleStr, kBonusGameZorder_title);
		m_contentBack = CCScale9Sprite::create("ui_game_clear_back.png", CCRectMake(0, 0, 157, 30),
																					CCRectMake(43, 6, 66, 20));
		m_contentBack->setContentSize(CCSizeMake(310, 72.f));
		m_contentBack->setPosition(ccp(650,150));
		addChild(m_contentBack, kBonusGameZorder_content);
		CCScale9Sprite* buttonBack = CCScale9Sprite::create("startsetting_item_buy.png");
		m_startMenu = CommonButton::create("게임시작", 23.f, CCSizeMake(175.f, 55.f),
																			buttonBack, m_touchPriority - 1);
		m_startMenu->setTitleColor(ccc3(37, 18, 0));
		m_startMenu->setPosition(ccp(240, -200));
		addChild(m_startMenu, kBonusGameZorder_menu);
		m_startMenu->setFunction([=](CCObject* t){
			menuAction(nullptr);

		});
		Json::Value value1, value2, value3, value4, value5, value6;
		value1["fillcolor"] = StyledLabelTTF::makeRGB(255, 255, 255);
		value1["font"] = mySGD->getFont().c_str();
		value1["size"] = 11.f;

		value3["linebreak"] = true;
		value3["linespacing"] = 20.f;
		KSLabelTTF* desc = KSLabelTTF::create("게임 설명", mySGD->getFont().c_str(), 13.f);
		desc->setColor(ccc3(180, 255, 0));

		desc->setPosition(ccp(m_contentBack->getContentSize().width/2.f, m_contentBack->getContentSize().height/2.f+20));
		m_contentBack->addChild(desc);
		

		
		
		if(m_gameCode == kBonusGameCode_gababo)
		{
			KSLabelTTF* title_gamename = KSLabelTTF::create("가위! 바위! 보!", mySGD->getFont().c_str(), 33.f);
			title_gamename->setColor(ccc3(255, 230, 0));
			title_gamename->enableOuterStroke(ccc3(0, 0, 0), 2.f, true);
			title_gamename->setPosition(ccp(m_titleBonusGame->getContentSize().width/2.f, -title_gamename->getContentSize().height/2.f-7));
			m_titleBonusGame->addChild(title_gamename);

			StyledLabelTTF* content_front = StyledLabelTTF::create({
				{"캐릭터와 눈치를 보며", value1.toStyledString()},
				{"", value3.toStyledString()},
				{"가위바위보를 해보세요!", value1.toStyledString()},
				{"", value3.toStyledString()}
			}, StyledAlignment::kCenterAlignment);
			content_front->setPosition(ccp(m_contentBack->getContentSize().width/2.f, m_contentBack->getContentSize().height/2.f-2));
			m_contentBack->addChild(content_front);
			content_front->setOldAnchorPoint();


		}
		if(m_gameCode == kBonusGameCode_touchtouch)
		{
			KSLabelTTF* title_gamename = KSLabelTTF::create("터치! 터치!", mySGD->getFont().c_str(), 24.f);

			title_gamename->setPosition(ccp(m_titleBonusGame->getContentSize().width/2.f, -title_gamename->getContentSize().height/2.f-7));
			m_titleBonusGame->addChild(title_gamename);

			StyledLabelTTF* content_front = StyledLabelTTF::create({
				{"최대한 많이 터치를 하세요.", value1.toStyledString()},
				{"", value3.toStyledString()},
				{"돈이 쏟아져요!", value1.toStyledString()},
				{"", value3.toStyledString()}
			}, StyledAlignment::kCenterAlignment);
			content_front->setPosition(ccp(m_contentBack->getContentSize().width/2.f, m_contentBack->getContentSize().height/2.f-2));
			content_front->setOldAnchorPoint();
			m_contentBack->addChild(content_front);
			
		}
		
		showPopup(nullptr);
		return true;
	}
	static CurtainNodeForBonusGame* createForEnding(int priority, BonusGameReward reward,
																									std::function<void(void)> onCloseCompleted, std::function<void(void)> onOpenCompleted)
	{
		CurtainNodeForBonusGame* t = new CurtainNodeForBonusGame();
		t->initForEnding(priority, reward, onCloseCompleted, onOpenCompleted);
		t->autorelease();
		return t;
	}
	bool initForEnding(int priority, BonusGameReward reward,
										 std::function<void(void)> onCloseCompleted, std::function<void(void)> onOpenCompleted)
	{
		CCLayer::init();
		
		startFormSetter(this);
		m_touchPriority = priority;
		m_onCloseCompleted = onCloseCompleted;
		m_onOpenCompleted = onOpenCompleted;
		setTouchEnabled(true);
		m_leftCurtain = CCSprite::create("curtain_left.png");
		m_leftCurtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
		m_leftCurtain->setAnchorPoint(ccp(1.f, 0.5f));
		m_leftCurtain->setPosition(ccp(-80, 160));
		addChild(m_leftCurtain, kBonusGameZorder_curtain);
		
		m_rightCurtain = CCSprite::create("curtain_left.png");
		m_rightCurtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
		m_rightCurtain->setFlipX(true);
		m_rightCurtain->setAnchorPoint(ccp(0.f, 0.5f));
		m_rightCurtain->setPosition(ccp(560,160));
		addChild(m_rightCurtain, kBonusGameZorder_curtain);
		
		
		auto contentBack = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
		contentBack->setPosition(ccp(240.0,155.0)); 			// dt (0.0,5.0)
		contentBack->setContentSize(CCSizeMake(288.0,204.5)); 			// dt (-22.0,54.5)
		addChild(contentBack, kBonusGameZorder_content);
		CCSprite* rName = CCSprite::create(reward.spriteName.c_str());
		
		CCScale9Sprite* inner_right = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
		
		inner_right->setPosition(ccp(144.0,84.0)); 			// dt (-203.0,-41.0)
		inner_right->setContentSize(CCSizeMake(280.5,157.0)); 			// dt (30.5,-78.0)
		contentBack->addChild(inner_right);
		
		rName->setPosition(ccpFromSize(contentBack->getContentSize()) / 2.f + ccp(0, 16.5));
		contentBack->addChild(rName);
		
		KSLabelTTF* rLabel = KSLabelTTF::create(reward.desc.c_str(), mySGD->getFont().c_str(), 16.f);
		rLabel->setPosition(ccpFromSize(contentBack->getContentSize()) / 2.f + ccp(0, 16.5));
//		rLabel->setColor(ccc3(254, 250, 50));
		rLabel->setColor(ccc3(255, 255, 255));
		rLabel->enableOuterStroke(ccc3(0, 0, 0), 1.f);
		contentBack->addChild(rLabel);
	
		StyledLabelTTF* comment = StyledLabelTTF::create("<font color=#FFFFFF newline=12>축하합니다.</font>"
																										 "<font color=#FFFFFF>보상은 선물함에서 확인하세요.</font>", mySGD->getFont().c_str(),
																										 12.f, 999, StyledAlignment::kCenterAlignment);
		contentBack->addChild(comment);
		comment->setPosition(ccpFromSize(contentBack->getContentSize()) / 2.f + ccp(0, -12.5));
		setFormSetter(comment);
		CCScale9Sprite* buttonBack = CCScale9Sprite::create("startsetting_item_buy.png");
//		auto m_startMenu = CommonButton::create("보상받기", 23.f, CCSizeMake(160, 50),
//																			 buttonBack, m_touchPriority - 1);
//		m_startMenu->setPosition( ccp(240, 65.f) );
		auto obtainReward = CommonButton::create(getLocal(LK::kRewardConfirm), 23.f, CCSizeMake(160, 50),
																			 CommonButtonLightPupple, m_touchPriority - 1);
		obtainReward->setPosition(ccpFromSize(contentBack->getContentSize()) / 2.f + ccp(0, -50 - 15.5));
		if(obtainReward)
		{
			addChild(KSGradualValue<CCPoint>::create(obtainReward->getPosition(), ccp(240, 85.f), 0.3f,
																							 [=](CCPoint t){
																								 //																								 m_obtainReward->setPosition(t);
																							 },
																							 [=](CCPoint t){
																								 //																								 m_obtainReward->setPosition(t);
																								 if(onCloseCompleted)
																								 {
																									 onCloseCompleted();
																								 }
																							 }));
		}

		setFormSetter(contentBack);
		setFormSetter(obtainReward);
		setFormSetter(inner_right);
		setFormSetter(rName);
		setFormSetter(rLabel);
		contentBack->addChild(obtainReward, kBonusGameZorder_menu);
		setFormSetter(contentBack);
		showPopup(onCloseCompleted);
		obtainReward->setFunction([=](CCObject* t){
			CommonAnimation::closePopup(this, contentBack, nullptr, [=](){},
																	[=](){
																		menuAction(onOpenCompleted);
																	});
			setFormSetter(obtainReward);
		});
	
		KSLabelTTF* rewardConfirm = KSLabelTTF::create(getLocal(LK::kRewardTitle), mySGD->getFont().c_str(), 20.f);
		rewardConfirm->setColor(ccc3(255, 155, 0));
		contentBack->addChild(rewardConfirm);
		rewardConfirm->setPosition(ccp(contentBack->getContentSize().width / 2.f, 178.0)); 			// dt (52.5,178.0)
	
		CommonAnimation::openPopup(this, contentBack, nullptr, nullptr, nullptr);
//		StyledLabelTTF* sltObtain = StyledLabelTTF::create("<font color=970 size=18>획득</font><font color=999 size=18> 하였습니다.</font>",
//																											 mySGD->getFont().c_str(), 18, 999, StyledAlignment::kCenterAlignment);
//		m_contentBack->addChild(sltObtain);
//		sltObtain->setPosition(ccpFromSize(m_contentBack->getContentSize()) / 2.f);
//		sltObtain->setPositionY(74.f);
		

		
		
		
		return true;
	}

	
	void menuAction(std::function<void(void)> callback);
	void removeCurtain()
	{
		removeFromParent();
	}
	void startGame()
	{
		removeFromParent();
		m_leftCurtain->setVisible(false);
		m_rightCurtain->setVisible(false);
		m_titleBonusGame->removeFromParent();
		m_contentBack->removeFromParent();
		// mini_game->startGame();
	}
	void showPopup(std::function<void(void)> callback)
	{
		CCMoveTo* left_move = CCMoveTo::create(0.3f, ccp(240,160));
		m_leftCurtain->runAction(left_move);

		CCMoveTo* right_move = CCMoveTo::create(0.3f, ccp(240,160));
		m_rightCurtain->runAction(right_move);

		if(m_titleBonusGame)
		{
			CCMoveTo* title_move = CCMoveTo::create(0.3f, ccp(240, 257.5f));
			m_titleBonusGame->runAction(title_move);
		}
		
		if(m_titleStr)
		{
			CCMoveTo* title_move2 = CCMoveTo::create(0.3f, ccp(240, 257.5f));
			m_titleStr->runAction(title_move2);
	
		}
		
		if(m_contentBack)
		{
			CCMoveTo* content_move = CCMoveTo::create(0.3f, ccp(240, 144.5));
			m_contentBack->runAction(content_move);
		}
		if(m_startMenu)
		{
			addChild(KSGradualValue<CCPoint>::create(m_startMenu->getPosition(), ccp(240, 65.f), 0.3f,
																							 [=](CCPoint t){
																								 m_startMenu->setPosition(t);
																							 },
																							 [=](CCPoint t){
																								 m_startMenu->setPosition(t);
																								 if(callback)
																								 {
																									 callback();
																								 }
																							 }));
		}
				
		
//		CCMoveTo* menu_move = CCMoveTo::create(0.3f, ccp(240, 65));
//		CCCallFunc* menu_call = CCCallFunc::create(this, callfunc_selector(CurtainNodeForBonusGame::endShowPopup));
//		CCSequence* menu_seq = CCSequence::createWithTwoActions(menu_move, menu_call);
//		m_startMenu->runAction(menu_seq);
	}
	void endShowPopup()
	{
	}
	void onEnter()
	{
		CCLayer::onEnter();

		
	}
	virtual void registerWithTouchDispatcher();
	void removeFromParent()
	{
		CCLayer::removeFromParent();
	}
protected:
	int m_touchPriority;
	CCSprite* m_leftCurtain;
	CCSprite* m_rightCurtain;
	CCSprite* m_titleBonusGame;
	KSLabelTTF* m_titleStr;
	CCScale9Sprite* m_contentBack;
	CommonButton* m_startMenu;
//	CommonButton* m_obtainReward;
	BonusGameCode m_gameCode;
	std::function<void(void)> m_onPressStartButton;
	
	std::function<void(void)> m_onCloseCompleted;
	std::function<void(void)> m_onOpenCompleted;
	//std::function<void(void)> startMiniGame;
};

#endif
