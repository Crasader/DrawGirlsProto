#include "StyledLabelTTF.h"
#include "jsoncpp/json.h"
#include "ks19937.h"
#include "KSLabelTTF.h"
using namespace boost;

StyledLabelTTF::StyledLabelTTF() :
m_currentPosition(0.f),
m_oneLineSize(0.f),
m_currentLinePosition(0.f),
m_fontSize(12.f),
m_fontColor(999)
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
	m_oneLineContainer->setTag(1);
	addChild(m_oneLineContainer);
	
	Json::Value jsonStyle;
	for(auto iter = m_texts.begin(); iter != m_texts.end(); ++iter)
	{
		StyledText t = *iter;
		
		StyledText st = t;
		
		Json::Reader reader;
		reader.parse(st.m_style, jsonStyle);
		
		this->setContentSize(CCSizeMake(m_currentPosition, abs(m_currentLinePosition)));
		
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
			m_oneLineContainer->setTag(1);
			addChild(m_oneLineContainer);
			m_currentLinePosition -= jsonStyle.get("linespacing", 18.f).asFloat();
			m_currentPosition = 0.f;
			
			
			
		}
		else
		{
			KSLabelTTF* ttf = KSLabelTTF::create(st.m_text.c_str(), jsonStyle.get("font", "").asString().c_str(), jsonStyle.get("size", 12.f).asFloat());
			float strokeSize = jsonStyle.get("strokesize", 0.f).asFloat();
			unsigned long strokeColor = jsonStyle.get("strokecolor", 0).asUInt();
			if(strokeSize > 0.f)
			{
				ttf->enableStroke(ccc3(getRed(strokeColor), getGreen(strokeColor), getBlue(strokeColor)), strokeSize);
			}
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




StyledLabelTTF* StyledLabelTTF::create(const char* text,const char* defaultFont,int defaultFontSize, int defaultFontColor,StyledAlignment sa){
	
	
	
	StyledLabelTTF* slttf = StyledLabelTTF::create();
	slttf->m_font = defaultFont;
	slttf->m_fontSize = defaultFontSize;
	slttf->m_fontColor= defaultFontColor;
	slttf->m_currentAlignment = sa;
	slttf->setStringByTag(text);
	return slttf;
	
}
//
//void StyledLabelTTF::setString(const char* text){
//	if(m_string==text)return;
//	
//	//<|rgb|size|newline|tag>   ex)   <|900|15|15|>안녕하세요   ->빨간색 15 사이즈 텍스트
//	std::string str = text;
//	bool isSetMode=false;
//	std::string content="";
//	std::string option="";
//	Json::Value sData;
//	int labelNo=0;
//	int optionNo=0;
//	for (int i=0; i<str.length(); i++) {
//		if((str[i]=='<' && str[i+1]=='|') || i==str.length()-1){
//			if(i==str.length()-1)content+=str[i];
//			sData[labelNo]["content"]=content;
//			
//			if(i!=0)labelNo++;
//			
//			isSetMode=true;
//			content="";
//			optionNo=0;
//			i++;
//			
//			continue;
//		}
//		
//		
//		
//		if(isSetMode){
//			if(str[i]=='|'){
//				//모은옵션스트링저장하기
//				if(optionNo==0)sData[labelNo]["option"]["color"]=std::atoi(option.c_str());
//				else if(optionNo==1)sData[labelNo]["option"]["size"]=std::atoi(option.c_str());
//				else if(optionNo==2)sData[labelNo]["option"]["newline"]=std::atoi(option.c_str());
//				else if(optionNo==3)sData[labelNo]["option"]["tag"]=std::atoi(option.c_str());
//				
//				option="";
//				optionNo++;
//				
//				
//				if(str[i]=='|' && str[i+1]=='>'){
//					i++;
//					isSetMode=false;
//					continue;
//				}
//				continue;
//			}
//			
//			if(str[i]==' ')continue;
//			option+=str[i];
//			
//		}else{
//			if(str[i]=='\n')continue;
//			
//			content+=str[i];
//			
//		}
//	}
//	
//	
//	
//	std::vector<StyledText> texts;
//	for(int k = 0;k<sData.size();k++){
//		Json::Value p;
//		int rgb = sData[k]["option"].get("color", 000).asInt();
//		p["fillcolor"]=StyledLabelTTF::makeRGB((rgb/100)/9.f*255, (rgb/10%10)/9.f*255, (rgb%10)/9.f*255);
//		p["font"]=m_font;
//		p["size"]=sData[k]["option"].get("size", 12).asInt();
//		p["tag"]=sData[k]["option"].get("tag", 0).asInt();
//		
//		texts.push_back({sData[k]["content"].asString(),p.toStyledString()});
//		
//		if(sData[k]["option"].get("newline",0).asInt()>0){
//			Json::Value nl;
//			nl["linebreak"]=true;
//			nl["linespacing"]=sData[k]["option"].get("newline",0).asInt();
//			texts.push_back({"",nl.toStyledString()});
//		}
//	}
//	
//	Json::Value nl;
//	
//	nl["linebreak"]=true;
//	nl["linespacing"]=1;
//	texts.push_back({"",nl.toStyledString()});
//	
//	
//	m_texts.clear();
//	
//	m_texts=texts;
//	m_string=text;
//	updateTexture();
//	
//}
//


bool StyledLabelTTF::isSameStringAtIndex(std::string str, int idx, std::string option){
	int j=0;
	
	for(int i=idx;i<idx+option.length();i++){
		if(i>=str.length())return false;
		if(str[i]!=option[j])return false;
		j++;
	}
	
	return true;
}

void StyledLabelTTF::setStringByTag(const char* text){
	if(m_string==text)return;
	
	//<|rgb|size|newline|tag>   ex)   <|900|15|15|>안녕하세요   ->빨간색 15 사이즈 텍스트
	std::string str = text;
	bool isSetMode=false;
	std::string content="";
	std::string option="";
	std::string optionName="";
	Json::Value sData;
	int labelNo=0;
	int optionNo=0;
	for (int i=0; i<str.length(); i++) {
		if(isSameStringAtIndex(str, i, "<font") || i==str.length()-1){
			if(i==str.length()-1)content+=str[i];
			sData[labelNo]["content"]=content;
			
			
			if(i!=0)labelNo++;
			
			isSetMode=true;
			content="";
			optionNo=0;
			optionName="";
			//i++;
			i+=4;
			
			continue;
		}
		
		
		
		if(isSetMode){
			if(str[i]==' ' || str[i]=='>'){
				//모은옵션스트링저장하기
//				if(optionNo==0)sData[labelNo]["option"]["color"]=std::atoi(option.c_str());
//				else if(optionNo==1)sData[labelNo]["option"]["size"]=std::atoi(option.c_str());
//				else if(optionNo==2)sData[labelNo]["option"]["newline"]=std::atoi(option.c_str());
//				else if(optionNo==3)sData[labelNo]["option"]["tag"]=std::atoi(option.c_str());
//				
				if(optionName!="")sData[labelNo][optionName]=option.c_str();
				//CCLog("option : %s -> value : %s",optionName.c_str(),option.c_str());
				option="";
				optionName="";
				optionNo++;
				
				
				if(str[i]=='>'){
					isSetMode=false;
					continue;
				}
				continue;
			}
			

			if(isSameStringAtIndex(str, i, "size=")){optionName="size"; i+=4; continue;}
			if(isSameStringAtIndex(str, i, "color=")){optionName="color"; i+=5; continue;}
			if(isSameStringAtIndex(str, i, "tag=")){optionName="tag"; i+=3; continue;}
			if(isSameStringAtIndex(str, i, "font=")){optionName="font"; i+=4; continue;}
			if(isSameStringAtIndex(str, i, "newline=")){optionName="newline"; i+=7; continue;}
			
			if(str[i]==' ')continue;
			
			option+=str[i];
			
		}else{
			if(str[i]=='\n')continue;
			if(isSameStringAtIndex(str, i, "</font>")){i+=6;continue;}
			content+=str[i];
			
		}
	}
	
	
	//CCLog("ok go - %s",sData.toStyledString().c_str());
	
	std::vector<StyledText> texts;
	for(int k = 0;k<sData.size();k++){
		Json::Value p;
		int rgb = sData[k].get("color", m_fontColor).asInt();
		p["fillcolor"]=StyledLabelTTF::makeRGB((rgb/100)/9.f*255, (rgb/10%10)/9.f*255, (rgb%10)/9.f*255);
		p["size"]=sData[k].get("size", m_fontSize).asInt();
		p["tag"]=sData[k].get("tag", 0).asInt();
		p["font"]=sData[k].get("font", m_font).asString();
		
		texts.push_back({sData[k]["content"].asString(),p.toStyledString()});
		
		//CCLog("ok new ttf is %s and %s",sData[k]["content"].asString().c_str(),p.toStyledString().c_str());
		if(sData[k].get("newline",0).asInt()>0){
			Json::Value nl;
			nl["linebreak"]=true;
			nl["linespacing"]=sData[k].get("newline",0).asInt();
			texts.push_back({"",nl.toStyledString()});
		}
	}
	
	Json::Value nl;
	
	nl["linebreak"]=true;
	nl["linespacing"]=1;
	texts.push_back({"",nl.toStyledString()});
	
	m_texts.clear();
	
	m_texts=texts;
	m_string=text;
	updateTexture();
	
}

KSLabelTTF* StyledLabelTTF::getLabelByTag(int tag){
	for(int i=0;i<this->getChildrenCount();i++){
		CCNode* lineNode = (CCNode*)this->getChildren()->objectAtIndex(i);
		if(lineNode->getTag()==1){
			for(int j=0;j<lineNode->getChildrenCount();j++){
				KSLabelTTF* label = (KSLabelTTF *)lineNode->getChildByTag(tag);
				if (label) {
					return label;
				}
			}
		}
	}
	
	return nullptr;
}




