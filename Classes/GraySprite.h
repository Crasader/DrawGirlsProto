//
//  GraySprite.h
//  exGraySprite
//
//  Created by 이정우 on 13. 8. 29..
//
//

#ifndef __exGraySprite__GraySprite__
#define __exGraySprite__GraySprite__


#include "cocoa/CCGeometry.h"
#include "cocos2d.h"
USING_NS_CC;

class GraySprite : public CCSprite{
    
    
public:
    GraySprite();
    virtual ~GraySprite();
	static GraySprite* create();
    static GraySprite* create(const char* pszFileName);
	static GraySprite* createWithTexture(CCTexture2D* pTexture);
	static GraySprite* createWithTexture(CCTexture2D* pTexture, const CCRect& tRect);
	bool initWithTexture(CCTexture2D* pTexture);
    bool initWithTexture(CCTexture2D* pTexture, const CCRect& tRect);
    void setGray (bool isGray);
	void setDeepGray(bool isGray);
    virtual void draw();
};

#endif /* defined(__exGraySprite__GraySprite__) */
