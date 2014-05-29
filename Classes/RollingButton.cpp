//
//  ScrollButton.cpp
//  DGproto
//
//  Created by LitQoo on 2014. 1. 20..
//
//

#include "RollingButton.h"

#include "MyLocalization.h"
bool RollingButton::init(string text){
		
		if(!CCLayer::init()){
			return false;
		}
		
		m_isOpened = false;
		this->m_openStdFunc = NULL;
		this->m_closeStdFunc = NULL;
		this->m_openTarget = NULL;
		this->m_closeTarget = NULL;
		
		
		
		m_openBtn = CCMenuItemImage::create("ui_mission_button_open.png", "ui_mission_button_open.png", this, menu_selector(RollingButton::doOpen));
		
		m_openBtn->setPosition(ccp(0,0));
		
		m_missionStr = CCSprite::create(CCString::createWithFormat("ui_mission_button_font_%s.png", myLoc->getSupportLocalCode())->getCString());
		m_missionStr->setPosition(ccp(30,19));
		m_openBtn->addChild(m_missionStr);
		
		m_closeBtn = CCMenuItemImage::create("ui_mission_button_close.png", "ui_mission_button_close.png", this, menu_selector(RollingButton::doClose));
		m_closeBtn->setPosition(ccp(0,0));
		m_closeBtn->setOpacity(0);
		m_closeBtn->setVisible(false);
		
		m_success = CCSprite::create(CCString::createWithFormat("ui_mission_clear_%s.png", myLoc->getSupportLocalCode())->getCString());
		
		m_success->setAnchorPoint(ccp(0,0));
		m_success->setPosition(ccp(0,0));
		m_success->setVisible(false);
		m_openBtn->addChild(m_success);
		
		m_menu = CCMenu::create(m_openBtn,m_closeBtn,NULL);
		m_menu->setPosition(ccp(0,0));
		this->addChild(m_menu,13);
		
		
		m_black = CommonButton::getBackgroundByType(CommonButtonGray2);
		m_black->setAnchorPoint(ccp(0.5f,0.5f));
		m_black->setPosition(ccp(0,0));
		m_black->setContentSize(CCSizeMake(15,14));
		this->addChild(m_black,1);
		
//		m_black_left = CCSprite::create("ui_mission_back_left.png");
//		m_black_left->setAnchorPoint(ccp(0.5,0.5));
//		m_black_left->setPosition(ccp(-11,0));
//		this->addChild(m_black_left,1);
//		
//		m_black_center = CCSprite::create("ui_mission_back_center.png");
//		m_black_center->setPosition(ccp(0,0));
//		this->addChild(m_black_center,1);
//		
//		
//		m_black_right = CCSprite::create("ui_mission_back_right.png");
//		m_black_right->setAnchorPoint(ccp(0.5,0.5));
//		m_black_right->setPosition(ccp(11,0));
//		this->addChild(m_black_right,1);
		
		
//		m_btnBackRight = CCSprite::create("ui_mission_button_back.png");
//		m_btnBackRight->setAnchorPoint(ccp(0.5f,0.5f));
//		m_btnBackRight->setPosition(ccp(0,0));
//		this->addChild(m_btnBackRight,2);
		
		
		// Setup scroll view
		
		m_maqueeLbl = LabelTTFMarquee::create(ccc4(160,0,0,0), 155, 14, text);
		m_maqueeLbl->setAnchorPoint(ccp(0.47,0.5));
		m_maqueeLbl->setPosition(ccp(3,0));
		m_maqueeLbl->setVisible(false);
		this->addChild(m_maqueeLbl,10);
		

//		CCSprite* m_btnBackLeft = CCSprite::create("ui_mission_button_open.png");
//		m_btnBackLeft->setPosition(ccp(16,18));
//		m_black_left->addChild(m_btnBackLeft,2);
		
				return true;
	}