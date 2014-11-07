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
	m_cursor = nullptr;
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
	
	back_img = CCSprite::create("ingame_back3.png");
	//back_img->setContentSize(CCSizeMake(309, 270));
	back_img->setPosition(ccp(0,0));
	m_container->addChild(back_img, kOnePercentTutorial_Z_back);
	
	CCClippingNode* clippingNode = CCClippingNode::create();
	m_container->addChild(clippingNode, kOnePercentTutorial_Z_back);

	CCScale9Sprite* innerStencil = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	innerStencil->setContentSize(CCSizeMake(450 / 2.f + 8.f, 176.5f - 5.f));
	innerStencil->setPosition(ccp(0.0, 6.0));
	clippingNode->setStencil(innerStencil);
	
	CCScale9Sprite* inner_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	inner_back->setContentSize(CCSizeMake(450 / 2.f + 8.f, 176.5f - 5.f));
	inner_back->setPosition(ccp(0.0, 6.0));
	clippingNode->addChild(inner_back);

	CCSprite* katsu = CCSprite::create("kt_cha_katsuragi_1.png");
	katsu->setScale(0.8f);
	katsu->setPosition(ccp(-80.5, -5));
	clippingNode->addChild(katsu);
	setFormSetter(katsu);
	//m_container->addChild(inner_back, kOnePercentTutorial_Z_back);
	
	setFormSetter(back_img);
	setFormSetter(inner_back);
	
//	KSLabelTTF* titleLbl = KSLabelTTF::create(getLocal(LK::kMyLocalKey_onePercentTutorial1), mySGD->getFont().c_str(), 12.f);
	KSLabelTTF* titleLbl = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_onePercentTutorial1), mySGD->getFont().c_str(), 12.f);
	titleLbl->disableOuterStroke();
	titleLbl->setPosition(ccp(89.0, 247.5));
//	titleLbl->setPosition(ccp(back_img->getContentSize().width/2.f, back_img->getContentSize().height-25)); 			// dt (6.0, 112.5)
	back_img->addChild(titleLbl);
	
	CommonAnimation::applyShadow(titleLbl, 12);
	setFormSetter(titleLbl);
	auto puppleInner = CCScale9Sprite::create("kt_talkbox_purple.png",
																						CCRectMake(0, 0, 170.f/2.f, 230/2.f),
																						CCRectMake(30, 115 - 30, 2, 2));

	setFormSetter(puppleInner);
	puppleInner->setPosition(ccp(26.5, 33.5));
	puppleInner->setScaleX(-1.0); 			// dt -2.0
	puppleInner->setContentSize(CCSizeMake(177.0, 131.0));


	m_container->addChild(puppleInner, kOnePercentTutorial_Z_back);
	setFormSetter(puppleInner);
	m_gachaDesc = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_onePercentTutorial2),
																			 mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
	m_container->addChild(m_gachaDesc, kOnePercentTutorial_Z_content);
	m_gachaDesc->setAnchorPoint(ccp(0.5f,0.5f));
	m_gachaDesc->setPosition(ccp(35.5, 33.5));
//	m_gachaDesc->setPosition(ccpFromSize(puppleInner->getContentSize()/2.f));
	setFormSetter(m_gachaDesc);
	

	
	
	CCScale9Sprite* gacha_back = CCScale9Sprite::create("subbutton_purple3.png" , CCRectMake(0,0, 100, 39), CCRectMake(57, 22, 2, 2));
	CCLabelTTF* t_label = CCLabelTTF::create();
	
	KSLabelTTF* gacha_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_onePercentTutorial3), mySGD->getFont().c_str(), 15);
	gacha_label->disableOuterStroke();
	t_label->addChild(gacha_label);

	gacha_button = CCControlButton::create(t_label, gacha_back);
//	gachaNode->setScale(0.7f);
	gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::gachaAction), CCControlEventTouchUpInside);
	gacha_button->setPreferredSize(CCSizeMake(350/2.f, 86 / 2.f));
	gacha_button->setPosition(ccp(back_img->getContentSize().width/2.f, 32.5));
	back_img->addChild(gacha_button);
	gacha_button->setTouchPriority(-180);
	setFormSetter(gacha_label);
	
	
	cancel_menu = CommonButton::createCloseButton();
	cancel_menu->setScale(0.85f);
	cancel_menu->setTouchPriority(-180);
	cancel_menu->setFunction([=](CCObject* sender) {
		if(m_cancelGacha) {
			m_cancelGacha();
			removeFromParent();
		}
	});
	
	cancel_menu->setPosition(ccpFromSize(back_img->getContentSize()) + ccp(-25 + 7.5f, -25 + 0.5f));
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
		
		m_gachaDesc = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_onePercentTutorial4),
																				 mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
		t_parent->addChild(m_gachaDesc, kOnePercentTutorial_Z_content);
		m_gachaDesc->setAnchorPoint(ccp(0.5f,0.5f));
		m_gachaDesc->setPosition(t_position); 			// dt (0.0, 71.5)
		m_tutorialStep++;
		
		CCScale9Sprite* progress_back = CCScale9Sprite::create("loading_progress_back.png", CCRectMake(0, 0, 13, 13), CCRectMake(6, 6, 1, 1));
		progress_back->setContentSize(CCSizeMake(201, 13));
		
		m_container->addChild(progress_back, kOnePercentTutorial_Z_content);
		progress_back->setPosition(ccp(0, -67));
		CCSprite* validArea = CCSprite::create("one_percent_graph100.png");
//		validArea->setTextureRect(CCRectMake(0, 0, 50, 15));
		validArea->setPosition(ccp(0, -67));
		m_container->addChild(validArea, kOnePercentTutorial_Z_content);
//		validArea->setColor(ccc3(232, 0, 192));
		
		setFormSetter(progress_back);
		setFormSetter(validArea);
		
		m_cursor = CCSprite::create("one_percent_gacha_02.png");
		m_cursor->setVisible(false);
		m_cursor->setPosition(ccp(0, -67));
		m_container->addChild(m_cursor, kOnePercentTutorial_Z_content);
		
		StyledLabelTTF* desc2 = StyledLabelTTF::create(
																									 CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_onePercentDesc2), 3, 0.3f)->getCString(),
																									 mySGD->getFont().c_str(), 10.f, 0, StyledAlignment::kCenterAlignment);
		m_container->addChild(desc2, kOnePercentTutorial_Z_content);
		desc2->setPosition(ccp(0.0, -39.0 - 5.f));
		setFormSetter(desc2);
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
	if(m_cursor)
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
}
