//
//  StageNode.h
//  DGproto
//
//  Created by 사원3 on 2014. 3. 12..
//
//

#ifndef __DGproto__StageNode__
#define __DGproto__StageNode__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum StageNodeViewMode
{
	kStageNodeViewMode_open = 0,
	kStageNodeViewMode_buy,
	kStageNodeViewMode_lock
};

class StageNode : public CCNode
{
public:
	static StageNode* create(int t_puzzle_number, int t_stage_number, CCObject* t_target, SEL_MenuHandler t_delegate, function<void(int)> t_destroy_func)
	{
		StageNode* t_sn = new StageNode();
		t_sn->myInit(t_puzzle_number, t_stage_number, t_target, t_delegate, t_destroy_func);
		t_sn->autorelease();
		return t_sn;
	}
	
	virtual ~StageNode()
	{
		destroy_func(stage_number);
	}
	
	void startUnlockAnimation(function<void(void)> t_end_animation)
	{
		setViewMode(kStageNodeViewMode_open);
		
		CCParticleSystemQuad* t_particle = CCParticleSystemQuad::createWithTotalParticles(150);
		t_particle->setPositionType(kCCPositionTypeRelative);
		t_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("get_piece_particle.png"));
		t_particle->setEmissionRate(800);
		t_particle->setAngle(90.0);
		t_particle->setAngleVar(45.0);
		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
		t_particle->setBlendFunc(blendFunc);
		t_particle->setDuration(0.1);
		t_particle->setEmitterMode(kCCParticleModeGravity);
		t_particle->setStartColor(ccc4f(1.f, 1.f, 1.f, 1.f));
		t_particle->setStartColorVar(ccc4f(0.57f, 0.57f, 0.54f, 0.f));
		t_particle->setEndColor(ccc4f(1.f, 1.f, 1.f, 0.f));
		t_particle->setEndColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
		t_particle->setStartSize(10.0);
		t_particle->setStartSizeVar(5.0);
		t_particle->setEndSize(20.0);
		t_particle->setEndSizeVar(5.0);
		t_particle->setGravity(ccp(0,-400));
		t_particle->setRadialAccel(0.0);
		t_particle->setRadialAccelVar(0.0);
		t_particle->setSpeed(150);
		t_particle->setSpeedVar(70.0);
		t_particle->setTangentialAccel(0);
		t_particle->setTangentialAccelVar(0);
		t_particle->setTotalParticles(150);
		t_particle->setLife(0.40);
		t_particle->setLifeVar(0.5);
		t_particle->setStartSpin(0);
		t_particle->setStartSpinVar(180);
		t_particle->setEndSpin(0);
		t_particle->setEndSpinVar(180);
		t_particle->setPosVar(ccp(15,15));
		t_particle->setPosition(ccp(0,0));
		t_particle->setAutoRemoveOnFinish(true);
		addChild(t_particle);
		
		t_end_animation();
	}
	
	void startGetStarAnimation(function<void(void)> t_end_animation)
	{
		keep_func = t_end_animation;
		
		int star_count = star_vector.size();
		
		if(star_count == 1)
		{
			CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 1.5f);
			CCScaleTo* t_scale2 = CCScaleTo::create(0.3f, 1.f);
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale1, t_scale2);
			CCRotateBy* t_rotate = CCRotateBy::create(0.6f, 360);
			CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_seq, t_rotate);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StageNode::endGetStarAnimation));
			CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_spawn, t_call);
			star_vector[0]->runAction(t_seq2);
		}
		else if(star_count == 2)
		{
			CCScaleTo* t_scale1 = CCScaleTo::create(0.2f, 1.5f);
			CCScaleTo* t_scale2 = CCScaleTo::create(0.2f, 1.f);
			CCSequence* t_seq1 = CCSequence::createWithTwoActions(t_scale1, t_scale2);
			CCRotateBy* t_rotate1 = CCRotateBy::create(0.4f, 360);
			CCSpawn* t_spawn1 = CCSpawn::createWithTwoActions(t_seq1, t_rotate1);
			star_vector[0]->runAction(t_spawn1);
			
			CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
			
			CCScaleTo* t_scale3 = CCScaleTo::create(0.2f, 1.5f);
			CCScaleTo* t_scale4 = CCScaleTo::create(0.2f, 1.f);
			CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_scale3, t_scale4);
			CCRotateBy* t_rotate2 = CCRotateBy::create(0.4f, 360);
			CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_seq2, t_rotate2);
			
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StageNode::endGetStarAnimation));
			
			CCSequence* t_seq3 = CCSequence::create(t_delay1, t_spawn2, t_call, NULL);
			star_vector[1]->runAction(t_seq3);
		}
		else if(star_count == 3)
		{
			CCScaleTo* t_scale1 = CCScaleTo::create(0.2f, 1.5f);
			CCScaleTo* t_scale2 = CCScaleTo::create(0.2f, 1.f);
			CCSequence* t_seq1 = CCSequence::createWithTwoActions(t_scale1, t_scale2);
			CCRotateBy* t_rotate1 = CCRotateBy::create(0.4f, 360);
			CCSpawn* t_spawn1 = CCSpawn::createWithTwoActions(t_seq1, t_rotate1);
			star_vector[0]->runAction(t_spawn1);
			
			CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
			
			CCScaleTo* t_scale3 = CCScaleTo::create(0.2f, 1.5f);
			CCScaleTo* t_scale4 = CCScaleTo::create(0.2f, 1.f);
			CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_scale3, t_scale4);
			CCRotateBy* t_rotate2 = CCRotateBy::create(0.4f, 360);
			CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_seq2, t_rotate2);
			
			CCSequence* t_seq3 = CCSequence::create(t_delay1, t_spawn2, NULL);
			star_vector[1]->runAction(t_seq3);
			
			CCDelayTime* t_delay2 = CCDelayTime::create(0.4f);
			
			CCScaleTo* t_scale5 = CCScaleTo::create(0.2f, 1.5f);
			CCScaleTo* t_scale6 = CCScaleTo::create(0.2f, 1.f);
			CCSequence* t_seq4 = CCSequence::createWithTwoActions(t_scale5, t_scale6);
			CCRotateBy* t_rotate3 = CCRotateBy::create(0.4f, 360);
			CCSpawn* t_spawn3 = CCSpawn::createWithTwoActions(t_seq4, t_rotate3);
			
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StageNode::endGetStarAnimation));
			
			CCSequence* t_seq5 = CCSequence::create(t_delay2, t_spawn3, t_call, NULL);
			star_vector[2]->runAction(t_seq5);
		}
	}
	
	void setViewMode(StageNodeViewMode t_mode);
	
private:
	
	int puzzle_number;
	int stage_number;
	int piece_no;
	CCObject* menu_target;
	SEL_MenuHandler menu_delegate;
	StageNodeViewMode recent_mode;
	function<void(int)> destroy_func;
	vector<CCSprite*> star_vector;
	function<void(void)> keep_func;
	
	void endGetStarAnimation()
	{
		keep_func();
	}
	
	void myInit(int t_puzzle_number, int t_stage_number, CCObject* t_target, SEL_MenuHandler t_delegate, function<void(int)> t_destroy_func);
};

#endif /* defined(__DGproto__StageNode__) */
