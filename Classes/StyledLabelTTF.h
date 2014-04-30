#ifndef STYLEDLABELTTF_H
#define STYLEDLABELTTF_H

#include "cocos2d.h"
USING_NS_CC;
#include <string>
#include <boost/format.hpp>
#include <vector>

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
//		m_text = _list;
//		m_style = _list[1];
	}
};

enum class StyledAlignment
{
	kLeftAlignment,
	kCenterAlignment,
	kRightAlignment
};

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
	
	static StyledLabelTTF* create(const char* text,const char* font ,StyledAlignment sa);
	CCLabelTTF* getLabelByTag(int tag);
	
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
	//virtual void registerWithTouchDispatcher();
protected:
	float m_currentPosition;
	float m_oneLineSize;
	CCNode* m_oneLineContainer;
//	float m_lineSpacing;
	float m_currentLinePosition;
	std::vector<StyledText> m_texts;
	StyledAlignment m_currentAlignment;
};

#endif
