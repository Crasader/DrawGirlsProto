//
//  MainCumberKS.h
//  DGproto
//
//  Created by ksoo k on 13. 9. 4..
//
//

#ifndef __DGproto__MainCumberKS__
#define __DGproto__MainCumberKS__
#include "cocos2d.h"
#include "CumberBase.h"
#include "AudioEngine.h"
#include "AlertEngine.h"
#include "CumberEmotion.h"
#include "BossEye.h"
#include <algorithm>
#include <vector>
#include <map>
#include <queue>


class MainCumberKS : public Cumber
{
public:
	static MainCumberKS* create()
	{
		MainCumberKS* t_MC = new MainCumberKS();
		t_MC->myInit();
		t_MC->autorelease();
		return t_MC;
	}
	
	void cumberImgStartRotating(float speed);
	void checkingJackCrash();
	void startMoving();
	void stopMoving();
	void setCumberState(int t_cs);
	cumberState getCumberState();
	void setGameover();
	void tickingOn();
	void tickingOff();
	void startTeleport();
	void smaller();
	void changePosition();
	void lightSmaller();
	void endTeleport();
	void createSheild();
	void crashSheild();
	void startInvisible();
	void stopInvisible();
	void silenceItem(bool t_b);

	void setCasting(bool t_b);
	void showEmotion(EmotionType t_type);
	void nullmEmotion();
	CCNode* getBossEye();
	void caughtBoss(CCObject* t_setCaught, SEL_CallFunc d_setCaught);
	void endCaughtBoss();
	void changeMaxSize(float t_p);
	
private:
	CCObject* target_setCaught;
	SEL_CallFunc delegate_setCaught;
	
	GameData* myGD;
	bool m_isGameover;
	int m_furyFrame;
	CCParticleSystemQuad* m_furyMode;
	bool m_ableTickingTimeBomb;
	int m_tickingFrame;
	CCSprite* m_tickingImg;
	CCSprite* m_teleportImg;
	SilenceEffect* m_silenceImg;
	int m_invisibleFrame;
	int m_add_value;
	int m_zero_cnt;
	bool m_is_silenced;
	bool m_is_casting;
	CCPoint m_b_c_p;
	BossEye* m_my_eye;
	
	//	int t_chapter_number;
	//	int t_stage_number;
	
	bool m_is_attackOn;
	
	int m_attack_rate;
	int m_noattack_cnt;
	
	int m_before_keep_frame_cnt;
	map<int,IntPoint> m_before_points;
	int m_keep_number;
	
	int m_map_visit_cnt;
	int m_max_visit_cnt;
	
	Emotion* m_mEmotion;
	
	float m_dash_angle;
	int checking_array[360];
	
	void alertAction(int t1, int t2);
	void moving(float dt); // 스케쥴러!
	
	void invisibling(float dt); // 스케쥴러!
	
	
	void crashMapForIntPoint(IntPoint t_p);
	void myInit();
	void randomShuffle();
	
	/// 해당 위치가 적절한지 ... 
	bool checkingFunc(IntPoint check_position, bool* is_not_position, bool* notEmotion, int* before_areacrash_cnt, bool* is_map_visited);
};



#endif /* defined(__DGproto__MainCumberKS__) */
