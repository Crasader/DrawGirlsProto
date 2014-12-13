//
//  EffectSprite.h
//  DGproto
//
//  Created by ksoo k on 2014. 3. 24..
//
//

#ifndef __DGproto__EffectSprite__
#define __DGproto__EffectSprite__

#include "cocos2d.h"

USING_NS_CC;

static const GLchar * pixelationVert =
#include "PixelationVert.h"

static const GLchar * pixelationFrag =
#include "PixelationFrag.h"

static const GLchar* blurVert =
#include "BlurVert.h"

static const GLchar* blurFrag =
#include "BlurFrag.h"

static const GLchar* brightenFrag =
#include "BrightenFrag.h"

static const GLchar* silCvtFrag =
#include "SilhouetteConvertFrag.h"

static const GLchar* colorSilFrag =
#include "ColorSilhouette.h"

static const GLchar* colorOpaSilFrag =
#include "ColorOpacitySilhouette.h"

static const GLchar * grayVert =
#include "GrayVert.h"

static const GLchar * grayFrag =
#include "GrayFrag.h"

static const GLchar * grayPixelationVert =
#include "GrayPixelationVert.h"

static const GLchar * grayPixelationFrag =
#include "GrayPixelationFrag.h"

enum class CurrentMode
{
	kNone = 0,
	kBlur,
	kPixelation,
	kBrighten,
	kSilCvt,
	kGray,
	kGrayPixelation,
	kGrayBlur,
	kColorSil,
	kColorOpaSil
};
class EffectSprite : public CCSprite
{
public:
	EffectSprite()
	{
		m_currentMode = CurrentMode::kNone;
	}
	static EffectSprite* createWithTexture(CCTexture2D *pTexture)
	{
    EffectSprite *pobSprite = new EffectSprite();
    if (pobSprite && pobSprite->initWithTexture(pTexture))
    {
			pobSprite->autorelease();
			return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
	}
//	bool initWithTexture(CCTexture2D *pTexture)
//	{
//    CCAssert(pTexture != NULL, "Invalid texture for sprite");
//		
//    CCRect rect = CCRectZero;
//    rect.size = pTexture->getContentSize();
//    
//    return initWithTexture(pTexture, rect);
//	}
	virtual ~EffectSprite()
	{
		
	}
//	static EffectSprite* create()
//	{
//		EffectSprite* sp = new EffectSprite();
//		sp->init();
//		return sp;
//	}
	static EffectSprite* create(const std::string& str)
	{
		EffectSprite* sp = new EffectSprite();
		sp->initWithFile(str.c_str());
		sp->autorelease();
		
		return sp;
	}
	void setBlur()
	{
		if(m_currentMode != CurrentMode::kBlur)
		{
			CCGLProgram* pProgram = new CCGLProgram();
			pProgram->initWithVertexShaderByteArray(blurVert, blurFrag);
			setShaderProgram(pProgram);
			pProgram->release();
			
			//		pProgram->setUniformLocationWith1f(glGetUniformLocation( pProgram->getProgram(), "u_blurSize"),
			//																		 3);
			m_currentMode = CurrentMode::kBlur;
			afterEffect(pProgram);
		}
		else
		{
			getShaderProgram()->use();
		}
	}
	void setBrighten(float b = 1.2f)
	{
		if(m_currentMode != CurrentMode::kBrighten)
		{
			CCGLProgram* pProgram = new CCGLProgram();
			pProgram->initWithVertexShaderByteArray(blurVert, brightenFrag);
			setShaderProgram(pProgram);
			pProgram->release();
			
			//		pProgram->setUniformLocationWith1f(glGetUniformLocation( pProgram->getProgram(), "u_blurSize"),
			//																		 3);
			m_currentMode = CurrentMode::kBrighten;
			
			afterEffect(pProgram);
		}
		else
		{
			getShaderProgram()->use();
		}
		getShaderProgram()->setUniformLocationWith1f
						(glGetUniformLocation(getShaderProgram()->getProgram(), "u_brighten"),
																		 b);
	}
	void setPixelation(float pixelationSize = 30.f)
	{
		if(m_currentMode != CurrentMode::kPixelation)
		{
			CCGLProgram* pProgram = new CCGLProgram();
			pProgram->initWithVertexShaderByteArray(pixelationVert, pixelationFrag);
			setShaderProgram(pProgram);
			pProgram->release();
			
			m_currentMode = CurrentMode::kPixelation;
			afterEffect(pProgram);
		}
		else
		{
			getShaderProgram()->use();
		}
		
		getShaderProgram()->setUniformLocationWith1f
						(glGetUniformLocation(getShaderProgram()->getProgram(), "u_pixelationSize"),
																		 pixelationSize);
	}
	void setGray()
	{
		if(m_currentMode != CurrentMode::kGray)
		{
			CCGLProgram* pProgram = new CCGLProgram();
			pProgram->initWithVertexShaderByteArray(grayVert, grayFrag);
			setShaderProgram(pProgram);
			pProgram->release();
			
			m_currentMode = CurrentMode::kGray;
			afterEffect(pProgram);
		}
		else
		{
			getShaderProgram()->use();
		}
	}
	void setGrayPixelation(float pixelationSize = 30.f)
	{
		if(m_currentMode != CurrentMode::kGrayPixelation)
		{
			CCGLProgram* pProgram = new CCGLProgram();
			pProgram->initWithVertexShaderByteArray(grayPixelationVert, grayPixelationFrag);
			setShaderProgram(pProgram);
			pProgram->release();
			
			m_currentMode = CurrentMode::kGrayPixelation;
			afterEffect(pProgram);
		}
		else
		{
			getShaderProgram()->use();
		}
		
		getShaderProgram()->setUniformLocationWith1f
						(glGetUniformLocation(getShaderProgram()->getProgram(), "u_pixelationSize"),
																		 pixelationSize);
	}
	
	void setSilhouetteConvert(int t_type)
	{
		if(m_currentMode != CurrentMode::kSilCvt)
		{
			CCGLProgram* pProgram = new CCGLProgram();
			pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, silCvtFrag);
			setShaderProgram(pProgram);
			pProgram->release();
			m_currentMode = CurrentMode::kSilCvt;
			afterEffect(pProgram);
		}
		else
		{
			getShaderProgram()->use();
		}
		
		getShaderProgram()->setUniformLocationWith1i(glGetUniformLocation(getShaderProgram()->getProgram(), "u_type"), t_type);
	}
	
	void setColorSilhouette(int t_r, int t_g, int t_b)
	{
		if(m_currentMode != CurrentMode::kColorSil)
		{
			CCGLProgram* pProgram = new CCGLProgram();
			pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, colorSilFrag);
			setShaderProgram(pProgram);
			pProgram->release();
			m_currentMode = CurrentMode::kColorSil;
			afterEffect(pProgram);
		}
		else
		{
			getShaderProgram()->use();
		}
		
		GLint color3Index = getShaderProgram()->getUniformLocationForName("color3");
		getShaderProgram()->setUniformLocationWith3f(color3Index, t_r/255.f, t_g/255.f, t_b/255.f);
	}
	
	void setColorOpacitySilhouette(int t_r, int t_g, int t_b, int t_o)
	{
		if(m_currentMode != CurrentMode::kColorOpaSil)
		{
			CCGLProgram* pProgram = new CCGLProgram();
			pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, colorOpaSilFrag);
			setShaderProgram(pProgram);
			pProgram->release();
			m_currentMode = CurrentMode::kColorOpaSil;
			afterEffect(pProgram);
		}
		else
		{
			getShaderProgram()->use();
		}
		
		GLint color4Index = getShaderProgram()->getUniformLocationForName("color4");
		getShaderProgram()->setUniformLocationWith4f(color4Index, t_r/255.f, t_g/255.f, t_b/255.f, t_o/255.f);
	}
	
	void setNonEffect()
	{
		if(m_currentMode != CurrentMode::kNone)
		{
			setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
			m_currentMode = CurrentMode::kNone;
		}
		
	}
	void afterEffect(CCGLProgram* shader)
	{
		shader->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		shader->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    shader->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    shader->link();
    shader->updateUniforms();
		shader->setUniformLocationWith2f(glGetUniformLocation( shader->getProgram(), "u_imageSize"),
																								 getTexture()->getContentSizeInPixels().width,
																								 getTexture()->getContentSizeInPixels().height);
	}
//	bool init(const std::string& str)
//	{
//		CCSprite::initWithFile(str.c_str());
//		return true;
//	}
	/*
	 p->initWithVertexShaderByteArray(ccPositionTexture_vert ,ccPositionTexture_frag);
	 
	 p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	 p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

	 */
	void draw()
	{
		if(m_currentMode == CurrentMode::kBlur)
		{
			CCPoint blur_ = ccp(4/getTexture()->getContentSizeInPixels().width,
													4/getTexture()->getContentSizeInPixels().height);
			ccGLEnable(m_eGLServerState);
			CHECK_GL_ERROR_DEBUG();
			getShaderProgram()->use();
			CHECK_GL_ERROR_DEBUG();
			getShaderProgram()->setUniformsForBuiltins();
CHECK_GL_ERROR_DEBUG();
			
			ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
			CHECK_GL_ERROR_DEBUG();
			ccGLBindTexture2D( m_pobTexture->getName() );
			CHECK_GL_ERROR_DEBUG();
			ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
			CHECK_GL_ERROR_DEBUG();
			getShaderProgram()->setUniformLocationWith2f(glGetUniformLocation( getShaderProgram()->getProgram(), "u_blurSize"), blur_.x, 0);
//			ccGLBindTexture2D( getTexture()->getName());
			CHECK_GL_ERROR_DEBUG();
			
			
			//
			// Attributes
			//
#define kQuadSize sizeof(m_sQuad.bl)
			long offset = (long)&m_sQuad;
			
			// vertex
			int diff = offsetof( ccV3F_C4B_T2F, vertices);
			glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
			
			// texCoods
			diff = offsetof( ccV3F_C4B_T2F, texCoords);
			glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
			
			// color
			diff = offsetof( ccV3F_C4B_T2F, colors);
			glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
			CHECK_GL_ERROR_DEBUG();
			
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			CHECK_GL_ERROR_DEBUG();
			CC_INCREMENT_GL_DRAWS(1);
			
			getShaderProgram()->setUniformLocationWith2f(glGetUniformLocation( getShaderProgram()->getProgram(), "u_blurSize"), 0, blur_.y);
			ccGLBindTexture2D( getTexture()->getName());
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			
		}
		else
		{
			CCSprite::draw();
		}
	}
	
protected:
	CurrentMode m_currentMode;
	
};

#endif /* defined(__DGproto__EffectSprite__) */
