#pragma once


#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "KSGeometry.h"
USING_NS_CC;
USING_NS_CC_EXT;


class ScrollBar : public CCLayerColor {
public:
	//자신의 크기를 정하고, 바를 생성
	ScrollBar();
	virtual ~ScrollBar();
	static ScrollBar* createScrollbar(CCScrollView* table, float offset, const std::string& h = "popup_bar_h.png",
																		const std::string& v = "popup_bar_v.png", int touchPrioity = INT_MIN);
	static ScrollBar* createScrollbar(CCScrollView* table, float offset, CCScale9Sprite* h9, CCScale9Sprite* v9, int touchPrioity = INT_MIN)
	{
		ScrollBar* obj = new ScrollBar();
		
    if(obj->init(table, offset, h9, v9, touchPrioity)) {
			obj->autorelease();
			return obj;
    } else {
			delete obj;
			return NULL;
    }
	}
	bool init(CCScrollView* sv, float offset, const std::string& h, const std::string& v, int touchPriority);
	bool init(CCScrollView* sv, float offset, CCScale9Sprite* h9, CCScale9Sprite* v9, int touchPriority);
	//주기적으로 호출 되면서 내용을 갱신할 함수
	void setBarRefresh();
	void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touchPriority, true);
	}
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	CCRect getRectForObject(CCScale9Sprite* ob)
	{
		CCPoint obPosition = ob->getPosition();
		CCSize obContentSize = ob->getContentSize();
		CCPoint obAnchorPoint = ob->getAnchorPoint();
		return rtSetScale( CCRectMake( obPosition.x - obContentSize.width * obAnchorPoint.x,
											obPosition.y - obContentSize.height * obAnchorPoint.y,
											obContentSize.width, obContentSize.height), 1.5f );
	}
protected:
	void setBarRefreshH();
	void setBarRefreshV();
	bool dynamicScrollSize; // 스크롤 크기가 컨텐츠 사이즈에 따라 바뀔지 결정하는 함수.
public:
	bool getDynamicScrollSize(){return dynamicScrollSize;}
	void setDynamicScrollSize(bool b)
	{
		dynamicScrollSize = b;
		setBarRefresh();
	}
	
	CCScale9Sprite* selectedBarV;
	CCScale9Sprite* selectedBarH;
	int touchPriority;
	CC_SYNTHESIZE(float, marginOffset, MarginOffset);
	CC_SYNTHESIZE(CCScale9Sprite*, barV, BarV);
	CC_SYNTHESIZE(CCScale9Sprite*, barH, BarH);
	CC_SYNTHESIZE(CCSprite*, fixedBarV, FixedBarV);
	CC_SYNTHESIZE(CCSprite*, fixedBarH, FixedBarH);
	CC_SYNTHESIZE(CCScrollView*, scrollView, ScrollView);
	CC_SYNTHESIZE(int, scrollBarNarrowSize, ScrollBarNarrowSize);
};

