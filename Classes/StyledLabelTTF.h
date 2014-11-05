#ifndef STYLEDLABELTTF_H
#define STYLEDLABELTTF_H

#include "cocos2d.h"
USING_NS_CC;
#include <string>
#include <boost/format.hpp>
#include <vector>

// 문자열 중간에 수정법.
	/*
	 * auto ret = ???->getTexts();
	 * ret[1] = "2/2";
	 * ???->updateTexture();
	 *
	 * */
/*	
	Json::Value value1, value2, value3, value4, value5, value6;
	value1["fillcolor"] = StyledLabelTTF::makeRGB(255, 0, 0);
	value1["font"] = mySGD->getFont().c_str();
	value1["size"] = 15.f;
	value2["fillcolor"] = StyledLabelTTF::makeRGB(0, 255, 0);
	value2["font"] = mySGD->getFont().c_str();
	value2["size"] = 15.f;

	value3["linebreak"] = true;
	value3["linespacing"] = 30.f;
	
	value4["fillcolor"] = StyledLabelTTF::makeRGB(0, 0, 255);
	value4["font"] = mySGD->getFont();
	value4["size"] = 20.f;
	
	value5["fillcolor"] = StyledLabelTTF::makeRGB(255, 0, 255);
	value5["font"] = mySGD->getFont();
	value5["size"] = 25.f;
	
	
	StyledLabelTTF* slttf = StyledLabelTTF::create({
		{"가을하늘공활한데", value1.toStyledString()},
		{"높고구름없이", value2.toStyledString()},
		{"", value3.toStyledString()},
		{"밝은달은우리가슴", value4.toStyledString()},
		{"일편단심일세", value5.toStyledString()},
		{"", value3.toStyledString()}
	}, StyledAlignment::kCenterAlignment);
	slttf->setPosition(ccp(240, 240));
	addChild(slttf);
	{
		Json::Value value1, value2, value3, value4, value5, value6;
		value1["fillcolor"] = StyledLabelTTF::makeRGB(255, 0, 0);
		value1["font"] = mySGD->getFont().c_str();
		value1["size"] = 15.f;
		value2["fillcolor"] = StyledLabelTTF::makeRGB(0, 255, 0);
		value2["font"] = mySGD->getFont().c_str();
		value2["size"] = 15.f;
		
		value3["linebreak"] = true;
		value3["linespacing"] = 30.f;
		
		value4["fillcolor"] = StyledLabelTTF::makeRGB(0, 0, 255);
		value4["font"] = mySGD->getFont();
		value4["size"] = 20.f;
		
		value5["fillcolor"] = StyledLabelTTF::makeRGB(255, 0, 255);
		value5["font"] = mySGD->getFont();
		value5["size"] = 25.f;
		
		
		StyledLabelTTF* slttf = StyledLabelTTF::create({
			{"동해물과백두산이", value1.toStyledString()},
			{"마르고닳도록", value2.toStyledString()},
			{"", value3.toStyledString()},
			{"하느님이보우하사", value4.toStyledString()},
			{"우리나라만세", value5.toStyledString()},
			{"", value3.toStyledString()}
		}, StyledAlignment::kLeftAlignment);
		slttf->setPosition(ccp(0, 160));
		addChild(slttf);
	}
*/	
struct StyledText
{
	std::string m_text;
	std::string m_style; // style json;
	StyledText(const std::initializer_list<std::string>& _list)
	{
		int c = 0;
		for(auto i : _list)
		{
			if(c == 0)
				m_text = i;
			else if(c == 1)
				m_style = i;
			c++;
		}
	}
	StyledText(const std::string& _style)
	{
		m_style = _style;
	}
};

enum class StyledAlignment
{
	kLeftAlignment,
	kCenterAlignment,
	kRightAlignment
};

class KSLabelTTF;
class StyledLabelTTF : public CCNode
{
public:
	StyledLabelTTF();
	virtual ~StyledLabelTTF();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	bool init(const std::vector<StyledText>& texts, StyledAlignment sa);
	static StyledLabelTTF* create()
	{
		StyledLabelTTF* t = new StyledLabelTTF();
		t->init();
		t->autorelease();
		return t;
	}
	static StyledLabelTTF* create(const std::vector<StyledText>& texts, StyledAlignment sa)
	{
		StyledLabelTTF* t = new StyledLabelTTF();
		t->init(texts, sa);
		t->autorelease();
		return t;
	}
	
	static StyledLabelTTF* create(const char* text,const char* defaultFont,int defaultFontSize, int defaultFontColor,StyledAlignment sa);
	
	bool isSameStringAtIndex(std::string str, int idx, std::string option);
	
	std::string getSameStringAtIndex(std::string str, int idx, std::string option);
	void updateTexture();
	static unsigned int makeRGB(unsigned char R, unsigned char G, unsigned char B)
	{
		return R | (G << 8) | (B << 16);
	}
	static unsigned char getRed(unsigned int r)
	{
		return r;
	}
	static unsigned char getGreen(unsigned int r)
	{
		return r >> 8;
	}
	static unsigned char getBlue(unsigned int r)
	{
		return r >> 16;
	}
	
	//void setString(const char* text);
	void setStringByTag(const char* text);
	KSLabelTTF* getLabelByTag(int tag);
	void setOldAnchorPoint();
	//virtual void registerWithTouchDispatcher();
protected:
	float m_firstLineMaxY;
	float m_firstLineMinY;
	float m_currentPosition;
	float m_oneLineSize;
	CCNode* m_oneLineContainer;
//	float m_lineSpacing;
	float m_currentLinePosition;
	std::string m_font;
	std::string m_string;
	int m_fontSize;
	int m_fontColor;
	std::vector<StyledText> m_texts;
	StyledAlignment m_currentAlignment;
	

	
	float m_maxWidth;
public:
	
	void setColorPos(ccColor3B _color);
	std::vector<StyledText> getTexts()
	{
		return m_texts;
	}
	void enableOuterStrokePos(const ccColor3B &strokeColor, float strokeSize, GLubyte strokeOpacity, bool mustUpdateTexture);
	std::vector<StyledText> getTrimedTexts(const std::vector<StyledText>& texts, int s, int e);
	CC_SYNTHESIZE(int, m_trimStart, TrimStart);
	CC_SYNTHESIZE(int, m_trimEnd, TrimEnd);
	void trimText(int s, int e);
	int getTextLength();
};

#endif
