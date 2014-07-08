//
//  CommonButton.cpp
//  DGproto
//
//  Created by LitQoo on 2014. 2. 3..
//
//

#include "CommonButton.h"

#include "MyLocalization.h"

#include "KSLabelTTF.h"
bool CommonButton::init(CCSprite* backSprite, int touchPriority){
	if(CCNode::init()==false){
		return false;
	}
	m_titleColorNomal=ccc3(255,255,255);
	m_titleColorDisable=ccc3(255,255,255);
	
	m_priceType = PriceTypeNone;
	m_price=0;
	
	m_priceTypeSprite = NULL;
	m_priceLbl = NULL;
	
	m_fontSize=13;
	m_title="";
	this->setAnchorPoint(ccp(0.5f,0.5f));
	
	m_btnTitle = KSLabelTTF::create(m_title.c_str(), mySGD->getFont().c_str(), m_fontSize);
	
	CCLabelTTF* titleNode = CCLabelTTF::create();
	titleNode->addChild(m_btnTitle);
	
	m_btnType = CommonButtonFree;
	
	m_btnBack = CCScale9Sprite::create("_block.png");
	m_btnBack->setTag(119);
	m_btnBack->setContentSize(CCSizeZero);
	m_btn = CCControlButton::create(titleNode, m_btnBack);
	m_btn->setPreferredSize(backSprite->getContentSize());
	
	this->setContentSize(m_btn->getContentSize());
	m_btn->setAnchorPoint(ccp(0.5f,0.5f));
	m_btn->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::callFunc), CCControlEventTouchUpInside);
	m_btn->setPosition(m_btn->getContentSize().width/2, m_btn->getContentSize().height/2);
	if(touchPriority!=0)m_btn->setTouchPriority(touchPriority);
	addChild(m_btn,2);
	backSprite->setAnchorPoint(ccp(0,0));
	backSprite->setTag(112);
	addChild(backSprite,1);
	
	//	CCLayerColor* lc = CCLayerColor::create(ccc4(255, 0, 0, 100), size.width, size.height);
	//	this->addChild(lc);
	//	lc->setTag(999);
	return true;
}



bool CommonButton::init(string title, int fontSize, CCSize size,CommonButtonType btnType, int touchPriority){
	
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
	m_btnTitle = KSLabelTTF::create(title.c_str(), mySGD->getFont().c_str(), fontSize);
	
	m_btnType = btnType;
	
	m_btnBack = CommonButton::getBackgroundByType(btnType);
	
	CCLabelTTF* titleNode = CCLabelTTF::create();
	
	titleNode->addChild(m_btnTitle);
	
	m_btn = CCControlButton::create(titleNode, m_btnBack);
	
	if(btnType == CommonButtonClose){
		this->setSize(CCSizeMake(55,55));
	}
	
	
	if(size.height>0){
		m_btn->setPreferredSize(size);
	}else{
		m_btn->setMargins(10, 5);
	}
	
	this->setContentSize(m_btn->getContentSize());
	
	m_btn->setAnchorPoint(ccp(0.5f,0.5f));
	m_btn->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::callFunc), CCControlEventTouchUpInside);
	m_btn->setPosition(m_btn->getContentSize().width/2, m_btn->getContentSize().height/2);
	if(touchPriority!=0)m_btn->setTouchPriority(touchPriority);
	addChild(m_btn,2);
	
//	CCLayerColor* lc = CCLayerColor::create(ccc4(255, 0, 0, 100), size.width, size.height);
//	this->addChild(lc);
//	lc->setTag(999);
	return true;
}

bool CommonButton::init(string title, int fontSize, CCSize size, CCScale9Sprite* button_back, int touchPriority){
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
	
	m_btnTitle = KSLabelTTF::create(title.c_str(), mySGD->getFont().c_str(), fontSize);
	
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
	this->setContentSize(m_btn->getContentSize());
	m_btn->setAnchorPoint(ccp(0.5f,0.5f));
	m_btn->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::callFunc), CCControlEventTouchUpInside);
	m_btn->setPosition(m_btn->getContentSize().width/2, m_btn->getContentSize().height/2);
	if(touchPriority!=0)m_btn->setTouchPriority(touchPriority);
	addChild(m_btn,2);
	
	
	
//	CCLayerColor* lc = CCLayerColor::create(ccc4(255, 0, 0, 100), size.width, size.height);
//	this->addChild(lc);
//	lc->setTag(999);
	return true;
}

CCScale9Sprite* CommonButton::getBackgroundByType(CommonButtonType btnType){
	
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
		back->setInsetBottom(12);
		back->setInsetTop(34-12*2);
		back->setInsetLeft(12);
		back->setInsetRight(34-12*2);
	}
	else if(btnType != CommonButtonClose){
		back->setInsetBottom(12);
		back->setInsetTop(38-12*2);
		back->setInsetLeft(9);
		back->setInsetRight(38-9*2);
	}
	
	return back;
}

void CommonButton::setButtonInset(CommonButtonType type){
	if(type == CommonButtonClose){
		this->setSize(CCSizeMake(55,55));
	}
	else if(type >= CommonButtonDarkPupple && type <= CommonButtonYellowUp)
	{
		m_btnBack->setInsetBottom(12);
		m_btnBack->setInsetTop(34-12*2);
		m_btnBack->setInsetLeft(12);
		m_btnBack->setInsetRight(34-12*2);
	}
	else{
		m_btnBack->setInsetBottom(12);
		m_btnBack->setInsetTop(38-12*2);
		m_btnBack->setInsetLeft(9);
		m_btnBack->setInsetRight(38-9*2);
	}
}

void CommonButton::callFunc(CCObject* obj, CCControlEvent event){
	if(m_priceLbl!=NULL)m_btnTitle->setPositionY(m_priceLbl->getContentSize().height/2.f+2);
	if(m_func)m_func((CCObject*)this);
	
}

void CommonButton::setTouchPriority(int touchPriority){
	m_btn->setTouchPriority(touchPriority);
}
void CommonButton::setTitle(string title){
	m_btnTitle->setString(title.c_str());
}

void CommonButton::setTitleSize(int size){
	m_btnTitle->setFontSize(size);
}


void CommonButton::setSize(CCSize size){
	this->setContentSize(size);
	m_btn->setPreferredSize(size);
	m_btnBack->setPreferredSize(size);
	m_btn->setPosition(m_btn->getContentSize().width/2, m_btn->getContentSize().height/2);
	CCLayerColor* b = (CCLayerColor*)this->getChildByTag(999);
	if(b)b->setContentSize(size);
}

void CommonButton::setEnabled(bool isEnabled){
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

bool CommonButton::isEnabled(){
	return m_btn->isEnabled();
}

void CommonButton::setSelected(bool isSelected){
	m_btn->setSelected(isSelected);
}

bool CommonButton::isSelected(){
	return m_btn->isSelected();
}

void CommonButton::setTitleForState(cocos2d::CCString *title, CCControlState state){
	m_btn->setTitleForState(title, state);
}


void CommonButton::setBackgroundSpriteForState(cocos2d::extension::CCScale9Sprite *sprite, CCControlState state){
	m_btn->setBackgroundSpriteForState(sprite, state);
}

void CommonButton::setBackgroundTypeForDisabled(CommonButtonType type){
	this->setBackgroundTypeForState(type,CCControlStateDisabled);
	
}

void CommonButton::setBackgroundTypeForState(CommonButtonType type,CCControlState state){
	CCScale9Sprite* back = CommonButton::getBackgroundByType(type);
	this->setBackgroundSpriteForState(back,state);
	
}

void CommonButton::setTitleColor(ccColor3B color){
	m_titleColorNomal=color;
	m_btnTitle->setColor(color);
	if(m_priceLbl!=NULL)this->m_priceLbl->setColor(color);
}


void CommonButton::setTitleColorForDisable(ccColor3B color){
	m_titleColorDisable=color;
}

void CommonButton::setPrice(PriceType priceType, int price){
	m_priceType=priceType;
	m_price = price;
	
	
	if(m_priceLbl==NULL){
		if(m_priceType >= PriceTypePass1 && m_priceType <= PriceTypePass5)
			m_priceLbl=KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_free), mySGD->getFont().c_str(), 13);
		else
			m_priceLbl=KSLabelTTF::create(CCString::createWithFormat("%d",m_price)->getCString(), mySGD->getFont().c_str(), 13);
		
		if(isEnabled()){
			this->m_priceLbl->setColor(m_titleColorNomal);
		}else{
			this->m_priceLbl->setColor(m_titleColorDisable);
		}
		
		m_priceLbl->setAnchorPoint(ccp(0.5,0.5));
		m_priceLbl->setPosition(ccp(this->getContentSize().width/2+5,getContentSize().height/2.f-m_priceLbl->getContentSize().height/2.f-2));
		m_btnTitle->setPositionY(m_priceLbl->getContentSize().height/2.f+2);
		m_btn->addChild(m_priceLbl,10);
	}else{
		if(m_priceType >= PriceTypePass1 && m_priceType <= PriceTypePass5)
			m_priceLbl->setString(myLoc->getLocalForKey(kMyLocalKey_free));
		else
			m_priceLbl->setString(CCString::createWithFormat("%d",m_price)->getCString());
		
		m_priceLbl->setFontSize(13);
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
		m_priceTypeSprite->setAnchorPoint(ccp(0.5,0.5));
		m_priceTypeSprite->setScale(0.9);
		m_priceTypeSprite->setPosition(ccp(m_priceLbl->getPositionX()-m_priceLbl->getContentSize().width/2-20+2,m_priceLbl->getPositionY()));//getContentSize().height/2.f-m_priceTypeSprite->getContentSize().height/2.f-2));
		m_btn->addChild(m_priceTypeSprite,11);
	}
	
	
	float w1 = m_priceTypeSprite->getContentSize().width;
	float w2 = m_priceLbl->getContentSize().width;
	float w3 = getContentSize().width;
	
	float a = (w3-(w1+w2+5))/2.f;
	
	m_priceTypeSprite->setPositionX(a+w1/2.f);
	m_priceLbl->setPositionX(a+w1+w2/2.f);
	
	if(this->m_title==""){
		m_priceLbl->setFontSize(this->m_btnTitle->getFontSize());
		m_priceLbl->setColor(this->m_btnTitle->getColor());
		m_priceLbl->setPositionY(getContentSize().height/2.f);
		m_priceTypeSprite->setPositionY(getContentSize().height/2.f);
	}
}

int CommonButton::getPrice(){
	return m_price;
}

PriceType CommonButton::getPriceType(){
	return m_priceType;
}

void CommonButton::setOpacity(GLubyte opacity){
	if(m_btnBack->getTag()!=119)m_btn->setOpacity(opacity);
	CCSprite* backSprite = (CCSprite*)this->getChildByTag(112);
	if(backSprite)backSprite->setOpacity(opacity);
}

KSLabelTTF* CommonButton::getTitleLabel(){
	return m_btnTitle;
}

KSLabelTTF* CommonButton::getPriceLabel(){
	return m_priceLbl;
}

CCSprite* CommonButton::getPriceSprite(){
	return m_priceTypeSprite;
}
