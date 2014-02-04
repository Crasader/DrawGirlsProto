//
//  CommonButton.h
//  DGproto
//
//  Created by LitQoo on 2014. 2. 3..
//
//

#ifndef __DGproto__CommonButton__
#define __DGproto__CommonButton__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "StarGoldData.h"
#include "KSUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


enum CommonButtonType {
		CommonButtonYellow,
		CommonButtonGray,
		CommonButtonBlue,
		CommonButtonGreen,
		CommonButtonOrange,
		CommonButtonPupple,
		CommonButtonClose
	};

enum PriceType{
	PriceTypeGold,
	PriceTypeRuby,
	PriceTypeSocial,
	PriceTypeCoin,
	PriceTypeMoney,
	PriceTypeNone
};


class CommonButton : public CCNode {
	CCControlButton* m_btn;
	std::function<void(CCObject*)> m_func;
	CCLabelTTF* m_btnTitle;
	CCScale9Sprite* m_btnBack;
	CommonButtonType m_btnType;
	int m_price;
	PriceType m_priceType;
	CCSprite* m_priceTypeSprite;
	CCLabelTTF* m_priceLbl;
public:
	
	void setFunction(std::function<void(CCObject*)> func){
		m_func=func;
	}
	
	static CommonButton* create(string title, int fontSize, CCSize size,CommonButtonType btnType, int touchPriority){
		CommonButton* btn = new CommonButton();
		if(btn->init(title,fontSize,size,btnType,touchPriority)==false){
			return NULL;
		}
		btn->autorelease();
		return btn;
	}
	
	static CommonButton* createCloseButton(){
		CommonButton* btn = new CommonButton();
		if(btn->init("",20,CCSizeMake(0, 0),CommonButtonClose,0)==false){
			return NULL;
		}
		btn->autorelease();
		return btn;
	}
	
	
	static CommonButton* createCloseButton(int touchPriority){
		CommonButton* btn = new CommonButton();
		if(btn->init("",20,CCSizeMake(0, 0),CommonButtonClose,touchPriority)==false){
			return NULL;
		}
		btn->autorelease();
		return btn;
	}
	
	static CommonButton* create(string title,CommonButtonType btnType){
		CommonButton* btn = new CommonButton();
		if(btn->init(title,20,CCSizeMake(0, 0),btnType,0)==false){
			return NULL;
		}
		btn->autorelease();
		return btn;
	}
	
	bool init(string title, int fontSize, CCSize size,CommonButtonType btnType, int touchPriority){
		
		if(CCNode::init()==false){
			return false;
		}
		
		m_priceType = PriceTypeNone;
		m_price=0;
		
		m_priceTypeSprite = NULL;
		m_priceLbl = NULL;
		
		this->setAnchorPoint(ccp(0.5f,0.5f));
		m_btnTitle = CCLabelTTF::create(title.c_str(), mySGD->getFont().c_str(), fontSize);
		
		string btnBackImg;
		
		m_btnType = btnType;
		
		if(btnType==CommonButtonYellow) btnBackImg = "common_button_yellow.png";
		else if(btnType==CommonButtonYellow) btnBackImg = "common_button_yellow.png";
		else if(btnType==CommonButtonGray) btnBackImg = "common_button_gray.png";
		else if(btnType==CommonButtonBlue) btnBackImg = "common_button_blue.png";
		else if(btnType==CommonButtonGreen) btnBackImg = "common_button_green.png";
		else if(btnType==CommonButtonOrange) btnBackImg = "common_button_orange.png";
		else if(btnType==CommonButtonPupple) btnBackImg = "common_button_pupple.png";
		else if(btnType==CommonButtonClose) btnBackImg = "common_button_close.png";
		
		m_btnBack = CCScale9Sprite::create(btnBackImg.c_str());
		
		
		m_btn = CCControlButton::create(m_btnTitle, m_btnBack);
		this->setButtonInset(btnType);
		
		
		if(size.height>0){
			m_btn->setPreferredSize(size);
		}
		
		m_btn->setAnchorPoint(ccp(0.5f,0.5f));
		m_btn->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::callFunc), CCControlEventTouchUpInside);
		m_btn->setPosition(m_btn->getContentSize().width/2, m_btn->getContentSize().height/2);
		if(touchPriority!=0)m_btn->setTouchPriority(touchPriority);
		addChild(m_btn,2);
		
		this->setContentSize(m_btn->getContentSize());
		
		
		return true;
	}
	
	void setButtonInset(CommonButtonType type){
		if(type == CommonButtonClose){
			this->setSize(CCSizeMake(36,36));
		}else{
			m_btnBack->setInsetBottom(12);
			m_btnBack->setInsetTop(38-12*2);
			m_btnBack->setInsetLeft(9);
			m_btnBack->setInsetRight(38-9*2);
		}
	}
	void callFunc(CCObject* obj, CCControlEvent event){
		if(m_func)m_func((CCObject*)this);
	}
	
	void setTouchPriority(int touchPriority){
		m_btn->setTouchPriority(touchPriority);
	}
	void setTitle(string title){
		m_btnTitle->setString(title.c_str());
	}
	
	void setTitleSize(int size){
		m_btnTitle->setFontSize(size);
	}
	
	void setTitleColor(ccColor3B color){
		m_btnTitle->setFontFillColor(color);
	}
	
	void setSize(CCSize size){
		m_btn->setPreferredSize(size);
		this->setContentSize(size);
		m_btn->setPosition(m_btn->getContentSize().width/2, m_btn->getContentSize().height/2);
	}
	
	void setEnabled(bool isEnabled){
		m_btn->setEnabled(isEnabled);
	}
	
	bool isEnabled(){
		return m_btn->isEnabled();
	}
	
	void setSelected(bool isSelected){
		m_btn->setSelected(isSelected);
	}
	
	bool isSelected(){
		return m_btn->isSelected();
	}
	
	void setTitleForState(cocos2d::CCString *title, CCControlState state){
		m_btn->setTitleForState(title, state);
	}
	
	void setBackgroundSpriteForState(cocos2d::extension::CCScale9Sprite *sprite, CCControlState state){
		m_btn->setBackgroundSpriteForState(sprite, state);
	}
	
	void setTitleColorForState(ccColor3B color, CCControlState state){
		m_btn->setTitleColorForState(color,state);
	}
	
	void setPrice(PriceType priceType, int price){
		m_priceType=priceType;
		m_price = price;
		
		
		if(m_priceLbl==NULL){
			m_priceLbl=CCLabelTTF::create(CCString::createWithFormat("%d",m_price)->getCString(), mySGD->getFont().c_str(), 13);
			m_priceLbl->setAnchorPoint(ccp(0.5,0));
			m_priceLbl->setPosition(ccp(this->getContentSize().width/2+5,7));
			m_btnTitle->setPositionY(m_btnTitle->getPositionY()+8);
			addChild(m_priceLbl,10);
		}else{
			m_priceLbl->setString(CCString::createWithFormat("%d",m_price)->getCString());
		}
		
		if(m_priceTypeSprite==NULL && m_priceType!=PriceTypeNone){
			string priceTypeImg = "common_button_gold.png";
			if(m_priceType == PriceTypeCoin)priceTypeImg="common_button_coin.png";
			else if(m_priceType == PriceTypeGold)priceTypeImg="common_button_gold.png";
			else if(m_priceType == PriceTypeSocial)priceTypeImg="common_button_social.png";
			else if(m_priceType == PriceTypeRuby)priceTypeImg="common_button_ruby.png";
			
			m_priceTypeSprite = CCSprite::create(priceTypeImg.c_str());
			m_priceTypeSprite->setScale(0.9);
			m_priceTypeSprite->setPosition(ccp(m_priceLbl->getPositionX()-m_priceLbl->getContentSize().width/2-10+2,m_priceLbl->getPositionY()+m_priceLbl->getContentSize().height/2));
			addChild(m_priceTypeSprite,11);
		}
	}
	
	int getPrice(){
		return m_price;
	}
	
	PriceType getPriceType(){
		return m_priceType;
	}
	
	
	
};
#endif /* defined(__DGproto__CommonButton__) */
