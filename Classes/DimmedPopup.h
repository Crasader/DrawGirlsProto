//
//  DimmedPopup.h
//  DGproto
//
//  Created by LitQoo on 2014. 2. 4..
//
//

#ifndef __DGproto__DimmedPopup__
#define __DGproto__DimmedPopup__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "KSUtil.h"
#include "DataStorageHub.h"
#include "AudioEngine.h"
USING_NS_CC;


class DimmedPopup : public CCLayer {
	CCSprite* m_gray;
	CCNode* m_back;
	bool m_isEnable;
	CCObject* m_target_final;
	SEL_CallFunc m_delegate_final;
	bool m_isOpenAni;
public:
	virtual ~DimmedPopup(){
		
	}
	
	virtual void onEnter(){
		CCLayer::onEnter();
		this->showPopup();
	}
	
	CCObject* getTarget(){
		return m_target_final;
	}

	
	static DimmedPopup* create(){
		DimmedPopup* pop = DimmedPopup::create();
		pop->init();
		pop->autorelease();
		return pop;
	}
	
	virtual bool init(){
		
		if(CCLayer::init()==false)return false;
		
		m_isEnable=false;
		m_isOpenAni=true;
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		m_gray = CCSprite::create("back_gray.png");
		m_gray->setOpacity(0);
		m_gray->setPosition(ccp(240,160));
		m_gray->setScaleX(screen_scale_x);
		m_gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChildOnDimmed(m_gray, 0);
		
		return true;
	}

	void setBackground(CCScale9Sprite* sprite){

		m_back = sprite;
		m_back->setAnchorPoint(ccp(0.5,0.5));
		m_back->setPosition(ccp(240,160-14-450));
		addChildOnDimmed(m_back, 0);
	}
	void setBackground(CCSprite* sprite){
		this->setBackground(sprite, 0);
	}
	
	void setOpenAnimation(bool doOpen){
		m_isOpenAni=doOpen;
	}
	
	void setBackground(CCSprite* sprite,float yOffset){
		m_back = sprite;
		m_back->setAnchorPoint(ccp(0.5,0.5));
		m_back->setPosition(ccp(240,160-14+yOffset-450));
		addChildOnDimmed(m_back, 0);
	}
	
	void setBackground(string backImg){
		CCSprite* back = CCSprite::create(backImg.c_str());
		this->setBackground(back, 0);
	}
	
	
	void showPopup()
	{
		if(m_isOpenAni){
			CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
			m_gray->runAction(gray_fade);

			CCMoveBy* main_move = CCMoveBy::create(0.5f, ccp(0,450));
			CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(ThisClassType::endShowPopup));
			CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
			m_back->runAction(main_seq);
		}else{
			m_back->setPositionY(m_back->getPositionY()+450);
			m_gray->setOpacity(255);
			endShowPopup();
		}
	}
	
	void endShowPopup()
	{
		m_isEnable = true;
	}
	
	void hidePopup()
	{
		m_isEnable = false;
		
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
		CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
		m_gray->runAction(gray_fade);
		
		CCMoveBy* main_move = CCMoveBy::create(0.5f, ccp(0,-450));
		CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(ThisClassType::endHidePopup));
		CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
		m_back->runAction(main_seq);
	}
	
	void endHidePopup()
	{
		if(m_target_final)
			(m_target_final->*m_delegate_final)();
		
		removeFromParent();
	}
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final){
			m_target_final = t_final;
			m_delegate_final = d_final;
	}
	
	virtual void addChild(CCNode *child, int order){
		m_back->addChild(child,order);
	}
	
	virtual void addChild(CCNode *child){
		m_back->addChild(child);
	}
	
	void addChildOnDimmed(CCNode *child,int order){
		CCLayer::addChild(child,order);
	}
};
#endif /* defined(__DGproto__DimmedPopup__) */
