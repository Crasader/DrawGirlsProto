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

static const GLchar * ksVert =
#include "PixelationVert.h"

static const GLchar * ksFrag =
#include "PixelationFrag.h"

static const GLchar* blurVert =
#include "BlurVert.h"

static const GLchar* blurFrag =
#include "BlurFrag.h"

static const GLchar* brightenFrag =
#include "BrightenFrag.h"

enum class CurrentMode
{
	kNone = 0,
	kBlur,
	kPixelation,
	kBrighten
};
class EffectSprite : public CCSprite
{
public:
	EffectSprite()
	{
		m_currentMode = CurrentMode::kNone;
	}
	
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
		
		return sp;
	}
	void setBlur()
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
	void setBrighten()
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
	void setPixelation()
	{
		CCGLProgram* pProgram = new CCGLProgram();
    pProgram->initWithVertexShaderByteArray(ksVert, ksFrag);
    setShaderProgram(pProgram);
    pProgram->release();
		
		m_currentMode = CurrentMode::kPixelation;
    afterEffect(pProgram);
		
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
