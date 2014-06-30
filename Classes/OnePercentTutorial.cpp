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
	inner_back->setContentSize(CCSizeMake(295.0, 183.0)); 			// dt (0.0, 12.0)
	inner_back->setPosition(ccp(0.0, 6.0));
	m_container->addChild(inner_back, kOnePercentTutorial_Z_back);
	
	setFormSetter(back_img);
	setFormSetter(inner_back);
	
	KSLabelTTF* titleLbl = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_onePercentTutorial1), mySGD->getFont().c_str(), 15.f);
	titleLbl->setColor(ccc3(255, 170, 20));
	titleLbl->setPosition(ccp(back_img->getContentSize().width/2.f, back_img->getContentSize().height-25)); 			// dt (6.0, 112.5)
	back_img->addChild(titleLbl);
	setFormSetter(titleLbl);
	CCScale9Sprite* puppleInner = CCScale9Sprite::create("mainpopup_pupple4.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	
	puppleInner->setContentSize(CCSizeMake(280.0, 114.0)); 			// dt (-1.0, -26.0)
	puppleInner->setPosition(ccp(0.0, 31.0));


	m_container->addChild(puppleInner, kOnePercentTutorial_Z_back);
	setFormSetter(puppleInner);
	
	m_gachaDesc = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_onePercentTutorial2),
																			 mySGD->getFont().c_str(), 14, 999, StyledAlignment::kCenterAlignment);
	puppleInner->addChild(m_gachaDesc, kOnePercentTutorial_Z_content);
	m_gachaDesc->setAnchorPoint(ccp(0.5f,0.5f));
	m_gachaDesc->setPosition(ccpFromSize(puppleInner->getContentSize()/2.f));
	setFormSetter(m_gachaDesc);
	
	StyledLabelTTF* desc2 = StyledLabelTTF::create(
			CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_onePercentDesc2), 3, 0.3f)->getCString(),
			mySGD->getFont().c_str(), 12.f, 0, StyledAlignment::kCenterAlignment);
	m_container->addChild(desc2, kOnePercentTutorial_Z_content);
	desc2->setPosition(ccp(0.0, -39.0));
	setFormSetter(desc2);

	CCSprite* progress_back = CCSprite::create("one_percent_gacha_graph_back.png");

	m_container->addChild(progress_back, kOnePercentTutorial_Z_content);
	progress_back->setPosition(ccp(0, -67));
	CCSprite* validArea = CCSprite::create();
	validArea->setTextureRect(CCRectMake(0, 0, 50, 15));
	validArea->setPosition(ccp(0, -67));
	m_container->addChild(validArea, kOnePercentTutorial_Z_content);
	validArea->setColor(ccc3(232, 0, 192));
	
	setFormSetter(progress_back);
	setFormSetter(validArea);
	
	m_cursor = CCSprite::create("one_percent_gacha_02.png");
	m_cursor->setVisible(false);
	m_cursor->setPosition(ccp(0, -67));
	m_container->addChild(m_cursor, kOnePercentTutorial_Z_content);
	
	
	CCScale9Sprite* gacha_back = CCScale9Sprite::create("common_button_yellowup.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	CCLabelTTF* t_label = CCLabelTTF::create();
	
	KSLabelTTF* gacha_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_onePercentTutorial3), mySGD->getFont().c_str(), 15);
	gacha_label->setColor(ccBLACK);
	gacha_label->disableOuterStroke();
	t_label->addChild(gacha_label);

	gacha_button = CCControlButton::create(t_label, gacha_back);
//	gachaNode->setScale(0.7f);
	gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::gachaAction), CCControlEventTouchUpInside);
	gacha_button->setPreferredSize(CCSizeMake(118, 45));
	gacha_button->setPosition(ccp(back_img->getContentSize().width/2.f, 30));
	back_img->addChild(gacha_button);
	gacha_button->setTouchPriority(-180);
	setFormSetter(gacha_label);
	
	
	cancel_menu = CommonButton::createCloseButton();
	cancel_menu->setTouchPriority(-180);
	cancel_menu->setFunction([=](CCObject* sender) {
		if(m_cancelGacha) {
			removeFromParent();
			m_cancelGacha();
		}
	});
	cancel_menu->setPosition(ccpFromSize(back_img->getContentSize()) + ccp(-25, -25));
	back_img->addChild(cancel_menu);
	CCFadeTo* gray_fade = CCFadeTo::create(0.25f, 255);
	m_grayBack->runAction(gray_fade);
	
	setFormSetter(cancel_menu);
	
	CommonAnimation::openPopup(this, m_container, nullptr);
	
	scheduleUpdate();
	return true;
}


void OnePercentTutorial::gachaAction(CCObject* sender, CCControlEvent t_event)
{
	if(m_tutorialStep == 1)
	{
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
		CCNode* t_parent = m_gachaDesc->getParent();
		CCPoint t_position = m_gachaDesc->getPosition();
		m_gachaDesc->removeFromParent();
		
		m_gachaDesc = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_onePercentTutorial4),
																				 mySGD->getFont().c_str(), 14, 999, StyledAlignment::kCenterAlignment);
		t_parent->addChild(m_gachaDesc, kOnePercentTutorial_Z_content);
		m_gachaDesc->setAnchorPoint(ccp(0.5f,0.5f));
		m_gachaDesc->setPosition(t_position); 			// dt (0.0, 71.5)
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