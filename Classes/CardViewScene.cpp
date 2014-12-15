//
//  CardViewScene.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 4..
//
//

#include "CardViewScene.h"
#include "DataStorageHub.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"
#include "StageImgLoader.h"
#include "utf8.h"
#include "CommonButton.h"
#include "bustMorphing.h"
#include "RankUpPopup.h"
#include "GraySprite.h"
#include "CCMenuLambda.h"
#include "BuyMorphingPopup.h"
#include "FormSetter.h"
#include "LoadingLayer.h"
#include "Diary19Popup.h"

#define CV_SCROLL_SPEED_MAX_BASE	20
#define CV_SCROLL_SPEED_DECEASE_BASE	0.2f

CCScene* CardViewScene::scene(function<void()> t_end_func)
{
    CCScene *scene = CCScene::create();
    CardViewScene *layer = CardViewScene::create();
    scene->addChild(layer);
	layer->temp_end_func = t_end_func;
	
    return scene;
}

enum CV_Zorder{
	kCV_Z_back = 0,
	kCV_Z_first_img,
	kCV_Z_next_button
};

void CardViewScene::completedAnimationSequenceNamed(const char *name)
{
	CCTouch* t_touch = new CCTouch();
	t_touch->setTouchInfo(0, 240, myDSH->ui_center_y);
	t_touch->autorelease();
	
//	first_img->ccTouchEnded(t_touch, NULL);
	
	t_manager->setDelegate(NULL);
}

bool CardViewScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	buy_morphing = nullptr;
	t_ccb_img = nullptr;
	startFormSetter(this);
	
	
//	CCLayer* top_bottom_layer = CCLayer::create();
//	top_bottom_layer->setPosition(ccp(0, 0));
//	addChild(top_bottom_layer, kCV_Z_back);
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
	addChild(back_img, kCV_Z_back);
	
//	CCSprite* temp = CCSprite::create("whitePaper.png");
//	temp->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
//	temp->setPosition(ccp(240,myDSH->ui_center_y));
//	addChild(temp);
	
	card_back = KS::loadCCBI<CCSprite*>(this, "zoom_back.ccbi").first;
	card_back->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(card_back, kCV_Z_back);
	
//	CCSprite* title_name = CCSprite::create("temp_title_name.png");
//	title_name->setPosition(ccp(240,myDSH->ui_center_y));
//	addChild(title_name, kCV_Z_back);
	
	game_node = CCNode::create();
	game_node->setScale(1.5f);
	game_node->setAnchorPoint(ccp(0.5,0.5));
	game_node->setContentSize(CCSizeMake(320,460));
	game_node->setPosition(ccp(240,myDSH->ui_center_y));
	setFormSetter(game_node);
	addChild(game_node, kCV_Z_first_img);
	
	sound_img = NULL;
	
	int card_number = mySGD->selected_collectionbook;
	
	int sound_count = NSDS_GI(kSDS_CI_int1_soundCnt_i, card_number);
	for(int i=1;i<=sound_count;i++)
	{
		AudioEngine::sharedInstance()->preloadGroanEffect(NSDS_GS(kSDS_CI_int1_soundType_int1_s, card_number, i));
	}
	
	is_morphing = mySGD->isCardMorphing(card_number);
	
	CCLog("why two2");
	first_img = MyNode::create(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", card_number)->getCString()), card_number);
	first_img->setAnchorPoint(ccp(0.5,0.5));
	if(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()))
		first_img->loadRGB(mySIL->getDocumentPath() + CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()); // 실루엣 z 정보 넣는 곳.

	
	first_img->setPosition(ccp(160,230));
	first_img->setAnchorPoint(ccp(0.5,0.5));
	setFormSetter(first_img);
	first_img->setTouchEnabled(false);
	game_node->addChild(first_img, kCV_Z_first_img);
	
	ccb_manager = NULL;
	
	if(/*is_morphing && */NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, card_number))
	{
		auto t_ccb = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, card_number));
		t_ccb_img = t_ccb.first;
		t_ccb_img->setPosition(ccp(160, 215));
		first_img->addChild(t_ccb_img);
		
		ccb_manager = t_ccb.second;
		ccb_manager->runAnimationsForSequenceNamed("Default Timeline");
		
		first_img->touch_ccb_func = [=]()
		{
			ccb_manager->runAnimationsForSequenceNamed("touch1");
		};
	}
	
//	if(mySGD->is_safety_mode)
//	{
//		safety_img = EffectSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()));
//		safety_img->setSilhouetteConvert(0);
//		safety_img->setPosition(ccp(160, 230));
//		game_node->addChild(safety_img, kCV_Z_first_img);
//	}
	
	CCPoint center_position = ccp(160,230);
	
	CCSprite* top_case = CCSprite::create("diary_frame_top.png");
	top_case->setPosition(ccpAdd(center_position, ccp(0,215)));
	game_node->addChild(top_case, kCV_Z_first_img);
	
	CCSprite* bottom_case = CCSprite::create("diary_frame_bottom.png");
	bottom_case->setPosition(ccpAdd(center_position, ccp(0,-215)));
	game_node->addChild(bottom_case, kCV_Z_first_img);
	
	CCSprite* left_case = CCSprite::create("diary_frame_left.png");
	left_case->setPosition(ccpAdd(center_position, ccp(-160,0)));
	game_node->addChild(left_case, kCV_Z_first_img);
	
	CCSprite* right_case = CCSprite::create("diary_frame_right.png");
	right_case->setPosition(ccpAdd(center_position, ccp(160,0)));
	game_node->addChild(right_case, kCV_Z_first_img);
	
	
//	zoom_img = CCSprite::create("ending_expand.png");
	
	CCPoint morphing_position = ccp(447,36.5f);
	
	function<void()> refresh_morphing_sound = [=]()
	{
		if(sound_img)
		{
			sound_img->removeFromParent();
			sound_img = NULL;
		}
		
		if(sound_count > 0)
		{
			CCPoint sound_position;
			if(is_morphing)
				sound_position = morphing_position;
			else
				sound_position = morphing_position + ccp(0,49.5f);
				
			if(myDSH->getBoolForKey(kDSH_Key_isOffCardMorphingSound))
			{
				sound_img = CCSprite::create("diary_voice_on.png");
				sound_img->setPosition(sound_position);
				addChild(sound_img, kCV_Z_next_button);
			}
			else
			{
				sound_img = CCSprite::create("diary_voice_off.png");
				sound_img->setPosition(sound_position);
				addChild(sound_img, kCV_Z_next_button);
			}
		}
	};
	
    t_manager = NULL;
    morphing_img = NULL;
    
	string morphing_filename = "";
	auto liveGirl = [=](){
		is_actioned = false;
		is_morphing = true;
		
		if(NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, card_number))
		{
			auto t_ccb = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, card_number));
			t_ccb_img = t_ccb.first;
			t_ccb_img->setPosition(ccp(160, 215));
			first_img->addChild(t_ccb_img);
			
			ccb_manager = t_ccb.second;
			
			first_img->touch_ccb_func = [=]()
			{
				ccb_manager->runAnimationsForSequenceNamed("touch1");
			};
			
			ccb_manager->runAnimationsForSequenceNamed("Default Timeline");
		}
		
		refresh_morphing_sound();
		
		CCPoint sound_position;
		if(is_morphing)
			sound_position = morphing_position;
		else
			sound_position = morphing_position + ccp(0,49.5f);
		
		CCSprite* n_sound = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 50, 50));
		CCSprite* s_sound = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 50, 50));
		CCMenuItemLambda* sound_item = CCMenuItemSpriteLambda::create(n_sound, s_sound, [=](CCObject* sender)
																	  {
																		  if(!is_actioned)
																			{
																				is_actioned = true;
																				AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																				myDSH->setBoolForKey(kDSH_Key_isOffCardMorphingSound, !myDSH->getBoolForKey(kDSH_Key_isOffCardMorphingSound));
																				refresh_morphing_sound();
																				is_actioned = false;
																			}
																	  });
		CCMenuLambda* sound_menu = CCMenuLambda::createWithItem(sound_item);
        sound_menu->setEnabled(false);
		sound_menu->setPosition(sound_position);
		addChild(sound_menu, kCV_Z_next_button);
		
		
		
		
		
		
		
		// 퍼즐씬 -> 카드자세히보기 에서 자동으로 생명의돌팝업뜨길래 쓴다고 눌렀는데 팝업창 닫히면서 죽음
		
		
		
		
		
		
		
		
		if(buy_morphing)
			buy_morphing->removeFromParent();
		if(morphing_img)
            morphing_img->removeFromParent();
//		morphing_img = KS::loadCCBI<CCSprite*>(this, "morphing_heart_on.ccbi").first;
//		morphing_img->setPosition(morphing_position);
//		addChild(morphing_img, kCV_Z_next_button);
		
		
//		auto tuto = KS::loadCCBI<CCSprite*>(this, "tutorial_touch.ccbi");
//		
//		zoom_img = tuto.first;
//		tuto.second->setDelegate(this);
//		t_manager = tuto.second;
//		tuto.second->runAnimationsForSequenceNamed("Default Timeline");
//		
//		zoom_img->setPosition(ccp(240, myDSH->ui_center_y));
//		addChild(zoom_img, kCV_Z_next_button);
	};
    
    auto tuto = KS::loadCCBI<CCSprite*>(this, "tutorial_touch_rotation.ccbi");
    
    zoom_img = tuto.first;
    tuto.second->runAnimationsForSequenceNamed("Default Timeline");
    
    
    zoom_img->setPosition(ccp(240, myDSH->ui_center_y));
    addChild(zoom_img, kCV_Z_next_button);
    
	if(!is_morphing)
	{
//		buy_morphing = CommonButton::create("", 10, CCSizeMake(63, 63), CCScale9Sprite::create("whitepaper2.png", CCRectMake(0, 0, 63, 63), CCRectMake(31, 31, 1, 1)), -160);
//		buy_morphing->setPosition(morphing_position);
//		buy_morphing->setFunction([=](CCObject* sender)
//								  {
//									  if(!is_actioned)
//									  {
//										  is_actioned = true;
//										  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
//										  
//										  CCLOG("11111111");
//										  
//										  BuyMorphingPopup* t_popup = BuyMorphingPopup::create(-200, [=](){
//											  is_actioned = false;
//										  }, liveGirl);
//										  addChild(t_popup, 999);
//									  }
//								  });
//		addChild(buy_morphing, kCV_Z_next_button);
		
//		morphing_filename = "morphing_heart_off.ccbi";
	}
	else
	{
//		morphing_filename = "morphing_heart_on.ccbi";
		
		refresh_morphing_sound();
		
		CCPoint sound_position;
		if(is_morphing)
			sound_position = morphing_position;
		else
			sound_position = morphing_position + ccp(0,49.5f);
		
		CCSprite* n_sound = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 50, 50));
		CCSprite* s_sound = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 50, 50));
		CCMenuItemLambda* sound_item = CCMenuItemSpriteLambda::create(n_sound, s_sound, [=](CCObject* sender)
																	  {
																		  if(!is_actioned)
																		  {
																			  is_actioned = true;
																			  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																			  myDSH->setBoolForKey(kDSH_Key_isOffCardMorphingSound, !myDSH->getBoolForKey(kDSH_Key_isOffCardMorphingSound));
																			  refresh_morphing_sound();
																			  is_actioned = false;
																		  }
																	  });
		CCMenuLambda* sound_menu = CCMenuLambda::createWithItem(sound_item);
		sound_menu->setPosition(sound_position);
        sound_menu->setEnabled(false);
		addChild(sound_menu, kCV_Z_next_button);
		
//		auto tuto = KS::loadCCBI<CCSprite*>(this, "tutorial_touch.ccbi");
//		
//		zoom_img = tuto.first;
//		tuto.second->runAnimationsForSequenceNamed("Default Timeline");
//		
//		
//		zoom_img->setPosition(ccp(240, myDSH->ui_center_y));
//		addChild(zoom_img, kCV_Z_next_button);
//		
//		tuto.second->setDelegate(this);
//		t_manager = tuto.second;
	}
	
	
	if(morphing_filename != "")
	{
		morphing_img = KS::loadCCBI<CCSprite*>(this, morphing_filename.c_str()).first;
		morphing_img->setPosition(morphing_position);
		addChild(morphing_img, kCV_Z_next_button);
	}
	
//	if(!is_morphing && mySGD->is_morphing_noti)
//	{
//		mySGD->is_morphing_noti = false;
//		
//		BuyMorphingPopup* t_popup = BuyMorphingPopup::create(-200, [=](){is_actioned = false;}, liveGirl);
//		addChild(t_popup, 999);
//	}
	
	
	next_button = CommonButton::createCloseButton(-160);
	next_button->setFunction([=](CCObject* sender){menuAction(sender);});
	next_button->setPosition(ccp(480-35,myDSH->ui_top-35));
	next_button->setVisible(false);
	addChild(next_button, kCV_Z_next_button);
	
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
	addChild(mode_button, kCV_Z_next_button);
	
	if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number) && mySGD->getOnAdultTag())
	{
		CCSprite* n_19 = CCSprite::create("diary_19.png");
		CCSprite* s_19 = CCSprite::create("diary_19.png");
		s_19->setColor(ccGRAY);
		
		CCMenuItemLambda* to_diary_19_item = CCMenuItemSpriteLambda::create(n_19, s_19, [=](CCObject* sender)
																			{
																				if(!is_actioned)
																				{
																					is_actioned = true;
																					AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																					
																					if(graphdog->isExistApp())
																					{
																						// 다이어리 앱 설치되어 있음
																						
																						LoadingLayer* t_loading = LoadingLayer::create(-9999, true);
																						addChild(t_loading, 9999);
																						t_loading->startLoading();
																						
																						Json::Value t_param;
																						t_param["memberID"] = myHSP->getMemberID();
																						
																						myHSP->command("makediarycode", t_param, [=](Json::Value result_data)
																									   {
																										   if(result_data["result"]["code"].asInt() == GDSUCCESS)
																										   {
																											   graphdog->openDiaryApp(t_param["memberID"].asString(), result_data["diaryCode"].asString(), card_number); // 다이어리 앱 실행 result_data["diaryCode"].asString() 과 myHSP->getMemberID() 를 보내줌
																										   }
																										   else
																										   {
																											   CCLOG("failed makediarycode");
																										   }
																										   
																										   t_loading->removeFromParent();
																										   is_actioned = false;
																									   });
																					}
																					else
																					{
																						Diary19Popup* t_popup = Diary19Popup::create(-999, [=]()
																																	 {
																																		 is_actioned = false;
																																	 }, true);
																						addChild(t_popup, 999);
																					}
																				}
																			});
		
		CCMenuLambda* to_diary_19_menu = CCMenuLambda::createWithItem(to_diary_19_item);
		to_diary_19_menu->setPosition(ccp(40,64));
		addChild(to_diary_19_menu, kCV_Z_next_button);
	}
	
	//	is_touched_menu = false;
	is_actioned = true;
	
	screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	minimum_scale = (screen_size.height*320)/(screen_size.width*430)*1.25f;
	
	//game_node->setPosition(ccp(0,-430*1.5f+480.f*screen_size.height/screen_size.width));
	
	return true;
}

void CardViewScene::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
	startTouchAction();
	
//	CCMoveTo* move1 = CCMoveTo::create(1.f, ccp(0,0));
//	CCDelayTime* delay1 = CCDelayTime::create(1.f);
//	
//	CCMoveTo* move2 = CCMoveTo::create(0.7f, ccp((480.f-320.f*minimum_scale)/2.f, 0));
//	CCScaleTo* t_scale = CCScaleTo::create(0.7f, minimum_scale);
//	CCSpawn* t_spawn = CCSpawn::create(move2, t_scale, NULL);
//	
//	//	CCMoveTo* move2 = CCMoveTo::create(1.f, ccp(0,-430*1.5f+480.f*screen_size.height/screen_size.width));
//	CCDelayTime* delay2 = CCDelayTime::create(1.f);
//	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::startScript));
//	game_node->runAction(CCSequence::create(move1, delay1, t_spawn, delay2, t_call, NULL));
}

void CardViewScene::startTouchAction()
{
	is_actioned = false;
	setTouchEnabled(true);
	next_button->setVisible(true);
	//	mode_button->setVisible(true);
	
	//	target_node->setTouchEnabled(true);
	
	is_scrolling = false;
	is_before_scrolling = is_scrolling;
	
	save_position = game_node->getPosition();
	//schedule(schedule_selector(CardViewScene::moveChecking));
}

void CardViewScene::moveChecking()
{
	CCPoint after_position = game_node->getPosition();
	
	if(is_scrolling)
	{
//		if(is_morphing)
//			first_img->movingDistance(ccpSub(after_position, save_position));
		is_before_scrolling = is_scrolling;
	}
	else if(is_before_scrolling)
	{
		is_before_scrolling = false;
//		if(is_morphing)
//			first_img->movingDistance(CCPointZero);
	}
	save_position = after_position;
}

void CardViewScene::menuAction(CCObject *sender)
{
	if(!is_actioned)
	{
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
		
		//unschedule(schedule_selector(CardViewScene::moveChecking));
		
		is_actioned = true;
		next_button->setVisible(false);
		mode_button->setVisible(false);
		setTouchEnabled(false);
		first_img->setTouchEnabled(false);
		unschedule(schedule_selector(CardViewScene::moveAnimation));
		
		temp_end_func();
		removeAllChildren();
//		CCTransitionFadeTR* t_trans = CCTransitionFadeTR::create(1.f, ZoomScript::scene());
		CCDirector::sharedDirector()->popScene();
	}
}

void CardViewScene::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	
	pDispatcher->addStandardDelegate(this, -150);
}

void CardViewScene::moveListXY(CCPoint t_p)
{
	if(t_p.x > CV_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.x = CV_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	if(t_p.x < -CV_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.x = -CV_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	
	if(t_p.y > CV_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.y = CV_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	if(t_p.y < -CV_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.y = -CV_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	//240,160
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
	
	
//	if(game_node->getScale() <= 1.5f)
//	{
//		if(a_p.x > (480.f-320.f*game_node->getScale()/2.f))
//			a_p.x = 480.f-320.f*game_node->getScale()/2.f;
//		else if(a_p.x < 320.f*game_node->getScale()/2.f)
//			a_p.x = 320.f*game_node->getScale()/2.f;
//		
//	}
//	else
//	{
//		if(a_p.x - game_node->getScale()*480 > 480)
//			a_p.x = 480;
//		else if(a_p.x < 0)
//			a_p.x = 0;
//	
//	}
//	
//	if(a_p.y - game_node->getScale()*320 > 320)
//		a_p.y = 320;
//	else if(a_p.y < 0)
//		a_p.y = 0;
	
//	if(a_p.y > 0+40.f)
//		a_p.y = 0+40.f;
//	if(a_p.y < -430*game_node->getScale()+480*screen_size.height/screen_size.width-40.f)
//		a_p.y = -430*game_node->getScale()+480*screen_size.height/screen_size.width-40.f;
//	
	game_node->setPosition(a_p);
}

void CardViewScene::moveAnimation()
{
	isAnimated = true;
	
	if(moveSpeed_p.x >= CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())			moveSpeed_p.x -= CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else if(moveSpeed_p.x <= -CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())	moveSpeed_p.x += CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else							moveSpeed_p.x = 0;
	
	if(moveSpeed_p.y >= CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())			moveSpeed_p.y -= CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else if(moveSpeed_p.y <= CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())	moveSpeed_p.y += CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else							moveSpeed_p.y = 0;
	
	if((moveSpeed_p.x < CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale() && moveSpeed_p.x > -CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale() &&
		moveSpeed_p.y < CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale() && moveSpeed_p.y > -CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale()) || isAnimated == false)
	{
		this->unschedule(schedule_selector(CardViewScene::moveAnimation));
		moveSpeed_p = CCPointZero;
		isAnimated = false;
	}
	this->moveListXY(ccpMult(moveSpeed_p, -1));
}

void CardViewScene::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
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
		
		this->unschedule(schedule_selector(CardViewScene::moveAnimation));
		
		isAnimated=false;
		
		if(multiTouchData.size() >= 1)
		{
			first_img->setTouchEnabled(false);
		}
		
		if(multiTouchData.size() == 1)
		{
			first_touch_time = touchStartTime;
			first_touch_p = (int)touch;
			first_touch_point = location;
			is_scrolling = true;
//			first_img->ccTouchBegan(touch,pEvent);
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

void CardViewScene::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
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
			
			if(multiTouchData.size() == 1 && is_morphing && first_img->m_waveRange==1)
			{
				
				touch_p = location;
//				if(is_morphing)first_img->ccTouchMoved(touch,pEvent);
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

void CardViewScene::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
	CCSetIterator iter;
	CCTouch* touch;
	
	for(iter = pTouches->begin();iter != pTouches->end();++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		
		map<int, CCPoint>::iterator o_it;
		o_it = multiTouchData.find((int)touch);
		int touchSize = multiTouchData.size();
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
//					if(is_morphing && touch_mode!=2)
//						first_img->ccTouchEnded(touch, pEvent);
				}
				else
				{
					
//					if(is_morphing && touch_mode!=2)
//						first_img->morphing(touch, pEvent);
//					return;
					unsigned long long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
					CCPoint _spd = ccpMult(ccpSub(location, touchStart_p), 1.f/_time*10000);
					
					float spd_value = sqrtf(powf(_spd.x, 2.f) + powf(_spd.y, 2.f));
					if(isAnimated == false && fabsf(spd_value) > 2 && !is_spin_mode)
					{
						moveSpeed_p = _spd;
						this->schedule(schedule_selector(CardViewScene::moveAnimation));
					}
				}
			}
		}
	}
}

void CardViewScene::ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
{
	ccTouchesEnded(pTouches, pEvent);
}

void CardViewScene::keyBackClicked()
{
	CommonButton::callBackKey();
}
