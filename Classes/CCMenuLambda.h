#pragma once

#include "cocos2d.h"
#include "CCMenuItemLambda.h"
USING_NS_CC;

/** @brief A CCMenu
 *
 * Features and Limitation:
 *  - You can add MenuItem objects in runtime using addChild:
 *  - But the only accepted children are MenuItem objects
 */
class CC_DLL CCMenuLambda : public CCLayerRGBA
{
	/** whether or not the menu will receive events */
	bool m_bEnabled;
	
public:
	CCMenuLambda() : m_pSelectedItem(NULL) {}
	virtual ~CCMenuLambda(){}
	
	/** creates an empty CCMenu */
	static CCMenuLambda* create();
	
	/** creates a CCMenu with CCMenuItemLambda objects */
	static CCMenuLambda* create(CCMenuItemLambda* item, ...);
	
	/** creates a CCMenu with a CCArray of CCMenuItemLambda objects */
	static CCMenuLambda* createWithArray(CCArray* pArrayOfItems);
	
	/** creates a CCMenu with it's item, then use addChild() to add
	 * other items. It is used for script, it can't init with undetermined
	 * number of variables.
	 */
	static CCMenuLambda* createWithItem(CCMenuItemLambda* item);
	
	/** creates a CCMenu with CCMenuItemLambda objects */
	static CCMenuLambda* createWithItems(CCMenuItemLambda *firstItem, va_list args);
	
	/** initializes an empty CCMenu */
	bool init();
	
	/** initializes a CCMenu with a NSArray of CCMenuItemLambda objects */
	bool initWithArray(CCArray* pArrayOfItems);
	
	/** align items vertically */
	void alignItemsVertically();
	/** align items vertically with padding
	 @since v0.7.2
	 */
	void alignItemsVerticallyWithPadding(float padding);
	
	/** align items horizontally */
	void alignItemsHorizontally();
	/** align items horizontally with padding
	 @since v0.7.2
	 */
	void alignItemsHorizontallyWithPadding(float padding);
	
	/** align items in rows of columns */
	void alignItemsInColumns(unsigned int columns, ...);
	void alignItemsInColumns(unsigned int columns, va_list args);
	void alignItemsInColumnsWithArray(CCArray* rows);
	
	/** align items in columns of rows */
	void alignItemsInRows(unsigned int rows, ...);
	void alignItemsInRows(unsigned int rows, va_list args);
	void alignItemsInRowsWithArray(CCArray* columns);
	
	/** set event handler priority. By default it is: kCCMenuTouchPriority */
	void setHandlerPriority(int newPriority);
	
	//super methods
	virtual void addChild(CCNode * child);
	virtual void addChild(CCNode * child, int zOrder);
	virtual void addChild(CCNode * child, int zOrder, int tag);
	virtual void registerWithTouchDispatcher();
	virtual void removeChild(CCNode* child, bool cleanup);
	
	/**
	 @brief For phone event handle functions
	 */
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	
	/**
	 @since v0.99.5
	 override onExit
	 */
	virtual void onExit();
	
	virtual void setOpacityModifyRGB(bool bValue) {CC_UNUSED_PARAM(bValue);}
	virtual bool isOpacityModifyRGB(void) { return false;}
	
	virtual bool isEnabled() { return m_bEnabled; }
	virtual void setEnabled(bool value) { m_bEnabled = value; };
	
protected:
	CCMenuItemLambda* itemForTouch(CCTouch * touch);
	tCCMenuState m_eState;
	CCMenuItemLambda *m_pSelectedItem;
	
	CCPoint m_touchesStart;
	CCPoint m_touchesEnd;
};

// end of GUI group
/// @}
/// @}