//
//  EndlessModeOpening.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 19..
//
//

#include "EndlessModeOpening.h"
#include "DataStorageHub.h"
#include "KSLabelTTF.h"
#include "StarGoldData.h"
#include "CommonButton.h"
#include "TouchSuctionLayer.h"
#include "CCMenuLambda.h"
#include "FlagSelector.h"
#include "FormSetter.h"
#include "LoadingLayer.h"
#include "ASPopupView.h"
#include "DownloadFile.h"
#include "StageImgLoader.h"
#include "StartSettingPopup.h"
#include "MainFlowScene.h"
#include "StyledLabelTTF.h"
#include "MyLocalization.h"
#include "RivalSelectPopup.h"
#include "CommonAnimation.h"
#include "FormSetter.h"
#include "LabelTTFMarquee.h"
#include "StyledLabelTTF.h"
enum EndlessModeOpeningZorder
{
	kEndlessModeOpeningZorder_gray = 0,
	kEndlessModeOpeningZorder_back,
	kEndlessModeOpeningZorder_content
};
const static int kCellCase = 0x45451;
bool EndlessModeOpening::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	current_rank_percent = 100.f;
	animation_node1 = animation_node2 = nullptr;
	startFormSetter(this);
//	FormSetter::get()->start();
	
	refresh_ing_win_func = nullptr;
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
	
	touch_priority = -300;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kEndlessModeOpeningZorder_gray);
	
	gray->runAction(CCFadeTo::create(0.5f, 255));
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	suction->setTouchEnabled(true);
	addChild(suction);
	
	is_menu_enable = false;
	
	setMain();
	
	Json::Value param;
	param["memberID"] = myHSP->getMemberID();
	
	myHSP->command("getendlessrank", param, this,json_selector(this, EndlessModeOpening::resultGetEndlessRank));
	
	return true;
}

void EndlessModeOpening::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

void EndlessModeOpening::setMain()
{
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kEndlessModeOpeningZorder_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessMode), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setAnchorPoint(ccp(0,0.5f));
	title_label->setPosition(ccp(26, main_case->getContentSize().height-25));
	main_case->addChild(title_label);
	setFormSetter(title_label);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(main_case->getContentSize().width-25, main_case->getContentSize().height-25));
	close_button->setFunction([=](CCObject* sender)
							  {
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  
								  myHSP->removeTarget(this);
								  myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
								  
								  addChild(KSGradualValue<float>::create(1.f, 0.f, 0.2f, [=](float t)
																		 {
																			 gray->setOpacity(255*t);
																		 }, [=](float t)
																		 {
																			 gray->setOpacity(0);
																			 if(target_final && delegate_final)
																				 (target_final->*delegate_final)();
																			 removeFromParent();
																		 }));
								  
									CommonAnimation::closePopup(this, main_case, gray, [=](){
										
									}, [=](){
//										end_func(); removeFromParent();
									});
//								  addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
//									  addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
//								  
//								  addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
//								  {
//									  KS::setOpacity(main_case, t);
//									  if(t > 100)
//										{
//											this->n_ready_label2->setOpacity(100);
//											this->s_ready_label2->setOpacity(100);
//											
//											
//										}
//								  }, [=](int t)
//								  {
//									  KS::setOpacity(main_case, 0);
//									  this->n_ready_label2->setOpacity(0);
//									  this->s_ready_label2->setOpacity(0);
//										
//								  }));
							  });
	main_case->addChild(close_button, 2);
	
	left_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	left_back->setContentSize(CCSizeMake(255, 232));
	left_back->setPosition(ccp(10+left_back->getContentSize().width/2.f, main_case->getContentSize().height*0.44f));
	main_case->addChild(left_back);
	CCScale9Sprite* tipBack = CCScale9Sprite::create("common_time.png", CCRectMake(0, 0, 22, 22), CCRectMake(10, 10, 2, 2));
	tipBack->setPosition(ccp(173.5,255.5f)); 			// dt (173.5,246.0)
	main_case->addChild(tipBack, 2);
	tipBack->setContentSize(CCSizeMake(65, 26));
	KSLabelTTF* tipLbl = KSLabelTTF::create("Tip", mySGD->getFont().c_str(), 14.f);
	tipLbl->setColor(ccc3(255, 155, 0));
	tipBack->addChild(tipLbl);
	tipLbl->setPosition(ccpFromSize(tipBack->getContentSize()) / 2.f);
	
	LabelTTFMarquee* tipMaquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 245, 25, "");
	tipMaquee->addText("<font size=13>PVP랭킹은 연승이 중요해요.</font>");
	tipMaquee->addText("<font size=13>연승수가 같은 경우 누적점수로 랭킹이 결정됩니다.</font>");
	tipMaquee->addText("<font size=13>PVP점수는 기본주간랭킹에 추가되지 않습니다.</font>");
	tipMaquee->addText("<font size=13>랭킹은 매주 갱신됩니다.</font>");
//	tipMaquee->setPosition(ccp(201.0,237.0)); 			// dt (131.0,232.0)
	tipMaquee->setPosition(ccp(199.5, 244.f));
	tipMaquee->startMarquee();
	main_case->addChild(tipMaquee, 1);
	
	KSLabelTTF* left_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessWeeklyranking), mySGD->getFont().c_str(), 13);
	left_title->setColor(ccc3(255, 170, 20));
	left_title->setAnchorPoint(ccp(0,0.5f));
	left_title->setPosition(ccp(15.5,left_back->getContentSize().height-20));
	left_back->addChild(left_title);
	
	startFormSetter(this);
	setFormSetter(left_title);
	setFormSetter(tipBack);
	setFormSetter(tipMaquee);
	loading_circle = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
	loading_circle->setPosition(ccp(left_back->getContentSize().width/2.f, left_back->getContentSize().height/2.f));
	left_back->addChild(loading_circle);
	
	rest_back = CCScale9Sprite::create("common_time.png", CCRectMake(0, 0, 22, 22), CCRectMake(10, 10, 2, 2));
	left_back->addChild(rest_back);
	
	rest_time_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_restTime), mySGD->getFont().c_str(), 11);
	rest_time_title->setColor(ccc3(255, 170, 0));
	rest_time_title->setAnchorPoint(ccp(0,0.5f));
	
	rest_back->addChild(rest_time_title);
	
	rest_time_value = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_restTimeHour), mySGD->getFont().c_str(), 11);
	rest_time_value->setVisible(false);
	rest_time_value->setAnchorPoint(ccp(1,0.5f));
	rest_back->addChild(rest_time_value);
	
	rest_back->setContentSize(CCSizeMake(rest_time_title->getContentSize().width + rest_time_value->getContentSize().width + 20, 22));
	rest_back->setPosition(ccp(left_back->getContentSize().width-10-rest_back->getContentSize().width/2.f,left_back->getContentSize().height-20));
	
	rest_time_title->setPosition(ccp(10 + 4.5 + 10.f, rest_back->getContentSize().height/2.f));
	rest_time_value->setPosition(ccp(rest_back->getContentSize().width-10 + 18.f - 10.f, rest_back->getContentSize().height/2.f));
	
	right_back = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
	right_back->setContentSize(CCSizeMake(205, 180));
	right_back->setPosition(ccp(main_case->getContentSize().width-10-right_back->getContentSize().width/2.f, 149));
	main_case->addChild(right_back);
	
//	setFormSetter(rest_back);
//	setFormSetter(rest_time_title);
//	setFormSetter(rest_time_value);
	
	
//	KSLabelTTF* right_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessMyInfo), mySGD->getFont().c_str(), 18);
//	right_title->setColor(ccc3(255, 170, 0));
//	right_title->setPosition(ccp(right_back->getContentSize().width/2.f, 162));
//	right_back->addChild(right_title);
	
	CCScale9Sprite* graph_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	graph_back->setContentSize(CCSizeMake(right_back->getContentSize().width-20 + 0.5f, 54 + 6.f));
	graph_back->setPosition(ccp(right_back->getContentSize().width/2.f, 126 - 5.f));
	right_back->addChild(graph_back);
		
	setFormSetter(graph_back);
	CCSprite* graph_img = CCSprite::create("endless_graph.png");
	graph_img->setPosition(ccp(graph_back->getContentSize().width/2.f, 16));
	graph_back->addChild(graph_img);
	
	setFormSetter(graph_img);
//	KSLabelTTF* t_rank_a = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankA), mySGD->getFont().c_str(), 9);
//	t_rank_a->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//	t_rank_a->setPosition(ccp(33.5f,graph_img->getPositionY()));
//	graph_back->addChild(t_rank_a);
//	
//	KSLabelTTF* t_rank_b = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankB), mySGD->getFont().c_str(), 9);
//	t_rank_b->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//	t_rank_b->setPosition(ccp(74.5f,graph_img->getPositionY()));
//	graph_back->addChild(t_rank_b);
//	
//	KSLabelTTF* t_rank_c = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankC), mySGD->getFont().c_str(), 9);
//	t_rank_c->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//	t_rank_c->setPosition(ccp(115.5f,graph_img->getPositionY()));
//	graph_back->addChild(t_rank_c);
//	
//	KSLabelTTF* t_rank_d = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankD), mySGD->getFont().c_str(), 9);
//	t_rank_d->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//	t_rank_d->setPosition(ccp(156.5f,graph_img->getPositionY()));
//	graph_back->addChild(t_rank_d);
	
	
//	CCLabelTTF* all_user_label = CCLabelTTF::create(CCString::createWithFormat("/%d", 9999)->getCString(), mySGD->getFont().c_str(), 10);
//	all_user_label->setColor(ccc3(255, 170, 20));
//	all_user_label->setAnchorPoint(ccp(1,0.5));
//	all_user_label->setPosition(ccp(graph_back->getContentSize().width-10, 13));
//	graph_back->addChild(all_user_label);
//	
//	CCLabelTTF* my_rank_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_myrankValue), 500)->getCString(), mySGD->getFont().c_str(), 10);
//	my_rank_label->setAnchorPoint(ccp(1,0.5));
//	my_rank_label->setPosition(ccp(all_user_label->getPositionX()-all_user_label->getContentSize().width, all_user_label->getPositionY()));
//	graph_back->addChild(my_rank_label);
//	my_rank_label->setOpacity(0);
	
	
	rank_percent_case = CCSprite::create("gameresult_rank_percent.png");
	rank_percent_case->setAnchorPoint(ccp(0.5,0));
	rank_percent_case->setPosition(ccp(177,15.5f)); // 177
	graph_back->addChild(rank_percent_case);
	
	percent_label = KSLabelTTF::create("", mySGD->getFont().c_str(), 13);
	percent_label->setColor(ccc3(255, 170, 20));
	percent_label->enableOuterStroke(ccc3(50, 25, 0), 1);
	percent_label->setPosition(ccp(rank_percent_case->getContentSize().width/2.f+1,
																 rank_percent_case->getContentSize().height/2.f+2));
	setFormSetter(rank_percent_case);
	rank_percent_case->addChild(percent_label);
	percent_label->setOpacity(0);
	
	
	record_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	record_back->setContentSize(CCSizeMake(right_back->getContentSize().width-20 + 1.f, 35));
	record_back->setPosition(ccp(right_back->getContentSize().width/2.f, 85.5f - 7.5f));
	right_back->addChild(record_back);
	
	KSLabelTTF* record_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessInfoScore), mySGD->getFont().c_str(), 13);
	record_title->setAnchorPoint(ccp(0,0.5f));
	record_title->setPosition(ccp(10, record_back->getContentSize().height/2.f));
	record_title->enableOuterStroke(ccc3(0, 0, 0), 1.f);
	record_back->addChild(record_title);
	
	setFormSetter(record_title);
	setFormSetter(record_back);

	
	CCScale9Sprite* highscore_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	highscore_back->setContentSize(CCSizeMake(right_back->getContentSize().width-20 + 1.f, 35));
	highscore_back->setPosition(ccp(right_back->getContentSize().width/2.f + 1.f - 1.f, 55 - 4.5f));
	right_back->addChild(highscore_back);
	setFormSetter(highscore_back);
	KSLabelTTF* highscore_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessHighScore), mySGD->getFont().c_str(), 13);
	setFormSetter(highscore_title);
	highscore_title->setAnchorPoint(ccp(0,0.5f));
	highscore_title->setPosition(ccp(8, highscore_back->getContentSize().height/2.f + 2.f - 1.f));
	highscore_back->addChild(highscore_title);
	highscore_title->enableOuterStroke(ccc3(0, 0, 0), 1.f);
	highscore_content = KSLabelTTF::create("", mySGD->getFont().c_str(), 13);
	highscore_content->setAnchorPoint(ccp(1,0.5f));
	highscore_content->setPosition(ccp(highscore_back->getContentSize().width-10 + 0.5f,
																		 highscore_back->getContentSize().height/2.f + 1.5f - 0.5f));
	highscore_content->enableOuterStroke(ccc3(0, 0, 0), 1.f);
	highscore_back->addChild(highscore_content);
	
	CCScale9Sprite* straight_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	straight_back->setContentSize(CCSizeMake(right_back->getContentSize().width-20 + 1.f, 35));
	straight_back->setPosition(ccp(right_back->getContentSize().width/2.f, 24.5f - 1.f));
	right_back->addChild(straight_back);
	
	KSLabelTTF* straight_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessHighStraight), mySGD->getFont().c_str(), 13);
	straight_title->setAnchorPoint(ccp(0,0.5f));
	straight_title->setPosition(ccp(10, straight_back->getContentSize().height/2.f + 1.5f - 0.5f));
	straight_back->addChild(straight_title);
	straight_title->enableOuterStroke(ccc3(0, 0, 0), 1.f);
	
	straight_content = KSLabelTTF::create("", mySGD->getFont().c_str(), 13);
	straight_content->setAnchorPoint(ccp(1,0.5f));
	straight_content->setPosition(ccp(straight_back->getContentSize().width-10,
																		straight_back->getContentSize().height/2.f + 2.f - 1.0f));
	straight_content->enableOuterStroke(ccc3(0, 0, 0), 1.f);
	straight_back->addChild(straight_content);
	
	setFormSetter(straight_title);
	setFormSetter(straight_back);
	
	
	CCSprite* n_ready = CCSprite::create("endless_ready.png");
	
	n_ready_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessReady), mySGD->getFont().c_str(), 22);
	n_ready_label2->setColor(ccWHITE);
	n_ready_label2->disableOuterStroke();
	n_ready_label2->setOpacity(100);
	n_ready_label2->setPosition(ccp(n_ready->getContentSize().width/2.f, n_ready->getContentSize().height/2.f-2));
	n_ready->addChild(n_ready_label2);
	
	n_ready_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessReady), mySGD->getFont().c_str(), 22);
	n_ready_label->setColor(ccc3(50, 30, 5));
	n_ready_label->setPosition(ccp(n_ready->getContentSize().width/2.f, n_ready->getContentSize().height/2.f-1));
	n_ready->addChild(n_ready_label);
	
	CCSprite* s_ready = CCSprite::create("endless_ready.png");
	s_ready->setColor(ccGRAY);
	
	s_ready_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessReady), mySGD->getFont().c_str(), 22);
	s_ready_label2->setColor(ccWHITE);
	s_ready_label2->setOpacity(100);
	s_ready_label2->disableOuterStroke();
	s_ready_label2->setPosition(ccp(s_ready->getContentSize().width/2.f, s_ready->getContentSize().height/2.f-2));
	s_ready->addChild(s_ready_label2);
	
	s_ready_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessReady), mySGD->getFont().c_str(), 22);
	s_ready_label->setColor(ccc3(50, 30, 5));
	s_ready_label->setPosition(ccp(s_ready->getContentSize().width/2.f, s_ready->getContentSize().height/2.f-1));
	s_ready->addChild(s_ready_label);
	
	CCMenuItemLambda* ready_item = CCMenuItemSpriteLambda::create(n_ready, s_ready, [=](CCObject* sender)
																  {
																	  if(!is_menu_enable)
																		  return;
																	  
																	  is_menu_enable = false;
																	  
																	  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																	  
																	  ready_loading = LoadingLayer::create(-99999);
																	  addChild(ready_loading, 99999);
																	  
																	  Json::Value param;
																	  param["memberID"] = myHSP->getMemberID();
//																	  param["no"] = 34;
																	  myHSP->command("startendlessplay", param, this,json_selector(this, EndlessModeOpening::resultGetEndlessPlayData));
																  });
	
	CCMenuLambda* ready_menu = CCMenuLambda::createWithItem(ready_item);
	ready_menu->setPosition(ccp(right_back->getContentSize().width/2.f,-25));
	ready_menu->setTouchPriority(touch_priority);
	right_back->addChild(ready_menu);
	
	CommonAnimation::openPopup(this, main_case, gray, [=](){
		
//		if(t > 100)
//		{
//			n_ready_label2->setOpacity(100);
//			s_ready_label2->setOpacity(100);
//		}
	}, [=](){
		n_ready_label2->setOpacity(100);
		s_ready_label2->setOpacity(100);
		//											 mySelection->setOpacity(0);
		
		if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
		{
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			if(screen_scale_x < 1.f)
				screen_scale_x = 1.f;
			
			CCNode* t_stencil_node = CCNode::create();
			CCScale9Sprite* t_stencil1 = CCScale9Sprite::create("rank_normal.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
			t_stencil1->setContentSize(right_back->getContentSize() + CCSizeMake(10, 10));
			t_stencil1->setPosition(main_case->getPosition() - ccpFromSize(main_case->getContentSize()/2.f) + right_back->getPosition());
			t_stencil_node->addChild(t_stencil1);
			
			CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
			t_clipping->setAlphaThreshold(0.1f);
			
			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
			
			float change_scale = 1.f;
			CCPoint change_origin = ccp(0,0);
			if(screen_scale_x > 1.f)
			{
				change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
				change_scale = screen_scale_x;
			}
			if(screen_scale_y > 1.f)
				change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
			CCSize win_size = CCDirector::sharedDirector()->getWinSize();
			t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
			
			
			CCSprite* t_gray = CCSprite::create("back_gray.png");
			t_gray->setScaleX(screen_scale_x);
			t_gray->setScaleY(myDSH->ui_top/myDSH->screen_convert_rate/320.f);
			t_gray->setOpacity(0);
			t_gray->setPosition(ccp(240,160));
			t_clipping->addChild(t_gray);
			
			CCSprite* t_arrow1 = CCSprite::create("main_tutorial_arrow1.png");
			t_arrow1->setRotation(180);
			t_arrow1->setPosition(t_stencil1->getPosition() + ccp(0,t_stencil1->getContentSize().height/2.f + 15));
			t_clipping->addChild(t_arrow1);
			
			StyledLabelTTF* t_ment1 = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessTutorialMent2), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_ment1->setAnchorPoint(ccp(0.5f,0.f));
			t_ment1->setPosition(t_arrow1->getPosition() + ccp(0, t_arrow1->getContentSize().height/2.f + 3));
			t_clipping->addChild(t_ment1);
			
			t_suction = TouchSuctionLayer::create(-10000);
			addChild(t_suction);
			t_suction->setTouchEnabled(true);
			
			t_clipping->setInverted(true);
			addChild(t_clipping, 9999);
			
			addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
												   {
													   t_gray->setOpacity(t*255);
												   }, [=](float t)
												   {
													   t_gray->setOpacity(255);
													   this->is_menu_enable = true;
													   
													   this->t_suction->is_on_touch_began_func = true;
													   this->t_suction->touch_began_func = [=]()
													   {
														   this->t_suction->is_on_touch_began_func = false;
														   if(!this->is_menu_enable)
															   return;
														   
														   this->is_menu_enable = false;
														   
														   t_arrow1->removeFromParent();
														   t_ment1->removeFromParent();
														   
														   t_stencil1->setContentSize(left_back->getContentSize() + CCSizeMake(10, 10));
														   t_stencil1->setPosition(main_case->getPosition() - ccpFromSize(main_case->getContentSize()/2.f) + left_back->getPosition());
														   
														   CCSprite* t_arrow2 = CCSprite::create("main_tutorial_arrow1.png");
														   t_arrow2->setRotation(-90);
														   t_arrow2->setPosition(t_stencil1->getPosition() + ccp(t_stencil1->getContentSize().width/2.f + 15, 0));
														   t_clipping->addChild(t_arrow2);
														   
														   StyledLabelTTF* t_ment2 = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessTutorialMent3), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kLeftAlignment);
														   t_ment2->setAnchorPoint(ccp(0.f,0.5f));
														   t_ment2->setPosition(t_arrow2->getPosition() + ccp(t_arrow2->getContentSize().width/2.f + 3, 0));
														   t_clipping->addChild(t_ment2);
														   
														   this->is_menu_enable = true;
														   this->t_suction->touchFuncOn();
														   this->t_suction->touch_began_func = [=]()
														   {
															   this->t_suction->is_on_touch_began_func = false;
															   if(!this->is_menu_enable)
																   return;
															   
															   
															   this->is_menu_enable = false;
															   
															   t_arrow2->removeFromParent();
															   t_ment2->removeFromParent();
															   
															   t_stencil1->setContentSize(n_ready->getContentSize() + CCSizeMake(10, 10));
															   t_stencil1->setPosition(main_case->getPosition() - ccpFromSize(main_case->getContentSize()/2.f) + right_back->getPosition() - ccpFromSize(right_back->getContentSize()/2.f) + ready_menu->getPosition());
															   
															   CCSprite* t_arrow3 = CCSprite::create("main_tutorial_arrow1.png");
															   t_arrow3->setRotation(180);
															   t_arrow3->setPosition(t_stencil1->getPosition() + ccp(0, t_stencil1->getContentSize().height/2.f + 15));
															   t_clipping->addChild(t_arrow3);
															   
															   StyledLabelTTF* t_ment3 = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessTutorialMent4), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
															   t_ment3->setAnchorPoint(ccp(0.5f,0.f));
															   t_ment3->setPosition(t_arrow3->getPosition() + ccp(0, t_arrow3->getContentSize().height/2.f + 3));
															   t_clipping->addChild(t_ment3);
															   
															   this->is_menu_enable = true;
															   this->t_suction->is_on_touch_began_func = true;
															   this->t_suction->touch_began_func = [=]()
															   {
																   this->t_suction->is_on_touch_began_func = false;
																   if(!this->is_menu_enable)
																	   return;
																   
																   this->is_menu_enable = false;
																   
																   this->tutorial_success_func = [=]()
																   {
																	   t_arrow3->removeFromParent();
																	   t_ment3->removeFromParent();
																	   
																	   addChild(KSGradualValue<float>::create(1.f, 0.f, 1.f, [=](float t)
																											  {
																												  t_gray->setOpacity(255*t);
																											  }, [=](float t)
																											  {
																												  t_gray->setOpacity(255*t);
																												  
																												  this->t_suction->removeFromParent();
																												  
																												  t_clipping->removeFromParent();
																											  }));
																   };
																   
																   this->tutorial_fail_func = [=]()
																   {
																	   this->t_suction->is_on_touch_began_func = true;
																	   this->is_menu_enable = true;
																   };
																   
																   ready_loading = LoadingLayer::create(-99999);
																   addChild(ready_loading, 99999);
																   
																   Json::Value param;
																   param["memberID"] = myHSP->getMemberID();
																   //																	  param["no"] = 34;
																   myHSP->command("startendlessplay", param, this,json_selector(this, EndlessModeOpening::resultGetEndlessPlayData));
															   };
														   };
													   };
												   }));
		}
		else
			is_menu_enable = true;
	});
}

void EndlessModeOpening::resultGetEndlessPlayData(Json::Value result_data)
{
	GraphDogLib::JsonToLog("getendlessplaydata : %s", result_data);
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->resetReplayPlayingInfo();
		
		if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
		{
			string easy_play_data = R"({"as":0,"cg":0,"cs":0,"ds":0,"gt":150,"icm":false,"ics":false,"lbc":null,"lose":37,"md":[[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,196608,196608,196608,196608,196608,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]],"mt":[0],"os":0,"sd":[0],"st":[0],"ta":0.01499999966472387,"tg":0,"ts":[{"mi":0,"si":0},{"boss":[{"bpx":188.7735595703125,"bpy":328.9600524902344}],"cpx":161,"cpy":217,"mob":[{"x":67.68907928466797,"y":298.0100708007812}]},{"boss":[{"bpx":154.3245086669922,"bpy":321.8156127929688}],"cpx":161,"cpy":217,"mob":[{"x":54.15271759033203,"y":317.3423461914062}]},{"boss":[{"bpx":121.0160217285156,"bpy":310.5301513671875}],"cpx":161,"cpy":217,"mob":[{"x":40.00989151000977,"y":334.8093872070312}]},{"boss":[{"bpx":95.23295593261719,"bpy":295.490966796875}],"cpx":161,"cpy":217,"mob":[{"x":20.6778450012207,"y":321.2723999023438}]},{"boss":[{"bpx":106.7893981933594,"bpy":262.2425842285156}],"cpx":143,"cpy":219,"mob":[{"x":6.386994361877441,"y":306.707763671875}]},{"boss":[{"bpx":113.7460403442383,"bpy":230.3540344238281}],"cpx":121,"cpy":257,"id":true,"mob":[{"x":19.92340850830078,"y":287.37548828125}]},{"boss":[{"bpx":129.7234344482422,"bpy":221.6747436523438}],"cpx":137,"cpy":219,"mob":[{"x":33.45981216430664,"y":268.043212890625}]},{"boss":[{"bpx":112.8137741088867,"bpy":252.497314453125}],"cpx":137,"cpy":219,"mob":[{"x":46.99611282348633,"y":248.7109375}]},{"boss":[{"bpx":95.82832336425781,"bpy":283.3076477050781}],"cpx":137,"cpy":219,"mob":[{"x":57.15585708618164,"y":230.096435546875}]},{"boss":[{"bpx":78.37179565429688,"bpy":313.8701782226562}],"cpx":137,"cpy":219,"mob":[{"x":36.92659378051758,"y":217.94189453125}]},{"boss":[{"bpx":57.63263320922852,"bpy":329.7658081054688}],"cpx":113,"cpy":247,"mob":[{"x":16.69741821289062,"y":205.787353515625}]},{"boss":[{"bpx":29.94655227661133,"bpy":308.0441284179688}],"cpx":127,"cpy":293,"mob":[{"x":8.981722831726074,"y":190.3717041015625}]},{"boss":[{"bpx":5.879875659942627,"bpy":287.7563171386719}],"cpx":61,"cpy":293,"mob":[{"x":21.1366081237793,"y":170.1422119140625}]},{"boss":[{"bpx":13.40987110137939,"bpy":266.0831298828125}],"cpx":11,"cpy":277,"gi":-1,"id":true,"mob":[{"x":33.29150390625,"y":149.9127197265625}]}],"tts":0,"uic":0,"win":5})";
			
			Json::Value read_data;
			Json::Reader reader;
			reader.parse(easy_play_data, read_data);
			mySGD->setReplayPlayingInfo(read_data);
		}
		else if(!result_data["rival"]["playData"].isNull())
		{
			Json::Value read_data;
			Json::Reader reader;
			reader.parse(result_data["rival"]["playData"].asString(), read_data);
			mySGD->setReplayPlayingInfo(read_data);
		}
		
		mySGD->dummy_list = result_data["dummy"];
		
		mySGD->endless_memberID = result_data["rival"]["memberID"].asInt64();
		
		mySGD->endless_score = result_data["rival"]["score"].asInt();
		mySGD->endless_regDate = result_data["rival"]["regDate"].asInt64();
		
		mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_stageNo)] = result_data["stageInfo"]["realNo"].asInt();
		
		saveStageInfo(result_data["stageInfo"]);
	}
	else
	{
		ready_loading->removeFromParent();
		ready_loading = NULL;
		
		addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(kMyLocalKey_endlessServerError)), 999);
		
		if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
		{
			tutorial_fail_func();
		}
	}
}

void EndlessModeOpening::saveStageInfo(Json::Value result_data)
{
	df_list.clear();
	cf_list.clear();
	
	stage_number = result_data["no"].asInt();
	
	NSDS_SI(stage_number, kSDS_SI_puzzle_i, result_data["puzzle"].asInt(), false);
	NSDS_SI(stage_number, kSDS_SI_playtime_i, result_data["playtime"].asInt(), false);
	NSDS_SD(stage_number, kSDS_SI_scoreRate_d, result_data["scoreRate"].asDouble(), false);
	NSDS_SD(stage_number, kSDS_SI_scale_d, result_data["scale"].asDouble(), false);
	NSDS_SB(stage_number, kSDS_SI_minigame_b, result_data["minigame"].asBool(), false);
	NSDS_SS(stage_number, kSDS_SI_type_s, result_data["type"].asString(), false);
	
	Json::Value t_mission = result_data["mission"];
	NSDS_SI(stage_number, kSDS_SI_missionType_i, t_mission["type"].asInt(), false);
	
	Json::Value t_option;
	if(!t_mission["option"].isObject())			t_option["key"]="value";
	else										t_option =t_mission["option"];
	
	if(t_mission["type"].asInt() == kCLEAR_bossLifeZero)
		NSDS_SI(stage_number, kSDS_SI_missionOptionEnergy_i, t_option["energy"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_subCumberCatch)
		NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_bigArea)
	{
		NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
		NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
	}
	else if(t_mission["type"].asInt() == kCLEAR_itemCollect)
		NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_perfect)
		NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_timeLimit)
		NSDS_SI(stage_number, kSDS_SI_missionOptionSec_i, t_option["sec"].asInt(), false);
	
	
	Json::Value shopItems = result_data["shopItems"];
	NSDS_SI(stage_number, kSDS_SI_shopItemsCnt_i, shopItems.size(), false);
	for(int i=0;i<shopItems.size();i++)
	{
		Json::Value t_item = shopItems[i];
		NSDS_SS(stage_number, kSDS_SI_shopItems_int1_currency_s, i, t_item["currency"].asString(), false);
		NSDS_SI(stage_number, kSDS_SI_shopItems_int1_type_i, i, t_item["type"].asInt(), false);
		NSDS_SI(stage_number, kSDS_SI_shopItems_int1_price_i, i, t_item["price"].asInt(), false);
		
		
		Json::Value t_option;
		if(!t_item["option"].isObject())				t_option["key"]="value";
		else											t_option =t_item["option"];
		
		if(t_item["type"].asInt() == kIC_attack)
			NSDS_SI(stage_number, kSDS_SI_itemOptionAttackPower_i, t_option.get("power",0).asInt(), false);
		else if(t_item["type"].asInt() == kIC_addTime)
			NSDS_SI(stage_number, kSDS_SI_itemOptionAddTimeSec_i, t_option.get("sec",0).asInt(), false);
		else if(t_item["type"].asInt() == kIC_fast)
			NSDS_SI(stage_number, kSDS_SI_itemOptionFastSec_i, t_option.get("sec",0).asInt(), false);
		else if(t_item["type"].asInt() == kIC_silence)
			NSDS_SI(stage_number, kSDS_SI_itemOptionSilenceSec_i, t_option.get("sec",0).asInt(), false);
		else if(t_item["type"].asInt() == kIC_doubleItem)
			NSDS_SI(stage_number, kSDS_SI_itemOptionDoubleItemPercent_i, t_option["percent"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_longTime)
			NSDS_SI(stage_number, kSDS_SI_itemOptionLongTimeSec_i, t_option["sec"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_baseSpeedUp)
			NSDS_SI(stage_number, kSDS_SI_itemOptionBaseSpeedUpUnit_i, t_option["unit"].asInt(), false);
	}
	
	Json::Value defItems = result_data["defItems"];
	NSDS_SI(stage_number, kSDS_SI_defItemsCnt_i, defItems.size(), false);
	for(int i=0;i<defItems.size();i++)
	{
		Json::Value t_item = defItems[i];
		NSDS_SI(stage_number, kSDS_SI_defItems_int1_type_i, i, t_item["type"].asInt(), false);
		
		Json::Value t_option;
		if(!t_item["option"].isObject())				t_option["key"]="value";
		else											t_option =t_item["option"];
		
		if(t_item["type"].asInt() == kIC_attack)
			NSDS_SI(stage_number, kSDS_SI_itemOptionAttackPower_i, t_option["power"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_addTime)
			NSDS_SI(stage_number, kSDS_SI_itemOptionAddTimeSec_i, t_option["sec"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_fast)
			NSDS_SI(stage_number, kSDS_SI_itemOptionFastSec_i, t_option["sec"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_silence)
			NSDS_SI(stage_number, kSDS_SI_itemOptionSilenceSec_i, t_option["sec"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_doubleItem)
			NSDS_SI(stage_number, kSDS_SI_itemOptionDoubleItemPercent_i, t_option["percent"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_longTime)
			NSDS_SI(stage_number, kSDS_SI_itemOptionLongTimeSec_i, t_option["sec"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_baseSpeedUp)
			NSDS_SI(stage_number, kSDS_SI_itemOptionBaseSpeedUpUnit_i, t_option["unit"].asInt(), false);
	}
	
	Json::Value cards = result_data["cards"];
	NSDS_SI(stage_number, kSDS_SI_cardCount_i, cards.size(), false);
	for(int i=0;i<cards.size();i++)
	{
		Json::Value t_card = cards[i];
		NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
		NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
		NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
		NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
		
		NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
//		NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
		NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
		
		Json::Value t_card_missile = t_card["missile"];
		NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str(), false);
		NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt(), false);
		NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt(), false);
		NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble(), false);
		
		NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str(), false);
		
		Json::Value t_ability = t_card["ability"];
		NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), int(t_ability.size()), false);
		for(int j=0;j<t_ability.size();j++)
		{
			Json::Value t_abil = t_ability[j];
			NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt(), false);
			
			Json::Value t_option;
			if(!t_abil["option"].isObject())                    t_option["key"]="value";
			else												t_option =t_abil["option"];
			
			if(t_abil["type"].asInt() == kIC_attack)
				NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt(), false);
			else if(t_abil["type"].asInt() == kIC_addTime)
				NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
			else if(t_abil["type"].asInt() == kIC_fast)
				NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
			else if(t_abil["type"].asInt() == kIC_silence)
				NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
			else if(t_abil["type"].asInt() == kIC_doubleItem)
				NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
			else if(t_abil["type"].asInt() == kIC_longTime)
				NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
			else if(t_abil["type"].asInt() == kIC_baseSpeedUp)
				NSDS_SI(kSDS_CI_int1_abilityBaseSpeedUpOptionUnit_i, t_card["no"].asInt(), t_option["unit"].asInt(), false);
		}
		
		Json::Value t_imgInfo = t_card["imgInfo"];
		
		bool is_add_cf = false;
		
		if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
		{
			// check, after download ----------
			DownloadFile t_df;
			t_df.size = t_imgInfo["size"].asInt();
			t_df.img = t_imgInfo["img"].asString().c_str();
			t_df.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
			t_df.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
			df_list.push_back(t_df);
			// ================================
			
			CopyFile t_cf;
			t_cf.from_filename = t_df.filename.c_str();
			t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
			cf_list.push_back(t_cf);
			
			is_add_cf = true;
		}
		
		Json::Value t_aniInfo = t_card["aniInfo"];
		NSDS_SB(kSDS_CI_int1_aniInfoIsAni_b, t_card["no"].asInt(), t_aniInfo["isAni"].asBool(), false);
		if(t_aniInfo["isAni"].asBool())
		{
			Json::Value t_detail = t_aniInfo["detail"];
			NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card["no"].asInt(), t_detail["loopLength"].asInt(), false);
			
			Json::Value t_loopSeq = t_detail["loopSeq"];
			for(int j=0;j<t_loopSeq.size();j++)
				NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card["no"].asInt(), j, t_loopSeq[j].asInt(), false);
			
			NSDS_SI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card["no"].asInt(), t_detail["cutWidth"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card["no"].asInt(), t_detail["cutHeight"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_aniInfoDetailCutLength_i, t_card["no"].asInt(), t_detail["cutLength"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card["no"].asInt(), t_detail["positionX"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card["no"].asInt(), t_detail["positionY"].asInt(), false);
			
			if(NSDS_GS(kSDS_CI_int1_aniInfoDetailImg_s, t_card["no"].asInt()) != t_detail["img"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = t_detail["size"].asInt();
				t_df.img = t_detail["img"].asString().c_str();
				t_df.filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
				t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
				df_list.push_back(t_df);
				// ================================
			}
			
			if(is_add_cf)
			{
				CopyFile t_cf = cf_list.back();
				cf_list.pop_back();
				t_cf.is_ani = true;
				t_cf.cut_width = t_detail["cutWidth"].asInt();
				t_cf.cut_height = t_detail["cutHeight"].asInt();
				t_cf.position_x = t_detail["positionX"].asInt();
				t_cf.position_y = t_detail["positionY"].asInt();
				
				t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
				
				cf_list.push_back(t_cf);
			}
		}
		
		NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
		NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].asString(), false);
		NSDS_SS(kSDS_CI_int1_name_s, t_card["no"].asInt(), t_card["name"].asString(), false);
		NSDS_SI(kSDS_CI_int1_mPrice_ruby_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_ruby)].asInt(), false);
		NSDS_SI(kSDS_CI_int1_mPrice_pass_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_pass6)].asInt(), false);
		
		NSDS_SI(kSDS_CI_int1_type_i, t_card["no"].asInt(), t_card["type"].asInt(), false);
		NSDS_SS(kSDS_CI_int1_category_s, t_card["no"].asInt(), t_card["category"].asString(), false);
		NSDS_SI(kSDS_CI_int1_level_i, t_card["no"].asInt(), t_card["level"].asInt(), false);
		
		int sound_cnt = t_card["sound"].size();
		NSDS_SI(kSDS_CI_int1_soundCnt_i, t_card["no"].asInt(), sound_cnt, false);
		for(int j=1;j<=sound_cnt;j++)
		{
			NSDS_SI(kSDS_CI_int1_soundNumber_int1_i, t_card["no"].asInt(), j, t_card["sound"][j-1].asInt(), false);
		}
		
		NSDS_SI(kSDS_CI_int1_characterNo_i, t_card["no"].asInt(), t_card["characterNo"].asInt(), false);
		
		Json::Value t_silImgInfo = t_card["silImgInfo"];
		NSDS_SB(kSDS_CI_int1_silImgInfoIsSil_b, t_card["no"].asInt(), t_silImgInfo["isSil"].asBool(), false);
		if(t_silImgInfo["isSil"].asBool())
		{
			if(NSDS_GS(kSDS_CI_int1_silImgInfoImg_s, t_card["no"].asInt()) != t_silImgInfo["img"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = t_silImgInfo["size"].asInt();
				t_df.img = t_silImgInfo["img"].asString().c_str();
				t_df.filename = CCSTR_CWF("card%d_invisible.png", t_card["no"].asInt())->getCString();
				t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
				df_list.push_back(t_df);
				// ================================
			}
		}
	}
	
	NSDS_SI(stage_number, kSDS_SI_level_i, result_data["level"].asInt(), false);
	
	NSDS_SS(stage_number, kSDS_SI_boss_s, result_data["boss"].asString(), false);
	NSDS_SS(stage_number, kSDS_SI_junior_s, result_data["junior"].asString(), false);
	NSDS_SI(stage_number, kSDS_SI_autoBalanceTry_i, result_data["autoBalanceTry"].asInt(), false);
	
	if(df_list.size() > 0) // need download
	{
		mySDS->fFlush(stage_number, kSDS_SI_scale_d);
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		download_version = result_data["version"].asInt();
		ing_download_cnt = 1;
		ing_download_per = 0;
//		download_state = CCLabelBMFont::create(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString(), "allfont.fnt");
//		download_state->setPosition(ccp(240,130));
//		addChild(download_state, kSID_Z_content);
		startDownload();
	}
	else
	{
		SDS_SI(kSDF_stageInfo, stage_number, "version", result_data["version"].asInt(), false);
		mySDS->fFlush(stage_number, kSDS_SI_scale_d);
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		successGetStageInfo();
	}
}

void EndlessModeOpening::startDownload()
{
	CCLOG("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
	
	mySIL->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename, this, callfunc_selector(EndlessModeOpening::successAction), this, callfunc_selector(EndlessModeOpening::failAction));
	
//	schedule(schedule_selector(StageInfoDown::downloadingAction));
}

void EndlessModeOpening::successAction()
{
	SDS_SS(kSDF_cardInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img, false);
	
	if(ing_download_cnt >= df_list.size())
	{
		for(int i=0;i<cf_list.size();i++)
		{
			CCSprite* target_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].from_filename.c_str()));
			target_img->setAnchorPoint(ccp(0,0));
			
			if(cf_list[i].is_ani)
			{
				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].ani_filename.c_str()), CCRectMake(0, 0, cf_list[i].cut_width, cf_list[i].cut_height));
				ani_img->setPosition(ccp(cf_list[i].position_x, cf_list[i].position_y));
				target_img->addChild(ani_img);
			}
			
			target_img->setScale(0.2f);
			
			CCRenderTexture* t_texture = CCRenderTexture::create(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY());
			t_texture->setSprite(target_img);
			t_texture->begin();
			t_texture->getSprite()->visit();
			t_texture->end();
			
			t_texture->saveToFile(cf_list[i].to_filename.c_str(), kCCImageFormatPNG);
		}
		
		NSDS_SI(stage_number, kSDS_SI_version_i, download_version, false);
		mySDS->fFlush(stage_number, kSDS_SI_scale_d);
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
//		download_state->setString(CCSTR_CWF("%.0f        %d  %d", 1.f*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		
		successGetStageInfo();
	}
	else
	{
		ing_download_cnt++;
		ing_download_per = 0.f;
//		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		startDownload();
	}
}

void EndlessModeOpening::failAction()
{
	addChild(ASPopupView::getCommonNoti(-99999, myLoc->getLocalForKey(kMyLocalKey_endlessDownloadFail), [=]()
							   {
								   startDownload();
							   }));
}

void EndlessModeOpening::successGetStageInfo()
{
	ready_loading->removeFromParent();
	ready_loading = NULL;
	
	if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
	{
		tutorial_success_func();
	}
	
	RivalSelectPopup* t_popup = RivalSelectPopup::create(touch_priority-100, [=](){is_menu_enable = true;}, [=]()
														 {
															 addChild(KSGradualValue<float>::create(1.f, 0.f, 0.2f, [=](float t)
																									{
																										gray->setOpacity(255*t);
																									}, [=](float t)
																									{
																										gray->setOpacity(0);
																										mySGD->is_endless_mode = true;
//																										mySGD->endless_my_victory = 0;
																										mySD->setSilType(stage_number);
																										
																										StartSettingPopup* t_popup = StartSettingPopup::create();
																										t_popup->setHideFinalAction(getParent(), callfunc_selector(MainFlowScene::showEndlessOpening));
																										getParent()->addChild(t_popup, kMainFlowZorder_popup);
																										removeFromParent();
																									}));
															 
															 CommonAnimation::closePopup(this, main_case, nullptr, [=](){
																 
															 }, [=](){
//																 end_func(); removeFromParent();
															 });
//															 addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
//																 addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
//															 
//															 addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){
//																 KS::setOpacity(main_case, t);
//																 //		mySelection->setOpacity(0);
//															 }, [=](int t){
//																 KS::setOpacity(main_case, 0);
//																 //		mySelection->setOpacity(0);
//															 }));
														 });
	addChild(t_popup, 999);
//	
//	addChild(KSGradualValue<float>::create(1.f, 0.f, 0.2f, [=](float t)
//										   {
//											   gray->setOpacity(255*t);
//										   }, [=](float t)
//										   {
//											   gray->setOpacity(0);
//											   mySGD->is_endless_mode = true;
//											   mySGD->endless_my_victory = 0;
//											   mySD->setSilType(stage_number);
//											   
//											   StartSettingPopup* t_popup = StartSettingPopup::create();
//											   t_popup->setHideFinalAction(getParent(), callfunc_selector(MainFlowScene::showEndlessOpening));
//											   getParent()->addChild(t_popup, kMainFlowZorder_popup);
//											   removeFromParent();
//										   }));
//	
//	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
//		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
//	
//	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){
//		KS::setOpacity(main_case, t);
////		mySelection->setOpacity(0);
//	}, [=](int t){
//		KS::setOpacity(main_case, 0);
////		mySelection->setOpacity(0);
//	}));
}

class RewardTableDelegator : public CCNode, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	Json::Value m_rewardData;
//	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx)
//	{
//		
//	}
	virtual CCSize cellSizeForTable(CCTableView *table)
	{
		return CCSizeMake(171, 42);
	}
	virtual unsigned int numberOfCellsInTableView(CCTableView *table)
	{
		return m_rewardData.size();
	}
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell)
	{
		
	}
	virtual void scrollViewDidScroll(CCScrollView* view)
	{
		
	}
	virtual void scrollViewDidZoom(CCScrollView* view)
	{
		
	}

	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx)
	{
		CCTableViewCell* cell = new CCTableViewCell();
		cell->init();
		cell->autorelease();
		CCLog("idx = %d", idx);
		CCNode* A, *B, *C;
		A = B = C = nullptr;
		
		CCScale9Sprite* rewardBack2 = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
		rewardBack2->setPosition(ccp(0.f, 0.f));
		rewardBack2->setAnchorPoint(ccp(0.f, 0.0f));
		rewardBack2->setContentSize(CCSizeMake(171, 42));
		//			rewardBack2->setPosition(105,175);
		setFormSetter(rewardBack2);
		cell->addChild(rewardBack2);
		
		auto titleHanger = [=, &A](){
			std::string left1 = m_rewardData[idx]["left1"].asString();
			std::string left2 = m_rewardData[idx]["left2"].asString();
			//		left1 = KS::replaceAll(left1, "%", "%");
			//		left2 = KS::replaceAll(left2, "%", "%");
			StyledLabelTTF* reward2Title = StyledLabelTTF::create(
																														ccsf("<font color=#FFAA14 newline=13>%s</font><font color=#FFAA14>%s</font>",
																																 left1.c_str(), left2.c_str()),
																														mySGD->getFont().c_str(), 11,
																														999, StyledAlignment::kCenterAlignment);
			reward2Title->setAnchorPoint(ccp(0.5f, 0.5f));
			A = reward2Title;
			rewardBack2->addChild(reward2Title);
		};
		if(m_rewardData[idx]["left1"].asString() == "TOP")
		{
			int r = m_rewardData[idx]["left2"].asInt();
			if( r == 1 )
			{
				CCSprite* goldImg = CCSprite::create("rank_gold.png");
				A = goldImg;
				rewardBack2->addChild(goldImg);
			}
			else if(r == 2)
			{
				
				CCSprite* goldImg = CCSprite::create("rank_silver.png");
				A = goldImg;
				rewardBack2->addChild(goldImg);
			}
			else if(r == 3)
			{
				CCSprite* goldImg = CCSprite::create("rank_bronze.png");
				A = goldImg;
				rewardBack2->addChild(goldImg);
			}
			
			else
			{
				titleHanger();
			}
		}
		else
		{
			titleHanger();
		}
		
		std::string titleStr = m_rewardData[idx]["title"].asString();
		std::string contentStr = m_rewardData[idx]["content"].asString();
		
		//	contentStr = KS::replaceAll(contentStr, "%", "%");
		StyledLabelTTF* reward2Lbl = StyledLabelTTF::create(
																												ccsf("<font color=#FFAA14 size=11 newline=13>%s</font>"
																														 "<font color=999 size=9>%s</font>",
																														 titleStr.c_str(), contentStr.c_str()),
																												mySGD->getFont().c_str(),
																												15.f, 999, StyledAlignment::kLeftAlignment);
		rewardBack2->addChild(reward2Lbl);
		
		int rubyCount = m_rewardData[idx]["reward"][0]["count"].asInt();
		std::string rubyImgFileName;
		if(rubyCount >= 300)
		{
			rubyImgFileName = "shop_ruby6.png";
		}
		else if(rubyCount >= 200)
		{
			rubyImgFileName = "shop_ruby5.png";
		}
		else if(rubyCount >= 100)
		{
			rubyImgFileName = "shop_ruby4.png";
		}
		else if(rubyCount >= 30)
		{
			rubyImgFileName = "shop_ruby3.png";
		}
		else if(rubyCount >= 5)
		{
			rubyImgFileName = "shop_ruby2.png";
		}
		else
		{
			rubyImgFileName = "shop_ruby1.png";
		}
		
		CCSprite* reward2Img = CCSprite::create(rubyImgFileName.c_str());
		reward2Img->setScale(0.5f);
		B = reward2Lbl;
		rewardBack2->addChild(reward2Img);
		KSLabelTTF* reward2Detail = KSLabelTTF::create(ccsf("x %d", rubyCount), mySGD->getFont().c_str(),
																									 10.f);
		reward2Detail->setScale(2.f);
		reward2Detail->setPosition(ccpFromSize(reward2Img->getContentSize()) / 2.f + ccp(0, -20 + 6));
		C = reward2Img;
		reward2Detail->setColor(ccc3(255, 255, 255));
		reward2Detail->enableOuterStroke(ccc3(0, 0, 0), 1.f);
		reward2Img->addChild(reward2Detail);
		
		if(A)
		{
			A->setPosition(ccp(26.0,19.0 + 2)); 			// dt (26.0,19.0)
		}
		if(B)
		{
			
			B->setPosition(ccp(51.5,32.5)); 			// dt (9.5,-6.0)
		}
		if(C)
		{
			C->setPosition(ccp(148.0,27.5 - 10.5)); 			// dt (-0.5,0.5)
		}
		return cell;
	}
	
};
void EndlessModeOpening::resultGetEndlessRank(Json::Value result_data)
{
	loading_circle->removeFromParent();
	loading_circle = NULL;
	
	GraphDogLib::JsonToLog("getendlessrank", result_data);

	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CommonButton* rewardInfo = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_endlessRewardInfo), 12.f, CCSizeMake(60, 35),
																										CommonButtonYellow, touch_priority-1);
		rewardInfo->getTitleLabel()->setColor(ccc3(37, 15, 0));
		rewardInfo->setPosition(ccp(218.5, 211.5));
		left_back->addChild(rewardInfo);
		setFormSetter(rewardInfo);
		rewardInfo->setFunction([=](CCObject*){
			AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
			ASPopupView* rewardInfoPopup = ASPopupView::createDimmed(touch_priority - 2);
			CCNode* container = CCNode::create();
			
			rewardInfoPopup->setContainerNode(container);
			
			RewardTableDelegator* rtd = new RewardTableDelegator();
			rtd->init();
			rtd->autorelease();
			container->addChild(rtd);
			rtd->m_rewardData = result_data["rewardInfo"];
			auto back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
			auto front = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
			
			back->setContentSize(CCSizeMake(203, 242));
			back->setPosition(ccp(0, 0));
			
			//		front->setContentSize(back->getContentSize() - CCSizeMake(20, 50));
			front->setContentSize(CCSizeMake(187, 197));
//			front->setPosition(ccpFromSize(back->getContentSize()) / 2.f - ccp(0, -15 - 29.5 - 59.0));
			front->setPosition(ccpFromSize(back->getContentSize()) / 2.f + ccp(0, -18.5));
			//		front->setPosition(ccpFromSize(back->getContentSize()) / 2.f);
			
			rtd->addChild(back);
			back->addChild(front);
			
			CommonButton* closeButton = CommonButton::createCloseButton(touch_priority - 2);
			closeButton->setFunction([=](CCObject*){
				AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
				CommonAnimation::closePopup(this, container, rewardInfoPopup->getDimmedSprite(), [=](){
					
				}, [=](){
					rewardInfoPopup->removeFromParent();
				});
			});
			closeButton->setPosition(ccp(179.0, 218.0)); 			// dt (179.0, 218.0)
			back->addChild(closeButton);
			addChild(rewardInfoPopup);
			
			KSLabelTTF* rewardLbl = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessRewardInfo), mySGD->getFont().c_str(), 15.f);
			rewardLbl->setColor(ccc3(255, 170, 20));
			rewardLbl->setPosition(ccp(46.5, 216.0)); 			// dt (46.5, 219.0)
			back->addChild(rewardLbl);
			setFormSetter(closeButton);
			setFormSetter(rewardLbl);
			setFormSetter(front);
			setFormSetter(back);
			
			reward_table = CCTableView::create(rtd, CCSizeMake(187, 180));
			reward_table->setPosition(ccp(7.5, 9.0)); 			// dt (7.5, 9.0)
			
			reward_table->setTouchPriority(touch_priority - 2);
			front->addChild(reward_table);
			reward_table->setDirection(kCCScrollViewDirectionVertical);
			
			//추가시 정렬 기준 설정 kCCTableViewFillTopDown : 아래부분으로 추가됨, kCCTableViewFillBottomUp : 위에서 부터 추가됨.
			reward_table->setVerticalFillOrder(kCCTableViewFillTopDown);
			reward_table->setDelegate(rtd);
			setFormSetter(reward_table);

			CommonAnimation::openPopup(this, container, rewardInfoPopup->getDimmedSprite());
			//		setFormSetter(closeButton);
		});
		rank_list.clear();
		
		Json::Value t_list = result_data["list"];
		int list_size = t_list.size();
		for(int i=0;i<list_size;i++)
		{
			EndlessRankInfo t_rank;
			t_rank.memberID = t_list[i]["memberID"].asInt64();
			t_rank.regWeek = t_list[i]["regWeek"].asInt();
			t_rank.score = t_list[i]["score"].asInt();
			t_rank.nick = t_list[i]["nick"].asString();
			t_rank.level = t_list[i]["level"].asInt();
			t_rank.flag = t_list[i]["flag"].asString();
			t_rank.victory = t_list[i]["victory"].asInt();
			t_rank.regDate = t_list[i]["regDate"].asInt64();
			
			rank_list.push_back(t_rank);
		}
		
		myscore = result_data["myscore"].asInt();
		victory = result_data["victory"].asInt();
		alluser = result_data["alluser"].asInt();
		myrank = result_data["myrank"].asInt();
		remainTime = result_data["remainTime"].asInt();
		mySGD->endless_my_level = result_data["level"].asInt();
		
		if(!result_data["myInfo"].isNull())
		{
			mySGD->endless_my_high_victory = result_data["myInfo"]["victory"].asInt();
			mySGD->endless_my_lose = result_data["myInfo"]["lose"].asInt();
			mySGD->endless_my_high_score = result_data["myInfo"]["score"].asInt();
			mySGD->endless_my_win = result_data["myInfo"]["win"].asInt();
			
			mySGD->endless_my_ing_win = result_data["myInfo"]["ing_win"].asInt();
			mySGD->endless_my_victory = result_data["myInfo"]["ing_win"].asInt();
			
			mySGD->setUserdataEndlessIngWin(mySGD->endless_my_victory.getV());
			
			if(refresh_ing_win_func != nullptr)
			{
				refresh_ing_win_func();
				refresh_ing_win_func = nullptr;
			}
			
			mySGD->endless_my_ing_score = result_data["myInfo"]["ing_score"].asInt();
			
			if(mySGD->endless_my_ing_win.getV() > 0)
			{
				n_ready_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessReadyIngWin), mySGD->endless_my_ing_win.getV()+1)->getCString());
				n_ready_label2->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessReadyIngWin), mySGD->endless_my_ing_win.getV()+1)->getCString());
				s_ready_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessReadyIngWin), mySGD->endless_my_ing_win.getV()+1)->getCString());
				s_ready_label2->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessReadyIngWin), mySGD->endless_my_ing_win.getV()+1)->getCString());
			}
		}
		else
		{
			mySGD->endless_my_high_victory = 0;
			mySGD->endless_my_lose = 0;
			mySGD->endless_my_high_score = 0;
			mySGD->endless_my_win = 0;
		}
		
		int remain_time = remainTime.getV();
		if(remain_time < 60)
//			rest_time_value->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_restTimeSecond), remain_time)->getCString());
			rest_time_value->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_restTimeMinute), 0)->getCString());
		else if(remain_time < 60*60)
			rest_time_value->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_restTimeMinute), remain_time/60)->getCString());
		else if(remain_time < 60*60*24)
			rest_time_value->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_restTimeHour), remain_time/60/60)->getCString());
		else
			rest_time_value->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_restTimeDay), remain_time/60/60/24)->getCString());
		
		rest_time_value->setVisible(true);
		
		rest_back->setContentSize(CCSizeMake(rest_time_title->getContentSize().width + rest_time_value->getContentSize().width + 20 + 5, 22));
		rest_back->setPosition(ccp(left_back->getContentSize().width-60-rest_back->getContentSize().width/2.f - 17.5 + 13.5f,left_back->getContentSize().height-20));
		
		rest_time_title->setPosition(ccp(10, rest_back->getContentSize().height/2.f));
		rest_time_value->setPosition(ccp(rest_back->getContentSize().width-10, rest_back->getContentSize().height/2.f));
		CCRect table_rect = CCRectMake(10, left_back->getContentSize().height/2.f - 150/2.f - 30, 255, 150);
		
//		CCSprite* table_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_rect.size.width, table_rect.size.height));
//		table_back->setOpacity(100);
//		table_back->setAnchorPoint(ccp(0,0));
//		table_back->setPosition(table_rect.origin);
//		left_back->addChild(table_back);
		
		CCScale9Sprite* scroll_back = CCScale9Sprite::create("cardsetting_scroll.png", CCRectMake(0, 0, 7, 13), CCRectMake(3, 6, 1, 1));
		scroll_back->setContentSize(CCSizeMake(7, table_rect.size.height-20));
		scroll_back->setPosition(ccpAdd(table_rect.origin, ccp(table_rect.size.width-23, table_rect.size.height/2.f)));
		left_back->addChild(scroll_back);
		scroll_back->setVisible(false);
		rank_table = CCTableView::create(this, table_rect.size);
		rank_table->setVerticalFillOrder(kCCTableViewFillTopDown);
		rank_table->setPosition(table_rect.origin);
		left_back->addChild(rank_table);
		rank_table->setTouchPriority(touch_priority);
		rank_table->setDelegate(this);
		
		m_scrollBar = ScrollBar::createScrollbar(rank_table, -18, NULL, CCScale9Sprite::create("cardsetting_scrollbutton.png",
																																																					CCRect(0, 0, 12, 33), CCRectMake(5, 5, 3, 20)), touch_priority-1);
		m_scrollBar->setDynamicScrollSize(true);
		m_scrollBar->setVisible(false);
		
		CCScale9Sprite* list_cell_case = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
		list_cell_case->setContentSize(CCSizeMake(235, 37));
		list_cell_case->setPosition(ccp(10+list_cell_case->getContentSize().width/2.f,180));//list_cell_case->getContentSize().height/2.f+5));
		list_cell_case->setTag(kCellCase);
		left_back->addChild(list_cell_case);
		
		mySelection = CommonButton::create(CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 225, 37)), touch_priority);
//		mySelection->setOpacity(0);
		mySelection->setPosition(ccp(15.5+list_cell_case->getContentSize().width/2.f,180));//list_cell_case->getContentSize().height/2.f+5));
		setFormSetter(mySelection);
		mySelection->setFunction([=](CCObject* obj){
			Json::Value param;
			param["endlessData"] = Json::Value();
			param["endlessData"]["victory"] = result_data["myInfo"]["victory"].asInt();
			param["endlessData"]["lose"] = result_data["myInfo"]["lose"].asInt();
			param["endlessData"]["win"] = result_data["myInfo"]["win"].asInt();
			param["endlessData"]["score"] = result_data["myInfo"]["score"].asInt();
			param["rank"] = result_data["myrank"].asInt();
			param["alluser"] = result_data["alluser"].asInt();
			param["level"] = result_data["level"].asInt();
			param["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
			param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
			putInformation(param);
			if(currentSelectedCell)
			{
				currentSelectedCell->removeFromParent();
				currentSelectedCell = nullptr;
				currentSelectedIdx = -1;
			}
			currentSelectedCell = CCScale9Sprite::create("common_select.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
			currentSelectedCell->setPosition(ccp(119.5,16.0));
			//				currentSelectedCell->setPosition(ccp(114.0,15.5)); 			// dt (114.0,15.5)
			currentSelectedCell->setContentSize(CCSizeMake(241.0,46.5)); 			// dt (6.0,9.5)
			//	list_cell_case->setPosition(ccp(10+list_cell_case->getContentSize().width/2.f,180));//list_cell_case->getContentSize().height/2.f+5));
			list_cell_case->addChild(currentSelectedCell);
			setFormSetter(currentSelectedCell);
		});
		left_back->addChild(mySelection);
		CCPoint rank_position = ccp(28 - 2.5,18);
		int my_rank = myrank.getV();
		if(my_rank == 1)
		{
			CCSprite* gold_medal = CCSprite::create("rank_gold.png");
			//gold_medal->setScale(0.75f);
			gold_medal->setPosition(rank_position);
			list_cell_case->addChild(gold_medal);
			
		}
		else if(my_rank == 2)
		{
			CCSprite* silver_medal = CCSprite::create("rank_silver.png");
			//silver_medal->setScale(0.75f);
			silver_medal->setPosition(rank_position);
			list_cell_case->addChild(silver_medal);
		}
		else if(my_rank == 3)
		{
			CCSprite* bronze_medal = CCSprite::create("rank_bronze.png");
			//bronze_medal->setScale(0.75f);
			bronze_medal->setPosition(rank_position);
			list_cell_case->addChild(bronze_medal);
		}
		else
		{
			KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", my_rank)->getCString(), mySGD->getFont().c_str(), 12);
			rank_label->enableOuterStroke(ccBLACK, 1);
			rank_label->setPosition(rank_position);
			list_cell_case->addChild(rank_label);
		}
		
		string flag = myDSH->getStringForKey(kDSH_Key_flag);
		
		CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
		selectedFlagSpr->setPosition(ccp(50 + 13.5 - 4.5, 18));
		selectedFlagSpr->setScale(0.8f);
		list_cell_case->addChild(selectedFlagSpr);
		
		KSLabelTTF* nick_label = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12);
		nick_label->setAnchorPoint(ccp(0,0.5f));
		nick_label->enableOuterStroke(ccc3(50, 25, 0), 1);
		nick_label->setPosition(ccp(79 - 3.5,19));
		list_cell_case->addChild(nick_label);
		
		KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d", mySGD->endless_my_high_score.getV())->getCString()).c_str(), mySGD->getFont().c_str(), 12);
		score_label->setColor(ccc3(255, 255, 255));
		score_label->setAnchorPoint(ccp(1,0.5f));
		score_label->enableOuterStroke(ccc3(50, 25, 0), 1.f);
		score_label->setPosition(ccp(215 + 9.5,18));
		setFormSetter(score_label);
		list_cell_case->addChild(score_label);
		
//		KSLabelTTF* victory_label = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessHighStraightValue), victory.getV())->getCString(), mySGD->getFont().c_str(), 12);
		StyledLabelTTF* victory_label =
				StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(kMyLocalKey_endlessHighStraightValue1), mySGD->endless_my_high_victory.getV()),
															 mySGD->getFont().c_str(), 12, 999, StyledAlignment::kRightAlignment);
		victory_label->setAnchorPoint(ccp(1,0.5f));
//		victory_label->enableOuterStroke(ccc3(50, 25, 0), 1.f);
		victory_label->setPosition(ccp(152 + 12 + 3.0, 18));
		list_cell_case->addChild(victory_label);
		setFormSetter(victory_label);
		///////////////////////////////////
		
		Json::Value param;
		
		param["endlessData"] = Json::Value();
		param["endlessData"]["victory"] = result_data["myInfo"]["victory"].asInt();
		param["endlessData"]["lose"] = result_data["myInfo"]["lose"].asInt();
		param["endlessData"]["win"] = result_data["myInfo"]["win"].asInt();
		param["endlessData"]["score"] = result_data["myInfo"]["score"].asInt();
		param["rank"] = result_data["myrank"].asInt();
		param["alluser"] = result_data["alluser"].asInt();
		param["level"] = result_data["level"].asInt();
		param["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
		param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
//		param["score"] = result_data["myInfo"]["score"].asInt();
//		param["victory"] = result_data["myInfo"]["score"].asInt();
//		param["level"] = result_data["level"].asInt();
//		param["alluser"] = result_data["alluser"].asInt();
//		param["rank"] = result_data["rank"].asInt();
		putInformation(param);
		currentSelectedCell = CCScale9Sprite::create("common_select.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
		currentSelectedCell->setPosition(ccp(119.5,16.0));
		//				currentSelectedCell->setPosition(ccp(114.0,15.5)); 			// dt (114.0,15.5)
		currentSelectedCell->setContentSize(CCSizeMake(241.0,46.5)); 			// dt (6.0,9.5)
		//	list_cell_case->setPosition(ccp(10+list_cell_case->getContentSize().width/2.f,180));//list_cell_case->getContentSize().height/2.f+5));
		list_cell_case->addChild(currentSelectedCell);
//		int win_count = mySGD->endless_my_win.getV();
//		int lose_count = mySGD->endless_my_lose.getV();
//		int win_rate = 100.f*win_count/(win_count + lose_count);
//		
//		record_content->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessInfoScoreValue), win_count, lose_count, win_rate)->getCString());
//		highscore_content->setString(KS::insert_separator(CCString::createWithFormat("%d", mySGD->endless_my_high_score.getV())->getCString()).c_str());
//		straight_content->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessHighStraightValue), mySGD->endless_my_high_victory.getV())->getCString());
//		
//		float rank_percent = 1.f*myrank.getV()/alluser.getV();
//		
//		percent_label->setString(CCString::createWithFormat("%.0f%%", rank_percent*100.f)->getCString());
//		
//		CCMoveTo* t_move = CCMoveTo::create(2.f*(1.f-rank_percent), ccp(17 + 160.f*rank_percent,15.5f));
//		rank_percent_case->runAction(t_move);
//		
//		CCDelayTime* t_delay2 = CCDelayTime::create(1.f);
//		CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 255);
//		CCSequence* t_seq2 = CCSequence::create(t_delay2, t_fade2, NULL);
//		percent_label->runAction(t_seq2);
	}
	else
	{
		
	}
	
	

	
}

unsigned int EndlessModeOpening::numberOfCellsInTableView(CCTableView *table)
{
	return rank_list.size();
}
CCTableViewCell* EndlessModeOpening::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	int my_rank = myrank.getV();
	
	string case_name;
//	if(my_rank == idx+1)
//	{
//		case_name = "mainpopup_pupple1.png";
//	}
//	else
//	{
		case_name = "rank_normal.png";
//	}
	
	CCScale9Sprite* list_cell_case = CCScale9Sprite::create(case_name.c_str(), CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
	list_cell_case->setContentSize(CCSizeMake(235, 37));
	list_cell_case->setPosition(ccp(list_cell_case->getContentSize().width/2.f,list_cell_case->getContentSize().height/2.f-5));
	cell->addChild(list_cell_case);
	
	CCPoint rank_position = ccp(28 - 2.5,18);
	if(idx == 0)
	{
		CCSprite* gold_medal = CCSprite::create("rank_gold.png");
		//gold_medal->setScale(0.75f);
		gold_medal->setPosition(rank_position);
		list_cell_case->addChild(gold_medal);
		setFormSetter(gold_medal);
	}
	else if(idx == 1)
	{
		CCSprite* silver_medal = CCSprite::create("rank_silver.png");
		//silver_medal->setScale(0.75f);
		silver_medal->setPosition(rank_position);
		list_cell_case->addChild(silver_medal);
	}
	else if(idx == 2)
	{
		CCSprite* bronze_medal = CCSprite::create("rank_bronze.png");
		//bronze_medal->setScale(0.75f);
		bronze_medal->setPosition(rank_position);
		list_cell_case->addChild(bronze_medal);
	}
	else
	{
		KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 12);
		rank_label->enableOuterStroke(ccBLACK, 1);
		rank_label->setPosition(rank_position);
		list_cell_case->addChild(rank_label);
	}
	
	string flag = rank_list[idx].flag.getV();
	
	CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
	selectedFlagSpr->setPosition(ccp(50 + 13.5 - 4.5,18));
	selectedFlagSpr->setScale(0.8);
	list_cell_case->addChild(selectedFlagSpr);
	setFormSetter(selectedFlagSpr);
	
//	CCSprite* nick_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 62, 15));
//	nick_back->setAnchorPoint(ccp(0,0.5f));
//	nick_back->setPosition(ccp(79 - 3.5,19));
//	list_cell_case->addChild(nick_back);
	
	KSLabelTTF* nick_label = KSLabelTTF::create(rank_list[idx].nick.getV().c_str(), mySGD->getFont().c_str(), 12, CCSizeMake(62, 15), CCTextAlignment::kCCTextAlignmentLeft);
	nick_label->setAnchorPoint(ccp(0,0.5f));
	nick_label->enableOuterStroke(ccc3(50, 25, 0), 1);
	nick_label->setPosition(ccp(79 - 3.5,19));
	list_cell_case->addChild(nick_label);
	
	setFormSetter(nick_label);
	KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",rank_list[idx].score.getV())->getCString()).c_str(), mySGD->getFont().c_str(), 12);
	score_label->setColor(ccc3(255, 255, 255));
	score_label->setAnchorPoint(ccp(1,0.5f));
	score_label->setPosition(ccp(215 + 9.5,18));
	//score_label->setHorizontalAlignment(kCCTextAlignmentRight);
	//score_label->setPosition(ccp(160,18));
	//score_label->setPosition(ccp(223.5, 18.0));
	list_cell_case->addChild(score_label);
	
	setFormSetter(score_label);
	
	StyledLabelTTF* victory_label =
	StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(kMyLocalKey_endlessHighStraightValue1), rank_list[idx].victory.getV()),
												 mySGD->getFont().c_str(), 12, 999, StyledAlignment::kRightAlignment);
//	KSLabelTTF* victory_label = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessHighStraightValue),rank_list[idx].victory.getV())->getCString(), mySGD->getFont().c_str(), 12);
	victory_label->setAnchorPoint(ccp(1,0.5f));
//	victory_label->enableOuterStroke(ccc3(50, 25, 0), 1.f);
//	victory_label->setPosition(ccp(190,18));
	victory_label->setPosition(ccp(152 + 12 + 3.0,18));

	list_cell_case->addChild(victory_label);
	
	setFormSetter(victory_label);
	if(idx == currentSelectedIdx)
	{
		if(currentSelectedCell)
		{
			currentSelectedCell->removeFromParent();
		}
		currentSelectedCell = CCScale9Sprite::create("common_select.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
		currentSelectedCell->setPosition(ccp(114.0,11.5)); 			// dt (114.0,15.5)
		currentSelectedCell->setContentSize(CCSizeMake(241.0,46.5)); 			// dt (6.0,9.5)
		//	list_cell_case->setPosition(ccp(10+list_cell_case->getContentSize().width/2.f,180));//list_cell_case->getContentSize().height/2.f+5));
		cell->addChild(currentSelectedCell);
	}
	return cell;
}

void EndlessModeOpening::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	Json::Value param;
	int tempIndex = clampf(cell->getIdx(), 0, rank_list.size() - 1);
	param["memberID"] = rank_list[tempIndex].memberID.getV();
	loading_right_circle = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
	loading_right_circle->setPosition(ccp(right_back->getContentSize().width/2.f, right_back->getContentSize().height/2.f));
	right_back->addChild(loading_right_circle);
	myHSP->command("getendlessrankinfo", param, this, [=](Json::Value obj){
		if(obj["result"]["code"].asInt() == GDSUCCESS)
		{
			if(loading_right_circle){
				loading_right_circle->removeFromParent();
				loading_right_circle = nullptr;
			}
			putInformation(obj);
		}
		else
		{
			
		}
	});
	currentSelectedIdx = tempIndex;
	if(currentSelectedCell)
	{
		currentSelectedCell->removeFromParent();
	}
	currentSelectedCell = CCScale9Sprite::create("common_select.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
	currentSelectedCell->setPosition(ccp(114.0 + 5.5,10.5)); 			// dt (114.0,15.5)
	currentSelectedCell->setContentSize(CCSizeMake(241.0,46.5)); 			// dt (6.0,9.5)
//	list_cell_case->setPosition(ccp(10+list_cell_case->getContentSize().width/2.f,180));//list_cell_case->getContentSize().height/2.f+5));
	cell->addChild(currentSelectedCell);
	setFormSetter(currentSelectedCell);
}
void EndlessModeOpening::putInformation(Json::Value info)
{
	KS::KSLog("%", info);

	int win_count = info["endlessData"]["win"].asInt();
	int lose_count = info["endlessData"]["lose"].asInt();
	int win_rate = 0;
	if(win_count + lose_count != 0)
	{
		win_rate = 100.f*win_count/(win_count + lose_count);
	}
	
//	record_content->setAnchorPoint(ccp(1,0.5f));
	
	//////////////////////////////////////////
	if(record_content)
	{
		record_content->removeFromParent();
	}
	record_content = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessInfoScoreValue2), win_count, lose_count, win_rate)->getCString(),
																					mySGD->getFont().c_str(),
																					13.f, 0, StyledAlignment::kRightAlignment);
	
	
	setFormSetter(record_content);
	record_content->setOldAnchorPoint();
	record_content->setPosition(ccp(record_back->getContentSize().width-10, record_back->getContentSize().height/2.f));
	record_back->addChild(record_content);
	setFormSetter(record_back);
//	record_content->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessInfoScoreValue), win_count, lose_count, win_rate)->getCString());
//	highscore_content->setString(KS::insert_separator(CCString::createWithFormat("%d", mySGD->endless_my_high_score.getV())->getCString()).c_str());
	highscore_content->setString(KS::insert_separator(CCString::createWithFormat("%d", info["endlessData"]["score"].asInt())->getCString()).c_str());
//	straight_content->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessHighStraightValue), mySGD->endless_my_high_victory.getV())->getCString());
	straight_content->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessHighStraightValue2),
																												 info["endlessData"]["victory"].asInt())->getCString());
	
	setFormSetter(highscore_content);
	setFormSetter(straight_content);
	int	alluser = info["alluser"].asInt();
	int	myrank = info["rank"].asInt();
	float rank_percent = alluser == 0 ? 1.f : 1.f * myrank/alluser;
	
//	percent_label->setString(CCString::createWithFormat("%.0f%%", rank_percent*100.f)->getCString());
	
	if(animation_node1)
	{
		animation_node1->removeFromParent();
	}
	if(animation_node2)
	{
		animation_node2->removeFromParent();
	}
	addChild(animation_node1 = KSGradualValue<float>::create(current_rank_percent, rank_percent*100.f,
																				 1.f, [=](float t){
																					 percent_label->setString(ccsf("%.0f%%", t));
																				 }, [=](float t){
																					 percent_label->setString(ccsf("%.0f%%", t));
																					 animation_node1 = nullptr;
																				 }));
	setFormSetter(percent_label);
	
	addChild(animation_node2 = KSGradualValue<CCPoint>::create(rank_percent_case->getPosition(),
																														 ccp(17 + 160.f*rank_percent, 15.5f),
																														 1.f,
																														 [=](CCPoint t){
																															 rank_percent_case->setPosition(t);
																														 }, [=](CCPoint t){
																															 rank_percent_case->setPosition(t);
																															 animation_node2 = nullptr;
																														 }));
//	CCMoveTo* t_move = CCMoveTo::create(2.f*(1.f-rank_percent), ccp(17 + 160.f*rank_percent,15.5f));
//	rank_percent_case->runAction(t_move);
	
	CCDelayTime* t_delay2 = CCDelayTime::create(1.f);
	CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 255);
	CCSequence* t_seq2 = CCSequence::create(t_delay2, t_fade2, NULL);
	percent_label->runAction(t_seq2);
	current_rank_percent = rank_percent * 100.f;
	CCNode* flagnick = CCNode::create();
	if(right_flag)
	{
		right_flag->removeFromParent();
	}
	
	right_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(info.get("flag", "kr").asString()).c_str());
	right_flag->setScaleX(0.9); 			// dt -0.1
	right_flag->setScaleY(0.9); 			// dt -0.1
	right_flag->setAnchorPoint(ccp(0, 0.5f));
	flagnick->addChild(right_flag);
	
	setFormSetter(right_flag);
	
	if(right_nick)
	{
		right_nick->removeFromParent();
	}
	right_nick = KSLabelTTF::create(info.get("nick", "").asString().c_str(),
																	mySGD->getFont().c_str(), 15.f);
	right_nick->setAnchorPoint(ccp(0.f, 0.5f));
	
	right_nick->setPosition(ccp(right_flag->getContentSize().width + 10.f, 0 - 1.f));
	float flagNickWidth = right_flag->getContentSize().width + 15.f + right_nick->getContentSize().width;
	
	flagnick->addChild(right_nick);
	flagnick->setPosition(ccpFromSize(right_back->getContentSize()) / 2.f - ccp(flagNickWidth, 0) / 2.f);
	flagnick->setPositionY(73 - 45 + 134);
	right_back->addChild(flagnick);
	setFormSetter(right_nick);
	right_nick->setColor(ccc3(255, 155, 0));
	right_nick->enableOuterStroke(ccc3(0, 0, 0), 1);
}