//
//  OtherEffect.h
//  DrawingJack
//
//  Created by 사원3 on 13. 2. 22..
//
//

#ifndef DrawingJack_OtherEffect_h
#define DrawingJack_OtherEffect_h

class CircleNode : public CCSprite
{
public:
	static CircleNode* create(const char* imgFilename)
	{
		CircleNode* t_cn = new CircleNode();
		t_cn->myInit(imgFilename);
		t_cn->autorelease();
		return t_cn;
	}
	
	void startSmall()
	{
		CCScaleTo* t_scale = CCScaleTo::create(0.3, 0.2);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(CircleNode::selfRemove));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		runAction(t_seq);
	}
	
private:
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void myInit(const char* imgFilename)
	{
		initWithFile(imgFilename);
	}
};

class CircleCreater : public CCNode
{
public:
	static CircleCreater* create(ccColor3B t_color, int t_createFrame)
	{
		CircleCreater* t_cc = new CircleCreater();
		t_cc->myInit(t_color, t_createFrame);
		t_cc->autorelease();
		return t_cc;
	}
	
	void stopCreate()
	{
		unschedule(schedule_selector(CircleCreater::createFraming));
		startSelfRemoveSchedule();
	}
	
private:
	ccColor3B myColor;
	int createFrame;
	int ingFrame;
	
	void startSelfRemoveSchedule()
	{
		schedule(schedule_selector(CircleCreater::selfRemoveSchedule));
	}
	
	void selfRemoveSchedule()
	{
		if(getChildrenCount() == 0)
		{
			removeFromParentAndCleanup(true);
		}
	}
	
	void startCreate()
	{
		ingFrame = 0;
		schedule(schedule_selector(CircleCreater::createFraming));
	}
	
	void createFraming()
	{
		ingFrame++;
		
		if(ingFrame%createFrame == 0)
		{
			CircleNode* t_cn = CircleNode::create("sleep_circle.png");
			t_cn->setColor(myColor);
			addChild(t_cn);
			t_cn->startSmall();
		}
	}
	
	void myInit(ccColor3B t_color, int t_createFrame)
	{
		myColor = t_color;
		createFrame = t_createFrame;
		
		startCreate();
	}
};

class SpeedChangeEffect : public CCSprite
{
public:
	static SpeedChangeEffect* create(bool is_up)
	{
		SpeedChangeEffect* t_sue = new SpeedChangeEffect();
		if(t_sue)
		{
			t_sue->myInit(is_up);
			t_sue->autorelease();
			return t_sue;
		}
		CC_SAFE_DELETE(t_sue);
		return NULL;
	}
	
	void startAction()
	{
		if(is_up)
		{
			CCSprite* t_texture = CCSprite::create("fast_effect.png");
			CCAnimation* t_animation = CCAnimation::create();
			t_animation->setDelayPerUnit(0.1f);
			for(int i=0;i<4;i++)
				t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(i*25, 0, 25, 54));
			
			CCAnimate* t_animate = CCAnimate::create(t_animation);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(SpeedChangeEffect::selfRemove));
			
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_animate, t_call);
			
			runAction(t_seq);
		}
		else
		{
			t_cc = CircleCreater::create(ccYELLOW, 12); // purple 153,0,255
			addChild(t_cc);
			CCDelayTime* t_delay = CCDelayTime::create(1.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(SpeedChangeEffect::stopCreate));
			
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			
			runAction(t_seq);
		}
	}
	
private:
	bool is_up;
	CircleCreater* t_cc;
	
	void stopCreate()
	{
		t_cc->stopCreate();
		schedule(schedule_selector(SpeedChangeEffect::removableCheck));
	}
	
	void removableCheck()
	{
		if(getChildrenCount() == 0)
		{
			unschedule(schedule_selector(SpeedChangeEffect::removableCheck));
			selfRemove();
		}
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void myInit(bool t_is_up)
	{
		is_up = t_is_up;
		
		if(is_up)
		{
			CCSprite* t_texture = CCSprite::create("fast_effect.png");
			initWithTexture(t_texture->getTexture(), CCRectMake(0, 0, 25, 54));
		}
		else
		{
			init();
		}
	}
};

#endif
