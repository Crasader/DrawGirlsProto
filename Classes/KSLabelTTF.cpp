#include "KSLabelTTF.h"
#include "FormSetter.h"
void KSLabelTTF::enableOuterStroke(const ccColor3B &strokeColor, float strokeSize, bool mustUpdateTexture)
{
	m_outerStrokeOpacity = 255;
	m_outerIsStroke = true;
	m_outerStrokeColor = strokeColor;
	m_outerStrokeSize = strokeSize;
	updateTexture();
}

void KSLabelTTF::enableOuterStroke(const ccColor3B &strokeColor, float strokeSize, GLubyte strokeOpacity, bool mustUpdateTexture)
{
	m_outerStrokeOpacity = strokeOpacity;
	m_outerIsStroke = true;
	m_outerStrokeColor = strokeColor;
	m_outerStrokeSize = strokeSize;
	updateTexture();
}


void KSLabelTTF::setColor(ccColor3B t_color)
{
	if(t_color.r + t_color.g + t_color.b < 100)
	{
		//		enableOuterStroke(ccWHITE, m_outerStrokeSize, m_outerStrokeOpacity);
		disableOuterStroke();
	}
	CCLabelTTF::setColor(t_color);
}


void KSLabelTTF::disableOuterStroke(bool mustUpdateTexture)
{
	m_outerIsStroke = false;
	updateTexture();
}
bool KSLabelTTF::updateTexture()
{
	CCTexture2D *tex;
	tex = new CCTexture2D();
	
	if (!tex)
		return false;
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	
	ccFontDefinition texDef = _prepareTextDefinition(true);
	tex->initWithString( m_string.c_str(), &texDef );
	
#else
	
	tex->initWithString( m_string.c_str(),
											m_pFontName->c_str(),
											m_fFontSize * CC_CONTENT_SCALE_FACTOR(),
											CC_SIZE_POINTS_TO_PIXELS(m_tDimensions),
											m_hAlignment,
											m_vAlignment);
	
#endif
	
	// set the texture
	this->setTexture(tex);
	// release it
	tex->release();
	if(m_outerSprite)
	{
		m_outerSprite->removeFromParent();
		m_outerSprite = nullptr;
	}
	bool validString = false;
	for(auto i : m_string)
	{
		if(i != ' ')
		{
			validString = true;
			break;
		}
	}
	if(!validString)
	{
		
		return true;
	}
		
//	startFormSetter(this);
	auto textureForGradient = CCSprite::createWithTexture(getTexture());
	
	CCRect rect =CCRectZero;
	rect.size   = m_pobTexture->getContentSize();
	this->setTextureRect(rect);
	
	
	if(this->m_outerIsStroke)
	{
		CCSprite* label = CCSprite::createWithTexture(getTexture());
		auto oFlip = label->isFlipY();
		auto oColor = label->getColor();
		auto oPosition = label->getPosition();
		CCRenderTexture* rt = CCRenderTexture::create(tex->getContentSize().width + m_outerStrokeSize*2 , tex->getContentSize().height+m_outerStrokeSize*2);
		
		label->setFlipY(!oFlip);
		label->setColor(m_outerStrokeColor);
		
		ccBlendFunc originalBlendFunc = label->getBlendFunc();
		ccBlendFunc _t = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
		label->setBlendFunc(_t);
		CCPoint bottomLeft = ccp(label->getTexture()->getContentSize().width * label->getAnchorPoint().x + m_outerStrokeSize, label->getTexture()->getContentSize().height * label->getAnchorPoint().y + m_outerStrokeSize);
		CCPoint position = ccpSub(label->getPosition(), ccp(-label->getContentSize().width / 2.0f,-label->getContentSize().height / 2.0f));
		
		rt->begin();
		
		float devider = 8;
		for (int i=0; i<360; i+=360 / devider) // you should optimize that for your needs
		{
			label->setPosition(ccp(bottomLeft.x + sin(CC_DEGREES_TO_RADIANS(i))*m_outerStrokeSize,bottomLeft.y + cos(CC_DEGREES_TO_RADIANS(i))*m_outerStrokeSize));
			label->visit();
		}
		
		rt->end();
		label->setFlipY(oFlip);
		label->setColor(oColor);
		label->setBlendFunc(originalBlendFunc);
		m_outerSprite = CCSprite::createWithTexture(rt->getSprite()->getTexture());
		m_outerSprite->setOpacity(m_outerStrokeOpacity);
		addChild(m_outerSprite, -1);
		m_outerSprite->setPosition(ccp(getContentSize().width / 2.f, getContentSize().height / 2.f));
		label->setPosition(oPosition);
	}
	else
	{
	}
	//ok
	
	// 그라데이션 처리.
	if(m_gradationMode)
	{
		if(m_clippingNodeForGra)
		{
			m_clippingNodeForGra->removeFromParent();
		}
		CCClippingNode* cNode = CCClippingNode::create();
//		textureForGradient->getTexture()->setAliasTexParameters();
		cNode->setStencil(textureForGradient);
		cNode->setAlphaThreshold(0.1f);
		CCLayerGradient* cclg = CCLayerGradient::create(m_startColor, m_endColor, m_alongVector);
		cclg->setAnchorPoint(ccp(0.5f, 0.5f));
		cclg->ignoreAnchorPointForPosition(false);
		cNode->addChild(cclg);
		cclg->setContentSize(textureForGradient->getContentSize());
		addChild(cNode);
		cNode->setPosition(ccp(getContentSize().width / 2.f, getContentSize().height / 2.f));
		m_clippingNodeForGra = cNode;
	}
	return true;
}

void KSLabelTTF::setEnableItalic(float value)
{
	this->setSkewX(value);
}

void KSLabelTTF::setDisableItalic()
{
	this->setSkewX(0.f);
}
void KSLabelTTF::setOpacity(GLubyte opacity)
{
	CCLabelTTF::setOpacity(opacity);
	if(m_outerSprite)
	{
		m_outerSprite->setOpacity(opacity);
	}
	//	updateTexture();
}
void KSLabelTTF::setString(const char *string)
{
	CCAssert(string != NULL, "Invalid string");
	
	if (m_string.compare(string))
	{
		m_string = string;
		
		this->updateTexture();
	}
}

void KSLabelTTF::enableGradation(ccColor4B startColor, ccColor4B endColor, CCPoint alongVector)
{
	m_startColor = startColor;
	m_endColor = endColor;
	m_alongVector = alongVector;
	m_gradationMode = true;
	updateTexture();
	
}
void KSLabelTTF::disableGradation()
{
	m_gradationMode = false;
	if(m_clippingNodeForGra)
	{
		m_clippingNodeForGra->removeFromParent();
		m_clippingNodeForGra = nullptr;
	}
	updateTexture();
}