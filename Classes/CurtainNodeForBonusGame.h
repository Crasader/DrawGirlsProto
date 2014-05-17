#ifndef CURTAINNODEFORBONUSGAME_H
#define CURTAINNODEFORBONUSGAME_H

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
#include "KSLabelTTF.h"
#include "StyledLabelTTF.h"
#include "CommonButton.h"
#include "GaBaBo.h"
USING_NS_CC_EXT;

enum MiniGameZorder{
	kMiniGameZorder_game = 1,
	kMiniGameZorder_curtain,
	kMiniGameZorder_title,
	kMiniGameZorder_content,
	kMiniGameZorder_menu
};
enum MiniGameCode{
	kMiniGameCode_gababo = 0,
	kMiniGameCode_touchtouch = 1
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

	static CurtainNodeForBonusGame* create(MiniGameCode gameCode, int tPrior, std::function<void(void)> onPressStartButton)
	{
		CurtainNodeForBonusGame* t = new CurtainNodeForBonusGame();
		t->init(gameCode, tPrior, onPressStartButton);
		t->autorelease();
		return t;
	}
	bool init(MiniGameCode gameCode, int tPrior, std::function<void(void)> onPressStartButton)
	{
		CCLayer::init();

		m_touchPriority = tPrior;
		m_gameCode = gameCode;
		m_onPressStartButton = onPressStartButton;
		setTouchEnabled(true);
		m_leftCurtain = CCSprite::create("curtain_left.png");
		m_leftCurtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
		m_leftCurtain->setAnchorPoint(ccp(1.f, 0.5f));
		m_leftCurtain->setPosition(ccp(-80, 160));
		addChild(m_leftCurtain, kMiniGameZorder_curtain);

		m_rightCurtain = CCSprite::create("curtain_left.png");
		m_rightCurtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
		m_rightCurtain->setFlipX(true);
		m_rightCurtain->setAnchorPoint(ccp(0.f, 0.5f));
		m_rightCurtain->setPosition(ccp(560,160));
		addChild(m_rightCurtain, kMiniGameZorder_curtain);

		m_titleBonusGame = CCSprite::create("gababo_bonus.png");
		m_titleBonusGame->setPosition(ccp(240, 500));
		addChild(m_titleBonusGame, kMiniGameZorder_title);
		//m_titleBonusGame->setVisible(false);
		m_titleStr = KSLabelTTF::create("보너스 게임", mySGD->getFont().c_str(), 13.f);
		m_titleStr->setPosition(ccp(240, 500));
		m_titleStr->setColor(ccc3(50, 250, 0));
		addChild(m_titleStr, kMiniGameZorder_title);
		m_contentBack = CCScale9Sprite::create("ui_game_clear_back.png", CCRectMake(0, 0, 157, 30),
																					CCRectMake(43, 6, 66, 20));
		m_contentBack->setContentSize(CCSizeMake(310, 100));
		m_contentBack->setPosition(ccp(650,150));
		addChild(m_contentBack, kMiniGameZorder_content);
		CCScale9Sprite* buttonBack = CCScale9Sprite::create("startsetting_item_buy.png");
		m_startMenu = CommonButton::create("게임시작", 23.f, CCSizeMake(160, 50), 
																			buttonBack, m_touchPriority - 1);
		m_startMenu->setPosition(ccp(240, -200));
		addChild(m_startMenu, kMiniGameZorder_menu);
		m_startMenu->setFunction([=](CCObject* t){
			menuAction(t);

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
		if(m_gameCode == kMiniGameCode_gababo)
		{
			KSLabelTTF* title_gamename = KSLabelTTF::create("가위! 바위! 보!", mySGD->getFont().c_str(), 33.f);
			title_gamename->setColor(ccc3(255, 230, 0));
			title_gamename->enableStroke(ccc3(0, 0, 0), 2.f, true);
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
		}
		if(m_gameCode == kMiniGameCode_touchtouch)
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
			m_contentBack->addChild(content_front);
		}
		return true;
	}
	void menuAction(CCObject* pSender)
	{
		
		CCMoveTo* left_move = CCMoveTo::create(0.3f, ccp(-80,160));
		m_leftCurtain->runAction(left_move);

		CCMoveTo* right_move = CCMoveTo::create(0.3f, ccp(560,160));
		m_rightCurtain->runAction(right_move);

		CCMoveTo* title_move = CCMoveTo::create(0.3f, ccp(240, 500));
		m_titleBonusGame->runAction(title_move);

		CCMoveTo* title_move2 = CCMoveTo::create(0.3f, ccp(240, 500));
		m_titleStr->runAction(title_move2);

		CCMoveTo* content_move = CCMoveTo::create(0.3f, ccp(-170, 150));
		m_contentBack->runAction(content_move);

		CCMoveTo* menu_move = CCMoveTo::create(0.3f, ccp(240, -200));
		CCCallFunc* menu_call = CCCallFunc::create(this, callfunc_selector(CurtainNodeForBonusGame::startGame));
		CCSequence* menu_seq = CCSequence::createWithTwoActions(menu_move, menu_call);
		m_startMenu->runAction(menu_seq);
		// 커튼을 치기 전에 미리 생성해놈.
		if(m_onPressStartButton)
			m_onPressStartButton();
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
	void showPopup()
	{
		CCMoveTo* left_move = CCMoveTo::create(0.3f, ccp(240,160));
		m_leftCurtain->runAction(left_move);

		CCMoveTo* right_move = CCMoveTo::create(0.3f, ccp(240,160));
		m_rightCurtain->runAction(right_move);

		CCMoveTo* title_move = CCMoveTo::create(0.3f, ccp(240, 270));
		m_titleBonusGame->runAction(title_move);

		CCMoveTo* title_move2 = CCMoveTo::create(0.3f, ccp(240, 270));
		m_titleStr->runAction(title_move2);
		CCMoveTo* content_move = CCMoveTo::create(0.3f, ccp(240, 150));
		m_contentBack->runAction(content_move);

		CCMoveTo* menu_move = CCMoveTo::create(0.3f, ccp(240, 65));
		CCCallFunc* menu_call = CCCallFunc::create(this, callfunc_selector(CurtainNodeForBonusGame::endShowPopup));
		CCSequence* menu_seq = CCSequence::createWithTwoActions(menu_move, menu_call);
		m_startMenu->runAction(menu_seq);
	}
	void endShowPopup()
	{
	}
	void onEnter()
	{
		CCLayer::onEnter();

		showPopup();
	}
	virtual void registerWithTouchDispatcher();
protected:
	int m_touchPriority;
	CCSprite* m_leftCurtain;
	CCSprite* m_rightCurtain;
	CCSprite* m_titleBonusGame;
	KSLabelTTF* m_titleStr;
	CCScale9Sprite* m_contentBack;
	CommonButton* m_startMenu;
	MiniGameCode m_gameCode;
	std::function<void(void)> m_onPressStartButton;
	//std::function<void(void)> startMiniGame;
};

#endif