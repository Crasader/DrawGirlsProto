//
//  GoodsLight.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 1..
//
//

#ifndef __DGproto__GoodsLight__
#define __DGproto__GoodsLight__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "KSUtil.h"
#include "DataStorageHub.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class GoodsLight : public CCNode
{
public:
	static GoodsLight* create(CCSprite* t_target)
	{
		GoodsLight* t_gl = new GoodsLight();
		t_gl->myInit(t_target);
		t_gl->autorelease();
		return t_gl;
	}
	
private:
	
	CCSprite* t_light;
	
	void myInit(CCSprite* t_target)
	{
		CCClippingNode* main_clipping = CCClippingNode::create(CCSprite::createWithTexture(t_target->getTexture()));
		main_clipping->setPosition(ccp(0,0));
		addChild(main_clipping);
		
//		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//		if(screen_scale_x < 1.f)
//			screen_scale_x = 1.f;
//		
//		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//		
//		float change_scale = 1.f;
//		CCPoint change_origin = ccp(0,0);
//		if(screen_scale_x > 1.f)
//		{
//			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
//			change_scale = screen_scale_x;
//		}
//		if(screen_scale_y > 1.f)
//			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
//		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		main_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		
		main_clipping->setAlphaThreshold(0.1f);
		
		t_light = CCSprite::create("mainflow_top_light.png");
		t_light->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		t_light->setOpacity(100);
		main_clipping->addChild(t_light);
		
		addMoving();
	}
	void addMoving()
	{
		addChild(KSGradualValue<float>::create(-50, 50, 1.f, [=](float t){
			t_light->setPositionX(t);
		}, [=](float t){
			t_light->setPositionX(50);
			addMoving();
		}));
	}
};

#endif /* defined(__DGproto__GoodsLight__) */
