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


	
	virtual void registerWithTouchDispatcher()
	{
		cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate
		(this, INT_MIN + 1, true);
	}
	
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		CCLog("kscoverlayer swallow");
		return true; // swallow;
	}
	KSCoverLayer()
	{
		animation = true;
//		touchPriority-=2;
	}
	CREATE_FUNC(KSCoverLayer);
	virtual ~KSCoverLayer() {
//		touchPriority+=2;
	}
	
	bool init()
	{
		if ( !this->CCLayerColor::initWithColor(cocos2d::ccc4(0, 0, 0, 0)))
			return false;
		
		CCSprite* b = CCSprite::create();
		b->setTextureRect(CCRectMake(0, 0, 600, 320));
		b->setColor(ccc3(0, 0, 0));
		//
		if(animation)
		{
			b->setOpacity(0);
			
			b->runAction(CCFadeTo::create(0.5f, 150));
		}
		else
		{
			b->setOpacity(150);
		}
//		b->setOpacity(0);
		//	b->setContentSize(CCSizeMake(600, 320));
		b->setPosition(ccp(240, 160));
		b->setAnchorPoint(ccp(0.5f, 0.5f));
		
		addChild(b,0);
		//	this->ignoreAnchorPointForPosition(true);
		setTouchEnabled(true);
		
		return true;
	}
};


class KSAlertView : public CCNode, public CCScrollViewDelegate, public CCTouchDelegate
{
public:
	std::vector<CCMenuItemLambda*> m_menuItems;
	
	
	
//	virtual void registerWithTouchDispatcher() {
//		cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate
//		(this, INT_MIN, true);
//	}
	virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event) {
		return false;
	}
	KSAlertView() :
	m_width(350),
	m_height(300),
	m_shown(false),
	m_existCloseButton(false),
	m_horizonScroll(true),
	m_verticalScroll(true),
	m_centerX(240),
	m_centerY(160),
	m_backgroundFile("popup_back1.png"),
	m_buttonFile("ui_common_9_button_brown.png"),
	m_closeButtonFile("ui_common_close.png"),
	m_contentBorderFile("popup_back2.png"),
	m_titleStr("? ? ? ? !! ! ! "),
	m_hScroll(NULL),
	m_vScroll(NULL),
	m_contentNode(NULL)
	
	{
		
	}
	
	void scrollViewDidScroll(CCScrollView* view)
	{
		if(m_scrollBar != NULL) m_scrollBar->setBarRefresh();
	}
	void scrollViewDidZoom(CCScrollView* view)
	{
		
	}
	static KSAlertView* create()
	{
		KSAlertView* newO = new KSAlertView();
		newO->init();
		
		newO->autorelease();
		
		return newO;
	}
	bool init()
	{
		CCNode::init();
		return true;
	}
	void addButton(CCMenuItemLambda* item)
	{
		m_menuItems.push_back(item);
	}
	
	void show()
	{
		
		KSCoverLayer* cover = KSCoverLayer::create();
		addChild(cover, INT_MAX);
		
		
		CCScale9Sprite *btnBg = CCScale9Sprite::create(m_backgroundFile.c_str(),
																									 CCRectMake(0, 0, 150, 150),
																									 CCRectMake(10, 10, 150 - 10*2, 150 - 10*2));
		cover->addChild(btnBg);
		
		CCPoint centerPosition = convertToNodeSpace(ccp(m_centerX, m_centerY));
		float top = centerPosition.y + m_height / 2.f;
		float bottom = centerPosition.y - m_height / 2.f;
		float titleHeight = 50;
		float buttonHeight = 50;
		int contentMargin = 1<<3;
		float contentBorderMargin = contentMargin >> 1;
		btnBg->setPosition(convertToNodeSpace(centerPosition));
		btnBg->setContentSize(CCSizeMake(m_width, m_height));
		
		CCMenuLambda* _menu = CCMenuLambda::create();
		_menu->setTouchPriority(INT_MIN);
		btnBg->addChild(_menu);
		_menu->setPosition(btnBg->convertToNodeSpace(ccp(240, bottom + buttonHeight / 2.f)));
//		_menu->setPosition();
		
		
//		_menu->alignItemsHorizontallyWithPadding(10.f);
		
		for(auto it : m_menuItems)
		{
			_menu->addChild(it);
			it->m_afterSelector = [=](CCObject*)
			{
				removeFromParent();
			};
		}
		_menu->alignItemsHorizontally();
		
		
		
		if(m_titleStr != "")
		{
			CCLabelTTF* t = CCLabelTTF::create(m_titleStr.c_str(), "", 14.f);
			t->setAnchorPoint(ccp(0.5, 1.0));
			t->setPosition(btnBg->convertToNodeSpace(ccp(240, top)));
			btnBg->addChild(t);
		}
		
		if(m_contentNode)
		{
			
		}
		m_contentNode = CCLayerGradient::create(ccc4(255, 0, 0, 255), ccc4(0,255,255,255));
		m_contentNode->setContentSize(CCSizeMake(400, 500));
		
		CCScrollView* sv = CCScrollView::create();
		
		sv->setViewSize(CCSizeMake(m_width-contentMargin*2, m_height - titleHeight - buttonHeight));
		sv->setPosition(ccp(contentMargin, buttonHeight));
		sv->setDirection(CCScrollViewDirection::kCCScrollViewDirectionBoth);
		
		sv->setContainer(m_contentNode);

		sv->setContentOffset(ccp(0, sv->minContainerOffset().y));
		sv->setDelegate(this);
		sv->setTouchPriority(INT_MIN);
		
		if(m_hScroll && m_vScroll)
		{
			sv->setDirection(kCCScrollViewDirectionBoth);
			m_scrollBar = ScrollBar::createScrollbar(sv, 0, m_hScroll, m_vScroll);
 			if(m_hScroll->getCapInsets().equals(CCRectZero) == false &&
				 m_vScroll->getCapInsets().equals(CCRectZero) == false)
				m_scrollBar->setDynamicScrollSize(true);
			else
				m_scrollBar->setDynamicScrollSize(false);
		}
		if(m_hScroll && !m_vScroll)
		{
			sv->setDirection(kCCScrollViewDirectionHorizontal);
			m_scrollBar = ScrollBar::createScrollbar(sv, 0, m_hScroll, 0);
			if(m_hScroll->getCapInsets().equals(CCRectZero) == false)
				m_scrollBar->setDynamicScrollSize(true);
			else
				m_scrollBar->setDynamicScrollSize(false);
		}
		if(!m_hScroll && m_vScroll)
		{
			sv->setDirection(kCCScrollViewDirectionVertical);
			m_scrollBar = ScrollBar::createScrollbar(sv, 0, 0, m_vScroll);
			if(m_vScroll->getCapInsets().equals(CCRectZero) == false)
				m_scrollBar->setDynamicScrollSize(true);
			else
				m_scrollBar->setDynamicScrollSize(false);
		}
		
		
		btnBg->addChild(sv, 2);
		CCRect contentRect = CCRectMake(contentMargin, buttonHeight,
																		m_width-contentMargin*2, m_height - titleHeight - buttonHeight);
		CCRect transformedRect = rtSetScale(contentRect,
																									 (contentRect.size.height + 8)/ (contentRect.size.height),
																				ccp(0.5f, 0.5f));
		CCScale9Sprite *contentBorder = CCScale9Sprite::create(m_contentBorderFile.c_str(),
																									 CCRectMake(0, 0, 150, 150),
																									 CCRectMake(5, 5, 150 - 5*2, 150 - 5*2));
		contentBorder->setContentSize(ccp(transformedRect.size.width, transformedRect.size.height));
		btnBg->addChild(contentBorder, 1);
		contentBorder->setAnchorPoint(ccp(0, 0));
		contentBorder->setPosition(ccp(transformedRect.origin.x, transformedRect.origin.y));
	}
	
	ScrollBar* m_scrollBar;
	CC_SYNTHESIZE(CCNode*, m_contentNode, ContentNode);
	CC_SYNTHESIZE(int, m_centerX, CenterX);
	CC_SYNTHESIZE(int, m_centerY, CenterY);
	CC_SYNTHESIZE(int, m_width, Width);
	CC_SYNTHESIZE(int, m_height, Height);
	CC_SYNTHESIZE(bool, m_shown, IsShown);
	CC_SYNTHESIZE(bool, m_existCloseButton, ExistCloseButton);
	CC_SYNTHESIZE(bool, m_horizonScroll, HorizonScroll);
	CC_SYNTHESIZE(bool, m_verticalScroll, VerticalScroll);
	CC_SYNTHESIZE(std::string, m_backgroundFile, BackgroundFile);
	CC_SYNTHESIZE(std::string, m_buttonFile, ButtonFile);
	CC_SYNTHESIZE(std::string, m_titleStr, TitleStr);
	CC_SYNTHESIZE(std::string, m_closeButtonFile, CloseButtonFile);
	CC_SYNTHESIZE(std::string, m_contentBorderFile, ContentBorderFile);
	CC_SYNTHESIZE(CCScale9Sprite*, m_hScroll, HScroll);
	CC_SYNTHESIZE(CCScale9Sprite*, m_vScroll, VScroll);
};

