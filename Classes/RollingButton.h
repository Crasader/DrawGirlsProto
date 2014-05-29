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
	CCSprite* m_missionStr;
	CCScale9Sprite* m_black;
	LabelTTFMarquee* m_maqueeLbl;
	
	CCObject* m_openTarget;
	SEL_CallFuncN	m_openFunc;
	CCObject* m_closeTarget;
	SEL_CallFuncN	m_closeFunc;
	
	std::function<void(void)> m_openStdFunc;
	std::function<void(void)> m_closeStdFunc;
	
	bool m_isOpened;
	
	
	bool init(string text);
	

	
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
			m_missionStr->setVisible(false);
		}else{
			m_success->setVisible(false);
			m_missionStr->setVisible(true);
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
