////
////  CumberEmotion.h
////  DrawingJack
////
////  Created by 사원3 on 13. 5. 20..
////
////
//
//#ifndef __DrawingJack__CumberEmotion__
//#define __DrawingJack__CumberEmotion__
//
//#include "cocos2d.h"
//#include "GameData.h"
//
//using namespace cocos2d;
//
//enum EmotionType{
//	kEmotionType_angry = 1,
//	kEmotionType_sad,
//	kEmotionType_huh,
//	kEmotionType_sorry,
//	kEmotionType_joy,
//	kEmotionType_fun,
//	kEmotionType_bored
//};
//
//class EmotionParent : public CCNode
//{
//public:
//	static EmotionParent* create(CCObject* t_mainCumber, SEL_CallFuncI t_main_showEmotion)
//	{
//		EmotionParent* t_ep = new EmotionParent();
//		t_ep->myInit(t_mainCumber, t_main_showEmotion);
//		t_ep->autorelease();
//		return t_ep;
//	}
//	
//	void startCrashAction();
//	
//	void crashed();
//	
//	void stopCrashAction();
//	
//	void set_UI(CCObject* t_ui, SEL_CallFunc k_ui, SEL_CallFunc c_ui);
//	
//	void addJackAttack();
//	
//	void subJackAttack();
//	
//	void setGameover();
//	
//private:
//	int crashedCnt;
//	bool is_crashed;
//	int crashingCnt;
//	
//	bool is_gameover;
//	
//	int attackCnt;
//	
//	CCObject* mainCumber;
//	SEL_CallFuncI main_showEmotion;
//	CCObject* myUI;
//	SEL_CallFunc ui_keepLife;
//	SEL_CallFunc ui_checkLife;
//	
//	int boredCnt;
//	
//	void startBoredCheck();
//	
//	void boredChecking();
//	
//	
//	void stopBoredCheck();
//	
//	void myInit(CCObject* t_mainCumber, SEL_CallFuncI t_main_showEmotion);
//};
//
//class Emotion : public CCSprite
//{
//public:
//	static Emotion* create(EmotionType t_type, CCObject* t_p, SEL_CallFunc d_nullmEmotion)
//	{
//		Emotion* n_e = new Emotion();
//		n_e->myInit(t_type, t_p, d_nullmEmotion);
//		n_e->autorelease();
//		return n_e;
//	}
//	
//	virtual ~Emotion()
//	{
//		(myParent->*delegate_nullmEmotion)();
//	}
//	
//	void selfRemove();
//	
//private:
//	EmotionType mEmotionType;
//	CCObject* myParent;
//	SEL_CallFunc delegate_nullmEmotion;
//	
//	void myInit(EmotionType t_type, CCObject* t_p, SEL_CallFunc d_nullmEmotion);
//};
//
//#endif /* defined(__DrawingJack__CumberEmotion__) */
