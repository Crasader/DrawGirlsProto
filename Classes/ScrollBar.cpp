//
//  Scrollbar.cpp
//  ExTableView
//
//  Created by 이정우 on 13. 6. 19..
//
//

#include "ScrollBar.h"

#define THIS ScrollBar
#define barFixSize 10.0f
#define barMinSize 20.0f

THIS::THIS () {
	
	dynamicScrollSize = false;
	barV = barH = NULL;
	fixedBarH = fixedBarV = NULL;
	scrollBarNarrowSize = 30;
	selectedBarV = selectedBarH = nullptr;
}

THIS::~THIS() {
	//    if (bar) {
	//        bar = NULL;
	//    }
}
bool THIS::init(CCScrollView* sv, float offset, const std::string& h, const std::string& v, int _touchPriority)
{
	dynamicScrollSize = false;
	touchPriority = _touchPriority;
	scrollView = sv;
	CCLayerColor::initWithColor(ccc4(0, 255, 0, 0));
	marginOffset = offset;
	CCScrollViewDirection type = sv->getDirection();
	setTouchEnabled(true);
	switch(type)
	{
		case kCCScrollViewDirectionHorizontal:
			
			
			fixedBarH = CCSprite::create(h.c_str());
			fixedBarH->setAnchorPoint(ccp(0.5, 0.f));
			fixedBarH->setPosition(ccp(0, offset));
			addChild(fixedBarH);
			
			sv->cocos2d::CCNode::addChild(this, 0, -1);
			setBarRefresh();
			break;
		case kCCScrollViewDirectionVertical:
			
			
			fixedBarV = CCSprite::create(v.c_str());
			fixedBarV->setAnchorPoint(ccp(1.0, 0.5f));
			fixedBarV->setPosition(ccp(offset, 0));
			addChild(fixedBarV);
			
			sv->cocos2d::CCNode::addChild(this, 0, -1);
			setBarRefresh();
			break;
		case kCCScrollViewDirectionBoth:
			
			
			
			fixedBarH = CCSprite::create(h.c_str());
			fixedBarH->setAnchorPoint(ccp(0.5, 0.f));
			fixedBarH->setPosition(ccp(0, offset));
			addChild(fixedBarH);
			setBarRefreshH();
			
			
			
			fixedBarV = CCSprite::create(v.c_str());
			fixedBarV->setAnchorPoint(ccp(1.0, 0.5f));
			fixedBarV->setPosition(ccp(offset, 0));
			addChild(fixedBarV);

			setBarRefreshV();
			
			
			sv->cocos2d::CCNode::addChild(this, 0, -1);
			break;
	}
	return true;
}


bool THIS::init(CCScrollView* sv, float offset, CCScale9Sprite* h9, CCScale9Sprite* v9, int _touchPriority)
{
	dynamicScrollSize = true;
	touchPriority = _touchPriority;
	scrollView = sv;
	CCLayerColor::initWithColor(ccc4(0, 255, 0, 0));
	marginOffset = offset;
	setTouchEnabled(true);
//
	if(h9 && v9)
	{
		sv->setDirection(kCCScrollViewDirectionBoth);
	}
	if(!h9 && v9)
	{
		sv->setDirection(kCCScrollViewDirectionVertical);
	}
	if(h9 && !v9)
	{
		sv->setDirection(kCCScrollViewDirectionHorizontal);
	}
	if(!h9 && !v9)
	{
		sv->setDirection(kCCScrollViewDirectionNone);
	}
	CCScrollViewDirection type = sv->getDirection();
	switch(type)
	{
		case kCCScrollViewDirectionHorizontal:
			barH = h9;
			barH->setAnchorPoint(ccp(0.5, 0.f));
			barH->setPosition(ccp(0, offset));
			addChild(barH);
				
			sv->cocos2d::CCNode::addChild(this, 0, -1);
			setBarRefresh();
			break;
		case kCCScrollViewDirectionVertical:
			barV = v9;
			barV->setAnchorPoint(ccp(1.0, 0.5f));
			barV->setPosition(ccp(offset, 0));
			addChild(barV);
			
		
			sv->cocos2d::CCNode::addChild(this, 0, -1);
			setBarRefresh();
			break;
		case kCCScrollViewDirectionBoth:
			barH = h9;
			barH->setAnchorPoint(ccp(0.5, 0.f));
			barH->setPosition(ccp(0, offset));
			addChild(barH);
			
			
			
			barV = v9;
			barV->setAnchorPoint(ccp(1.0, 0.5f));
			barV->setPosition(ccp(offset, 0));
			addChild(barV);
			
			sv->cocos2d::CCNode::addChild(this, 0, -1);
			setBarRefresh();
			break;
		case kCCScrollViewDirectionNone:
			break;
			
	}

	
	return true;
}
THIS* THIS::createScrollbar(CCScrollView* sv, float offset, const std::string& h,
						   const std::string& v, int _touchPriority)
{
    THIS* obj = new THIS();
    if(obj->init(sv, offset, h, v, _touchPriority)) {
        obj->autorelease();
        return obj;
    } else {
        delete obj;
        return NULL;
    }
}

void THIS::setBarRefreshH()
{
	CCScrollView* sv = scrollView;
	CCSize viewSize = sv->getViewSize();
//	int scrollBarNarrowBorder = 10;
//	viewSize.width -= scrollBarNarrowSize;
	float total = sv->getContainer()->getContentSize().width - viewSize.width;
	float scrollBarSize = viewSize.width * viewSize.width / total;
	
	if(dynamicScrollSize)
	{
		barH->setContentSize(CCSizeMake(scrollBarSize, barH->getContentSize().height));
	}
	else
	{
		barH->setContentSize(barH->getOriginalSize());
	}
	CCSize barContentSize = barH->getContentSize();//, at);
	
	float offset = -sv->getContentOffset().x;
	float offsetY = -sv->getContentOffset().y;
	float percent = offset / total;
	percent = MIN(1, percent);
	percent = MAX(0, percent);
	percent = percent * (viewSize.width - barContentSize.width) / viewSize.width;
	float position = (percent * viewSize.width) + barContentSize.width / 2.f;
	if(barH)
	{
		barH->setPosition(ccp(offset + position, marginOffset));
	}
}
void THIS::setBarRefreshV()
{
	CCScrollView* sv = scrollView;
	CCSize viewSize = sv->getViewSize();
//	viewSize.height -= scrollBarNarrowSize;
	float total = sv->getContainer()->getContentSize().height - viewSize.height;
	float scrollBarSize = viewSize.height * viewSize.height / total;
	if(dynamicScrollSize)
	{
		barV->setContentSize(CCSizeMake(barV->getContentSize().width, scrollBarSize));
	}
	else
	{
		barV->setContentSize(barV->getOriginalSize());
	}
	CCSize barContentSize = barV->getContentSize();//, at);
	
	float offset = -sv->getContentOffset().y;
	float offsetX = -sv->getContentOffset().x;
	float percent = offset / total;
	percent = MIN(1, percent);
	percent = MAX(0, percent);
	percent = percent * (viewSize.height - barContentSize.height) / viewSize.height;
	float position = ((percent * viewSize.height) + barContentSize.height / 2.f);
	if(barV)
	{
		barV->setPosition(ccp(marginOffset + offsetX + viewSize.width, position));
		CCLOG("ASdasdasD %f", position);
	}
	
}
//주기적으로 호출 되면서 내용을 갱신할 함수 -> scrollViewDidScroll에서 호출됨.
void THIS::setBarRefresh() {
	CCScrollView* sv = scrollView;
	if(sv && sv->getDirection() == kCCScrollViewDirectionHorizontal)
	{
		setBarRefreshH();
	}
	else if(sv && sv->getDirection() == kCCScrollViewDirectionVertical)
	{
		setBarRefreshV();
	}
	else if(sv && sv->getDirection() == kCCScrollViewDirectionBoth)
	{
		setBarRefreshV();
		setBarRefreshH();
	}
	//	position = MIN(0.f, position);
	
	
	//	bar->setPosition(ccp(offset, 0));
	//	CCLOG("%f / %f .. %f", offset, total, bar->getPosition().x);
	
	
}


bool ScrollBar::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	// 처음 누른 놈이 선택이 되었다면 움직일 수 있는 상황으로 인식.
	
	CCPoint touchLocation = pTouch->getLocation();
	if(barV)
	{
		CCPoint local = barV->convertToNodeSpace(touchLocation);
		CCRect r = getRectForObject(barV);
		r.origin = CCPointZero;
		
		if (r.containsPoint(local))
		{
			selectedBarV = barV;
			return true;
		}
		else
		{
			return false;
		}

	}
	
	if(barH)
	{
		CCPoint localH = barH->convertToNodeSpace(touchLocation);
		
		CCRect rH = getRectForObject(barH);
		rH.origin = CCPointZero;
		if(rH.containsPoint(localH))
		{
			selectedBarH = barH;
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
	
}

void ScrollBar::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCPoint touchLocation = pTouch->getLocation();
//	CCLOG("zzz %f", touchLocation.y);
	if(selectedBarV)
	{
		CCPoint local = convertToNodeSpace(touchLocation);
		CCSize barContentSize = barV->getContentSize();//, at);
		CCSize viewSize = scrollView->getViewSize();
				
		float _100Position = ((((viewSize.height - barContentSize.height) / viewSize.height) * viewSize.height) + barContentSize.height / 2.f);
		float _0Position = barContentSize.height / 2.f;
		
		float scrollBarPercent = (local.y - _0Position) / (_100Position - _0Position);
		scrollBarPercent *= -1.f;
		float clamp = (scrollView->maxContainerOffset().y - scrollView->minContainerOffset().y) * scrollBarPercent;
		clamp = clampf(clamp, scrollView->minContainerOffset().y, scrollView->maxContainerOffset().y);
		scrollView->setContentOffset(ccp(scrollView->getContentOffset().x, clamp));
	}
	
	if(selectedBarH)
	{
		CCPoint local = convertToNodeSpace(touchLocation);
		CCSize barContentSize = barV->getContentSize();//, at);
		CCSize viewSize = scrollView->getViewSize();
		
		float _100Position = ((((viewSize.width - barContentSize.width) / viewSize.width) * viewSize.width) + barContentSize.width / 2.f);
		float _0Position = barContentSize.width / 2.f;
		
		float scrollBarPercent = (local.x - _0Position) / (_100Position - _0Position);
		scrollBarPercent *= -1.f;
		float clamp = (scrollView->maxContainerOffset().x - scrollView->minContainerOffset().x) * scrollBarPercent;
		clamp = clampf(clamp, scrollView->minContainerOffset().x, scrollView->maxContainerOffset().x);
		scrollView->setContentOffset(ccp(clamp, scrollView->getContentOffset().y));
	}
}


void ScrollBar::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	selectedBarH = selectedBarV = nullptr;
}
