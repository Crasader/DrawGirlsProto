//
//  ZoomScriptScene.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 6..
//
//

#include "ZoomScriptScene.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "StageImgLoader.h"
#include "PuzzleMapScene.h"
#include "PuzzleScene.h"
#include "utf8.h"
#include "MainFlowScene.h"
#include "CommonButton.h"
#include "NewMainFlowScene.h"
#include "bustMorphing.h"
#include "MyLocalization.h"
#include "RankUpPopup.h"
#include "FormSetter.h"
#include "CommonAnimation.h"
#include "AlertEngine.h"

#define ZS_SCROLL_SPEED_MAX_BASE	20
#define ZS_SCROLL_SPEED_DECEASE_BASE	0.2f

CCScene* ZoomScript::scene()
{
    CCScene *scene = CCScene::create();
    ZoomScript *layer = ZoomScript::create();
    scene->addChild(layer);
	
    return scene;
}

enum ZS_Zorder{
	kZS_Z_back = 0,
	kZS_Z_first_img,
	kZS_Z_second_img,
	kZS_Z_script_case,
	kZS_Z_script_label,
	kZS_Z_next_button,
	kZS_Z_showtime_back,
	kZS_Z_whitePaper
};

bool ZoomScript::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
//	setBackKeyFunc([=](){
//		CCNode* t_node = CCNode::create();
//		menuAction(t_node);
//	});
//	setBackKeyEnabled(true);
	
	setKeypadEnabled(true);
	
    safety_img = NULL;
    
	AudioEngine::sharedInstance()->playSound("bgm_normalshow.mp3", true);
	
	typing_sound_number = 1;
	
	is_rankup = false;
	is_time_event_card_on = false;
	is_ccb_end = false;
	is_next_on = false;
	
//	CCLayer* top_bottom_layer = CCLayer::create();
//	top_bottom_layer->setPosition(ccp(0, 0));
//	addChild(top_bottom_layer, kZS_Z_back);
//	
//	CCSpriteBatchNode* side_back = CCSpriteBatchNode::create("ingame_side_pattern.png");
//	top_bottom_layer->addChild(side_back);
//	
//	CCSize pattern_size = side_back->getTexture()->getContentSize();
//	
//	for(int i=0;i*pattern_size.width < 480;i++)
//	{
//		for(int j=0;j*pattern_size.height < myDSH->ui_top;j++)
//		{
//			CCSprite* t_pattern = CCSprite::createWithTexture(side_back->getTexture());
//			t_pattern->setAnchorPoint(ccp(0,0));
//			t_pattern->setPosition(ccp(i*pattern_size.width,j*pattern_size.height));
//			side_back->addChild(t_pattern);
//		}
//	}
	
	CCSprite* back_img = CCSprite::create("main_back.png");
	back_img->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(back_img, kZS_Z_back);
	
	card_back = KS::loadCCBI<CCSprite*>(this, "zoom_back.ccbi").first;
	card_back->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(card_back, kZS_Z_back);
	
//	CCSprite* title_name = CCSprite::create("temp_title_name.png");
//	title_name->setPosition(ccp(240,myDSH->ui_center_y));
//	addChild(title_name, kZS_Z_back);
	
	game_node = CCNode::create();
	game_node->setAnchorPoint(ccp(0.5,0.5));
	game_node->setContentSize(CCSizeMake(320,460));
	game_node->setPosition(ccp(240,myDSH->ui_center_y));
	setFormSetter(game_node);
	addChild(game_node, kZS_Z_first_img);
	
	silType = mySD->getSilType();
	
	is_showtime = mySGD->is_showtime;
//	is_exchanged = mySGD->is_exchanged;
	
	second_img = NULL;
	
	int card_number;
	
	if(mySGD->getStageGrade() >= 3)	card_number = NSDS_GI(silType, kSDS_SI_level_int1_card_i, 3);
	else							card_number = NSDS_GI(silType, kSDS_SI_level_int1_card_i, 1);
	
	int sound_count = NSDS_GI(kSDS_CI_int1_soundCnt_i, card_number);
	for(int i=1;i<=sound_count;i++)
	{
		AudioEngine::sharedInstance()->preloadGroanEffect(NSDS_GS(kSDS_CI_int1_soundType_int1_s, card_number, i));
	}
	
	int t_t_card;
	if(mySGD->getStageGrade() >= 3)	t_t_card = NSDS_GI(silType, kSDS_SI_level_int1_card_i, 4);
	else							t_t_card = NSDS_GI(silType, kSDS_SI_level_int1_card_i, 2);
	
	sound_count = NSDS_GI(kSDS_CI_int1_soundCnt_i, t_t_card);
	for(int i=1;i<=sound_count;i++)
	{
		AudioEngine::sharedInstance()->preloadGroanEffect(NSDS_GS(kSDS_CI_int1_soundType_int1_s, t_t_card, i));
	}
	
	first_img = MyNode::create(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", card_number)->getCString()), card_number);

	if(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()))
		first_img->loadRGB(mySIL->getDocumentPath() + CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()); // 실루엣 z 정보 넣는 곳.

	if(mySGD->getStageGrade() == 4 || mySGD->getStageGrade() == 2)is_morphing = true;
	else is_morphing = false;
	
	first_img->setPosition(ccp(160,230));
	first_img->setAnchorPoint(ccp(0.5,0.5));
	first_img->setTouchEnabled(false);
	game_node->addChild(first_img, kZS_Z_first_img);
	
	ccb_manager = NULL;
	
	if((NSDS_GI(kSDS_CI_int1_grade_i, card_number) == 2 || NSDS_GI(kSDS_CI_int1_grade_i, card_number) == 4) && NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, card_number))
	{
		auto t_ccb = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, card_number));
		CCSprite* ccb_img = t_ccb.first;
		ccb_img->setPosition(ccp(160,215));
		first_img->addChild(ccb_img);
		
		ccb_manager = t_ccb.second;
		
		first_img->touch_ccb_func = [=]()
		{
			ccb_manager->runAnimationsForSequenceNamed("touch1");
		};
		
		ccb_manager->runAnimationsForSequenceNamed("Default Timeline");
	}
	
	
	if(mySGD->is_safety_mode)
	{
		safety_img = EffectSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()));
		safety_img->setSilhouetteConvert(0);
		safety_img->setPosition(ccp(160, 230));
		game_node->addChild(safety_img, kZS_Z_second_img);
	}
	
	CCPoint center_position = ccp(160,230);
	
	CCSprite* top_case = CCSprite::create("diary_frame_top.png");
	top_case->setPosition(ccpAdd(center_position, ccp(0,215)));
	game_node->addChild(top_case, kZS_Z_script_case);
	
	CCSprite* bottom_case = CCSprite::create("diary_frame_bottom.png");
	bottom_case->setPosition(ccpAdd(center_position, ccp(0,-215)));
	game_node->addChild(bottom_case, kZS_Z_script_case);
	
	CCSprite* left_case = CCSprite::create("diary_frame_left.png");
	left_case->setPosition(ccpAdd(center_position, ccp(-160,0)));
	game_node->addChild(left_case, kZS_Z_script_case);
	
	CCSprite* right_case = CCSprite::create("diary_frame_right.png");
	right_case->setPosition(ccpAdd(center_position, ccp(160,0)));
	game_node->addChild(right_case, kZS_Z_script_case);
	
	target_node = first_img;
	
//	zoom_img = CCSprite::create("ending_expand.png");
//	zoom_img->setPosition(ccp(445,myDSH->ui_top-35));
//	addChild(zoom_img, kZS_Z_script_case);
	
	script_label = CCLabelTTF::create("", mySGD->getFont().c_str(), 16, CCSizeMake(330, 25), CCTextAlignment::kCCTextAlignmentCenter,
									  CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
	script_label->setPosition(ccp(210,30));
	addChild(script_label, kZS_Z_script_label);
	
	script_case = CCScale9Sprite::create("common_shadowgray2.png", CCRectMake(0, 0, 23, 23), CCRectMake(10, 10, 3, 3));
	script_case->setVisible(false);
	script_case->setPosition(ccp(210,30));
	addChild(script_case, kZS_Z_script_case);
	
	showtime_morphing_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_showtimeMorphingTouch), mySGD->getFont().c_str(), 12);
	showtime_morphing_label->enableOuterStroke(ccBLACK, 1, int(255*0.6f), true);
	showtime_morphing_label->setAnchorPoint(ccp(0,0));
	showtime_morphing_label->setPosition(ccp(0, script_case->getContentSize().height+1));
	showtime_morphing_label->setVisible(false);
	script_case->addChild(showtime_morphing_label);
	
	
	next_button = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_ok),15,CCSizeMake(101,44), CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0, 0, 101, 44), CCRectMake(50, 21, 1, 2)), -160);
	next_button->setFunction([=](CCObject* sender){menuAction(sender);});
	next_button->setPosition(ccp(480-50,30));
	is_next_on = false;
	next_button->setVisible(is_next_on && is_ccb_end);
	addChild(next_button, kZS_Z_next_button);
	
	is_spin_mode = false;
	mode_button = CommonButton::create("이동", 10, CCSizeMake(40, 30), CommonButtonOrange, -160);
	mode_button->setPosition(ccp(480-50,myDSH->ui_top-30));
	mode_button->setFunction([=](CCObject* sender)
							 {
								 is_spin_mode = !is_spin_mode;
								 if(is_spin_mode)
									 mode_button->setTitle("회전");
								 else
									 mode_button->setTitle("이동");
							 });
	mode_button->setVisible(false);
	addChild(mode_button, kZS_Z_next_button);
	
//	is_touched_menu = false;
	is_actioned = true;
	
	screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	minimum_scale = (screen_size.height*320)/(screen_size.width*430)*1.25f;
	
	//game_node->setPosition(ccp(0,-430*1.5f+480.f*screen_size.height/screen_size.width));
	
	return true;
}

void ZoomScript::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
	CCMoveTo* move1 = CCMoveTo::create(0.5f, ccp(240,myDSH->ui_center_y));
	CCDelayTime* delay1 = CCDelayTime::create(1.f);
	
	CCMoveTo* move2 = CCMoveTo::create(0.5f, ccp(240,myDSH->ui_center_y));
	CCScaleTo* t_scale = CCScaleTo::create(0.5f, minimum_scale);
	CCSpawn* t_spawn = CCSpawn::create(move2, t_scale, NULL);
	
//	CCMoveTo* move2 = CCMoveTo::create(1.f, ccp(0,-430*1.5f+480.f*screen_size.height/screen_size.width));
	CCDelayTime* delay2 = CCDelayTime::create(0.5f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::startScript));
	game_node->runAction(CCSequence::create(move1, delay1, t_spawn, delay2, t_call, NULL));
}

void ZoomScript::startScript()
{
	save_text = NSDS_GS(kSDS_CI_int1_script_s, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, (mySGD->getStageGrade() >= 3 ? 3 : 1)));
	
	CCLabelTTF* t_label = CCLabelTTF::create(save_text.c_str(), mySGD->getFont().c_str(), 16);
	script_label->setDimensions(CCSizeMake(330, t_label->getContentSize().height*(ceil(t_label->getContentSize().width/330.f))));
	script_case->setContentSize(CCSizeMake(353, t_label->getContentSize().height*(ceil(t_label->getContentSize().width/330.f)) + 15));
	script_label->setString("");
	showtime_morphing_label->setPosition(ccp(0,script_case->getContentSize().height+1));
	
	basic_string<wchar_t> result;
	utf8::utf8to16(save_text.begin(), save_text.end(), back_inserter(result));
	text_length = result.length();
	typing_frame = 0;
	delegate_typing_after = callfunc_selector(ZoomScript::startTouchAction);
	schedule(schedule_selector(ZoomScript::typingAnimation), 1.f/10.f);
}

void ZoomScript::typingAnimation()
{
	typing_frame++;
	if(typing_frame <= text_length)
	{
		basic_string<wchar_t> result;
		utf8::utf8to16(save_text.begin(), save_text.end(), back_inserter(result));
		
		if(!(result[typing_frame]==' ' || result[typing_frame]=='\n'))
		{
			AudioEngine::sharedInstance()->playEffect("se_typing_5.mp3", false);//ccsf("se_typing_%d.mp3", typing_sound_number++), false);
			if(typing_sound_number > 4)
				typing_sound_number = 1;
		}
		
		result = result.substr(0, typing_frame);
		string conver;
		utf8::utf16to8(result.begin(), result.end(), back_inserter(conver));
		if(typing_frame < text_length)
			conver = conver + "_";
		script_label->setString(conver.c_str());
		script_case->setVisible(true);
		
		if(typing_frame == text_length)
		{
			AudioEngine::sharedInstance()->playEffect("sound_crashed_map.mp3", false);
			unschedule(schedule_selector(ZoomScript::typingAnimation));
			
			int grade =  NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number);
			if(grade == 2 || grade == 4)
			{
				auto tuto = KS::loadCCBI<CCSprite*>(this, "tutorial_touch.ccbi");
				zoom_img = tuto.first;
				tuto.second->runAnimationsForSequenceNamed("Default Timeline");
				zoom_img->setPosition(ccp(240, myDSH->ui_center_y));
				addChild(zoom_img, kZS_Z_script_case);
				
				tuto.second->setAnimationCompletedCallbackLambda(this, [=](const char* seqName){
					(this->*delegate_typing_after)();
					CCTouch* t_touch = new CCTouch();
					t_touch->setTouchInfo(0,240, myDSH->ui_center_y);
					t_touch->autorelease();
					target_node->ccTouchEnded(t_touch, NULL);
					is_ccb_end = true;
					next_button->setVisible(is_next_on && is_ccb_end);
				});
			}
			else
			{
				auto tuto = KS::loadCCBI<CCSprite*>(this, "tutorial_touch_rotation.ccbi");
				
				zoom_img = tuto.first;
				tuto.second->runAnimationsForSequenceNamed("Default Timeline");
				
				
				zoom_img->setPosition(ccp(240, myDSH->ui_center_y));
				addChild(zoom_img, kZS_Z_script_case);
				(this->*delegate_typing_after)();
				
				tuto.second->setAnimationCompletedCallbackLambda(this, [=](const char* seqName){
					is_ccb_end = true;
					next_button->setVisible(is_next_on && is_ccb_end);
				});
			}
		}
	}
	else
	{
		CCLog("typing length out");
		CCTouch* t_touch = new CCTouch();
		t_touch->setTouchInfo(0,240, myDSH->ui_center_y);
		t_touch->autorelease();
		
		if(NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 2 || NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 4)
			target_node->ccTouchEnded(t_touch, NULL);
		
		unschedule(schedule_selector(ZoomScript::typingAnimation));
		(this->*delegate_typing_after)();
	}
}

void ZoomScript::startTouchAction()
{
	is_actioned = false;
	setTouchEnabled(true);
	is_next_on = true;
	next_button->setVisible(is_next_on && is_ccb_end);
	script_case->setVisible(true);
//	mode_button->setVisible(true);
	
//	target_node->setTouchEnabled(true);
	
	is_scrolling = false;
	is_before_scrolling = is_scrolling;
	
	save_position = game_node->getPosition();
	//schedule(schedule_selector(ZoomScript::moveChecking));
}

void ZoomScript::moveChecking()
{
	CCPoint after_position = game_node->getPosition();
	
	if(is_scrolling)
	{
		if(NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 2 || NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 4)
			target_node->movingDistance(ccpSub(after_position, save_position));
		is_before_scrolling = is_scrolling;
	}
	else if(is_before_scrolling)
	{
		is_before_scrolling = false;
		if(NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 2 || NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 4)
			target_node->movingDistance(CCPointZero);
	}
	save_position = after_position;
}

void ZoomScript::menuAction(CCObject *sender)
{
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);

	
	if(!is_actioned)
	{
		//unschedule(schedule_selector(ZoomScript::moveChecking));
		
		is_actioned = true;
		is_next_on = false;
		next_button->setVisible(is_next_on && is_ccb_end);
		script_case->setVisible(false);
		script_label->setVisible(false);
		mode_button->setVisible(false);
		setTouchEnabled(false);
		target_node->setTouchEnabled(false);
		unschedule(schedule_selector(ZoomScript::moveAnimation));
		
		if(mySGD->is_hell_mode)
		{
			AudioEngine::sharedInstance()->stopSound();
			
			if(mySGD->isHasGottenCards(mySD->getSilType(), mySGD->getStageGrade()) > 0)
			{
				nextScene();
			}
			else
			{
				mySGD->is_clear_diary = true;
				
				CCScaleTo* t_scale = CCScaleTo::create(0.3f, 1.5f);
				CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(240,myDSH->ui_center_y));
				
				CCSpawn* t_spawn = CCSpawn::create(t_scale, t_move, NULL);
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::nextScene));
				CCSequence* t_seq = CCSequence::create(t_spawn, t_call, NULL);
				
				game_node->runAction(t_seq);
			}
		}
		else if(is_showtime)
		{
			AudioEngine::sharedInstance()->playEffect("ment_showtime.mp3");
			showtime_back = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_showtime), mySGD->getFont().c_str(), 45);// CCSprite::create("showtime_back.png");
			showtime_back->enableOuterStroke(ccc3(65, 5, 35), 2.5f, 255, true);
			showtime_back->setGradientColor(ccc4(255, 115, 250, 255), ccc4(215, 60, 130, 255), ccp(0,-1));
			KSLabelTTF* shadow = CommonAnimation::applyBigShadow(showtime_back, showtime_back->getFontSize());
			showtime_back->setScale(10.f);
			showtime_back->setPosition(ccp(240,myDSH->ui_center_y));
			showtime_back->setOpacity(0);
			addChild(showtime_back, kZS_Z_showtime_back);
			
			white_paper = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 480, 320));
			white_paper->setOpacity(0);
			white_paper->setScaleY(myDSH->ui_top/320.f);
			white_paper->setPosition(ccp(240,myDSH->ui_center_y));
			addChild(white_paper, kZS_Z_whitePaper);
			
			CCDelayTime* white_paper_delay = CCDelayTime::create(46.f/60.f);
			CCFadeTo* white_paper_fade = CCFadeTo::create(18.f/60.f, 255);
			CCSequence* white_paper_seq = CCSequence::createWithTwoActions(white_paper_delay, white_paper_fade);
			white_paper->runAction(white_paper_seq);
			
			showtime_back->addChild(KSGradualValue<float>::create(0.f, 1.f, 14.f/30.f, [=](float t)
																  {
																	  showtime_back->setScale(10-9*t);
																	  showtime_back->setOpacity(255*t);
																	  shadow->setOpacityOuterStroke(255 * 0.3f*(t));
																  }, [=](float t)
																  {
																	  showtime_back->setScale(10-9*t);
																	  showtime_back->setOpacity(255*t);
																	  shadow->setOpacityOuterStroke(255 * 0.3f*(t));
																	  showtime_back->addChild(KSTimer::create(9.f/30.f, [=]()
																											  {
																												  showtime_back->addChild(KSGradualValue<float>::create(0.f, 1.f, 9.f/30.f, [=](float t)
																																										{
																																											showtime_back->setScale(1+11*t);
																																											showtime_back->setOpacity(255*(1-t));
																																											shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																										}, [=](float t)
																																										{
																																											showtime_back->setScale(1+11*t);
																																											showtime_back->setOpacity(255*(1-t));
																																											shadow->setOpacityOuterStroke(255 * 0.3f*(1-t));
																																											showtime_back->addChild(KSTimer::create(4.f/30.f, [=]()
																																																					{
																																																						showtimeFirstAction();
																																																					}));
																																										}));
																											  }));
																  }));
		}
		else
		{
			AudioEngine::sharedInstance()->stopSound();
			
			int take_grade = 1;
			if(target_node == first_img)
			{
				if(mySGD->getStageGrade() >= 3)
					take_grade = 3;
				else
					take_grade = 1;
			}
			else if(target_node == second_img)
			{
				if(mySGD->getStageGrade() >= 3)
					take_grade = 4;
				else
					take_grade = 2;
			}
			else
			{
				take_grade = mySGD->getStageGrade();
			}
			
			if(mySGD->isTimeEvent(kTimeEventType_card) && take_grade < 4 && !is_time_event_card_on && mySGD->getTimeEventIntValue(kTimeEventType_card) > 0)
			{
				is_time_event_card_on = true;
				int up_value = mySGD->getTimeEventIntValue(kTimeEventType_card);
				int after_value = take_grade + up_value;
				if(after_value > 4)
					after_value = 4;
				
				bool is_chance = false;
//				PieceHistory t_history = mySGD->getPieceHistory(mySD->getSilType());
				if(!mySGD->isHasGottenCards(mySD->getSilType(), after_value))// !t_history.is_clear[after_value-1].getV())
				{
					is_chance = true;
				}
				
				if(is_chance)
				{
					int t_card_number = NSDS_GI(silType, kSDS_SI_level_int1_card_i, after_value);
					
					MyNode* t_node = MyNode::create(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", t_card_number)->getCString()), t_card_number);
					
					if(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", t_card_number)->getCString()))
						t_node->loadRGB(mySIL->getDocumentPath() + CCString::createWithFormat("card%d_invisible.png", t_card_number)->getCString()); // 실루엣 z 정보 넣는 곳.
					
					if((NSDS_GI(kSDS_CI_int1_grade_i, t_card_number) == 2 || NSDS_GI(kSDS_CI_int1_grade_i, t_card_number) == 4) && NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, t_card_number))
					{
						auto t_ccb = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, t_card_number));
						CCSprite* ccb_img = t_ccb.first;
						ccb_img->setPosition(ccp(160,215));
						t_node->addChild(ccb_img);
						
						ccb_manager = t_ccb.second;
						
						t_node->touch_ccb_func = [=]()
						{
							ccb_manager->runAnimationsForSequenceNamed("touch1");
						};
						
						ccb_manager->runAnimationsForSequenceNamed("Default Timeline");
					}
					
					t_node->setPosition(ccp(160,230));
					t_node->setTouchEnabled(false);
					t_node->setVisible(false);
					game_node->addChild(t_node, -1);
					
					RankUpPopup* t_popup = RankUpPopup::create(-350, [=]()
															   {
																   if(mySGD->isHasGottenCards(mySD->getSilType(), take_grade) > 0)
																   {
																	   nextScene();
																   }
																   else
																   {
																	   mySGD->is_clear_diary = true;
																	   
																	   CCScaleTo* t_scale = CCScaleTo::create(0.3f, 1.5f);
																	   CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(240,myDSH->ui_center_y));
																	   
																	   CCSpawn* t_spawn = CCSpawn::create(t_scale, t_move, NULL);
																	   CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::nextScene));
																	   CCSequence* t_seq = CCSequence::create(t_spawn, t_call, NULL);
																	   
																	   game_node->runAction(t_seq);
																   }
															   }, [=](){
																   
//																   if(up_value == 1)
//																   {
//																	   if(take_grade == 1)
//																	   {
//																		   is_exchanged = true;
//																		   mySGD->is_exchanged = true;
//																		   mySGD->is_showtime = false;
//																	   }
//																	   else if(take_grade == 2)
//																	   {
//																		   is_exchanged = false;
//																		   mySGD->is_exchanged = false;
//																		   mySGD->setPercentage(1.f);
//																		   mySGD->is_showtime = true;
//																	   }
//																	   else if(take_grade == 3)
//																	   {
//																		   is_exchanged = true;
//																		   mySGD->is_exchanged = true;
//																		   mySGD->is_showtime = false;
//																	   }
//																   }
//																   else if(up_value == 2)
//																   {
//																	   if(take_grade == 1)
//																	   {
//																		   mySGD->setPercentage(1.f);
//																		   mySGD->is_showtime = true;
//																	   }
//																	   else if(take_grade >= 2)
//																	   {
//																		   is_exchanged = true;
//																		   mySGD->is_exchanged = true;
//																		   mySGD->setPercentage(1.f);
//																		   mySGD->is_showtime = true;
//																	   }
//																   }
//																   else if(up_value >= 3)
//																   {
//																	   is_exchanged = true;
//																	   mySGD->is_exchanged = true;
//																	   mySGD->setPercentage(1.f);
//																	   mySGD->is_showtime = true;
//																   }
																   
																   mySGD->setStageGrade(after_value);
                                                                   is_morphing = (after_value == 2 || after_value == 4);
																   
																   target_node->removeFromParent();
																   
//																   int card_number = NSDS_GI(silType, kSDS_SI_level_int1_card_i, mySGD->getStageGrade());
//																   
																   target_node = t_node;//MyNode::create(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", card_number)->getCString()));
																   t_node->setVisible(true);
//																   
//																   if(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()))
//																	   target_node->loadRGB(mySIL->getDocumentPath() + CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()); // 실루엣 z 정보 넣는 곳.
//																   
//																   
//																   target_node->setPosition(ccp(160,215));
//																   target_node->setTouchEnabled(false);
//																   game_node->addChild(target_node, kZS_Z_second_img);
																   game_node->reorderChild(target_node, kZS_Z_second_img);
																   
                                                                   if(mySGD->is_safety_mode)
                                                                   {
                                                                       if(safety_img)
                                                                       {
                                                                           safety_img->removeFromParent();
                                                                           safety_img = NULL;
                                                                       }
                                                                       
                                                                       safety_img = EffectSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", t_card_number)->getCString()));
                                                                       safety_img->setSilhouetteConvert(0);
                                                                       safety_img->setPosition(ccp(160, 230));
                                                                       game_node->addChild(safety_img, kZS_Z_second_img);
                                                                   }
                                                                   
																   game_node->setScale(0.5f);
																   game_node->setPosition(ccp(240,myDSH->ui_center_y));
															   }, [=](){
																	 CCDelayTime* delay1 = CCDelayTime::create(0.5f);
																	 CCMoveTo* move1 = CCMoveTo::create(0.5f, ccp(240,myDSH->ui_center_y));
																	 CCDelayTime* delay2 = CCDelayTime::create(0.5f);
																   
																	 CCMoveTo* move2 = CCMoveTo::create(0.5f, ccp(240,myDSH->ui_center_y));
																	 CCScaleTo* t_scale = CCScaleTo::create(0.5f, minimum_scale);
																	 CCSpawn* t_spawn = CCSpawn::create(move2, t_scale, NULL);
																   
																   //	CCMoveTo* move2 = CCMoveTo::create(1.f, ccp(0,-430*game_node->getScale()+480*screen_size.height/screen_size.width));
																   CCDelayTime* delay3 = CCDelayTime::create(0.5f);
																   CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::rankupAction));
																   
																   CCAction* t_seq = CCSequence::create(delay1, move1, delay2, t_spawn, delay3, t_call, NULL);
																   
																   game_node->runAction(t_seq);
															   }, [=](){
																   if(mySGD->isHasGottenCards(mySD->getSilType(), take_grade) > 0)
																   {
																	   nextScene();
																   }
																   else
																   {
																	   mySGD->is_clear_diary = true;
																	   
																	   CCScaleTo* t_scale = CCScaleTo::create(0.3f, 1.5f);
																	   CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(240,myDSH->ui_center_y));
																	   
																	   CCSpawn* t_spawn = CCSpawn::create(t_scale, t_move, NULL);
																	   CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::nextScene));
																	   CCSequence* t_seq = CCSequence::create(t_spawn, t_call, NULL);
																	   
																	   game_node->runAction(t_seq);
																   }
															   }, true);
					addChild(t_popup, kZS_Z_whitePaper+1);
				}
				else
				{
					if(mySGD->isHasGottenCards(mySD->getSilType(), take_grade) > 0)
					{
						nextScene();
					}
					else
					{
						mySGD->is_clear_diary = true;
						
						CCScaleTo* t_scale = CCScaleTo::create(0.3f, 1.5f);
						CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(240,myDSH->ui_center_y));
						
						CCSpawn* t_spawn = CCSpawn::create(t_scale, t_move, NULL);
						CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::nextScene));
						CCSequence* t_seq = CCSequence::create(t_spawn, t_call, NULL);
						
						game_node->runAction(t_seq);
					}
				}
				
//				if(up_value == 1)
//				{
//					if(take_grade == 1)
//					{
//						is_exchanged = true;
//						mySGD->is_exchanged = true;
//						mySGD->is_showtime = false;
//					}
//					else if(take_grade == 2)
//					{
//						is_exchanged = false;
//						mySGD->is_exchanged = false;
//						mySGD->setPercentage(1.f);
//						mySGD->is_showtime = true;
//					}
//					else if(take_grade == 3)
//					{
//						is_exchanged = true;
//						mySGD->is_exchanged = true;
//						mySGD->is_showtime = false;
//					}
//				}
//				else if(up_value == 2)
//				{
//					if(take_grade == 1)
//					{
//						mySGD->setPercentage(1.f);
//						mySGD->is_showtime = true;
//					}
//					else if(take_grade >= 2)
//					{
//						is_exchanged = true;
//						mySGD->is_exchanged = true;
//						mySGD->setPercentage(1.f);
//						mySGD->is_showtime = true;
//					}
//				}
//				else if(up_value >= 3)
//				{
//					is_exchanged = true;
//					mySGD->is_exchanged = true;
//					mySGD->setPercentage(1.f);
//					mySGD->is_showtime = true;
//				}
//				
//				mySGD->setStageGrade(after_value);
//
//				showtime_back = CCSprite::create("event_cardupgrade.png");
//				showtime_back->setScale(10.f);
//				showtime_back->setPosition(ccp(240,myDSH->ui_center_y));
//				showtime_back->setOpacity(0);
//				addChild(showtime_back, kZS_Z_showtime_back);
//				
//				white_paper = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 480, 320));
//				white_paper->setOpacity(0);
//				white_paper->setScaleY(myDSH->ui_top/320.f);
//				white_paper->setPosition(ccp(240,myDSH->ui_center_y));
//				addChild(white_paper, kZS_Z_whitePaper);
//				
//				CCDelayTime* white_paper_delay = CCDelayTime::create(46.f/60.f);
//				CCFadeTo* white_paper_fade = CCFadeTo::create(18.f/60.f, 255);
//				CCSequence* white_paper_seq = CCSequence::createWithTwoActions(white_paper_delay, white_paper_fade);
//				white_paper->runAction(white_paper_seq);
//				
//				CCScaleTo* showtime_scale1 = CCScaleTo::create(28.f/60.f, 1.f);
//				CCDelayTime* showtime_delay1 = CCDelayTime::create(18.f/60.f);
//				CCScaleTo* showtime_scale2 = CCScaleTo::create(18.f/60.f, 12.f);
//				CCSequence* showtime_seq1 = CCSequence::create(showtime_scale1, showtime_delay1, showtime_scale2, NULL);
//				
//				CCFadeTo* showtime_fade1 = CCFadeTo::create(28.f/60.f, 255);
//				CCDelayTime* showtime_delay2 = CCDelayTime::create(18.f/60.f);
//				CCFadeTo* showtime_fade2 = CCFadeTo::create(18.f/60.f, 0);
//				CCSequence* showtime_seq2 = CCSequence::create(showtime_fade1, showtime_delay2, showtime_fade2, NULL);
//				
//				CCSpawn* showtime_spawn = CCSpawn::create(showtime_seq1, showtime_seq2, NULL);
//				CCDelayTime* showtime_delay = CCDelayTime::create(8.f/60.f);
//				
//				CCCallFunc* showtime_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::showtimeFifthAction));
//				CCSequence* showtime_seq = CCSequence::create(showtime_spawn, showtime_delay, showtime_call, NULL);
//				
//				showtime_back->runAction(showtime_seq);
			}
			else if(!mySGD->is_endless_mode && !is_rankup && !mySGD->isTimeEvent(kTimeEventType_card))
			{
				is_rankup = true;
				
				bool is_rank_up_chance = false;
				PieceHistory t_history = mySGD->getPieceHistory(mySD->getSilType());
				if(t_history.try_count.getV() >= mySGD->getRankUpConditionCount() && ((take_grade == 2 && !mySGD->isHasGottenCards(mySD->getSilType(), 3)/* !t_history.is_clear[2].getV()*/) || (take_grade == 3 && !mySGD->isHasGottenCards(mySD->getSilType(), 4)/* !t_history.is_clear[3].getV()*/)))
				{
					is_rank_up_chance = true;
				}
				
				if(is_rank_up_chance)
				{
					int t_card_number = NSDS_GI(silType, kSDS_SI_level_int1_card_i, mySGD->getStageGrade()+1);
					
					MyNode* t_node = MyNode::create(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", t_card_number)->getCString()), t_card_number);
					
					if(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", t_card_number)->getCString()))
						t_node->loadRGB(mySIL->getDocumentPath() + CCString::createWithFormat("card%d_invisible.png", t_card_number)->getCString()); // 실루엣 z 정보 넣는 곳.
					
					if((NSDS_GI(kSDS_CI_int1_grade_i, t_card_number) == 2 || NSDS_GI(kSDS_CI_int1_grade_i, t_card_number) == 4) && NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, t_card_number))
					{
						auto t_ccb = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, t_card_number));
						CCSprite* ccb_img = t_ccb.first;
						ccb_img->setPosition(ccp(160,215));
						t_node->addChild(ccb_img);
						
						ccb_manager = t_ccb.second;
						
						t_node->touch_ccb_func = [=]()
						{
							ccb_manager->runAnimationsForSequenceNamed("touch1");
						};
						
						ccb_manager->runAnimationsForSequenceNamed("Default Timeline");
					}
					
					t_node->setPosition(ccp(160,230));
					t_node->setTouchEnabled(false);
					t_node->setVisible(false);
					game_node->addChild(t_node, -1);
					
					RankUpPopup* t_popup = RankUpPopup::create(-350, [=]()
															   {
																   if(mySGD->isHasGottenCards(mySD->getSilType(), take_grade) > 0)
																   {
																	   nextScene();
																   }
																   else
																   {
																	   mySGD->is_clear_diary = true;
																	   
																	   CCScaleTo* t_scale = CCScaleTo::create(0.3f, 1.5f);
																	   CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(240,myDSH->ui_center_y));
																	   
																	   CCSpawn* t_spawn = CCSpawn::create(t_scale, t_move, NULL);
																	   CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::nextScene));
																	   CCSequence* t_seq = CCSequence::create(t_spawn, t_call, NULL);
																	   
																	   game_node->runAction(t_seq);
																   }
															   }, [=](){
																   target_node->removeFromParent();
																   
//																   int card_number = NSDS_GI(silType, kSDS_SI_level_int1_card_i, mySGD->getStageGrade());
//																   
																   target_node = t_node;//MyNode::create(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", card_number)->getCString()));
//
//																   if(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()))
//																	   target_node->loadRGB(mySIL->getDocumentPath() + CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()); // 실루엣 z 정보 넣는 곳.
//
//																   
//																   target_node->setPosition(ccp(160,215));
//																   target_node->setTouchEnabled(false);
//																   game_node->addChild(target_node, kZS_Z_second_img);
																   t_node->setVisible(true);
																   game_node->reorderChild(target_node, kZS_Z_second_img);
																   
                                                                   if(mySGD->is_safety_mode)
                                                                   {
                                                                       if(safety_img)
                                                                       {
                                                                           safety_img->removeFromParent();
                                                                           safety_img = NULL;
                                                                       }
                                                                       
                                                                       safety_img = EffectSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", t_card_number)->getCString()));
                                                                       safety_img->setSilhouetteConvert(0);
                                                                       safety_img->setPosition(ccp(160, 230));
                                                                       game_node->addChild(safety_img, kZS_Z_second_img);
                                                                   }
                                                                   
																   game_node->setScale(0.5f);
																   game_node->setPosition(ccp(240,myDSH->ui_center_y));
															   }, [=](){
                                                                   
                                                                   is_morphing = (mySGD->getStageGrade() == 2 || mySGD->getStageGrade() == 4);
                                                                   
																	 CCDelayTime* delay1 = CCDelayTime::create(0.5f);
																	 CCMoveTo* move1 = CCMoveTo::create(0.5f, ccp(240,myDSH->ui_center_y));
																	 CCDelayTime* delay2 = CCDelayTime::create(0.5f);
																   
																	 CCMoveTo* move2 = CCMoveTo::create(0.5f, ccp(240,myDSH->ui_center_y));
																	 CCScaleTo* t_scale = CCScaleTo::create(0.5f, minimum_scale);
																	 CCSpawn* t_spawn = CCSpawn::create(move2, t_scale, NULL);
																   
																   //	CCMoveTo* move2 = CCMoveTo::create(1.f, ccp(0,-430*game_node->getScale()+480*screen_size.height/screen_size.width));
																   CCDelayTime* delay3 = CCDelayTime::create(0.5f);
																   CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::rankupAction));
																   
																   CCAction* t_seq = CCSequence::create(delay1, move1, delay2, t_spawn, delay3, t_call, NULL);
																	 
																   game_node->runAction(t_seq);
															   }, [=](){
																   if(mySGD->isHasGottenCards(mySD->getSilType(), take_grade) > 0)
																   {
																	   nextScene();
																   }
																   else
																   {
																	   mySGD->is_clear_diary = true;
																	   
																	   CCScaleTo* t_scale = CCScaleTo::create(0.3f, 1.5f);
																	   CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(240,myDSH->ui_center_y));
																	   
																	   CCSpawn* t_spawn = CCSpawn::create(t_scale, t_move, NULL);
																	   CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::nextScene));
																	   CCSequence* t_seq = CCSequence::create(t_spawn, t_call, NULL);
																	   
																	   game_node->runAction(t_seq);
																   }
															   });
					addChild(t_popup, kZS_Z_whitePaper+1);
				}
				else
				{
					if(mySGD->isHasGottenCards(mySD->getSilType(), take_grade) > 0)
					{
						nextScene();
					}
					else
					{
						mySGD->is_clear_diary = true;
						
						CCScaleTo* t_scale = CCScaleTo::create(0.3f, 1.5f);
						CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(240,myDSH->ui_center_y));
						
						CCSpawn* t_spawn = CCSpawn::create(t_scale, t_move, NULL);
						CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::nextScene));
						CCSequence* t_seq = CCSequence::create(t_spawn, t_call, NULL);
						
						game_node->runAction(t_seq);
					}
				}
			}
			else
			{
				if(mySGD->isHasGottenCards(mySD->getSilType(), take_grade) > 0)
				{
					nextScene();
				}
				else
				{
					mySGD->is_clear_diary = true;
					
					CCScaleTo* t_scale = CCScaleTo::create(0.3f, 1.5f);
					CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(240,myDSH->ui_center_y));
					
					CCSpawn* t_spawn = CCSpawn::create(t_scale, t_move, NULL);
					CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::nextScene));
					CCSequence* t_seq = CCSequence::create(t_spawn, t_call, NULL);
					
					game_node->runAction(t_seq);
				}
			}
		}
	}
}

void ZoomScript::nextScene()
{
//	setBackKeyEnabled(false);
	
	if(mySGD->is_endless_mode || mySGD->is_hell_mode)
	{
		CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
	}
	else
	{
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_clear);
		CCDirector::sharedDirector()->replaceScene(PuzzleScene::scene());
	}
}

void ZoomScript::showtimeFirstAction()
{
	showtime_back->removeFromParent();
	
	script_label->setString("");
	script_case->setVisible(false);
	script_label->setVisible(false);
	
	int card_number;
	
	if(mySGD->getStageGrade() >= 3)		card_number = NSDS_GI(silType, kSDS_SI_level_int1_card_i, 4);
	else								card_number = NSDS_GI(silType, kSDS_SI_level_int1_card_i, 2);
	
	second_img = MyNode::create(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", card_number)->getCString()), card_number);

	if(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()))
		second_img->loadRGB(mySIL->getDocumentPath() + CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()); // 실루엣 z 정보 넣는 곳.

	if((NSDS_GI(kSDS_CI_int1_grade_i, card_number) == 2 || NSDS_GI(kSDS_CI_int1_grade_i, card_number) == 4) && NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, card_number))
	{
		auto t_ccb = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, card_number));
		CCSprite* ccb_img = t_ccb.first;
		ccb_img->setPosition(ccp(160,215));
		second_img->addChild(ccb_img);
		
		ccb_manager = t_ccb.second;
		second_img->touch_ccb_func = [=]()
		{
			ccb_manager->runAnimationsForSequenceNamed("touch1");
		};
		
		ccb_manager->runAnimationsForSequenceNamed("Default Timeline");
	}
	
	second_img->setPosition(ccp(160,230));
	second_img->setTouchEnabled(false);
	game_node->addChild(second_img, kZS_Z_second_img);
	
	if(mySGD->is_safety_mode)
	{
        if(safety_img)
        {
            safety_img->removeFromParent();
            safety_img = NULL;
        }
		
		safety_img = EffectSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()));
		safety_img->setSilhouetteConvert(0);
		safety_img->setPosition(ccp(160, 230));
		game_node->addChild(safety_img, kZS_Z_second_img);
	}
	
	target_node = second_img;
	
	game_node->setRotation(0);
	card_back->setRotation(0);
	
	game_node->setScale(1.f);
	game_node->setAnchorPoint(ccp(0.5,0.5));
	game_node->setContentSize(CCSizeMake(320,460));
	//game_node->setPosition(ccp(0,-430*game_node->getScale()+480*screen_size.height/screen_size.width));
	first_img->removeFromParentAndCleanup(true);
	first_img = NULL;
	
	int third_card_number = card_number;
	
	if(NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, third_card_number))
	{
		eye_ani_size = CCSizeMake(NSDS_GI(kSDS_CI_int1_aniInfoDetailCutWidth_i, third_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailCutHeight_i, third_card_number));
		loop_length = NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopLength_i, third_card_number);
		
		for(int i=0;i<loop_length;i++)
			animation_frame.push_back(NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, third_card_number, i));
		
		CCTexture2D* eye_texture = mySIL->addImage(CCString::createWithFormat("card%d_animation.png", third_card_number)->getCString());
		
		CCSprite* eye = CCSprite::createWithTexture(eye_texture, CCRectMake(0, 0, eye_ani_size.width, eye_ani_size.height));
		eye->setPosition(ccp(NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionX_i, third_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionY_i, third_card_number)));
		second_img->addChild(eye, 1, 1);
	}
	
//	CCFadeTo* t_fade = CCFadeTo::create(1.f/6.f, 0);
//	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::showtimeSecondAction));
//	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
//	
//	white_paper->runAction(t_seq);
	
	showtimeSecondAction();
}

void ZoomScript::showtimeFifthAction()
{
	showtime_back->removeFromParent();
	
	script_label->setString("");
	script_case->setVisible(false);
	script_label->setVisible(false);
	
	int card_number;
	
	card_number = NSDS_GI(silType, kSDS_SI_level_int1_card_i, mySGD->getStageGrade());
	
	third_img = MyNode::create(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", card_number)->getCString()), card_number);
	
	if(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()))
		third_img->loadRGB(mySIL->getDocumentPath() + CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()); // 실루엣 z 정보 넣는 곳.
	
	if((NSDS_GI(kSDS_CI_int1_grade_i, card_number) == 2 || NSDS_GI(kSDS_CI_int1_grade_i, card_number) == 4) && NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, card_number))
	{
		auto t_ccb = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, card_number));
		CCSprite* ccb_img = t_ccb.first;
		ccb_img->setPosition(ccp(160,215));
		third_img->addChild(ccb_img);
		
		ccb_manager = t_ccb.second;
		third_img->touch_ccb_func = [=]()
		{
			ccb_manager->runAnimationsForSequenceNamed("touch1");
		};
		
		ccb_manager->runAnimationsForSequenceNamed("Default Timeline");
	}
	
	third_img->setPosition(ccp(160,215));
	third_img->setTouchEnabled(false);
	game_node->addChild(third_img, kZS_Z_second_img);
	
	if(mySGD->is_safety_mode)
	{
        if(safety_img)
        {
            safety_img->removeFromParent();
            safety_img = NULL;
        }
		
		safety_img = EffectSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()));
		safety_img->setSilhouetteConvert(0);
		safety_img->setPosition(ccp(160, 215));
		game_node->addChild(safety_img, kZS_Z_second_img);
	}
	
	target_node = third_img;
	
	game_node->setScale(1.5f);
	game_node->setPosition(ccp(240,myDSH->ui_center_y));//ccp(0,-430*game_node->getScale()+480*screen_size.height/screen_size.width));
	
	game_node->setRotation(0);
	card_back->setRotation(0);
	
	if(second_img)
		second_img->removeFromParent();
	else
		first_img->removeFromParentAndCleanup(true);
	
//	int third_card_number = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 3);
//	
//	if(is_exchanged && NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, third_card_number))
//	{
//		eye_ani_size = CCSizeMake(NSDS_GI(kSDS_CI_int1_aniInfoDetailCutWidth_i, third_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailCutHeight_i, third_card_number));
//		loop_length = NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopLength_i, third_card_number);
//		
//		for(int i=0;i<loop_length;i++)
//			animation_frame.push_back(NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, third_card_number, i));
//		
//		CCTexture2D* eye_texture = mySIL->addImage(CCString::createWithFormat("card%d_animation.png", third_card_number)->getCString());
//		
//		CCSprite* eye = CCSprite::createWithTexture(eye_texture, CCRectMake(0, 0, eye_ani_size.width, eye_ani_size.height));
//		eye->setPosition(ccp(NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionX_i, third_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionY_i, third_card_number)));
//		second_img->addChild(eye, 1, 1);
//	}
//	
	CCFadeTo* t_fade = CCFadeTo::create(1.f/6.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::showtimeSixthAction));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	
	white_paper->runAction(t_seq);
}

void ZoomScript::showtimeSecondAction()
{
	white_paper->runAction(CCFadeOut::create(0.5f));
	
	CCDelayTime* delay1 = CCDelayTime::create(0.5f);
	CCMoveTo* move1 = CCMoveTo::create(0.5f, ccp(240,myDSH->ui_center_y));
	CCDelayTime* delay2 = CCDelayTime::create(0.5f);
	
	CCMoveTo* move2 = CCMoveTo::create(0.5f, ccp(240,myDSH->ui_center_y));
	CCScaleTo* t_scale = CCScaleTo::create(0.5f, minimum_scale);
	CCSpawn* t_spawn = CCSpawn::create(move2, t_scale, NULL);
	
//	CCMoveTo* move2 = CCMoveTo::create(1.f, ccp(0,-430*game_node->getScale()+480*screen_size.height/screen_size.width));
	CCDelayTime* delay3 = CCDelayTime::create(0.5f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::showtimeThirdAction));
	
	CCAction* t_seq = CCSequence::create(delay1, move1, delay2, t_spawn, delay3, t_call, NULL);
	
	game_node->runAction(t_seq);
}

void ZoomScript::showtimeSixthAction()
{
	white_paper->removeFromParent();
	
	CCDelayTime* delay1 = CCDelayTime::create(0.5f);
	CCMoveTo* move1 = CCMoveTo::create(1.f, ccp(0,0));
	CCDelayTime* delay2 = CCDelayTime::create(1.f);
	
	CCMoveTo* move2 = CCMoveTo::create(0.7f, ccp((480.f-320.f*minimum_scale)/2.f, 0));
	CCScaleTo* t_scale = CCScaleTo::create(0.7f, minimum_scale);
	CCSpawn* t_spawn = CCSpawn::create(move2, t_scale, NULL);
	
	//	CCMoveTo* move2 = CCMoveTo::create(1.f, ccp(0,-430*game_node->getScale()+480*screen_size.height/screen_size.width));
	CCDelayTime* delay3 = CCDelayTime::create(1.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::showtimeSeventhAction));
	
	CCAction* t_seq = CCSequence::create(delay1, move1, delay2, t_spawn, delay3, t_call, NULL);
	
	game_node->runAction(t_seq);
}

void ZoomScript::rankupAction()
{
	save_text = NSDS_GS(kSDS_CI_int1_script_s, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, mySGD->getStageGrade()));
	
	CCLabelTTF* t_label = CCLabelTTF::create(save_text.c_str(), mySGD->getFont().c_str(), 16);
	
	script_label->setDimensions(CCSizeMake(330, t_label->getContentSize().height*(ceil(t_label->getContentSize().width/330.f))));
	script_case->setContentSize(CCSizeMake(353, t_label->getContentSize().height*(ceil(t_label->getContentSize().width/330.f)) + 15));
	
	script_label->setString("");
	script_label->setVisible(true);
	script_case->setVisible(true);
	showtime_morphing_label->setPosition(ccp(0,script_case->getContentSize().height+1));
	
	if(is_morphing && (NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 2 || NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 4))showtime_morphing_label->setVisible(true);
	
	basic_string<wchar_t> result;
	utf8::utf8to16(save_text.begin(), save_text.end(), back_inserter(result));
	text_length = result.length();
	typing_frame = 0;
	delegate_typing_after = callfunc_selector(ZoomScript::showtimeForthAction);
	schedule(schedule_selector(ZoomScript::typingAnimation), 1.f/10.f);
}

void ZoomScript::showtimeThirdAction()
{
	white_paper->removeFromParent();

	
	if(NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 4)))
	{
		startStageAnimation();
	}
	
	save_text = NSDS_GS(kSDS_CI_int1_script_s, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, mySGD->getStageGrade()));
	CCLabelTTF* t_label = CCLabelTTF::create(save_text.c_str(), mySGD->getFont().c_str(), 16);
	
	script_label->setDimensions(CCSizeMake(330, t_label->getContentSize().height*(ceil(t_label->getContentSize().width/330.f))));
	script_case->setContentSize(CCSizeMake(353, t_label->getContentSize().height*(ceil(t_label->getContentSize().width/330.f)) + 15));
	
	script_label->setString("");
	script_case->setVisible(true);
	script_label->setVisible(true);
	showtime_morphing_label->setVisible(true);
	showtime_morphing_label->setPosition(ccp(0,script_case->getContentSize().height+1));
	
	basic_string<wchar_t> result;
	utf8::utf8to16(save_text.begin(), save_text.end(), back_inserter(result));
	text_length = result.length();
	typing_frame = 0;
	delegate_typing_after = callfunc_selector(ZoomScript::showtimeForthAction);
	schedule(schedule_selector(ZoomScript::typingAnimation), 1.f/10.f);
}

void ZoomScript::showtimeSeventhAction()
{
//	if(is_exchanged && NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 3)))
//	{
//		startStageAnimation();
//	}
	save_text = NSDS_GS(kSDS_CI_int1_script_s, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, mySGD->getStageGrade()));
	
	CCLabelTTF* t_label = CCLabelTTF::create(save_text.c_str(), mySGD->getFont().c_str(), 16);
	
	script_label->setDimensions(CCSizeMake(330, t_label->getContentSize().height*(ceil(t_label->getContentSize().width/330.f))));
	script_case->setContentSize(CCSizeMake(353, t_label->getContentSize().height*(ceil(t_label->getContentSize().width/330.f)) + 15));
	
	script_label->setString("");
	script_label->setVisible(true);
	script_case->setVisible(true);
	showtime_morphing_label->setPosition(ccp(0,script_case->getContentSize().height+1));
	
	if(is_morphing && (NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 2 || NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 4))showtime_morphing_label->setVisible(true);
	
	basic_string<wchar_t> result;
	utf8::utf8to16(save_text.begin(), save_text.end(), back_inserter(result));
	text_length = result.length();
	typing_frame = 0;
	delegate_typing_after = callfunc_selector(ZoomScript::showtimeForthAction);
	schedule(schedule_selector(ZoomScript::typingAnimation), 1.f/10.f);
}

void ZoomScript::showtimeForthAction()
{
	is_actioned = false;
	is_showtime = false;
//	is_touched_menu = false;
	is_next_on = true;
	next_button->setVisible(is_next_on && is_ccb_end);
	script_case->setVisible(true);
	script_label->setVisible(true);
//	mode_button->setVisible(true);
	setTouchEnabled(true);
	//schedule(schedule_selector(ZoomScript::moveChecking));
}

void ZoomScript::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	
	pDispatcher->addStandardDelegate(this, -150);
}

void ZoomScript::moveListXY(CCPoint t_p)
{
	script_label->setVisible(false);
	script_case->setVisible(false);
	
	if(t_p.x > ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.x = ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	if(t_p.x < -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.x = -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	
	if(t_p.y > ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.y = ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	if(t_p.y < -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.y = -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
//	
//	CCPoint a_p = ccpSub(game_node->getPosition(), t_p);
//	
//	if(game_node->getScale() <= 1.5f)
//	{
//		if(a_p.x > (480.f-320.f*game_node->getScale())/2.f+40.f)
//			a_p.x = (480.f-320.f*game_node->getScale())/2.f+40.f;
//		else if(a_p.x < (480.f-320.f*game_node->getScale())/2.f-40.f)
//			a_p.x = (480.f-320.f*game_node->getScale())/2.f-40.f;
//	}
//	else
//	{
//		if(a_p.x > 40.f)
//			a_p.x = 40.f;
//		else if(a_p.x < 480-320*game_node->getScale()-40.f)
//			a_p.x = 480-320*game_node->getScale()-40.f;
//	}
//	
//	if(a_p.y > 0+40.f)
//		a_p.y = 0+40.f;
//	if(a_p.y < -430*game_node->getScale()+480*screen_size.height/screen_size.width-40.f)
//		a_p.y = -430*game_node->getScale()+480*screen_size.height/screen_size.width-40.f;
//	
//	game_node->setPosition(a_p);


	// 120, 0  ->   +120 + 160
	CCPoint a_p = ccpSub(game_node->getPosition(), t_p);
	
	
	//size가 860 * 320
	int widthLimit = 320 * game_node->getScale();
	int heightLimit = (430-screen_size.height/2.f) * game_node->getScale();
	
	int checkWidth=0;
	
	float height = myDSH->ui_center_y*2;
	float contentHalfWidth = game_node->getScale()*game_node->getContentSize().width/2.f;
	float contentHalfHeight = game_node->getScale()*game_node->getContentSize().height/2.f;
	
	if(contentHalfWidth*2.f<=480){
		
		a_p.x=240;
		
	}else{
		
		
		if(a_p.x<480-contentHalfWidth){
			a_p.x = 480-contentHalfWidth;
		}
		
		if(contentHalfWidth<a_p.x){
			a_p.x = contentHalfWidth;
		}
		
	}
	
	
	if(contentHalfHeight*2>=height){
		//top & bottom
		if(a_p.y < height - contentHalfHeight){
			a_p.y=height - contentHalfHeight;
		}else if(a_p.y>contentHalfHeight){
			a_p.y=contentHalfHeight;
		}
	}else{
		a_p.y = myDSH->ui_center_y;
	}
	

	
	
	
	game_node->setPosition(a_p);
}

void ZoomScript::moveAnimation()
{
	isAnimated = true;
	
	if(moveSpeed_p.x >= ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())			moveSpeed_p.x -= ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else if(moveSpeed_p.x <= -ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())	moveSpeed_p.x += ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else							moveSpeed_p.x = 0;
	
	if(moveSpeed_p.y >= ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())			moveSpeed_p.y -= ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else if(moveSpeed_p.y <= ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())	moveSpeed_p.y += ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else							moveSpeed_p.y = 0;
	
	if((moveSpeed_p.x < ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale() && moveSpeed_p.x > -ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale() &&
		moveSpeed_p.y < ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale() && moveSpeed_p.y > -ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale()) || isAnimated == false)
	{
		this->unschedule(schedule_selector(ZoomScript::moveAnimation));
		moveSpeed_p = CCPointZero;
		isAnimated = false;
	}
	this->moveListXY(ccpMult(moveSpeed_p, -1));
}

void ZoomScript::frameAnimation()
{
	int loop_point = animation_frame[ing_animation_frame];
	((CCSprite*)second_img->getChildByTag(1))->setTextureRect(CCRectMake(loop_point*eye_ani_size.width, 0, eye_ani_size.width, eye_ani_size.height));

	ing_animation_frame++;
	if(ing_animation_frame >= loop_length)
		startStageAnimation();
	else
	{
		CCDelayTime* t_delay = CCDelayTime::create(0.1f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::frameAnimation));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		second_img->getChildByTag(1)->runAction(t_seq);
	}
}

void ZoomScript::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	CCSetIterator iter;
	CCTouch *touch;
	
	touch_mode=0;
	for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));

		multiTouchData[(int)touch] = location;

		touch_p = location;

		timeval time;
		gettimeofday(&time, NULL);
		touchStartTime = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
		touchStart_p = location;

		this->unschedule(schedule_selector(ZoomScript::moveAnimation));

		isAnimated=false;

		if(multiTouchData.size() >= 1)
		{
			target_node->setTouchEnabled(false);
		}
		
		if(multiTouchData.size() == 1)
		{
			first_touch_time = touchStartTime;
			first_touch_p = int(touch);
			first_touch_point = location;
			is_scrolling = true;
			if(NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 2 || NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 4)
				target_node->ccTouchBegan(touch,pEvent);
//			if(!is_touched_menu && next_button->ccTouchBegan(touch, pEvent))
//			{
//				is_touched_menu = true;
//			}
		}
		else if(multiTouchData.size() == 2)
		{
			is_scrolling = false;
			CCPoint sub_point = CCPointZero;
			map<int, CCPoint>::iterator it;
			for(it = multiTouchData.begin();it!=multiTouchData.end();it++)
			{
				sub_point = ccpMult(sub_point, -1);
				sub_point = ccpAdd(sub_point, it->second);
			}

			zoom_base_distance = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
			
			
			//회전관련하여 초기값 저장하기
			zoom_base_radian = atanf((float)sub_point.y/(float)sub_point.x);
			
			if(sub_point.x<0) zoom_base_radian+=CC_DEGREES_TO_RADIANS(90);
			else zoom_base_radian-=CC_DEGREES_TO_RADIANS(90);
			
			zoom_base_radian+=CC_DEGREES_TO_RADIANS(game_node->getRotation());

			
		}
		else
		{
			is_scrolling = false;
//			if(is_touched_menu)
//			{
//				next_button->ccTouchCancelled(touch, pEvent);
//				is_touched_menu = false;
//			}
		}
	}
}

void ZoomScript::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
	CCSetIterator iter;
	CCTouch* touch;

	for(iter = pTouches->begin();iter != pTouches->end();++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));

		map<int, CCPoint>::iterator o_it;
		o_it = multiTouchData.find((int)touch);
		if(o_it != multiTouchData.end())
		{
			o_it->second = location;
			
			if((int)touch == first_touch_p)
			{
				if(first_touch_point.getDistanceSq(location) > 100.f)
				{
					first_touch_time = -200000;
				}
			}
			
			if(multiTouchData.size() == 1 && is_morphing && target_node->m_waveRange==1)
			{
				
				touch_p = location;
				if(is_morphing && (NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 2 || NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 4))target_node->ccTouchMoved(touch,pEvent);
			}else if(multiTouchData.size() == 1){
				
				this->moveListXY(ccpSub(touch_p, location));
				touch_p = location;
				
			}else if(multiTouchData.size() == 2)
			{
				touch_mode=2;
				CCPoint sub_point = CCPointZero;
				CCPoint avg_point = CCPointZero;
				map<int, CCPoint>::iterator it;
				for(it = multiTouchData.begin();it!=multiTouchData.end();it++)
				{
					sub_point = ccpMult(sub_point, -1);
					sub_point = ccpAdd(sub_point, it->second);
					
					avg_point = ccpAdd(avg_point, it->second);
				}
				avg_point = ccpMult(avg_point,1/(float)multiTouchData.size());
				
						
				//회전~
				float last_radian = atanf((float)sub_point.y/(float)sub_point.x);
				
				if(sub_point.x<0) last_radian+=CC_DEGREES_TO_RADIANS(90);
				else last_radian-=CC_DEGREES_TO_RADIANS(90);
				float degree =CC_RADIANS_TO_DEGREES(zoom_base_radian-last_radian)+720;
				int degreeInt = abs((int)degree%360);
				
				//자석처럼 붙이자
				if(degreeInt>355 || degreeInt<5)degree=0;
				if(degreeInt<95 && degreeInt>85)degree=90;
				if(degreeInt<185 && degreeInt>175)degree=180;
				if(degreeInt<275 && degreeInt>265)degree=270;
				
				game_node->setRotation(degree);
				card_back->setRotation(degree);
				
				
				
				script_label->setVisible(false);
				script_case->setVisible(false);

				float before_scale = game_node->getScale();

				float changed_distance = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
				float after_scale = game_node->getScale()*changed_distance/zoom_base_distance;
				if(after_scale > 2.5f)			after_scale = 2.5f;
				else if(after_scale < minimum_scale)		after_scale = minimum_scale;
				zoom_base_distance = changed_distance;
				game_node->setScale(after_scale);
				
				this->moveListXY(ccpSub(old_center_pos, avg_point));
				
				old_center_pos = avg_point;

			}
		}
	}
}

void ZoomScript::startStageAnimation()
{
	ing_animation_frame = 0;

	int random_value = rand()%16;

	if(random_value >= 2 && random_value <= 4)
		random_value = 7;
	else if(random_value >= 5 && random_value <= 10)
		random_value = 10;
	else if(random_value >= 11 && random_value <= 13)
		random_value = 13;
	else if(random_value == 14 || random_value == 15)
		random_value = 16;
	else
		random_value++;

	CCDelayTime* t_delay = CCDelayTime::create(random_value*0.1f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::frameAnimation));
	CCAction* t_seq = CCSequence::create(t_delay, t_call, NULL);
	second_img->getChildByTag(1)->runAction(t_seq);
}

void ZoomScript::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
	CCSetIterator iter;
	CCTouch* touch;

	for(iter = pTouches->begin();iter != pTouches->end();++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));

		map<int, CCPoint>::iterator o_it;
		o_it = multiTouchData.find((int)touch);
		if(o_it != multiTouchData.end())
		{
			multiTouchData.erase((int)touch);

			if(multiTouchData.size() == 1)
			{
				is_scrolling = true;
			}
			else
			{
				is_scrolling = false;
			}
			
			if(multiTouchData.size() == 0)
			{
//				target_node->setTouchEnabled(true);
				
//				if(is_touched_menu)
//				{
//					next_button->ccTouchEnded(touch, pEvent);
//					is_touched_menu = false;
//				}

				timeval time;
				gettimeofday(&time, NULL);
				
				if((int)touch == first_touch_p && (((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - first_touch_time) < 200000)
				{
//					if(NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) >= 3)
//						target_node->ccTouchEnded(touch, pEvent);
					if(is_morphing && touch_mode!=2 && (NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 2 || NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 4))
						target_node->ccTouchEnded(touch, pEvent);
				}
				else
				{
					if(is_morphing && touch_mode!=2 && (NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 2 || NSDS_GI(kSDS_CI_int1_grade_i, target_node->card_number) == 4))
						target_node->morphing(touch, pEvent);
					return;

					unsigned long long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
					CCPoint _spd = ccpMult(ccpSub(location, touchStart_p), 1.f/_time*10000);
					
					float spd_value = sqrtf(powf(_spd.x, 2.f) + powf(_spd.y, 2.f));
					if(isAnimated == false && fabsf(spd_value) > 2 && !is_spin_mode)
					{
						moveSpeed_p = _spd;
						this->schedule(schedule_selector(ZoomScript::moveAnimation));
					}
				}
			}
		}
	}
}

void ZoomScript::ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
{
	ccTouchesEnded(pTouches, pEvent);
}

void ZoomScript::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void ZoomScript::keyBackClicked()
{
	
	
	CommonButton::callBackKey();
	
//	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(ZoomScript::alertAction));
//	if(isBackKeyEnabled() && next_button->isEnabled() && next_button->isVisible())
//	{
//		onBackKeyAction();
//		
//		setBackKeyFunc([=](){
//			CCNode* t_node = CCNode::create();
//			menuAction(t_node);
//		});
//		setBackKeyEnabled(true);
//	}
}