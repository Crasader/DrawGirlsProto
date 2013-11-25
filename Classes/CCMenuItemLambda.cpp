#include "CCMenuItemLambda.h"

static unsigned int _globalFontSize = kCCItemSize;
static std::string _globalFontName = "Marker Felt";
static bool _globalFontNameRelease = false;

const unsigned int    kCurrentItem = 0xc0c05001;
const unsigned int    kZoomActionTag = 0xc0c05002;

const unsigned int    kNormalTag = 0x1;
const unsigned int    kSelectedTag = 0x2;
const unsigned int    kDisableTag = 0x3;
//
// CCMenuItemLambda
//

CCMenuItemLambda* CCMenuItemLambda::create()
{
	return CCMenuItemLambda::create([](CCObject*){});
}

CCMenuItemLambda* CCMenuItemLambda::create(std::function<void(CCObject*)> selector)
{
	CCMenuItemLambda *pRet = new CCMenuItemLambda();
	pRet->initWithTarget(selector);
	pRet->autorelease();
	return pRet;
}

bool CCMenuItemLambda::initWithTarget(std::function<void(CCObject*)> selector)
{
	setAnchorPoint(ccp(0.5f, 0.5f));
	m_selector = selector;
	m_bEnabled = true;
	m_bSelected = false;
	return true;
}

CCMenuItemLambda::~CCMenuItemLambda()
{
	unregisterScriptTapHandler();
}

void CCMenuItemLambda::selected()
{
	m_bSelected = true;
}

void CCMenuItemLambda::unselected()
{
	m_bSelected = false;
}

void CCMenuItemLambda::registerScriptTapHandler(int nHandler)
{
	unregisterScriptTapHandler();
	m_nScriptTapHandler = nHandler;
	LUALOG("[LUA] Add CCMenuItemLambda script handler: %d", m_nScriptTapHandler);
}

void CCMenuItemLambda::unregisterScriptTapHandler(void)
{
	if (m_nScriptTapHandler)
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nScriptTapHandler);
		LUALOG("[LUA] Remove CCMenuItemLambda script handler: %d", m_nScriptTapHandler);
		m_nScriptTapHandler = 0;
	}
}

void CCMenuItemLambda::activate()
{
	if (m_bEnabled)
	{
		m_selector(this);
		
		if(m_afterSelector != nullptr)
			m_afterSelector(this);
//		if (kScriptTypeNone != m_eScriptType)
//		{
//			CCScriptEngineManager::sharedManager()->getScriptEngine()->executeMenuItemEvent(this);
//		}
	}
}

void CCMenuItemLambda::setEnabled(bool enabled)
{
	m_bEnabled = enabled;
}

bool CCMenuItemLambda::isEnabled()
{
	return m_bEnabled;
}

CCRect CCMenuItemLambda::rect()
{
	return CCRectMake( m_obPosition.x - m_obContentSize.width * m_obAnchorPoint.x,
										m_obPosition.y - m_obContentSize.height * m_obAnchorPoint.y,
										m_obContentSize.width, m_obContentSize.height);
}

bool CCMenuItemLambda::isSelected()
{
	return m_bSelected;
}

void CCMenuItemLambda::setTarget(std::function<void(CCObject*)> selector)
{
	m_selector = selector;
}

//
//CCMenuItemLabelLambda
//

const ccColor3B& CCMenuItemLabelLambda::getDisabledColor()
{
	return m_tDisabledColor;
}
void CCMenuItemLabelLambda::setDisabledColor(const ccColor3B& var)
{
	m_tDisabledColor = var;
}
CCNode *CCMenuItemLabelLambda::getLabel()
{
	return m_pLabel;
}
void CCMenuItemLabelLambda::setLabel(CCNode* var)
{
	if (var)
	{
		addChild(var);
		var->setAnchorPoint(ccp(0, 0));
		setContentSize(var->getContentSize());
	}
	
	if (m_pLabel)
	{
		removeChild(m_pLabel, true);
	}
	
	m_pLabel = var;
}

CCMenuItemLabelLambda * CCMenuItemLabelLambda::create(CCNode*label, std::function<void(CCObject*)> selector)
{
	CCMenuItemLabelLambda *pRet = new CCMenuItemLabelLambda();
	pRet->initWithLabel(label, selector);
	pRet->autorelease();
	return pRet;
}

CCMenuItemLabelLambda* CCMenuItemLabelLambda::create(CCNode *label)
{
	CCMenuItemLabelLambda *pRet = new CCMenuItemLabelLambda();
	pRet->initWithLabel(label, [](CCObject*){});
	pRet->autorelease();
	return pRet;
}

bool CCMenuItemLabelLambda::initWithLabel(CCNode* label, std::function<void(CCObject*)> selector)
{
	CCMenuItemLambda::initWithTarget( selector);
	m_fOriginalScale = 1.0f;
	m_tColorBackup = ccWHITE;
	setDisabledColor(ccc3(126,126,126));
	this->setLabel(label);
	
	setCascadeColorEnabled(true);
	setCascadeOpacityEnabled(true);
	
	return true;
}

CCMenuItemLabelLambda::~CCMenuItemLabelLambda()
{
}

void CCMenuItemLabelLambda::setString(const char * label)
{
	dynamic_cast<CCLabelProtocol*>(m_pLabel)->setString(label);
	this->setContentSize(m_pLabel->getContentSize());
}

void CCMenuItemLabelLambda::activate()
{
	if(m_bEnabled)
	{
		this->stopAllActions();
		this->setScale( m_fOriginalScale );
		CCMenuItemLambda::activate();
	}
}

void CCMenuItemLabelLambda::selected()
{
	// subclass to change the default action
	if(m_bEnabled)
	{
		CCMenuItemLambda::selected();
		
		CCAction *action = getActionByTag(kZoomActionTag);
		if (action)
		{
			this->stopAction(action);
		}
		else
		{
			m_fOriginalScale = this->getScale();
		}
		
		CCAction *zoomAction = CCScaleTo::create(0.1f, m_fOriginalScale * 1.2f);
		zoomAction->setTag(kZoomActionTag);
		this->runAction(zoomAction);
	}
}

void CCMenuItemLabelLambda::unselected()
{
	// subclass to change the default action
	if(m_bEnabled)
	{
		CCMenuItemLambda::unselected();
		this->stopActionByTag(kZoomActionTag);
		CCAction *zoomAction = CCScaleTo::create(0.1f, m_fOriginalScale);
		zoomAction->setTag(kZoomActionTag);
		this->runAction(zoomAction);
	}
}

void CCMenuItemLabelLambda::setEnabled(bool enabled)
{
	if( m_bEnabled != enabled )
	{
		if(enabled == false)
		{
			m_tColorBackup = dynamic_cast<CCRGBAProtocol*>(m_pLabel)->getColor();
			dynamic_cast<CCRGBAProtocol*>(m_pLabel)->setColor(m_tDisabledColor);
		}
		else
		{
			dynamic_cast<CCRGBAProtocol*>(m_pLabel)->setColor(m_tColorBackup);
		}
	}
	CCMenuItemLambda::setEnabled(enabled);
}

//
//CCMenuItemAtlasFontLambda
//

CCMenuItemAtlasFontLambda * CCMenuItemAtlasFontLambda::create(const char *value, const char *charMapFile, int itemWidth, int itemHeight, char startCharMap)
{
	return CCMenuItemAtlasFontLambda::create(value, charMapFile, itemWidth, itemHeight, startCharMap, [](CCObject*){});
}

CCMenuItemAtlasFontLambda * CCMenuItemAtlasFontLambda::create(const char *value, const char *charMapFile, int itemWidth, int itemHeight, char startCharMap, std::function<void(CCObject*)> selector)
{
	CCMenuItemAtlasFontLambda *pRet = new CCMenuItemAtlasFontLambda();
	pRet->initWithString(value, charMapFile, itemWidth, itemHeight, startCharMap,  selector);
	pRet->autorelease();
	return pRet;
}

bool CCMenuItemAtlasFontLambda::initWithString(const char *value, const char *charMapFile, int itemWidth, int itemHeight, char startCharMap, std::function<void(CCObject*)> selector)
{
	CCAssert( value != NULL && strlen(value) != 0, "value length must be greater than 0");
	CCLabelAtlas *label = new CCLabelAtlas();
	label->initWithString(value, charMapFile, itemWidth, itemHeight, startCharMap);
	label->autorelease();
	if (CCMenuItemLabelLambda::initWithLabel(label,  selector))
	{
		// do something ?
	}
	return true;
}
//
//CCMenuItemFontLambda
//

void CCMenuItemFontLambda::setFontSize(unsigned int s)
{
	_globalFontSize = s;
}

unsigned int CCMenuItemFontLambda::fontSize()
{
	return _globalFontSize;
}

void CCMenuItemFontLambda::setFontName(const char *name)
{
	if (_globalFontNameRelease)
	{
		_globalFontName.clear();
	}
	_globalFontName = name;
	_globalFontNameRelease = true;
}

const char * CCMenuItemFontLambda::fontName()
{
	return _globalFontName.c_str();
}

CCMenuItemFontLambda * CCMenuItemFontLambda::create(const char *value, std::function<void(CCObject*)> selector)
{
	CCMenuItemFontLambda *pRet = new CCMenuItemFontLambda();
	pRet->initWithString(value,  selector);
	pRet->autorelease();
	return pRet;
}

CCMenuItemFontLambda * CCMenuItemFontLambda::create(const char *value)
{
	CCMenuItemFontLambda *pRet = new CCMenuItemFontLambda();
	pRet->initWithString(value, [](CCObject*){});
	pRet->autorelease();
	return pRet;
}

bool CCMenuItemFontLambda::initWithString(const char *value, std::function<void(CCObject*)> selector)
{
	CCAssert( value != NULL && strlen(value) != 0, "Value length must be greater than 0");
	
	m_strFontName = _globalFontName;
	m_uFontSize = _globalFontSize;
	
	CCLabelTTF *label = CCLabelTTF::create(value, m_strFontName.c_str(), (float)m_uFontSize);
	if (CCMenuItemLabelLambda::initWithLabel(label,  selector))
	{
		// do something ?
	}
	return true;
}

void CCMenuItemFontLambda::recreateLabel()
{
	CCLabelTTF *label = CCLabelTTF::create(dynamic_cast<CCLabelProtocol*>(m_pLabel)->getString(),
																				 m_strFontName.c_str(), (float)m_uFontSize);
	this->setLabel(label);
}

void CCMenuItemFontLambda::setFontSizeObj(unsigned int s)
{
	m_uFontSize = s;
	recreateLabel();
}

unsigned int CCMenuItemFontLambda::fontSizeObj()
{
	return m_uFontSize;
}

void CCMenuItemFontLambda::setFontNameObj(const char* name)
{
	m_strFontName = name;
	recreateLabel();
}

const char* CCMenuItemFontLambda::fontNameObj()
{
	return m_strFontName.c_str();
}

//
//CCMenuItemSpriteLambda
//

CCNode * CCMenuItemSpriteLambda::getNormalImage()
{
	return m_pNormalImage;
}

void CCMenuItemSpriteLambda::setNormalImage(CCNode* pImage)
{
	if (pImage != m_pNormalImage)
	{
		if (pImage)
		{
			addChild(pImage, 0, kNormalTag);
			pImage->setAnchorPoint(ccp(0, 0));
		}
		
		if (m_pNormalImage)
		{
			removeChild(m_pNormalImage, true);
		}
		
		m_pNormalImage = pImage;
		this->setContentSize(m_pNormalImage->getContentSize());
		this->updateImagesVisibility();
	}
}

CCNode * CCMenuItemSpriteLambda::getSelectedImage()
{
	return m_pSelectedImage;
}

void CCMenuItemSpriteLambda::setSelectedImage(CCNode* pImage)
{
	if (pImage != m_pNormalImage)
	{
		if (pImage)
		{
			addChild(pImage, 0, kSelectedTag);
			pImage->setAnchorPoint(ccp(0, 0));
		}
		
		if (m_pSelectedImage)
		{
			removeChild(m_pSelectedImage, true);
		}
		
		m_pSelectedImage = pImage;
		this->updateImagesVisibility();
	}
}

CCNode * CCMenuItemSpriteLambda::getDisabledImage()
{
	return m_pDisabledImage;
}

void CCMenuItemSpriteLambda::setDisabledImage(CCNode* pImage)
{
	if (pImage != m_pNormalImage)
	{
		if (pImage)
		{
			addChild(pImage, 0, kDisableTag);
			pImage->setAnchorPoint(ccp(0, 0));
		}
		
		if (m_pDisabledImage)
		{
			removeChild(m_pDisabledImage, true);
		}
		
		m_pDisabledImage = pImage;
		this->updateImagesVisibility();
	}
}

//
//CCMenuItemSpriteLambda
//

CCMenuItemSpriteLambda * CCMenuItemSpriteLambda::create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite)
{
	return CCMenuItemSpriteLambda::create(normalSprite, selectedSprite, disabledSprite, [](CCObject*){});
}

CCMenuItemSpriteLambda * CCMenuItemSpriteLambda::create(CCNode* normalSprite, CCNode* selectedSprite, std::function<void(CCObject*)> selector)
{
	return CCMenuItemSpriteLambda::create(normalSprite, selectedSprite, NULL, selector);
}

CCMenuItemSpriteLambda * CCMenuItemSpriteLambda::create(CCNode *normalSprite, CCNode *selectedSprite, CCNode *disabledSprite, std::function<void(CCObject*)> selector)
{
	CCMenuItemSpriteLambda *pRet = new CCMenuItemSpriteLambda();
	pRet->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, selector);
	pRet->autorelease();
	return pRet;
}

bool CCMenuItemSpriteLambda::initWithNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, std::function<void(CCObject*)> selector)
{
	CCMenuItemLambda::initWithTarget( selector);
	setNormalImage(normalSprite);
	setSelectedImage(selectedSprite);
	setDisabledImage(disabledSprite);
	
	if(m_pNormalImage)
	{
		this->setContentSize(m_pNormalImage->getContentSize());
	}
	
	setCascadeColorEnabled(true);
	setCascadeOpacityEnabled(true);
	
	return true;
}

/**
 @since v0.99.5
 */
void CCMenuItemSpriteLambda::selected()
{
	CCMenuItemLambda::selected();
	
	if (m_pNormalImage)
	{
		if (m_pDisabledImage)
		{
			m_pDisabledImage->setVisible(false);
		}
		
		if (m_pSelectedImage)
		{
			m_pNormalImage->setVisible(false);
			m_pSelectedImage->setVisible(true);
		}
		else
		{
			m_pNormalImage->setVisible(true);
		}
	}
}

void CCMenuItemSpriteLambda::unselected()
{
	CCMenuItemLambda::unselected();
	if (m_pNormalImage)
	{
		m_pNormalImage->setVisible(true);
		
		if (m_pSelectedImage)
		{
			m_pSelectedImage->setVisible(false);
		}
		
		if (m_pDisabledImage)
		{
			m_pDisabledImage->setVisible(false);
		}
	}
}

void CCMenuItemSpriteLambda::setEnabled(bool bEnabled)
{
	if( m_bEnabled != bEnabled )
	{
		CCMenuItemLambda::setEnabled(bEnabled);
		this->updateImagesVisibility();
	}
}

// Helper
void CCMenuItemSpriteLambda::updateImagesVisibility()
{
	if (m_bEnabled)
	{
		if (m_pNormalImage)   m_pNormalImage->setVisible(true);
		if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
		if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
	}
	else
	{
		if (m_pDisabledImage)
		{
			if (m_pNormalImage)   m_pNormalImage->setVisible(false);
			if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
			if (m_pDisabledImage) m_pDisabledImage->setVisible(true);
		}
		else
		{
			if (m_pNormalImage)   m_pNormalImage->setVisible(true);
			if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
			if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
		}
	}
}

///
/// CCMenuItemImageLambda
///

CCMenuItemImageLambda* CCMenuItemImageLambda::create()
{
	CCMenuItemImageLambda *pRet = new CCMenuItemImageLambda();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool CCMenuItemImageLambda::init(void)
{
	return initWithNormalImage(NULL, NULL, NULL, [](CCObject*){});
}

CCMenuItemImageLambda * CCMenuItemImageLambda::create(const char *normalImage, const char *selectedImage)
{
	return CCMenuItemImageLambda::create(normalImage, selectedImage, NULL, [](CCObject*){});
}

CCMenuItemImageLambda * CCMenuItemImageLambda::create(const char *normalImage, const char *selectedImage, std::function<void(CCObject*)> selector)
{
	return CCMenuItemImageLambda::create(normalImage, selectedImage, NULL,  selector);
}

CCMenuItemImageLambda * CCMenuItemImageLambda::create(const char *normalImage, const char *selectedImage, const char *disabledImage, std::function<void(CCObject*)> selector)
{
	CCMenuItemImageLambda *pRet = new CCMenuItemImageLambda();
	if (pRet && pRet->initWithNormalImage(normalImage, selectedImage, disabledImage,  selector))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

CCMenuItemImageLambda * CCMenuItemImageLambda::create(const char *normalImage, const char *selectedImage, const char *disabledImage)
{
	CCMenuItemImageLambda *pRet = new CCMenuItemImageLambda();
	if (pRet && pRet->initWithNormalImage(normalImage, selectedImage, disabledImage, [](CCObject*){}))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool CCMenuItemImageLambda::initWithNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage, std::function<void(CCObject*)> selector)
{
	CCNode *normalSprite = NULL;
	CCNode *selectedSprite = NULL;
	CCNode *disabledSprite = NULL;
	
	if (normalImage)
	{
		normalSprite = CCSprite::create(normalImage);
	}
	
	if (selectedImage)
	{
		selectedSprite = CCSprite::create(selectedImage);
	}
	
	if(disabledImage)
	{
		disabledSprite = CCSprite::create(disabledImage);
	}
	return initWithNormalSprite(normalSprite, selectedSprite, disabledSprite,  selector);
}
//
// Setter of sprite frames
//
void CCMenuItemImageLambda::setNormalSpriteFrame(CCSpriteFrame * frame)
{
	setNormalImage(CCSprite::createWithSpriteFrame(frame));
}

void CCMenuItemImageLambda::setSelectedSpriteFrame(CCSpriteFrame * frame)
{
	setSelectedImage(CCSprite::createWithSpriteFrame(frame));
}

void CCMenuItemImageLambda::setDisabledSpriteFrame(CCSpriteFrame * frame)
{
	setDisabledImage(CCSprite::createWithSpriteFrame(frame));
}

//
// MenuItemToggle
//

void CCMenuItemToggleLambda::setSubItems(CCArray* var)
{
	CC_SAFE_RETAIN(var);
	CC_SAFE_RELEASE(m_pSubItems);
	m_pSubItems = var;
}

CCArray* CCMenuItemToggleLambda::getSubItems()
{
	return m_pSubItems;
}

CCMenuItemToggleLambda * CCMenuItemToggleLambda::createWithTarget(std::function<void(CCObject*)> selector, CCArray* menuItems)
{
	CCMenuItemToggleLambda *pRet = new CCMenuItemToggleLambda();
	pRet->CCMenuItemLambda::initWithTarget( selector);
	pRet->m_pSubItems = CCArray::create();
	pRet->m_pSubItems->retain();
	
	for (unsigned int z=0; z < menuItems->count(); z++)
	{
		CCMenuItemLambda* menuItem = (CCMenuItemLambda*)menuItems->objectAtIndex(z);
		pRet->m_pSubItems->addObject(menuItem);
	}
	
	pRet->m_uSelectedIndex = UINT_MAX;
	pRet->setSelectedIndex(0);
	return pRet;
}

CCMenuItemToggleLambda * CCMenuItemToggleLambda::createWithTarget(std::function<void(CCObject*)> selector, CCMenuItemLambda* item, ...)
{
	va_list args;
	va_start(args, item);
	CCMenuItemToggleLambda *pRet = new CCMenuItemToggleLambda();
	pRet->initWithTarget( selector, item, args);
	pRet->autorelease();
	va_end(args);
	return pRet;
}

CCMenuItemToggleLambda * CCMenuItemToggleLambda::create()
{
	CCMenuItemToggleLambda *pRet = new CCMenuItemToggleLambda();
	pRet->initWithItem(NULL);
	pRet->autorelease();
	return pRet;
}

bool CCMenuItemToggleLambda::initWithTarget(std::function<void(CCObject*)> selector, CCMenuItemLambda* item, va_list args)
{
	CCMenuItemLambda::initWithTarget( selector);
	this->m_pSubItems = CCArray::create();
	this->m_pSubItems->retain();
	int z = 0;
	CCMenuItemLambda *i = item;
	while(i)
	{
		z++;
		m_pSubItems->addObject(i);
		i = va_arg(args, CCMenuItemLambda*);
	}
	m_uSelectedIndex = UINT_MAX;
	this->setSelectedIndex(0);
	return true;
}

CCMenuItemToggleLambda* CCMenuItemToggleLambda::create(CCMenuItemLambda *item)
{
	CCMenuItemToggleLambda *pRet = new CCMenuItemToggleLambda();
	pRet->initWithItem(item);
	pRet->autorelease();
	return pRet;
}

bool CCMenuItemToggleLambda::initWithItem(CCMenuItemLambda *item)
{
	CCMenuItemLambda::initWithTarget([](CCObject*){});
	setSubItems(CCArray::create());
	
	if (item)
	{
		m_pSubItems->addObject(item);
	}
	m_uSelectedIndex = UINT_MAX;
	this->setSelectedIndex(0);
	
	setCascadeColorEnabled(true);
	setCascadeOpacityEnabled(true);
	
	return true;
}

void CCMenuItemToggleLambda::addSubItem(CCMenuItemLambda *item)
{
	m_pSubItems->addObject(item);
}

CCMenuItemToggleLambda::~CCMenuItemToggleLambda()
{
	CC_SAFE_RELEASE(m_pSubItems);
}
void CCMenuItemToggleLambda::setSelectedIndex(unsigned int index)
{
	if( index != m_uSelectedIndex && m_pSubItems->count() > 0 )
	{
		m_uSelectedIndex = index;
		CCMenuItemLambda *currentItem = (CCMenuItemLambda*)getChildByTag(kCurrentItem);
		if( currentItem )
		{
			currentItem->removeFromParentAndCleanup(false);
		}
		
		CCMenuItemLambda* item = (CCMenuItemLambda*)m_pSubItems->objectAtIndex(m_uSelectedIndex);
		this->addChild(item, 0, kCurrentItem);
		CCSize s = item->getContentSize();
		this->setContentSize(s);
		item->setPosition( ccp( s.width/2, s.height/2 ) );
	}
}
unsigned int CCMenuItemToggleLambda::getSelectedIndex()
{
	return m_uSelectedIndex;
}
void CCMenuItemToggleLambda::selected()
{
	CCMenuItemLambda::selected();
	((CCMenuItemLambda*)(m_pSubItems->objectAtIndex(m_uSelectedIndex)))->selected();
}
void CCMenuItemToggleLambda::unselected()
{
	CCMenuItemLambda::unselected();
	((CCMenuItemLambda*)(m_pSubItems->objectAtIndex(m_uSelectedIndex)))->unselected();
}
void CCMenuItemToggleLambda::activate()
{
	// update index
	if( m_bEnabled )
	{
		unsigned int newIndex = (m_uSelectedIndex + 1) % m_pSubItems->count();
		this->setSelectedIndex(newIndex);
	}
	CCMenuItemLambda::activate();
}
void CCMenuItemToggleLambda::setEnabled(bool enabled)
{
	if (m_bEnabled != enabled)
	{
		CCMenuItemLambda::setEnabled(enabled);
		
		if(m_pSubItems && m_pSubItems->count() > 0)
		{
			CCObject* pObj = NULL;
			CCARRAY_FOREACH(m_pSubItems, pObj)
			{
				CCMenuItemLambda* pItem = (CCMenuItemLambda*)pObj;
				pItem->setEnabled(enabled);
			}
		}
	}
}

CCMenuItemLambda* CCMenuItemToggleLambda::selectedItem()
{
	return (CCMenuItemLambda*)m_pSubItems->objectAtIndex(m_uSelectedIndex);
}
