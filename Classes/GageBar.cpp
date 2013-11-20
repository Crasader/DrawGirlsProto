// GageBar.cpp
//

#include "GageBar.h"
#define LZZ_INLINE inline
using namespace std;
GageBar * GageBar::create (string filename, float t_per)
        {
		GageBar* t_gb = new GageBar();
		t_gb->myInit(filename, t_per);
		t_gb->autorelease();
		return t_gb;
	}
void GageBar::setPercentage (float t_p)
        {
		if(t_p > 1.f)
			t_p = 1.f;
		else if(t_p < 0.f)
			t_p = 0.f;
		m_percentage = t_p;
		
		view_rect.size.width = max_length*m_percentage;
	}
void GageBar::actionPercentage (float t_p)
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
void GageBar::setPosition (CCPoint t_p)
        {
		CCSprite::setPosition(t_p);
		view_rect.origin = ccpAdd(ccp(-getContentSize().width/2.f, -getContentSize().height/2.f), getParent()->getPosition());
		view_rect.size.width = max_length*m_percentage;
	}
void GageBar::visit ()
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
void GageBar::startMyAction ()
        {
		is_animated = true;
		action_frame = 0;
		schedule(schedule_selector(GageBar::myAction));
	}
void GageBar::myAction ()
        {
		action_frame++;
		
		a_percentage += tick;
		view_rect.size.width = max_length*a_percentage;
		
		if(action_frame >= 30)
		{
			stopMyAction();
		}
	}
void GageBar::stopMyAction ()
        {
		view_rect.size.width = max_length*m_percentage;
		unschedule(schedule_selector(GageBar::myAction));
		is_animated = false;
	}
void GageBar::myInit (string filename, float t_per)
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
#undef LZZ_INLINE
