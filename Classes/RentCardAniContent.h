////
////  RentCardAniContent.h
////  DGproto
////
////  Created by 사원3 on 2013. 12. 5..
////
////
//
//#ifndef __DGproto__RentCardAniContent__
//#define __DGproto__RentCardAniContent__
//
//#include "cocos2d.h"
//#include "cocos-ext.h"
//#include "CCMenuLambda.h"
//#include "StageImgLoader.h"
//#include "StarGoldData.h"
//#include "ServerDataSave.h"
//#include "SilhouetteData.h"
//#include "CardCase.h"
//#include "DataStorageHub.h"
//
//USING_NS_CC;
//USING_NS_CC_EXT;
//using namespace std;
//
//class RentCardAniContent : public CCNode
//{
//public:
//	static RentCardAniContent* create(int t_touch_priority, function<void(CCObject*)> t_selector)
//	{
//		RentCardAniContent* t_ctc = new RentCardAniContent();
//		t_ctc->myInit(t_touch_priority, t_selector);
//		t_ctc->autorelease();
//		return t_ctc;
//	}
//	
//private:
//	int touch_priority;
//	function<void(CCObject*)> end_selector;
//	
//	CCSprite* rent_card;
//	CCSprite* card_back;
//	
//	void myInit(int t_touch_priority, function<void(CCObject*)> t_selector)
//	{
//		touch_priority = t_touch_priority;
//		end_selector = t_selector;
//		
//		int card_number = mySGD->getSelectedFriendCardData().card_number;
//		
//		rent_card = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png", card_number)->getCString());
//		rent_card->setScale(0.5f);
//		rent_card->setPosition(ccp(360,0));
//		addChild(rent_card);
//		
//		if(NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, card_number))
//		{
//			CCSize ani_size = CCSizeMake(NSDS_GI(kSDS_CI_int1_aniInfoDetailCutWidth_i, card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailCutHeight_i, card_number));
//			CCSprite* take_ani = mySIL->getLoadedImg(CCString::createWithFormat("card%d_animation.png", card_number)->getCString(), CCRectMake(0, 0, ani_size.width, ani_size.height));
//			take_ani->setPosition(ccp(NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionX_i, card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionY_i, card_number)));
//			rent_card->addChild(take_ani);
//		}
//		
//		CardCase* t_case = CardCase::create(card_number, mySGD->getSelectedFriendCardData().card_level, mySGD->getSelectedFriendCardData().card_passive);
//		t_case->setPosition(CCPointZero);
//		rent_card->addChild(t_case);
//		
//		CCLabelTTF* recent_durability_label = t_case->getRecentDurabilityLabel();
//		recent_durability_label->setString(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_durability_i, mySGD->getSelectedFriendCardData().card_number))->getCString());
////		recent_durability_label->setPosition(ccpAdd(recent_durability_label->getPosition(), ccp(6,-1)));
//		recent_durability_label->setFontSize(recent_durability_label->getFontSize()+3);
//		
////		t_case->getTotalDurabilityLabel()->removeFromParent();
//		
//		card_back = CCSprite::create("ending_take_card_back.png");
//		card_back->setVisible(false);
//		card_back->setScale(rent_card->getScale());
//		card_back->setPosition(rent_card->getPosition());
//		addChild(card_back);
//		
//		runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.2f), CCCallFunc::create(this, callfunc_selector(RentCardAniContent::openingAction))));
//	}
//	
//	void openingAction ()
//	{
//		CCMoveTo* t_move = CCMoveTo::create(0.8f, ccp(0,0));
//		
//		CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.1f, 0.2f, 0, 0, 90, 0, 0);// float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX
//		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(RentCardAniContent::changeBack));
//		CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.1f, 0.2f, 0, -90, 90, 0, 0);
//		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(RentCardAniContent::changeTakeCard));
//		CCSequence* t_seq = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
//		CCRepeat* t_repeat = CCRepeat::create(t_seq, 4);
//		CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(RentCardAniContent::endOpeningAction));
//		CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_repeat, t_call3);
//		CCSpawn* t_spawn1 = CCSpawn::create(t_move, t_seq2, NULL);
//		rent_card->runAction(t_spawn1);
//		
//		
//		CCMoveTo* t_move2 = CCMoveTo::create(0.8f, ccp(0,0));
//		CCOrbitCamera* t_orbit3 = CCOrbitCamera::create(0.1f, 0.2f, 0, 0, 90, 0, 0);
//		CCOrbitCamera* t_orbit4 = CCOrbitCamera::create(0.1f, 0.2f, 0, -90, 90, 0, 0);
//		CCSequence* t_seq3 = CCSequence::create(t_orbit3, t_orbit4, NULL);
//		CCRepeat* t_repeat2 = CCRepeat::create(t_seq3, 4);
//		CCSpawn* t_spawn2 = CCSpawn::create(t_move2, t_repeat2, NULL);
//		card_back->runAction(t_spawn2);
//	}
//	void changeBack ()
//	{
//		card_back->setVisible(true);
//		rent_card->setVisible(false);
//	}
//	void changeTakeCard ()
//	{
//		rent_card->setVisible(true);
//		card_back->setVisible(false);
//	}
//	
//	void endOpeningAction()
//	{
//		card_back->removeFromParent();
//		CCDelayTime* t_delay = CCDelayTime::create(1.f);
//		CCMoveTo* t_move = CCMoveTo::create(0.5f, ccp(-240+20+60,-myDSH->ui_center_y+myDSH->ui_top-65));
//		CCScaleTo* t_scale = CCScaleTo::create(0.5f, 0.1f);
//		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_move, t_scale);
//		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(RentCardAniContent::finalAction));
//		CCSequence* t_seq = CCSequence::create(t_delay, t_spawn, t_call, NULL);
//		
//		rent_card->runAction(t_seq);
//	}
//	
//	void finalAction()
//	{
//		end_selector(NULL);
//	}
//};
//
//#endif /* defined(__DGproto__RentCardAniContent__) */
