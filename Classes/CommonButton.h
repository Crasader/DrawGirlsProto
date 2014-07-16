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
	CommonButtonFree,
	CommonButtonAchievement,
	CommonButtonReward,
	CommonButtonIng,
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

class KSLabelTTF;
class CommonButton : public CCNode {
	CCControlButton* m_btn;
	std::function<void(CCObject*)> m_func;
	KSLabelTTF* m_btnTitle;
	CCScale9Sprite* m_btnBack;
	CommonButtonType m_btnType;
	int m_price;
	PriceType m_priceType;
	KSLabelTTF* m_priceLbl;
	float m_fontSize;
	string m_title;
	ccColor3B m_titleColorNomal;
	ccColor3B m_titleColorDisable;
public:
	CCSprite* m_priceTypeSprite;
	void setFunction(std::function<void(CCObject*)> func){
		m_func=func;
	}
	
	static CommonButton* create(string title, float fontSize, CCSize size,CommonButtonType btnType, int touchPriority){
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
	
	static CommonButton* create(string title, float fontSize, CCSize size, CCScale9Sprite* button_back, int touchPriority){
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
	
	
	static CommonButton* create(CCSprite* backSprite, int touchPriority){
		CommonButton* btn = new CommonButton();
		if(btn->init(backSprite,touchPriority)==false){
			return NULL;
		}
		btn->autorelease();
		return btn;
	}
	
	bool init(CCSprite* backSprite, int touchPriority);

	bool init(string title, float fontSize, CCSize size,CommonButtonType btnType, int touchPriority);
	
	bool init(string title, float fontSize, CCSize size, CCScale9Sprite* button_back, int touchPriority);
	static CCScale9Sprite* getBackgroundByType(CommonButtonType btnType);
	
	void setButtonInset(CommonButtonType type);
	void callFunc(CCObject* obj, CCControlEvent event);
	
	void setTouchPriority(int touchPriority);
	void setTitle(string title);
	void setTitleSize(float size);
	void setSize(CCSize size);
	void setEnabled(bool isEnabled);
	bool isEnabled();
	void setSelected(bool isSelected);
	
	bool isSelected();
	void setTitleForState(cocos2d::CCString *title, CCControlState state);
	void setBackgroundSpriteForState(cocos2d::extension::CCScale9Sprite *sprite, CCControlState state);
	
	void setBackgroundTypeForDisabled(CommonButtonType type);
	void setBackgroundTypeForState(CommonButtonType type,CCControlState state);
	void setTitleColor(ccColor3B color);
	
	KSLabelTTF* getTitleLabel();
	KSLabelTTF* getPriceLabel();
	CCSprite* getPriceSprite();
	
	void setTitleColorForDisable(ccColor3B color);
	
	void setPrice(PriceType priceType, int price);
	
	int getPrice();
	
	PriceType getPriceType();
	
	void setOpacity(GLubyte opacity);
	
};
#endif /* defined(__DGproto__CommonButton__) */
