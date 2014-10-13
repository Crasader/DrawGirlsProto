//
//
//  MaingameScene.cpp
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 23..
//
//

#include "MaingameScene.h"
#include "AudioEngine.h"
#include "AlertEngine.h"
#include "MyLocalization.h"
#include "StarGoldData.h"
#include "StartMapGacha.h"
#include "SearchEye.h"
#include "ASPopupView.h"
#include "CCMenuLambda.h"
#include "ControlTutorialContent.h"
#include "PatternTutorialContent.h"
#include "RentCardAniContent.h"
#include "PuzzleScene.h"
#include "MainFlowScene.h"
#include "AchieveNoti.h"
#include "PauseContent.h"
#include "ContinueContent.h"
#include "StartSettingScene.h"
#include "AcceptChallengeAniContent.h"
#include "NewMainFlowScene.h"
#include "LoadingTipScene.h"
#include "CumberShowWindow.h"
#include "FormSetter.h"
#include "StoryManager.h"
#include "LoadingLayer.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "EffectSprite.h"
#include "ShopPopup.h"
#include "OnePercentGame.h"
#include "ControlTipContent.h"
#include "EndlessStartContent.h"
#include "FlagSelector.h"
#include "ClearTimeEventContent.h"
#include "StyledLabelTTF.h"
#include "TouchSuctionLayer.h"
#include "OnePercentTutorial.h"
#include "TypingBox.h"
#include "FiveRocksCpp.h"

//#include "ScreenSide.h"

CCScene* Maingame::scene()
{
    CCScene *scene = CCScene::create();
    Maingame *layer = Maingame::create();
    scene->addChild(layer);
	
    return scene;
}

Maingame::~Maingame()
{
	CCLOG("~Maingame");
	mySGD->is_on_maingame = false;
	
	if(replay_boss)
	{
		replay_boss->removeAllObjects();
		replay_boss->release();
		replay_boss = NULL;
	}
	if(replay_sub)
	{
		replay_sub->removeAllObjects();
		replay_sub->release();
		replay_sub = NULL;
	}
	
	if(boss_thumbs)
	{
		boss_thumbs->removeAllObjects();
		boss_thumbs->release();
		boss_thumbs = NULL;
	}
	if(sub_thumbs)
	{
		sub_thumbs->removeAllObjects();
		sub_thumbs->release();
		sub_thumbs = NULL;
	}
	AudioEngine::sharedInstance()->stopAllEffects();
}

bool Maingame::init()
{
    if ( !CCLayer::init() )
    {
    
        return false;
    }
	
	myCP = NULL;
	
	replay_boss = NULL;
	replay_sub = NULL;
	boss_thumbs = NULL;
	sub_thumbs = NULL;
	
	mySGD->is_on_maingame = false;
	mySGD->is_paused = false;
	setTag(0);
	AudioEngine::sharedInstance()->startGame();
	
	sub_thumbs = NULL;
	
	damaged_score = 0;
	
	is_pause = false;
	is_gohome = false;
	t_smg = NULL;
	
	warning_count = 0;
	
	replay_continue_count = 0;
	replay_continue_label = NULL;
	
	recent_take_speed_up_value = -1;
	save_take_speed_up_effect = NULL;
	
	
	combo_value_img = NULL;
	myJack = NULL;
	
	
	isCheckingBacking = false;
	
	init_state = kMIS_beforeInit;
	
	myGD->V_V["Main_startBackTracking"] = std::bind(&Maingame::startBackTracking, this);
	myGD->V_V["Main_allStopSchedule"] = std::bind(&Maingame::allStopSchedule, this);
	myGD->V_V["Main_gameover"] = std::bind(&Maingame::gameover, this);
	myGD->V_V["Main_touchEnd"] = std::bind(&Maingame::touchEnd, this);
	myGD->V_V["Main_touchOn"] = std::bind(&Maingame::touchOn, this);
	myGD->V_V["Main_stunBackTracking"] = std::bind(&Maingame::stunBackTracking, this);
	myGD->V_V["Main_startSpecialAttack"] = std::bind(&Maingame::startSpecialAttack, this);
	myGD->V_V["Main_stopSpecialAttack"] = std::bind(&Maingame::stopSpecialAttack, this);
	myGD->V_CCP["Main_moveGamePosition"] = std::bind(&Maingame::moveGamePosition, this, _1);
	myGD->V_CCPI["Main_goldGettingEffect"] = std::bind(&Maingame::goldGettingEffect, this, _1, _2);
	myGD->V_FBCCP["Main_percentageGettingEffect"] = std::bind(&Maingame::percentageGettingEffect,
															  this, _1, _2, _3);
	myGD->V_Ip["Main_showLineDiePosition"] = std::bind(&Maingame::showLineDiePosition, this, _1);
	myGD->V_V["Main_resetIsLineDie"] = std::bind(&Maingame::resetIsLineDie, this);
	myGD->V_I["Main_showWarning"] = std::bind(&Maingame::showWarning, this, _1);
	myGD->showDetailMessage = std::bind(&Maingame::showDetailMessage, this, _1, _2);
	myGD->V_Str["Main_showTextMessage"] = std::bind(&Maingame::showTextMessage, this, _1);
	myGD->V_V["Main_showCoin"] = std::bind(&Maingame::showCoin, this);
	myGD->V_V["Main_startExchange"] = std::bind(&Maingame::startExchange, this);
	myGD->V_V["Main_showTakeCoin"] = std::bind(&Maingame::showTakeCoin, this);
	myGD->V_V["Main_showChangeCard"] = std::bind(&Maingame::showChangeCard, this);
	myGD->V_V["Main_startMoveToBossPosition"] = std::bind(&Maingame::startMoveToBossPosition, this);
	myGD->CCN_V["Main_gameNodePointer"] = std::bind(&Maingame::gameNodePointer, this);
	myGD->V_V["Main_setUnlimitMap"] = std::bind(&Maingame::setUnlimitMap, this);
	myGD->V_F["Main_startShake"] = std::bind(&Maingame::startShake, this, _1);
	myGD->V_CCP["Main_startMoveToCCPoint"] = std::bind(&Maingame::startMoveToCCPoint, this, _1);
	myGD->V_I["Main_takeSpeedUpEffect"] = std::bind(&Maingame::takeSpeedUpEffect, this, _1);
	myGD->V_CCP["Main_showMissMissile"] = std::bind(&Maingame::showMissMissile, this, _1);
	myGD->V_CCPI["Main_showDamageMissile"] = std::bind(&Maingame::showDamageMissile, this, _1, _2);
	myGD->CCP_V["Main_getGameNodePosition"] = std::bind(&Maingame::getGameNodePosition, this);
	myGD->V_V["Main_hideThumb"] = std::bind(&Maingame::hideThumb, this);
	myGD->V_V["Main_showDrawButtonTutorial"] = std::bind(&Maingame::showDrawButtonTutorial, this);
	myGD->V_V["Main_hideDrawButtonTutorial"] = std::bind(&Maingame::hideDrawButtonTutorial, this);
	myGD->V_V["Main_showPause"] = std::bind(&Maingame::showPause, this);
	myGD->V_I["Main_showShop"] = std::bind(&Maingame::showShop, this, _1);
	myGD->V_TDTD["Main_showContinue"] = std::bind(&Maingame::showContinue, this, _1, _2, _3, _4);
	myGD->V_B["Main_setLineParticle"] = std::bind(&Maingame::setLineParticle, this, _1);
	myGD->V_CCPI["Main_showComboImage"] = std::bind(&Maingame::showComboImage, this, _1, _2);
	myGD->V_CCP["Main_showThumbWarning"] = std::bind(&Maingame::showThumbWarning, this, _1);
	myGD->V_V["Main_showScreenSideWarning"] = std::bind(&Maingame::showScreenSideWarning, this);
	myGD->V_V["Main_hideScreenSideWarning"] = std::bind(&Maingame::hideScreenSideWarning, this);
	myGD->V_CCP["Main_initJackPosition"] = std::bind(&Maingame::initJackPosition, this, _1);
	myGD->V_I["Main_scoreAttackMissile"] = std::bind(&Maingame::scoreAttackMissile, this, _1);
	myGD->V_CCP["Main_showScoreMissileEffect"] = std::bind(&Maingame::showScoreMissileEffect, this, _1);
	myGD->V_V["Main_checkHideStartMapGacha"] = std::bind(&Maingame::checkHideStartMapGacha, this);
	myGD->V_V["Main_stopBackingCheck"] = std::bind(&Maingame::stopBackingCheck, this);
	
	mControl = NULL;
	is_line_die = false;
	
	game_node = CCNode::create();
	game_node->setScale((480.f-myGD->boarder_value*2)/(320.f));
	//game_node->setScale((480.f-myGD->boarder_value*2)/(320.f)*myGD->game_scale);
	if(myGD->gamescreen_type == kGT_leftUI)			game_node->setPosition(ccp(50+myGD->boarder_value,0));
	else if(myGD->gamescreen_type == kGT_rightUI)	game_node->setPosition(ccp(myGD->boarder_value, 0));
	else											game_node->setPosition(ccp(myGD->boarder_value,0));
	addChild(game_node, myMSZorder);
	
	myGD->F_V["Main_getGameNodeScale"] = std::bind(&CCNode::getScale, game_node);
	
	myMS = MapScanner::create();
	game_node->addChild(myMS, myMSZorder);
	
	return true;
}

void Maingame::checkHideStartMapGacha()
{
	if(t_smg)
	{
		t_smg->outAction();
	}
}

void Maingame::setLineParticle(bool t_b)
{
	if(t_b && myGIM->getIsFevering() && myJack->getJackState() == jackStateDrawing)
	{
		line_particle->setStartSize(5);
		line_particle->setStartSizeVar(3);
		line_particle->setEndSize(2);
		line_particle->setEndSizeVar(1);
	}
	else
	{
		line_particle->setStartSize(0);
		line_particle->setStartSizeVar(0);
		line_particle->setEndSize(0);
		line_particle->setEndSizeVar(0);
	}
}

void Maingame::showDrawButtonTutorial()
{
	if(mControl)
		((ControlJoystickButton*)mControl)->showDrawButtonTutorial();
}

void Maingame::hideDrawButtonTutorial()
{
	if(mControl)
		((ControlJoystickButton*)mControl)->hideDrawButtonTutorial();
}

void Maingame::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
//	if(mySD->getSilType() == 1)
//	{
//		if(!myDSH->getBoolForKey(kDSH_Key_hasShowTutorial_int1, kSpecialTutorialCode_control))
//		{
//			myDSH->setBoolForKey(kDSH_Key_hasShowTutorial_int1, kSpecialTutorialCode_control, true);
//			CCNode* exit_target = this;
//			exit_target->onExit();
//			
//			ASPopupView* t_popup = ASPopupView::create(-200);
//			
//			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//			if(screen_scale_x < 1.f)
//				screen_scale_x = 1.f;
//			
//			t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
//			t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
//			t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
//			
//			ControlTutorialContent* t_container = ControlTutorialContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
//																				 {
//																					 exit_target->onEnter();
//																					 
//																					 CCMoveBy* t_move = CCMoveBy::create(0.5f, ccp(-480,0));
//																					 CCCallFunc* t_call = CCCallFunc::create(t_popup, callfunc_selector(CCNode::removeFromParent));
//																					 CCSequence* t_seq = CCSequence::create(t_move, t_call, NULL);
//																					 t_popup->runAction(t_seq);
//																					 
//																					 //																					 t_popup->removeFromParent();
//																				 });
//			t_popup->setContainerNode(t_container);
//			exit_target->getParent()->addChild(t_popup);
//		}
//	}
	
	CCLayer* top_bottom_layer = CCLayer::create();
	top_bottom_layer->setPosition(ccp(0, 0));
	getParent()->addChild(top_bottom_layer, -1);
	
	CCSprite* side_back = CCSprite::create("ingame_outback.png");
	side_back->setPosition(ccp(240,myDSH->ui_center_y));
	top_bottom_layer->addChild(side_back, topBottomZorder);
	
	if(mySGD->getStartMapGachaCnt() == 0)
	{
		if(mySGD->is_endless_mode)
		{
			addChild(KSTimer::create(0.7f, [=](){endIntro();}));
		}
		else
		{
			intro_stencil = CCSprite::create("sight_out.png");
			intro_stencil->setScale(0.f);
			intro_stencil->setPosition(ccp(240,myDSH->ui_center_y+5));
			
			intro_clipping = CCClippingNode::create(intro_stencil);
			intro_clipping->setPosition(CCPointZero);
			addChild(intro_clipping, introZorder);
			
			intro_clipping->setInverted(true);
			intro_clipping->setAlphaThreshold(0.01f);
			
			EffectSprite* blur_img = EffectSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_visible.png",NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 1))->getCString()));
			blur_img->setColor(ccc3(30, 30, 30));
			blur_img->setAnchorPoint(ccp(0,0));
			blur_img->setPosition(ccp(0,0));
			
//			if(NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 1)))
//			{
//				CCSprite* ccb_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 1))).first;
//				ccb_img->setPosition(ccp(160,215));
//				blur_img->addChild(ccb_img);
//			}
			
			CCRenderTexture* intro_texture = CCRenderTexture::create(320, 430);
			intro_texture->beginWithClear(0, 0, 0, 0);
			blur_img->visit();
			intro_texture->end();
			
			intro_texture->setScale(game_node->getScale());
			intro_texture->setPosition(ccp(240, 430*game_node->getScale()/2.f));
			intro_clipping->addChild(intro_texture);
			
			intro_out_line = CCSprite::create("sight_out.png");
			intro_out_line->setScale(0.f);
			intro_out_line->setPosition(ccp(240,myDSH->ui_center_y+5));
			intro_clipping->addChild(intro_out_line);
			
			intro_boss = CumberShowWindow::create(mySD->getSilType(), kCumberShowWindowSceneCode_cardChange);
			intro_boss->setPosition(ccp(240,myDSH->ui_center_y+400));
			intro_boss->setScale(1.8f);
			addChild(intro_boss, searchEyeZorder);
			
			CCDelayTime* t_delay = CCDelayTime::create(1.f);
			CCMoveTo* t_move = CCMoveTo::create(0.7f, ccp(240,myDSH->ui_center_y+10));
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Maingame::startStory));
			CCSequence* t_seq = CCSequence::create(t_delay, t_move, t_call, NULL);
			intro_boss->runAction(t_seq);
		}
		
		CCNode* curtain_node = LoadingTipScene::getOpenCurtainNode();
		curtain_node->setPosition(ccp(240,myDSH->ui_center_y));
		curtain_node->setScale(myDSH->screen_convert_rate);
		addChild(curtain_node, introZorder);
	}
	else
	{
		endIntro();
	}
}

void Maingame::startStory()
{
	TypingBox* t_tb = TypingBox::create(-500, "boss_mentbox.png", CCRectMake(0, 0, 110, 75), CCRectMake(87, 37, 7, 15), CCRectMake(15, 30, 80, 31), CCSizeMake(180, 40), ccp(240,myDSH->ui_center_y-65), ccp(480.f-56.f,32.5f));
	t_tb->setBoxScale(myDSH->screen_convert_rate);
	addChild(t_tb, 100);
	
	function<void()> end_func1 = [=]()
	{
		t_tb->setVisible(false);
		
		t_tb->setTouchOffScrollAndButton();
		t_tb->setTouchSuction(false);
		
		CCDelayTime* t_delay1 = CCDelayTime::create(0.5f);
		CCMoveTo* t_move1 = CCMoveTo::create(0.3f, ccp(240,myDSH->ui_center_y+600));
		CCCallFunc* t_call1 = CCCallFunc::create(intro_boss, callfunc_selector(CCNode::removeFromParent));
		CCSequence* t_seq1 = CCSequence::create(t_delay1, t_move1, t_call1, NULL);
		
		intro_boss->runAction(t_seq1);
		
		CCScaleTo* t_scale2 = CCScaleTo::create(0.5f, 3);
		CCDelayTime* t_delay2 = CCDelayTime::create(0.3f);
		CCCallFunc* t_call2 = CCCallFunc::create(intro_clipping, callfunc_selector(CCNode::removeFromParent));
		CCSequence* t_seq2 = CCSequence::create(t_scale2, t_delay2, t_call2, NULL);
		
		intro_stencil->runAction(t_seq2);
		
		intro_out_line->setScale(0.4f);
		CCScaleTo* t_scale3 = CCScaleTo::create(0.5f, 4.f);
		CCDelayTime* t_delay4 = CCDelayTime::create(0.3f);
		CCCallFunc* t_call5 = CCCallFunc::create(intro_out_line, callfunc_selector(CCNode::removeFromParent));
		CCSequence* t_seq4 = CCSequence::create(t_scale3, t_delay4, t_call5, NULL);
		
		intro_out_line->runAction(t_seq4);
		
		
		//					  t_sm->addMent(true, "", "", "시노비결계!", nullptr);
		
		CCDelayTime* t_delay3 = CCDelayTime::create(0.8f);
		CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(Maingame::endIntro));
		CCCallFunc* t_call4 = CCCallFunc::create(t_tb, callfunc_selector(TypingBox::removeFromParent));
		CCSequence* t_seq3 = CCSequence::create(t_delay3, t_call3, t_call4, NULL);
		
		t_tb->runAction(t_seq3);
	};
	
	t_tb->startTyping(intro_boss->start_ment.c_str(), end_func1);
	
//	StoryManager* t_sm = StoryManager::create(-500);
//	addChild(t_sm, 100);
//	
//	t_sm->addMent(true, "", "", intro_boss->start_ment.c_str(), [=]()
//				  //"파란 실루엣 영역을 획득해야 게임 달성도가 올라갑니다.", [=]()
//				  {
//					  CCDelayTime* t_delay1 = CCDelayTime::create(0.5f);
//					  CCMoveTo* t_move1 = CCMoveTo::create(0.3f, ccp(240,myDSH->ui_center_y+600));
//					  CCCallFunc* t_call1 = CCCallFunc::create(intro_boss, callfunc_selector(CCNode::removeFromParent));
//					  CCSequence* t_seq1 = CCSequence::create(t_delay1, t_move1, t_call1, NULL);
//					  
//					  intro_boss->runAction(t_seq1);
//					  
//					  CCScaleTo* t_scale2 = CCScaleTo::create(0.5f, 3);
//					  CCDelayTime* t_delay2 = CCDelayTime::create(0.3f);
//					  CCCallFunc* t_call2 = CCCallFunc::create(intro_clipping, callfunc_selector(CCNode::removeFromParent));
//					  CCSequence* t_seq2 = CCSequence::create(t_scale2, t_delay2, t_call2, NULL);
//					  
//					  intro_stencil->runAction(t_seq2);
//					  
//					  intro_out_line->setScale(0.4f);
//					  CCScaleTo* t_scale3 = CCScaleTo::create(0.5f, 4.f);
//					  CCDelayTime* t_delay4 = CCDelayTime::create(0.3f);
//					  CCCallFunc* t_call5 = CCCallFunc::create(intro_out_line, callfunc_selector(CCNode::removeFromParent));
//					  CCSequence* t_seq4 = CCSequence::create(t_scale3, t_delay4, t_call5, NULL);
//					  
//					  intro_out_line->runAction(t_seq4);
//					  
//					  
////					  t_sm->addMent(true, "", "", "시노비결계!", nullptr);
//					  
//					  CCDelayTime* t_delay3 = CCDelayTime::create(0.8f);
//					  CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(Maingame::endIntro));
//					  CCCallFunc* t_call4 = CCCallFunc::create(t_sm, callfunc_selector(StoryManager::removeFromParent));
//					  CCSequence* t_seq3 = CCSequence::create(t_delay3, t_call3, t_call4, NULL);
//					  
//					  t_sm->runAction(t_seq3);
//				  }, CCSizeMake(350,100), ccp(0,-110), 12, true);
}

void Maingame::endIntro()
{
	init_state = kMIS_movingGameNode;
	
	setTouchEnabled(true);
	
	gamenode_moving_direction = kGNMD_up;
	
	touch_img = CCSprite::create("touch_before_start.png");
	touch_img->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(touch_img, myPMZorder);
	
	CCFadeTo* fade1 = CCFadeTo::create(1.f, 0);
	CCFadeTo* fade2 = CCFadeTo::create(1.f, 255);
	CCDelayTime* delay1 = CCDelayTime::create(0.3f);
	CCRepeatForever* repeat1 = CCRepeatForever::create(CCSequence::create(fade1, fade2, delay1, NULL));
	
	touch_img->runAction(repeat1);
	
	schedule(schedule_selector(Maingame::movingGameNode));
}

void Maingame::movingGameNode()
{
	CCPoint after_position = ccpAdd(game_node->getPosition(), ccp(0,gamenode_moving_direction*4));
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	
	if(after_position.y < -430*game_node->getScale()+480*screen_size.height/screen_size.width)
	{
		after_position.y = -430*game_node->getScale()+480*screen_size.height/screen_size.width;
		gamenode_moving_direction = kGNMD_down;
	}
	else if(after_position.y > 0)
	{
		after_position.y = 0;
		gamenode_moving_direction = kGNMD_up;
	}
	game_node->setPosition(after_position);
}

void Maingame::randomingRectView()
{
	if(ignore_cnt < 10)
		ignore_cnt++;
	
	myMS->randomingRectView(game_node->getPosition());
	
	if(ignore_cnt >= 10 && ignore_cnt <= 15)
	{
		if(!isTouchEnabled())
			setTouchEnabled(true);
		ignore_cnt = 20;
	}
}

void Maingame::controlStunOff()
{
	mControl->isStun = false;
	((ControlJoystickButton*)mControl)->resetTouch();
}

void Maingame::finalSetting()
{
	init_state = kMIS_startGame;
	
//	myGD->initStartRect();
	
	
	myJack = Jack::create();
	game_node->addChild(myJack, myJackZorder);
	myJack->initStartPosition(game_node->getPosition());
	
	myPM = PathManager::create();
	game_node->addChild(myPM, myPMZorder);
	
	myGIM = GameItemManager::create();
	game_node->addChild(myGIM, attackItemZorder);
	
	myGD->B_V["Main_isFever"] = std::bind(&GameItemManager::getIsFevering, myGIM);
	
	myCP = CumberParent::create();
	game_node->addChild(myCP, myCPZorder);
	
	
	line_particle = CCParticleSystemQuad::createWithTotalParticles(100);
	line_particle->setPositionType(kCCPositionTypeRelative);
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("meteor_bomb.png");
	line_particle->setTexture(texture);
	line_particle->setEmissionRate(100);
	line_particle->setAngle(0.0);
	line_particle->setAngleVar(360.0);
	ccBlendFunc blendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA}; //GL_SRC_ALPHA, GL_ONE
	line_particle->setBlendFunc(blendFunc);
	line_particle->setDuration(-1);
	line_particle->setEmitterMode(kCCParticleModeGravity);
	ccColor4F startColor = {1.00,1.00,1.00,1.00}; // 0.76 0.25 0.12
	line_particle->setStartColor(startColor);
	ccColor4F startColorVar = {0,0,0,0};
	line_particle->setStartColorVar(startColorVar);
	ccColor4F endColor = {1.00,1.00,1.00,1.00};
	line_particle->setEndColor(endColor);
	ccColor4F endColorVar = {0,0,0,0};
	line_particle->setEndColorVar(endColorVar);
	line_particle->setStartSize(0);
	line_particle->setStartSizeVar(0);
	line_particle->setEndSize(0);
	line_particle->setEndSizeVar(0);
	line_particle->setGravity(ccp(0,0));
	line_particle->setRadialAccel(0.0);
	line_particle->setRadialAccelVar(0.0);
	line_particle->setSpeed(20);
	line_particle->setSpeedVar(10);
	line_particle->setTangentialAccel(0);
	line_particle->setTangentialAccelVar(0);
	line_particle->setTotalParticles(100);
	line_particle->setLife(0.5);
	line_particle->setLifeVar(0.2);
	line_particle->setStartSpin(0.0);
	line_particle->setStartSpinVar(360.f);
	line_particle->setEndSpin(0.0);
	line_particle->setEndSpinVar(360.f);
	line_particle->setPosVar(ccp(0,0));
	game_node->addChild(line_particle, myJackZorder);
	
	
	
	line_particle->setPosition(myJack->getPosition());
	
	game_node->setScale(myGD->game_scale);
	
	myGD->is_setted_jack = true;
	
	myUI = PlayUI::create();
	addChild(myUI, myUIZorder);
	myUI->clear_time_event_func = bind(&Maingame::showClearTimeEvent, this, _1, _2);

//	OnePercentTutorial* opt = OnePercentTutorial::create(0.99, nullptr, nullptr);
//	addChild(opt, 9999);
//	OnePercentGame* opg = OnePercentGame::create(0.99, nullptr, nullptr);
//	addChild(opg, 9999);
	myUI->setMaxBossLife(mySD->getBossMaxLife());
	
	if(mySD->getClearCondition() == kCLEAR_percentage)
		myUI->setClearPercentage(NSDS_GI(mySD->getSilType(), kSDS_SI_missionOptionPercent_i)/100.f);
	else if(mySD->getClearCondition() == kCLEAR_hellMode)
		myUI->setClearPercentage(2.f);
	else if(mySGD->isTimeEvent(kTimeEventType_clear))
		myUI->setClearPercentage(mySGD->getTimeEventFloatValue(kTimeEventType_clear)/100.f);
	else
		myUI->setClearPercentage(0.85f);
	
//	myCP->setUI_forEP(myUI, callfunc_selector(PlayUI::keepBossLife), callfunc_selector(PlayUI::checkBossLife));
	
	
	FloatingCoinParent* floating_coin_parent = FloatingCoinParent::create([=](CCPoint t_point)
																		  {
																			  myGIM->showTakeItemEffect(t_point);
																		  });
	game_node->addChild(floating_coin_parent, clearGoldZorder);
	myGD->V_F["Main_startClearFloatingCoin"] = std::bind(&FloatingCoinParent::startClearFloatCoin, floating_coin_parent, std::placeholders::_1);
	myGD->V_V["Main_changeMonsterAutoLevel"] = std::bind(&Maingame::changeMonsterAutoLevel, this);
	
	
	myMS->scanMap();
	myGD->communication("VS_setSceneNode", this);
	
//	myUI->setControlTD(this, callfunc_selector(Maingame::setControlGesture), callfunc_selector(Maingame::setControlButton), callfunc_selector(Maingame::setControlJoystick), callfunc_selector(Maingame::startControl));
	
	vector<CCNode*> main_cumber_vector = myGD->getMainCumberCCNodeVector();
	search_eye_vector.clear();
	
	for(int i=0;i<main_cumber_vector.size();i++)
	{
		CCNode* t_boss = main_cumber_vector[i];
		
		SearchEye* t_search_eye = SearchEye::create(t_boss, &search_eye_vector);
		t_search_eye->setPosition(CCPointZero);
		addChild(t_search_eye, searchEyeZorder);
		
		t_search_eye->startSearch();
		
		search_eye_vector.push_back(t_search_eye);
	}
	
	myGD->V_CCO["Main_removeSearchEye"] = bind(&Maingame::removeSearchEye, this, std::placeholders::_1);
	
	mySGD->resetIngameDetailScore();
	
	startScene();
	
	float thumb_scale = 0.17f;
	
	thumb_case = CCScale9Sprite::create("minimap_back.png", CCRectMake(0, 0, 30, 30), CCRectMake(14, 14, 2, 2));
	thumb_case->setContentSize(CCSizeMake(320*thumb_scale, 430*thumb_scale));
	thumb_case->setPosition(ccp(40,myDSH->ui_center_y));
	addChild(thumb_case, clearshowtimeZorder);
	
	sil_thumb = EffectSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 1))->getCString()));
	int t_puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	sil_thumb->setColorSilhouette(NSDS_GI(t_puzzle_number, kSDS_PZ_color_r_d)*0.7f, NSDS_GI(t_puzzle_number, kSDS_PZ_color_g_d)*0.7f, NSDS_GI(t_puzzle_number, kSDS_PZ_color_b_d)*0.7f);
	sil_thumb->setScale(thumb_scale);
	sil_thumb->setPosition(ccp(40,myDSH->ui_center_y));
	addChild(sil_thumb, searchEyeZorder);
	
	thumb_texture = CCRenderTexture::create(320, 430);
	thumb_texture->setScale(thumb_scale);
	thumb_texture->setPosition(ccp(40,myDSH->ui_center_y));//myDSH->ui_top-90-215.f*thumb_scale));
	addChild(thumb_texture, clearshowtimeZorder);
	
	
	
//	thumb_case_top = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 320*thumb_scale + 2, 1));
//	thumb_case_top->setPosition(ccpAdd(thumb_texture->getPosition(), ccp(0,215*thumb_scale+1)));
//	addChild(thumb_case_top, myUIZorder);
//	
//	thumb_case_down = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 320*thumb_scale + 2, 1));
//	thumb_case_down->setPosition(ccpAdd(thumb_texture->getPosition(), ccp(0,-215*thumb_scale-1)));
//	addChild(thumb_case_down, myUIZorder);
//	
//	thumb_case_left = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 1, 430*thumb_scale + 2));
//	thumb_case_left->setPosition(ccpAdd(thumb_texture->getPosition(), ccp(-160*thumb_scale-1,0)));
//	addChild(thumb_case_left, myUIZorder);
//	
//	thumb_case_right = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 1, 430*thumb_scale + 2));
//	thumb_case_right->setPosition(ccpAdd(thumb_texture->getPosition(), ccp(160*thumb_scale+1,0)));
//	addChild(thumb_case_right, myUIZorder);
	
	screen_node = CCNode::create();
	screen_node->setPosition(thumb_texture->getPosition());
	addChild(screen_node, screenNodeZorder);
	screen_node->setScale(1.4563f/myGD->game_scale);
	
	CCSize screen_data = CCSizeMake(320.f*thumb_scale, 0);
	screen_data.height = screen_data.width/3.f*2.f*myDSH->ui_top/320.f;
	
	CCScale9Sprite* screen_img = CCScale9Sprite::create("minimap_front.png", CCRectMake(0, 0, 30, 30), CCRectMake(14, 14, 2, 2));
	screen_img->setContentSize(screen_data);
	screen_img->setPosition(ccp(0,0));
	screen_node->addChild(screen_img);
	
//	CCSprite* screen_top = CCSprite::create("whitePaper.png", CCRectMake(0, 0, screen_data.width+1, 1.5f));
//	screen_top->setColor(ccc3(255, 180, 0));
//	screen_top->setPosition(ccp(0,screen_data.height/2.f));
//	screen_node->addChild(screen_top);
//	CCSprite* screen_down = CCSprite::create("whitePaper.png", CCRectMake(0, 0, screen_data.width+1, 1.5f));
//	screen_down->setColor(ccc3(255, 180, 0));
//	screen_down->setPosition(ccp(0,-screen_data.height/2.f));
//	screen_node->addChild(screen_down);
//	CCSprite* screen_left = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 1.5f, screen_data.height+1));
//	screen_left->setColor(ccc3(255, 180, 0));
//	screen_left->setPosition(ccp(-screen_data.width/2.f,0));
//	screen_node->addChild(screen_left);
//	CCSprite* screen_right = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 1.5f, screen_data.height+1));
//	screen_right->setColor(ccc3(255, 180, 0));
//	screen_right->setPosition(ccp(screen_data.width/2.f,0));
//	screen_node->addChild(screen_right);
	
	myGD->communication("Main_initJackPosition", myJack->getPosition());
//	myJack->setPosition(myJack->getPosition());
	
	
	screen_side_warning_node = CCNode::create();
	screen_side_warning_node->setPosition(CCPointZero);
	addChild(screen_side_warning_node, screenNodeZorder);
	
	thumb_base_position = ccp(40-160.f*thumb_scale,myDSH->ui_center_y-215.f*thumb_scale);
	
	if(mySGD->is_endless_mode)
	{
		thumb_case->setPositionY(myDSH->ui_center_y-10);
		sil_thumb->setPositionY(myDSH->ui_center_y-10);
		thumb_texture->setPositionY(myDSH->ui_center_y-10);
		thumb_base_position.y -= 10;
	}
	
	
	CCDelayTime* thumb_delay = CCDelayTime::create(0.3f);
	CCCallFunc* thumb_call = CCCallFunc::create(this, callfunc_selector(Maingame::refreshThumb));
	CCSequence* thumb_seq = CCSequence::createWithTwoActions(thumb_delay, thumb_call);
	CCRepeatForever* thumb_repeat = CCRepeatForever::create(thumb_seq);
	thumb_texture->runAction(thumb_repeat);
	
	boss_thumbs = new CCArray(1);
	
//	boss_thumb = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 6, 6));
//	boss_thumb->setColor(ccRED);
//	CCNode* boss_pointer = myGD->getCommunicationNode("CP_getMainCumberPointer");
//	boss_thumb->setPosition(ccpAdd(thumb_base_position, ccpMult(boss_pointer->getPosition(), thumb_scale)));
//	addChild(boss_thumb, myUIZorder);
	
	character_thumb = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 4, 4));
	character_thumb->setColor(ccGREEN);
	character_thumb->setPosition(ccpAdd(thumb_base_position, ccpMult(myJack->getPosition(), thumb_scale)));
	addChild(character_thumb, clearshowtimeZorder);
	
	sub_thumbs = new CCArray(1);
	
	if(mySGD->is_play_replay)
	{
		endless_you_bomb_node = CCNode::create();
		endless_you_bomb_node->setPosition(ccp(440, myDSH->ui_center_y+60));
		addChild(endless_you_bomb_node, clearshowtimeZorder);
		
		CCClippingNode* you_bomb_clipping = CCClippingNode::create(CCSprite::create("endless_bomb_mask.png"));
		you_bomb_clipping->setAlphaThreshold(0.1f);
		you_bomb_clipping->setPosition(ccp(0,0));
		endless_you_bomb_node->addChild(you_bomb_clipping, -1);
		
		CCSprite* you_bomb_img = CCSprite::create("endless_bomb.png");
		you_bomb_img->setPosition(ccp(0, 0));
		endless_you_bomb_node->addChild(you_bomb_img);
		
		auto temp = KS::loadCCBI<CCSprite*>(this, "endless_bomb_you.ccbi");
		replay_bomb = temp.first;
		replay_bomb->setPosition(ccp(0, -54));
		you_bomb_clipping->addChild(replay_bomb);
		
		myGD->V_V["Main_readyBomb"] = [=]()
		{
			replay_bomb->removeChildByTag(9999);
			float t_y = replay_bomb->getPositionY();
			replay_bomb->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
																{
																	replay_bomb->setPositionY(t_y - t_y*t);
																}, [=](float t)
																{
																	replay_bomb->setPositionY(t_y - t_y*t);
																}), 0, 9999);
			//replay_manager->runAnimationsForSequenceNamed("bombcast1");
		};
		myGD->V_V["Main_stopBomb"] = [=]()
		{
			replay_bomb->removeChildByTag(9999);
			float t_y = replay_bomb->getPositionY();
			replay_bomb->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
																{
																	replay_bomb->setPositionY(t_y + (-54-t_y)*t);
																}, [=](float t)
																{
																	replay_bomb->setPositionY(t_y + (-54-t_y)*t);
																}), 0, 9999);
//			replay_manager->runAnimationsForSequenceNamed("Default Timeline");
		};
		
		replay_all_node = CCNode::create();
		replay_all_node->setPosition(CCPointZero);
		addChild(replay_all_node, clearshowtimeZorder);
		
//		if(mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapTime)].size() > 0)
//		{
			CCScale9Sprite* replay_thumb_case = CCScale9Sprite::create("minimap_back.png", CCRectMake(0, 0, 30, 30), CCRectMake(14, 14, 2, 2));
			replay_thumb_case->setContentSize(CCSizeMake(320*thumb_scale, 430*thumb_scale));
			replay_thumb_case->setPosition(ccp(480-40,myDSH->ui_center_y-10));
			replay_all_node->addChild(replay_thumb_case);
			
			replay_sil_thumb = EffectSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 1))->getCString()));
			int t_puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
			replay_sil_thumb->setColorSilhouette(NSDS_GI(t_puzzle_number, kSDS_PZ_color_r_d)*0.7f, NSDS_GI(t_puzzle_number, kSDS_PZ_color_g_d)*0.7f, NSDS_GI(t_puzzle_number, kSDS_PZ_color_b_d)*0.7f);
			replay_sil_thumb->setScale(thumb_scale);
			replay_sil_thumb->setPosition(ccp(480-40,myDSH->ui_center_y-10));
			replay_all_node->addChild(replay_sil_thumb, -1);
			
			replay_thumb_texture = CCRenderTexture::create(320, 430);
			replay_thumb_texture->setScale(thumb_scale);
			replay_thumb_texture->setPosition(ccp(480-40,myDSH->ui_center_y-10));//myDSH->ui_top-90-215.f*thumb_scale));
			replay_all_node->addChild(replay_thumb_texture);
			
			myGD->V_I["Main_refreshReplayThumb"] = std::bind(&Maingame::refreshReplayThumb, this, _1);
			
			
//			CCSprite* replay_case_top = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 320*thumb_scale + 2, 1));
//			replay_case_top->setPosition(ccpAdd(replay_thumb_texture->getPosition(), ccp(0,215*thumb_scale+1)));
//			replay_all_node->addChild(replay_case_top);
//			
//			CCSprite* replay_case_down = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 320*thumb_scale + 2, 1));
//			replay_case_down->setPosition(ccpAdd(replay_thumb_texture->getPosition(), ccp(0,-215*thumb_scale-1)));
//			replay_all_node->addChild(replay_case_down);
//			
//			CCSprite* replay_case_left = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 1, 430*thumb_scale + 2));
//			replay_case_left->setPosition(ccpAdd(replay_thumb_texture->getPosition(), ccp(-160*thumb_scale-1,0)));
//			replay_all_node->addChild(replay_case_left);
//			
//			CCSprite* replay_case_right = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 1, 430*thumb_scale + 2));
//			replay_case_right->setPosition(ccpAdd(replay_thumb_texture->getPosition(), ccp(160*thumb_scale+1,0)));
//			replay_all_node->addChild(replay_case_right);
//			
//			
//			replay_boss = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 6, 6));
//			replay_boss->setColor(ccRED);
//			replay_boss->setVisible(false);
//			replay_boss->setPosition(replay_thumb_texture->getPosition());
//			replay_all_node->addChild(replay_boss);
			
			replay_boss = new CCArray(1);
			
			replay_character = CCSprite::create("whitePaper.png", CCRectMake(0,0,4,4));
			replay_character->setColor(ccGREEN);
			replay_character->setVisible(false);
			replay_character->setPosition(replay_thumb_texture->getPosition());
			replay_all_node->addChild(replay_character);
			
			replay_sub = new CCArray(1);
			
			myGD->V_I["Main_refreshReplayPosition"] = std::bind(&Maingame::refreshReplayPosition, this, _1);
//		}
		
		replay_score = CountingBMLabel::create("0", "endless_score.fnt", 2.f, "%.0f");
		((CountingBMLabel*)replay_score)->onChangeScale(false);
		replay_score->setPosition(ccpAdd(replay_thumb_texture->getPosition(), ccp(0,-215.f*thumb_scale+10)));
		replay_all_node->addChild(replay_score, 99);
		
		myGD->V_I["Main_refreshReplayScore"] = std::bind(&Maingame::refreshReplayScore, this, _1);
		
		
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
        
		string flag = mySGD->endless_flag.getV();
		CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
		selectedFlagSpr->setPosition(ccp(480-40,myDSH->ui_center_y-10) + ccp(-20,215.f*0.17f+8));
		selectedFlagSpr->setScale(0.5f);
		replay_all_node->addChild(selectedFlagSpr);
		
		addChild(KSGradualValue<float>::create(480-40-20+100, 480-40-20, 0.5f, [=](float t){selectedFlagSpr->setPositionX(t);}, [=](float t){selectedFlagSpr->setPositionX(480-40-20);}));
		
		KSLabelTTF* nick_label = KSLabelTTF::create(mySGD->endless_nick.getV().c_str(), mySGD->getFont().c_str(), 10);
		nick_label->setAnchorPoint(ccp(0,0.5f));
		nick_label->setPosition(ccp(480-40,myDSH->ui_center_y-10) + ccp(-20,215.f*0.17f+8) + ccp(selectedFlagSpr->getContentSize().width/2.f*selectedFlagSpr->getScale()+2, 0));
		replay_all_node->addChild(nick_label);
		
		addChild(KSGradualValue<float>::create(480-40-20+selectedFlagSpr->getContentSize().width/2.f*selectedFlagSpr->getScale()+2+100, 480-40-20+selectedFlagSpr->getContentSize().width/2.f*selectedFlagSpr->getScale()+2,
											   0.5f, [=](float t){nick_label->setPositionX(t);}, [=](float t){nick_label->setPositionX(480-40-20+selectedFlagSpr->getContentSize().width/2.f*selectedFlagSpr->getScale()+2);}));
		
		int use_item_cnt = mySGD->replay_playing_info.get(mySGD->getReplayKey(kReplayKey_useItemCnt), Json::Value()).asInt();
		if(use_item_cnt > 0)
		{
			float item_scale = 0.18f;
			CCPoint item_base_position = ccp(480-40, myDSH->ui_center_y-10) + ccpMult(ccp(-160,-215), 0.17f) + ccp(7,-7);
			CCPoint distance_position = ccp(14,0);
			float signed_distance = 100.f;
			
			for(int i=0;i<use_item_cnt;i++)
			{
				ITEM_CODE t_code = ITEM_CODE(mySGD->replay_playing_info.get(CCString::createWithFormat(mySGD->getReplayKey(kReplayKey_useItem_int1_itemCode).c_str(), i+1)->getCString(), Json::Value()).asInt());
				
				GraySprite* item_img = GraySprite::create(CCString::createWithFormat("item%d.png", t_code)->getCString());
				item_img->setScale(item_scale);
				item_img->setOpacity(150);
				replay_all_node->addChild(item_img);
				
				CCSprite* item_case = CCSprite::create("ingame_itembox.png");
				item_case->setPosition(ccp(item_img->getContentSize().width/2.f, item_img->getContentSize().height/2.f));
				item_case->setScale(1.f/0.4f);
				item_case->setOpacity(150);
				item_img->addChild(item_case, -1);
				
				item_img->setPosition(ccpAdd(item_base_position, ccpMult(distance_position, i)));
				item_img->setPositionX(item_img->getPositionX()+signed_distance);
				replay_all_node->addChild(KSGradualValue<float>::create(ccpAdd(item_base_position, ccpMult(distance_position, i)).x+signed_distance, ccpAdd(item_base_position, ccpMult(distance_position, i)).x, 0.5f,
													   [=](float t){item_img->setPositionX(t);},
													   [=](float t){item_img->setPositionX(ccpAdd(item_base_position, ccpMult(distance_position, i)).x);}));
			}
		}
		
	}
	
	if(mySGD->is_endless_mode && mySGD->getStartMapGachaCnt() == 0)
	{
		CCNode* exit_target = this;
		exit_target->onExit();
		
		ASPopupView* t_popup = ASPopupView::create(-200);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
		t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
		t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
		
		EndlessStartContent* t_container = EndlessStartContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
																   {
																	   exit_target->onEnter();
																	   checkTutorial();
																	   
																   });
		t_popup->setContainerNode(t_container);
		exit_target->getParent()->addChild(t_popup);
	}
	else
	{
		checkTutorial();
	}
}

void Maingame::checkTutorial()
{
	if(mySD->getSilType() == 3 && !myDSH->getBoolForKey(kDSH_Key_hasShowTutorial_int1, kSpecialTutorialCode_lineTangle))
	{
		CCNode* exit_target = this;
		exit_target->onExit();
		
		ASPopupView* t_popup = ASPopupView::create(-200);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
		t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
		t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
		
		ControlTipContent* t_container = ControlTipContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
																   {
																	   myDSH->setBoolForKey(kDSH_Key_hasShowTutorial_int1, kSpecialTutorialCode_lineTangle, true);
																	   
																	   Json::Reader reader;
																	   Json::Value root;
																	   reader.parse(mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "boss"), root);
																	   Json::Value boss = root[0u];
																	   Json::Value patterns = boss["pattern"];
																	   
																	   vector<int> pattern_code;
																	   
																	   for(int i=0;i<patterns.size();i++)
																	   {
																		   int t_code = patterns[i]["pattern"].asInt();
																		   if(!myDSH->getBoolForKey(kDSH_Key_hasShowTutorial_int1, t_code))
																		   {
																			   vector<int>::iterator iter = find(pattern_code.begin(), pattern_code.end(), t_code);
																			   if(iter == pattern_code.end())
																				   pattern_code.push_back(t_code);
																		   }
																	   }
																	   
																	   if(pattern_code.size() > 0)
																	   {
																		   ASPopupView* t_popup = ASPopupView::create(-200);
																		   
																		   CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
																		   float screen_scale_x = screen_size.width/screen_size.height/1.5f;
																		   if(screen_scale_x < 1.f)
																			   screen_scale_x = 1.f;
																		   
																		   t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
																		   t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
																		   t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
																		   
																		   PatternTutorialContent* t_container = PatternTutorialContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
																																				{
																																					for(int i=0;i<pattern_code.size();i++)
																																						myDSH->setBoolForKey(kDSH_Key_hasShowTutorial_int1, pattern_code[i], true);
																																					exit_target->onEnter();
																																					mySGD->is_on_maingame = true;
																																				}, pattern_code);
																		   t_popup->setContainerNode(t_container);
																		   exit_target->getParent()->addChild(t_popup);
																	   }
																	   else
																	   {
																		   exit_target->onEnter();
																		   mySGD->is_on_maingame = true;
																	   }
																	   
																   }, kSpecialTutorialCode_lineTangle);
		t_popup->setContainerNode(t_container);
		exit_target->getParent()->addChild(t_popup);
	}
	else if(mySD->getSilType() == 6 && !myDSH->getBoolForKey(kDSH_Key_hasShowTutorial_int1, kSpecialTutorialCode_slimLine))
	{
		CCNode* exit_target = this;
		exit_target->onExit();
		
		ASPopupView* t_popup = ASPopupView::create(-200);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
		t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
		t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
		
		ControlTipContent* t_container = ControlTipContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
																   {
																	   myDSH->setBoolForKey(kDSH_Key_hasShowTutorial_int1, kSpecialTutorialCode_slimLine, true);
																	   
																	   Json::Reader reader;
																	   Json::Value root;
																	   reader.parse(mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "boss"), root);
																	   Json::Value boss = root[0u];
																	   Json::Value patterns = boss["pattern"];
																	   
																	   vector<int> pattern_code;
																	   
																	   for(int i=0;i<patterns.size();i++)
																	   {
																		   int t_code = patterns[i]["pattern"].asInt();
																		   if(!myDSH->getBoolForKey(kDSH_Key_hasShowTutorial_int1, t_code))
																		   {
																			   vector<int>::iterator iter = find(pattern_code.begin(), pattern_code.end(), t_code);
																			   if(iter == pattern_code.end())
																				   pattern_code.push_back(t_code);
																		   }
																	   }
																	   
																	   if(pattern_code.size() > 0)
																	   {
																		   ASPopupView* t_popup = ASPopupView::create(-200);
																		   
																		   CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
																		   float screen_scale_x = screen_size.width/screen_size.height/1.5f;
																		   if(screen_scale_x < 1.f)
																			   screen_scale_x = 1.f;
																		   
																		   t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
																		   t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
																		   t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
																		   
																		   PatternTutorialContent* t_container = PatternTutorialContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
																																				{
																																					for(int i=0;i<pattern_code.size();i++)
																																						myDSH->setBoolForKey(kDSH_Key_hasShowTutorial_int1, pattern_code[i], true);
																																					exit_target->onEnter();
																																					mySGD->is_on_maingame = true;
																																				}, pattern_code);
																		   t_popup->setContainerNode(t_container);
																		   exit_target->getParent()->addChild(t_popup);
																	   }
																	   else
																	   {
																		   exit_target->onEnter();
																		   mySGD->is_on_maingame = true;
																	   }
																	   
																   }, kSpecialTutorialCode_slimLine);
		t_popup->setContainerNode(t_container);
		exit_target->getParent()->addChild(t_popup);
	}
	else
	{
		Json::Reader reader;
		Json::Value root;
		reader.parse(mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "boss"), root);
		Json::Value boss = root[0u];
		Json::Value patterns = boss["pattern"];
		
		vector<int> pattern_code;
		
		for(int i=0;i<patterns.size();i++)
		{
			int t_code = patterns[i]["pattern"].asInt();
			if(!myDSH->getBoolForKey(kDSH_Key_hasShowTutorial_int1, t_code))
			{
				vector<int>::iterator iter = find(pattern_code.begin(), pattern_code.end(), t_code);
				if(iter == pattern_code.end())
					pattern_code.push_back(t_code);
			}
		}
		
		if(pattern_code.size() > 0)
		{
			CCNode* exit_target = this;
			exit_target->onExit();
			
			ASPopupView* t_popup = ASPopupView::create(-200);
			
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			if(screen_scale_x < 1.f)
				screen_scale_x = 1.f;
			
			t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
			t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
			t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
			
			PatternTutorialContent* t_container = PatternTutorialContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
																				 {
																					 for(int i=0;i<pattern_code.size();i++)
																						 myDSH->setBoolForKey(kDSH_Key_hasShowTutorial_int1, pattern_code[i], true);
																					 exit_target->onEnter();
																					 mySGD->is_on_maingame = true;
																				 }, pattern_code);
			t_popup->setContainerNode(t_container);
			exit_target->getParent()->addChild(t_popup);
		}
		else
		{
			mySGD->is_on_maingame = true;
		}
	}
	//	}
}

void Maingame::initJackPosition(CCPoint jack_position)
{
//	addChild(KSGradualValue<CCPoint>::create(ccp(160,getGameNodeToObjectPosition(game_node->getPosition()).y), jack_position, 0.5f,
//											 [=](CCPoint t){
//												 myGD->communication("VS_setMoveGamePosition", t);
//											 },
//											 [=](CCPoint t){
//												 myGD->communication("VS_setMoveGamePosition", jack_position);
//											 }, nullptr, false));
	
	addChild(KSGradualValue<CCPoint>::create(line_particle->getPosition(), jack_position, 0.5f,
											 [=](CCPoint t){
												 line_particle->setPosition(t);
											 },
											 [=](CCPoint t){
												 line_particle->setPosition(jack_position);
											 }, nullptr, false));
	
	addChild(KSGradualValue<CCPoint>::create(game_node->getPosition(), getObjectToGameNodePosition(jack_position), 0.5f,
											 [=](CCPoint t){
												 game_node->setPosition(t);
												 myGD->communication("VS_setMoveGamePosition", getGameNodeToObjectPosition(game_node->getPosition()));
											 },
											 [=](CCPoint t){
												 game_node->setPosition(getObjectToGameNodePosition(jack_position));
												 myGD->communication("VS_setMoveGamePosition", getGameNodeToObjectPosition(game_node->getPosition()));
											 }, nullptr, false));
}

void Maingame::startScene()
{
//	setBackKeyFunc([=](){
//			myUI->showPause();
//	});
//	setBackKeyEnabled(true);
	
	setKeypadEnabled(true);
	
	AudioEngine::sharedInstance()->playSound("sound_back_maingame.mp3", true);
	AudioEngine::sharedInstance()->preloadEffectScene("Maingame");
	
	startCounting();
}

void Maingame::checkAcceptChallenge()
{
//	if(mySGD->getIsAcceptChallenge())
//	{
//		CCNode* exit_target = this;
//		exit_target->onExit();
//		
//		ASPopupView* t_popup = ASPopupView::create(-200);
//		
//		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//		if(screen_scale_x < 1.f)
//			screen_scale_x = 1.f;
//		
//		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));
//		t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
//		t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
//		
//		AcceptChallengeAniContent* t_container = AcceptChallengeAniContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
//																				   {
//																					   checkFriendCard();
//																					   exit_target->onEnter();
//																					   t_popup->removeFromParent();
//																				   }, hspConnector::get()->getKakaoProfileURL(), hspConnector::get()->getKakaoNickname(),
//																				   mySGD->getAcceptChallengeProfile(), mySGD->getAcceptChallengeNick());
//		t_popup->setContainerNode(t_container);
//		exit_target->getParent()->addChild(t_popup);
//	}
//	else
//	{
		checkFriendCard();
//	}
}

void Maingame::checkFriendCard()
{
//	if(mySGD->isUsingItem(kIC_rentCard) && mySGD->getSelectedFriendCardData().card_number != 0)
//	{
//		CCNode* exit_target = this;
//		exit_target->onExit();
//		
//		ASPopupView* t_popup = ASPopupView::create(-200);
//		
//		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//		if(screen_scale_x < 1.f)
//			screen_scale_x = 1.f;
//		
//		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));
//		t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
//		t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
//		
//		RentCardAniContent* t_container = RentCardAniContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
//																			 {
//																				 myUI->setUseFriendCard();
//																				 exit_target->onEnter();
//																				 t_popup->removeFromParent();
//																			 });
//		t_popup->setContainerNode(t_container);
//		exit_target->getParent()->addChild(t_popup);
//	}
	
	setControlJoystickButton();
	startControl();
}

void Maingame::startCounting()
{
	condition_spr = KS::loadCCBI<CCSprite*>(this, "ui_ready.ccbi").first;
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	condition_spr = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ui_ready.ccbi",this));
	addChild(condition_spr, conditionLabelZorder);
//	reader->release();
	
	
	condition_spr->setPosition(ccp(240,myDSH->ui_center_y));
	
	CCDelayTime* t_delay = CCDelayTime::create(0.1f);
	CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(Maingame::checkAcceptChallenge));
	CCDelayTime* t_delay1 = CCDelayTime::create(0.65f);
	CCCallFunc* t_ms_startGame = CCCallFunc::create(myMS, callfunc_selector(MapScanner::startGame));
	CCDelayTime* t_delay2 = CCDelayTime::create(1.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Maingame::removeConditionLabel));
	runAction(CCSequence::create(t_delay, t_call1, t_delay1, t_ms_startGame, t_delay2, t_call, NULL));
	
	countingCnt = 0;
	
	myJack->isStun = true;
}

void Maingame::counting()
{
	countingCnt++;
	
	if(countingCnt/60 >= 2)
	{
		condition_spr->removeFromParent();
		unschedule(schedule_selector(Maingame::counting));
		
//		setTouchEnabled(true);
//		myCP->movingMainCumber();
//		myCP->movingSubCumbers();
	}
	else if(countingCnt/60 >= 1 && countingCnt%60 == 0)
	{
//		AudioEngine::sharedInstance()->playEffect("sound_go.mp3", false);
//		if(mySGD->getGoodsValue(kGoodsType_gold) >= mySGD->getGachaMapFee())
//		{
		
			t_smg = StartMapGacha::create(this, callfunc_selector(Maingame::gachaOn));
		t_smg->remove_func = [=]()
		{
			if(mySGD->start_map_gacha_use_gold_cnt > 0)
				fiverocks::FiveRocksBridge::trackEvent("UseGold", "StartDraw", ccsf("gacha %02d", mySGD->start_map_gacha_use_gold_cnt), ccsf("Stage %03d", mySD->getSilType()));
			t_smg = NULL;
		};
			addChild(t_smg, startGachaZorder);
//		}
		
		if(countingCnt/60 == 1)
		{
//			AudioEngine::sharedInstance()->playEffect("ment_go.mp3");
			setTag(1);
			myJack->isStun = false;
			myCP->onStartGame();
			myCP->startAutoAttacker();
			if(!is_gohome)
				myUI->startCounting();
			myGD->setIsGameover(false);
		}
	}
}

void Maingame::gachaOn()
{
	if(is_gohome)
		return;
	
	myLog->addLog(kLOG_gacha_startMap, -1);
	
	AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
	bool is_use_gold = false;
	
	if(mySGD->getGoodsValue(kGoodsType_pass2) > 0)
		mySGD->addChangeGoods("g_m_p", kGoodsType_pass2, 0, "", CCString::createWithFormat("%d", mySD->getSilType())->getCString(), "맵가챠(패스권)");
	else if(mySGD->getGoodsValue(kGoodsType_gold) >= mySGD->getGachaMapFee())
	{
		is_use_gold = true;
		mySGD->addChangeGoods("g_m_g", kGoodsType_gold, 0, "", CCString::createWithFormat("%d", mySD->getSilType())->getCString(), "맵가챠(골드)");
	}
	else
	{
		showShop(kSC_gold);
		return;
	}
	
	bool t_jack_stun = myJack->isStun;
	
	mySGD->is_on_maingame = false;
	
	CCNode* exit_target = this;
	mControl->setTouchEnabled(false);
	exit_target->onExit();
	
	LoadingLayer* t_loading = LoadingLayer::create(-9999, true);
	addChild(t_loading, 9999);
	
	int map_gacha_cnt = mySGD->getUserdataAchieveMapGacha()+1;
	mySGD->setUserdataAchieveMapGacha(map_gacha_cnt);
	
	vector<CommandParam> t_command_list;
	t_command_list.clear();
	t_command_list.push_back(mySGD->getChangeUserdataParam([=](Json::Value result_data)
								  {
									  if(result_data["result"]["code"].asInt() == GDSUCCESS)
										{
											for(int i=kAchievementCode_mapGacha1;i<=kAchievementCode_mapGacha3;i++)
											{
												if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
												   map_gacha_cnt >= myAchieve->getCondition((AchievementCode)i))
												{
													myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
													AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
													CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
												}
											}
										}
									  else
										{
											mySGD->clearChangeUserdata();
										}
								  }));
	
	mySGD->changeGoodsTransaction(t_command_list, [=](Json::Value result_data)
								  {
									  t_loading->removeFromParent();
									  if(result_data["result"]["code"].asInt() == GDSUCCESS)
									  {
//										  setBackKeyEnabled(false);
										  
										  mControl->isStun = false;
										  myJack->isStun = t_jack_stun;
//										  exit_target->onEnter();
										  
										  if(is_use_gold)
											{
												mySGD->start_map_gacha_use_gold_cnt++;
											}
										  
										  myGD->resetGameData();
										  mySGD->startMapGachaOn();
										  
										  CCDirector::sharedDirector()->replaceScene(Maingame::scene());
									  }
									  else
									  {
										  mySGD->clearChangeGoods();
										  addChild(ASPopupView::getCommonNoti(-99999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase), [=](){
											  mControl->isStun = false;
											  myJack->isStun = t_jack_stun;
											  exit_target->onEnter();
											  mySGD->is_on_maingame = true;
										  }), 9999);
									  }
									  
								  });
}

bool Maingame::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(init_state == kMIS_movingGameNode)
	{
		unschedule(schedule_selector(Maingame::movingGameNode));
		setTouchEnabled(false);
		init_state = kMIS_randomRectView;
		ignore_cnt = 0;
		AudioEngine::sharedInstance()->playEffect("se_mapgacha.mp3", true);
		randomingRectView();
		schedule(schedule_selector(Maingame::randomingRectView), 1.f/30.f);
		
		setLimitMap();
	}
	else if(init_state == kMIS_randomRectView)
	{
		unschedule(schedule_selector(Maingame::randomingRectView));
		myMS->stopRandomingRectView();
		touch_img->removeFromParent();
		
		AudioEngine::sharedInstance()->stopEffect("se_mapgacha.mp3");
		AudioEngine::sharedInstance()->playEffect("se_mapgacha_end.mp3", false);
		
		init_state = kMIS_startGame;
		setTouchEnabled(false);
		finalSetting();
	}
	
	return true;
}

void Maingame::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	
}

void Maingame::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	
}

void Maingame::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	
}

void Maingame::backTracking()
{
	IntPoint afterJackPoint = myPM->pathBackTracking();
	
	if(afterJackPoint.isNull())
	{
		stopBackTracking();
		return;
	}
	else
	{
		myJack->backTrackingAtAfterMoving(afterJackPoint);
	}
	
	afterJackPoint = myPM->pathBackTracking();
	
	if(afterJackPoint.isNull())
	{
		stopBackTracking();
		return;
	}
	else
	{
		myJack->backTrackingAtAfterMoving(afterJackPoint);
	}
	
	afterJackPoint = myPM->pathBackTracking();
	
	if(afterJackPoint.isNull())
	{
		stopBackTracking();
		return;
	}
	else
	{
		myJack->backTrackingAtAfterMoving(afterJackPoint);
	}
}

void Maingame::onEnter()
{
	CCLayer::onEnter();
}

void Maingame::onExit()
{
	touchEnd();
	if(mControl)
		((ControlJoystickButton*)mControl)->invisibleControl();
	CCLayer::onExit();
}

void Maingame::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -110, true);
}

void Maingame::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void Maingame::keyBackClicked()
{
	
	CommonButton::callBackKey();
	
//	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(Maingame::alertAction));
//	if(!myUI->isGameover)
//	{
//		onBackKeyAction();
//		
//		setBackKeyFunc([=](){
//			myUI->showPause();
//		});
//		setBackKeyEnabled(true);
//	}
}

void Maingame::touchEnd()
{
	if(myJack)
	{
		myJack->isStun = true;
		myJack->changeDirection(directionStop, directionStop);
		if(myJack->getJackState() == jackStateDrawing)
		{
			stunBackTracking();
		}
	}
	
	if(mControl)
	{
		mControl->isStun = true;
	//		mControl->setTouchEnabled(false);
//	((ControlJoystickButton*)mControl)->stopMySchedule();
		if(mControl->mType == kCT_Type_Joystick_button)
			myJack->setTouchPointByJoystick(CCPointZero, directionStop, true);
	}
}

void Maingame::touchOn()
{
//	((ControlJoystickButton*)mControl)->resetTouch();
	
	if(isCheckingBacking)
	{
		schedule(schedule_selector(Maingame::checkingBacking));
	}
	else
	{
		myJack->isStun = false;
		mControl->isStun = false;
		if(myJack->t_se)
		{
			myJack->t_se->selfRemove();
		}
		myJack->t_se = NULL;
		//			mControl->setTouchEnabled(true);
	}
}

void Maingame::setControlGesture()
{
	myDSH->setIntegerForKey(kDSH_Key_jackBaseSpeed, int(JackBaseSpeedTag::kJackBaseSpeedTag_level1));
	myGD->changeJackBaseSpeed(1.2f);
}

void Maingame::setControlButton()
{
	myDSH->setIntegerForKey(kDSH_Key_jackBaseSpeed, int(JackBaseSpeedTag::kJackBaseSpeedTag_level2));
	myGD->changeJackBaseSpeed(1.5f);
}

void Maingame::setControlJoystick()
{
	myDSH->setIntegerForKey(kDSH_Key_jackBaseSpeed, int(JackBaseSpeedTag::kJackBaseSpeedTag_level3));
	myGD->changeJackBaseSpeed(2.f);
}

void Maingame::setControlJoystickButton()
{
	if(mControl)		mControl->removeFromParentAndCleanup(true);
	mControl = ControlJoystickButton::create(this, callfunc_selector(Maingame::readyBackTracking), myJack);
	((ControlJoystickButton*)mControl)->pauseBackTracking = callfunc_selector(Maingame::pauseBackTracking);
	addChild(mControl, mControlZorder);
	myGD->V_V["Main_offDrawButtonTutorial"] = std::bind(&ControlJoystickButton::offDrawButtonTutorial, (ControlJoystickButton*)mControl);
}

void Maingame::startControl()
{
	if(mControl)
	{
		mControl->startControl();
	}
}

void Maingame::removeConditionLabel()
{
	//		AudioEngine::sharedInstance()->playEffect("sound_ready.mp3", false);
//	AudioEngine::sharedInstance()->playEffect("sound_gamestart.mp3", false);
	
//	AudioEngine::sharedInstance()->playEffect("ment_ready.mp3");

	myGIM->startItemSetting();
	myGIM->startCounting();

	if(myDSH->getIntegerForKey(kDSH_Key_showedScenario) == 5)
	{
		myDSH->setIntegerForKey(kDSH_Key_showedScenario, 6);
		
		bool t_jack_stun = myJack->isStun;
		
		mySGD->is_on_maingame = false;
		
		CCNode* exit_target = this;
		mControl->setTouchEnabled(false);
		exit_target->onExit();
		
		CCNode* scenario_node = CCNode::create();
		getParent()->addChild(scenario_node, 9999);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f;
		
		CCNode* t_stencil_node = CCNode::create();
		
		CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
		t_clipping->setAlphaThreshold(0.1f);
		
		CCPoint change_origin = ccp(0,0);
		t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, 480, 320*screen_scale_y));
		
		CCSprite* t_gray = CCSprite::create("back_gray.png");
		t_gray->setScaleX(screen_scale_x);
		t_gray->setScaleY(screen_scale_y);
		t_gray->setOpacity(0);
		t_gray->setPosition(ccp(240,myDSH->ui_center_y));
		t_clipping->addChild(t_gray);
		
		t_clipping->setInverted(true);
		scenario_node->addChild(t_clipping, 0);
		
		
		CCSprite* asuka = CCSprite::create("kt_cha_ikaruga_1.png");
		asuka->setScale(1.f-((360-myDSH->ui_top)*3.f/16.f)/100.f);
		asuka->setAnchorPoint(ccp(1,0));
		asuka->setPosition(ccp(480+asuka->getContentSize().width*asuka->getScale(), 0));
		scenario_node->addChild(asuka, 1);
		
		TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 60), ccp(255, 60), ccp(480.f-56.f,32.5f));
		typing_box->setBoxScale(myDSH->screen_convert_rate);
		typing_box->setHide();
		scenario_node->addChild(typing_box, 2);
		
		CCSprite* n_skip = CCSprite::create("kt_skip.png");
		CCSprite* s_skip = CCSprite::create("kt_skip.png");
		s_skip->setColor(ccGRAY);
		
		CCMenuLambda* skip_menu = CCMenuLambda::create();
		skip_menu->setPosition(ccp(35, myDSH->ui_top - 25 + 150));
		scenario_node->addChild(skip_menu, 3);
		skip_menu->setTouchPriority(-19999);
		skip_menu->setEnabled(false);
		
		CCMenuItemLambda* skip_item = CCMenuItemSpriteLambda::create(n_skip, s_skip, [=](CCObject* sender)
																	 {
																		 skip_menu->setEnabled(false);
																		 
																		 mControl->isStun = false;
																		 myJack->isStun = t_jack_stun;
																		 
																		 exit_target->onEnter();
																		 mySGD->is_on_maingame = true;
																		 startControl();
																		 ((ControlJoystickButton*)mControl)->resetTouch();
																		 
																		 addChild(KSTimer::create(0.1f, [=]()
																								  {
																									  scenario_node->removeFromParent();
																								  }));
																	 });
		skip_menu->addChild(skip_item);
		
		typing_box->showAnimation(0.3f);
		
		function<void()> end_func4 = [=]()
		{
			scenario_node->removeChildByTag(3);
			
			skip_menu->setEnabled(false);
			
			mControl->isStun = false;
			myJack->isStun = t_jack_stun;
			
			exit_target->onEnter();
			mySGD->is_on_maingame = true;
			startControl();
			((ControlJoystickButton*)mControl)->resetTouch();
			
			addChild(KSTimer::create(0.1f, [=]()
									 {
										 scenario_node->removeFromParent();
									 }));
		};
		
		function<void()> end_func3 = [=]()
		{
			skip_menu->setVisible(true);
			
			scenario_node->removeChildByTag(2);
			
			CCSprite* t_arrow3 = CCSprite::create("kt_arrow_big.png");
			t_arrow3->setRotation(90);
			t_arrow3->setPosition(ccp(480-35, myDSH->ui_top-25-65));
			scenario_node->addChild(t_arrow3, 4, 3);
			
			CCMoveTo* down_action = CCMoveTo::create(0.4f, ccp(480-35, myDSH->ui_top-25-95));
			CCMoveTo* up_action = CCMoveTo::create(0.4f, ccp(480-35, myDSH->ui_top-25-65));
			CCSequence* t_seq = CCSequence::create(down_action, up_action, NULL);
			CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
			
			t_arrow3->runAction(t_repeat);
			
			typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent25), end_func4);
		};
		
		function<void()> end_func2 = [=]()
		{
			skip_menu->setVisible(false);
			
			scenario_node->removeChildByTag(1);
			
			CCSprite* t_arrow2 = CCSprite::create("kt_arrow_big.png");
			t_arrow2->setRotation(90);
			t_arrow2->setPosition(ccp(25, myDSH->ui_top-25-50));
			scenario_node->addChild(t_arrow2, 4, 2);
			
			CCMoveTo* down_action = CCMoveTo::create(0.4f, ccp(25, myDSH->ui_top-25-80));
			CCMoveTo* up_action = CCMoveTo::create(0.4f, ccp(25, myDSH->ui_top-25-50));
			CCSequence* t_seq = CCSequence::create(down_action, up_action, NULL);
			CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
			
			t_arrow2->runAction(t_repeat);
			
			typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent24), end_func3);
		};
		
		function<void()> end_func1 = [=]()
		{
			scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.2f, [=](float t)
																  {
																	  t_gray->setOpacity(255-255*t);
																  }, [=](float t)
																  {
																	  t_gray->setOpacity(0);
																	  t_gray->removeFromParent();
																  }));
			
			CCSprite* t_arrow1 = CCSprite::create("kt_arrow_big.png");
			t_arrow1->setPosition(ccp(110, myDSH->ui_center_y));
			scenario_node->addChild(t_arrow1, 4, 1);
			
			CCMoveTo* right_action = CCMoveTo::create(0.4f, ccp(140, myDSH->ui_center_y));
			CCMoveTo* left_action = CCMoveTo::create(0.4f, ccp(110, myDSH->ui_center_y));
			CCSequence* t_seq = CCSequence::create(right_action, left_action, NULL);
			CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
			
			t_arrow1->runAction(t_repeat);
			
			typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent23), end_func2);
		};
		
		scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
															  {
																  t_gray->setOpacity(255*t);
																  asuka->setPositionX(480+asuka->getContentSize().width*asuka->getScale() - asuka->getContentSize().width*asuka->getScale()*2.f/3.f*t);
																  skip_menu->setPositionY(myDSH->ui_top - 25 + 150 - 150*t);
															  }, [=](float t)
															  {
																  t_gray->setOpacity(255);
																  asuka->setPositionX(480+asuka->getContentSize().width*asuka->getScale() - asuka->getContentSize().width*asuka->getScale()*2.f/3.f*t);
																  skip_menu->setPositionY(myDSH->ui_top - 25 + 150 - 150*t);
																  skip_menu->setEnabled(true);
																  
																  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent22), end_func1);
															  }));
	}
	else if(mySGD->is_endless_mode && myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
	{
		bool t_jack_stun = myJack->isStun;
		
		mySGD->is_on_maingame = false;
		
		CCNode* exit_target = this;
		mControl->setTouchEnabled(false);
		exit_target->onExit();
		
		CCNode* scenario_node = CCNode::create();
		getParent()->addChild(scenario_node, 9999);
		
		float screen_scale_y = myDSH->ui_top/320.f;
		
		CCNode* t_stencil_node = CCNode::create();
		
		CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
		t_clipping->setAlphaThreshold(0.1f);
		
		CCPoint change_origin = ccp(0,0);
		t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, 480, 320*screen_scale_y));
		
//		CCSprite* t_gray = CCSprite::create("back_gray.png");
//		t_gray->setScaleY(screen_scale_y);
//		t_gray->setOpacity(0);
//		t_gray->setPosition(ccp(240,myDSH->ui_center_y));
//		t_clipping->addChild(t_gray);
		
		t_clipping->setInverted(true);
		scenario_node->addChild(t_clipping, 0);
		
		
		CCSprite* asuka = CCSprite::create("kt_cha_ikaruga_1.png");
		asuka->setScale(1.f-((360-myDSH->ui_top)*3.f/16.f)/100.f);
		asuka->setAnchorPoint(ccp(1,0));
		asuka->setPosition(ccp(480+asuka->getContentSize().width*asuka->getScale(), 0));
		scenario_node->addChild(asuka, 1);
		
		TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 60), ccp(255, 60), ccp(480.f-56.f,32.5f));
		typing_box->setBoxScale(myDSH->screen_convert_rate);
		typing_box->setHide();
		scenario_node->addChild(typing_box, 2);
		
		typing_box->showAnimation(0.3f);
		
		function<void()> end_func3 = [=]()
		{
			mControl->isStun = false;
			myJack->isStun = t_jack_stun;
			
			exit_target->onEnter();
			mySGD->is_on_maingame = true;
			startControl();
			((ControlJoystickButton*)mControl)->resetTouch();
			
			addChild(KSTimer::create(0.1f, [=]()
									 {
										 scenario_node->removeFromParent();
									 }));
		};
		
		function<void()> end_func2 = [=]()
		{
			scenario_node->removeChildByTag(1);
			
			typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent59), end_func3);
		};
		
		function<void()> end_func1 = [=]()
		{
			CCSprite* t_arrow1 = CCSprite::create("kt_arrow_big.png");
			t_arrow1->setPosition(ccp(110, myDSH->ui_center_y+65));
			scenario_node->addChild(t_arrow1, 1, 1);
			
			CCMoveTo* right_action = CCMoveTo::create(0.4f, t_arrow1->getPosition() + ccp(30,0));
			CCMoveTo* left_action = CCMoveTo::create(0.4f, t_arrow1->getPosition());
			CCSequence* t_seq = CCSequence::create(right_action, left_action, NULL);
			CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
			
			t_arrow1->runAction(t_repeat);
			
//			CCSprite* t_arrow2 = CCSprite::create("kt_arrow_big.png");
//			t_arrow2->setPosition(ccp(480-110, myDSH->ui_center_y+75));
//			scenario_node->addChild(t_arrow2, 1, 2);
//			
//			CCMoveTo* left_action2 = CCMoveTo::create(0.4f, t_arrow2->getPosition() + ccp(-30,0));
//			CCMoveTo* right_action2 = CCMoveTo::create(0.4f, t_arrow2->getPosition());
//			CCSequence* t_seq2 = CCSequence::create(left_action2, right_action2, NULL);
//			CCRepeatForever* t_repeat2 = CCRepeatForever::create(t_seq2);
//			
//			t_arrow2->runAction(t_repeat2);
			
			typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent58), end_func2);
		};
		
		scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
															  {
																  asuka->setPositionX(480+asuka->getContentSize().width*asuka->getScale() - asuka->getContentSize().width*asuka->getScale()*2.f/3.f*t);
															  }, [=](float t)
															  {
																  asuka->setPositionX(480+asuka->getContentSize().width*asuka->getScale() - asuka->getContentSize().width*asuka->getScale()*2.f/3.f*t);
																  
																  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent57), end_func1);
															  }));
		
		
		
		
		
		
		
		
		
		
		
		
		
		
//		CCNode* tutorial_node = CCNode::create();
//		getParent()->addChild(tutorial_node);
//		
//		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//		if(screen_scale_x < 1.f)
//			screen_scale_x = 1.f;
//		
//		CCNode* t_stencil_node = CCNode::create();
//		CCScale9Sprite* t_stencil1 = CCScale9Sprite::create("rank_normal.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
//		t_stencil1->setContentSize(CCSizeMake(320*thumb_texture->getScale(), 430*thumb_texture->getScale()) + CCSizeMake(20, 40));
//		t_stencil1->setPosition(ccp(40, myDSH->ui_center_y-10));
//		t_stencil_node->addChild(t_stencil1);
//		
//		CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
//		t_clipping->setAlphaThreshold(0.1f);
//		
//		float screen_scale_y = myDSH->ui_top/320.f;
//		
//		float change_scale = 1.f;
//		CCPoint change_origin = ccp(0,0);
////		if(screen_scale_x > 1.f)
////		{
////			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
////			change_scale = screen_scale_x;
////		}
////		if(screen_scale_y > 1.f)
////			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
//		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
//		
//		
//		CCSprite* t_gray = CCSprite::create("back_gray.png");
//		t_gray->setScaleX(screen_scale_x);
//		t_gray->setScaleY(myDSH->ui_top/320.f);
//		t_gray->setOpacity(0);
//		t_gray->setPosition(ccp(240,myDSH->ui_center_y));
//		t_clipping->addChild(t_gray);
//		
//		CCSprite* t_arrow1 = CCSprite::create("main_tutorial_arrow1.png");
//		t_arrow1->setRotation(-90);
//		t_arrow1->setPosition(t_stencil1->getPosition() + ccp(t_stencil1->getContentSize().width/2.f + 15,0));
//		t_clipping->addChild(t_arrow1);
//		
//		StyledLabelTTF* t_ment1 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent6), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kLeftAlignment);
//		t_ment1->setAnchorPoint(ccp(0.f,0.5f));
//		t_ment1->setPosition(t_arrow1->getPosition() + ccp(t_arrow1->getContentSize().width/2.f + 3, 0));
//		t_clipping->addChild(t_ment1);
//		
//		CCSprite* t_arrow2 = CCSprite::create("main_tutorial_arrow1.png");
//		t_arrow2->setRotation(-90);
//		t_arrow2->setPosition(t_stencil1->getPosition() + ccp(t_stencil1->getContentSize().width/2.f + 15, -25));
//		t_clipping->addChild(t_arrow2);
//		
//		StyledLabelTTF* t_ment2 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent7), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kLeftAlignment);
//		t_ment2->setAnchorPoint(ccp(0.f,0.5f));
//		t_ment2->setPosition(t_arrow2->getPosition() + ccp(t_arrow2->getContentSize().width/2.f + 3, 0));
//		t_clipping->addChild(t_ment2);
//		
//		CCSprite* t_arrow3 = CCSprite::create("main_tutorial_arrow1.png");
//		t_arrow3->setRotation(-90);
//		t_arrow3->setPosition(t_stencil1->getPosition() + ccp(t_stencil1->getContentSize().width/2.f + 15, -43));
//		t_clipping->addChild(t_arrow3);
//		
//		StyledLabelTTF* t_ment3 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent8), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kLeftAlignment);
//		t_ment3->setAnchorPoint(ccp(0.f,0.5f));
//		t_ment3->setPosition(t_arrow3->getPosition() + ccp(t_arrow3->getContentSize().width/2.f + 3, 0));
//		t_clipping->addChild(t_ment3);
//		
//		
//		float item_scale = 0.18f;
//		CCPoint item_base_position = ccp(40, myDSH->ui_center_y-10) + ccpMult(ccp(-160,-215), 0.17f) + ccp(7,-7);
//		CCPoint distance_position = ccp(14,0);
//		
//		CCNode* temp_item_node = CCNode::create();
//		tutorial_node->addChild(temp_item_node);
//		
//		vector<ITEM_CODE> t_vec_list;
//		t_vec_list.clear();
//		t_vec_list.push_back(kIC_baseSpeedUp);
//		t_vec_list.push_back(kIC_doubleItem);
//		t_vec_list.push_back(kIC_magnet);
//		t_vec_list.push_back(kIC_heartUp);
//		
//		for(int i=0;i<t_vec_list.size();i++)
//		{
//			GraySprite* item_img = GraySprite::create(CCString::createWithFormat("item%d.png", t_vec_list[i])->getCString());
//			item_img->setScale(item_scale);
//			item_img->setOpacity(150);
//			item_img->setPosition(ccpAdd(item_base_position, ccpMult(distance_position, i)));
//			temp_item_node->addChild(item_img);
//			
//			CCSprite* item_case = CCSprite::create("ingame_itembox.png");
//			item_case->setPosition(ccp(item_img->getContentSize().width/2.f, item_img->getContentSize().height/2.f));
//			item_case->setScale(1.f/0.4f);
//			item_case->setOpacity(150);
//			item_img->addChild(item_case, -1);
//		}
//		
//		
//		TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-9999);
//		tutorial_node->addChild(t_suction);
//		t_suction->setTouchEnabled(true);
//		
//		t_clipping->setInverted(true);
//		tutorial_node->addChild(t_clipping, 9999);
//		
//		tutorial_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
//											   {
//												   t_gray->setOpacity(t*255);
//											   }, [=](float t)
//											   {
//												   t_gray->setOpacity(255);
//												   
//												   
//												   t_suction->is_on_touch_began_func = true;
//												   t_suction->touch_began_func = [=]()
//												   {
//													   t_suction->is_on_touch_began_func = false;
//													   
//													   temp_item_node->removeFromParent();
//													   
//													   t_arrow1->removeFromParent();
//													   t_ment1->removeFromParent();
//													   
//													   t_arrow2->removeFromParent();
//													   t_ment2->removeFromParent();
//													   
//													   t_arrow3->removeFromParent();
//													   t_ment3->removeFromParent();
//													   
//													   t_stencil1->setPosition(ccp(440, myDSH->ui_center_y-10));
//													   
//													   CCSprite* t_arrow4 = CCSprite::create("main_tutorial_arrow1.png");
//													   t_arrow4->setRotation(90);
//													   t_arrow4->setPosition(t_stencil1->getPosition() + ccp(-t_stencil1->getContentSize().width/2.f - 15, 0));
//													   t_clipping->addChild(t_arrow4);
//													   
//													   StyledLabelTTF* t_ment4 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent9), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kRightAlignment);
//													   t_ment4->setAnchorPoint(ccp(1.f,0.5f));
//													   t_ment4->setPosition(t_arrow4->getPosition() + ccp(-t_arrow4->getContentSize().width/2.f - 3, 0));
//													   t_clipping->addChild(t_ment4);
//													   
//													   t_suction->is_on_touch_began_func = true;
//													   t_suction->touch_began_func = [=]()
//													   {
//														   t_suction->is_on_touch_began_func = false;
//														   
//														   t_arrow4->removeFromParent();
//														   t_ment4->removeFromParent();
//														   
//														   t_stencil1->setContentSize(CCSizeMake(70, 70));
//														   t_stencil1->setPosition(ccp(40, myDSH->ui_center_y-10 + 75));
//														   
//														   CCLabelBMFont* t_combo_label = CCLabelBMFont::create("288", "combo.fnt");
//														   t_combo_label->setAnchorPoint(ccp(0.5f,0.5f));
//														   t_combo_label->setPosition(t_stencil1->getPosition() + ccp(0,-4));
//														   t_combo_label->setScale(1.f/game_node->getScale());
//														   tutorial_node->addChild(t_combo_label);
//														   
//														   CCSprite* t_combo_front = CCSprite::create("combo_front.png");
//														   t_combo_front->setAnchorPoint(ccp(0.5f,0.5f));
//														   t_combo_front->setPosition(t_stencil1->getPosition() + ccp(0,-10));
//														   t_combo_front->setScale(0.6f/game_node->getScale());
//														   tutorial_node->addChild(t_combo_front);
//														   
//														   CCSprite* t_arrow5 = CCSprite::create("main_tutorial_arrow1.png");
//														   t_arrow5->setRotation(-90);
//														   t_arrow5->setPosition(t_stencil1->getPosition() + ccp(t_stencil1->getContentSize().width/2.f + 15, 0));
//														   t_clipping->addChild(t_arrow5);
//														   
//														   StyledLabelTTF* t_ment5 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent10), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kLeftAlignment);
//														   t_ment5->setAnchorPoint(ccp(0.f,0.5f));
//														   t_ment5->setPosition(t_arrow5->getPosition() + ccp(t_arrow5->getContentSize().width/2.f + 3, 0));
//														   t_clipping->addChild(t_ment5);
//														   
//														   t_suction->is_on_touch_began_func = true;
//														   t_suction->touch_began_func = [=]()
//														   {
//															   t_suction->is_on_touch_began_func = false;
//															   
//															   t_combo_label->removeFromParent();
//															   t_combo_front->removeFromParent();
//															   
//															   t_arrow5->removeFromParent();
//															   t_ment5->removeFromParent();
//															   
//															   t_stencil1->setPosition(ccp(-100,-100));
//															   
//															   KSLabelTTF* t_final_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent11), mySGD->getFont().c_str(), 15);
//															   t_final_label->setColor(ccc3(100, 200, 255));
//															   t_final_label->enableOuterStroke(ccBLACK, 2.f);
//															   t_final_label->setPosition(ccp(240, 50));
//															   t_clipping->addChild(t_final_label);
//															   
//															   t_suction->is_on_touch_began_func = true;
//															   t_suction->touch_began_func = [=]()
//															   {
//																   t_suction->is_on_touch_began_func = false;
//																   
//																   t_final_label->removeFromParent();
//																   
//																   tutorial_node->addChild(KSGradualValue<float>::create(1.f, 0.f, 1.f, [=](float t)
//																														 {
//																															 t_gray->setOpacity(255*t);
//																														 }, [=](float t)
//																														 {
//																															 t_gray->setOpacity(255*t);
//																															 
//																															 mControl->isStun = false;
//																															 myJack->isStun = t_jack_stun;
//																															 
//																															 exit_target->onEnter();
//																															 startControl();
//																															 ((ControlJoystickButton*)mControl)->resetTouch();
//																															 tutorial_node->removeFromParent();
//																														 }));
//															   };
//														   };
//													   };
//												   };
//												   
//											   }));
	}
	
	schedule(schedule_selector(Maingame::counting));
}

void Maingame::checkingBacking()
{
	if(!isCheckingBacking)
	{
		unschedule(schedule_selector(Maingame::checkingBacking));
		myJack->isStun = false;
		mControl->isStun = false;
		if(myJack->t_se)
		{
			myJack->t_se->selfRemove();
		}
		myJack->t_se = NULL;
		//			mControl->setTouchEnabled(true);
	}
}

void Maingame::stunBackTracking()
{
	myJack->willBackTracking = true;
	if(!myJack->isMoving)
	{
		isCheckingBacking = true;
		AudioEngine::sharedInstance()->playEffect("se_drawcancel.mp3", false);
//		AudioEngine::sharedInstance()->playEffect("sound_returntojack.mp3", false);
		schedule(schedule_selector(Maingame::stunBacking));
	}
}

void Maingame::stopBackingCheck()
{
	unschedule(schedule_selector(Maingame::checkingBacking));
	
	if(isCheckingBacking)
	{
		myJack->endBackTracking();
		isCheckingBacking = false;
	}
	unschedule(schedule_selector(Maingame::stunBacking));
	
	if(((ControlJoystickButton*)mControl)->isBacking)
	{
		((ControlJoystickButton*)mControl)->isBacking = false;
		myJack->endBackTracking();
	}
	unschedule(schedule_selector(Maingame::backTracking));
	
	myJack->willBackTracking = false;
}

void Maingame::stunBacking()
{
	IntPoint afterJackPoint = myPM->pathBackTracking();

	if(afterJackPoint.isNull())
	{
		myGD->communication("SW_stopAllSW");
		
		unschedule(schedule_selector(Maingame::stunBacking));
		myJack->endBackTracking();
		myJack->willBackTracking = false;
		isCheckingBacking = false;
		return;
	}
	else
	{
		myJack->backTrackingAtAfterMoving(afterJackPoint);
	}

	afterJackPoint = myPM->pathBackTracking();

	if(afterJackPoint.isNull())
	{
		myGD->communication("SW_stopAllSW");
		
		unschedule(schedule_selector(Maingame::stunBacking));
		myJack->endBackTracking();
		myJack->willBackTracking = false;
		isCheckingBacking = false;
	}
	else
	{
		myJack->backTrackingAtAfterMoving(afterJackPoint);
	}
}

void Maingame::readyBackTracking()
{
	//		mControl->setTouchEnabled(false);
	((ControlJoystickButton*)mControl)->stopMySchedule();
	myJack->willBackTracking = true;
	if(!myJack->isMoving)
		startBackTracking();
}

void Maingame::startBackTracking()
{
	AudioEngine::sharedInstance()->playEffect("se_drawcancel.mp3", false);
//	AudioEngine::sharedInstance()->playEffect("sound_returntojack.mp3", false);
	((ControlJoystickButton*)mControl)->isBacking = true;
	schedule(schedule_selector(Maingame::backTracking));
}

void Maingame::stopBackTracking()
{
	myGD->communication("SW_stopAllSW");
	
	unschedule(schedule_selector(Maingame::backTracking));
	((ControlJoystickButton*)mControl)->isBacking = false;
	myJack->endBackTracking();
	myJack->willBackTracking = false;
	//		if(!mControl->isStun)
	//			mControl->setTouchEnabled(true);
}

void Maingame::pauseBackTracking()
{
	unschedule(schedule_selector(Maingame::backTracking));
	((ControlJoystickButton*)mControl)->isBacking = false;
	myJack->willBackTracking = false;
}

void Maingame::allStopSchedule()
{
	myJack->stopJack();
	// myMS : nothing schedule
	// myPM : nothing schedule
	myCP->allStopSchedule();
	stopBackTracking();
	mControl->setTouchEnabled(false);
}

void Maingame::startSpecialAttack()
{
	myJack->changeDirection(directionStop, directionStop);
//	mControl->isStun = true;
	//		mControl->setTouchEnabled(false);
}

void Maingame::gameover()
{
	mySGD->resetLabels();

	int clear_info;
	if(!mySGD->getIsCleared())
	{
		if(mySD->getClearCondition() == kCLEAR_hellMode)
			mySGD->gameOver(myUI->getUseTime()*500, myUI->getPercentage(), myUI->getUseTime());
		else
			mySGD->gameOver(myUI->getScore(), myUI->getPercentage(), myUI->getUseTime());
		clear_info = -1;
	}
	else
		clear_info = 1;

	myGD->communication("UI_writeGameOver", clear_info);
	
	mControl->setVisible(false);

	AudioEngine::sharedInstance()->stopSound();

	myUI->stopCounting();
	
	if(mySGD->getIsCleared())
	{
//		AudioEngine::sharedInstance()->playEffect("sound_clear_bgm.mp3", false);
//			AudioEngine::sharedInstance()->playEffect("bgm_gameclear.mp3");
//		AudioEngine::sharedInstance()->playEffect("sound_clear_ment.mp3", false);
		AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("ment_stageclear%d.mp3", rand()%2+2)->getCString(), false, true);
		ClearShowTime* t_cst = ClearShowTime::create(myUI->getIsExchanged(), myUI->getPercentage() >= 1.f, game_node, this, callfunc_selector(Maingame::clearScenario));
		game_node->addChild(t_cst, clearshowtimeZorder);

		t_cst->startClearView(game_node->getPosition());

		myUI->setVisible(false);
	}
	else
	{
//		AudioEngine::sharedInstance()->playEffect("sound_gameover_bgm.mp3", false);
		AudioEngine::sharedInstance()->playEffect("bgm_gameover.mp3");
//		AudioEngine::sharedInstance()->playEffect("sound_gameover_ment.mp3", false);
		AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("ment_gameover%d.mp3", rand()%3+1)->getCString(), false, true);

		AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
		
		KSLabelTTF* game_over_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_failTitleGameover), mySGD->getFont().c_str(), 45);
		game_over_label->setGradientColor(ccc4(255, 115, 250, 255), ccc4(215, 60, 130, 255), ccp(0,-1));
		game_over_label->enableOuterStroke(ccc3(65, 5, 35), 2.5f, 255, true);
		game_over_label->setPosition(ccp(240,myDSH->ui_center_y+93));
		game_over_label->setOpacity(0);
		myUI->addChild(game_over_label);
		
		KSLabelTTF* shadow = CommonAnimation::applyBigShadow(game_over_label, game_over_label->getFontSize());
		
		game_over_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 13.f/30.f, [=](float t)
																{
																	float convert_t;
																	if (t < 1 / 2.75)
																	{
																		convert_t = 7.5625f * t * t;
																	} else if (t < 2 / 2.75)
																	{
																		t -= 1.5f / 2.75f;
																		convert_t = 7.5625f * t * t + 0.75f;
																	} else if(t < 2.5 / 2.75)
																	{
																		t -= 2.25f / 2.75f;
																		convert_t = 7.5625f * t * t + 0.9375f;
																	}
																	
																	t -= 2.625f / 2.75f;
																	convert_t = 7.5625f * t * t + 0.984375f;
																	
																	game_over_label->setPosition(ccp(240,myDSH->ui_center_y+93-93*convert_t));
																	game_over_label->setOpacity(t*255);
																	shadow->setOpacityOuterStroke(255 * 0.3f*(t));
																}, [=](float t)
																{
																	game_over_label->setPosition(ccp(240,myDSH->ui_center_y));
																	game_over_label->setOpacity(255);
																	shadow->setOpacityOuterStroke(255 * 0.3f*(t));
																	
																	game_over_label->addChild(KSTimer::create(32.f/30.f, [=]()
																											  {
																												  game_over_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 5.f/30.f, [=](float t)
																																										  {
																																											  game_over_label->setScale(1.f+t*0.6f);
																																											  game_over_label->setOpacity(255-t*255);
																																											  shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																										  }, [=](float t)
																																										  {
																																											  game_over_label->setScale(1.6f);
																																											  game_over_label->setOpacity(0);
																																											  shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																											  
																																											  addChild(KSTimer::create(10.f/30.f, [=]()
																																																	   {
																																																		   myGD->hideBosses();
																																																		   hideThumb();
																																																		   failScenario();
																																																	   }));
																																											  
																																											  game_over_label->removeFromParent();
																																										  }));
																											  }));
																}));
	}
}

void Maingame::clearScenario()
{
	myMS->setVisible(false);
	if(mySGD->is_endless_mode)
	{
		CCNode* curtain_node = LoadingTipScene::getCurtainTipImage();
		curtain_node->setPosition(ccp(240,myDSH->ui_center_y));
		curtain_node->setScale(myDSH->screen_convert_rate);
		addChild(curtain_node, shutterZorder+5);
		
		CCDelayTime* t_delay = CCDelayTime::create(0.7f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Maingame::closeShutter));
		CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
		curtain_node->runAction(t_seq);
	}
	else
	{
		intro_boss = CumberShowWindow::create(mySD->getSilType(), kCumberShowWindowSceneCode_cardChange);
		intro_boss->setPosition(ccp(240,myDSH->ui_center_y+400));
		intro_boss->setScale(1.8f);
		addChild(intro_boss, searchEyeZorder);
		
		CCDelayTime* t_delay = CCDelayTime::create(1.f);
		CCMoveTo* t_move = CCMoveTo::create(0.7f, ccp(240,myDSH->ui_center_y+10));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Maingame::clearScenario2));
		CCSequence* t_seq = CCSequence::create(t_delay, t_move, t_call, NULL);
		intro_boss->runAction(t_seq);
	}
}

void Maingame::clearScenario2()
{
	TypingBox* t_tb = TypingBox::create(-500, "boss_mentbox.png", CCRectMake(0, 0, 110, 75), CCRectMake(87, 37, 7, 15), CCRectMake(15, 30, 80, 31), CCSizeMake(180, 40), ccp(240,myDSH->ui_center_y-65), ccp(480.f-56.f,32.5f));
	t_tb->setBoxScale(myDSH->screen_convert_rate);
	addChild(t_tb, 100);
	
	function<void()> end_func1 = [=]()
	{
		t_tb->setVisible(false);
		
		CCDelayTime* t_delay1 = CCDelayTime::create(0.5f);
		CCMoveTo* t_move1 = CCMoveTo::create(0.3f, ccp(240,myDSH->ui_center_y+600));
		CCCallFunc* t_call0 = CCCallFunc::create(this, callfunc_selector(Maingame::clearScenario3));
		CCCallFunc* t_call1 = CCCallFunc::create(intro_boss, callfunc_selector(CCNode::removeFromParent));
		CCSequence* t_seq1 = CCSequence::create(t_delay1, t_move1, t_call0, t_call1, NULL);
		
		intro_boss->runAction(t_seq1);
		
		t_tb->removeFromParent();
	};
	
	t_tb->startTyping(intro_boss->clear_ment.c_str(), end_func1);
	
//	StoryManager* t_sm = StoryManager::create(-500);
//	addChild(t_sm, 100);
//	
//	t_sm->addMent(true, "", "", intro_boss->clear_ment.c_str(), [=]()
//				  //"파란 실루엣 영역을 획득해야 게임 달성도가 올라갑니다.", [=]()
//				  {
//					  CCDelayTime* t_delay1 = CCDelayTime::create(0.5f);
//					  CCMoveTo* t_move1 = CCMoveTo::create(0.3f, ccp(240,myDSH->ui_center_y+600));
//					  CCCallFunc* t_call0 = CCCallFunc::create(this, callfunc_selector(Maingame::clearScenario3));
//					  CCCallFunc* t_call1 = CCCallFunc::create(intro_boss, callfunc_selector(CCNode::removeFromParent));
//					  CCSequence* t_seq1 = CCSequence::create(t_delay1, t_move1, t_call0, t_call1, NULL);
//					  
//					  intro_boss->runAction(t_seq1);
//					  
//					  //					  t_sm->addMent(true, "", "", "시노비결계!", nullptr);
//					  
//					  
////					  ASPopupView* t_popup = ASPopupView::create(-600);
////					  
////					  float height_value = myDSH->ui_top;
////					  
////					  t_popup->setDimmedSize(CCSizeMake(480.f, height_value));
////					  t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
////					  
////					  CCSprite* dimmed_img = t_popup->getDimmedSprite();
////					  dimmed_img->setOpacity(0);
////					  dimmed_img->runAction(CCFadeTo::create(0.3f, 100));
////					  
////					  CCNode* t_container = CCNode::create();
////					  t_popup->setContainerNode(t_container);
////					  addChild(t_popup, shutterZorder+5);
////					  
////					  t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
////					  
////					  CCSprite* title_img = CCSprite::create(CCString::createWithFormat("ending_reward_%s.png", myLoc->getSupportLocalCode())->getCString());
////					  title_img->setPosition(ccp(0,75));
////					  title_img->setOpacity(0);
////					  t_container->addChild(title_img);
////					  
////					  KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_selectClearBox), mySGD->getFont().c_str(), 18);
////					  title_label->enableOuterStroke(ccBLACK, 1.f);
////					  title_label->setPosition(ccp(0,40));
////					  title_label->setOpacity(0);
////					  t_container->addChild(title_label);
////					  
////					  int reward_type;
////					  int random_value;
////					  random_value = rand()%1000;
////					  int gold_get_rate = 500;
////					  int gold_or_item_get_rate = gold_get_rate + 300;
////					  
////					  if(random_value < gold_get_rate)
////					  {
////						  // gold
////						  reward_type = 1;
////					  }
////					  else if(random_value < gold_or_item_get_rate)
////					  {
////						  // gold or item
////						  reward_type = 2;
////					  }
////					  else
////					  {
////						  // stone
////						  reward_type = 3;
////					  }
////					  
////					  CCSprite* goldbox1 = CCSprite::create("goldbox_off.png");
////					  goldbox1->setOpacity(0);
////					  goldbox1->setPosition(ccp(0,-30));
////					  t_container->addChild(goldbox1);
////					  
////					  CCSprite* goldbox2 = CCSprite::create("goldbox_off.png");
////					  goldbox2->setOpacity(0);
////					  goldbox2->setPosition(ccp(0,-30));
////					  t_container->addChild(goldbox2);
////					  
////					  CCSprite* goldbox3 = CCSprite::create("goldbox_off.png");
////					  goldbox3->setOpacity(0);
////					  goldbox3->setPosition(ccp(0,-30));
////					  t_container->addChild(goldbox3);
////					  
////					  
////					  CCMenuLambda* goldbox_menu = CCMenuLambda::create();
////					  goldbox_menu->setPosition(ccp(0,-30));
////					  t_container->addChild(goldbox_menu);
////					  goldbox_menu->setVisible(false);
////					  goldbox_menu->setTouchPriority(t_popup->getTouchPriority()-1);
////					  
////					  
////					  CCSprite* n_goldbox1 = CCSprite::create("goldbox_off.png");
////					  n_goldbox1->setOpacity(0);
////					  CCSprite* s_goldbox1 = CCSprite::create("goldbox_off.png");
////					  s_goldbox1->setOpacity(0);
////					  
////					  CCMenuItemLambda* goldbox1_item = CCMenuItemSpriteLambda::create(n_goldbox1, s_goldbox1, [=](CCObject* sender)
////																					   {
////																						   AudioEngine::sharedInstance()->playEffect("se_clearreward.mp3", false);
////																						   
////																						   goldbox1->removeFromParent();
////																						   CCSprite* goldbox4 = CCSprite::create("goldbox_on.png");
////																						   goldbox4->setPosition(ccp(-150,-30));
////																						   t_container->addChild(goldbox4, 2);
////																						   goldbox4->runAction(CCFadeTo::create(0.5f, 0));
////																						   
////																						   goldbox2->removeFromParent();
////																						   CCSprite* goldbox5 = CCSprite::create("goldbox_on.png");
////																						   goldbox5->setPosition(ccp(0,-30));
////																						   t_container->addChild(goldbox5, 2);
////																						   goldbox5->runAction(CCFadeTo::create(0.5f, 0));
////																						   
////																						   goldbox3->removeFromParent();
////																						   CCSprite* goldbox6 = CCSprite::create("goldbox_on.png");
////																						   goldbox6->setPosition(ccp(150,-30));
////																						   t_container->addChild(goldbox6, 2);
////																						   goldbox6->runAction(CCFadeTo::create(0.5f, 0));
////																						   
////																						   
////																						   CCNode* gold_node = CCNode::create();
////																						   KSLabelTTF* item_gold = KSLabelTTF::create("GOLD 100", mySGD->getFont().c_str(), 12);
////																						   item_gold->enableOuterStroke(ccBLACK, 1.f);
////																						   item_gold->setPosition(ccp(0,20));
////																						   gold_node->addChild(item_gold);
////																						   CCSprite* t_gold_img = CCSprite::create("shop_gold3.png");
////																						   t_gold_img->setPosition(ccp(0, -20));
////																						   gold_node->addChild(t_gold_img);
////																						   
////																						   
////																						   CCNode* gold_item_node = CCNode::create();
////																						   KSLabelTTF* item_gold_or_item;
////																						   item_gold_or_item = KSLabelTTF::create("GOLD 200", mySGD->getFont().c_str(), 12);
////																						   item_gold_or_item->enableOuterStroke(ccBLACK, 1.f);
////																						   item_gold_or_item->setPosition(ccp(0,20));
////																						   gold_item_node->addChild(item_gold_or_item);
////																						   CCSprite* t_gold_img2 = CCSprite::create("shop_gold3.png");
////																						   t_gold_img2->setPosition(ccp(0, -20));
////																						   gold_item_node->addChild(t_gold_img2);
////																						   
////																						   
////																						   CCNode* stone_node = CCNode::create();
////																						   KSLabelTTF* item_stone = KSLabelTTF::create("GOLD 300", mySGD->getFont().c_str(), 12);
////																						   item_stone->enableOuterStroke(ccBLACK, 1.f);
////																						   item_stone->setPosition(ccp(0,20));
////																						   stone_node->addChild(item_stone);
////																						   CCSprite* t_stone_img = CCSprite::create("shop_gold3.png");
////																						   t_stone_img->setPosition(ccp(0, -20));
////																						   stone_node->addChild(t_stone_img);
////																						   
////																						   int random_left_right = rand()%2;
////																						   
////																						   CCNode* selected_node;
////																						   KSLabelTTF* another_label1;
////																						   KSLabelTTF* another_label2;
////																						   CCSprite* another_img1;
////																						   CCSprite* another_img2;
////																						   if(reward_type == 1)
////																						   {
////																							   mySGD->addChangeGoodsIngameGold(100);
////																							   gold_node->setPosition(ccp(-150,-30));
////																							   selected_node = gold_node;
////																							   another_label1 = item_gold_or_item;
////																							   another_img1 = t_gold_img2;
////																							   another_label2 = item_stone;
////																							   another_img2 = t_stone_img;
////																							   if(random_left_right == 0)
////																							   {
////																								   gold_item_node->setPosition(ccp(0,-30));
////																								   stone_node->setPosition(ccp(150,-30));
////																							   }
////																							   else
////																							   {
////																								   gold_item_node->setPosition(ccp(150,-30));
////																								   stone_node->setPosition(ccp(0,-30));
////																							   }
////																						   }
////																						   else if(reward_type == 2)
////																						   {
////																							   mySGD->addChangeGoodsIngameGold(200);
////																							   
////																							   gold_item_node->setPosition(ccp(-150,-30));
////																							   selected_node = gold_item_node;
////																							   another_label1 = item_gold;
////																							   another_img1 = t_gold_img;
////																							   another_label2 = item_stone;
////																							   another_img2 = t_stone_img;
////																							   if(random_left_right == 0)
////																							   {
////																								   gold_node->setPosition(ccp(0,-30));
////																								   stone_node->setPosition(ccp(150,-30));
////																							   }
////																							   else
////																							   {
////																								   gold_node->setPosition(ccp(150,-30));
////																								   stone_node->setPosition(ccp(0,-30));
////																							   }
////																						   }
////																						   else
////																						   {
////																							   mySGD->addChangeGoodsIngameGold(300);
////																							   
////																							   stone_node->setPosition(ccp(-150,-30));
////																							   selected_node = stone_node;
////																							   another_label1 = item_gold_or_item;
////																							   another_label2 = item_gold;
////																							   another_img1 = t_gold_img2;
////																							   another_img2 = t_gold_img;
////																							   if(random_left_right == 0)
////																							   {
////																								   gold_node->setPosition(ccp(0,-30));
////																								   gold_item_node->setPosition(ccp(150,-30));
////																							   }
////																							   else
////																							   {
////																								   gold_node->setPosition(ccp(150,-30));
////																								   gold_item_node->setPosition(ccp(0,-30));
////																							   }
////																						   }
////																						   
////																						   t_container->addChild(gold_node);
////																						   t_container->addChild(gold_item_node);
////																						   t_container->addChild(stone_node);
////																						   
////																							 
////																						   t_container->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.7f, [=](float t)
////																																			   {
////																																				   
////																																					 
////																																				   another_img1->setColor(ccc3(255-t*100, 255-t*100, 255-t*100));
////																																				   another_img2->setColor(ccc3(255-t*100, 255-t*100, 255-t*100));
////																																				   another_label1->setColor(ccc3(255-t*100, 255-t*100, 255-t*100));
////																																				   another_label2->setColor(ccc3(255-t*100, 255-t*100, 255-t*100));
////																																			   }, [=](float t)
////																																			   {
////																																				   another_img1->setColor(ccc3(155, 155, 155));
////																																				   another_img2->setColor(ccc3(155, 155, 155));
////																																				   another_label1->setColor(ccc3(155, 155, 155));
////																																				   another_label2->setColor(ccc3(155, 155, 155));
////																																			   }));
////																						   
////																						   goldbox_menu->setVisible(false);
////																						   
////																						   myDSH->saveAllUserData(nullptr);
////																						   
////																						   CCFadeTo* dimmed_fade = CCFadeTo::create(1.f, 0);
////																						   CCCallFunc* dimmed_call = CCCallFunc::create(this, callfunc_selector(Maingame::clearScenario3));
////																						   CCCallFunc* remove_call = CCCallFunc::create(t_popup, callfunc_selector(CCLayer::removeFromParent));
////																						   CCSequence* t_seq = CCSequence::create(CCDelayTime::create(1.0),dimmed_fade, dimmed_call, remove_call, NULL);
////																						   dimmed_img->runAction(t_seq);
////																					   });
////					  goldbox1_item->setPosition(ccp(-150,-30));
////					  goldbox_menu->addChild(goldbox1_item);
////					  
////					  
////					  CCSprite* n_goldbox2 = CCSprite::create("goldbox_off.png");
////					  n_goldbox2->setOpacity(0);
////					  CCSprite* s_goldbox2 = CCSprite::create("goldbox_off.png");
////					  s_goldbox2->setOpacity(0);
////					  
////					  CCMenuItemLambda* goldbox2_item = CCMenuItemSpriteLambda::create(n_goldbox2, s_goldbox2, [=](CCObject* sender)
////																					   {
////																						   AudioEngine::sharedInstance()->playEffect("se_clearreward.mp3", false);
////																						   
////																						   goldbox1->removeFromParent();
////																						   CCSprite* goldbox4 = CCSprite::create("goldbox_on.png");
////																						   goldbox4->setPosition(ccp(-150,-30));
////																						   t_container->addChild(goldbox4, 2);
////																						   goldbox4->runAction(CCFadeTo::create(0.5f, 0));
////																						   
////																						   goldbox2->removeFromParent();
////																						   CCSprite* goldbox5 = CCSprite::create("goldbox_on.png");
////																						   goldbox5->setPosition(ccp(0,-30));
////																						   t_container->addChild(goldbox5, 2);
////																						   goldbox5->runAction(CCFadeTo::create(0.5f, 0));
////																						   
////																						   goldbox3->removeFromParent();
////																						   CCSprite* goldbox6 = CCSprite::create("goldbox_on.png");
////																						   goldbox6->setPosition(ccp(150,-30));
////																						   t_container->addChild(goldbox6, 2);
////																						   goldbox6->runAction(CCFadeTo::create(0.5f, 0));
////																						   
////																						   CCNode* gold_node = CCNode::create();
////																						   KSLabelTTF* item_gold = KSLabelTTF::create("GOLD 100", mySGD->getFont().c_str(), 12);
////																						   item_gold->enableOuterStroke(ccBLACK, 1.f);
////																						   item_gold->setPosition(ccp(0,20));
////																						   gold_node->addChild(item_gold);
////																						   CCSprite* t_gold_img = CCSprite::create("shop_gold3.png");
////																						   t_gold_img->setPosition(ccp(0, -20));
////																						   gold_node->addChild(t_gold_img);
////																						   
////																						   
////																						   CCNode* gold_item_node = CCNode::create();
////																						   KSLabelTTF* item_gold_or_item;
////																						   item_gold_or_item = KSLabelTTF::create("GOLD 200", mySGD->getFont().c_str(), 12);
////																						   item_gold_or_item->enableOuterStroke(ccBLACK, 1.f);
////																						   item_gold_or_item->setPosition(ccp(0,20));
////																						   gold_item_node->addChild(item_gold_or_item);
////																						   CCSprite* t_gold_img2 = CCSprite::create("shop_gold3.png");
////																						   t_gold_img2->setPosition(ccp(0, -20));
////																						   gold_item_node->addChild(t_gold_img2);
////																						   
////																						   
////																						   CCNode* stone_node = CCNode::create();
////																						   KSLabelTTF* item_stone = KSLabelTTF::create("GOLD 300", mySGD->getFont().c_str(), 12);
////																						   item_stone->enableOuterStroke(ccBLACK, 1.f);
////																						   item_stone->setPosition(ccp(0,20));
////																						   stone_node->addChild(item_stone);
////																						   CCSprite* t_stone_img = CCSprite::create("shop_gold3.png");
////																						   t_stone_img->setPosition(ccp(0, -20));
////																						   stone_node->addChild(t_stone_img);
////																						   
////																						   int random_left_right = rand()%2;
////																						   
////																						   CCNode* selected_node;
////																						   KSLabelTTF* another_label1;
////																						   KSLabelTTF* another_label2;
////																						   CCSprite* another_img1;
////																						   CCSprite* another_img2;
////																						   
////																						   if(reward_type == 1)
////																						   {
////																							   mySGD->addChangeGoodsIngameGold(100);
////																							   
////																							   selected_node = gold_node;
////																							   another_label1 = item_gold_or_item;
////																							   another_img1 = t_gold_img2;
////																							   another_label2 = item_stone;
////																							   another_img2 = t_stone_img;
////																							   
////																							   gold_node->setPosition(ccp(0,-30));
////																							   if(random_left_right == 0)
////																							   {
////																								   gold_item_node->setPosition(ccp(-150,-30));
////																								   stone_node->setPosition(ccp(150,-30));
////																							   }
////																							   else
////																							   {
////																								   gold_item_node->setPosition(ccp(150,-30));
////																								   stone_node->setPosition(ccp(-150,-30));
////																							   }
////																						   }
////																						   else if(reward_type == 2)
////																						   {
////																							   mySGD->addChangeGoodsIngameGold(200);
////																							   
////																							   selected_node = gold_item_node;
////																							   another_label1 = item_gold;
////																							   another_img1 = t_gold_img;
////																							   another_label2 = item_stone;
////																							   another_img2 = t_stone_img;
////																							   
////																							   gold_item_node->setPosition(ccp(0,-30));
////																							   if(random_left_right == 0)
////																							   {
////																								   gold_node->setPosition(ccp(-150,-30));
////																								   stone_node->setPosition(ccp(150,-30));
////																							   }
////																							   else
////																							   {
////																								   gold_node->setPosition(ccp(150,-30));
////																								   stone_node->setPosition(ccp(-150,-30));
////																							   }
////																						   }
////																						   else
////																						   {
////																							   mySGD->addChangeGoodsIngameGold(300);
////																							   
////																							   selected_node = stone_node;
////																							   another_label1 = item_gold_or_item;
////																							   another_label2 = item_gold;
////																							   another_img1 = t_gold_img2;
////																							   another_img2 = t_gold_img;
////																							   
////																							   stone_node->setPosition(ccp(0,-30));
////																							   if(random_left_right == 0)
////																							   {
////																								   gold_node->setPosition(ccp(-150,-30));
////																								   gold_item_node->setPosition(ccp(150,-30));
////																							   }
////																							   else
////																							   {
////																								   gold_node->setPosition(ccp(150,-30));
////																								   gold_item_node->setPosition(ccp(-150,-30));
////																							   }
////																						   }
////																						   t_container->addChild(gold_node);
////																						   t_container->addChild(gold_item_node);
////																						   t_container->addChild(stone_node);
////																						   
////																						   t_container->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.7f, [=](float t)
////																																			   {
////																																				   selected_node->setScale(1.f+t*0.5f);
////																																				   another_img1->setColor(ccc3(255-t*100, 255-t*100, 255-t*100));
////																																				   another_img2->setColor(ccc3(255-t*100, 255-t*100, 255-t*100));
////																																				   another_label1->setColor(ccc3(255-t*100, 255-t*100, 255-t*100));
////																																				   another_label2->setColor(ccc3(255-t*100, 255-t*100, 255-t*100));
////																																			   }, [=](float t)
////																																			   {
////																																				   selected_node->setScale(1.5f);
////																																				   another_img1->setColor(ccc3(155, 155, 155));
////																																				   another_img2->setColor(ccc3(155, 155, 155));
////																																				   another_label1->setColor(ccc3(155, 155, 155));
////																																				   another_label2->setColor(ccc3(155, 155, 155));
////																																			   }));
////																						   
////																						   goldbox_menu->setVisible(false);
////																						   
////																						   myDSH->saveAllUserData(nullptr);
////																						   
////																						   CCFadeTo* dimmed_fade = CCFadeTo::create(1.f, 0);
////																						   CCCallFunc* dimmed_call = CCCallFunc::create(this, callfunc_selector(Maingame::clearScenario3));
////																						   CCCallFunc* remove_call = CCCallFunc::create(t_popup, callfunc_selector(CCLayer::removeFromParent));
////																						   CCSequence* t_seq = CCSequence::create(CCDelayTime::create(1.0),dimmed_fade, dimmed_call, remove_call, NULL);
////																						   dimmed_img->runAction(t_seq);
////																					   });
////					  goldbox2_item->setPosition(ccp(0,-30));
////					  goldbox_menu->addChild(goldbox2_item);
////					  
////					  
////					  CCSprite* n_goldbox3 = CCSprite::create("goldbox_off.png");
////					  n_goldbox3->setOpacity(0);
////					  CCSprite* s_goldbox3 = CCSprite::create("goldbox_off.png");
////					  s_goldbox3->setOpacity(0);
////					  
////					  CCMenuItemLambda* goldbox3_item = CCMenuItemSpriteLambda::create(n_goldbox3, s_goldbox3, [=](CCObject* sender)
////																					   {
////																						   AudioEngine::sharedInstance()->playEffect("se_clearreward.mp3", false);
////																						   
////																						   goldbox1->removeFromParent();
////																						   CCSprite* goldbox4 = CCSprite::create("goldbox_on.png");
////																						   goldbox4->setPosition(ccp(-150,-30));
////																						   t_container->addChild(goldbox4, 2);
////																						   goldbox4->runAction(CCFadeTo::create(0.5f, 0));
////																						   
////																						   goldbox2->removeFromParent();
////																						   CCSprite* goldbox5 = CCSprite::create("goldbox_on.png");
////																						   goldbox5->setPosition(ccp(0,-30));
////																						   t_container->addChild(goldbox5, 2);
////																						   goldbox5->runAction(CCFadeTo::create(0.5f, 0));
////																						   
////																						   goldbox3->removeFromParent();
////																						   CCSprite* goldbox6 = CCSprite::create("goldbox_on.png");
////																						   goldbox6->setPosition(ccp(150,-30));
////																						   t_container->addChild(goldbox6, 2);
////																						   goldbox6->runAction(CCFadeTo::create(0.5f, 0));
////																						   
////																						   CCNode* gold_node = CCNode::create();
////																						   KSLabelTTF* item_gold = KSLabelTTF::create("GOLD 100", mySGD->getFont().c_str(), 12);
////																						   item_gold->enableOuterStroke(ccBLACK, 1.f);
////																						   item_gold->setPosition(ccp(0,20));
////																						   gold_node->addChild(item_gold);
////																						   CCSprite* t_gold_img = CCSprite::create("shop_gold3.png");
////																						   t_gold_img->setPosition(ccp(0, -20));
////																						   gold_node->addChild(t_gold_img);
////																						   
////																						   
////																						   CCNode* gold_item_node = CCNode::create();
////																						   KSLabelTTF* item_gold_or_item;
////																						   item_gold_or_item = KSLabelTTF::create("GOLD 200", mySGD->getFont().c_str(), 12);
////																						   item_gold_or_item->enableOuterStroke(ccBLACK, 1.f);
////																						   item_gold_or_item->setPosition(ccp(0,20));
////																						   gold_item_node->addChild(item_gold_or_item);
////																						   CCSprite* t_gold_img2 = CCSprite::create("shop_gold3.png");
////																						   t_gold_img2->setPosition(ccp(0, -20));
////																						   gold_item_node->addChild(t_gold_img2);
////																						   
////																						   
////																						   CCNode* stone_node = CCNode::create();
////																						   KSLabelTTF* item_stone = KSLabelTTF::create("GOLD 300", mySGD->getFont().c_str(), 12);
////																						   item_stone->enableOuterStroke(ccBLACK, 1.f);
////																						   item_stone->setPosition(ccp(0,20));
////																						   stone_node->addChild(item_stone);
////																						   CCSprite* t_stone_img = CCSprite::create("shop_gold3.png");
////																						   t_stone_img->setPosition(ccp(0, -20));
////																						   stone_node->addChild(t_stone_img);
////																						   
////																						   int random_left_right = rand()%2;
////																						   
////																						   CCNode* selected_node;
////																						   KSLabelTTF* another_label1;
////																						   KSLabelTTF* another_label2;
////																						   CCSprite* another_img1;
////																						   CCSprite* another_img2;
////																						   
////																						   if(reward_type == 1)
////																						   {
////																							   mySGD->addChangeGoodsIngameGold(100);
////																							   
////																							   selected_node = gold_node;
////																							   another_label1 = item_gold_or_item;
////																							   another_img1 = t_gold_img2;
////																							   another_label2 = item_stone;
////																							   another_img2 = t_stone_img;
////																							   
////																							   gold_node->setPosition(ccp(150,-30));
////																							   if(random_left_right == 0)
////																							   {
////																								   gold_item_node->setPosition(ccp(-150,-30));
////																								   stone_node->setPosition(ccp(0,-30));
////																							   }
////																							   else
////																							   {
////																								   gold_item_node->setPosition(ccp(0,-30));
////																								   stone_node->setPosition(ccp(-150,-30));
////																							   }
////																						   }
////																						   else if(reward_type == 2)
////																						   {
////																							   mySGD->addChangeGoodsIngameGold(200);
////																							   
////																							   selected_node = gold_item_node;
////																							   another_label1 = item_gold;
////																							   another_img1 = t_gold_img;
////																							   another_label2 = item_stone;
////																							   another_img2 = t_stone_img;
////																							   
////																							   gold_item_node->setPosition(ccp(150,-30));
////																							   if(random_left_right == 0)
////																							   {
////																								   gold_node->setPosition(ccp(-150,-30));
////																								   stone_node->setPosition(ccp(0,-30));
////																							   }
////																							   else
////																							   {
////																								   gold_node->setPosition(ccp(0,-30));
////																								   stone_node->setPosition(ccp(-150,-30));
////																							   }
////																						   }
////																						   else
////																						   {
////																							   mySGD->addChangeGoodsIngameGold(300);
////																							   
////																							   selected_node = stone_node;
////																							   another_label1 = item_gold_or_item;
////																							   another_label2 = item_gold;
////																							   another_img1 = t_gold_img2;
////																							   another_img2 = t_gold_img;
////																							   
////																							   stone_node->setPosition(ccp(150,-30));
////																							   if(random_left_right == 0)
////																							   {
////																								   gold_node->setPosition(ccp(-150,-30));
////																								   gold_item_node->setPosition(ccp(0,-30));
////																							   }
////																							   else
////																							   {
////																								   gold_node->setPosition(ccp(0,-30));
////																								   gold_item_node->setPosition(ccp(-150,-30));
////																							   }
////																						   }
////																						   t_container->addChild(gold_node);
////																						   t_container->addChild(gold_item_node);
////																						   t_container->addChild(stone_node);
////																						   
////																						   t_container->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.7f, [=](float t)
////																																			   {
////																																				   selected_node->setScale(1.f+t*0.5f);
////																																				   another_img1->setColor(ccc3(255-t*100, 255-t*100, 255-t*100));
////																																				   another_img2->setColor(ccc3(255-t*100, 255-t*100, 255-t*100));
////																																				   another_label1->setColor(ccc3(255-t*100, 255-t*100, 255-t*100));
////																																				   another_label2->setColor(ccc3(255-t*100, 255-t*100, 255-t*100));
////																																			   }, [=](float t)
////																																			   {
////																																				   selected_node->setScale(1.5f);
////																																				   another_img1->setColor(ccc3(155, 155, 155));
////																																				   another_img2->setColor(ccc3(155, 155, 155));
////																																				   another_label1->setColor(ccc3(155, 155, 155));
////																																				   another_label2->setColor(ccc3(155, 155, 155));
////																																			   }));
////																						   
////																						   goldbox_menu->setVisible(false);
////																						   
////																						   myDSH->saveAllUserData(nullptr);
////																						   
////																						   CCFadeTo* dimmed_fade = CCFadeTo::create(1.f, 0);
////																						   CCCallFunc* dimmed_call = CCCallFunc::create(this, callfunc_selector(Maingame::clearScenario3));
////																						   CCCallFunc* remove_call = CCCallFunc::create(t_popup, callfunc_selector(CCLayer::removeFromParent));
////																						   CCSequence* t_seq = CCSequence::create(CCDelayTime::create(1.0),dimmed_fade, dimmed_call, remove_call, NULL);
////																						   dimmed_img->runAction(t_seq);
////																					   });
////					  goldbox3_item->setPosition(ccp(150,-30));
////					  goldbox_menu->addChild(goldbox3_item);
////					  
////					  CCSprite* falling_box = CCSprite::create("goldbox_off.png");
////					  falling_box->setPosition(ccp(0,600));
////					  t_container->addChild(falling_box);
////					  
////					  t_container->addChild(KSGradualValue<float>::create(0, 1.f, 1.f, [=](float t)
////																		  {
////																			  float t_t;
////																			  if (t < 1 / 2.75)
////																			  {
////																				  t_t = 7.5625f * t * t;
////																			  } else if (t < 2 / 2.75)
////																			  {
////																				  t -= 1.5f / 2.75f;
////																				  t_t = 7.5625f * t * t + 0.75f;
////																			  } else if(t < 2.5 / 2.75)
////																			  {
////																				  t -= 2.25f / 2.75f;
////																				  t_t = 7.5625f * t * t + 0.9375f;
////																			  }
////																			  else
////																			  {
////																				  t -= 2.625f / 2.75f;
////																				  t_t = 7.5625f * t * t + 0.984375f;
////																			  }
////																			  
////																			  falling_box->setPositionY(600.f - 630.f*t_t);
////																		  }, [=](float t)
////																		  {
////																			  falling_box->setPositionY(-30);
////																			  
////																			  t_container->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
////																																  {
////																																	  title_img->setOpacity(t*255);
////																																	  title_label->setOpacity(t*255);
////																																	  goldbox1->setOpacity(t*255);
////																																	  goldbox2->setOpacity(t*255);
////																																	  goldbox3->setOpacity(t*255);
////																																	  goldbox1->setPositionX(-150.f*t);
////																																	  goldbox3->setPositionX(150.f*t);
////																																  }, [=](float t)
////																																  {
////																																	  falling_box->removeFromParent();
////																																	  title_img->setOpacity(255);
////																																	  title_label->setOpacity(255);
////																																	  goldbox1->setOpacity(255);
////																																	  goldbox2->setOpacity(255);
////																																	  goldbox3->setOpacity(255);
////																																	  goldbox1->setPositionX(-150.f);
////																																	  goldbox3->setPositionX(150.f);
////																																	  
////																																	  goldbox_menu->setVisible(true);
////																																  }));
////																		  }));
//					  t_sm->removeFromParent();
//				  }, CCSizeMake(350,100), ccp(0,-110), 12, true);
}

void Maingame::clearScenario3()
{
//	if(myUI->getPercentage() >= 1.f)
//	{
//		AudioEngine::sharedInstance()->playEffect("ment_rankup.mp3", false, true);
//		
//		CCSprite* spin_light = CCSprite::create("showtime_spin_light.png");
//		spin_light->setOpacity(0);
//		spin_light->setPosition(ccp(240,myDSH->ui_center_y));
//		addChild(spin_light, particleZorder);
//		
//		CCScaleTo* light_scale = CCScaleTo::create(1.2f, 2.f);//2.f/3.f, 2);
//		CCRotateTo* light_rotate = CCRotateTo::create(1.2f, -90);//2.f/3.f, -90);
//		
//		CCFadeTo* light_fade1 = CCFadeTo::create(0.3f, 255);//1.f/6.f, 255);
//		CCFadeTo* light_fade2 = CCFadeTo::create(0.3f, 20);//1.f/6.f, 20);
//		CCFadeTo* light_fade3 = CCFadeTo::create(0.3f, 255);//1.f/6.f, 255);
//		CCFadeTo* light_fade4 = CCFadeTo::create(0.3f, 0);//1.f/6.f, 0);
//		CCCallFunc* light_call = CCCallFunc::create(spin_light, callfunc_selector(CCSprite::removeFromParent));
//		CCSequence* light_seq = CCSequence::create(light_fade1, light_fade2, light_fade3, light_fade4, light_call, NULL);
//		
//		CCSpawn* light_spawn = CCSpawn::create(light_scale, light_rotate, light_seq, NULL);
//		spin_light->runAction(light_spawn); // 1.3
//		
//		
//		CCSprite* bakper = CCSprite::create("bakper.png");
//		bakper->setOpacity(0);
//		bakper->setPosition(ccp(240,myDSH->ui_center_y));
//		addChild(bakper, particleZorder);
//		
//		CCScaleTo* bak_scale1 = CCScaleTo::create(32.4f/60.f, 1.3f);
//		CCScaleTo* bak_scale2 = CCScaleTo::create(18.f/60.f, 0.9f);
//		CCScaleTo* bak_scale3 = CCScaleTo::create(7.2f/60.f,1.2f);
//		CCScaleTo* bak_scale4 = CCScaleTo::create(14.4f/60.f,0.f);
//		CCSequence* bak_seq1 = CCSequence::create(bak_scale1, bak_scale2, bak_scale3, bak_scale4, NULL);
//		
//		CCFadeTo* bak_fade1 = CCFadeTo::create(32.4f/60.f, 255);
//		CCDelayTime* bak_delay = CCDelayTime::create(25.2f/60.f);
//		CCFadeTo* bak_fade2 = CCFadeTo::create(14.4f/60.f, 0);
//		CCCallFunc* bak_call = CCCallFunc::create(bakper, callfunc_selector(CCSprite::removeFromParent));
//		CCSequence* bak_seq2 = CCSequence::create(bak_fade1, bak_delay, bak_fade2, bak_call, NULL);
//		
//		CCSpawn* bak_spawn = CCSpawn::create(bak_seq1, bak_seq2, NULL);
//		bakper->runAction(bak_spawn);
//		
//		addChild(KSTimer::create(1.5f, [=]()
//								 {
//									 closeShutter();
//								 }));
//	}
//	else
//	{
		closeShutter();
//	}
}

void Maingame::failScenario()
{
	if(mySGD->is_endless_mode)
	{
		CCNode* curtain_node = LoadingTipScene::getCurtainTipImage();
		curtain_node->setPosition(ccp(240,myDSH->ui_center_y));
		curtain_node->setScale(myDSH->screen_convert_rate);
		addChild(curtain_node, shutterZorder+5);
		
		CCDelayTime* t_delay = CCDelayTime::create(0.7f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Maingame::closeShutter));
		CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
		curtain_node->runAction(t_seq);
	}
	else
	{
		intro_boss = CumberShowWindow::create(mySD->getSilType(), kCumberShowWindowSceneCode_cardChange);
		intro_boss->setPosition(ccp(240,myDSH->ui_center_y+400));
		intro_boss->setScale(1.8f);
		addChild(intro_boss, searchEyeZorder);
		
		CCDelayTime* t_delay = CCDelayTime::create(1.f);
		CCMoveTo* t_move = CCMoveTo::create(0.7f, ccp(240,myDSH->ui_center_y+10));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Maingame::failScenario2));
		CCSequence* t_seq = CCSequence::create(t_delay, t_move, t_call, NULL);
		intro_boss->runAction(t_seq);
	}
}

void Maingame::failScenario2()
{
	TypingBox* t_tb = TypingBox::create(-500, "boss_mentbox.png", CCRectMake(0, 0, 110, 75), CCRectMake(87, 37, 7, 15), CCRectMake(15, 30, 80, 31), CCSizeMake(180, 40), ccp(240,myDSH->ui_center_y-65), ccp(480.f-56.f,32.5f));
	t_tb->setBoxScale(myDSH->screen_convert_rate);
	addChild(t_tb, 100);
	
	function<void()> end_func1 = [=]()
	{
		t_tb->setVisible(false);
		
		CCNode* curtain_node = LoadingTipScene::getCurtainTipImage();
		curtain_node->setPosition(ccp(240,myDSH->ui_center_y));
		curtain_node->setScale(myDSH->screen_convert_rate);
		addChild(curtain_node, shutterZorder+5);
		
		CCDelayTime* t_delay = CCDelayTime::create(0.7f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Maingame::closeShutter));
		CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
		curtain_node->runAction(t_seq);
		
		t_tb->removeFromParent();
	};
	
	t_tb->startTyping(intro_boss->fail_ment.c_str(), end_func1);
	
//	StoryManager* t_sm = StoryManager::create(-500);
//	addChild(t_sm, 100);
//	
//	t_sm->addMent(true, "", "", intro_boss->fail_ment.c_str(), [=]()
//				  {
//					  CCNode* curtain_node = LoadingTipScene::getCurtainTipImage();
//					  curtain_node->setPosition(ccp(240,myDSH->ui_center_y));
//					  curtain_node->setScale(myDSH->screen_convert_rate);
//					  addChild(curtain_node, shutterZorder+5);
//					  
//					  CCDelayTime* t_delay = CCDelayTime::create(0.7f);
//					  CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Maingame::closeShutter));
//					  CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
//					  curtain_node->runAction(t_seq);
//					  
//					  t_sm->removeFromParent();
//				  }, CCSizeMake(350,100), ccp(0,-110), 12, true);
	
}

void Maingame::closeShutter()
{
	endCloseShutter();
}

void Maingame::endCloseShutter()
{
//	setBackKeyEnabled(false);
	
	if(mySGD->is_endless_mode)
	{
		CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
	}
	else if(mySGD->is_hell_mode)
	{
		if(mySGD->getIsCleared())
		{
			AudioEngine::sharedInstance()->unloadEffectScene("Maingame");
			
			CCTransitionFadeTR* t_trans = CCTransitionFadeTR::create(1.f, ZoomScript::scene());
			CCDirector::sharedDirector()->replaceScene(t_trans);
		}
		else
		{
			AudioEngine::sharedInstance()->unloadEffectScene("Maingame");
			
			CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
			//			mySGD->setNextSceneName("newmainflow");
			//			CCDirector::sharedDirector()->replaceScene(LoadingTipScene::scene());
		}
	}
	else
	{
		if(mySGD->getIsCleared())
		{
			AudioEngine::sharedInstance()->unloadEffectScene("Maingame");
			
			CCTransitionFadeTR* t_trans = CCTransitionFadeTR::create(1.f, ZoomScript::scene());
			CCDirector::sharedDirector()->replaceScene(t_trans);
		}
		else
		{
			AudioEngine::sharedInstance()->unloadEffectScene("Maingame");
			
			myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_fail);
			CCDirector::sharedDirector()->replaceScene(PuzzleScene::scene());
			//			mySGD->setNextSceneName("newmainflow");
			//			CCDirector::sharedDirector()->replaceScene(LoadingTipScene::scene());
		}
	}
}

void Maingame::startMoveToBossPosition()
{
	CCNode* boss_node = myGD->getMainCumberCCNodeVector()[0];
	
	startMoveToCCPoint(myGD->getMainCumberPoint(boss_node).convertToCCP());
}

void Maingame::startMoveToCCPoint( CCPoint t_point )
{
	move_to_boss_position_frame = 0;
	CCPoint after_position = getObjectToGameNodePosition(t_point);
	CCPoint sub_position = ccpSub(after_position, game_node->getPosition());
	CCLOG("boss : %.2f\t recent : %.2f", after_position.y, game_node->getPositionY());
	move_to_boss_position_ds = ccpMult(sub_position, 1.f/30.f);
	schedule(schedule_selector(Maingame::moveToBossPosition));
}

void Maingame::moveToBossPosition()
{
	move_to_boss_position_frame++;

	CCPoint after_position = ccpAdd(game_node->getPosition(), move_to_boss_position_ds);
	game_node->setPosition(after_position);
	myGD->communication("VS_setMoveGamePosition", getGameNodeToObjectPosition(after_position));

	if(move_to_boss_position_frame >= 30)
	{
		unschedule(schedule_selector(Maingame::moveToBossPosition));
	}
}

CCPoint Maingame::getObjectToGameNodePosition( CCPoint t_p )
{
	CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float y_value = -t_p.y*myGD->game_scale+480.f*frame_size.height/frame_size.width/2.f;
	
	if(!myDSH->getBoolForKey(kDSH_Key_isAlwaysCenterCharacter))
	{
		if(y_value > 80)																		y_value = 80;
		else if(y_value < -430*myGD->game_scale+480*frame_size.height/frame_size.width - 60)			y_value = -430*myGD->game_scale+480*frame_size.height/frame_size.width - 60;
	}
	
	float x_value = -t_p.x*myGD->game_scale+480.f/2.f;
	if(!myDSH->getBoolForKey(kDSH_Key_isAlwaysCenterCharacter))
	{
		if(x_value > myGD->boarder_value+80)														x_value = myGD->boarder_value+80;
		else if(x_value < -320*myGD->game_scale-myGD->boarder_value+480.f-80)						x_value = -320*myGD->game_scale-myGD->boarder_value+480.f-80;
	}

	CCPoint after_position;

//	if(myGD->gamescreen_type == kGT_full)					after_position = ccp(myGD->boarder_value,y_value);
//	else if(myGD->gamescreen_type == kGT_leftUI)			after_position = ccp(50+myGD->boarder_value,y_value);
//	else if(myGD->gamescreen_type == kGT_rightUI)			after_position = ccp(myGD->boarder_value,y_value);
	
	after_position = ccp(x_value, y_value);

	return after_position;
}

CCPoint Maingame::getObjectToGameNodePositionCoin( CCPoint t_p )
{
	float scale_value = NSDS_GD(mySD->getSilType(), kSDS_SI_scale_d);
	if(scale_value < 0.1f)
		scale_value = 1.f;
	float x_value = t_p.x/320.f*(720.f*scale_value-myGD->boarder_value*2.f);
	float y_value = t_p.y/320.f*(720.f*scale_value-myGD->boarder_value*2.f);

	x_value = x_value+game_node->getPositionX();
	y_value = y_value+game_node->getPositionY();

	return ccp(x_value, y_value);
}

CCPoint Maingame::getGameNodeToObjectPosition( CCPoint t_p )
{
	CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float y_value = -(t_p.y - 480.f*frame_size.height/frame_size.width/2.f)/myGD->game_scale;
	float x_value = -(t_p.x - 480.f/2.f)/myGD->game_scale;
	
//	if(y_value > 60)																		y_value = 60;
//	else if(y_value < -490*myGD->game_scale+480*frame_size.height/frame_size.width)			y_value = -490*myGD->game_scale+480*frame_size.height/frame_size.width;
//	if(x_value > myGD->boarder_value)														x_value = myGD->boarder_value;
//	else if(x_value < -320*myGD->game_scale-myGD->boarder_value+480.f)						x_value = -320*myGD->game_scale-myGD->boarder_value+480.f;
	
	CCPoint after_position;
	after_position = ccp(x_value,y_value);

	return after_position;
}

void Maingame::moveGamePosition( CCPoint t_p )
{
	//		if(!myGD->is_setted_jack)// || myGD->game_step == kGS_unlimited)
	line_particle->setPosition(t_p);
	
	game_node->setPosition(getObjectToGameNodePosition(t_p));
	if(thumb_texture)
	{
		CCPoint screen_position = ccp(fabsf(game_node->getPositionX()-myGD->boarder_value-480.f/2.f), fabsf(game_node->getPositionY()-myGD->boarder_value-myDSH->ui_top/2.f));
		screen_position = ccpMult(screen_position, thumb_texture->getScale()/game_node->getScale());
		screen_position = ccpAdd(screen_position, thumb_texture->getPosition());
		screen_position = ccpSub(screen_position, ccp(320*thumb_texture->getScale()/2.f, 430*thumb_texture->getScale()/2.f));
		screen_node->setPosition(screen_position);
	}
	
	if(character_thumb)
		character_thumb->setPosition(ccpAdd(thumb_base_position, ccpMult(myJack->getPosition(), thumb_texture->getScale())));
}

CCPoint Maingame::getGameNodePosition()
{
	return game_node->getPosition();
}

void Maingame::goldGettingEffect( CCPoint t_p, int t_i )
{
	GetGold* t_gg = GetGold::create(t_p, t_i);
	game_node->addChild(t_gg, goldZorder);
	t_gg->startMyAction();
}

void Maingame::percentageGettingEffect( float t_f, bool t_b, CCPoint t_p )
{
	GetPercentage* t_gp = GetPercentage::create(t_f, t_b);
	
	CCPoint add_point;
	if(t_p.x < 30.f)
		add_point = ccp(30.f, 0.f);
	else if(t_p.x > 290.f)
		add_point = ccp(-30.f, 0.f);
	else
	{
		if(t_p.y > 400.f)
			add_point = ccp(0, -20.f);
		else
			add_point = ccp(0, 20.f);
	}
	
	t_gp->setPosition(ccpAdd(t_p, add_point));
	game_node->addChild(t_gp, goldZorder);
}

void Maingame::takeSpeedUpEffect( int t_step )
{
	CCPoint jack_position = myGD->getJackPoint().convertToCCP();
	
	if(recent_take_speed_up_value < t_step)
	{
		if(save_take_speed_up_effect)
			save_take_speed_up_effect->removeFromParent();
		
		recent_take_speed_up_value = t_step;
		CCPoint add_point;
		if(jack_position.x < 30.f)
			add_point = ccp(30.f, 20.f);
		else if(jack_position.x > 290.f)
			add_point = ccp(-30.f, 20.f);
		else
		{
			if(jack_position.y > 400.f)
				add_point = ccp(30.f, -20.f);
			else
				add_point = ccp(30.f, 20.f);
		}
		
		TakeSpeedUp* t_tsu = TakeSpeedUp::create(t_step, bind(&Maingame::endTakeSpeedUpEffect, this));
		t_tsu->setScale(1.f/myGD->game_scale);
		t_tsu->setPosition(ccpAdd(t_tsu->getPosition(), ccpAdd(jack_position, add_point)));
		game_node->addChild(t_tsu, goldZorder);
		
		save_take_speed_up_effect = t_tsu;
	}
}

void Maingame::endTakeSpeedUpEffect()
{
	recent_take_speed_up_value = -1;
	save_take_speed_up_effect = NULL;
}

void Maingame::showMissMissile( CCPoint t_position )
{
	t_position.x += rand()%21 - 10;
	t_position.y += rand()%21 - 10;

	KSLabelTTF* miss_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_miss), mySGD->getFont().c_str(), 30);
	miss_label->enableOuterStroke(ccc3(60, 20, 0), 2.5f, 255, true);
	miss_label->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
	KSLabelTTF* shadow = CommonAnimation::applyBigShadow(miss_label, miss_label->getFontSize());
//	CCSprite* miss_label = CCSprite::create("missile_miss.png");
	miss_label->setScale(1.f/myGD->game_scale);
	miss_label->setPosition(t_position);
	game_node->addChild(miss_label, goldZorder);

	miss_label->addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
													   {
														   miss_label->setOpacity(255-255*t);
														   shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
													   }, [=](float t)
													   {
														   miss_label->removeFromParent();
													   }));
}

void Maingame::showDamageMissile( CCPoint t_position, int t_damage )
{
	CCNode* container = CCNode::create();
	container->setScale(1.f/myGD->game_scale);
	container->setPosition(t_position);
	game_node->addChild(container, goldZorder);
	
	CountingBMLabel* damage_label = CountingBMLabel::create("1", "missile_damage_label.fnt", 0.3f, "%d");
	container->addChild(damage_label, goldZorder);
	
	damage_label->setString(CCString::createWithFormat("%d", t_damage)->getCString());
	
	CCDelayTime* t_delay = CCDelayTime::create(0.5f);
	CCFadeTo* t_fade = CCFadeTo::create(0.5f, 0);
	CCCallFunc* t_call = CCCallFunc::create(container, callfunc_selector(CCNode::removeFromParent));
	CCSequence* t_seq = CCSequence::create(t_delay, t_fade, t_call, NULL);
	damage_label->runAction(t_seq);
	
//	MissileDamageLabel* damage_label = MissileDamageLabel::create(t_damage);
//	damage_label->setScale(1.f/1.5f);
//	damage_label->setPosition(t_position);
//	game_node->addChild(damage_label, goldZorder);
//
//	damage_label->startMyAction();
}

void Maingame::showComboImage(CCPoint t_position, int t_combo_value)
{
	return;
	if(combo_value_img)
		combo_value_img->getParent()->removeFromParent();
	
	CCNode* container = CCNode::create();
	container->setScale(1.f/myGD->game_scale);
	container->setPosition(ccpAdd(t_position, ccp(0,20)));
	game_node->addChild(container, goldZorder);
	
	combo_value_img = KSLabelTTF::create(ccsf("%s%d", myLoc->getLocalForKey(LK::kMyLocalKey_combo), t_combo_value), mySGD->getFont().c_str(), 30);
	combo_value_img->enableOuterStroke(ccc3(0, 45, 10), 2.5f, 255, true);
	combo_value_img->setGradientColor(ccc4(240, 255, 10, 255), ccc4(110, 190, 5, 255), ccp(0,-1));
	CommonAnimation::applyBigShadow(combo_value_img, combo_value_img->getFontSize());
	combo_value_img->setPosition(ccp(0,0));
	container->addChild(combo_value_img);
	
	CCDelayTime* t_delay1 = CCDelayTime::create(0.5f);
	CCFadeTo* t_fade1 = CCFadeTo::create(0.5f, 0);
	CCCallFuncO* t_call1 = CCCallFuncO::create(this, callfuncO_selector(Maingame::removeComboImage), container);
	CCSequence* t_seq1 = CCSequence::create(t_delay1, t_fade1, t_call1, NULL);
	
	combo_value_img->runAction(t_seq1);
}

void Maingame::removeComboImage(CCObject* t_node)
{
	combo_value_img = NULL;
	((CCNode*)t_node)->removeFromParent();
}

void Maingame::showLineDiePosition( IntPoint t_p )
{
	if(!is_line_die)
	{
		is_line_die = true;
		CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("line_die.png");
		CCSprite* line_die = CCSprite::createWithTexture(t_texture, CCRectMake(0, 0, 100, 100));
		line_die->setPosition(ccp((t_p.x-1.f)*pixelSize+1.f, (t_p.y-1.f)*pixelSize+1.f));
		line_die->setScale(1.f/myGD->game_scale);
		game_node->addChild(line_die, goldZorder);

		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=0;i<4;i++)
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*100, 0, 100, 100));
		CCAnimate* t_animate = CCAnimate::create(t_animation);

		CCFadeTo* t_fade = CCFadeTo::create(0.5f, 0);
		CCCallFuncO* t_call = CCCallFuncO::create(this, callfuncO_selector(Maingame::removeLineDieEffect), line_die);

		CCAction* t_action = CCSequence::create(t_animate, t_fade, t_call, NULL);

		line_die->runAction(t_action);
	}
}

void Maingame::removeLineDieEffect( CCObject* t_obj )
{
	game_node->removeChild((CCNode*)t_obj, true);
}

void Maingame::resetIsLineDie()
{
	is_line_die = false;
}

void Maingame::showWarning( int t1 )
{
	Warning* t_w = Warning::create(t1);
	addChild(t_w, goldZorder);
	t_w->startAction();
}

void Maingame::showDetailMessage(const std::string& fileName, const std::string& type)
{
	//DetailWarning* w = DetailWarning::create(fileName);
	//addChild(w, goldZorder);
	
	// 이미 표시되고 있는게 있다면 큐에서 비워버리고 이동시켜버림.
	for(auto it : detailWarningQueue)
	{
		auto iter = std::find(detailWarningQueue.begin(), detailWarningQueue.end(), it);
		if(iter != detailWarningQueue.end()){
			detailWarningQueue.erase(iter);
		}
		it->getChildByTag(0x3030)->removeAllChildren(); // 붙은 스케쥴러 다 뗌.
		it->addChild(KSGradualValue<CCPoint>::create(it->getPosition(),
																													ccp(0 - it->getContentSize().width, it->getPosition().y),
																													10.f / 30.f,
																													[=](CCPoint t){
																														it->setPosition(t);
																													}, [=](CCPoint t){
																														it->setPosition(t);
																														it->removeFromParent();
																														
																													}));
	}
	
	// 바뀜 테스트.
	// 또 바뀌나??
	std::map<std::string, LK> fileName2Language;
	fileName2Language["warning_9.ccbi"] = LK::kMyLocalKey_warning9;
	fileName2Language["warning_105.ccbi"] = LK::kMyLocalKey_warning105;
	fileName2Language["warning_106.ccbi"] = LK::kMyLocalKey_warning106;
	fileName2Language["warning_107.ccbi"] = LK::kMyLocalKey_warning107;
	fileName2Language["warning_108.ccbi"] = LK::kMyLocalKey_warning108;
	fileName2Language["warning_109.ccbi"] = LK::kMyLocalKey_warning109;
	fileName2Language["warning_110.ccbi"] = LK::kMyLocalKey_warning110;
	fileName2Language["warning_111.ccbi"] = LK::kMyLocalKey_warning111;
	fileName2Language["warning_112.ccbi"] = LK::kMyLocalKey_warning112;
	fileName2Language["warning_113.ccbi"] = LK::kMyLocalKey_warning113;
	fileName2Language["warning_1001.ccbi"] = LK::kMyLocalKey_warning1001;
	fileName2Language["warning_1002.ccbi"] = LK::kMyLocalKey_warning1002;
	fileName2Language["warning_1003.ccbi"] = LK::kMyLocalKey_warning1003;
	fileName2Language["warning_1004.ccbi"] = LK::kMyLocalKey_warning1004;
	fileName2Language["warning_1005.ccbi"] = LK::kMyLocalKey_warning1005;
	fileName2Language["warning_1006.ccbi"] = LK::kMyLocalKey_warning1006;
	fileName2Language["warning_1007.ccbi"] = LK::kMyLocalKey_warning1007;
	fileName2Language["warning_1008.ccbi"] = LK::kMyLocalKey_warning1008;
	fileName2Language["warning_1009.ccbi"] = LK::kMyLocalKey_warning1009;
	fileName2Language["warning_1010.ccbi"] = LK::kMyLocalKey_warning1010;
	fileName2Language["warning_1011.ccbi"] = LK::kMyLocalKey_warning1011;
	fileName2Language["warning_1012.ccbi"] = LK::kMyLocalKey_warning1012;
	fileName2Language["warning_1013.ccbi"] = LK::kMyLocalKey_warning1013;
	fileName2Language["warning_1014.ccbi"] = LK::kMyLocalKey_warning1014;
	fileName2Language["warning_1015.ccbi"] = LK::kMyLocalKey_warning1015;
	fileName2Language["warning_1016.ccbi"] = LK::kMyLocalKey_warning1016;
	fileName2Language["warning_1017.ccbi"] = LK::kMyLocalKey_warning1017;
	fileName2Language["warning_1018.ccbi"] = LK::kMyLocalKey_warning1018;
	fileName2Language["warning_1019.ccbi"] = LK::kMyLocalKey_warning1019;
	fileName2Language["warning_1020.ccbi"] = LK::kMyLocalKey_warning1020;
	fileName2Language["warning_boss_success.ccbi"] = LK::kMyLocalKey_warningBossSuccess;
	fileName2Language["warning_over_02.ccbi"] = LK::kMyLocalKey_warningLastLife;
	
	KSLabelTTF* textMessage = KSLabelTTF::create(myLoc->getLocalForKey(fileName2Language[fileName]), mySGD->getFont().c_str(), 30.f);
	detailWarningQueue.push_back(textMessage);
	textMessage->setColor(ccc3(255, 0, 0));
	textMessage->enableOuterStroke(ccc3(0, 0, 0), 3.f, true);
	textMessage->setEnableItalic();
	textMessage->setAnchorPoint(ccp(0.f, 0.5f));
	
	float width = textMessage->getContentSize().width;
	float centerPosition = 240 - width / 2.f;
	addChild(textMessage, goldZorder);

	textMessage->setPosition(ccp(520, 160));
	CCNode* tempScheduler = CCNode::create();
	textMessage->addChild(tempScheduler, 0, 0x3030);
	tempScheduler->addChild(KSGradualValue<CCPoint>::create(textMessage->getPosition(),
																					 ccp(centerPosition, textMessage->getPosition().y),
																					 7.f / 30.f,
																					 [=](CCPoint t){
																						 textMessage->setPosition(t);
																					 }, [=](CCPoint t){
																						 textMessage->setPosition(t);
																						 // 가운데 까지 왔다.
																						 //
																						 ccColor3B colors[2];
																						 if(type == "w")
																						 {
																							 colors[0] = ccc3(255, 255, 0);
																							 colors[1] = ccc3(255, 0, 0);
																						 }
																						 else if(type == "i")
																						 {
																							 colors[0] = ccc3(255, 210, 255);
																							 colors[1] = ccc3(191, 241, 255);
																						 }
																						 else if(type == "h")
																						 {
																							 colors[0] = ccc3(30, 200, 255);
																							 colors[1] = ccc3(255, 150, 100);
																						 }
																						 for(int i=0; i<11; i++) {
																							 tempScheduler->addChild(KSTimer::create(3/30.f * (i + 1), [=](){
																								 textMessage->setColor(colors[i % 2]);
																								 // 마지막 조건
																								 if(i == 10) {
																									 // 마지막 조건일 때 큐에서 이미 지워버림. 어차피 지우고 있는 중이라 ㄱㅊ.
																									 auto iter = std::find(detailWarningQueue.begin(), detailWarningQueue.end(), textMessage);
																									 if(iter != detailWarningQueue.end()){
																										 detailWarningQueue.erase(iter);
																									 }
																									 tempScheduler->addChild(KSGradualValue<CCPoint>::create(textMessage->getPosition(),
																																																				 ccp(0 - width, textMessage->getPosition().y),
																																																				 10.f / 30.f,
																																																				 [=](CCPoint t){
																																																					 textMessage->setPosition(t);
																																																				 }, [=](CCPoint t){
																																																					 textMessage->setPosition(t);
																																																					 textMessage->removeFromParent();

																																																				 }));
																										
																								 }
																							 }));
																						 }

																							
																					 }));
}
void Maingame::showTextMessage(const std::string& text)
{
	CCLabelTTF* msg = CCLabelTTF::create(text.c_str(), "", 28.f);
	addChild(msg, goldZorder);
	addChild(KSGradualValue<float>::create(600, 240, 0.7f, [=](float t)
				{
					msg->setPosition(ccp(t, myDSH->ui_center_y));
				},
				[=](float t)
				{
					addChild(KSGradualValue<float>::create(255, 0, 2.f, [=](float t)
							{
								msg->setOpacity(t);
							},
							[=](float t)
							{
								msg->removeFromParent();
							}));
				}));
}
void Maingame::showTakeCoin()
{
	if(myGD->getIsGameover())
		return;
	
	AudioEngine::sharedInstance()->playEffect("ment_spread_change.mp3", false, true);
	
	TakeCoin* t_w = TakeCoin::create();
	addChild(t_w, goldZorder);
}

CCNode* Maingame::gameNodePointer()
{
	return game_node;
}

void Maingame::showChangeCard()
{
	if(myGD->getIsGameover())
		return;
	
	ChangeCard* t_w = ChangeCard::create();
	addChild(t_w, goldZorder);
}

void Maingame::showCoin()
{
	myGIM->showCoin(this, callfuncCCpI_selector(Maingame::takeExchangeCoin));
}

void Maingame::takeExchangeCoin( CCPoint t_start_position, int t_coin_number )
{
	myUI->takeExchangeCoin(getObjectToGameNodePositionCoin(t_start_position), t_coin_number);
}

void Maingame::startExchange()
{
	AudioEngine::sharedInstance()->playSound("sound_back_maingame2.mp3", true);
	AudioEngine::sharedInstance()->playEffect("ment_change_success.mp3", false, true);
	
	mySGD->is_exchanged = true;
	mySD->exchangeSilhouette();
	myMS->exchangeMS();
	
	
	// ######################## hs code bbu woo~ ##############################
	std::vector<KSCumberBase*> maincumbers = myCP->getMainCumbers();
	for(int i=0;i<maincumbers.size();i++){
		((KSCumberBase*)maincumbers[i])->applyAutoBalance(true);
	}

	std::vector<KSCumberBase*> subcumbers = myCP->getSubCumberArrayPointer();
	for(int i=0;i<subcumbers.size();i++){
		((KSCumberBase*)subcumbers[i])->applyAutoBalance(true);
	}
	// ######################## hs code bbu woo~ ##############################
	
	bool is_visible = sil_thumb->isVisible();
	float t_scale = sil_thumb->getScale();
	CCPoint t_position = sil_thumb->getPosition();
	
	sil_thumb->removeFromParent();
	
	sil_thumb = EffectSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 3))->getCString()));
	int t_puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	sil_thumb->setColorSilhouette(NSDS_GI(t_puzzle_number, kSDS_PZ_color_r_d), NSDS_GI(t_puzzle_number, kSDS_PZ_color_g_d), NSDS_GI(t_puzzle_number, kSDS_PZ_color_b_d));
	sil_thumb->setScale(t_scale);
	sil_thumb->setPosition(t_position);
	sil_thumb->setVisible(is_visible);
	
	
	
	addChild(sil_thumb, searchEyeZorder);
	
	
	
	if(mySGD->is_play_replay && mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapTime)].size() > 0)
	{
		CCPoint t_position = replay_sil_thumb->getPosition();
		replay_sil_thumb->removeFromParent();
		
		replay_sil_thumb = EffectSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 3))->getCString()));
		int t_puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
		replay_sil_thumb->setColorSilhouette(NSDS_GI(t_puzzle_number, kSDS_PZ_color_r_d), NSDS_GI(t_puzzle_number, kSDS_PZ_color_g_d), NSDS_GI(t_puzzle_number, kSDS_PZ_color_b_d));
		replay_sil_thumb->setScale(t_scale);
		replay_sil_thumb->setPosition(t_position);
		replay_sil_thumb->setVisible(is_visible);
		replay_all_node->addChild(replay_sil_thumb, -1);
	}
	
	myGD->communication("UI_writeImageChange");
}

void Maingame::setLimitMap()
{
	myGD->game_step = kGS_limited;
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();

	float screen_to_map_center_y = (-game_node->getPositionY() + 240.f*screen_size.height/screen_size.width)/game_node->getScale();

	int screen_to_map_top = floorf((screen_to_map_center_y + 240.f*screen_size.height/screen_size.width/game_node->getScale())/2.f);
	int screen_to_map_bottom = ceilf((screen_to_map_center_y - 240.f*screen_size.height/screen_size.width/game_node->getScale())/2.f);

	myGD->limited_step_top = screen_to_map_top;
	myGD->limited_step_bottom = screen_to_map_bottom;

	for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
	{
		if(j < screen_to_map_bottom || j > screen_to_map_top)
		{
			for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
			{
				myGD->mapState[i][j] = mapOutline;
			}
		}
	}

	myGD->communication("MS_setTopBottomBlock");
}

void Maingame::setUnlimitMap()
{
	myGD->game_step = kGS_changing;

	int total_cell = 0;
	int take_cell = 0;
	
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(mySD->silData[i][j])
			{
				total_cell++;
				if(myGD->mapState[i][j] == mapOldget || myGD->mapState[i][j] == mapOldline)
					take_cell++;
			}
		}
	}
	
	float recent_percent = 1.f*take_cell/total_cell;
	
	if(recent_percent >= 0.85f || (mySGD->isTimeEvent(kTimeEventType_clear) && recent_percent >= mySGD->getTimeEventFloatValue(kTimeEventType_clear)/100.f))
	{
		
	}
	else
	{
		showAreaScroll();
	}

	limitted_map_position = game_node->getPosition();
	myGD->communication("VS_setLimittedMapPosition");
	myGD->communication("MS_startRemoveBlock");

	for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
	{
		if(j < myGD->limited_step_bottom || j > myGD->limited_step_top)
		{
			for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
			{
				if(myGD->mapState[i][j] == mapOutline)
					myGD->mapState[i][j] = mapEmpty;
			}
		}
	}
	
	if(myDSH->getBoolForKey(kDSH_Key_isAlwaysCenterCharacter))
	{
		changing_game_step_frame = 0;
		schedule(schedule_selector(Maingame::changingGameStep));
	}
	else
	{
		myGD->game_step = kGS_unlimited;
	}
}

void Maingame::changingGameStep()
{
	changing_game_step_frame++;
	gameNodeChangingGameStep(limitted_map_position, changing_game_step_frame);
	myGD->communication("VS_changingGameStep", changing_game_step_frame);

	if(changing_game_step_frame >= 15)
	{
		unschedule(schedule_selector(Maingame::changingGameStep));
		myGD->game_step = kGS_unlimited;
	}
}

void Maingame::gameNodeChangingGameStep( CCPoint t_p, int t_step )
{
	IntPoint jack_point = myGD->getJackPoint();
	CCPoint jack_position = jack_point.convertToCCP();
	jack_position = getObjectToGameNodePosition(jack_position);

	CCPoint after_position = ccpAdd(limitted_map_position, ccpMult(ccpSub(jack_position, t_p), t_step/15.f));
	game_node->setPosition(after_position);
}

void Maingame::showAreaScroll()
{
	if(myGD->getIsGameover())
		return;
	
	AreaScroll* t_w = AreaScroll::create();
	addChild(t_w, goldZorder);
	t_w->startAction();
}

void Maingame::startShake( float t_direction_angle )
{
	stopAllActions();

	CCPoint change_position = ccp(cosf(t_direction_angle/180.f*M_PI), sinf(t_direction_angle/180.f*M_PI));
	change_position = ccpMult(change_position, 3.f/sqrtf(powf(change_position.x, 2.f) + powf(change_position.y, 2.f)));

	setPosition(ccpAdd(getPosition(), change_position));
	CCDelayTime* t_delay = CCDelayTime::create(0.1f);
	CCMoveTo* t_move = CCMoveTo::create(0.2f, CCPointZero);
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_move);
	runAction(t_seq);

//	shake_frame = 0;
//	schedule(schedule_selector(Maingame::shaking), 1.f/20.f);
}

void Maingame::shaking()
{
	shake_frame++;

	if(shake_frame == 1)
	{
		setRotation(-1.5f);
		setPosition(ccp(-1,-2));
	}
	else if(shake_frame == 2)
	{
		setRotation(1.f);
		setPosition(ccp(1,0));
	}
	else if(shake_frame == 3)
	{
		setRotation(-1.2f);
		setPosition(ccp(-2,1));
	}
	else if(shake_frame == 4)
	{
		setRotation(0.7f);
		setPosition(ccp(1,-1));
	}
	else if(shake_frame == 5)
	{
		setRotation(0);
		setPosition(CCPointZero);

		stopShake();
	}
}

void Maingame::stopShake()
{
	unschedule(schedule_selector(Maingame::shaking));
	setRotation(0);
	setPosition(CCPointZero);
	shake_frame = 0;
}

void Maingame::showScreenSideWarning()
{
	if(warning_count == 0)
	{
		CCSprite* left_top = CCSprite::create("warning_edge.png");
		left_top->setAnchorPoint(ccp(0,1));
		left_top->setPosition(ccp(0,myDSH->ui_top));
		screen_side_warning_node->addChild(left_top);
		left_top->setOpacity(0);
		
		CCSprite* right_top = CCSprite::create("warning_edge.png");
		right_top->setAnchorPoint(ccp(0,1));
		right_top->setRotation(90);
		right_top->setPosition(ccp(480,myDSH->ui_top));
		screen_side_warning_node->addChild(right_top);
		right_top->setOpacity(0);
		
		CCSprite* top_bar = CCSprite::create("warning_border.png");
		top_bar->setAnchorPoint(ccp(0.5,1));
		top_bar->setScaleX((480-left_top->getContentSize().width*2.f)/top_bar->getContentSize().width);
		top_bar->setPosition(ccp(240,myDSH->ui_top));
		screen_side_warning_node->addChild(top_bar);
		top_bar->setOpacity(0);
		
		CCSprite* left_bottom = CCSprite::create("warning_edge.png");
		left_bottom->setAnchorPoint(ccp(0,1));
		left_bottom->setRotation(-90);
		left_bottom->setPosition(ccp(0,0));
		screen_side_warning_node->addChild(left_bottom);
		left_bottom->setOpacity(0);
		
		CCSprite* left_bar = CCSprite::create("warning_border.png");
		left_bar->setAnchorPoint(ccp(0.5,1));
		left_bar->setRotation(-90);
		left_bar->setScaleX((myDSH->ui_top-left_top->getContentSize().width*2.f)/left_bar->getContentSize().width);
		left_bar->setPosition(ccp(0,myDSH->ui_center_y));
		screen_side_warning_node->addChild(left_bar);
		left_bar->setOpacity(0);
		
		CCSprite* right_bottom = CCSprite::create("warning_edge.png");
		right_bottom->setAnchorPoint(ccp(0,1));
		right_bottom->setRotation(180);
		right_bottom->setPosition(ccp(480,0));
		screen_side_warning_node->addChild(right_bottom);
		right_bottom->setOpacity(0);
		
		CCSprite* right_bar = CCSprite::create("warning_border.png");
		right_bar->setAnchorPoint(ccp(0.5,1));
		right_bar->setRotation(90);
		right_bar->setScaleX((myDSH->ui_top-left_top->getContentSize().width*2.f)/right_bar->getContentSize().width);
		right_bar->setPosition(ccp(480,myDSH->ui_center_y));
		screen_side_warning_node->addChild(right_bar);
		right_bar->setOpacity(0);
		
		CCSprite* bottom_bar = CCSprite::create("warning_border.png");
		bottom_bar->setAnchorPoint(ccp(0.5,1));
		bottom_bar->setRotation(180);
		bottom_bar->setScaleX((480-left_top->getContentSize().width*2.f)/bottom_bar->getContentSize().width);
		bottom_bar->setPosition(ccp(240,0));
		screen_side_warning_node->addChild(bottom_bar);
		bottom_bar->setOpacity(0);
		
		left_top->runAction(CCFadeTo::create(0.5f, 255));
		right_top->runAction(CCFadeTo::create(0.5f, 255));
		top_bar->runAction(CCFadeTo::create(0.5f, 255));
		left_bottom->runAction(CCFadeTo::create(0.5f, 255));
		left_bar->runAction(CCFadeTo::create(0.5f, 255));
		right_bottom->runAction(CCFadeTo::create(0.5f, 255));
		right_bar->runAction(CCFadeTo::create(0.5f, 255));
		bottom_bar->runAction(CCFadeTo::create(0.5f, 255));
	}
	
	warning_count++;
}
void Maingame::hideScreenSideWarning()
{
	if(warning_count > 0)
		warning_count--;
	
	if(warning_count == 0)
	{
		screen_side_warning_node->removeAllChildren();
	}
}

void Maingame::showThumbWarning(CCPoint t_point)
{
	CCNode* t_node = CCNode::create();
	CCSprite* t_warning = KS::loadCCBI<CCSprite*>(this, "minimap_warning.ccbi").first;
	t_warning->setPosition(ccpAdd(thumb_base_position, ccpMult(t_point, thumb_texture->getScale())));
	t_node->addChild(t_warning);
	addChild(t_node, myUIZorder);
	
	CCDelayTime* t_delay = CCDelayTime::create(0.5f);
	CCCallFunc* t_call = CCCallFunc::create(t_node, callfunc_selector(CCNode::removeFromParent));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	t_node->runAction(t_seq);
}

void Maingame::showScoreMissileEffect(CCPoint t_position)
{
	CCSprite* take_effect = KS::loadCCBI<CCSprite*>(this, "fx_item2.ccbi").first;
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	CCSprite* take_effect = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("fx_item2.ccbi",this));
	take_effect->setPosition(t_position);
	addChild(take_effect, myUIZorder);
//	reader->autorelease();
	
	addChild(KSTimer::create(22.f/30.f, [=](){take_effect->removeFromParent();}));
}

void Maingame::scoreAttackMissile(int t_damage)
{
	CCPoint origin_position = ccp(40, myDSH->ui_center_y+60);
	CCPoint final_position = ccp(440, myDSH->ui_center_y-10);
	
	CCSprite* t_missile = KS::loadCCBI<CCSprite*>(this, "endless_missile_me.ccbi").first;
	t_missile->setPosition(origin_position);
	addChild(t_missile, myUIZorder);
	
	float alpha_value = 30.f/180.f*M_PI;
	float v_zero = (final_position.x-origin_position.x)/60.f/cosf(alpha_value);
	float v_x_zero = v_zero*cosf(alpha_value);
	float v_y_zero = v_zero*sinf(alpha_value);
	float gravity = (-final_position.y + origin_position.y + v_zero*sinf(alpha_value)*60.f)/3600.f*2.f;
	
	addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
										   {
											   CCPoint before_position = t_missile->getPosition();
											   t_missile->setPosition(ccp(origin_position.x + v_x_zero*t*60, origin_position.y + v_y_zero*t*60 - gravity*t*t*60*60/2));
											   
											   t_missile->setRotation(-(t_missile->getPosition() - before_position).getAngle()/M_PI*180.f);
										   }, [=](float t)
										   {
											   CCPoint before_position = t_missile->getPosition();
											   t_missile->setPosition(ccp(origin_position.x + v_x_zero*1.f*60, origin_position.y + v_y_zero*1.f*60 - gravity*1.f*1.f*60*60/2));
											   
											   t_missile->setRotation(-(t_missile->getPosition() - before_position).getAngle()/M_PI*180.f);
											   
											   t_missile->removeFromParent();
											   
											   CCSprite* bomb_img = KS::loadCCBI<CCSprite*>(this, "bossbomb2.ccbi").first;
											   bomb_img->setPosition(ccp(440, myDSH->ui_center_y-10));
											   addChild(bomb_img, myUIZorder);
											   
											   bomb_img->addChild(KSTimer::create(24.f/30.f, [=](){bomb_img->removeFromParent();}));
											   
											   KSLabelTTF* t_label = KSLabelTTF::create(CCString::createWithFormat("%d", -t_damage)->getCString(), mySGD->getFont().c_str(), 12);
											   t_label->setColor(ccRED);
											   t_label->enableOuterStroke(ccBLACK, 1.f);
											   t_label->setPosition(ccp(440, myDSH->ui_center_y-10));
											   t_label->setScale(0.7f);
											   addChild(t_label, myUIZorder);
											   
											   addChild(KSGradualValue<float>::create(0.f, 1.f, 5.f/30.f, [=](float t)
																					  {
																						  t_label->setOpacity(t*255);
																						  t_label->setScale(0.7f+t);
																					  }, [=](float t)
																					  {
																						  t_label->setOpacity(255);
																						  t_label->setScale(1.7f);
																						  addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
																																 {
																																	 t_label->setScale(1.7f-t*0.5f);
																																 }, [=](float t)
																																 {
																																	 t_label->setScale(1.2f);
																																	 addChild(KSTimer::create(8.f/30.f, [=]()
																																							  {
																																								  addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
																																																		 {
																																																			 t_label->setPosition(ccp(440, myDSH->ui_center_y-35*t));
																																																			 t_label->setScale(1.2f-0.3f*t);
																																																			 t_label->setOpacity(255-t*255);
																																																		 }, [=](float t)
																																																		 {
																																																			 t_label->setPosition(ccp(440, myDSH->ui_center_y-35));
																																																			 t_label->setScale(0.9f);
																																																			 t_label->setOpacity(0);
																																																			 t_label->removeFromParent();
																																																		 }));
																																							  }));
																																 }));
																					  }));
											   
											   int before_score = atoi(replay_score->getString());
											   damaged_score = damaged_score.getV() - t_damage;
											   replay_score->setString(CCString::createWithFormat("%.0f", float(damaged_score.getV() + before_score))->getCString());
										   }));
	
	
	
	
//	CCPoint base_position = ccp(110, myDSH->ui_center_y+70+40);
//	CCPoint random_position = ccp(rand()%41-20, rand()%31-15);
//	CCPoint sum_position = base_position + random_position;
//	CCPoint sub_position = sum_position - origin_position;
//	
//	float angle = atan2(sum_position.y - origin_position.y, sum_position.x - origin_position.x)/M_PI*180.f;
//	t_missile->setRotation(-angle);
//	
//	addChild(KSGradualValue<float>::create(0.f, 1.f, 20.f/30.f, [=](float t)
//										   {
//											   t_missile->setPosition(origin_position + sub_position*t);
//										   }, [=](float t)
//										   {
//											   t_missile->setPosition(origin_position + sub_position);
//											   CCPoint final_position = ccp(440,myDSH->ui_center_y);
//											   CCPoint t_sub_position = final_position - sum_position;
//											   
//											   float angle2 = atan2(final_position.y - sum_position.y, final_position.x - sum_position.x)/M_PI*180.f;
//											   t_missile->setRotation(-angle2);
//											   
//											   addChild(KSGradualValue<float>::create(0.f, 1.f, 10.f/30.f, [=](float t)
//																					  {
//																						  t_missile->setPosition(sum_position + t_sub_position*t);
//																					  }, [=](float t)
//																					  {
//																						  t_missile->setPosition(sum_position + t_sub_position);
//																						  t_missile->removeFromParent();
//																						  
//																						  CCSprite* bomb_img = KS::loadCCBI<CCSprite*>(this, "bossbomb2.ccbi").first;
//																						  bomb_img->setPosition(ccp(440, myDSH->ui_center_y));
//																						  addChild(bomb_img, myUIZorder);
//																						  
//																						  bomb_img->addChild(KSTimer::create(24.f/30.f, [=](){bomb_img->removeFromParent();}));
//																						  
//																						  KSLabelTTF* t_label = KSLabelTTF::create(CCString::createWithFormat("%d", -t_damage)->getCString(), mySGD->getFont().c_str(), 12);
//																						  t_label->setColor(ccRED);
//																						  t_label->enableOuterStroke(ccBLACK, 1.f);
//																						  t_label->setPosition(ccp(440, myDSH->ui_center_y));
//																						  t_label->setScale(0.7f);
//																						  addChild(t_label, myUIZorder);
//																						  
//																						  addChild(KSGradualValue<float>::create(0.f, 1.f, 5.f/30.f, [=](float t)
//																																 {
//																																	 t_label->setOpacity(t*255);
//																																	 t_label->setScale(0.7f+t);
//																																 }, [=](float t)
//																																 {
//																																	 t_label->setOpacity(255);
//																																	 t_label->setScale(1.7f);
//																																	 addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
//																																											{
//																																												t_label->setScale(1.7f-t*0.5f);
//																																											}, [=](float t)
//																																											{
//																																												t_label->setScale(1.2f);
//																																												addChild(KSTimer::create(8.f/30.f, [=]()
//																																																		 {
//																																																			 addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
//																																																													{
//																																																														t_label->setPosition(ccp(440, myDSH->ui_center_y-25*t));
//																																																														t_label->setScale(1.2f-0.3f*t);
//																																																														t_label->setOpacity(255-t*255);
//																																																													}, [=](float t)
//																																																													{
//																																																														t_label->setPosition(ccp(440, myDSH->ui_center_y-25));
//																																																														t_label->setScale(0.9f);
//																																																														t_label->setOpacity(0);
//																																																														t_label->removeFromParent();
//																																																													}));
//																																																		 }));
//																																											}));
//																																 }));
//																						  
//																						  int before_score = atoi(replay_score->getString());
//																						  damaged_score = damaged_score.getV() - t_damage;
//																						  replay_score->setString(CCString::createWithFormat("%d", damaged_score.getV() + before_score)->getCString());
//																						  
//																					  }));
//										   }));
	
	
	
	
	
//	addChild(KSGradualValue<float>::create(0.f, 1.f, 32.f/30.f, [=](float t)
//										   {
//											   t_missile->setScale(0.3f+t*0.3f);
//											   t_missile->setRotation(-720*t);
//										   }, [=](float t)
//										   {
//											   t_missile->setScale(0.6f);
//											   t_missile->setRotation(-720);
//										   }));
//	
//	addChild(KSGradualValue<float>::create(0.f, 1.f, 11.f/30.f, [=](float t)
//   {
//	   t_missile->setPosition(ccp(40+130*t, myDSH->ui_center_y+70+20*t));
//   }, [=](float t)
//   {
//	   t_missile->setPosition(ccp(40+130, myDSH->ui_center_y+70+20));
//	   addChild(KSGradualValue<float>::create(0.f, 1.f, 11.f/30.f, [=](float t)
//	  {
//		  t_missile->setPosition(ccp(40+130+150*t, myDSH->ui_center_y+70+20-25*t));
//	  }, [=](float t)
//	  {
//		  t_missile->setPosition(ccp(40+130+150, myDSH->ui_center_y+70+20-25)); // 320
//		  addChild(KSGradualValue<float>::create(0.f, 1.f, 10.f/30.f, [=](float t)
//		 {
//			 t_missile->setPosition(ccp(40+130+150+120*t, myDSH->ui_center_y+70+20-25-65*t));
//		 }, [=](float t)
//		 {
//			 t_missile->setPosition(ccp(40+130+150+120, myDSH->ui_center_y+70+20-25-65));
//			 showScoreMissileEffect(ccp(440, myDSH->ui_center_y));
//			 addChild(KSTimer::create(2.f/30.f, [=]()
//		  {
//			  addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
//			 {
//				 t_missile->setScale(0.6f-0.6f*t);
//			 }, [=](float t)
//			 {
//				 t_missile->removeFromParent();
//			 }));
//			  
//			  CCSprite* bomb_img = KS::loadCCBI<CCSprite*>(this, "bossbomb2.ccbi").first;
//			  bomb_img->setPosition(ccp(440, myDSH->ui_center_y));
//			  addChild(bomb_img, myUIZorder);
//			  
//			  bomb_img->addChild(KSTimer::create(24.f/30.f, [=](){bomb_img->removeFromParent();}));
//			  
//			  KSLabelTTF* t_label = KSLabelTTF::create(CCString::createWithFormat("%d", -t_damage)->getCString(), mySGD->getFont().c_str(), 12);
//			  t_label->setColor(ccRED);
//			  t_label->enableOuterStroke(ccBLACK, 1.f);
//			  t_label->setPosition(ccp(440, myDSH->ui_center_y));
//			  t_label->setScale(0.7f);
//			  addChild(t_label, myUIZorder);
//			  
//			  addChild(KSGradualValue<float>::create(0.f, 1.f, 5.f/30.f, [=](float t)
//			 {
//				 t_label->setOpacity(t*255);
//				 t_label->setScale(0.7f+t);
//			 }, [=](float t)
//			 {
//				 t_label->setOpacity(255);
//				 t_label->setScale(1.7f);
//				 addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
//				{
//					t_label->setScale(1.7f-t*0.5f);
//				}, [=](float t)
//				{
//					t_label->setScale(1.2f);
//					addChild(KSTimer::create(8.f/30.f, [=]()
//				 {
//					 addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
//					{
//						t_label->setPosition(ccp(440, myDSH->ui_center_y-25*t));
//						t_label->setScale(1.2f-0.3f*t);
//						t_label->setOpacity(255-t*255);
//					}, [=](float t)
//					{
//						t_label->setPosition(ccp(440, myDSH->ui_center_y-25));
//						t_label->setScale(0.9f);
//						t_label->setOpacity(0);
//						t_label->removeFromParent();
//					}));
//				 }));
//				}));
//			 }));
//			  
//			  int before_score = atoi(replay_score->getString());
//			  damaged_score = damaged_score.getV() - t_damage;
//			  replay_score->setString(CCString::createWithFormat("%d", damaged_score.getV() + before_score)->getCString());
//			  
//		  }));
//		 }));
//	  }));
//   }));
	
}

void Maingame::refreshThumb()
{
	if(!myMS->isVisible())
		return;
	VisibleSprite* t_vs = (VisibleSprite*)myMS->getVisibleSprite();
	thumb_texture->beginWithClear(0, 0, 0.f, 0.f);
	t_vs->visitForThumb();
	thumb_texture->end();
	
	
	vector<KSCumberBase*> boss_array = myGD->getMainCumberVector();
	while(boss_thumbs->count() > boss_array.size())
	{
		CCNode* boss_position_img = (CCNode*)boss_thumbs->lastObject();
		boss_thumbs->removeObject(boss_position_img);
		boss_position_img->removeFromParent();
	}
	while (boss_thumbs->count() < boss_array.size())
	{
		CCSprite* boss_position_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 6, 6));
		boss_position_img->setColor(ccRED);
		addChild(boss_position_img, myUIZorder);
		
		boss_thumbs->addObject(boss_position_img);
	}
	
	for(int i=0;i<boss_array.size();i++)
	{
		CCNode* boss_position_img = (CCNode*)boss_thumbs->objectAtIndex(i);
		CCNode* boss_pointer = (CCNode*)boss_array[i];
		boss_position_img->setPosition(ccpAdd(thumb_base_position, ccpMult(boss_pointer->getPosition(), thumb_texture->getScale())));//thumb_scale)));
	}
	
	
//	CCNode* boss_pointer = myGD->getCommunicationNode("CP_getMainCumberPointer");
//	boss_thumb->setPosition(ccpAdd(thumb_base_position, ccpMult(boss_pointer->getPosition(), thumb_texture->getScale())));
	
	vector<KSCumberBase*> sub_array = myGD->getSubCumberVector();
	while(sub_thumbs->count() > sub_array.size())
	{
		CCNode* sub_position_img = (CCNode*)sub_thumbs->lastObject();
		sub_thumbs->removeObject(sub_position_img);
		sub_position_img->removeFromParent();
	}
	while (sub_thumbs->count() < sub_array.size())
	{
		CCSprite* sub_position_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 4, 4));
		sub_position_img->setColor(ccYELLOW);
		addChild(sub_position_img, clearshowtimeZorder);
		
		sub_thumbs->addObject(sub_position_img);
	}
	
	for(int i=0;i<sub_array.size();i++)
	{
		CCNode* sub_position_img = (CCNode*)sub_thumbs->objectAtIndex(i);
		CCNode* sub_pointer = (CCNode*)sub_array[i];
		sub_position_img->setPosition(ccpAdd(thumb_base_position, ccpMult(sub_pointer->getPosition(), thumb_texture->getScale())));//thumb_scale)));
	}
	
	t_vs->visit();
}

void Maingame::refreshReplayThumb(int temp_time)
{
	if(!myMS->isVisible())
		return;
	
	int play_index = mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_playIndex)].asInt();
	if(mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapTime)].size() <= play_index || mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapTime)][play_index].asInt() > temp_time)
		return;
	
	VisibleSprite* t_vs = (VisibleSprite*)myMS->getVisibleSprite();
	replay_thumb_texture->beginWithClear(0, 0.f, 0, 0.f);
	t_vs->replayVisitForThumb(temp_time);
	replay_thumb_texture->end();
	
	t_vs->visit();
}

void Maingame::refreshReplayScore(int temp_time)
{
	int score_index = mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_timeStamp)][temp_time][mySGD->getReplayKey(kReplayKey_timeStamp_scoreIndex)].asInt();
	if(score_index <= 0)
		return;
	if(mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_scoreTime)][score_index].asInt() > temp_time)
		return;
	
	replay_score->setString(CCString::createWithFormat("%.0f", float(damaged_score.getV() + mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_scoreData)][score_index].asInt()))->getCString());
}

void Maingame::refreshReplayPosition(int temp_time)
{
	if(mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)].size() <= temp_time)
		return;
	
	CCPoint replay_base_position = ccpAdd(replay_thumb_texture->getPosition(), ccp(-160.f*replay_thumb_texture->getScale(), -215.f*replay_thumb_texture->getScale()));
	
	Json::Value position_data = mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_timeStamp)][temp_time];
	
	CCPoint character_position;
	character_position.x = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_characterPositionX)].asFloat();
	character_position.y = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_characterPositionY)].asFloat();
	replay_character->setPosition(ccpAdd(replay_base_position, ccpMult(character_position, replay_thumb_texture->getScale())));
	replay_character->setVisible(true);
	
	int boss_count = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_boss)].size();
	
	while(replay_boss->count() > boss_count)
	{
		CCNode* boss_position_img = (CCNode*)replay_boss->lastObject();
		replay_boss->removeObject(boss_position_img);
		boss_position_img->removeFromParent();
	}
	while(replay_boss->count() < boss_count)
	{
		CCSprite* boss_position_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 6, 6));
		boss_position_img->setColor(ccRED);
		replay_all_node->addChild(boss_position_img);
		
		replay_boss->addObject(boss_position_img);
	}
	
	for(int i=0;i<replay_boss->count();i++)
	{
		CCNode* boss_position_img = (CCNode*)replay_boss->objectAtIndex(i);
		CCPoint cumber_position;
		cumber_position.x = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_boss)][i][mySGD->getReplayKey(kReplayKey_timeStamp_boss_x)].asFloat();
		cumber_position.y = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_boss)][i][mySGD->getReplayKey(kReplayKey_timeStamp_boss_y)].asFloat();
		boss_position_img->setPosition(ccpAdd(replay_base_position, ccpMult(cumber_position, replay_thumb_texture->getScale())));
		boss_position_img->setVisible(true);
	}
	
//	CCPoint boss_position;
//	boss_position.x = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_bossPositionX)].asFloat();
//	boss_position.y = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_bossPositionY)].asFloat();
//	replay_boss->setPosition(ccpAdd(replay_base_position, ccpMult(boss_position, replay_thumb_texture->getScale())));
//	replay_boss->setVisible(true);
	

	int monster_count = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_monster)].size();
	
	while(replay_sub->count() > monster_count)
	{
		CCNode* sub_position_img = (CCNode*)replay_sub->lastObject();
		replay_sub->removeObject(sub_position_img);
		sub_position_img->removeFromParent();
	}
	while(replay_sub->count() < monster_count)
	{
		CCSprite* sub_position_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 4, 4));
		sub_position_img->setColor(ccYELLOW);
		replay_all_node->addChild(sub_position_img);
		
		replay_sub->addObject(sub_position_img);
	}
	
	for(int i=0;i<replay_sub->count();i++)
	{
		CCNode* sub_position_img = (CCNode*)replay_sub->objectAtIndex(i);
		CCPoint cumber_position;
		cumber_position.x = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_monster)][i][mySGD->getReplayKey(kReplayKey_timeStamp_monster_x)].asFloat();
		cumber_position.y = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_monster)][i][mySGD->getReplayKey(kReplayKey_timeStamp_monster_y)].asFloat();
		sub_position_img->setPosition(ccpAdd(replay_base_position, ccpMult(cumber_position, replay_thumb_texture->getScale())));
		sub_position_img->setVisible(true);
	}
	
	if(position_data[mySGD->getReplayKey(kReplayKey_timeStamp_isDie)].asBool())
	{
		CCSprite* t_continue = CCSprite::create("endless_life.png");
		t_continue->setScale(0.f);
		t_continue->setOpacity(0);
		t_continue->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), 215.f*replay_thumb_texture->getScale())));
		replay_all_node->addChild(t_continue);
		
		CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 1.f);
		CCFadeTo* t_fade1 = CCFadeTo::create(0.3f, 255);
		CCSpawn* t_spawn1 = CCSpawn::create(t_scale1, t_fade1, NULL);
		
		CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
		
		CCScaleTo* t_scale2 = CCScaleTo::create(0.3f, 0.f);
		CCFadeTo* t_fade2 = CCFadeTo::create(0.3f, 0);
		CCSpawn* t_spawn2 = CCSpawn::create(t_scale2, t_fade2, NULL);
		
		CCCallFunc* t_call = CCCallFunc::create(t_continue, callfunc_selector(CCSprite::removeFromParent));
		
		CCSequence* t_seq = CCSequence::create(t_spawn1, t_delay1, t_spawn2, t_call, NULL);
		
		t_continue->runAction(t_seq);
		
//		CCLabelTTF* die_label = CCLabelTTF::create("Life -1", mySGD->getFont().c_str(), 10);
//		die_label->setScale(0.5f);
//		die_label->setColor(ccRED);
//		die_label->setPosition(replay_character->getPosition());
//		replay_all_node->addChild(die_label);
//		
//		CCScaleTo* t_scale = CCScaleTo::create(1.5f, 5.f);
//		CCFadeTo* t_fade = CCFadeTo::create(1.5f, 0);
//		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_scale, t_fade);
//		CCCallFunc* t_call = CCCallFunc::create(die_label, callfunc_selector(CCLabelTTF::removeFromParent));
//		CCSequence* t_seq = CCSequence::createWithTwoActions(t_spawn, t_call);
//		die_label->runAction(t_seq);
	}
	
	if(position_data[mySGD->getReplayKey(kReplayKey_timeStamp_isImageChange)].asBool())
	{
		CCSprite* t_continue = CCSprite::create("endless_change.png");
		t_continue->setScale(0.f);
		t_continue->setOpacity(0);
		t_continue->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), 215.f*replay_thumb_texture->getScale())));
		replay_all_node->addChild(t_continue);
		
		CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 1.f);
		CCFadeTo* t_fade1 = CCFadeTo::create(0.3f, 255);
		CCSpawn* t_spawn1 = CCSpawn::create(t_scale1, t_fade1, NULL);
		
		CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
		
		CCScaleTo* t_scale2 = CCScaleTo::create(0.3f, 0.f);
		CCFadeTo* t_fade2 = CCFadeTo::create(0.3f, 0);
		CCSpawn* t_spawn2 = CCSpawn::create(t_scale2, t_fade2, NULL);
		
		CCCallFunc* t_call = CCCallFunc::create(t_continue, callfunc_selector(CCSprite::removeFromParent));
		
		CCSequence* t_seq = CCSequence::create(t_spawn1, t_delay1, t_spawn2, t_call, NULL);
		
		t_continue->runAction(t_seq);
		
//		CCLabelTTF* change_label = CCLabelTTF::create("체인지", mySGD->getFont().c_str(), 14);
//		change_label->setColor(ccYELLOW);
//		change_label->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), 215.f*replay_thumb_texture->getScale()+10)));
//		replay_all_node->addChild(change_label);
	}
	
	int game_info = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_gameInfo)].asInt();
	if(game_info == 1)
	{
		CCSprite* t_continue = CCSprite::create("endless_clear.png");
		t_continue->setScale(0.f);
		t_continue->setOpacity(0);
		t_continue->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), 215.f*replay_thumb_texture->getScale())));
		replay_all_node->addChild(t_continue);
		
		CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 1.f);
		CCFadeTo* t_fade1 = CCFadeTo::create(0.3f, 255);
		CCSpawn* t_spawn1 = CCSpawn::create(t_scale1, t_fade1, NULL);
		
//		CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
//		
//		CCScaleTo* t_scale2 = CCScaleTo::create(0.3f, 0.f);
//		CCFadeTo* t_fade2 = CCFadeTo::create(0.3f, 0);
//		CCSpawn* t_spawn2 = CCSpawn::create(t_scale2, t_fade2, NULL);
//		
//		CCCallFunc* t_call = CCCallFunc::create(t_continue, callfunc_selector(CCSprite::removeFromParent));
//		
//		CCSequence* t_seq = CCSequence::create(t_spawn1, t_delay1, t_spawn2, t_call, NULL);
		
		t_continue->runAction(t_spawn1);
		
//		CCLabelTTF* clear_label = CCLabelTTF::create("클리어", mySGD->getFont().c_str(), 14);
//		clear_label->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), 215.f*replay_thumb_texture->getScale()-10)));
//		replay_all_node->addChild(clear_label);
		
		replay_character->setVisible(false);
		
		int replay_boss_loop = replay_boss->count();
		for(int i=0;i<replay_boss_loop;i++)
		{
			((CCSprite*)replay_boss->objectAtIndex(i))->setVisible(false);
		}
		
		int replay_sub_loop = replay_sub->count();
		for(int i=0;i<replay_sub_loop;i++)
		{
			((CCSprite*)replay_sub->objectAtIndex(i))->setVisible(false);
		}
	}
	else if(game_info == -1)
	{
		CCSprite* t_continue = CCSprite::create("endless_gameover.png");
		t_continue->setScale(0.f);
		t_continue->setOpacity(0);
		t_continue->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), 215.f*replay_thumb_texture->getScale())));
		replay_all_node->addChild(t_continue);
		
		CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 1.f);
		CCFadeTo* t_fade1 = CCFadeTo::create(0.3f, 255);
		CCSpawn* t_spawn1 = CCSpawn::create(t_scale1, t_fade1, NULL);
		
//		CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
//		
//		CCScaleTo* t_scale2 = CCScaleTo::create(0.3f, 0.f);
//		CCFadeTo* t_fade2 = CCFadeTo::create(0.3f, 0);
//		CCSpawn* t_spawn2 = CCSpawn::create(t_scale2, t_fade2, NULL);
//		
//		CCCallFunc* t_call = CCCallFunc::create(t_continue, callfunc_selector(CCSprite::removeFromParent));
//		
//		CCSequence* t_seq = CCSequence::create(t_spawn1, t_delay1, t_spawn2, t_call, NULL);
		
		t_continue->runAction(t_spawn1);
		
//		CCLabelTTF* game_over_label = CCLabelTTF::create("게임오버", mySGD->getFont().c_str(), 12);
//		game_over_label->setColor(ccBLACK);
//		game_over_label->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), 215.f*replay_thumb_texture->getScale()-10)));
//		replay_all_node->addChild(game_over_label);
		
		replay_character->setVisible(false);
		
		int replay_boss_loop = replay_boss->count();
		for(int i=0;i<replay_boss_loop;i++)
		{
			((CCSprite*)replay_boss->objectAtIndex(i))->setVisible(false);
		}
		
		int replay_sub_loop = replay_sub->count();
		for(int i=0;i<replay_sub_loop;i++)
		{
			((CCSprite*)replay_sub->objectAtIndex(i))->setVisible(false);
		}
	}
	
	if(position_data[mySGD->getReplayKey(kReplayKey_timeStamp_isContinue)].asBool())
	{
		CCSprite* t_continue = CCSprite::create("endless_continue.png");
		t_continue->setScale(0.f);
		t_continue->setOpacity(0);
		t_continue->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), 215.f*replay_thumb_texture->getScale())));
		replay_all_node->addChild(t_continue);

		CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 1.f);
		CCFadeTo* t_fade1 = CCFadeTo::create(0.3f, 255);
		CCSpawn* t_spawn1 = CCSpawn::create(t_scale1, t_fade1, NULL);
		
		CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
		
		CCScaleTo* t_scale2 = CCScaleTo::create(0.3f, 0.f);
		CCFadeTo* t_fade2 = CCFadeTo::create(0.3f, 0);
		CCSpawn* t_spawn2 = CCSpawn::create(t_scale2, t_fade2, NULL);
		
		CCCallFunc* t_call = CCCallFunc::create(t_continue, callfunc_selector(CCSprite::removeFromParent));
		
		CCSequence* t_seq = CCSequence::create(t_spawn1, t_delay1, t_spawn2, t_call, NULL);
		
		t_continue->runAction(t_seq);
		
		replay_continue_count++;
//		if(!replay_continue_label)
//		{
//			replay_continue_label = CCLabelTTF::create("이어하기 : 1", mySGD->getFont().c_str(), 8);
//			replay_continue_label->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), -5)));
//			replay_all_node->addChild(replay_continue_label);
//		}
//		else
//			replay_continue_label->setString(CCString::createWithFormat("이어하기 : %d", replay_continue_count)->getCString());
//		
//		CCLabelTTF* continue_label = CCLabelTTF::create("이어하기!!", mySGD->getFont().c_str(), 10);
//		continue_label->setScale(0.5f);
//		continue_label->setColor(ccMAGENTA);
//		continue_label->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), 215.f*replay_thumb_texture->getScale())));
//		replay_all_node->addChild(continue_label);
//		
//		CCScaleTo* t_scale = CCScaleTo::create(1.5f, 5.f);
//		CCFadeTo* t_fade = CCFadeTo::create(1.5f, 0);
//		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_scale, t_fade);
//		CCCallFunc* t_call = CCCallFunc::create(continue_label, callfunc_selector(CCLabelTTF::removeFromParent));
//		CCSequence* t_seq = CCSequence::createWithTwoActions(t_spawn, t_call);
//		continue_label->runAction(t_seq);
	}
}

void Maingame::removeSearchEye(CCObject* t_node)
{
	for(auto iter=search_eye_vector.begin();iter!=search_eye_vector.end();iter++)
	{
		SearchEye* t_search_eye = *iter;
		if(t_search_eye == t_node)
		{
			search_eye_vector.erase(iter);
			return;
		}
	}
}

void Maingame::hideThumb()
{
	if(mySGD->is_endless_mode)
	{
		endless_you_bomb_node->setVisible(false);
	}
	myUI->hideThumb();
	sil_thumb->setVisible(false);
	
	for(int i=0;i<search_eye_vector.size();i++)
	{
		SearchEye* t_search_eye = search_eye_vector[i];
//		t_search_eye->setVisible(false);
		t_search_eye->removeFromParent();
	}
	search_eye_vector.clear();
	
	mControl->setVisible(false);
	
	
	thumb_texture->setVisible(false);
	character_thumb->setVisible(false);
	
	int boss_loop = boss_thumbs->count();
	for(int i=0;i<boss_loop;i++)
	{
		CCSprite* t_boss = (CCSprite*)boss_thumbs->objectAtIndex(i);
		t_boss->setVisible(false);
	}
	
	screen_node->setVisible(false);
	
	int sub_loop = sub_thumbs->count();
	for(int i=0;i<sub_loop;i++)
	{
		CCSprite* t_sub = (CCSprite*)sub_thumbs->objectAtIndex(i);
		t_sub->setVisible(false);
	}
	
	thumb_case->setVisible(false);
//	thumb_case_top->setVisible(false);
//	thumb_case_down->setVisible(false);
//	thumb_case_left->setVisible(false);
//	thumb_case_right->setVisible(false);
	
	if(mySGD->is_play_replay)
	{
		replay_all_node->setVisible(false);
		replay_bomb->setVisible(false);
	}
}

void Maingame::showShop(int t_shopcode)
{
	if(is_pause)
		return;
	
	is_pause = true;
	
	bool t_jack_stun = myJack->isStun;
	
	mySGD->is_on_maingame = false;
	
	CCNode* exit_target = this;
	mControl->setTouchEnabled(false);
	exit_target->onExit();
	
	ShopPopup* t_popup = ShopPopup::create();
	t_popup->setScale(myDSH->screen_convert_rate);
	t_popup->setShopCode((ShopCode)t_shopcode);
	t_popup->setCloseFunc([=]()
						  {
							  mControl->isStun = false;
							  startControl();
							  myJack->isStun = t_jack_stun;
							  exit_target->onEnter();
							  mySGD->is_on_maingame = true;
							  is_pause = false;
						  });
	exit_target->getParent()->addChild(t_popup);
	
	CCSprite* dimmed_sprite = CCSprite::create("whitePaper.png");
	dimmed_sprite->setPosition(ccp(240, myDSH->ui_center_y));
	dimmed_sprite->setColor(ccc3(0, 0, 0));
	dimmed_sprite->setOpacity(100);
	dimmed_sprite->setScaleY(myDSH->ui_top/320);
	t_popup->addChild(dimmed_sprite, -1);
	
//	ASPopupView* t_popup = ASPopupView::create(-200);
//	
//	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//	if(screen_scale_x < 1.f)
//		screen_scale_x = 1.f;
//	
//	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
//	t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
//	t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
//	
//	PauseContent* t_container = PauseContent::create(t_popup->getTouchPriority(), [=]()
//													 {
//														 mControl->isStun = false;
//														 myJack->isStun = t_jack_stun;
//														 exit_target->onEnter();
//														 cancelHome();
//														 is_pause = false;
//														 t_popup->removeFromParent();
//													 }, [=]()
//													 {
//														 mControl->isStun = false;
//														 myJack->isStun = t_jack_stun;
//														 exit_target->onEnter();
//														 myUI->stopCounting();
//														 goHome();
//														 is_pause = false;
//														 t_popup->removeFromParent();
//													 }, [=]()
//													 {
//														 mControl->isStun = false;
//														 myJack->isStun = t_jack_stun;
//														 exit_target->onEnter();
//														 goReplay();
//														 is_pause = false;
//														 t_popup->removeFromParent();
//													 });
//	
//	t_popup->setContainerNode(t_container);
//	exit_target->getParent()->addChild(t_popup);
//	t_container->startShow();
}

void Maingame::showClearTimeEvent(function<void()> no_func, function<void()> yes_func)
{
	if(is_gohome)
		return;
	
	if(is_pause)
		return;
	
	is_pause = true;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	bool t_jack_stun = myJack->isStun;
	
	mySGD->is_on_maingame = false;
	
	CCNode* exit_target = this;
	mControl->setTouchEnabled(false);
	exit_target->onExit();
	
	ASPopupView* t_popup = ASPopupView::create(-200);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
	t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
	t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
	
	ClearTimeEventContent* t_container = ClearTimeEventContent::create(t_popup->getTouchPriority(), [=]()
																	   {
																		   AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
																		   mControl->isStun = false;
																		   myJack->isStun = t_jack_stun;
																		   exit_target->onEnter();
																		   mySGD->is_on_maingame = true;
																		   is_pause = false;
																		   no_func();
																		   startControl();
																		   ((ControlJoystickButton*)mControl)->resetTouch();
																	   }, [=]()
																	   {
																		   AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
																		   mControl->isStun = false;
																		   myJack->isStun = t_jack_stun;
																		   exit_target->onEnter();
																		   mySGD->is_on_maingame = true;
																		   is_pause = false;
																		   yes_func();
																		   startControl();
																		   ((ControlJoystickButton*)mControl)->resetTouch();
																	   });
	t_popup->setContainerNode(t_container);
	exit_target->getParent()->addChild(t_popup);
	t_container->startShow();
}

void Maingame::showPause()
{
	if(is_gohome)
		return;
	
	if(is_pause)
		return;
	
	is_pause = true;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	bool t_jack_stun = myJack->isStun;
	
	mySGD->is_on_maingame = false;
	
	CCNode* exit_target = this;
	if(mControl)
		mControl->setTouchEnabled(false);
	exit_target->onExit();
	
	ASPopupView* t_popup = ASPopupView::create(-200);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
	t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
	t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
	
	PauseContent* t_container = PauseContent::create(t_popup->getTouchPriority(), [=]()
	{
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
		if(mControl)
			mControl->isStun = false;
		myJack->isStun = t_jack_stun;
		exit_target->onEnter();
		mySGD->is_on_maingame = true;
		cancelHome();
		is_pause = false;
		t_popup->removeFromParent();
	}, [=]()
	{
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
		if(mControl)
			mControl->isStun = false;
		myJack->isStun = t_jack_stun;
		exit_target->onEnter();
		myUI->stopCounting();
		if(t_smg)
			t_smg->outAction();
		goHome();
		is_pause = false;
		t_popup->removeFromParent();
	}, [=]()
	{
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
		if(mControl)
			mControl->isStun = false;
		myJack->isStun = t_jack_stun;
		exit_target->onEnter();
		goReplay();
		is_pause = false;
		t_popup->removeFromParent();
	});
	
	t_popup->setContainerNode(t_container);
	exit_target->getParent()->addChild(t_popup);
	t_container->startShow();
}
void Maingame::goHome ()
{
	is_gohome = true;
	myLog->addLog(kLOG_getCoin_i, -1, mySGD->getStageGold());
	
//	myLog->sendLog(CCString::createWithFormat("home_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)))->getCString());
	AudioEngine::sharedInstance()->stopAllEffects();
	AudioEngine::sharedInstance()->stopSound();
	
	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_fail);
	
	mySGD->is_paused = false;
	AudioEngine::sharedInstance()->setAppFore();
	gameover();
}
void Maingame::goReplay ()
{
//	setBackKeyEnabled(false);
	
	mySGD->setUserdataAchieveNoFail(0);
	for(int i=kAchievementCode_fail1;i<=kAchievementCode_fail3;i++)
	{
		if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) &&
		   mySGD->getUserdataAchieveFail() + 1 >= myAchieve->getCondition((AchievementCode)i))
		{
			myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
			AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
			CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
		}
	}
	
	mySGD->setUserdataAchieveFail(mySGD->getUserdataAchieveFail() + 1);
	mySGD->setUserdataFailCount(mySGD->getUserdataFailCount()+1);
	mySGD->changeUserdata(nullptr);
	
	
	myLog->addLog(kLOG_getCoin_i, -1, mySGD->getStageGold());
	
//	myLog->sendLog(CCString::createWithFormat("replay_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)))->getCString());
	AudioEngine::sharedInstance()->stopAllEffects();
	AudioEngine::sharedInstance()->stopSound();
	
	AudioEngine::sharedInstance()->playSound("bgm_ui.mp3", true);
	
	mySGD->is_paused = false;
	AudioEngine::sharedInstance()->setAppFore();
	mySGD->gameOver(0, 0, 0);
	mySGD->resetLabels();
	myGD->resetGameData();
	
	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stageSetting);
	CCDirector::sharedDirector()->replaceScene(PuzzleScene::scene());
}
void Maingame::cancelHome ()
{
	startControl();
	mySGD->is_paused = false;
	AudioEngine::sharedInstance()->setAppFore();
}
void Maingame::showContinue(CCObject * t_end, SEL_CallFunc d_end, CCObject * t_continue, SEL_CallFunc d_continue)
{
	bool t_jack_stun = myJack->isStun;
	
	mySGD->is_on_maingame = false;
	
	CCNode* exit_target = this;
	mControl->setTouchEnabled(false);
	exit_target->onExit();
	
	ASPopupView* t_popup = ASPopupView::create(-200);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
	t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
	t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
	
	ContinueContent* t_container = ContinueContent::create(t_popup->getTouchPriority(), [=]()
														   {
															   mControl->isStun = false;
															   myJack->isStun = t_jack_stun;
															   exit_target->onEnter();
															   is_gohome = true;
															   mySGD->is_on_maingame = true;
															   mySGD->is_paused = false;
															   myUI->stopCounting();
															   AudioEngine::sharedInstance()->setAppFore();
															   (t_end->*d_end)();
																 t_popup->removeFromParent();
														   }, [=]()
														   {
															   mControl->isStun = false;
															   myJack->isStun = t_jack_stun;
															   exit_target->onEnter();
															   mySGD->is_on_maingame = true;
															   (t_continue->*d_continue)();
															   continueAction();
																 
																 t_popup->removeFromParent();
														   });
	
	t_popup->setContainerNode(t_container);
	exit_target->getParent()->addChild(t_popup);
	t_container->startShow();
}
void Maingame::continueAction()
{
	myLog->addLog(kLOG_action_continue, -1);
	
//	myDSH->saveUserData({kSaveUserData_Key_star}, nullptr);
	
	startControl();
	mySGD->is_paused = false;
	AudioEngine::sharedInstance()->setAppFore();
}

void Maingame::changeMonsterAutoLevel()
{
    // ######################## hs code bbu woo~ re autobalance ##############################
    std::vector<KSCumberBase*> maincumbers = myCP->getMainCumbers();
    for(int i=0;i<maincumbers.size();i++){
        ((KSCumberBase*)maincumbers[i])->applyAutoBalance(mySGD->is_exchanged);
    }
    
    std::vector<KSCumberBase*> subcumbers = myCP->getSubCumberArrayPointer();
    for(int i=0;i<subcumbers.size();i++){
        ((KSCumberBase*)subcumbers[i])->applyAutoBalance(mySGD->is_exchanged);
    }
    // ######################## hs code bbu woo~ ##############################
}
