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

	float maxY = 0.f;
	float minY = 0.f;
	float maxX = 0.f;
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
			if(st.m_text != "")
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
				ttf->setColor(ccc3(getRed(fillColor), getGreen(fillColor), getBlue(fillColor)));
				ttf->setTag(jsonStyle.get("tag", 0).asInt());
				m_currentPosition += ttf->getContentSize().width;
				m_oneLineSize += ttf->getContentSize().width;
				maxY = MAX(maxY, m_currentLinePosition + ttf->getContentSize().height / 2.f);
				minY = MIN(minY, m_currentLinePosition - ttf->getContentSize().height / 2.f);
				maxX = MAX(maxX, m_oneLineSize);
			}
			else
			{
				CCSprite* sprite = CCSprite::create(jsonStyle["img"].asString().c_str());
				sprite->setScale(jsonStyle.get("scale", 1.f).asFloat());
				
				m_oneLineContainer->addChild(sprite);
				sprite->setAnchorPoint(ccp(0.f, 0.5f));
				sprite->setPosition(ccp(m_currentPosition, m_currentLinePosition));
				
				m_currentPosition += sprite->getContentSize().width;
				m_oneLineSize += sprite->getContentSize().width;
				maxY = MAX(maxY, m_currentLinePosition + sprite->getContentSize().height * sprite->getScaleY() / 2.f);
				minY = MIN(minY, m_currentLinePosition - sprite->getContentSize().height * sprite->getScaleY() / 2.f);
				maxX = MAX(maxX, m_oneLineSize);
			}
		}
	}
	
	setContentSize(CCSizeMake(maxX, maxY - minY));
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
	Json::Value sData;
	int labelNo=0;
	int lastIncludeContentTagNo=-1;
	for (int i=0; i<str.length(); i++) {
		
		//닫는태그처리
		if(isSameStringAtIndex(str, i,"</")){
			i++;
			while (str[i]!='>') {
				i++;
			}
		//태그시작
		}else if(str[i]=='<'){
			//1.태그이름 뽑기
			std::string tagName="";
			i++;
			int overCount=0;
			while(str[i]!=' ' && str[i]!='>'){
				tagName+=str[i];
				i++;
				overCount++;
				if(overCount>100)assert("tag name over");
			}
			if(str[i]!='>')i++;
			CCLog("tagName is %s at %d",tagName.c_str(),labelNo);
			sData[labelNo]["tagName"]=tagName;
			
			//내용을 포함하는 태그종류는 현재번호를 등록해놓기
			if(tagName=="font")lastIncludeContentTagNo=labelNo;
			
			labelNo++;
			
			//2.태그옵션뽑기
			overCount=0;
			while (str[i]!='>') {
				std::string optionName="";
				std::string optionValue="";
				bool isOptionName=true;
				
				int overCount2=0;
				while(str[i]!=' ' && str[i]!='>'){
					if(str[i]=='='){
						i++;
						isOptionName=false;
						continue;
					}
					
					if(!isOptionName && str[i]=='"'){
						i++;
						continue;
					}
					
					if(isOptionName)optionName+=str[i];
					else optionValue+=str[i];
					
					i++;
					overCount2++;
					if(overCount2>100)assert("option name over");
				}
				
				
				
				CCLog("tagOption is %s = %s , i = %d",optionName.c_str(),optionValue.c_str(),i);
				//i=i+ti+oi;
				if(optionName!="")sData[labelNo-1][optionName.c_str()]=optionValue.c_str();
				
				if(str[i]=='>')break;
				i++;
				overCount++;
				if(overCount>10000)assert("tag option over");
			}
			CCLog("tag number %d ok , i = %d",labelNo-1,i);
		//내용모으기 및 적용
		}else{
			//int ti=0;
			CCLog("take content %d",i);
			std::string content="";
			while(str[i]!='<'){
				if(i>str.length())break;
				if(str[i]!='\n')content+=str[i];
				i++;
				//if(ti>100000)assert("content over");
			}
			i--;
			
			if(lastIncludeContentTagNo>=0)sData[lastIncludeContentTagNo]["content"]=content;
			else CCLog("dont find set content");
		}
		
		
//		
//		
//		
//		
//		
//		
//		if(i==str.length()-1 || ((str[i]=='<' && isSetMode==false))){
//			
//			if(isSameStringAtIndex(str, i, "</")){
//				//닫는태그처리
//				int ti=1;
//				while(str[i+ti]!='>'){
//					if(ti>100)break;
//					ti++;
//				}
//				i+=ti;
//				continue;
//			}
//			
//			if(i==str.length()-1)content+=str[i];
//			sData[labelNo]["content"]=content;
//			if(i==str.length()-1)break;
//			
//			if(i!=0)labelNo++;
//			
//			isSetMode=true;
//			content="";
//			optionNo=0;
//			optionName="";
//			//i++;
//			
//			int ti=1;
//			std::string tagName="";
//			while(str[i+ti]!=' ' && str[i+ti]!='>'){
//				tagName+=str[i+ti];
//				if(ti>100)break;
//				ti++;
//			}
//			i+=ti-1;
//			sData[labelNo]["tagname"]=tagName;
//			continue;
//		}
//		
//		
//		
//		if(isSetMode){
//			if(str[i]==' ' || str[i]=='>'){
//				//모은옵션스트링저장하기
////				if(optionNo==0)sData[labelNo]["option"]["color"]=std::atoi(option.c_str());
////				else if(optionNo==1)sData[labelNo]["option"]["size"]=std::atoi(option.c_str());
////				else if(optionNo==2)sData[labelNo]["option"]["newline"]=std::atoi(option.c_str());
////				else if(optionNo==3)sData[labelNo]["option"]["tag"]=std::atoi(option.c_str());
////				
//				if(optionName!="")sData[labelNo][optionName]=option.c_str();
//				//CCLog("option : %s -> value : %s",optionName.c_str(),option.c_str());
//				option="";
//				optionName="";
//				optionNo++;
//				
//				
//				if(str[i]=='>'){
//					isSetMode=false;
//					continue;
//				}
//				continue;
//			}
//			
//
//			if(isSameStringAtIndex(str, i, "size=")){optionName="size"; i+=4; continue;}
//			if(isSameStringAtIndex(str, i, "color=")){optionName="color"; i+=5; continue;}
//			if(isSameStringAtIndex(str, i, "tag=")){optionName="tag"; i+=3; continue;}
//			if(isSameStringAtIndex(str, i, "font=")){optionName="font"; i+=4; continue;}
//			if(isSameStringAtIndex(str, i, "newline=")){optionName="newline"; i+=7; continue;}
//			
//			if(str[i]==' ')continue;
//			
//			option+=str[i];
//			
//		}else{
//			if(str[i]=='\n')continue;
//			//if(isSameStringAtIndex(str, i, "</font>")){i+=6; continue;}
//			//if(isSameStringAtIndex(str, i, "<br>")){};
//			content+=str[i];
//			
//		}
	}
	
	
	CCLog("ok go - %s",sData.toStyledString().c_str());
	
	std::vector<StyledText> texts;
	for(int k = 0;k<sData.size();k++){
		Json::Value p;
		int rgb = sData[k].get("color", m_fontColor).asInt();
		p["tagName"]=sData[k].get("tagName", "").asString();
		p["img"]=sData[k].get("src", "").asString();
		p["fillcolor"]=StyledLabelTTF::makeRGB((rgb/100)/9.f*255, (rgb/10%10)/9.f*255, (rgb%10)/9.f*255);
		p["size"]=sData[k].get("size", m_fontSize).asInt();
		p["tag"]=sData[k].get("tag", 0).asInt();
		p["font"]=sData[k].get("font", m_font).asString();
		auto strokeColor = sData[k].get("strokecolor", 0).asUInt();

		p["strokecolor"]=StyledLabelTTF::makeRGB((strokeColor/100)/9.f*255, (strokeColor/10%10)/9.f*255, (strokeColor%10)/9.f*255);
		p["strokesize"]=sData[k].get("strokesize", 0.f).asFloat(); 
		texts.push_back({sData[k].get("content", "").asString(),p.toStyledString()});
		
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




