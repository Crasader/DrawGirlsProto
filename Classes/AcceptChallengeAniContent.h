//
//  AcceptChallengeAniContent.h
//  DGproto
//
//  Created by 사원3 on 2014. 2. 3..
//
//

#ifndef __DGproto__AcceptChallengeAniContent__
#define __DGproto__AcceptChallengeAniContent__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class AcceptChallengeAniContent : public CCNode
{
public:
	static AcceptChallengeAniContent* create(int t_touch_priority, function<void(CCObject*)> t_selector, string t_profile1, string t_nick1, string t_profile2, string t_nick2)
	{
		AcceptChallengeAniContent* t_ctc = new AcceptChallengeAniContent();
		t_ctc->myInit(t_touch_priority, t_selector, t_profile1, t_nick1, t_profile2, t_nick2);
		t_ctc->autorelease();
		return t_ctc;
	}
	
private:
	int touch_priority;
	function<void(CCObject*)> end_selector;
	
	CCNode* base_node;
	CCLabelTTF* nick1_label;
	CCLabelTTF* nick2_label;
	
	CCParticleSystemQuad* remove_particle;
	
	void startMyAnimation()
	{
		CCMoveTo* t_move = CCMoveTo::create(0.2f, ccp(0,0));
		CCDelayTime* t_delay = CCDelayTime::create(0.5f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AcceptChallengeAniContent::fightAnimation));
		CCSequence* t_seq = CCSequence::create(t_move, t_delay, t_call, NULL);
		base_node->runAction(t_seq);
	}
	
	void fightAnimation()
	{
		CCMoveTo* t_move1_1 = CCMoveTo::create(0.2f, ccp(-40, -40));
		CCDelayTime* t_delay1_1 = CCDelayTime::create(1.f);
		CCMoveTo* t_move1_2 = CCMoveTo::create(0.2f, ccp(-400,-40));
		CCSequence* t_seq1 = CCSequence::create(t_move1_1, t_delay1_1, t_move1_2, NULL);
		nick1_label->runAction(t_seq1);
		
		CCMoveTo* t_move2_1 = CCMoveTo::create(0.2f, ccp(40, -40));
		CCDelayTime* t_delay2_1 = CCDelayTime::create(1.f);
		CCMoveTo* t_move2_2 = CCMoveTo::create(0.2f, ccp(400,-40));
		CCSequence* t_seq2 = CCSequence::create(t_move2_1, t_delay2_1, t_move2_2, NULL);
		nick2_label->runAction(t_seq2);
		
		CCDelayTime* t_delay3_1 = CCDelayTime::create(0.2f);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(AcceptChallengeAniContent::showParticle));
		CCDelayTime* t_delay3_2 = CCDelayTime::create(1.f);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(AcceptChallengeAniContent::hideParticle));
		CCDelayTime* t_delay3_3 = CCDelayTime::create(0.2f);
		CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(AcceptChallengeAniContent::endAction));
		CCSequence* t_seq = CCSequence::create(t_delay3_1, t_call1, t_delay3_2, t_call2, t_delay3_3, t_call3, NULL);
		
		runAction(t_seq);
	}
	
	void showParticle()
	{
		remove_particle = CCParticleSystemQuad::createWithTotalParticles(150);
		remove_particle->setPositionType(kCCPositionTypeRelative);
		remove_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("fever_particle.png"));
		remove_particle->setEmissionRate(5000);
		remove_particle->setAngle(0.0);
		remove_particle->setAngleVar(0.0);
		ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
		remove_particle->setBlendFunc(blendFunc);
		remove_particle->setDuration(-1.f);
		remove_particle->setEmitterMode(kCCParticleModeGravity);
		remove_particle->setStartColor(ccc4f(1.f, 1.f, 1.f, 0.f));
		remove_particle->setStartColorVar(ccc4f(0,0,0,1.f));
		remove_particle->setEndColor(ccc4f(1.f,1.f,0.f,0.f));
		remove_particle->setEndColorVar(ccc4f(0, 0, 0, 1.f));
		remove_particle->setStartSize(8.0);
		remove_particle->setStartSizeVar(5.0);
		remove_particle->setEndSize(20.0);
		remove_particle->setEndSizeVar(10.0);
		remove_particle->setGravity(ccp(39.47f,118.4f));
		remove_particle->setRadialAccel(-671.0);
		remove_particle->setRadialAccelVar(0.0);
		remove_particle->setSpeed(51);
		remove_particle->setSpeedVar(300.0);
		remove_particle->setTangentialAccel(0);
		remove_particle->setTangentialAccelVar(0);
		remove_particle->setTotalParticles(150);
		remove_particle->setLife(0.03);
		remove_particle->setLifeVar(0.1);
		remove_particle->setStartSpin(0.0);
		remove_particle->setStartSpinVar(0.f);
		remove_particle->setEndSpin(360.0);
		remove_particle->setEndSpinVar(0.f);
		remove_particle->setPosVar(ccp(0,40));
		remove_particle->setPosition(ccp(0,-10));
		base_node->addChild(remove_particle);
	}
	
	void hideParticle()
	{
		remove_particle->setAutoRemoveOnFinish(true);
		remove_particle->setDuration(0);
	}
	
	void endAction()
	{
		end_selector(NULL);
	}
	
	void myInit(int t_touch_priority, function<void(CCObject*)> t_selector, string t_profile1, string t_nick1, string t_profile2, string t_nick2);
};

#endif /* defined(__DGproto__AcceptChallengeAniContent__) */
