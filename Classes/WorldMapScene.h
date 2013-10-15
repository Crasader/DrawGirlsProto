//
//  WorldMapScene.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 10..
//
//

#ifndef __DGproto__WorldMapScene__
#define __DGproto__WorldMapScene__

#include "cocos2d.h"
#include "AlertEngine.h"
#include "GameData.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "GraphDog.h"
#include "hspConnector.h"

USING_NS_CC;
using namespace std;

class WorldMapScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(WorldMapScene);
	
private:
	bool is_menu_enable;
	void alertAction(int t1, int t2);
	virtual void keyBackClicked(void);
	
	CCNode* map_node;
	
	CCPoint getCouponPosition(int stage);
	CCPoint getStagePosition(int stage);
	CCPoint getUiButtonPosition(int t_tag);
	
	void resultLogin(Json::Value result_data);
	
	int pass_target_stage;
	
	void passTicketStage();
	void resetWorldMapScene();
	void setWorldMapScene();
	void setMapNode();
	void setUIs();
	CCPoint getTrackPositionMove(string d);
	int getRotateValue(string d);
	int getTurnRotateValue(string d, string a_d);
	
//	void showPopup(int stage);
	void stageCancel();
	void popupClose();
	
	void showConvertSildata(string filename);
	
	float minimum_scale;
	float maximum_scale;
	
	float left_x, right_x, top_y, bottom_y;
	
	bool isAnimated;
	CCPoint touch_p;
    long touchStartTime;
	CCPoint touchStart_p;
	CCPoint moveSpeed_p;
	float zoom_base_distance;
	
	map<int, CCPoint> multiTouchData;
	
	void moveListXY(CCPoint t_p);
	void moveAnimation();
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
	{
		CCSetIterator iter;
		CCTouch *touch;
		
		for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			
			multiTouchData[(int)touch] = location;
			
			touch_p = location;
			
			timeval time;
			gettimeofday(&time, NULL);
			touchStartTime = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
			touchStart_p = location;
			
			this->unschedule(schedule_selector(WorldMapScene::moveAnimation));
			
			isAnimated=false;
			
			if(multiTouchData.size() == 1)
			{
				
			}
			else if(multiTouchData.size() == 2)
			{
				CCPoint sub_point = CCPointZero;
				map<int, CCPoint>::iterator it;
				for(it = multiTouchData.begin();it!=multiTouchData.end();it++)
				{
					sub_point = ccpMult(sub_point, -1);
					sub_point = ccpAdd(sub_point, it->second);
				}
				
				zoom_base_distance = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
			}
			else
			{
				
			}
		}
	}
	
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
	{
		CCSetIterator iter;
		CCTouch* touch;
		
		for(iter = pTouches->begin();iter != pTouches->end();++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				o_it->second = location;
				if(multiTouchData.size() == 1)
				{
					this->moveListXY(ccpSub(touch_p, location));
					touch_p = location;
				}
				else if(multiTouchData.size() == 2)
				{
					CCPoint sub_point = CCPointZero;
					map<int, CCPoint>::iterator it;
					for(it = multiTouchData.begin();it!=multiTouchData.end();it++)
					{
						sub_point = ccpMult(sub_point, -1);
						sub_point = ccpAdd(sub_point, it->second);
					}
					
					float before_scale = map_node->getScale();
					
					float changed_distance = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
					float after_scale = map_node->getScale()*changed_distance/zoom_base_distance;
					if(after_scale > maximum_scale)				after_scale = maximum_scale;
					else if(after_scale < minimum_scale)		after_scale = minimum_scale;
					zoom_base_distance = changed_distance;
					map_node->setScale(after_scale);
					
//					if(map_node->getScale() <= maximum_scale)
//						map_node->setPositionX((480.f*maximum_scale-480.f*map_node->getScale())/2.f);
//					else
//					{
						float x_value = map_node->getPositionX()-240.f;
						x_value = x_value*map_node->getScale()/before_scale;
						x_value += 240.f;
					
						map_node->setPositionX(x_value);
					
						if(map_node->getPositionX() > -(left_x*map_node->getScale() - 240.f))
							map_node->setPositionX(-(left_x*map_node->getScale() - 240.f));
						else if(map_node->getPositionX() < -(right_x*map_node->getScale() - 240.f))
							map_node->setPositionX(-(right_x*map_node->getScale() - 240.f));
//					}
//
					float y_value = map_node->getPositionY()-160.f;
					y_value = y_value*map_node->getScale()/before_scale;
					y_value += 160.f;
					
					map_node->setPositionY(y_value);
					
					if(map_node->getPositionY() > -(bottom_y*map_node->getScale() - 160.f))
						map_node->setPositionY(-(bottom_y*map_node->getScale() - 160.f));
					else if(map_node->getPositionY() < -(top_y*map_node->getScale() - 160.f))
						map_node->setPositionY(-(top_y*map_node->getScale() - 160.f));
				}
			}
		}
	}
	
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
	{
		CCSetIterator iter;
		CCTouch* touch;
		
		for(iter = pTouches->begin();iter != pTouches->end();++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				multiTouchData.erase((int)touch);
				
				if(multiTouchData.size() == 0)
				{
					timeval time;
					gettimeofday(&time, NULL);
					long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
					CCPoint _spd = ccpMult(ccpSub(location, touchStart_p), 1.f/_time*10000);
					
					float spd_value = sqrtf(powf(_spd.x, 2.f) + powf(_spd.y, 2.f));
					if(isAnimated == false && fabsf(spd_value) > 2)
					{
						moveSpeed_p = _spd;
						this->schedule(schedule_selector(WorldMapScene::moveAnimation));
					}
				}
			}
		}
	}
	
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
	{
		ccTouchesEnded(pTouches, pEvent);
	}
	
	virtual void registerWithTouchDispatcher(void);
};

#endif /* defined(__DGproto__WorldMapScene__) */
