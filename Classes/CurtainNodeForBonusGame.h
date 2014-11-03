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
#include "AudioEngine.h"

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
//		m_leftCurtain = CCSprite::create("curtain_left.png");
//		m_leftCurtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
//		m_leftCurtain->setAnchorPoint(ccp(1.f, 0.5f));
//		m_leftCurtain->setPosition(ccp(-80, 160));
//		addChild(m_leftCurtain, kBonusGameZorder_curtain);
//
//		m_rightCurtain = CCSprite::create("curtain_left.png");
//		m_rightCurtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
//		m_rightCurtain->setFlipX(true);
//		m_rightCurtain->setAnchorPoint(ccp(0.f, 0.5f));
//		m_rightCurtain->setPosition(ccp(560,160));
//		addChild(m_rightCurtain, kBonusGameZorder_curtain);
//
//		// 커튼 없애기
//		m_leftCurtain->setVisible(false);
//		m_rightCurtain->setVisible(false);
//		/*
//		 
		 
		 
		 
		 
		 
		// */
		
		float height_value = 320.f;
		if(myDSH->screen_convert_rate < 1.f)
			height_value = 320.f/myDSH->screen_convert_rate;
		
		if(height_value < myDSH->ui_top)
			height_value = myDSH->ui_top;
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		CCSprite* dimmed_sprite = CCSprite::create("whitePaper.png");
		dimmed_sprite->setPosition(ccp(240, myDSH->ui_center_y));
		dimmed_sprite->setColor(ccc3(0, 0, 0));
		dimmed_sprite->setOpacity(100);
		dimmed_sprite->setScaleX(screen_scale_x*480.f);
		dimmed_sprite->setScaleY(height_value);// /myDSH->screen_convert_rate));
		addChild(dimmed_sprite, 0);
		
		auto back = CCSprite::create("ingame_back2.png");
		//	back->setContentSize(CCSizeMake(357, 308));
		back->setPosition(ccp(240, 160));
		addChild(back, kBonusGameZorder_content);
		
		KSLabelTTF* titleLbl = KSLabelTTF::create(getLocal(LK::kBonusGame), mySGD->getFont().c_str(), 15);
		titleLbl->setPosition(ccp(84.0, 301.0));
		back->addChild(titleLbl);
		setFormSetter(titleLbl);
		
		CommonAnimation::applyShadow(titleLbl);

		
		
		auto realFront = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
		realFront->setContentSize(CCSizeMake(548 / 2.f, 506 / 2.f));
		realFront->setPosition(ccpFromSize(back->getContentSize()) / 2.f + ccp(0, -3.0));
		back->addChild(realFront);
		setFormSetter(realFront);
		
		auto front = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
		front->setPosition(ccpFromSize(back->getContentSize()) / 2.f + ccp(0, -95 - 12.5 + 19.5f));
		front->setContentSize(CCSizeMake(538 / 2.f, 156 / 2.f));
		back->addChild(front);
		
		//////////열리는 효과
		CommonAnimation::openPopup(this, back, nullptr, nullptr, nullptr);
		//////////////////////////////////////////////////////
		
		CCScale9Sprite* buttonBack = CCScale9Sprite::create("mainbutton_purple.png");
		m_startMenu = CommonButton::create(getLocal(LK::kBonusGameStart), 23.f, CCSizeMake(175.f, 55.f),
																			 buttonBack, m_touchPriority - 1);
		//		m_startMenu->setTitleColorForDisable(ccc3(37, 18, 0));
		//		m_startMenu->setTitleColor(ccc3(37, 18, 0));
		m_startMenu->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, 0));
//		addChild(m_startMenu, kBonusGameZorder_content);
		m_startMenu->setFunction([=](CCObject* t){
			if(m_startMenu->isEnabled() == false)
				return;
			AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
			m_startMenu->setEnabled(false);
			
			CommonAnimation::closePopup(this, back, nullptr, [=](){},
																	[=](){
																		menuAction(nullptr);
//																		menuAction(onOpenCompleted);
																	});
		});
		front->addChild(m_startMenu);
	
		Json::Value value1, value2, value3, value4, value5, value6;
		value1["fillcolor"] = StyledLabelTTF::makeRGB(255, 255, 255);
		value1["font"] = mySGD->getFont().c_str();
		value1["size"] = 11.f;

		value3["linebreak"] = true;
		value3["linespacing"] = 20.f;
		KSLabelTTF* desc = KSLabelTTF::create(getLocal(LK::kBonusGameDesc), mySGD->getFont().c_str(), 13.f);
		desc->setColor(ccc3(180, 255, 0));

		desc->setPosition(ccp(realFront->getContentSize().width/2.f, realFront->getContentSize().height/2.f+20));
		realFront->addChild(desc);
		

		setFormSetter(m_startMenu);
		setFormSetter(desc);
		
		
		if(m_gameCode == kBonusGameCode_gababo)
		{
			KSLabelTTF* title_gamename = KSLabelTTF::create(getLocal(LK::kBonusGameGababo), mySGD->getFont().c_str(), 33.f);
			title_gamename->setColor(ccc3(255, 230, 0));
			title_gamename->enableOuterStroke(ccc3(0, 0, 0), 2.f, true);
			title_gamename->setPosition(ccpFromSize(realFront->getContentSize()) / 2.f + ccp(0, 80.f));
			realFront->addChild(title_gamename);

			StyledLabelTTF* content_front = StyledLabelTTF::create(getLocal(LK::kGababoDesc1),
																														 mySGD->getFont().c_str(),
																														 11.f, 999, StyledAlignment::kCenterAlignment);
			content_front->setPosition(ccp(realFront->getContentSize().width/2.f, realFront->getContentSize().height/2.f-2));
			realFront->addChild(content_front);
			content_front->setOldAnchorPoint();

			setFormSetter(title_gamename);

		}
		if(m_gameCode == kBonusGameCode_touchtouch)
		{
			KSLabelTTF* title_gamename = KSLabelTTF::create(getLocal(LK::kBonusGameTouch), mySGD->getFont().c_str(), 24.f);

			title_gamename->setPosition(ccpFromSize(realFront->getContentSize()) / 2.f + ccp(0, 80.f));
			realFront->addChild(title_gamename);

			StyledLabelTTF* content_front = StyledLabelTTF::create(getLocal(LK::kTouchDesc1),
																														 mySGD->getFont().c_str(),
																														 11.f, 999, StyledAlignment::kCenterAlignment);
			content_front->setPosition(ccp(realFront->getContentSize().width/2.f, realFront->getContentSize().height/2.f-2));
			content_front->setOldAnchorPoint();
			realFront->addChild(content_front);
			
		}
		
		
		auto meChar = KS::loadCCBI<CCSprite*>(this, "gababo_me.ccbi").first;
		meChar->setPosition(ccp(34.0, 121.0));
		realFront->addChild(meChar);
		auto npcChar = KS::loadCCBI<CCSprite*>(this, "gababo_you.ccbi").first;
		npcChar->setPosition(ccp(234.5, 121.0));
		realFront->addChild(npcChar);
		
		setFormSetter(meChar);
		setFormSetter(npcChar);
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
//		m_leftCurtain = CCSprite::create("curtain_left.png");
//		m_leftCurtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
//		m_leftCurtain->setAnchorPoint(ccp(1.f, 0.5f));
//		m_leftCurtain->setPosition(ccp(-80, 160));
//		addChild(m_leftCurtain, kBonusGameZorder_curtain);
		
//		m_rightCurtain = CCSprite::create("curtain_left.png");
//		m_rightCurtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
//		m_rightCurtain->setFlipX(true);
//		m_rightCurtain->setAnchorPoint(ccp(0.f, 0.5f));
//		m_rightCurtain->setPosition(ccp(560,160));
//		addChild(m_rightCurtain, kBonusGameZorder_curtain);
//		m_leftCurtain->setVisible(false);
//		m_rightCurtain->setVisible(false);

		float height_value = 320.f;
		if(myDSH->screen_convert_rate < 1.f)
			height_value = 320.f/myDSH->screen_convert_rate;
		
		if(height_value < myDSH->ui_top)
			height_value = myDSH->ui_top;
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		CCSprite* dimmed_sprite = CCSprite::create("whitePaper.png");
		dimmed_sprite->setPosition(ccp(240, myDSH->ui_center_y));
		dimmed_sprite->setColor(ccc3(0, 0, 0));
		dimmed_sprite->setOpacity(100);
		dimmed_sprite->setScaleX(screen_scale_x*480.f);
		dimmed_sprite->setScaleY(height_value);// /myDSH->screen_convert_rate));
		addChild(dimmed_sprite, 0);

		
		auto contentBack = CCSprite::create("popup_large_back.png");
		contentBack->setPosition(ccp(240.0,155.0)); 			// dt (0.0,5.0)
//		contentBack->setContentSize(CCSizeMake(288.0,204.5)); 			// dt (-22.0,54.5)
		addChild(contentBack, kBonusGameZorder_content);
		CCSprite* rName = CCSprite::create(reward.spriteName.c_str());
		
		CCScale9Sprite* inner_right = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
		inner_right->setContentSize(CCSizeMake(502 / 2.f, 282 / 2.f));
		inner_right->setPosition(ccpFromSize(contentBack->getContentSize()/2.f) + ccp(0, 15));
		contentBack->addChild(inner_right);
		
//		inner_right->setPosition(ccp(144.0,84.0)); 			// dt (-203.0,-41.0)
//		inner_right->setContentSize(CCSizeMake(280.5,157.0)); 			// dt (30.5,-78.0)
//		contentBack->addChild(inner_right);
		
		rName->setPosition(ccpFromSize(contentBack->getContentSize()) / 2.f + ccp(0, 16.5 + 41.f - 29.f));
		contentBack->addChild(rName);
		
		KSLabelTTF* rLabel = KSLabelTTF::create(reward.desc.c_str(), mySGD->getFont().c_str(), 13.f);
		rLabel->setPosition(ccpFromSize(contentBack->getContentSize()) / 2.f + ccp(0, 16.5 - 19.f + 22 + 18.f));
//		rLabel->setColor(ccc3(254, 250, 50));
		rLabel->setColor(ccc3(255, 255, 255));
		rLabel->enableOuterStroke(ccc3(0, 0, 0), 1.f);
		contentBack->addChild(rLabel);
	
		// 보상선물은 우편함에서... 메시지인데 일단 없앰.
		StyledLabelTTF* comment = StyledLabelTTF::create(getLocal(LK::kBonusCong), mySGD->getFont().c_str(),
																										 13.f, 999, StyledAlignment::kCenterAlignment);
		contentBack->addChild(comment);
		comment->setPosition(ccpFromSize(contentBack->getContentSize()) / 2.f + ccp(0, -12.5 + 10.f));
		setFormSetter(comment);
//		auto m_startMenu = CommonButton::create("보상받기", 23.f, CCSizeMake(160, 50),
//																			 buttonBack, m_touchPriority - 1);
//		m_startMenu->setPosition( ccp(240, 65.f) );
		auto obtainReward = CommonButton::create(getLocal(LK::kRewardConfirm), 13.f, CCSizeMake(160, 50),
																						 CCScale9Sprite::create("mainbutton_purple.png"),
																						 m_touchPriority -1);
		
		obtainReward->setPosition(ccpFromSize(contentBack->getContentSize()) / 2.f + ccp(0, -50 - 15.5 - 18.5f));
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
			AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
			CommonAnimation::closePopup(this, contentBack, nullptr, [=](){},
																	[=](){
																		menuAction(onOpenCompleted);
																	});
			setFormSetter(obtainReward);
		});
	
		KSLabelTTF* rewardConfirm = KSLabelTTF::create(getLocal(LK::kRewardTitle), mySGD->getFont().c_str(), 12.f);
		contentBack->addChild(rewardConfirm);
		rewardConfirm->setPosition(ccp(65.5, 230.0));
		setFormSetter(rewardConfirm);
	
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
//		m_leftCurtain->setVisible(false);
//		m_rightCurtain->setVisible(false);
//		m_titleBonusGame->removeFromParent();
//		m_contentBack->removeFromParent();
		// mini_game->startGame();
	}
	void showPopup(std::function<void(void)> callback)
	{
//		CCMoveTo* left_move = CCMoveTo::create(0.3f, ccp(240,160));
//		m_leftCurtain->runAction(left_move);
//
//		CCMoveTo* right_move = CCMoveTo::create(0.3f, ccp(240,160));
//		m_rightCurtain->runAction(right_move);

//		if(m_titleBonusGame)
//		{
//			CCMoveTo* title_move = CCMoveTo::create(0.3f, ccp(240, 257.5f));
//			m_titleBonusGame->runAction(title_move);
//		}
		
		if(m_titleStr)
		{
			CCMoveTo* title_move2 = CCMoveTo::create(0.3f, ccp(240, 257.5f));
			m_titleStr->runAction(title_move2);
	
		}
		
//		if(m_contentBack)
//		{
//			CCMoveTo* content_move = CCMoveTo::create(0.3f, ccp(240, 144.5));
//			m_contentBack->runAction(content_move);
//		}
		if(m_startMenu)
		{
			addChild(KSGradualValue<CCPoint>::create(m_startMenu->getPosition(), ccp(240, 65.f), 0.3f,
																							 [=](CCPoint t){
//																								 m_startMenu->setPosition(t);
																							 },
																							 [=](CCPoint t){
//																								 m_startMenu->setPosition(t);
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
	//CCSprite* m_leftCurtain;
	//CCSprite* m_rightCurtain;
//	CCSprite* m_titleBonusGame;
	KSLabelTTF* m_titleStr;
//	CCScale9Sprite* m_contentBack;
	CommonButton* m_startMenu;
//	CommonButton* m_obtainReward;
	BonusGameCode m_gameCode;
	std::function<void(void)> m_onPressStartButton;
	
	std::function<void(void)> m_onCloseCompleted;
	std::function<void(void)> m_onOpenCompleted;
	//std::function<void(void)> startMiniGame;
};

#endif
