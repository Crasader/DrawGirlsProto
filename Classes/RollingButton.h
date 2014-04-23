//
//  ScrollButton.h
//  DGproto
//
//  Created by LitQoo on 2014. 1. 20..
//
//

#ifndef __DGproto__ScrollButton__
#define __DGproto__ScrollButton__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "LabelTTFMarquee.h"
#include "KSUtil.h"
#include "CommonButton.h"
using namespace cocos2d;
using namespace std;
using namespace cocos2d::extension;

class RollingButton : public CCLayer {
		
public:
	CCMenuItemImage* m_openBtn;
	CCMenuItemImage* m_closeBtn;
	
	CCSprite* m_success;
	
	CCMenu* m_menu;
	
//	CCSprite* m_black_left;
//	CCSprite* m_black_center;
//	CCSprite* m_black_right;
	CCSprite* m_btnBackRight;
	
	CCScale9Sprite* m_black;
	LabelTTFMarquee* m_maqueeLbl;
	
	CCObject* m_openTarget;
	SEL_CallFuncN	m_openFunc;
	CCObject* m_closeTarget;
	SEL_CallFuncN	m_closeFunc;
	
	std::function<void(void)> m_openStdFunc;
	std::function<void(void)> m_closeStdFunc;
	
	bool m_isOpened;
	
	
	bool init(string text){
		
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
		
		CCSprite* missionStr = CCSprite::create("ui_mission_button_font_ko.png");
		missionStr->setPosition(ccp(30,19));
		m_openBtn->addChild(missionStr);
		
		m_closeBtn = CCMenuItemImage::create("ui_mission_button_close.png", "ui_mission_button_close.png", this, menu_selector(RollingButton::doClose));
		m_closeBtn->setPosition(ccp(0,0));
		m_closeBtn->setOpacity(0);
		m_closeBtn->setVisible(false);
		
		m_success = CCSprite::create("ui_mission_clear.png");
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
		
		m_maqueeLbl = LabelTTFMarquee::create(ccc4(160,0,0,0), 155, 12, text);
		m_maqueeLbl->setAnchorPoint(ccp(0.47,0.5));
		m_maqueeLbl->setPosition(ccp(3,0));
		m_maqueeLbl->setVisible(false);
		this->addChild(m_maqueeLbl,10);
		

//		CCSprite* m_btnBackLeft = CCSprite::create("ui_mission_button_open.png");
//		m_btnBackLeft->setPosition(ccp(16,18));
//		m_black_left->addChild(m_btnBackLeft,2);
		
				return true;
	}
	

	
	static RollingButton* create(string text)
	{
		RollingButton* _btn = new RollingButton();
		_btn->init(text);
		_btn->autorelease();
		
		return _btn;
	}
	
	void setTouchPriority(int priority){
		m_menu->setTouchPriority(priority);
	}
	void setOpenTargetAndSelector(CCObject* target, SEL_CallFuncN delegate){
		m_openTarget = target;
		m_openFunc = delegate;
		
	}
	
	void setCloseTargetAndSelector(CCObject* target, SEL_CallFuncN delegate){
		m_closeTarget = target;
		m_closeFunc = delegate;
	}
	
	void setOpenFunction(std::function<void(void)> func){
		this->m_openStdFunc = func;
	}
	
	void setCloseFunction(std::function<void(void)> func){
		this->m_closeStdFunc = func;
	}
	
	int addText(string text){
		return this->m_maqueeLbl->addText(text);
	}
	
	bool setTextAtIndex(string text, int index){
		return this->m_maqueeLbl->setTextAtIndex(text,index);
	}
	
	void startMarquee(){
		this->m_maqueeLbl->startMarquee();
	}
	
	void stopMarqueeAtNowIndex(){
		this->m_maqueeLbl->stopMarqueeAtNowIndex();
	}
	
	void stopMarqueeAtIndex(int index){
		this->m_maqueeLbl->stopMarqueeAtIndex(index);
	}
	
	void isSuccessed(bool isSuccessed){
		if(isSuccessed){
			m_success->setVisible(true);
		}else{
			m_success->setVisible(false);
		}
	}
	
	void doOpen(){
		if(m_isOpened==true)return;
		
		this->addChild(KSGradualValue<float>::create(38,218,0.3f,[=](float t){
			m_black->setContentSize(ccp(t,28));
		}, nullptr));

//		{
//			CCFiniteTimeAction* ani  = CCMoveBy::create(0.3, ccp(-90,0));
//			m_black_left->runAction(ani);
//		}
		{
			CCFiniteTimeAction* ani  = CCMoveBy::create(0.3, ccp(-90,0));
			m_openBtn->runAction(ani);
		}
//		{
//			CCFiniteTimeAction* ani  = CCMoveBy::create(0.3, ccp(90,0));
//			m_btnBackRight->runAction(ani);
//		}
//		{
//			CCFiniteTimeAction* ani  = CCScaleTo::create(0.3, 15, 1);
//			m_black_center->runAction(ani);
//		}
		{
			m_closeBtn->setVisible(true);
			CCFiniteTimeAction* ani  = CCSpawn::create(CCMoveBy::create(0.3, ccp(95,0)),CCFadeIn::create(0.5),NULL);
			m_closeBtn->runAction(ani);
		}
		{
			addChild(KSTimer::create(0.3, [=](){
				m_maqueeLbl->setVisible(true);
			}));
		}
		m_isOpened=true;
		
		if(m_openTarget)(m_openTarget->*m_openFunc)(this);
		if(m_openStdFunc)m_openStdFunc();
	}
	LabelTTFMarquee* getLabelTTFMarquee(){
		return this->m_maqueeLbl;
	}
	void doClose(){
		if(m_isOpened==false)return;
		
		
		this->addChild(KSGradualValue<float>::create(218,15,0.3f,[=](float t){
			m_black->setContentSize(ccp(t,15));
		}, nullptr));
		
//		{
//			CCFiniteTimeAction* ani  = CCMoveBy::create(0.3, ccp(90,0));
//			m_black_left->runAction(ani);
//		}
		{
			CCFiniteTimeAction* ani  = CCMoveBy::create(0.3, ccp(90,0));
			m_openBtn->runAction(ani);
		}
//		}
//		{
//			CCFiniteTimeAction* ani  = CCMoveBy::create(0.3, ccp(-90,0));
//			m_btnBackRight->runAction(ani);
//		}
//		{
//			CCFiniteTimeAction* ani  = CCScaleTo::create(0.3, 1, 1);
//			m_black_center->runAction(ani);
//		}
		{
			m_maqueeLbl->setVisible(false);
		}
		{
			m_closeBtn->setVisible(false);
			CCFiniteTimeAction* ani  = CCSpawn::create(CCMoveBy::create(0.3, ccp(-95,0)),CCFadeIn::create(0.5),NULL);
			m_closeBtn->runAction(ani);
		}
		m_isOpened=false;
		
		
		if(m_closeTarget)(m_closeTarget->*m_closeFunc)(this);
		if(m_closeStdFunc)m_closeStdFunc();
	}
};

#endif /* defined(__DGproto__ScrollButton__) */
