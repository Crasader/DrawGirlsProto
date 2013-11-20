/**
 @file      ShadowSprite.cpp
 @date      13.09.06
 @author    Kwon Oh-Jung (coz)
 @version   1.0 (x-2.1.4)
 @desc.     쉐도우 스프라이트 구현파일 입니다.
            blog : http://cozycoz.egloos.com
            email : coz.architect@gmail.com
 */

#include "ShadowSprite.h"

ShadowSprite::ShadowSprite()
{
}

ShadowSprite* ShadowSprite::create( const std::string& fn )
{
	ShadowSprite *pRet = new ShadowSprite();

	if(pRet && pRet->init(fn) )
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}

	return pRet;
}

void ShadowSprite::setPosition( const CCPoint& pt )
{
	CCSprite::setPosition(pt);
	KS::KSLog("%", m_fileName);
	const char* tt = m_fileName.c_str();
	KS::KSLog("%", tt);
	CCSprite *pShadow = CCSprite::create(tt);
	//    pShadow->setPosition( pt );
	this->getParent()->addChild(pShadow);

	pShadow->runAction(CCSequence::create(
		CCFadeOut::create(0.3f),
		CCCallFuncN::create(this, callfuncN_selector(ShadowSprite::removeFromParentAndCleanup)),
		NULL));
}

bool ShadowSprite::init( const std::string& fn )
{
	if( !CCSprite::initWithFile(fn.c_str()) )
		return false;

	m_fileName = fn;
	return true;
}





