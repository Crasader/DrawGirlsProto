//
//  GraySprite.cpp
//  exGraySprite
//
//  Created by 이정우 on 13. 8. 29..
//
//

#include "GraySprite.h"

GraySprite::GraySprite(){
    
    
}


GraySprite::~GraySprite(){
    
    
}

GraySprite* GraySprite::create()
{
	GraySprite *pSprite = new GraySprite();
    if (pSprite && pSprite->init())
    {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

GraySprite* GraySprite::create( const char* pszFileName ){
    GraySprite* graySprite = new GraySprite;
    if (graySprite && graySprite->initWithFile(pszFileName)){
        graySprite->autorelease();
        return graySprite;
    }else{
        CC_SAFE_RELEASE(graySprite);
        return NULL;
    }
}

GraySprite* GraySprite::createWithTexture(CCTexture2D* pTexture)
{
    GraySprite* graySprite = new GraySprite;
    if (graySprite && graySprite->cocos2d::CCSprite::initWithTexture(pTexture)){
        graySprite->autorelease();
        return graySprite;
    }else{
        CC_SAFE_RELEASE(graySprite);
        return NULL;
    }
}

GraySprite* GraySprite::createWithTexture(CCTexture2D* pTexture, const CCRect& tRect)
{
    GraySprite* graySprite = new GraySprite;
    if (graySprite && graySprite->cocos2d::CCSprite::initWithTexture(pTexture, tRect)){
        graySprite->autorelease();
        return graySprite;
    }else{
        CC_SAFE_RELEASE(graySprite);
        return NULL;
    }
}

bool GraySprite::initWithTexture(CCTexture2D* pTexture, const CCRect& tRect ){
    do{
        CC_BREAK_IF(!CCSprite::initWithTexture(pTexture, tRect));

        setGray(false);
        return true;
    } while (0);
    return false;
}

bool GraySprite::initWithTexture(CCTexture2D* pTexture)
{
	do{
		CC_BREAK_IF(!CCSprite::initWithTexture(pTexture));
		
		setGray(false);
		return true;
	} while (0);
	return false;
}

void GraySprite::setGray (bool isGray) {
    if(isGray) {
        GLchar* pszFragSource =
        "#ifdef GL_ES \n \
        precision mediump float; \n \
        #endif \n \
        uniform sampler2D u_texture; \n \
        varying vec2 v_texCoord; \n \
        varying vec4 v_fragmentColor; \n \
        void main(void) \n \
        { \n \
        // Convert to greyscale using NTSC weightings \n \
        vec4 color = texture2D(u_texture, v_texCoord).rgba;\n \
        float grey = dot(color.rgb, vec3(0.299, 0.587, 0.114)); \n \
        gl_FragColor = vec4(grey, grey, grey, color.a); \n \
        }";
        
        CCGLProgram* pProgram = new CCGLProgram();
        pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
        this->setShaderProgram(pProgram);
        pProgram->release();
        CHECK_GL_ERROR_DEBUG();

    } else {
        CCGLProgram* pProgram = new CCGLProgram();
        pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, ccPositionTextureColor_frag);
        this->setShaderProgram(pProgram);
        pProgram->release();
        CHECK_GL_ERROR_DEBUG();
    }
    
    this->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    this->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    this->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    CHECK_GL_ERROR_DEBUG();
    
    
    this->getShaderProgram()->link();
    CHECK_GL_ERROR_DEBUG();
    
    
    this->getShaderProgram()->updateUniforms();
    CHECK_GL_ERROR_DEBUG();

}

void GraySprite::setDeepGray (bool isGray) {
    if(isGray) {
        GLchar* pszFragSource =
        "#ifdef GL_ES \n \
        precision mediump float; \n \
        #endif \n \
        uniform sampler2D u_texture; \n \
        varying vec2 v_texCoord; \n \
        varying vec4 v_fragmentColor; \n \
        void main(void) \n \
        { \n \
        // Convert to greyscale using NTSC weightings \n \
        vec4 color = texture2D(u_texture, v_texCoord).rgba;\n \
        float grey = dot(color.rgb, vec3(0.180, 0.350, 0.100)); \n \
        gl_FragColor = vec4(grey, grey, grey, color.a); \n \
        }";
        
        CCGLProgram* pProgram = new CCGLProgram();
        pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
        this->setShaderProgram(pProgram);
        pProgram->release();
        CHECK_GL_ERROR_DEBUG();
		
    } else {
        CCGLProgram* pProgram = new CCGLProgram();
        pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, ccPositionTextureColor_frag);
        this->setShaderProgram(pProgram);
        pProgram->release();
        CHECK_GL_ERROR_DEBUG();
    }
    
    this->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    this->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    this->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    CHECK_GL_ERROR_DEBUG();
    
    
    this->getShaderProgram()->link();
    CHECK_GL_ERROR_DEBUG();
    
    
    this->getShaderProgram()->updateUniforms();
    CHECK_GL_ERROR_DEBUG();
	
}

void GraySprite::draw(){
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
    ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
    
    
    this->getShaderProgram()->use();
    this->getShaderProgram()->setUniformsForBuiltins();
    
    ccGLBindTexture2D( this->getTexture()->getName() );
    
    
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
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    CC_INCREMENT_GL_DRAWS(1);
}