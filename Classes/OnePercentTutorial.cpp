#include "OnePercentTutorial.h"

#include "MyLocalization.h"
#include "CommonButton.h"
#include "ASPopupView.h"
#include "CCMenuLambda.h"
#include "KSLabelTTF.h"
#include "FormSetter.h"
#include "LoadingLayer.h"
#include "ShopPopup.h"
#include "StyledLabelTTF.h"
#include <boost/format.hpp>
#include "FormSetter.h"
#include "EasingAction.h"
#include "StageImgLoader.h"
#include "FormSetter.h"
#include "CommonAnimation.h"
#include "OnePercentGame.h"
#include "DataStorageHub.h"

OnePercentTutorial::OnePercentTutorial()
{
	
}
OnePercentTutorial::~OnePercentTutorial()
{
}
//void OnePercentTutorial::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool OnePercentTutorial::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool OnePercentTutorial::init(float originalPercent,
															std::function<void(void)> cancelGacha,
															std::function<void(float)> tryGacha)
{
	CCLayer::init();

	
	m_originalPercent = originalPercent;
	m_cancelGacha = cancelGacha;
	m_tryGacha = tryGacha;
	
	m_cursorDirection = 1;
	m_tutorialStep = 1;
	startFormSetter(this);
	m_grayBack = CCSprite::create("back_gray.png");
	m_grayBack->setPosition(ccp(240,myDSH->ui_center_y));
	m_grayBack->setScaleY(myDSH->ui_top/320.f);
	addChild(m_grayBack, kOnePercentTutorial_Z_gray);
	m_grayBack->setOpacity(0);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(m_container, kOnePercentTutorial_Z_back);
	
	back_img = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_img->setContentSize(CCSizeMake(309, 270));
	back_img->setPosition(ccp(0,0));
	m_container->addChild(back_img, kOnePercentTutorial_Z_back);
	
	CCScale9Sprite* inner_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	inner_back->setContentSize(CCSizeMake(295.0, 192.0)); 			// dt (0.0, 12.0)
	inner_back->setPosition(ccp(0.0, 6.0));
	m_container->addChild(inner_back, kOnePercentTutorial_Z_back);
	
	setFormSetter(back_img);
	setFormSetter(inner_back);
	KSLabelTTF* titleLbl = KSLabelTTF::create("보너스 영역 뽑기", mySGD->getFont().c_str(), 15.f);
	titleLbl->setColor(ccc3(255, 170, 20));
	titleLbl->setPosition(ccp(.0, 112.5)); 			// dt (6.0, 112.5)
	m_container->addChild(titleLbl, kOnePercentGame_Z_content);
	setFormSetter(titleLbl);
	CCScale9Sprite* puppleInner = CCScale9Sprite::create("mainpopup_pupple4.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	
	puppleInner->setContentSize(CCSizeMake(280.0, 114.0)); 			// dt (-1.0, -26.0)
	puppleInner->setPosition(ccp(0.0, 31.0));


	m_container->addChild(puppleInner, kOnePercentTutorial_Z_back);
	
	m_gachaDesc = StyledLabelTTF::create("<font color=#FFFFFF size=12 newline=15>아슬아슬하게 영역을 100% 획득을 못했군요!</font>"
																			 "<font color=#FFAA14 size=12 newline=30>보너스 영역 뽑기에 도전해보세요!</font>"
																			 "<font color=#FFAA14 size=14 newline=15>100% 영역 획득 성공시</font>"
																			 "<font color=#FFAA14 size=14 newline=15>4등급 카드를 획득 할 수 있어요!</font>",
																			 mySGD->getFont().c_str(), 14, 999, StyledAlignment::kCenterAlignment);
	m_container->addChild(m_gachaDesc, kOnePercentTutorial_Z_content);
	m_gachaDesc->setPosition(ccp(0.0, 67.5));

	
	StyledLabelTTF* desc2 = StyledLabelTTF::create(
			CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_onePercentDesc2), 3, 0.3f)->getCString(),
			mySGD->getFont().c_str(), 12.f, 0, StyledAlignment::kCenterAlignment);
	m_container->addChild(desc2, kOnePercentTutorial_Z_content);
	desc2->setPosition(ccp(0.0, -41.0));


	CCSprite* progress_back = CCSprite::create("one_percent_gacha_graph_back.png");
//	progress_back->setPosition(ccp(0,10));

	m_container->addChild(progress_back, kOnePercentTutorial_Z_content);
	progress_back->setPosition(ccp(0, -69));
	CCSprite* validArea = CCSprite::create();
//	m_validArea = validArea;
	validArea->setTextureRect(CCRectMake(0, 0, 50, 15));
	validArea->setPosition(ccp(0, -60));
	m_container->addChild(validArea, kOnePercentTutorial_Z_content);
	validArea->setColor(ccc3(232, 0, 192));
	
	
	m_cursor = CCSprite::create("one_percent_gacha_02.png");
	m_cursor->setVisible(false);
	m_cursor->setPosition(ccp(0, -60));
	m_container->addChild(m_cursor, kOnePercentTutorial_Z_content);
	
	
	CCScale9Sprite* gacha_back = CCScale9Sprite::create("common_button_yellowup.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	CCNode* gachaNode = CCNode::create();
	CCLabelTTF* t_label = CCLabelTTF::create();
	
	KSLabelTTF* gacha_label = KSLabelTTF::create("다음", mySGD->getFont().c_str(), 15);
	gacha_label->setColor(ccBLACK);
//	gacha_label->setPosition(ccp(0,15));
	t_label->addChild(gacha_label);

	gacha_button = CCControlButton::create(t_label, gacha_back);
//	gachaNode->setScale(0.7f);
	gachaNode->addChild(gacha_button); // 스케일을 위해 하나 더 만듬.
	gachaNode->setPosition(ccp(0.0, 38.0)); 			// dt (0.0, -4.0
	gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::gachaAction), CCControlEventTouchUpInside);
	gacha_button->setPreferredSize(CCSizeMake(118, 45));
	gacha_button->setPosition(ccp(0, -145));
	m_container->addChild(gachaNode, kOnePercentTutorial_Z_content);
	gacha_button->setTouchPriority(-180);
	setFormSetter(gachaNode);
	setFormSetter(puppleInner);
	setFormSetter(m_container);
	setFormSetter(m_gachaDesc);
	setFormSetter(desc2);
	setFormSetter(progress_back);
	setFormSetter(gacha_label);
	
	
	cancel_menu = CommonButton::createCloseButton();
	setFormSetter(cancel_menu);
	cancel_menu->setTouchPriority(-180);
	cancel_menu->setFunction([=](CCObject* sender) {
		if(m_cancelGacha) {
			removeFromParent();
			m_cancelGacha();
		}
		//CCNode* t_node = CCNode::create();
		//t_node->setTag(kOnePercentGame_MT_cancel);
		//menuAction(t_node);
	});
	cancel_menu->setPosition(ccp(143, 112));
	m_container->addChild(cancel_menu, kOnePercentTutorial_Z_content);
	CCFadeTo* gray_fade = CCFadeTo::create(0.25f, 255);
	m_grayBack->runAction(gray_fade);
	
	CommonAnimation::openPopup(this, m_container, nullptr);
	
	
	setFormSetter(puppleInner);
	setFormSetter(m_gachaDesc);
	setFormSetter(desc2);
	
	scheduleUpdate();
	return true;
}


void OnePercentTutorial::gachaAction(CCObject* sender, CCControlEvent t_event)
{
	if(m_tutorialStep == 1)
	{
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
		m_gachaDesc->removeFromParent();
		
		m_gachaDesc = StyledLabelTTF::create("<font color=#FFFFFF size=12 newline=15>아래 게이지바에서 분홍색 영역을 맞히면</font>"
																				 "<font color=#FFAA14 size=12 newline=15>0.3%를 추가로 획득할 수 있습니다.</font>"
																				 "<font color=#FFAA14 size=12 newline=30>기회는 총 3번 주어집니다.</font>"
																				 "<font color=#FFAA14 size=14 newline=15>자 그럼 100% 에 도전해 볼까요?</font>",
																				 mySGD->getFont().c_str(), 14, 999, StyledAlignment::kCenterAlignment);
		m_container->addChild(m_gachaDesc, kOnePercentTutorial_Z_content);
		m_gachaDesc->setPosition(ccp(0.0, 71.5)); 			// dt (0.0, 71.5)
		m_tutorialStep++;
	}
	else if(m_tutorialStep == 2)
	{
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
		myDSH->setIntegerForKey(kSDH_Key_isShowOnePercentTutorial, 1);
		OnePercentGame* opg = OnePercentGame::create(m_originalPercent, m_cancelGacha, m_tryGacha, false);
		getParent()->addChild(opg, getZOrder());
		removeFromParent();
		
		// OnePercentGame::cr ... 호출 해야함.
	}
}


void OnePercentTutorial::update(float dt)
{
	if(m_cursorDirection > 0)
	{
		m_cursor->setPositionX(m_cursor->getPositionX() + 2.5f);
		if(m_cursor->getPositionX() > rightLimit)
		{
			m_cursorDirection = -1;
		}
	}
	else
	{
		m_cursor->setPositionX(m_cursor->getPositionX() - 2.5f);
		if(m_cursor->getPositionX() < leftLimit)
		{
			m_cursorDirection = 1;
		}
	}
}