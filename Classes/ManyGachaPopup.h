//
//  ManyGachaPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 10. 26..
//
//

#ifndef __DGproto__ManyGachaPopup__
#define __DGproto__ManyGachaPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LoadingLayer;
class KSLabelTTF;
class DownloadFile;
class ManyGachaPopup : public CCLayer, public CCBAnimationManagerDelegate
{
public:
	virtual bool init();
	void menuAction(CCObject* sender);
	
	CREATE_FUNC(ManyGachaPopup);
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
	virtual void onEnter();
	
	virtual void completedAnimationSequenceNamed(const char *name);
	
private:
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	void showPopup();
	void endShowPopup();
	void hidePopup();
	void endHidePopup();
	
	bool is_menu_enable;
	
	CCSprite* main_case;
	CCSprite* gray;
	
	CCScale9Sprite* main_inner;
	CCNode* inner_node;
	
	bool is_opening;
	
	void setOpening();
	void setNormalGacha();
	void setPremiumGacha();
	
	LoadingLayer* loading_layer;
	
	void resultGetGachaList(Json::Value result_data);
	
	bool loading_type_is_normal;
	
	vector<CCNode*> reward_list;
	vector<int> enable_gacha_list;
	Json::Value json_list;
	
	CCControlButton* gacha_button;
	KSLabelTTF* gacha_label;
	pair<CCSprite*, CCBAnimationManager*> normal_ccb;
	pair<CCSprite*, CCBAnimationManager*> premium_ccb;
	
	int keep_value;
	int selected_index;
	
	function<void()> success_func;
	
	void normalAction(CCObject* sender, CCControlEvent t_event);
	void resultNormalProperties(Json::Value result_data);
	void resultNormalExchange(Json::Value result_data);
	void premiumAction(CCObject* sender, CCControlEvent t_event);
	void resultPremiumProperties(Json::Value result_data);
	void resultPremiumExchange(Json::Value result_data);
	
	KSLabelTTF* refresh_time_label;
	
	void refreshTimeChecking();
	void normalRefresh(CCObject* sender, CCControlEvent t_event);
	void premiumRefresh(CCObject* sender, CCControlEvent t_event);
	
	vector<DownloadFile> card_download_list;
	function<void()> card_downloaded_func;
	int ing_card_download;
	void startCardDownload();
	void successCardDownload();
	void failCardDownload();
	int keep_card_number;
};

#endif /* defined(__DGproto__ManyGachaPopup__) */
