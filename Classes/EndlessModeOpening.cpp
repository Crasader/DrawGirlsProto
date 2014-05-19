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

enum EndlessModeOpeningZorder
{
	kEndlessModeOpeningZorder_gray = 0,
	kEndlessModeOpeningZorder_back,
	kEndlessModeOpeningZorder_content
};

bool EndlessModeOpening::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	
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
	main_case->setPosition(ccp(240,160-22.f));
	addChild(main_case, kEndlessModeOpeningZorder_back);
	
	main_case->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(main_case, t);}, [=](int t)
										 {
											 KS::setOpacity(main_case, 255);
											 
											 is_menu_enable = true;
										 }));
	
	KSLabelTTF* title_label = KSLabelTTF::create("무한모드", mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setAnchorPoint(ccp(0,0.5f));
	title_label->setPosition(ccp(17,main_case->getContentSize().height-25));
	main_case->addChild(title_label);
	
	KSLabelTTF* rest_time_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_restTime), mySGD->getFont().c_str(), 11);
	rest_time_title->setColor(ccc3(255, 170, 0));
	rest_time_title->setPosition(ccp(320, main_case->getContentSize().height-25));
	main_case->addChild(rest_time_title);
	
	KSLabelTTF* rest_time_value = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_restTimeHour), mySGD->getFont().c_str(), 11);
	rest_time_value->setPosition(ccp(380, main_case->getContentSize().height-25));
	main_case->addChild(rest_time_value);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(main_case->getContentSize().width-25, main_case->getContentSize().height-25));
	close_button->setFunction([=](CCObject* sender)
							  {
								  is_menu_enable = false;
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
								  
								  addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
									  addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
								  
								  addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(main_case, t);}, [=](int t){KS::setOpacity(main_case, 0);}));
							  });
	main_case->addChild(close_button);
	
	CCScale9Sprite* left_back = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
	left_back->setContentSize(CCSizeMake(210, 232));
	left_back->setPosition(ccp(10+left_back->getContentSize().width/2.f, main_case->getContentSize().height*0.44f));
	main_case->addChild(left_back);
	
	KSLabelTTF* left_title = KSLabelTTF::create("내 정보", mySGD->getFont().c_str(), 18);
	left_title->setColor(ccc3(255, 170, 0));
	left_title->setPosition(ccp(left_back->getContentSize().width/2.f, left_back->getContentSize().height-19));
	left_back->addChild(left_title);
	
	CCScale9Sprite* graph_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	graph_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 66));
	graph_back->setPosition(ccp(left_back->getContentSize().width/2.f, left_back->getContentSize().height/2.f+52));
	left_back->addChild(graph_back);
	
	CCSprite* graph_img = CCSprite::create("ending_graph.png");
	graph_img->setScaleX(0.85f);
	graph_img->setPosition(ccp(graph_back->getContentSize().width/2.f, graph_back->getContentSize().height/2.f-5));
	graph_back->addChild(graph_img);
	
	KSLabelTTF* t_rank_a = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankA), mySGD->getFont().c_str(), 9);
	t_rank_a->enableOuterStroke(ccc3(41, 41, 41), 1.f);
	t_rank_a->setPosition(ccp(35,graph_img->getPositionY()));
	graph_back->addChild(t_rank_a);
	
	KSLabelTTF* t_rank_b = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankB), mySGD->getFont().c_str(), 9);
	t_rank_b->enableOuterStroke(ccc3(41, 41, 41), 1.f);
	t_rank_b->setPosition(ccp(35+40,graph_img->getPositionY()));
	graph_back->addChild(t_rank_b);
	
	KSLabelTTF* t_rank_c = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankC), mySGD->getFont().c_str(), 9);
	t_rank_c->enableOuterStroke(ccc3(41, 41, 41), 1.f);
	t_rank_c->setPosition(ccp(35+80,graph_img->getPositionY()));
	graph_back->addChild(t_rank_c);
	
	KSLabelTTF* t_rank_d = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankD), mySGD->getFont().c_str(), 9);
	t_rank_d->enableOuterStroke(ccc3(41, 41, 41), 1.f);
	t_rank_d->setPosition(ccp(35+120,graph_img->getPositionY()));
	graph_back->addChild(t_rank_d);
	
	
	CCLabelTTF* all_user_label = CCLabelTTF::create(CCString::createWithFormat("/%d", 9999)->getCString(), mySGD->getFont().c_str(), 10);
	all_user_label->setColor(ccc3(255, 170, 20));
	all_user_label->setAnchorPoint(ccp(1,0.5));
	all_user_label->setPosition(ccp(graph_back->getContentSize().width-10, 13));
	graph_back->addChild(all_user_label);
	
	CCLabelTTF* my_rank_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_myrankValue), 500)->getCString(), mySGD->getFont().c_str(), 10);
	my_rank_label->setAnchorPoint(ccp(1,0.5));
	my_rank_label->setPosition(ccp(all_user_label->getPositionX()-all_user_label->getContentSize().width, all_user_label->getPositionY()));
	graph_back->addChild(my_rank_label);
	my_rank_label->setOpacity(0);
	
	
	float rank_percent = 1.f*500/9999;
	
	CCSprite* rank_percent_case = CCSprite::create("gameresult_rank_percent.png");
	rank_percent_case->setAnchorPoint(ccp(0.5,0));
	rank_percent_case->setPosition(ccp(177-160,28)); // 177
	graph_back->addChild(rank_percent_case);
	
	KSLabelTTF* percent_label = KSLabelTTF::create(CCString::createWithFormat("%.0f%%", rank_percent*100.f)->getCString(), mySGD->getFont().c_str(), 13);
	percent_label->setColor(ccc3(255, 170, 20));
	percent_label->enableOuterStroke(ccc3(50, 25, 0), 1);
	percent_label->setPosition(ccp(rank_percent_case->getContentSize().width/2.f+1, rank_percent_case->getContentSize().height/2.f+2));
	rank_percent_case->addChild(percent_label);
//	percent_label->setOpacity(0);
	
	
//	CCMoveTo* t_move = CCMoveTo::create(2.f*(1.f-rank_percent), ccp(257 + 195.f*rank_percent,230));
//	rank_percent_case->runAction(t_move);
//	
//	CCDelayTime* t_delay1 = CCDelayTime::create(1.f);
//	CCFadeTo* t_fade1 = CCFadeTo::create(0.5f, 255);
//	CCSequence* t_seq1 = CCSequence::create(t_delay1, t_fade1, NULL);
//	my_rank_label->runAction(t_seq1);
//	
//	CCDelayTime* t_delay2 = CCDelayTime::create(1.f);
//	CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 255);
//	CCSequence* t_seq2 = CCSequence::create(t_delay2, t_fade2, NULL);
//	percent_label->runAction(t_seq2);
	
	
	CCScale9Sprite* record_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	record_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 35));
	record_back->setPosition(ccp(left_back->getContentSize().width/2.f, left_back->getContentSize().height/2.f+2));
	left_back->addChild(record_back);
	
	KSLabelTTF* record_title = KSLabelTTF::create("전적", mySGD->getFont().c_str(), 13);
	record_title->setAnchorPoint(ccp(0,0.5f));
	record_title->setPosition(ccp(10, record_back->getContentSize().height/2.f));
	record_back->addChild(record_title);
	
	KSLabelTTF* record_content = KSLabelTTF::create("n승 n패 승률n%", mySGD->getFont().c_str(), 13);
	record_content->setAnchorPoint(ccp(1,0.5f));
	record_content->setPosition(ccp(record_back->getContentSize().width-10, record_back->getContentSize().height/2.f));
	record_back->addChild(record_content);
	
	
	CCScale9Sprite* highscore_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	highscore_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 35));
	highscore_back->setPosition(ccp(left_back->getContentSize().width/2.f, left_back->getContentSize().height/2.f-29));
	left_back->addChild(highscore_back);
	
	
	CCScale9Sprite* straight_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	straight_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 35));
	straight_back->setPosition(ccp(left_back->getContentSize().width/2.f, left_back->getContentSize().height/2.f-60));
	left_back->addChild(straight_back);
	
	
	CCScale9Sprite* combo_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	combo_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 35));
	combo_back->setPosition(ccp(left_back->getContentSize().width/2.f, left_back->getContentSize().height/2.f-91));
	left_back->addChild(combo_back);
	
	
	
	CCScale9Sprite* right_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	right_back->setContentSize(CCSizeMake(250, 232));
	right_back->setPosition(ccp(main_case->getContentSize().width-10-right_back->getContentSize().width/2.f, main_case->getContentSize().height*0.44f));
	main_case->addChild(right_back);
	
	CCSprite* table_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 210, 160));
	table_back->setOpacity(100);
	table_back->setPosition(ccp(10+table_back->getContentSize().width/2.f, right_back->getContentSize().height-10-table_back->getContentSize().height/2.f));
	right_back->addChild(table_back);
	
	CCSprite* n_ready = CCSprite::create("ending_button.png");
	KSLabelTTF* n_ready_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ready), mySGD->getFont().c_str(), 22);
	n_ready_label->setColor(ccc3(50, 30, 5));
	n_ready_label->enableOuterStroke(ccc3(50, 30, 5), 0.25f);
	n_ready_label->setPosition(ccp(n_ready->getContentSize().width/2.f, n_ready->getContentSize().height/2.f-3));
	n_ready->addChild(n_ready_label);
	
	CCSprite* s_ready = CCSprite::create("ending_button.png");
	s_ready->setColor(ccGRAY);
	KSLabelTTF* s_ready_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ready), mySGD->getFont().c_str(), 22);
	s_ready_label->setColor(ccc3(50, 30, 5));
	s_ready_label->enableOuterStroke(ccc3(50, 30, 5), 0.25f);
	s_ready_label->setPosition(ccp(s_ready->getContentSize().width/2.f, s_ready->getContentSize().height/2.f-3));
	s_ready->addChild(s_ready_label);
	
	CCMenuItemLambda* ready_item = CCMenuItemSpriteLambda::create(n_ready, s_ready, [=](CCObject* sender)
																  {
																	  if(!is_menu_enable)
																		  return;
																	  
																	  is_menu_enable = false;
																  });
	
	CCMenuLambda* ready_menu = CCMenuLambda::createWithItem(ready_item);
	ready_menu->setPosition(ccp(right_back->getContentSize().width/2.f,36));
	ready_menu->setTouchPriority(touch_priority);
	right_back->addChild(ready_menu);
}


