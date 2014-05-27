#ifndef ASYNCSPRITE_H
#define ASYNCSPRITE_H

#include "cocos2d.h"
USING_NS_CC;

#include "GraySprite.h"
class AsyncSpriteManager
{
protected:
	std::vector<CCObject*> m_aliveObject;
public:
	static AsyncSpriteManager* getInstance()
	{
		static AsyncSpriteManager* t_GD = NULL;
		if(t_GD == NULL)
		{
			t_GD = new AsyncSpriteManager();
			//		t_GD->myInit();
		}
		return t_GD;
	}
	void registerObject(CCObject* obj)
	{
		m_aliveObject.push_back(obj);
	}
	void unregisterObject(CCObject* obj)
	{
		m_aliveObject.erase(std::find(m_aliveObject.begin(), m_aliveObject.end(), obj));
	}
	bool isAliveObject(CCObject* obj)
	{
		return std::find(m_aliveObject.begin(), m_aliveObject.end(), obj) != m_aliveObject.end();
	}
};
class AsyncSprite : public CCSprite
{
public:
	AsyncSprite();
	virtual ~AsyncSprite();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	static AsyncSprite* create()
	{
		AsyncSprite* t = new AsyncSprite();
		t->init();
		t->autorelease();
		return t;
	}
	virtual bool init(const char* fileName);
	static AsyncSprite* create(const char* fileName)
	{
		AsyncSprite* t = new AsyncSprite();
		t->init(fileName);
		t->autorelease();
		return t;
	}
	//virtual void registerWithTouchDispatcher();
	
	AsyncSprite* createWithTexture(CCTexture2D *pTexture)
	{
    AsyncSprite *pobSprite = new AsyncSprite();
    if (pobSprite && pobSprite->initWithTexture(pTexture))
    {
			pobSprite->autorelease();
			return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
	}
	bool initWithTexture(CCTexture2D *pTexture)
	{
    CCAssert(pTexture != NULL, "Invalid texture for sprite");
		
    CCRect rect = CCRectZero;
    rect.size = pTexture->getContentSize();
    
    return initWithTexture(pTexture, rect, false);
	}
	bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated)
	{
    if (CCNodeRGBA::init())
    {
			m_pobBatchNode = NULL;
			
			m_bRecursiveDirty = false;
			setDirty(false);
			
			m_bOpacityModifyRGB = true;
			
			m_sBlendFunc.src = CC_BLEND_SRC;
			m_sBlendFunc.dst = CC_BLEND_DST;
			
			m_bFlipX = m_bFlipY = false;
			
			// default transform anchor: center
			setAnchorPoint(ccp(0.5f, 0.5f));
			
			// zwoptex default values
			m_obOffsetPosition = CCPointZero;
			
			m_bHasChildren = false;
			
			// clean the Quad
			memset(&m_sQuad, 0, sizeof(m_sQuad));
			
			// Atlas: Color
			ccColor4B tmpColor = { 255, 255, 255, 255 };
			m_sQuad.bl.colors = tmpColor;
			m_sQuad.br.colors = tmpColor;
			m_sQuad.tl.colors = tmpColor;
			m_sQuad.tr.colors = tmpColor;
			
			// shader program
			setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
			
			// update texture (calls updateBlendFunc)
			setTexture(pTexture);
			setTextureRect(rect, rotated, rect.size);
			
			// by default use "Self Render".
			// if the sprite is added to a batchnode, then it will automatically switch to "batchnode Render"
			setBatchNode(NULL);
			
			return true;
    }
    else
    {
			return false;
    }
	}
	void loadImage(CCObject* parent);
	void loadedImage(CCObject* obj);
protected:
	std::string m_fileName;
	CCObject* m_parent;
};

#endif
