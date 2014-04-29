#include "StyledLabelTTF.h"
#include "jsoncpp/json.h"
#include "ks19937.h"
using namespace boost;

StyledLabelTTF::StyledLabelTTF() :
m_currentPosition(0.f),
m_oneLineSize(0.f),
m_currentLinePosition(0.f)
{
	
}
StyledLabelTTF::~StyledLabelTTF()
{
}
//void StyledLabelTTF::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool StyledLabelTTF::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool StyledLabelTTF::init()
{
	CCNode::init();
	
	return true;
}

void StyledLabelTTF::updateTexture()
{
	removeAllChildren();
	m_oneLineContainer = CCNode::create();
	addChild(m_oneLineContainer);
	
	Json::Value jsonStyle;
	for(auto iter = m_texts.begin(); iter != m_texts.end(); ++iter)
	{
		StyledText t = *iter;
		
		StyledText st = t;
		
		Json::Reader reader;
		reader.parse(st.m_style, jsonStyle);
		if(jsonStyle.get("linebreak", false).asBool()) // 줄바꿈이거나 마지막 요소이면.
		{
			if(m_currentAlignment == StyledAlignment::kLeftAlignment)
			{
				// default
			}
			else if(m_currentAlignment == StyledAlignment::kCenterAlignment)
			{
				m_oneLineContainer->setPosition(ccp(-m_oneLineSize / 2.f, 0));
			}
			else if(m_currentAlignment == StyledAlignment::kRightAlignment)
			{
				m_oneLineContainer->setPosition(ccp(-m_oneLineSize, 0));
			}
			m_oneLineSize = 0.f;
			m_oneLineContainer = CCNode::create();
			addChild(m_oneLineContainer);
			m_currentLinePosition -= jsonStyle.get("linespacing", 18.f).asFloat();
			m_currentPosition = 0.f;
			
			
			
		}
		else
		{
			CCLabelTTF* ttf = CCLabelTTF::create(st.m_text.c_str(), jsonStyle.get("font", "").asString().c_str(), jsonStyle.get("size", 12.f).asFloat());
			m_oneLineContainer->addChild(ttf);
			ttf->setAnchorPoint(ccp(0.f, 0.5f));
			ttf->setPosition(ccp(m_currentPosition, m_currentLinePosition));
			unsigned long fillColor = jsonStyle.get("fillcolor", 0).asUInt();
			ttf->setFontFillColor(ccc3(getRed(fillColor), getGreen(fillColor), getBlue(fillColor)));
			m_currentPosition += ttf->getContentSize().width;
			m_oneLineSize += ttf->getContentSize().width;
			
			
		}
	}
	
	//	m_oneLineContainer->setPosition(ccp(-m_oneLineSize / 2.f, 0));
	//	m_oneLineSize = 0.f;
	//	m_oneLineContainer = CCNode::create();
	//	addChild(m_oneLineContainer);
	//	m_currentLinePosition -= jsonStyle.get("linespacing", 18.f).asFloat();
	//	m_currentPosition = 0.f;
}
bool StyledLabelTTF::init(const std::vector<StyledText>& texts, StyledAlignment sa)
{
	CCNode::init();
	
	m_currentAlignment = sa;
	m_texts = texts; // 나중에 재정렬을 위해서 가지고 있어야 됨.
	// 한개 만들고, 그 다음에 만드는 좌표...가 필요.
	// 계속 이어붙임.
	updateTexture();
	
	
	
	
	
	//		CCSprite* tt = CCSprite::create();
	//		tt->setAnchorPoint(ccp(0, 0.5f));
	//		tt->setOpacity(100);
	//		tt->setTextureRect(CCRectMake(0,0, ttf->getContentSize().width, ttf->getContentSize().height));
	////		tt->setContentSize(ttf->getContentSize());
	//		tt->setPosition(ccp(m_currentPosition, 30 + ks19937::getIntValue(30, 50)));
	//		tt->setColor(ccc3(255, ks19937::getIntValue(0, 255), 0));
	//		addChild(tt);
	
	
	
	return true;
}
