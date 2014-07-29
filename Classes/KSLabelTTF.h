#pragma once


#include "cocos2d.h"

USING_NS_CC;
#include <string>

#if CC_USE_LA88_LABELS
#define SHADER_PROGRAM kCCShader_PositionTextureColor
#else
#define SHADER_PROGRAM kCCShader_PositionTextureA8Color
#endif


class KSLabelTTF : public CCLabelTTF
{
public:
	KSLabelTTF() : m_outerIsStroke(false), m_outerSprite(nullptr), m_renderTexture(nullptr),
	m_gradationMode(false),
	m_clippingNodeForGra(nullptr)
	{
	}
	virtual void enableOuterStroke(const ccColor3B &strokeColor, float strokeSize, bool mustUpdateTexture = true);
	virtual void enableOuterStroke(const ccColor3B &strokeColor, float strokeSize,GLubyte strokeOpacity, bool mustUpdateTexture = true);
	virtual void disableOuterStroke(bool mustUpdateTexture = true);
	virtual bool updateTexture();
	virtual void setEnableItalic(float value = 30.f);
	virtual void setDisableItalic();
	virtual void setOpacity(GLubyte opacity);
	virtual void setOpacityOuterStroke(GLubyte opa);
	
	virtual void setColor(ccColor3B t_color);
	virtual void draw(void);
	virtual void setGradientColor(const ccColor4B& start, const ccColor4B& end, const CCPoint& v);
	//	void draw();
	//virtual void setString(const char *label);
	static KSLabelTTF * create()
	{
		KSLabelTTF * pRet = new KSLabelTTF();
		if (pRet && pRet->init())
		{
			pRet->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(pRet);
		}
		return pRet;
	}
	
	static KSLabelTTF * create(const char *string, const char *fontName, float fontSize)
	{
		return create(string, fontName, fontSize,
									CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
	}
	
	static KSLabelTTF * create(const char *string, const char *fontName, float fontSize,
														 const CCSize& dimensions, CCTextAlignment hAlignment)
	{
		return create(string, fontName, fontSize, dimensions, hAlignment, kCCVerticalTextAlignmentTop);
	}
	
	static KSLabelTTF* create(const char *string, const char *fontName, float fontSize,
														const CCSize &dimensions, CCTextAlignment hAlignment,
														CCVerticalTextAlignment vAlignment)
	{
		KSLabelTTF *pRet = new KSLabelTTF();
		if(pRet && pRet->initWithString(string, fontName, fontSize, dimensions, hAlignment, vAlignment))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
	
	static KSLabelTTF * createWithFontDefinition(const char *string, ccFontDefinition &textDefinition)
	{
		KSLabelTTF *pRet = new KSLabelTTF();
		if(pRet && pRet->initWithStringAndTextDefinition(string, textDefinition))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
	
	virtual bool init()
	{
		return this->initWithString("", "Helvetica", 12);
	}
	
	virtual bool initWithString(const char *label, const char *fontName, float fontSize,
															const CCSize& dimensions, CCTextAlignment alignment)
	{
		return this->initWithString(label, fontName, fontSize, dimensions, alignment, kCCVerticalTextAlignmentTop);
	}
	
	virtual bool initWithString(const char *label, const char *fontName, float fontSize)
	{
		return this->initWithString(label, fontName, fontSize,
																CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
	}
	
	virtual bool initWithString(const char *string, const char *fontName, float fontSize,
															const cocos2d::CCSize &dimensions, CCTextAlignment hAlignment,
															CCVerticalTextAlignment vAlignment);
	
	
	virtual void setString(const char *string);
	void enableGradation(ccColor4B startColor, ccColor4B endColor, CCPoint alongVector);
	void disableGradation();
	void updateColor();
protected:
	float m_outerStrokeSize;
	ccColor3B m_outerStrokeColor;
	bool m_outerIsStroke;
	CCSprite* m_outerSprite;
	GLubyte m_outerStrokeOpacity;
	CCRenderTexture* m_renderTexture;
	bool m_gradationMode;
	ccColor4B m_startColor;
	ccColor4B m_endColor;
	CCPoint m_alongVector; // 그라데이션 방향.
	CCClippingNode* m_clippingNodeForGra;
	ccColor4F  m_pSquareColors[4];	
//	CC_PROPERTY_PASS_BY_REF(ccColor3B, m_startColor, StartColor)
//	CC_PROPERTY_PASS_BY_REF(ccColor3B, m_endColor, EndColor)
//	CC_PROPERTY(GLubyte, m_cStartOpacity, StartOpacity)
//	CC_PROPERTY(GLubyte, m_cEndOpacity, EndOpacity)
//	CC_PROPERTY_PASS_BY_REF(CCPoint, m_AlongVector, Vector)
};
