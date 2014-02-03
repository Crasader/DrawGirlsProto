//
//  KHAlertView.cpp
//  DGproto
//
//  Created by ksoo k on 2013. 11. 13..
//
//

#include "KHAlertView.h"



bool KHAlertView::ccTouchBegan( CCTouch *touch, CCEvent *event )
{
	return false;
}

void KHAlertView::scrollViewDidScroll( CCScrollView* view )
{
	if(m_scrollBar != NULL) m_scrollBar->setBarRefresh();
}

void KHAlertView::scrollViewDidZoom( CCScrollView* view )
{

}

KHAlertView* KHAlertView::create()
{
	KHAlertView* newO = new KHAlertView();
	newO->init();

	newO->autorelease();

	return newO;
}

bool KHAlertView::init()
{
	CCNode::init();
	return true;
}

void KHAlertView::addButton(CommonButton* item, std::function<void(CCObject*)> actionOnPress )
{
	m_menuItems.push_back(item);
	auto aop = actionOnPress;
	auto totalAction = [=](CCObject* obj){
		aop(obj);
		if(this->m_closeOnPress) {
			this->removeFromParent();
		}
	};
	item->setFunction(totalAction);
}

void KHAlertView::setCloseButton(CCMenuItemLambda* item)
{
	m_closeItem = item;
}

void KHAlertView::show(std::function<void(void)> closeFunc)
{
	m_customCloseFunction = closeFunc;
	show();
}
void KHAlertView::show()
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

	float top = m_centerY + m_height / 2.f;
	float bottom = m_centerY - m_height / 2.f;
	float titleHeight = m_titleHeight;
	float buttonHeight = m_buttonHeight;
	int contentMargin = 1<<3;
	float contentBorderMargin = contentMargin >> 1;
	btnBg->setPosition(ccp(m_centerX, m_centerY));
	btnBg->setContentSize(CCSizeMake(m_width, m_height));
	
	KSNode* _menu = new KSNode();
	_menu->init();
	_menu->autorelease();
	//CCMenuLambda* _menu = CCMenuLambda::create();
	//_menu->setTouchPriority(INT_MIN);
	btnBg->addChild(_menu);
	_menu->setPosition(ccp(m_width / 2, buttonHeight / 2.f + m_buttonOffsetY));

	for(auto it : m_menuItems)
	{
		_menu->addChild(it);
//		if(m_closeOnPress)
//		{
//			it->m_afterSelector = [=](CCObject*)
//			{
//				removeFromParent();
//			};
//		}
		
	}
	// 아래쪽 버튼만 넣고 정렬 때림.
	_menu->alignItemsHorizontallyWithPadding(m_buttonPadding);
	
	CCMenuLambda* _closeMenu = CCMenuLambda::create();
	_closeMenu->setTouchPriority(INT_MIN);
	_closeMenu->setPosition(ccp(0, 0));
	btnBg->addChild(_closeMenu);
	
	// 닫기 버튼은 정렬 안함
	if(m_closeItem)
	{
		_closeMenu->addChild(m_closeItem);
		m_closeItem->setPosition(ccp(m_width - m_closeItem->getContentSize().width / 2.f, m_height - m_closeItem->getContentSize().height / 2.f));
//		m_closeItem->setPosition(btnBg->convertToNodeSpace
//														 (ccp(m_centerX + m_width / 2.f - m_closeItem->getContentSize().width / 2.f,
//																	m_centerY + m_height / 2.f - m_closeItem->getContentSize().height / 2.f)));
		if(m_closeOnPress)
		{
			m_closeItem->m_afterSelector = [=](CCObject*)
			{
				removeFromParent();
			};
		}
	}
	if(m_titleFileName != "")
	{
		CCSprite* titleSprite = CCSprite::create(m_titleFileName.c_str());
		titleSprite->setAnchorPoint(ccp(0.5, 0.5));
		titleSprite->setPosition(ccp(m_width / 2.f, m_height - 20));
		btnBg->addChild(titleSprite);
		//CCLabelTTF* t = CCLabelTTF::create(m_titleFileName.c_str(), "", 14.f);
		//t->setAnchorPoint(ccp(0.5, 1.0));
		//t->setPosition(ccp(m_width / 2, m_height));
		//btnBg->addChild(t);
	}

//	CCAssert(m_contentNode != NULL, "not null");
//	m_contentNode = CCLayerGradient::create(ccc4(255, 0, 0, 255), ccc4(0,255,255,255));
//	m_contentNode->setContentSize(CCSizeMake(400, 500));

	CCScrollView* sv = CCScrollView::create();
	m_containerScrollView = sv;
	CCRect contentRect = CCRectMake(contentMargin, buttonHeight,
																	m_width-contentMargin*2, m_height - titleHeight - buttonHeight);
	contentRect = rtSetScale(contentRect, m_borderScale);
	sv->setViewSize(CCSizeMake(contentRect.size.width, contentRect.size.height));
	sv->setPosition(ccp(contentRect.origin.x, contentRect.origin.y));
	sv->setDirection(CCScrollViewDirection::kCCScrollViewDirectionBoth);

	if(m_contentNode)
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
		(contentRect.size.height)/ (contentRect.size.height),
		ccp(0.5f, 0.5f));
//	if(m_contentBorder == 0)
//	{
//		m_contentBorder = CCScale9Sprite::create("popup_back2.png", CCRectMake(0, 0, 150, 150),
//																						 CCRectMake(5, 5, 150 - 5*2, 150 - 5*2));
//	}
//	else
//	{
//		
//	}
	if(m_contentBorder)
	{
		CCScale9Sprite *contentBorder = m_contentBorder;
		contentBorder->setContentSize(ccp(transformedRect.size.width, transformedRect.size.height));
		btnBg->addChild(contentBorder, 1);
		contentBorder->setAnchorPoint(ccp(0, 0));
		contentBorder->setPosition(ccp(transformedRect.origin.x, transformedRect.origin.y));
	}
}
