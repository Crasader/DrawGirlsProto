//
//  GageBar.h
//  DrawingJack
//
//  Created by 사원3 on 13. 7. 2..
//
//

#ifndef __DrawingJack__GageBar__
#define __DrawingJack__GageBar__

#include "cocos2d.h"
#include "DataStorageHub.h"

USING_NS_CC;
using namespace std;

class GageBar : public CCSprite
{
public:
	static GageBar* create(string filename, float t_per)
	{
		GageBar* t_gb = new GageBar();
		t_gb->myInit(filename, t_per);
		t_gb->autorelease();
		return t_gb;
	}
	
	void setPercentage(float t_p)
	{
		if(t_p > 1.f)
			t_p = 1.f;
		else if(t_p < 0.f)
			t_p = 0.f;
		m_percentage = t_p;
		
		view_rect.size.width = max_length*m_percentage;
	}
	
	void actionPercentage(float t_p)
	{
		if(!is_animated)
			a_percentage = m_percentage;
		
		if(t_p > 1.f)
			t_p = 1.f;
		else if(t_p < 0.f)
			t_p = 0.f;
		m_percentage = t_p;
		
		tick = (m_percentage - a_percentage)/30.f;
		if(!is_animated)
			startMyAction();
	}
	
	virtual void setPosition(CCPoint t_p)
	{
		CCSprite::setPosition(t_p);
		view_rect.origin = ccpAdd(ccp(-getContentSize().width/2.f, -getContentSize().height/2.f), getParent()->getPosition());
		view_rect.size.width = max_length*m_percentage;
	}
	
	virtual void visit()
	{
		glEnable(GL_SCISSOR_TEST);

		int viewport [4];
		glGetIntegerv (GL_VIEWPORT, viewport);
		CCSize rSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize(); // getSize
		float wScale = viewport[2] / rSize.width;
		float hScale = viewport[3] / rSize.height;
		float x = view_rect.origin.x*wScale + viewport[0];
		float y = view_rect.origin.y*hScale + viewport[1];
		float w = view_rect.size.width*wScale;
		float h = view_rect.size.height*hScale;
		glScissor(x,y,w,h);
		
//		glScissor(floor(view_rect.origin.x*visit_factor + device_margine.width), floor(view_rect.origin.y*visit_factor + device_margine.height),
//				  ceil(view_rect.size.width*visit_factor), ceil(view_rect.size.height*visit_factor));
		CCSprite::visit();
		
		glDisable(GL_SCISSOR_TEST);
	}
	
private:
	CCRect view_rect;
	float m_percentage;
	float a_percentage;
	float tick;
	bool is_animated;
//	float scaleFactor;
//	float device_rate;
//	float visit_factor;
//	CCSize device_margine;
	
	float max_length;
	
	int action_frame;
	
	void startMyAction()
	{
		is_animated = true;
		action_frame = 0;
		schedule(schedule_selector(GageBar::myAction));
	}
	
	void myAction()
	{
		action_frame++;
		
		a_percentage += tick;
		view_rect.size.width = max_length*a_percentage;
		
		if(action_frame >= 30)
		{
			stopMyAction();
		}
	}
	
	void stopMyAction()
	{
		view_rect.size.width = max_length*m_percentage;
		unschedule(schedule_selector(GageBar::myAction));
		is_animated = false;
	}
	
	void myInit(string filename, float t_per)
	{
		is_animated = false;
//		scaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
//		device_rate = DataStorageHub::sharedInstance()->device_rate;
//		visit_factor = scaleFactor*device_rate;
//		device_margine = DataStorageHub::sharedInstance()->device_margine;
		
		initWithFile(filename.c_str());
		max_length = getContentSize().width;
		view_rect = boundingBox();
		setPercentage(t_per);
	}
};

#endif /* defined(__DrawingJack__GageBar__) */
