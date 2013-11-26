//
//  KSAlertView.cpp
//  DGproto
//
//  Created by ksoo k on 2013. 11. 13..
//
//

#include "KSAlertView.h"


int KSCoverLayer::touchPriority = INT_MIN + 1000;



void KSCoverLayer::registerWithTouchDispatcher()
{
	cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate
		(this, INT_MIN + 1, true);
}

bool KSCoverLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLog("kscoverlayer swallow");
	return true; // swallow;
}

bool KSCoverLayer::init()
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

bool KSAlertView::ccTouchBegan( CCTouch *touch, CCEvent *event )
{
	return false;
}

void KSAlertView::scrollViewDidScroll( CCScrollView* view )
{
	if(m_scrollBar != NULL) m_scrollBar->setBarRefresh();
}

void KSAlertView::scrollViewDidZoom( CCScrollView* view )
{

}

KSAlertView* KSAlertView::create()
{
	KSAlertView* newO = new KSAlertView();
	newO->init();

	newO->autorelease();

	return newO;
}

bool KSAlertView::init()
{
	CCNode::init();
	return true;
}

void KSAlertView::addButton( CCMenuItemLambda* item )
{
	m_menuItems.push_back(item);
}

void KSAlertView::setCloseButton(CCMenuItemLambda* item)
{
	m_closeItem = item;
}

void KSAlertView::show()
{
	KSCoverLayer* cover = KSCoverLayer::create();
	addChild(cover, INT_MAX);

	if(!m_back9)
	{
		m_back9 = CCScale9Sprite::create("popup_back1.png",
																									 CCRectMake(0, 0, 150, 150),
																									 CCRectMake(10, 10, 150 - 10*2, 150 - 10*2));
		cover->addChild(m_back9);
	}
	else
	{
		cover->addChild(m_back9);
	}
	CCScale9Sprite *btnBg = m_back9;

	CCPoint centerPosition = convertToNodeSpace(ccp(m_centerX, m_centerY));
	float top = m_centerY + m_height / 2.f;
	float bottom = m_centerY - m_height / 2.f;
	float titleHeight = m_titleHeight;
	float buttonHeight = m_buttonHeight;
	int contentMargin = 1<<3;
	float contentBorderMargin = contentMargin >> 1;
	btnBg->setPosition(ccp(m_centerX, m_centerY));
	btnBg->setContentSize(CCSizeMake(m_width, m_height));

	CCMenuLambda* _menu = CCMenuLambda::create();
	_menu->setTouchPriority(INT_MIN);
	btnBg->addChild(_menu);
	_menu->setPosition(btnBg->convertToNodeSpace(ccp(m_centerX, bottom + buttonHeight / 2.f)));
	//		_menu->setPosition();
	
	
	
	//		_menu->alignItemsHorizontallyWithPadding(10.f);

	for(auto it : m_menuItems)
	{
		_menu->addChild(it);
		if(m_closeOnPress)
		{
			it->m_afterSelector = [=](CCObject*)
			{
				removeFromParent();
			};
		}
		
	}
	// 아래쪽 버튼만 넣고 정렬 때림.
	_menu->alignItemsHorizontally();
	
	CCMenuLambda* _closeMenu = CCMenuLambda::create();
	_closeMenu->setTouchPriority(INT_MIN);
	_closeMenu->setPosition(ccp(0, 0));
	btnBg->addChild(_closeMenu);
	
	// 닫기 버튼은 정렬 안함
	if(m_closeItem)
	{
		_closeMenu->addChild(m_closeItem);
		m_closeItem->setPosition(btnBg->convertToNodeSpace
														 (ccp(m_centerX + m_width / 2.f - m_closeItem->getContentSize().width / 2.f,
																	m_centerY + m_height / 2.f - m_closeItem->getContentSize().height / 2.f)));
		if(m_closeOnPress)
		{
			m_closeItem->m_afterSelector = [=](CCObject*)
			{
				removeFromParent();
			};
		}
	}
	if(m_titleStr != "")
	{
		CCLabelTTF* t = CCLabelTTF::create(m_titleStr.c_str(), "", 14.f);
		t->setAnchorPoint(ccp(0.5, 1.0));
		t->setPosition(btnBg->convertToNodeSpace(ccp(240, top)));
		btnBg->addChild(t);
	}

	CCAssert(m_contentNode != NULL, "not null");
//	m_contentNode = CCLayerGradient::create(ccc4(255, 0, 0, 255), ccc4(0,255,255,255));
//	m_contentNode->setContentSize(CCSizeMake(400, 500));

	CCScrollView* sv = CCScrollView::create();

	CCRect contentRect = CCRectMake(contentMargin, buttonHeight,
																	m_width-contentMargin*2, m_height - titleHeight - buttonHeight);
	contentRect = rtSetScale(contentRect, m_borderScale);
	sv->setViewSize(CCSizeMake(contentRect.size.width, contentRect.size.height));
	sv->setPosition(ccp(contentRect.origin.x, contentRect.origin.y));
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
	
	CCRect transformedRect = rtSetScale(contentRect,
		(contentRect.size.height + 8)/ (contentRect.size.height),
		ccp(0.5f, 0.5f));
	if(m_contentBorder == 0)
	{
		m_contentBorder = CCScale9Sprite::create("popup_back2.png", CCRectMake(0, 0, 150, 150),
																						 CCRectMake(5, 5, 150 - 5*2, 150 - 5*2));
	}
	else
	{
		
	}
	CCScale9Sprite *contentBorder = m_contentBorder;
	contentBorder->setContentSize(ccp(transformedRect.size.width, transformedRect.size.height));
	btnBg->addChild(contentBorder, 1);
	contentBorder->setAnchorPoint(ccp(0, 0));
	contentBorder->setPosition(ccp(transformedRect.origin.x, transformedRect.origin.y));
}
