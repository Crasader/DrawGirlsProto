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
	CommonButtonClose
	};

class CommonButton : public CCNode {
	CCControlButton* m_btn;
	std::function<void(void)> m_func;
	CCLabelTTF* m_btnTitle;
	CCScale9Sprite* m_btnBack;
	CommonButtonType m_btnType;
public:
	
	void setFunction(std::function<void(void)> func){
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
		
		this->setAnchorPoint(ccp(0.5f,0.5f));
		m_btnTitle = CCLabelTTF::create(title.c_str(), mySGD->getFont().c_str(), fontSize);
		
		string btnBackImg;
		
		m_btnType = btnType;
		
		if(btnType==CommonButtonYellow) btnBackImg = "commonButtonYellow.png";
		else if(btnType==CommonButtonYellow) btnBackImg = "commonButtonYellow.png";
		else if(btnType==CommonButtonGray) btnBackImg = "commonButtonGray.png";
		else if(btnType==CommonButtonBlue) btnBackImg = "commonButtonBlue.png";
		else if(btnType==CommonButtonGreen) btnBackImg = "commonButtonGreen.png";
		else if(btnType==CommonButtonOrange) btnBackImg = "commonButtonOrange.png";
		else if(btnType==CommonButtonClose) btnBackImg = "commonButtonClose.png";
		
		m_btnBack= CCScale9Sprite::create(btnBackImg.c_str());
		
		
		m_btn = CCControlButton::create(m_btnTitle, m_btnBack);
		this->setButtonInset(btnType);
		
		
		if(size.height>0){
			m_btn->setPreferredSize(size);
		}
		
		m_btn->setAnchorPoint(ccp(0.5f,0.5f));
		m_btn->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::callFunc), CCControlEventTouchUpInside);
		m_btn->setPosition(m_btn->getContentSize().width/2, m_btn->getContentSize().height/2);
		if(touchPriority!=0)m_btn->setTouchPriority(touchPriority);
		addChild(m_btn,1000);
		
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
		
		if(m_func)m_func();
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
	
	
};
#endif /* defined(__DGproto__CommonButton__) */
