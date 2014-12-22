//
//  TakeCardToDiary.h
//  DGproto
//
//  Created by 사원3 on 2014. 4. 17..
//
//

#ifndef __DGproto__TakeCardToDiary__
#define __DGproto__TakeCardToDiary__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"
#include "KSUtil.h"
#include <random>
#include "KSLabelTTF.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum TakeCardToDiaryMenuTag
{
	kTCTD_MT_second = 1,
	kTCTD_MT_third,
	kTCTD_MT_forth
};

class TakeCardToDiary : public CCLayer
{
public:
	static TakeCardToDiary* create(int t_take_card_number, std::function<void()> t_end_func);
	
private:
	
	int recent_card_number;
	std::function<void()> end_func;
	
	CCSprite* recent_left_img;
	CCSprite* recent_right_img;
	CCSprite* smaller_img;
	CCSprite* top_cover;
	CCSprite* bottom_cover;
	CCSprite* gray;
	CCScale9Sprite* exp_back;
	
	void myInit(int t_take_card_number, std::function<void()> t_end_func);
	
	float animation_angle;
	CCSprite* animation_img;
	int touch_direction;
	int touch_end_direction;
	SEL_CallFunc end_animation_delegate;
	
	void startPrePageFull();
	
	void ingPageFull();
	
	void startPrePage();

	void ingPage();
	
	void startHiding();
	
	CCPoint getContentPosition(int t_tag);
	
	void setRightPage(CCNode *target, int card_number);
	
	void setLeftPage(CCNode *target, int card_number);
};


#endif /* defined(__DGproto__TakeCardToDiary__) */
