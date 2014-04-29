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
			ttf->setTag(jsonStyle.get("tag", 0).asInt());
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




StyledLabelTTF* StyledLabelTTF::create(const char* text,const char* font ,StyledAlignment sa){
	
	//<|rgb|size|newline|tag>   ex)   <|900|15|15|>안녕하세요   ->빨간색 15 사이즈 텍스트
	std::string str = text;
	bool isSetMode=false;
	std::string content="";
	std::string option="";
	Json::Value sData;
	int labelNo=0;
	int optionNo=0;
	for (int i=0; i<str.length(); i++) {
		if((str[i]=='<' && str[i+1]=='|') || i==str.length()-1){
			if(i==str.length()-1)content+=str[i];
			sData[labelNo]["content"]=content;
			
			if(i!=0)labelNo++;
			
			isSetMode=true;
			content="";
			optionNo=0;
			i++;
			
			continue;
		}
		
		
		
		if(isSetMode){
			if(str[i]=='|'){
				//모은옵션스트링저장하기
				if(optionNo==0)sData[labelNo]["option"]["color"]=std::atoi(option.c_str());
				else if(optionNo==1)sData[labelNo]["option"]["size"]=std::atoi(option.c_str());
				else if(optionNo==2)sData[labelNo]["option"]["newline"]=std::atoi(option.c_str());
				else if(optionNo==3)sData[labelNo]["option"]["tag"]=std::atoi(option.c_str());
				
				option="";
				optionNo++;
				
				
				if(str[i]=='|' && str[i+1]=='>'){
					i++;
					isSetMode=false;
					continue;
				}
				continue;
			}
			
			if(str[i]==' ')continue;
			option+=str[i];
			
		}else{
			if(str[i]=='\n')continue;
			
			content+=str[i];
			
		}
	}
	
	std::vector<StyledText> texts;
	for(int k = 0;k<sData.size();k++){
		Json::Value p;
		int rgb = sData[k]["option"].get("color", 000).asInt();
		p["fillcolor"]=StyledLabelTTF::makeRGB((rgb/100)/9.f*255, (rgb/10%10)/9.f*255, (rgb%10)/9.f*255);
		p["font"]=font;
		p["size"]=sData[k]["option"].get("size", 12).asInt();
		p["tag"]=sData[k]["option"].get("tag", 0).asInt();
		
		texts.push_back({sData[k]["content"].asString(),p.toStyledString()});
		
		if(sData[k]["option"].get("newline",0).asInt()>0){
			Json::Value nl;
			nl["linebreak"]=true;
			nl["linespacing"]=sData[k]["option"].get("newline",0).asInt();
			texts.push_back({"",nl.toStyledString()});
		}
	}
	
	StyledLabelTTF* slttf = StyledLabelTTF::create(texts, sa);
	
	return slttf;
	
}

CCLabelTTF* StyledLabelTTF::getLabelByTag(int tag){
	return (CCLabelTTF*)(m_oneLineContainer->getChildByTag(tag));
}