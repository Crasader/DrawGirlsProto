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
	std::function<void(void)> m_customCloseFunction;
	
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
	m_back9(NULL),
	m_contentBorder(NULL),
	m_containerScrollView(NULL),
	m_borderScale(1.f),
	m_titleHeight(50),
	m_buttonHeight(50),
	m_scrollBar(NULL),
	m_customCloseFunction(nullptr),
	m_titleOffsetX(0),
	m_titleOffsetY(0)
	{
		
	}
	
	void scrollViewDidScroll(CCScrollView* view);
	void scrollViewDidZoom(CCScrollView* view);
	static KSAlertView* create();
	bool init();
	void addButton(CCMenuItemLambda* item); // 버튼을 추가함
	void setCloseButton(CCMenuItemLambda* item); // 우측 상단 닫기 버튼을 설정함
	void show(std::function<void(void)> closeFunc);
	void show(); // 창을 띄움, 창을 띄우기 전에 모든게 세팅이 되어야 함.
	
	CCSize getViewSize()
	{
		int contentMargin = 1<<3;
		
		CCRect contentRect = CCRectMake(contentMargin, m_buttonHeight,
																		m_width-contentMargin*2, m_height - m_titleHeight - m_buttonHeight);
		contentRect = rtSetScale(contentRect, m_borderScale);
		return CCSizeMake(contentRect.size.width, contentRect.size.height);

	}
	ScrollBar* m_scrollBar;
	CC_SYNTHESIZE(CCNode*, m_contentNode, ContentNode); // 내용물
	CC_SYNTHESIZE(int, m_centerX, CenterX); // 팝업이 뜬 위치가 맘에 안들면 조절해야함. 기본값 가운데
	CC_SYNTHESIZE(int, m_centerY, CenterY); // 팝업이 뜬 위치가 맘에 안들면 조절해야함. 기본값 가운데
	CC_SYNTHESIZE(int, m_width, Width);   // 전체 프레임의 가로크기
	CC_SYNTHESIZE(int, m_height, Height); // 전체 프레임의 세로 크기
	CC_SYNTHESIZE(int, m_titleHeight, TitleHeight); // 제목줄의 크기
	CC_SYNTHESIZE(int, m_buttonHeight, ButtonHeight); // 버튼의 크기
	CC_SYNTHESIZE(bool, m_shown, IsShown); // 알 필요 없음

	CC_SYNTHESIZE(bool, m_horizonScroll, HorizonScroll);   // 알 필요없음
	CC_SYNTHESIZE(bool, m_verticalScroll, VerticalScroll); // 알 필요없음
	
	CC_SYNTHESIZE(std::string, m_titleStr, TitleStr); // 타이틀 문자열, 있으면 위에서 여백이 생김

	
	CC_SYNTHESIZE(CCScale9Sprite*, m_contentBorder, ContentBorder); // 내용물 테두리
	CC_SYNTHESIZE(float, m_borderScale, BorderScale); // 전체 프레임의 스케일,
	CC_SYNTHESIZE(CCScale9Sprite*, m_hScroll, HScroll); // 가로 스크롤, 설정안하면 안보임
	CC_SYNTHESIZE(CCScale9Sprite*, m_vScroll, VScroll); // 세로 스크롤, 설정안하면 안보임
	CC_SYNTHESIZE(CCScale9Sprite*, m_back9, Back9);    // 배경이미지
	CC_SYNTHESIZE(bool, m_closeOnPress, CloseOnPress); // 눌렀을 때 닫히게 할거냐, 기본값 true
	CC_SYNTHESIZE(CCScrollView*, m_containerScrollView, ContainerScrollView); //
	CC_SYNTHESIZE(int, m_titleOffsetX, TitleOffsetX); //
	CC_SYNTHESIZE(int, m_titleOffsetY, TitleOffsetY); //
};

