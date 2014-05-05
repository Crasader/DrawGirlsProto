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
#include "MyLocalization.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


enum CommonButtonType {
		CommonButtonYellow,
	CommonButtonGray,
	CommonButtonGray2,
		CommonButtonBlue,
		CommonButtonGreen,
		CommonButtonOrange,
		CommonButtonPupple,
	CommonButtonDarkPupple,
	CommonButtonLightPupple,
	CommonButtonYellowDown,
	CommonButtonYellowUp,
		CommonButtonClose,
	CommonButtonFree
	};

enum PriceType{
	PriceTypeGold,
	PriceTypeRuby,
	PriceTypeSocial,
	PriceTypeCoin,
	PriceTypeMoney,
	PriceTypePass1,
	PriceTypePass2,
	PriceTypePass3,
	PriceTypePass4,
	PriceTypePass5,
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
	CCLabelTTF* m_priceLbl;
	float m_fontSize;
	string m_title;
	ccColor3B m_titleColorNomal;
	ccColor3B m_titleColorDisable;
public:
	CCSprite* m_priceTypeSprite;
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
	
	static CommonButton* create(string title, int fontSize, CCSize size, CCScale9Sprite* button_back, int touchPriority){
		CommonButton* btn = new CommonButton();
		if(btn->init(title, fontSize, size, button_back, touchPriority) == false){
			return NULL;
		}
		btn->autorelease();
		return btn;
	}
	
	static CommonButton* create(string title, CCScale9Sprite* button_back){
		CommonButton* btn = new CommonButton();
		if(btn->init(title,20,CCSizeMake(0, 0),button_back,0)==false){
			return NULL;
		}
		btn->autorelease();
		return btn;
	}
	
	bool init(string title, int fontSize, CCSize size,CommonButtonType btnType, int touchPriority){
		
		if(CCNode::init()==false){
			return false;
		}
		m_titleColorNomal=ccc3(255,255,255);
		m_titleColorDisable=ccc3(255,255,255);
		
		m_priceType = PriceTypeNone;
		m_price=0;
		
		m_priceTypeSprite = NULL;
		m_priceLbl = NULL;
		
		m_fontSize=fontSize;
		m_title=title;
		this->setAnchorPoint(ccp(0.5f,0.5f));
		m_btnTitle = CCLabelTTF::create(title.c_str(), mySGD->getFont().c_str(), fontSize);
		
		m_btnType = btnType;
		
		m_btnBack = CommonButton::getBackgroundByType(btnType);
		
		CCLabelTTF* titleNode = CCLabelTTF::create();
		
		titleNode->addChild(m_btnTitle);
		
		m_btn = CCControlButton::create(titleNode, m_btnBack);
		
		if(btnType == CommonButtonClose){
			this->setSize(CCSizeMake(45,45));
		}

		
		if(size.height>0){
			m_btn->setPreferredSize(size);
		}else{
			m_btn->setMargins(10, 5);
		}
		
		m_btn->setAnchorPoint(ccp(0.5f,0.5f));
		m_btn->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::callFunc), CCControlEventTouchUpInside);
		m_btn->setPosition(m_btn->getContentSize().width/2, m_btn->getContentSize().height/2);
		if(touchPriority!=0)m_btn->setTouchPriority(touchPriority);
		addChild(m_btn,2);
		this->setContentSize(m_btn->getContentSize());
		
		
		return true;
	}
	
	bool init(string title, int fontSize, CCSize size, CCScale9Sprite* button_back, int touchPriority){
		if(CCNode::init()==false){
			return false;
		}
		m_titleColorNomal=ccc3(255,255,255);
		m_titleColorDisable=ccc3(255,255,255);
		
		m_priceType = PriceTypeNone;
		m_price=0;
		
		m_priceTypeSprite = NULL;
		m_priceLbl = NULL;
		
		m_fontSize=fontSize;
		m_title=title;
		this->setAnchorPoint(ccp(0.5f,0.5f));
		
		m_btnTitle = CCLabelTTF::create(title.c_str(), mySGD->getFont().c_str(), fontSize);
		
		CCLabelTTF* titleNode = CCLabelTTF::create();
		titleNode->addChild(m_btnTitle);
		
		m_btnType = CommonButtonFree;
		
		m_btnBack = button_back;
		
		
		m_btn = CCControlButton::create(titleNode, m_btnBack);
		
		if(size.height>0){
			m_btn->setPreferredSize(size);
		}else{
			m_btn->setMargins(10, 5);
		}
		
		m_btn->setAnchorPoint(ccp(0.5f,0.5f));
		m_btn->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::callFunc), CCControlEventTouchUpInside);
		m_btn->setPosition(m_btn->getContentSize().width/2, m_btn->getContentSize().height/2);
		if(touchPriority!=0)m_btn->setTouchPriority(touchPriority);
		addChild(m_btn,2);
		this->setContentSize(m_btn->getContentSize());
		
		
		return true;
	}
	
	static CCScale9Sprite* getBackgroundByType(CommonButtonType btnType){
		string btnBackImg;
		if(btnType==CommonButtonYellow) btnBackImg = "common_button_yellow.png";
		else if(btnType==CommonButtonYellow) btnBackImg = "common_button_yellow.png";
		else if(btnType==CommonButtonGray) btnBackImg = "common_button_gray.png";
		else if(btnType==CommonButtonGray2) btnBackImg = "common_button_gray2.png";
		else if(btnType==CommonButtonBlue) btnBackImg = "common_button_blue.png";
		else if(btnType==CommonButtonGreen) btnBackImg = "common_button_green.png";
		else if(btnType==CommonButtonOrange) btnBackImg = "common_button_orange.png";
		else if(btnType==CommonButtonPupple) btnBackImg = "common_button_pupple.png";
		else if(btnType==CommonButtonDarkPupple)	btnBackImg = "common_button_darkpupple.png";
		else if(btnType==CommonButtonLightPupple)	btnBackImg = "common_button_lightpupple.png";
		else if(btnType==CommonButtonYellowDown)	btnBackImg = "common_button_yellowdown.png";
		else if(btnType==CommonButtonYellowUp)		btnBackImg = "common_button_yellowup.png";
		else if(btnType==CommonButtonClose) btnBackImg = "common_button_close.png";
		
		CCScale9Sprite* back = CCScale9Sprite::create(btnBackImg.c_str());
		
		if(btnType == CommonButtonGray2){
			back->setInsetBottom(9);
			back->setInsetTop(9);
			back->setInsetLeft(9);
			back->setInsetRight(9);
			
		}
		else if(btnType >= CommonButtonDarkPupple && btnType <= CommonButtonYellowUp)
		{
			back->setInsetBottom(16);
			back->setInsetTop(34-16*2);
			back->setInsetLeft(13);
			back->setInsetRight(34-13*2);
		}
		else if(btnType != CommonButtonClose){
			back->setInsetBottom(12);
			back->setInsetTop(38-12*2);
			back->setInsetLeft(9);
			back->setInsetRight(38-9*2);
		}
		
		return back;
	}
	
	void setButtonInset(CommonButtonType type){
		if(type == CommonButtonClose){
			this->setSize(CCSizeMake(45,45));
		}
		else if(type >= CommonButtonDarkPupple && type <= CommonButtonYellowUp)
		{
			m_btnBack->setInsetBottom(16);
			m_btnBack->setInsetTop(34-16*2);
			m_btnBack->setInsetLeft(13);
			m_btnBack->setInsetRight(34-13*2);
		}
		else{
			m_btnBack->setInsetBottom(12);
			m_btnBack->setInsetTop(38-12*2);
			m_btnBack->setInsetLeft(9);
			m_btnBack->setInsetRight(38-9*2);
		}
	}
	void callFunc(CCObject* obj, CCControlEvent event){
		if(m_priceLbl!=NULL)m_btnTitle->setPositionY(m_btnTitle->getPositionY()+8);
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
	

	void setSize(CCSize size){
		m_btn->setPreferredSize(size);
		this->setContentSize(size);
		m_btn->setPosition(m_btn->getContentSize().width/2, m_btn->getContentSize().height/2);
	}
	
	void setEnabled(bool isEnabled){
		m_btn->setEnabled(isEnabled);
		if(isEnabled){
			this->m_btnTitle->setColor(m_titleColorNomal);
			if(m_priceLbl)
				m_priceLbl->setColor(m_titleColorNomal);
		}else{
			this->m_btnTitle->setColor(m_titleColorDisable);
			if(m_priceLbl)
				m_priceLbl->setColor(m_titleColorDisable);
		}
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
	
	void setBackgroundTypeForDisabled(CommonButtonType type){
		this->setBackgroundTypeForState(type,CCControlStateDisabled);
		
	}
	
	void setBackgroundTypeForState(CommonButtonType type,CCControlState state){
		CCScale9Sprite* back = CommonButton::getBackgroundByType(type);
		this->setBackgroundSpriteForState(back,state);
		
	}

	void setTitleColor(ccColor3B color){
		m_titleColorNomal=color;
		m_btnTitle->setColor(color);
		if(m_priceLbl!=NULL)this->m_priceLbl->setColor(color);
	}
	
	
	void setTitleColorForDisable(ccColor3B color){
		m_titleColorDisable=color;
	}
	
	void setPrice(PriceType priceType, int price){
		m_priceType=priceType;
		m_price = price;
		
		
		if(m_priceLbl==NULL){
			if(m_priceType >= PriceTypePass1 && m_priceType <= PriceTypePass5)
				m_priceLbl=CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_free), mySGD->getFont().c_str(), 13);
			else
				m_priceLbl=CCLabelTTF::create(CCString::createWithFormat("%d",m_price)->getCString(), mySGD->getFont().c_str(), 13);
			
			if(isEnabled()){
				this->m_priceLbl->setColor(m_titleColorNomal);
			}else{
				this->m_priceLbl->setColor(m_titleColorDisable);
			}
			
			m_priceLbl->setAnchorPoint(ccp(0.5,0.5));
			m_priceLbl->setPosition(ccp(this->getContentSize().width/2+5,getContentSize().height/2.f-m_priceLbl->getContentSize().height/2.f-1));
			m_btnTitle->setPositionY(m_priceLbl->getContentSize().height/2.f+1);
			addChild(m_priceLbl,10);
		}else{
			if(m_priceType >= PriceTypePass1 && m_priceType <= PriceTypePass5)
				m_priceLbl->setString(myLoc->getLocalForKey(kMyLocalKey_free));
			else
				m_priceLbl->setString(CCString::createWithFormat("%d",m_price)->getCString());
		}
		
		if(m_priceType!=PriceTypeNone){
			
			if(m_priceTypeSprite)
			{
				m_priceTypeSprite->removeFromParent();
				m_priceTypeSprite = NULL;
			}
			
			string priceTypeImg = "price_gold_img.png";
			if(m_priceType == PriceTypeCoin)priceTypeImg="common_button_coin.png";
			else if(m_priceType == PriceTypeGold)priceTypeImg="price_gold_img.png";
			else if(m_priceType == PriceTypeSocial)priceTypeImg="common_button_social.png";
			else if(m_priceType == PriceTypeRuby)priceTypeImg="price_ruby_img.png";
			else if(m_priceType == PriceTypePass1)priceTypeImg="pass_ticket1.png";
			else if(m_priceType == PriceTypePass2)priceTypeImg="pass_ticket2.png";
			else if(m_priceType == PriceTypePass3)priceTypeImg="pass_ticket3.png";
			else if(m_priceType == PriceTypePass4)priceTypeImg="pass_ticket4.png";
			else if(m_priceType == PriceTypePass5)priceTypeImg="pass_ticket5.png";
			
			m_priceTypeSprite = CCSprite::create(priceTypeImg.c_str());
			m_priceTypeSprite->setScale(0.9);
			m_priceTypeSprite->setPosition(ccp(m_priceLbl->getPositionX()-m_priceLbl->getContentSize().width/2-10+2,getContentSize().height/2.f-m_priceTypeSprite->getContentSize().height/2.f-1));
			addChild(m_priceTypeSprite,11);
		}
		
		float w1 = m_priceTypeSprite->getContentSize().height;
		float w2 = m_priceLbl->getContentSize().width;
		
		m_priceTypeSprite->setPositionX(getContentSize().width/2.f-w2/2.f);
		m_priceLbl->setPositionX(getContentSize().width/2.f+w1/2.f);
	}
	
	int getPrice(){
		return m_price;
	}
	
	PriceType getPriceType(){
		return m_priceType;
	}
	
	void setOpacity(float opacity){
			m_btn->setOpacity(opacity);
	}
	
	
};
#endif /* defined(__DGproto__CommonButton__) */
