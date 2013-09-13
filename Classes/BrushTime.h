//
//  BrushTime.h
//  DrawingJack
//
//  Created by 사원3 on 13. 2. 28..
//
//

#ifndef DrawingJack_BrushTime_h
#define DrawingJack_BrushTime_h

#include "cocos2d.h"
#include "StarGoldData.h"
#include "AlertEngine.h"
#include "MyLocalization.h"

using namespace cocos2d;

#define BRUSH_DISTANCE	9

class BrushTime : public CCNode
{
public:
	static BrushTime* create(CCObject* t_stageSetting, SEL_CallFunc t_goChapter)
	{
		BrushTime* t_bt = new BrushTime();
		t_bt->myInit(t_stageSetting, t_goChapter);
		t_bt->autorelease();
		return t_bt;
	}
	
	virtual ~BrushTime()
	{
		brush_array->removeAllObjects();
		brush_array->release();
	}
	
	bool getLoaded(){	return is_loaded;	}
	
	void moveScene()
	{
		mySGD->setBrushTime(brush_time);
		mySGD->removeGraphDogTarget();
	}
	
	void rightingReview()
	{
		mySGD->setBrushCnt(5);
		brush_time = -1;
		mySGD->setBrushTime(brush_time);
		
		for(int i=0;i<5;i++)
		{
			CCSprite* t_brush = CCSprite::create("stagesetting_brush.png");
			t_brush->setPosition(ccp(i*BRUSH_DISTANCE,0));
			addChild(t_brush);
			brush_array->addObject(t_brush);
		}
	}
	
	void boughtInfBrush()
	{
		is_brush_inf = true;
		is_loaded = true;
		unschedule(schedule_selector(BrushTime::timeFlow));
		is_running = false;
		DataStorageHub::sharedInstance()->setBoolForKey(kDSH_Key_isBrushInf, true);
		removeAllChildrenWithCleanup(true);
		
		CCSprite* t_brush = CCSprite::create("stagesetting_brush.png");
		t_brush->setPosition(ccp(15,0));
		addChild(t_brush);
		
		CCSprite* t_inf = CCSprite::create("stagesetting_inf.png");
		t_inf->setPosition(ccp(40,-1));
		addChild(t_inf);
	}
	
	void gameStart(CCObject* t_t, SEL_CallFuncB t_d)
	{
		if(!is_brush_inf)
		{
			int brush_cnt = mySGD->getBrushCnt();
			if(is_loaded && brush_cnt > 0)
			{
				if(brush_cnt > 5)
				{
					(t_t->*t_d)(true);
				}
				else
				{
					mySGD->setTargetDelegate(t_t, t_d);
					mySGD->refreshServerTime();
				}
			}
			else
			{
				(t_t->*t_d)(false);
			}
		}
		else
		{
			(t_t->*t_d)(true);
		}
	}
	
	void successGameStart()
	{
		int brush_cnt = mySGD->getBrushCnt();
		if(is_brush_inf)
		{
			
		}
		else if(brush_cnt > 5)
		{
			over_brush->setString(CCString::createWithFormat("+%d", brush_cnt-1-5)->getCString());
			brush_cnt--;
			if(brush_cnt <= 5)
			{
				over_brush->removeFromParentAndCleanup(true);
			}
			mySGD->setBrushCnt(brush_cnt);
		}
		else if(!is_brush_inf)
		{
			brush_cnt--;
			CCSprite* t_brush = (CCSprite*)brush_array->lastObject();
			removeChild(t_brush, true);
			brush_array->removeLastObject();
			
			mySGD->setBrushCnt(brush_cnt);
			
			if(!is_running)
				brush_time = 360;
			
			mySGD->setBrushTime(brush_time);
		}
	}
	
	void alertAction(int t1, int t2)
	{
		moveScene();
		(stageSetting->*goChapter)();
	}
	
private:
	CCArray* brush_array;
	StarGoldData* mySGD;
	bool is_loaded;
	bool is_brush_inf;
	int last_server_time;
	int refreshed_server_time;
	int brush_time;
	CCLabelBMFont* time_min;
	CCLabelBMFont* time_sec;
	CCLabelBMFont* over_brush;
	CCSprite* time_main;
	CCSprite* time_max;
//	int brush_cnt;
	bool is_running;
	
	CCObject* stageSetting;
	SEL_CallFunc goChapter;
	
	void refreshedServerTime(bool t_b)
	{
		if(t_b)
		{
			refreshed_server_time = mySGD->getSavedServerTime();
			int sub_value = refreshed_server_time - last_server_time;
			
			brush_time = mySGD->getBrushTime();
			int brush_cnt = mySGD->getBrushCnt();
			
			if(brush_time > sub_value)
			{
				brush_time -= sub_value;
				mySGD->setBrushTime(brush_time);
				
				time_min->setString(CCString::createWithFormat("%d", brush_time/60)->getCString());
				time_sec->setString(CCString::createWithFormat("%2d", brush_time%60)->getCString());
				is_running = true;
				schedule(schedule_selector(BrushTime::timeFlow), 1.f);
			}
			else if(brush_cnt < 5)
			{
				sub_value -= brush_time;
				CCSprite* t_brush = CCSprite::create("stagesetting_brush.png");
				t_brush->setPosition(ccp(brush_array->count()*BRUSH_DISTANCE,0));
				addChild(t_brush);
				brush_array->addObject(t_brush);
				brush_cnt++;
				
				mySGD->setBrushCnt(brush_cnt);
				mySGD->setBrushTime(brush_time);
				
				while(true)
				{
					if(brush_array->count() < 5)
					{
						if(sub_value >= 360)
						{
							sub_value -= 360;
							CCSprite* t_brush = CCSprite::create("stagesetting_brush.png");
							t_brush->setPosition(ccp(brush_array->count()*BRUSH_DISTANCE,0));
							addChild(t_brush);
							brush_array->addObject(t_brush);
							brush_cnt++;
							mySGD->setBrushCnt(brush_cnt);
						}
						else
						{
							brush_time = 360 - sub_value;
							mySGD->setBrushTime(brush_time);
							
							time_min->setString(CCString::createWithFormat("%d", brush_time/60)->getCString());
							time_sec->setString(CCString::createWithFormat("%2d", brush_time%60)->getCString());
							is_running = true;
							schedule(schedule_selector(BrushTime::timeFlow), 1.f);
							break;
						}
					}
					else
					{
						brush_time = -1;
						mySGD->setBrushTime(brush_time);
						
						time_main->removeFromParentAndCleanup(true);
						time_min->removeFromParentAndCleanup(true);
						time_sec->removeFromParentAndCleanup(true);
						
						time_max = CCSprite::create("stagesetting_brush_max.png");
						time_max->setPosition(ccp(35,-17)); // 65, 0
						addChild(time_max);
						break;
					}
				}
			}
			is_loaded = true;
		}
		else
		{
			AlertEngine::sharedInstance()->addSingleAlert("error", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_canNotConnectedServer), "Ok", 0, this, alertfuncII_selector(BrushTime::alertAction));
		}
	}
	
	void timeFlow()
	{
		if(!mySGD->getIsCreditServerTime())
		{
			unschedule(schedule_selector(BrushTime::timeFlow));
			removeAllChildrenWithCleanup(true);
			brush_array->removeAllObjects();
			brush_array->release();
			myInit(stageSetting, goChapter);
			return;
		}
		
		if(brush_time == 0)
		{
			CCSprite* t_brush = CCSprite::create("stagesetting_brush.png");
			t_brush->setPosition(ccp(brush_array->count()*BRUSH_DISTANCE,0));
			addChild(t_brush);
			brush_array->addObject(t_brush);
			
			mySGD->setBrushCnt(mySGD->getBrushCnt() + 1);
			
			if(mySGD->getBrushCnt() >= 5)
			{
				brush_time = -1;
				
				time_main->removeFromParentAndCleanup(true);
				time_min->removeFromParentAndCleanup(true);
				time_sec->removeFromParentAndCleanup(true);
				
				time_max = CCSprite::create("stagesetting_brush_max.png");
				time_max->setPosition(ccp(35,-17)); // 65, 0
				addChild(time_max);
				
				unschedule(schedule_selector(BrushTime::timeFlow));
				is_running = false;
				return;
			}
			brush_time = 360;
		}
		brush_time--;
		time_min->setString(CCString::createWithFormat("%d", brush_time/60)->getCString());
		time_sec->setString(CCString::createWithFormat("%2d", brush_time%60)->getCString());
	}
	
	void myInit(CCObject* t_stageSetting, SEL_CallFunc t_goChapter)
	{
		stageSetting = t_stageSetting;
		goChapter = t_goChapter;
		is_brush_inf = DataStorageHub::sharedInstance()->getBoolForKey(kDSH_Key_isBrushInf);
		is_loaded = false;
		is_running = false;
		over_brush = NULL;
		brush_array = new CCArray(1);
		mySGD = mySGD;
		mySGD->setBrushTimeInstance(this);
		
		if(is_brush_inf)
		{
			CCSprite* t_brush = CCSprite::create("stagesetting_brush.png");
			t_brush->setPosition(ccp(15,0));
			addChild(t_brush);
			
			CCSprite* t_inf = CCSprite::create("stagesetting_inf.png");
			t_inf->setPosition(ccp(40,-1));
			addChild(t_inf);
			
			is_loaded = true;
		}
		else
		{
			int brush_cnt = mySGD->getBrushCnt();
			
			int create_brush_cnt = brush_cnt >= 5 ? 5 : brush_cnt;
			for(int i=0;i<create_brush_cnt;i++)
			{
				CCSprite* t_brush = CCSprite::create("stagesetting_brush.png");
				t_brush->setPosition(ccp(i*BRUSH_DISTANCE,0));
				addChild(t_brush);
				brush_array->addObject(t_brush);
			}
			
			if(brush_cnt > 5)
			{
				if(!over_brush)
				{
					over_brush = CCLabelBMFont::create("", "brush_plus.fnt");
					over_brush->setAlignment(kCCTextAlignmentLeft);
					over_brush->setAnchorPoint(ccp(0,0.5));
					over_brush->setPosition(ccp(25,-20)); // 55, -3
					addChild(over_brush);
				}
				over_brush->setString(CCString::createWithFormat("+%d", brush_cnt-5)->getCString());
			}
			else if(brush_cnt == 5)
			{
				time_max = CCSprite::create("stagesetting_brush_max.png");
				time_max->setPosition(ccp(35,-17)); // 65, 0
				addChild(time_max);
			}
			else
			{
				time_main = CCSprite::create("stagesetting_time.png");
				time_main->setPosition(ccp(28,-17)); // 58, 0
				addChild(time_main);
				
				time_min = CCLabelBMFont::create("0", "font_brush_time.fnt");
				time_min->setAlignment(kCCTextAlignmentRight);
				time_min->setAnchorPoint(ccp(1.f,0.5f));
				time_min->setPosition(ccp(26,-23)); // 56, -6
				addChild(time_min);
				
				time_sec = CCLabelBMFont::create("00", "font_brush_time.fnt");
				time_sec->setAlignment(kCCTextAlignmentLeft);
				time_sec->setAnchorPoint(ccp(0.f,0.5f));
				time_sec->setPosition(ccp(34,-23)); // 64, -6
				addChild(time_sec);
			}
			
			if(brush_cnt >= 5)
			{
				mySGD->setBrushTime(-1);
			}
			last_server_time = mySGD->getSavedServerTime();
			mySGD->setTargetDelegate(this, callfuncB_selector(BrushTime::refreshedServerTime));
			mySGD->refreshServerTime();
		}
	}
};

#endif
