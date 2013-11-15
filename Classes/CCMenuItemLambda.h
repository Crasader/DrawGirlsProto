#pragma once

#include "cocos2d.h"

USING_NS_CC;

/**
 * @addtogroup GUI
 * @{
 * @addtogroup menu
 * @{
 */

/** @brief CCMenuItem base class
 *
 *  Subclass CCMenuItem (or any subclass) to create your custom CCMenuItem objects.
 */
class CCMenuItemLambda : public CCNodeRGBA
{
protected:
	/** whether or not the item is selected
	 @since v0.8.2
	 */
	bool m_bSelected;
	bool m_bEnabled;
	
public:
	CCMenuItemLambda()
	: m_bSelected(false)
	, m_bEnabled(false)
	, m_nScriptTapHandler(0),
	m_afterSelector(nullptr)
	{}
	virtual ~CCMenuItemLambda();
	
	/** Creates a CCMenuItem with no target/selector */
	static CCMenuItemLambda* create();
	/** Creates a CCMenuItem with a target/selector */
	static CCMenuItemLambda* create(std::function<void(CCObject*)> selector);
	/** Initializes a CCMenuItem with a target/selector */
	bool initWithTarget(std::function<void(CCObject*)> selector);
	/** Returns the outside box */
	CCRect rect();
	/** Activate the item */
	virtual void activate();
	/** The item was selected (not activated), similar to "mouse-over" */
	virtual void selected();
	/** The item was unselected */
	virtual void unselected();
	
	/** Register menu handler script function */
	virtual void registerScriptTapHandler(int nHandler);
	virtual void unregisterScriptTapHandler(void);
	int getScriptTapHandler() { return m_nScriptTapHandler; };
	
	virtual bool isEnabled();
	//@note: It's 'setIsEnable' in cocos2d-iphone.
	virtual void setEnabled(bool value);
	virtual bool isSelected();
	
	virtual void setOpacityModifyRGB(bool bValue) {CC_UNUSED_PARAM(bValue);}
	virtual bool isOpacityModifyRGB(void) { return false;}
	
	/** set the target/selector of the menu item*/
	void setTarget(std::function<void(CCObject*)> selector);
	
public:
	std::function<void(CCObject*)> m_selector;
	std::function<void(CCObject*)> m_afterSelector;
	int             m_nScriptTapHandler;
};

/** @brief An abstract class for "label" CCMenuItemLabel items
 Any CCNode that supports the CCLabelProtocol protocol can be added.
 Supported nodes:
 - CCBitmapFontAtlas
 - CCLabelAtlas
 - CCLabelTTF
 */
class CCMenuItemLabelLambda : public CCMenuItemLambda
{
	/** the color that will be used to disable the item */
	CC_PROPERTY_PASS_BY_REF(ccColor3B, m_tDisabledColor, DisabledColor);
	/** Label that is rendered. It can be any CCNode that implements the CCLabelProtocol */
	CC_PROPERTY(CCNode*, m_pLabel, Label);
public:
	CCMenuItemLabelLambda()
	: m_pLabel(NULL)
	, m_fOriginalScale(0.0)
	{}
	virtual ~CCMenuItemLabelLambda();
	
	/** creates a CCMenuItemLabel with a Label, target and selector */
	static CCMenuItemLabelLambda * create(CCNode*label, std::function<void(CCObject*)> selector);
	/** creates a CCMenuItemLabel with a Label. Target and selector will be nil */
	static CCMenuItemLabelLambda* create(CCNode *label);
	
	/** initializes a CCMenuItemLabel with a Label, target and selector */
	bool initWithLabel(CCNode* label, std::function<void(CCObject*)> selector);
	/** sets a new string to the inner label */
	void setString(const char * label);
	// super methods
	virtual void activate();
	virtual void selected();
	virtual void unselected();
	/** Enable or disabled the CCMenuItemFont
	 @warning setEnabled changes the RGB color of the font
	 */
	virtual void setEnabled(bool enabled);
	
protected:
	ccColor3B    m_tColorBackup;
	float        m_fOriginalScale;
};


/** @brief A CCMenuItemAtlasFont
 Helper class that creates a MenuItemLabel class with a LabelAtlas
 */
class CCMenuItemAtlasFontLambda : public CCMenuItemLabelLambda
{
public:
	CCMenuItemAtlasFontLambda(){}
	virtual ~CCMenuItemAtlasFontLambda(){}
	
	/** creates a menu item from a string and atlas with a target/selector */
	static CCMenuItemAtlasFontLambda* create(const char *value, const char *charMapFile, int itemWidth, int itemHeight, char startCharMap);
	/** creates a menu item from a string and atlas. Use it with MenuItemToggle */
	static CCMenuItemAtlasFontLambda* create(const char *value, const char *charMapFile, int itemWidth, int itemHeight, char startCharMap, std::function<void(CCObject*)> selector);
	/** initializes a menu item from a string and atlas with a target/selector */
	bool initWithString(const char *value, const char *charMapFile, int itemWidth, int itemHeight, char startCharMap, std::function<void(CCObject*)> selector);
};


/** @brief A CCMenuItemFont
 Helper class that creates a CCMenuItemLabel class with a Label
 */
class CCMenuItemFontLambda : public CCMenuItemLabelLambda
{
public:
	CCMenuItemFontLambda() : m_uFontSize(0), m_strFontName(""){}
	virtual ~CCMenuItemFontLambda(){}
	/** set default font size */
	static void setFontSize(unsigned int s);
	/** get default font size */
	static unsigned int fontSize();
	/** set the default font name */
	static void setFontName(const char *name);
	/** get the default font name */
	static const char *fontName();
	
	/** creates a menu item from a string without target/selector. To be used with CCMenuItemToggle */
	static CCMenuItemFontLambda * create(const char *value);
	/** creates a menu item from a string with a target/selector */
	static CCMenuItemFontLambda * create(const char *value, std::function<void(CCObject*)> selector);
	
	/** initializes a menu item from a string with a target/selector */
	bool initWithString(const char *value, std::function<void(CCObject*)> selector);
	
	/** set font size
	 * c++ can not overload static and non-static member functions with the same parameter types
	 * so change the name to setFontSizeObj
	 */
	void setFontSizeObj(unsigned int s);
	
	/** get font size */
	unsigned int fontSizeObj();
	
	/** set the font name
	 * c++ can not overload static and non-static member functions with the same parameter types
	 * so change the name to setFontNameObj
	 */
	void setFontNameObj(const char* name);
	
	const char* fontNameObj();
	
protected:
	void recreateLabel();
	
	unsigned int m_uFontSize;
	std::string m_strFontName;
};


/** @brief CCMenuItemSprite accepts CCNode<CCRGBAProtocol> objects as items.
 The images has 3 different states:
 - unselected image
 - selected image
 - disabled image
 
 @since v0.8.0
 */
class  CCMenuItemSpriteLambda : public CCMenuItemLambda
{
	/** the image used when the item is not selected */
	CC_PROPERTY(CCNode*, m_pNormalImage, NormalImage);
	/** the image used when the item is selected */
	CC_PROPERTY(CCNode*, m_pSelectedImage, SelectedImage);
	/** the image used when the item is disabled */
	CC_PROPERTY(CCNode*, m_pDisabledImage, DisabledImage);
public:
	CCMenuItemSpriteLambda()
	:m_pNormalImage(NULL)
	,m_pSelectedImage(NULL)
	,m_pDisabledImage(NULL)
	{}
	
	/** creates a menu item with a normal, selected and disabled image*/
	static CCMenuItemSpriteLambda * create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite = NULL);
	/** creates a menu item with a normal and selected image with target/selector */
	static CCMenuItemSpriteLambda * create(CCNode* normalSprite, CCNode* selectedSprite, std::function<void(CCObject*)> selector);
	/** creates a menu item with a normal,selected  and disabled image with target/selector */
	static CCMenuItemSpriteLambda * create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, std::function<void(CCObject*)> selector);
	
	/** initializes a menu item with a normal, selected  and disabled image with target/selector */
	bool initWithNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, std::function<void(CCObject*)> selector);
	
	/**
	 @since v0.99.5
	 */
	virtual void selected();
	virtual void unselected();
	virtual void setEnabled(bool bEnabled);
	
	virtual void setOpacityModifyRGB(bool bValue) {CC_UNUSED_PARAM(bValue);}
	virtual bool isOpacityModifyRGB(void) { return false;}
protected:
	virtual void updateImagesVisibility();
};


/** @brief CCMenuItemImage accepts images as items.
 The images has 3 different states:
 - unselected image
 - selected image
 - disabled image
 
 For best results try that all images are of the same size
 */
class CCMenuItemImageLambda : public CCMenuItemSpriteLambda
{
public:
	CCMenuItemImageLambda(){}
	virtual ~CCMenuItemImageLambda(){}
	
	/** creates a menu item with a normal and selected image*/
	static CCMenuItemImageLambda* create(const char *normalImage, const char *selectedImage);
	/** creates a menu item with a normal,selected  and disabled image*/
	static CCMenuItemImageLambda* create(const char *normalImage, const char *selectedImage, const char *disabledImage);
	/** creates a menu item with a normal and selected image with target/selector */
	static CCMenuItemImageLambda* create(const char *normalImage, const char *selectedImage, std::function<void(CCObject*)> selector);
	/** creates a menu item with a normal,selected  and disabled image with target/selector */
	static CCMenuItemImageLambda* create(const char *normalImage, const char *selectedImage, const char *disabledImage, std::function<void(CCObject*)> selector);
	
	bool init();
	/** initializes a menu item with a normal, selected  and disabled image with target/selector */
	bool initWithNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage, std::function<void(CCObject*)> selector);
	/** sets the sprite frame for the normal image */
	void setNormalSpriteFrame(CCSpriteFrame* frame);
	/** sets the sprite frame for the selected image */
	void setSelectedSpriteFrame(CCSpriteFrame* frame);
	/** sets the sprite frame for the disabled image */
	void setDisabledSpriteFrame(CCSpriteFrame* frame);
	
	/** Creates an CCMenuItemImage.
	 */
	static CCMenuItemImageLambda* create();
};


/** @brief A CCMenuItemToggle
 A simple container class that "toggles" it's inner items
 The inner items can be any MenuItem
 */
class CCMenuItemToggleLambda : public CCMenuItemLambda
{
	/** returns the selected item */
	CC_PROPERTY(unsigned int, m_uSelectedIndex, SelectedIndex);
	/** CCMutableArray that contains the subitems. You can add/remove items in runtime, and you can replace the array with a new one.
	 @since v0.7.2
	 */
	CC_PROPERTY(CCArray*, m_pSubItems, SubItems);
public:
	CCMenuItemToggleLambda()
	: m_uSelectedIndex(0)
	, m_pSubItems(NULL)
	{}
	virtual ~CCMenuItemToggleLambda();
	
	/** creates a menu item from a CCArray with a target selector */
	static CCMenuItemToggleLambda * createWithTarget(std::function<void(CCObject*)> selector, CCArray* menuItems);
	
	/** creates a menu item from a list of items with a target/selector */
	static CCMenuItemToggleLambda* createWithTarget(std::function<void(CCObject*)> selector, CCMenuItemLambda* item, ...);
	
	/** creates a menu item with no target/selector and no items */
	static CCMenuItemToggleLambda* create();
	
	/** initializes a menu item from a list of items with a target selector */
	bool initWithTarget(std::function<void(CCObject*)> selector, CCMenuItemLambda* item, va_list args);
	
	/** creates a menu item with a item */
	static CCMenuItemToggleLambda* create(CCMenuItemLambda *item);
	
	/** initializes a menu item with a item */
	bool initWithItem(CCMenuItemLambda *item);
	/** add more menu item */
	void addSubItem(CCMenuItemLambda *item);
	
	/** return the selected item */
	CCMenuItemLambda* selectedItem();
	// super methods
	virtual void activate();
	virtual void selected();
	virtual void unselected();
	virtual void setEnabled(bool var);
	
	virtual void setOpacityModifyRGB(bool bValue) {CC_UNUSED_PARAM(bValue);}
	virtual bool isOpacityModifyRGB(void) { return false;}
};


