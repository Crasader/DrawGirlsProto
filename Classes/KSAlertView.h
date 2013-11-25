#pragma once


#include "cocos-ext.h"
#include <string>
#include "CCMenuLambda.h"
#include "KSGeometry.h"
#include "ScrollBar.h"

USING_NS_CC;
USING_NS_CC_EXT;



class KSCoverLayer : public cocos2d::CCLayerColor
{
public:
	static int touchPriority;
	bool animation;


	
	virtual void registerWithTouchDispatcher();
	
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	KSCoverLayer()
	{
		animation = true;
//		touchPriority-=2;
	}
	CREATE_FUNC(KSCoverLayer);
	virtual ~KSCoverLayer() {
//		touchPriority+=2;
	}
	
	bool init();
};


class KSAlertView : public CCNode, public CCScrollViewDelegate, public CCTouchDelegate
{
public:
	std::vector<CCMenuItemLambda*> m_menuItems;
	CCMenuItemLambda* m_closeItem;
	
	
//	virtual void registerWithTouchDispatcher() {
//		cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate
//		(this, INT_MIN, true);
//	}
	virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
	KSAlertView() :
	m_width(350),
	m_height(300),
	m_shown(false),
//	m_existCloseButton(false),
	m_horizonScroll(true),
	m_verticalScroll(true),
	m_centerX(240),
	m_centerY(160),
//	m_buttonFile("ui_common_9_button_brown.png"),
//	m_closeButtonFile("ui_common_close.png"),
//	m_contentBorderFile("popup_back2.png"),
	m_titleStr(""),
	m_hScroll(NULL),
	m_vScroll(NULL),
	m_contentNode(NULL),
	m_closeItem(NULL),
	m_closeOnPress(true),
	m_back9(NULL)
	
	{
		
	}
	
	void scrollViewDidScroll(CCScrollView* view);
	void scrollViewDidZoom(CCScrollView* view);
	static KSAlertView* create();
	bool init();
	void addButton(CCMenuItemLambda* item);
	void setCloseButton(CCMenuItemLambda* item);
	void show();
	
	ScrollBar* m_scrollBar;
	CC_SYNTHESIZE(CCNode*, m_contentNode, ContentNode);
	CC_SYNTHESIZE(int, m_centerX, CenterX);
	CC_SYNTHESIZE(int, m_centerY, CenterY);
	CC_SYNTHESIZE(int, m_width, Width);
	CC_SYNTHESIZE(int, m_height, Height);
	CC_SYNTHESIZE(bool, m_shown, IsShown);
//	CC_SYNTHESIZE(bool, m_existCloseButton, ExistCloseButton);
	CC_SYNTHESIZE(bool, m_horizonScroll, HorizonScroll);
	CC_SYNTHESIZE(bool, m_verticalScroll, VerticalScroll);
//	CC_SYNTHESIZE(std::string, m_buttonFile, ButtonFile);
	CC_SYNTHESIZE(std::string, m_titleStr, TitleStr);
//	CC_SYNTHESIZE(std::string, m_closeButtonFile, CloseButtonFile);
	CC_SYNTHESIZE(CCScale9Sprite*, m_contentBorder, ContentBorder);
	CC_SYNTHESIZE(CCScale9Sprite*, m_hScroll, HScroll);
	CC_SYNTHESIZE(CCScale9Sprite*, m_vScroll, VScroll);
	CC_SYNTHESIZE(CCScale9Sprite*, m_back9, Back9);
	CC_SYNTHESIZE(bool, m_closeOnPress, CloseOnPress);
};

