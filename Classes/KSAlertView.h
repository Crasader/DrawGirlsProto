#pragma once


#include "cocos-ext.h"
#include <string>
#include "CCMenuLambda.h"

USING_NS_CC;
USING_NS_CC_EXT;


class KSAlertView : public CCNode
{
public:
	std::vector<CCMenuItemLambda*> m_menuItems;
	
	KSAlertView() :
	m_width(350),
	m_height(300),
	m_shown(false),
	m_existCloseButton(false),
	m_horizonScroll(true),
	m_verticalScroll(true),
	m_centerX(240),
	m_centerY(160),
	m_backgroundFile("ui_common_9_button_brown.png"),
	m_buttonFile("ui_common_9_button_brown.png")
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
		CCScale9Sprite *btnBg = CCScale9Sprite::create(m_backgroundFile.c_str(),
																									 CCRectMake(0, 0, 38, 38),
																									 CCRectMake(10, 10, 18, 18));
		addChild(btnBg);
		
		CCPoint centerPosition = convertToNodeSpace(ccp(m_centerX, m_centerY));
		float top = centerPosition.y + m_height / 2.f;
		float bottom = centerPosition.y - m_height / 2.f;
		btnBg->setPosition(convertToNodeSpace(centerPosition));
		
		
		btnBg->setContentSize(CCSizeMake(m_width, m_height));
		
		CCMenuLambda* _menu = CCMenuLambda::create();
		btnBg->addChild(_menu);
		_menu->setPosition(btnBg->convertToNodeSpace(ccp(240, bottom)));
//		_menu->setPosition();
		
		
//		_menu->alignItemsHorizontallyWithPadding(10.f);
		
		for(auto it : m_menuItems)
		{
			_menu->addChild(it);
		}
		_menu->alignItemsHorizontally();
		
		
	}
	
	
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
};

