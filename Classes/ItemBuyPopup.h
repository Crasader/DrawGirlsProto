// ItemBuyPopup.h
//

#ifndef LZZ_ItemBuyPopup_h
#define LZZ_ItemBuyPopup_h
#include "cocos2d.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"

USING_NS_CC;
#define LZZ_INLINE inline
enum IBP_MenuTag
{
	kIBP_MT_buy = 1,
	kIBP_MT_close
};
enum IBP_Zorder
{
	kIBP_Z_back = 1,
	kIBP_Z_case,
	kIBP_Z_menu
};
class ItemBuyPopup : public CCLayer
{
public:
	static ItemBuyPopup * create (ITEM_CODE t_item_type, int t_clicked_item_number, CCObject * t_buy, SEL_CallFuncII d_buy);
	void setCloseTargetDelegate(CCObject* t_close, SEL_CallFunc d_close);
private:
	bool is_menu_enable;
	ITEM_CODE item_type;
	int clicked_item_number;
	int touched_number;
	CCMenu * buy_menu;
	CCMenu * close_menu;
	CCObject * target_buy;
	SEL_CallFuncII delegate_buy;
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	void myInit (ITEM_CODE t_item_type, int t_clicked_item_number, CCObject * t_buy, SEL_CallFuncII d_buy);
	void menuAction (CCObject * sender);
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	virtual void registerWithTouchDispatcher ();
};
#undef LZZ_INLINE
#endif
