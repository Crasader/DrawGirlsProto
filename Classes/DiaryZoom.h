//
//  DiaryZoom.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 2..
//
//

#ifndef __DGproto__DiaryZoom__
#define __DGproto__DiaryZoom__

#include "cocos2d.h"
#include "DataStorageHub.h"
#include <deque>

USING_NS_CC;
using namespace std;

class DiaryZoom : public CCLayer
{
public:
	virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(DiaryZoom);
	
	virtual void onEnterTransitionDidFinish();
	
private:
	CCNode* game_node;
	
	int ing_animation_frame;
	
	bool is_touched_menu;
	bool is_actioned;
	
	CCSprite* first_img;
	CCMenu* next_button;
	
	bool is_animation;
	
	float minimum_scale;
	CCSize screen_size;
	
	void menuAction(CCObject* sender);
	
	void startTouchAction();
	
	bool isAnimated;
	CCPoint touch_p;
    long touchStartTime;
	CCPoint touchStart_p;
	CCPoint moveSpeed_p;
	float zoom_base_distance;
	
	map<int, CCPoint> multiTouchData;
	
	CCSize eye_ani_size;
	int loop_length;
	deque<int> animation_frame;
	
	void startStageAnimation()
	{
		ing_animation_frame = 0;
		
		int random_value = rand()%16;
		
		if(random_value >= 2 && random_value <= 4)
			random_value = 7;
		else if(random_value >= 5 && random_value <= 10)
			random_value = 10;
		else if(random_value >= 11 && random_value <= 13)
			random_value = 13;
		else if(random_value == 14 || random_value == 15)
			random_value = 16;
		else
			random_value++;
		
		CCDelayTime* t_delay = CCDelayTime::create(random_value*0.1f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(DiaryZoom::frameAnimation));
		CCAction* t_seq = CCSequence::create(t_delay, t_call, NULL);
		first_img->getChildByTag(1)->runAction(t_seq);
	}
	
	void frameAnimation()
	{
		int loop_point = animation_frame[ing_animation_frame];
		((CCSprite*)first_img->getChildByTag(1))->setTextureRect(CCRectMake(loop_point*eye_ani_size.width, 0, eye_ani_size.width, eye_ani_size.height));
		
		ing_animation_frame++;
		if(ing_animation_frame >= loop_length)
			startStageAnimation();
		else
		{
			CCDelayTime* t_delay = CCDelayTime::create(0.1f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(DiaryZoom::frameAnimation));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			first_img->getChildByTag(1)->runAction(t_seq);
		}
	}
	
	
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
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			multiTouchData[(int)touch] = location;
			
			touch_p = location;
			
			timeval time;
			gettimeofday(&time, NULL);
			touchStartTime = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
			touchStart_p = location;
			
			this->unschedule(schedule_selector(DiaryZoom::moveAnimation));
			
			isAnimated=false;
			
			if(multiTouchData.size() == 1)
			{
				if(!is_touched_menu && next_button->ccTouchBegan(touch, pEvent))
				{
					is_touched_menu = true;
				}
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
				if(is_touched_menu)
				{
					next_button->ccTouchCancelled(touch, pEvent);
					is_touched_menu = false;
				}
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
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				o_it->second = location;
				if(multiTouchData.size() == 1)
				{
					if(is_touched_menu)
					{
						next_button->ccTouchMoved(touch, pEvent);
					}
					
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
					
					float before_scale = game_node->getScale();
					
					float changed_distance = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
					float after_scale = game_node->getScale()*changed_distance/zoom_base_distance;
					if(after_scale > 2.5f)			after_scale = 2.5f;
					else if(after_scale < minimum_scale)		after_scale = minimum_scale;
					zoom_base_distance = changed_distance;
					game_node->setScale(after_scale);
					
					if(game_node->getScale() <= 1.5f)
						game_node->setPositionX((480.f-320.f*game_node->getScale())/2.f);
					else
					{
						float comp_scale = before_scale < 1.5f ? 1.5f : before_scale;
						comp_scale = game_node->getScale() - comp_scale;
						
						game_node->setPositionX(game_node->getPositionX() - 320*comp_scale/2.f);
						
						if(game_node->getPositionX() > 0)
							game_node->setPositionX(0);
						else if(game_node->getPositionX() < 480-320*game_node->getScale())
							game_node->setPositionX(480-320*game_node->getScale());
					}
					
					float comp_scale = before_scale;
					comp_scale = game_node->getScale() - comp_scale;
					
					game_node->setPositionY(game_node->getPositionY() - 430*comp_scale/2.f);
					
					if(game_node->getPositionY() > 0)
						game_node->setPositionY(0);
					else if(game_node->getPositionY() < -430*game_node->getScale()+480*screen_size.height/screen_size.width)
						game_node->setPositionY(-430*game_node->getScale()+480*screen_size.height/screen_size.width);
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
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				multiTouchData.erase((int)touch);
				
				if(multiTouchData.size() == 0)
				{
					if(is_touched_menu)
					{
						next_button->ccTouchEnded(touch, pEvent);
						is_touched_menu = false;
					}
					
					timeval time;
					gettimeofday(&time, NULL);
					long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
					CCPoint _spd = ccpMult(ccpSub(location, touchStart_p), 1.f/_time*10000);
					
					float spd_value = sqrtf(powf(_spd.x, 2.f) + powf(_spd.y, 2.f));
					if(isAnimated == false && fabsf(spd_value) > 2)
					{
						moveSpeed_p = _spd;
						this->schedule(schedule_selector(DiaryZoom::moveAnimation));
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

#endif /* defined(__DGproto__DiaryZoom__) */
