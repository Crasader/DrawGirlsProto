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
	m_gradationMode = false;
	if(t_color.r + t_color.g + t_color.b < 100)
	{
		//		enableOuterStroke(ccWHITE, m_outerStrokeSize, m_outerStrokeOpacity);
		disableOuterStroke();
	}
	CCLabelTTF::setColor(t_color);
}


void KSLabelTTF::setGradientColor(const ccColor4B& start, const ccColor4B& end, const CCPoint& v)
{
	m_gradationMode = true;
	m_startColor = start;
	m_endColor = end;
	m_alongVector = v;
	
	//	CCLabelTTF::setColor(ccc3(m_startColor.r, m_startColor.g, m_startColor.b));
	updateColor();
}

											
void KSLabelTTF::draw(void)
{
	CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
	
	CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");
	
	CC_NODE_DRAW_SETUP();
	
	ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
	
	ccGLBindTexture2D( m_pobTexture->getName() );
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
//	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color );

#define kQuadSize sizeof(m_sQuad.bl)
#ifdef EMSCRIPTEN
	long offset = 0;
	setGLBufferData(&m_sQuad, 4 * kQuadSize, 0);
#else
	long offset = (long)&m_sQuad;
#endif // EMSCRIPTEN
	
	// vertex
	int diff = offsetof( ccV3F_C4B_T2F, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
	
	/////////////////
//	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, m_pSquareVertices);
	//////////////////
	// texCoods
	diff = offsetof( ccV3F_C4B_T2F, texCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
	
	// color
	diff = offsetof( ccV3F_C4B_T2F, colors);
	m_pSquareColors[0] = ccc4FFromccc4B(m_sQuad.tl.colors);
	m_pSquareColors[1] = ccc4FFromccc4B(m_sQuad.bl.colors);
	m_pSquareColors[2] = ccc4FFromccc4B(m_sQuad.br.colors);
	m_pSquareColors[3] = ccc4FFromccc4B(m_sQuad.tr.colors);
	if(m_pSquareColors[0].a != 1.f ||
		 m_pSquareColors[1].a != 1.f ||
		 m_pSquareColors[2].a != 1.f ||
		 m_pSquareColors[3].a != 1.f)
	{
		CCLog("breakp");
	}
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, m_pSquareColors);
//	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, kQuadSize, m_pSquareColors);
	
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	CHECK_GL_ERROR_DEBUG();
	
	
#if CC_SPRITE_DEBUG_DRAW == 1
	// draw bounding box
	CCPoint vertices[4]={
		ccp(m_sQuad.tl.vertices.x,m_sQuad.tl.vertices.y),
		ccp(m_sQuad.bl.vertices.x,m_sQuad.bl.vertices.y),
		ccp(m_sQuad.br.vertices.x,m_sQuad.br.vertices.y),
		ccp(m_sQuad.tr.vertices.x,m_sQuad.tr.vertices.y),
	};
	ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
	// draw texture box
	CCSize s = this->getTextureRect().size;
	CCPoint offsetPix = this->getOffsetPosition();
	CCPoint vertices[4] = {
		ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
		ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
	};
	ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW
	
	CC_INCREMENT_GL_DRAWS(1);
	
	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
	
}
void KSLabelTTF::updateColor()
{
	if(m_gradationMode == false)
	{
		CCLabelTTF::updateColor();
		return;
	}
	else
	{
		float h = ccpLength(m_alongVector);
		if (h == 0)
			return;
		
		float c = sqrtf(2.0f);
		CCPoint u = ccp(m_alongVector.x / h, m_alongVector.y / h);
		
		// Compressed Interpolation mode
		//	if (m_bCompressedInterpolation)
		{
			float h2 = 1 / ( fabsf(u.x) + fabsf(u.y) );
			u = ccpMult(u, h2 * (float)c);
		}
		
		//	float opacityf = (float)_displayedOpacity / 255.0f;
		
		ccColor4F S = {
			m_startColor.r / 255.0f,
			m_startColor.g / 255.0f,
			m_startColor.b / 255.0f,
//			0.f
			m_startColor.a / 255.0f * _displayedOpacity / 255.f
		};
		
		ccColor4F E = {
			m_endColor.r / 255.0f,
			m_endColor.g / 255.0f,
			m_endColor.b / 255.0f,
//			0.f
			m_endColor.a / 255.0f * _displayedOpacity / 255.f
		};
		
		// (-1, -1)
		ccColor4F m_pSquareColors[4];
		m_pSquareColors[0].r = E.r + (S.r - E.r) * ((c + u.x + u.y) / (2.0f * c));
		m_pSquareColors[0].g = E.g + (S.g - E.g) * ((c + u.x + u.y) / (2.0f * c));
		m_pSquareColors[0].b = E.b + (S.b - E.b) * ((c + u.x + u.y) / (2.0f * c));
		m_pSquareColors[0].a = E.a + (S.a - E.a) * ((c + u.x + u.y) / (2.0f * c));
		// (1, -1)
		m_pSquareColors[1].r = E.r + (S.r - E.r) * ((c - u.x + u.y) / (2.0f * c));
		m_pSquareColors[1].g = E.g + (S.g - E.g) * ((c - u.x + u.y) / (2.0f * c));
		m_pSquareColors[1].b = E.b + (S.b - E.b) * ((c - u.x + u.y) / (2.0f * c));
		m_pSquareColors[1].a = E.a + (S.a - E.a) * ((c - u.x + u.y) / (2.0f * c));
		// (-1, 1)
		m_pSquareColors[2].r = E.r + (S.r - E.r) * ((c + u.x - u.y) / (2.0f * c));
		m_pSquareColors[2].g = E.g + (S.g - E.g) * ((c + u.x - u.y) / (2.0f * c));
		m_pSquareColors[2].b = E.b + (S.b - E.b) * ((c + u.x - u.y) / (2.0f * c));
		m_pSquareColors[2].a = E.a + (S.a - E.a) * ((c + u.x - u.y) / (2.0f * c));
		// (1, 1)
		m_pSquareColors[3].r = E.r + (S.r - E.r) * ((c - u.x - u.y) / (2.0f * c));
		m_pSquareColors[3].g = E.g + (S.g - E.g) * ((c - u.x - u.y) / (2.0f * c));
		m_pSquareColors[3].b = E.b + (S.b - E.b) * ((c - u.x - u.y) / (2.0f * c));
		m_pSquareColors[3].a = E.a + (S.a - E.a) * ((c - u.x - u.y) / (2.0f * c));
		
		
		
		m_sQuad.bl.colors = ccc4BFromccc4F(m_pSquareColors[0]);
		m_sQuad.br.colors = ccc4BFromccc4F(m_pSquareColors[1]);
		m_sQuad.tl.colors = ccc4BFromccc4F(m_pSquareColors[2]);
		m_sQuad.tr.colors = ccc4BFromccc4F(m_pSquareColors[3]);
		
		// renders using batch node
		if (m_pobBatchNode)
		{
			if (m_uAtlasIndex != CCSpriteIndexNotInitialized)
			{
				m_pobTextureAtlas->updateQuad(&m_sQuad, m_uAtlasIndex);
			}
			else
			{
				// no need to set it recursively
				// update dirty_, don't update recursiveDirty_
				setDirty(true);
			}
		}
	}
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
	
	bool validString = false;
	for(auto i : m_string)
	{
		if(i != ' ')
		{
			validString = true;
			setVisible(true);
			break;
		}
	}
	if(!validString)
	{
		setVisible(false);
		return true;
	}
	if(m_outerSprite)
	{
		m_outerSprite->removeFromParent();
		m_outerSprite = nullptr;
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
		
		float devider = (m_fFontSize - 10)*9.f / 10.f + 8.f;
		//float devider = 16;
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
	
//	// 그라데이션 처리.
//	if(m_gradationMode)
//	{
//		if(m_clippingNodeForGra)
//		{
//			m_clippingNodeForGra->removeFromParent();
//		}
//		CCClippingNode* cNode = CCClippingNode::create();
//		//		textureForGradient->getTexture()->setAliasTexParameters();
//		cNode->setStencil(textureForGradient);
//		cNode->setAlphaThreshold(0.1f);
//		CCLayerGradient* cclg = CCLayerGradient::create(m_startColor, m_endColor, m_alongVector);
//		cclg->setAnchorPoint(ccp(0.5f, 0.5f));
//		cclg->ignoreAnchorPointForPosition(false);
//		cNode->addChild(cclg);
//		cclg->setContentSize(textureForGradient->getContentSize());
//		addChild(cNode);
//		cNode->setPosition(ccp(getContentSize().width / 2.f, getContentSize().height / 2.f));
//		m_clippingNodeForGra = cNode;
//	}
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
	CCNodeRGBA::setOpacity(opacity);
	if(m_outerSprite)
	{
		m_outerSprite->setOpacity(opacity);
	}
	updateColor();
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
	// test!!!!
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

