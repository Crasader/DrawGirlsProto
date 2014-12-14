//
//  InvenPopup.h
//  DGproto
//
//  Created by ksoo k on 2014. 12. 8..
//
//

#ifndef __DGproto__InvenPopup__
#define __DGproto__InvenPopup__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"
#include "ScrollBar.h"
#include "StarGoldData.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
class ScrollBar;
enum TagNumber
{
	myChar, myCard, closeTag
};
class InvenPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	virtual bool init();
	
	// a selector callback
	void menuAction(CCObject* pSender);
	
	// preprocessor macro for "static create()" constructor ( node() deprecated )
	CREATE_FUNC(InvenPopup);
	
	virtual void onEnter();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
	virtual ~InvenPopup();
	
private:
	
	//
	//	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	//	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	//	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	//	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	//	virtual void registerWithTouchDispatcher ();
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual CCTableViewCell* itemTableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual CCTableViewCell* infoTableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void scrollViewDidScroll(CCScrollView* view);
	virtual void scrollViewDidZoom(CCScrollView* view);
	
	std::string goodsToImageFile(GoodsType t_goods)
	{
		if(t_goods == kGoodsType_gold)
		{
			return("icon_g.png");
			//		return_node->setScale(0.7f);
		}
		else if(t_goods == kGoodsType_ruby)
		{
			return("icon_r.png");
			//		return_node->setScale(0.7f);
		}
		else if(t_goods == kGoodsType_item9)
		{
			return("icon_i9.png");
			//		return_node->setScale(0.55f);
		}
		else if(t_goods == kGoodsType_item6)
		{
			return("icon_i6.png");
			//		return_node->setScale(0.55f);
		}
		else if(t_goods == kGoodsType_item11)
		{
			return("icon_i11.png");
			//		return_node->setScale(0.55f);
		}
		else if(t_goods == kGoodsType_pass1)
		{
			return("icon_p1.png");
		}
		else if(t_goods == kGoodsType_pass2)
		{
			return("icon_p2.png");
		}
		else if(t_goods == kGoodsType_pass3)
		{
			return("icon_p3.png");
		}
		else if(t_goods == kGoodsType_pass4)
		{
			return("icon_p4.png");
		}
		else if(t_goods == kGoodsType_pass5)
		{
			return("icon_p5.png");
		}
		else if(t_goods == kGoodsType_pass6)
		{
			return("icon_p6.png");
		}
		else if(t_goods == kGoodsType_pass7)
		{
			return("icon_p7.png");
		}
		else if(t_goods == kGoodsType_pass8)
		{
			return("icon_p8.png");
		}
		else if(t_goods == kGoodsType_pass9)
		{
			return("icon_p9.png");
		}
		else if(t_goods == kGoodsType_pass10)
		{
			return("icon_p10.png");
		}
		else if(t_goods == kGoodsType_pass11)
		{
			return("icon_p11.png");
		}
		else if(t_goods == kGoodsType_heart)
		{
			return("icon_h.png");
		}
		else
		{
			return("icon_box.png");
		}
	}
	//	Json::Value user_list;
	Json::Value rank_data;
	CCSprite* loading_img;
	CCScale9Sprite* m_main_inner_right;
	
	struct HasItem
	{
		std::string itemDesc;
		std::string itemFile;
		int itemCount;
		HasItem(const std::string& i, const std::string& ifa, int ic) :
		itemDesc(i), itemFile(ifa), itemCount(ic)
		{
			
		}
	};
	
	std::vector<HasItem> m_itemList;
	CCTableView* m_itemTable;
	
	
	struct MyInfo
	{
		std::string styledTitle;
		std::string styledContent;
		MyInfo(const std::string& t, const std::string& c) :
		styledTitle(t), styledContent(c)
		{
			
		}
	};
	
	std::vector<MyInfo> m_infoList;
	CCTableView* m_infoTable;
	
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	
	//ScrollBar* m_scrollBar;
	CCTableView* rank_table;
	CCSprite* main_case;
	CCSprite* gray;
	CCNode* rankBack;
	int delay_index;
	ScrollBar* m_scrollBar, *m_rewardScrollBar;
	vector<function<void()>> cell_action_list;
	
	void showPopup();
	void endShowPopup();
	void hidePopup();
	void endHidePopup();
	
	void reOpenPopup();
	
	bool is_menu_enable;
	CCTableView* reward_table;

};

#endif /* defined(__DGproto__InvenPopup__) */
