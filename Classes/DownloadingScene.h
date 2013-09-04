//
//  DownloadingScene.h
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 14..
//
//

#ifndef __DrawingJack__DownloadingScene__
#define __DrawingJack__DownloadingScene__

#include "cocos2d.h"
#include "DataStorageHub.h"
#include "StageImgLoader.h"

using namespace cocos2d;

class ScanningImg : public CCSprite
{
public:
	static ScanningImg* create(const char* filename)
	{
		ScanningImg* t_si = new ScanningImg();
		if(t_si && t_si->initWithFile(filename))
		{
			t_si->myInit();
			t_si->autorelease();
			return t_si;
		}
		CC_SAFE_DELETE(t_si);
		return NULL;
	}
	
	void setPercentage(float t_p)
	{
		if(t_p > 1.f)
			myRect.origin.y = 0;
		else
			myRect.origin.y = 430.f - t_p*430.f;
	}
	
private:
	CCRect myRect;
//	float scaleFactor;
//	float device_rate;
//	float visit_factor;
//	CCSize device_margine;
	
	virtual void visit()
	{
		glEnable(GL_SCISSOR_TEST);
		
		int viewport [4];
		glGetIntegerv (GL_VIEWPORT, viewport);
		CCSize rSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize(); // getSize
		float wScale = viewport[2] / rSize.width;
		float hScale = viewport[3] / rSize.height;
		float x = myRect.origin.x*wScale + viewport[0];
		float y = myRect.origin.y*hScale + viewport[1];
		float w = myRect.size.width*wScale;
		float h = myRect.size.height*hScale;
		glScissor(x,y,w,h);
		
//		glScissor(floor(myRect.origin.x*visit_factor + device_margine.width), floor(myRect.origin.y*visit_factor + device_margine.height),
//				  ceil(myRect.size.width*visit_factor), ceil(myRect.size.height*visit_factor));
		draw();
		
		glDisable(GL_SCISSOR_TEST);
	}
	
	void myInit()
	{
//		scaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
//		device_rate = DataStorageHub::sharedInstance()->device_rate;
//		visit_factor = scaleFactor*device_rate;
//		device_margine = DataStorageHub::sharedInstance()->device_margine;
		
		myRect = CCRectMake(0, 430, 320, 430);
	}
};

class SuccessfulImg : public CCSprite
{
public:
	static SuccessfulImg* create(int t_chapter_number, CCObject* t_after, SEL_CallFunc d_after)
	{
		SuccessfulImg* t_si = new SuccessfulImg();
		t_si->myInit(t_chapter_number, t_after, d_after);
		t_si->autorelease();
		return t_si;
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
//	float scaleFactor;
//	float device_rate;
//	float visit_factor;
//	CCSize device_margine;
	
	int chapter_number;
	CCObject* target_after;
	SEL_CallFunc delegate_after;
	
	int ing_frame;
	float percentage;
	CCSprite* bottom_scanner;
	CCSprite* top_scanner;
	
	CCRect view_rect;
	
	void afterAction()
	{
		(target_after->*delegate_after)();
	}
	
	void startScan()
	{
		ing_frame = 0;
		schedule(schedule_selector(SuccessfulImg::scanning));
	}
	
	void endScan()
	{
		unschedule(schedule_selector(SuccessfulImg::scanning));
		CCDelayTime* t_delay = CCDelayTime::create(0.1f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(SuccessfulImg::afterAction));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		runAction(t_seq);
	}
	
	void scanning()
	{
		ing_frame++;
		
		percentage = ing_frame/90.f;
		view_rect.origin.y = -10.f + percentage*450.f;
		
		bottom_scanner->setPositionY(view_rect.origin.y);
		top_scanner->setPositionY(view_rect.origin.y + view_rect.size.height);
		
		if(ing_frame >= 90)
			endScan();
	}
	
	void myInit(int t_chapter_number, CCObject* t_after, SEL_CallFunc d_after)
	{
//		scaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
//		device_rate = DataStorageHub::sharedInstance()->device_rate;
//		visit_factor = scaleFactor*device_rate;
//		device_margine = DataStorageHub::sharedInstance()->device_margine;
		
		chapter_number = t_chapter_number;
		target_after = t_after;
		delegate_after = d_after;
		CCSprite* t_img = StageImgLoader::sharedInstance()->getLoadedImg(chapter_number, 5);
		initWithTexture(t_img->getTexture());
		setPosition(ccp(160,240));
		
		percentage = 0.f;
		view_rect = CCRectMake(0, -10.f + percentage*450.f, 320, 20);
		
		bottom_scanner = CCSprite::create("down_img_scanner.png");
		bottom_scanner->setPosition(ccp(160,view_rect.origin.y));
		addChild(bottom_scanner);
		top_scanner = CCSprite::create("down_img_scanner.png");
		top_scanner->setPosition(ccp(160,view_rect.origin.y + view_rect.size.height));
		addChild(top_scanner);
		
		ing_frame = 0;
		CCDelayTime* t_delay = CCDelayTime::create(0.3f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(SuccessfulImg::startScan));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		runAction(t_seq);
	}
};

class Downloading : public CCLayer
{
public:
	virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(Downloading);
	
	virtual void onEnter();
	virtual void onExit();
	
private:
	ScanningImg* my_si;
	CCSprite* my_scanner;
	CCSprite* loading_ment;
	bool is_successed;
	
	int chapter_bonusImg_number;
	
	bool isAction;
	
	void ingFrame();
	void successAction();
	void realSuccess();
	void failAction();
	void realFail();
	
	void closeShutter();
	void shutterClosedSound();
	void endCloseShutter();
	
	void alertAction(int t1, int t2);
	
	void myReplaceScene(DownAfterScene a_code);
};

#endif /* defined(__DrawingJack__DownloadingScene__) */
