/**
 @file      ShadowSprite.h
 @date      13.09.06
 @author    Kwon Oh-Jung (coz)
 @version   1.0 (x-2.1.4)
 @desc.     쉐도우 스프라이트 헤더파일 입니다.
 blog : http://cozycoz.egloos.com
 email : coz.architect@gmail.com
 */

#ifndef ShadowSprite_h
#define ShadowSprite_h

#include "cocos2d.h"
#include "KSUtil.h"

USING_NS_CC;

class ShadowSprite : public CCSprite
{
public:
	ShadowSprite();
	virtual ~ShadowSprite(){}
	
	static ShadowSprite* create(const std::string& fn);
	void setPosition(const CCPoint& pt);
	
	virtual bool init(const std::string& fn);
protected:
	std::string m_fileName;
};

#endif
