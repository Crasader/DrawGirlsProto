//
//  LoadingTipScene.h
//  DGproto
//
//  Created by 사원3 on 2014. 1. 21..
//
//

#ifndef __DGproto__LoadingTipScene__
#define __DGproto__LoadingTipScene__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class CommonButton;
class CCMenuLambda;
class KSLabelTTF;
class LoadingTipScene : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
	static CCSprite* getLoadingTipImage();
	static CCNode* getCurtainTipImage();
	static CCNode* getOpenCurtainNode(bool is_gameover = false);
	
	static LoadingTipScene* getLoadingTipSceneLayer();
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(LoadingTipScene);
	
private:
	
	CCSprite* loading_progress_img;
	
	string next_scene_name;
	KSLabelTTF* progress_label;
	int total_load_img;
	int ing_load_img;
	
	bool is_minimum_time;
	
	bool is_mission_tip;
	
	void onMinimumTime();
	
	vector<string> sil_load_list;
	vector<string> default_load_list;
	
	void readyLoading();
	void countingFunc(CCObject* sender);
	void startLoading();
	
	
	CCSprite* ok_img;
	CCMenuLambda* ok_menu;
	CCScale9Sprite* no_img;
	
	CCNode* content_node;
	
	void endLoadingTip();
	
	void showButton();
	
	
	CCNode* getMissionTipImage();
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked();
};

#endif /* defined(__DGproto__LoadingTipScene__) */
