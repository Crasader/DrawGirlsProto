// TakeCardPopup.cpp
//

#include "TakeCardPopup.h"
#define LZZ_INLINE inline
using namespace std;

#include "CardCase.h"
#include "StageImgLoader.h"
TakeCardPopup * TakeCardPopup::create (int t_stage, int t_grade, CCObject* t_end_call, SEL_CallFunc d_end_call)
{
	TakeCardPopup* t_cp = new TakeCardPopup();
	t_cp->myInit(t_stage, t_grade, t_end_call, d_end_call);
	t_cp->autorelease();
	return t_cp;
}
void TakeCardPopup::myInit (int t_stage, int t_grade, CCObject* t_end_call, SEL_CallFunc d_end_call)
{
	target_end_call = t_end_call;
	delegate_end_call = d_end_call;
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	gray->setOpacity(0);
	addChild(gray, kTakeCardPopup_Z_gray);
	
	gray->runAction(CCFadeTo::create(0.4f, 255));
	
	take_label = CCSprite::create("ending_take_card.png");
	take_label->setPosition(ccp(240,280));
	take_label->setOpacity(0);
	addChild(take_label, kTakeCardPopup_Z_img);
	
	take_label->runAction(CCFadeTo::create(0.4f, 255));
	
	take_card = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", t_stage, t_grade)->getCString());
	take_card->setScale(0.5f);
	take_card->setPosition(ccp(600,130));
	addChild(take_card, kTakeCardPopup_Z_img);
	
	if(t_grade == 3 && mySD->isAnimationStage())
	{
		CCSize ani_size = mySD->getAnimationCutSize();
		CCSprite* take_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", t_stage, t_grade)->getCString(), CCRectMake(0, 0, ani_size.width, ani_size.height));
		take_ani->setPosition(mySD->getAnimationPosition());
		take_card->addChild(take_ani);
	}
	
	CardCase* t_case = CardCase::create(t_stage, t_grade);
	t_case->setPosition(CCPointZero);
	take_card->addChild(t_case);
	
	CCLabelTTF* recent_durability_label = t_case->getRecentDurabilityLabel();
	recent_durability_label->setString(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_durability_i, NSDS_GI(t_stage, kSDS_SI_level_int1_card_i, t_grade)))->getCString());
	recent_durability_label->setPosition(ccpAdd(recent_durability_label->getPosition(), ccp(6,-1)));
	recent_durability_label->setFontSize(recent_durability_label->getFontSize()+3);
	
	t_case->getTotalDurabilityLabel()->removeFromParent();
	
	card_back = CCSprite::create("ending_take_card_back.png");
	card_back->setVisible(false);
	card_back->setScale(take_card->getScale());
	card_back->setPosition(take_card->getPosition());
	addChild(card_back, kTakeCardPopup_Z_img);
	
	is_touch_enable = false;
	
	setTouchEnabled(true);
	
	runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.2f), CCCallFunc::create(this, callfunc_selector(TakeCardPopup::openingAction))));
}

void TakeCardPopup::endCall()
{
	(target_end_call->*delegate_end_call)();
	removeFromParent();
}

void TakeCardPopup::openingAction ()
{
	CCMoveTo* t_move = CCMoveTo::create(0.8f, ccp(240,130));
	
	CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.1f, 0.2f, 0, 0, 90, 0, 0);// float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX
	CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::changeBack));
	CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.1f, 0.2f, 0, -90, 90, 0, 0);
	CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::changeTakeCard));
	CCSequence* t_seq = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
	CCRepeat* t_repeat = CCRepeat::create(t_seq, 4);
	CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::touchOn));
	CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_repeat, t_call3);
	CCSpawn* t_spawn1 = CCSpawn::create(t_move, t_seq2, NULL);
	take_card->runAction(t_spawn1);
	
	
	CCMoveTo* t_move2 = CCMoveTo::create(0.8f, ccp(240,130));
	CCOrbitCamera* t_orbit3 = CCOrbitCamera::create(0.1f, 0.2f, 0, 0, 90, 0, 0);
	CCOrbitCamera* t_orbit4 = CCOrbitCamera::create(0.1f, 0.2f, 0, -90, 90, 0, 0);
	CCSequence* t_seq3 = CCSequence::create(t_orbit3, t_orbit4, NULL);
	CCRepeat* t_repeat2 = CCRepeat::create(t_seq3, 4);
	CCSpawn* t_spawn2 = CCSpawn::create(t_move2, t_repeat2, NULL);
	card_back->runAction(t_spawn2);
}
void TakeCardPopup::changeBack ()
{
	card_back->setVisible(true);
	take_card->setVisible(false);
}
void TakeCardPopup::changeTakeCard ()
{
	take_card->setVisible(true);
	card_back->setVisible(false);
}
void TakeCardPopup::touchOn ()
{
	is_touch_enable = true;
	
	star_particle = CCParticleSystemQuad::createWithTotalParticles(10);
	star_particle->setPositionType(kCCPositionTypeRelative);
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("ending_take_particle.png");
	star_particle->setTexture(texture);
	star_particle->setEmissionRate(13);
	star_particle->setAngle(90.0);
	star_particle->setAngleVar(5.0);
	ccBlendFunc blendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
	star_particle->setBlendFunc(blendFunc);
	star_particle->setDuration(-1.0);
	star_particle->setEmitterMode(kCCParticleModeGravity);
	star_particle->setStartColor(ccc4f(1.f, 1.f, 1.f, 1.f));
	star_particle->setStartColorVar(ccc4f(0, 0, 0, 0));
	star_particle->setEndColor(ccc4f(1.f, 1.f, 1.f, 0));
	star_particle->setEndColorVar(ccc4f(0, 0, 0, 0));
	star_particle->setStartSize(30.0);
	star_particle->setStartSizeVar(10.0);
	star_particle->setEndSize(-1.0);
	star_particle->setEndSizeVar(0.0);
	star_particle->setGravity(ccp(0,0));
	star_particle->setRadialAccel(0.0);
	star_particle->setRadialAccelVar(0.0);
	star_particle->setSpeed(60);
	star_particle->setSpeedVar(20.0);
	star_particle->setTangentialAccel(0);
	star_particle->setTangentialAccelVar(0);
	star_particle->setLife(1.f);
	star_particle->setLifeVar(0.5f);
	star_particle->setStartSpin(0.0);
	star_particle->setStartSpinVar(180.f);
	star_particle->setEndSpin(0.0);
	star_particle->setEndSpinVar(0.f);
	star_particle->setPosition(ccp(240,130));
	star_particle->setPosVar(ccp(60,60));
	star_particle->setAutoRemoveOnFinish(true);
	
	addChild(star_particle, kTakeCardPopup_Z_particle);
}
void TakeCardPopup::fadeGrayTitle ()
{
	gray->runAction(CCFadeTo::create(0.4f, 0));
	take_label->runAction(CCFadeTo::create(0.4f, 0));
}
void TakeCardPopup::closingAction ()
{
	CCMoveTo* t_move = CCMoveTo::create(0.8f, ccp(-120,130));
	
	CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.1f, 0.2f, 0, 0, 90, 0, 0);// float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX
	CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::changeBack));
	CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.1f, 0.2f, 0, -90, 90, 0, 0);
	CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::changeTakeCard));
	CCSequence* t_seq = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
	CCRepeat* t_repeat = CCRepeat::create(t_seq, 2);
	
	CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::fadeGrayTitle));
	
	CCOrbitCamera* t_orbit5 = CCOrbitCamera::create(0.1f, 0.2f, 0, 0, 90, 0, 0);// float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX
	CCCallFunc* t_call4 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::changeBack));
	CCOrbitCamera* t_orbit6 = CCOrbitCamera::create(0.1f, 0.2f, 0, -90, 90, 0, 0);
	CCCallFunc* t_call5 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::changeTakeCard));
	CCSequence* t_seq4 = CCSequence::create(t_orbit5, t_call4, t_orbit6, t_call5, NULL);
	CCRepeat* t_repeat3 = CCRepeat::create(t_seq4, 2);
	
	CCCallFunc* t_call6 = CCCallFunc::create(this, callfunc_selector(TakeCardPopup::endCall));
	CCSequence* t_seq2 = CCSequence::create(t_repeat, t_call3, t_repeat3, t_call6, NULL);
	CCSpawn* t_spawn1 = CCSpawn::create(t_move, t_seq2, NULL);
	take_card->runAction(t_spawn1);
	
	
	CCMoveTo* t_move2 = CCMoveTo::create(0.8f, ccp(-120,130));
	CCOrbitCamera* t_orbit3 = CCOrbitCamera::create(0.1f, 0.2f, 0, 0, 90, 0, 0);
	CCOrbitCamera* t_orbit4 = CCOrbitCamera::create(0.1f, 0.2f, 0, -90, 90, 0, 0);
	CCSequence* t_seq3 = CCSequence::create(t_orbit3, t_orbit4, NULL);
	CCRepeat* t_repeat2 = CCRepeat::create(t_seq3, 4);
	CCSpawn* t_spawn2 = CCSpawn::create(t_move2, t_repeat2, NULL);
	card_back->runAction(t_spawn2);
}
bool TakeCardPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	if(is_touch_enable)
	{
		closingAction();
		star_particle->setDuration(0);
		is_touch_enable = false;
	}
	return true;
}
void TakeCardPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{}
void TakeCardPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{}
void TakeCardPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{}
void TakeCardPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
#undef LZZ_INLINE
