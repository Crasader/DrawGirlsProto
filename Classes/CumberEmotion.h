//
//  CumberEmotion.h
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 20..
//
//

#ifndef __DrawingJack__CumberEmotion__
#define __DrawingJack__CumberEmotion__

#include "cocos2d.h"
#include "GameData.h"

using namespace cocos2d;

enum EmotionType{
	kEmotionType_angry = 1,
	kEmotionType_sad,
	kEmotionType_huh,
	kEmotionType_sorry,
	kEmotionType_joy,
	kEmotionType_fun,
	kEmotionType_bored
};

class EmotionParent : public CCNode
{
public:
	static EmotionParent* create(CCObject* t_mainCumber, SEL_CallFuncI t_main_showEmotion)
	{
		EmotionParent* t_ep = new EmotionParent();
		t_ep->myInit(t_mainCumber, t_main_showEmotion);
		t_ep->autorelease();
		return t_ep;
	}
	
	void startCrashAction()
	{
		if(crashingCnt == 0)
		{
			is_crashed = false;
			crashedCnt = 0;
		}
		crashingCnt++;
	}
	
	void crashed()
	{
		is_crashed = true;
		crashedCnt++;
	}
	
	void stopCrashAction()
	{
		crashingCnt--;
		
		if(crashingCnt == 0)
		{
			if(!is_crashed)
			{
				(mainCumber->*main_showEmotion)(kEmotionType_sorry);
			}
			else if(crashedCnt >= 700)
			{
				(mainCumber->*main_showEmotion)(kEmotionType_fun);
			}
		}
	}
	
	void set_UI(CCObject* t_ui, SEL_CallFunc k_ui, SEL_CallFunc c_ui)
	{
		myUI = t_ui;
		ui_keepLife = k_ui;
		ui_checkLife = c_ui;
	}
	
	void addJackAttack()
	{
		if(attackCnt == 0)
		{
			stopBoredCheck();
			(myUI->*ui_keepLife)();
		}
		attackCnt++;
	}
	
	void subJackAttack()
	{
		attackCnt--;
		if(attackCnt == 0)
		{
			(myUI->*ui_checkLife)();
			startBoredCheck();
		}
	}
	
	void setGameover()
	{
		is_gameover = true;
		stopBoredCheck();
	}
	
private:
	int crashedCnt;
	bool is_crashed;
	int crashingCnt;
	
	bool is_gameover;
	
	int attackCnt;
	
	CCObject* mainCumber;
	SEL_CallFuncI main_showEmotion;
	CCObject* myUI;
	SEL_CallFunc ui_keepLife;
	SEL_CallFunc ui_checkLife;
	
	int boredCnt;
	
	void startBoredCheck()
	{
		if(!is_gameover)
		{
			boredCnt = 0;
			schedule(schedule_selector(EmotionParent::boredChecking), 1.f);
		}
	}
	
	void boredChecking()
	{
		boredCnt++;
		if(boredCnt > 20)
		{
			boredCnt = 0;
			(mainCumber->*main_showEmotion)(7);
		}
	}
	
	
	void stopBoredCheck()
	{
		unschedule(schedule_selector(EmotionParent::boredChecking));
	}
	
	void myInit(CCObject* t_mainCumber, SEL_CallFuncI t_main_showEmotion)
	{
		CCTextureCache::sharedTextureCache()->addImage("emotion_angry.png");
		CCTextureCache::sharedTextureCache()->addImage("emotion_bored.png");
		CCTextureCache::sharedTextureCache()->addImage("emotion_fun.png");
		CCTextureCache::sharedTextureCache()->addImage("emotion_huh.png");
		CCTextureCache::sharedTextureCache()->addImage("emotion_joy.png");
		CCTextureCache::sharedTextureCache()->addImage("emotion_sad.png");
		CCTextureCache::sharedTextureCache()->addImage("emotion_sorry.png");
		
		is_gameover = false;
		attackCnt = 0;
		myUI = NULL;
		crashedCnt = 0;
		crashingCnt = 0;
		mainCumber = t_mainCumber;
		main_showEmotion = t_main_showEmotion;
		
//		GameData::sharedGameData()->regEP(this, callfunc_selector(EmotionParent::startCrashAction),
//										  callfunc_selector(EmotionParent::crashed),
//										  callfunc_selector(EmotionParent::stopCrashAction),
//										  callfunc_selector(EmotionParent::addJackAttack),
//										  callfunc_selector(EmotionParent::subJackAttack));
		
		GameData* myGD = GameData::sharedGameData();
		myGD->V_V["EP_startCrashAction"] = std::bind(&EmotionParent::startCrashAction, this);
		myGD->V_V["EP_crashed"] = std::bind(&EmotionParent::crashed, this);
		myGD->V_V["EP_stopCrashAction"] = std::bind(&EmotionParent::stopCrashAction, this);
		myGD->V_V["EP_addJackAttack"] = std::bind(&EmotionParent::addJackAttack, this);
		myGD->V_V["EP_subJackAttack"] = std::bind(&EmotionParent::subJackAttack, this);
		startBoredCheck();
	}
};

class Emotion : public CCSprite
{
public:
	static Emotion* create(EmotionType t_type, CCObject* t_p, SEL_CallFunc d_nullmEmotion)
	{
		Emotion* n_e = new Emotion();
		n_e->myInit(t_type, t_p, d_nullmEmotion);
		n_e->autorelease();
		return n_e;
	}
	
	virtual ~Emotion()
	{
		(myParent->*delegate_nullmEmotion)();
	}
	
	void selfRemove()
	{
		(myParent->*delegate_nullmEmotion)();
		removeFromParentAndCleanup(true);
	}
	
private:
	EmotionType mEmotionType;
	CCObject* myParent;
	SEL_CallFunc delegate_nullmEmotion;
	
	void myInit(EmotionType t_type, CCObject* t_p, SEL_CallFunc d_nullmEmotion)
	{
		myParent = t_p;
		delegate_nullmEmotion = d_nullmEmotion;
		mEmotionType = t_type;
		CCTexture2D* t_texture;
		if(mEmotionType == kEmotionType_angry)
		{
			t_texture = CCTextureCache::sharedTextureCache()->addImage("emotion_angry.png");
			
			initWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			
			CCAnimation* t_animation = CCAnimation::create();
			t_animation->setDelayPerUnit(0.1);
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(45, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(90, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(90, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(135, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(180, 0, 45, 30));
			CCAnimate* t_animate = CCAnimate::create(t_animation);
			CCFadeOut* t_fadeout = CCFadeOut::create(0.5f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Emotion::selfRemove));
			
			CCSequence* t_seq1 = CCSequence::createWithTwoActions(t_animate, t_fadeout);
			CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_seq1, t_call);
			
			runAction(t_seq2);
		}
		else if(mEmotionType == kEmotionType_sad)
		{
			t_texture = CCTextureCache::sharedTextureCache()->addImage("emotion_sad.png");
			initWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			
			CCAnimation* t_animation = CCAnimation::create();
			t_animation->setDelayPerUnit(0.1);
			for(int i=0;i<4;i++)
				t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*45, 0, 45, 30));
			for(int i=0;i<4;i++)
				t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*45, 0, 45, 30));
			CCAnimate* t_animate = CCAnimate::create(t_animation);
			CCFadeOut* t_fadeout = CCFadeOut::create(1.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Emotion::selfRemove));
			
			CCSequence* t_seq1 = CCSequence::createWithTwoActions(t_animate, t_fadeout);
			CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_seq1, t_call);
			
			runAction(t_seq2);
		}
		else if(mEmotionType == kEmotionType_huh)
		{
			t_texture = CCTextureCache::sharedTextureCache()->addImage("emotion_huh.png");
			initWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			
			CCAnimation* t_animation = CCAnimation::create();
			t_animation->setDelayPerUnit(0.1);
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(45, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(90, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(45, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(45, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(90, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(45, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(135, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(180, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(135, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(180, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(135, 0, 45, 30));
			CCAnimate* t_animate = CCAnimate::create(t_animation);
			CCFadeOut* t_fadeout = CCFadeOut::create(0.5f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Emotion::selfRemove));
			
			CCSequence* t_seq1 = CCSequence::createWithTwoActions(t_animate, t_fadeout);
			CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_seq1, t_call);
			
			runAction(t_seq2);
		}
		else if(mEmotionType == kEmotionType_sorry)
		{
			t_texture = CCTextureCache::sharedTextureCache()->addImage("emotion_sorry.png");
			
			initWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			
			CCAnimation* t_animation = CCAnimation::create();
			t_animation->setDelayPerUnit(0.1);
			for(int i=0;i<4;i++)
				t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*45, 0, 45, 30));
			CCAnimate* t_animate = CCAnimate::create(t_animation);
			CCFadeOut* t_fadeout = CCFadeOut::create(2.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Emotion::selfRemove));
			
			CCSequence* t_seq1 = CCSequence::createWithTwoActions(t_animate, t_fadeout);
			CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_seq1, t_call);
			
			runAction(t_seq2);
		}
		else if(mEmotionType == kEmotionType_joy)
		{
			t_texture = CCTextureCache::sharedTextureCache()->addImage("emotion_joy.png");
			
			initWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			
			CCAnimation* t_animation = CCAnimation::create();
			t_animation->setDelayPerUnit(0.1);
			for(int i=0;i<4;i++)
				t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*45, 0, 45, 30));
			for(int i=0;i<4;i++)
				t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*45, 0, 45, 30));
			CCAnimate* t_animate = CCAnimate::create(t_animation);
			CCFadeOut* t_fadeout = CCFadeOut::create(0.5f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Emotion::selfRemove));
			
			CCSequence* t_seq1 = CCSequence::createWithTwoActions(t_animate, t_fadeout);
			CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_seq1, t_call);
			
			runAction(t_seq2);
		}
		else if(mEmotionType == kEmotionType_fun)
		{
			t_texture = CCTextureCache::sharedTextureCache()->addImage("emotion_fun.png");
			
			initWithTexture(t_texture, CCRectMake(45, 0, 45, 30));
			
			CCAnimation* t_animation = CCAnimation::create();
			t_animation->setDelayPerUnit(0.1);
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(45, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(90, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(90, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(90, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(45, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(90, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(90, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(90, 0, 45, 30));
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			CCAnimate* t_animate = CCAnimate::create(t_animation);
			CCFadeOut* t_fadeout = CCFadeOut::create(0.5f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Emotion::selfRemove));
			
			CCSequence* t_seq1 = CCSequence::createWithTwoActions(t_animate, t_fadeout);
			CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_seq1, t_call);
			
			runAction(t_seq2);
		}
		else if(mEmotionType == kEmotionType_bored)
		{
			t_texture = CCTextureCache::sharedTextureCache()->addImage("emotion_bored.png");
			
			initWithTexture(t_texture, CCRectMake(0, 0, 45, 30));
			
			CCAnimation* t_animation = CCAnimation::create();
			t_animation->setDelayPerUnit(0.1);
			for(int i=0;i<5;i++)
				t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*45, 0, 45, 30));
			for(int i=2;i<5;i++)
				t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*45, 0, 45, 30));
			for(int i=1;i>=0;i--)
				t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*45, 0, 45, 30));
			CCAnimate* t_animate = CCAnimate::create(t_animation);
			CCFadeOut* t_fadeout = CCFadeOut::create(1.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Emotion::selfRemove));
			
			CCSequence* t_seq1 = CCSequence::createWithTwoActions(t_animate, t_fadeout);
			CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_seq1, t_call);
			
			runAction(t_seq2);
		}
	}
};

#endif /* defined(__DrawingJack__CumberEmotion__) */
