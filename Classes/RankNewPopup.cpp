//
//  RankNewPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 3. 27..
//
//

#include "RankNewPopup.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "CommonButton.h"
#include "KSLabelTTF.h"
#include "FormSetter.h"
#include "LabelTTFMarquee.h"
#include "MyLocalization.h"
#include "TouchSuctionLayer.h"
#include "FlagSelector.h"
#include "FormSetter.h"
#include "ScrollBar.h"
#include "CommonAnimation.h"
#include "TypingBox.h"
#include "CCMenuLambda.h"
#include "StoryLayer.h"
#include "StageImgLoader.h"

void RankNewPopup::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}



// on "init" you need to initialize your instance
bool RankNewPopup::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	startFormSetter(this);
	//	setFormSetterGuide("_0614_주간누적랭킹1.png");
	is_menu_enable = false;
	
	TouchSuctionLayer* suction = TouchSuctionLayer::create(-180);
	addChild(suction);
	suction->setTouchEnabled(true);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray);
	
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-14.f));
	setFormSetter(main_case);
	addChild(main_case, 1);
	
	
	CCSprite* title_back = CCSprite::create("title_tab.png");
	title_back->setPosition(ccp(60,main_case->getContentSize().height-13));
	main_case->addChild(title_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_weeklyranking), mySGD->getFont().c_str(), 12);
	title_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0, 2));
	title_back->addChild(title_label);
	
//<<<<<<< HEAD
//	CCScale9Sprite* main_inner_left = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
//	
//=======
	
	CommonAnimation::applyShadow(title_label);

	
	CCScale9Sprite* main_inner_left = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));                                                                        

//>>>>>>> origin/standAlone
	main_inner_left->setContentSize(CCSizeMake(298 / 2.f, 408 / 2.f));
	main_inner_left->setPosition(ccp(378.5 + 1.f, 125.5));
	setFormSetter(main_inner_left);
	main_case->addChild(main_inner_left);
	
	CCScale9Sprite* main_inner_right = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	m_main_inner_right = main_inner_right;
	main_inner_right->setContentSize(CCSizeMake(538 / 2.f, 408 / 2.f));
	main_inner_right->setPosition(ccp(160.5 - 1.f, 125.5));
	setFormSetter(main_inner_right);
	main_case->addChild(main_inner_right);
	
	KSLabelTTF* myRankLbl = KSLabelTTF::create("나의 순위", mySGD->getFont().c_str(), 13.f);
	main_inner_right->addChild(myRankLbl);
	myRankLbl->setPosition(ccp(39.0, 190.5));
	setFormSetter(myRankLbl);
	
	
	
	{
		reward_table = CCTableView::create(this, CCSizeMake(187, 175));
		reward_table->setDirection(kCCScrollViewDirectionVertical);
		
		//추가시 정렬 기준 설정 kCCTableViewFillTopDown : 아래부분으로 추가됨, kCCTableViewFillBottomUp : 위에서 부터 추가됨.
		reward_table->setVerticalFillOrder(kCCTableViewFillTopDown);
		
		//		reward_table->setVerticalFillOrder(kCCTableViewFillTopDown);
		reward_table->setDirection(CCScrollViewDirection::kCCScrollViewDirectionVertical);
		//		reward_table->setContentOffset(reward_table->minContainerOffset());
		reward_table->setPosition(ccp(4.0, 6.f));
		reward_table->setDelegate(this);
		CCScale9Sprite* scrollBar = CCScale9Sprite::create("cardsetting_scrollbutton.png",
																											 CCRect(0, 0, 12, 33), CCRectMake(5, 5, 3, 20));
		//	scrollBar->setVisible(false);
		m_rewardScrollBar = ScrollBar::createScrollbar(reward_table, 0, NULL, scrollBar, -191);
		m_rewardScrollBar->setDynamicScrollSize(true);
		m_rewardScrollBar->setVisible(false);
		setFormSetter(reward_table);
		
		
		main_inner_left->addChild(reward_table);
		
		reward_table->setTouchPriority(-184);
		
		KSLabelTTF* rewardTitle = KSLabelTTF::create("보상내용", mySGD->getFont().c_str(), 13.f);
		rewardTitle->setPosition(ccp(40.5f,  189.5f));
		main_inner_left->addChild(rewardTitle);
		setFormSetter(rewardTitle);
		
		
		
		
		
		
		
		//		CCScale9Sprite* rewardBack2 = CCScale9Sprite::create("startsetting_left_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
		//		rewardBack2->setContentSize(CCSizeMake(190,75));
		//		setFormSetter(rewardBack2);
		//		rewardBack2->setPosition(100,42);
		//
		//		rewardBack1->addChild(rewardBack2);
		//
		//		CCSprite* rewardTag= CCSprite::create("rankpopup_top10.png");
		//		rewardTag->setPosition(ccp(45,50));
		//		setFormSetter(rewardTag);
		//
		//		rewardBack2->addChild(rewardTag);
		//
		//		CCSprite* rewardImg= CCSprite::create("rankpopup_top10_ruby.png");
		//		rewardImg->setPosition(ccp(135,45));
		//		setFormSetter(rewardImg);
		//		rewardBack2->addChild(rewardImg);
		
	}
	
	//	{
	//		CCScale9Sprite* rewardBack1 = CCScale9Sprite::create("startsetting_left_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	//		rewardBack1->setContentSize(CCSizeMake(200,85));
	//		rewardBack1->setPosition(105,175-70);
	//		main_inner_left->addChild(rewardBack1);
	//
	//
	//		CCScale9Sprite* rewardBack2 = CCScale9Sprite::create("startsetting_left_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	//		rewardBack2->setContentSize(CCSizeMake(190,75));
	//		rewardBack2->setPosition(100,42);
	//
	//		rewardBack1->addChild(rewardBack2);
	//
	//		CCSprite* rewardTag= CCSprite::create("rankpopup_top30.png");
	//		rewardTag->setPosition(ccp(45,50));
	//		rewardBack2->addChild(rewardTag);
	//
	//		CCSprite* rewardImg= CCSprite::create("rankpopup_top30_gold.png");
	//		rewardImg->setPosition(ccp(135,45));
	//		rewardBack2->addChild(rewardImg);
	//	}
	
	//	{
	//		CCScale9Sprite* rewardBack1 = CCScale9Sprite::create("startsetting_left_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	//		rewardBack1->setContentSize(CCSizeMake(200,85));
	//		rewardBack1->setPosition(105,175-140);
	//		main_inner_left->addChild(rewardBack1);
	//
	//
	//		CCScale9Sprite* rewardBack2 = CCScale9Sprite::create("startsetting_left_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	//		rewardBack2->setContentSize(CCSizeMake(190,75));
	//		rewardBack2->setPosition(100,42);
	//
	//		rewardBack1->addChild(rewardBack2);
	//
	//		CCSprite* rewardTag= CCSprite::create("rankpopup_top50.png");
	//		rewardTag->setPosition(ccp(45,50));
	//		rewardBack2->addChild(rewardTag);
	//
	//		CCSprite* rewardImg= CCSprite::create("rankpopup_top50_gift.png");
	//		rewardImg->setPosition(ccp(135,45));
	//		rewardBack2->addChild(rewardImg);
	//	}
	
	
	rankBack = CCNode::create();
	rankBack->setPosition(ccp(-226.0,-41.5)); 			// dt (-213.0,-11.5)
	setFormSetter(rankBack);
	main_inner_right->addChild(rankBack);
	
	CCScale9Sprite* barBack = CCScale9Sprite::create("cardsetting_scroll.png", CCRectMake(0, 0, 7, 13), CCRectMake(3, 6, 1, 1));
	barBack->setVisible(false);
	barBack->setPosition(ccp(243.0, 76.f));
	barBack->setContentSize(CCSizeMake(5.5,116.5)); 			// dt (-1.5,-38.5)
	//	FormSetter::get()->addObject("testksoo", barBack);
	main_inner_right->addChild(barBack);
	setFormSetter(barBack);
	rank_table = CCTableView::create(this, CCSizeMake(540 / 2.f, 107));
	rank_table->setPosition(ccp(-4.5, 1.5)); 			// dt (-9.5,-1.5)
	rank_table->setDelegate(this);
	rank_table->reloadData();
	CCScale9Sprite* scrollBar = CCScale9Sprite::create("cardsetting_scrollbutton.png",
																										 CCRect(0, 0, 12, 33), CCRectMake(5, 5, 3, 20));
	scrollBar->setVisible(false);
	m_scrollBar = ScrollBar::createScrollbar(rank_table, 0, NULL, scrollBar, -191);
	m_scrollBar->setVisible(false);
	m_scrollBar->setDynamicScrollSize(true);
	setFormSetter(rank_table);
	
	
	main_inner_right->addChild(rank_table);
	
	rank_table->setTouchPriority(-184);
	
	
	
	
	CommonButton* close_menu = CommonButton::createCloseButton(-185);
	close_menu->setPosition(ccpFromSize(main_case->getContentSize()) + ccp(-20,-12));
	close_menu->setFunction([=](CCObject* sender)
													{
														CCNode* t_node = CCNode::create();
														menuAction(t_node);
													});
	setFormSetter(close_menu);
	main_case->addChild(close_menu, 5);
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	loading_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
	loading_img = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
	loading_img->setPosition(ccp(main_inner_right->getContentSize().width/2.f,main_inner_right->getContentSize().height*0.4f));
	main_inner_right->addChild(loading_img);
//	reader->release();
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["start"]=1;
	param["limit"]=50;
	
	hspConnector::get()->command("getweeklyrankbyalluser", param, this, json_selector(this, RankNewPopup::resultGetRank));
	
	if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kUI_rank))
	{
		myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kUI_rank, true);
		
		StoryLayer::startStory(this, "menu_rank", nullptr);
		
//		CCNode* scenario_node = CCNode::create();
//		addChild(scenario_node, 9999);
//		
//		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//		if(screen_scale_x < 1.f)
//			screen_scale_x = 1.f;
//		
//		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//		
//		
//		CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
//		ikaruga->setAnchorPoint(ccp(0,0));
//		ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
//		scenario_node->addChild(ikaruga, 1);
//		
//		TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(225, 50));
//		typing_box->setHide();
//		scenario_node->addChild(typing_box, 2);
//		
//		CCSprite* n_skip = CCSprite::create("kt_skip.png");
//		CCSprite* s_skip = CCSprite::create("kt_skip.png");
//		s_skip->setColor(ccGRAY);
//		
//		CCMenuLambda* skip_menu = CCMenuLambda::create();
//		skip_menu->setPosition(ccp(240-240*screen_scale_x + 35, 160+160*screen_scale_y - 25 + 150));
//		scenario_node->addChild(skip_menu, 3);
//		skip_menu->setTouchPriority(-19999);
//		skip_menu->setEnabled(false);
//		
//		CCMenuItemLambda* skip_item = CCMenuItemSpriteLambda::create(n_skip, s_skip, [=](CCObject* sender)
//																	 {
//																		 skip_menu->setEnabled(false);
//																		 
//																		 addChild(KSTimer::create(0.1f, [=]()
//																								  {
//																									  scenario_node->removeFromParent();
//																								  }));
//																	 });
//		skip_menu->addChild(skip_item);
//		
//		typing_box->showAnimation(0.3f);
//		
//		function<void()> end_func2 = [=]()
//		{
//			skip_menu->setEnabled(false);
//			
//			addChild(KSTimer::create(0.1f, [=]()
//									 {
//										 scenario_node->removeFromParent();
//									 }));
//		};
//		
//		function<void()> end_func1 = [=]()
//		{
//			typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial7), end_func2);
//		};
//		
//		scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
//															  {
//																  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																  skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
//															  }, [=](float t)
//															  {
//																  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																  skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
//																  skip_menu->setEnabled(true);
//																  
//																  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial6), end_func1);
//															  }));
	}
	
	return true;
}

#define kZ_CP_img 1000

void RankNewPopup::resultGetRank(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		//cell_action_list.clear();
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
		
		CCSprite* graph_back = CCSprite::create("rankreward_graph.png");
		graph_back->setPosition(ccp(361.0, 210.0));
		//		graph_back->setPosition(ccp(361.0, 209.5));
		//		graph_back->setPosition(ccpFromSize(m_main_inner_right->getContentSize() / 2.f));
		
		rankBack->addChild(graph_back, kZ_CP_img);
		setFormSetter(graph_back);
		//		KSLabelTTF* t_rank_a = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankA), mySGD->getFont().c_str(), 9);
		//		t_rank_a->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		//		t_rank_a->setPosition(ccp(25,8));
		//		graph_back->addChild(t_rank_a);
		//
		//		KSLabelTTF* t_rank_b = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankB), mySGD->getFont().c_str(), 9);
		//		t_rank_b->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		//		t_rank_b->setPosition(ccp(25+49,8));
		//		graph_back->addChild(t_rank_b);
		//
		//		KSLabelTTF* t_rank_c = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankC), mySGD->getFont().c_str(), 9);
		//		t_rank_c->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		//		t_rank_c->setPosition(ccp(25+98,8));
		//		graph_back->addChild(t_rank_c);
		//
		//		KSLabelTTF* t_rank_d = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankD), mySGD->getFont().c_str(), 9);
		//		t_rank_d->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		//		t_rank_d->setPosition(ccp(25+147,8));
		//		graph_back->addChild(t_rank_d);
		
		int alluser = result_data["alluser"].asInt();
		int lefttime = result_data["remainTime"].asInt();
		int myrank = result_data["myrank"].asInt();
		//		string lefttimestr;
		//		if(lefttime>60*60*24){
		//			lefttimestr=CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_dayAfterReset), lefttime/(60*60*24)+1)->getCString();
		//		}else if(lefttime>60*60){
		//			lefttimestr=CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_hourAfterReset), lefttime/(60*60)+1)->getCString();
		//		}else{
		//			lefttimestr=CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_secondAfterReset), lefttime/60+1)->getCString();
		//		}
		
		CCScale9Sprite* rest_back = CCScale9Sprite::create("common_deepgray.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
		rest_back->setPosition(ccp(440.5, 230.5));
		rest_back->setContentSize(CCSizeMake(194 / 2.f, 40 / 2.f));
		rankBack->addChild(rest_back);
		KSLabelTTF* lefttime_label = KSLabelTTF::create("", mySGD->getFont().c_str(), 10.5f);
		lefttime_label->setPosition(ccp(77.5 + 5.f + 6,11.0  - 2 )); 			// dt (77.5,11.0)
		//lefttime_label->setColor(ccc3(255, 170, 20));
		lefttime_label->setAnchorPoint(ccp(1,0.5));
		rest_back->addChild(lefttime_label); // , kZ_CP_img);
		setFormSetter(rest_back);
		
		KSLabelTTF* rest_time_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_restTime), mySGD->getFont().c_str(), 10.5f);
		rest_time_title->setColor(ccc3(255, 255, 255));
		rest_time_title->setAnchorPoint(ccp(0,0.5f));
		rest_time_title->setPosition(ccp(10, rest_back->getContentSize().height / 2.f));
		rest_back->addChild(rest_time_title);
		setFormSetter(rest_time_title);
		setFormSetter(lefttime_label);
		if(lefttime < 60)
			//			rest_time_value->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_restTimeSecond), remain_time)->getCString());
			lefttime_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_restTimeMinute), 0)->getCString());
		else if(lefttime < 60*60)
			lefttime_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_restTimeMinute), lefttime/60)->getCString());
		else if(lefttime < 60*60*24)
			lefttime_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_restTimeHour), lefttime/60/60)->getCString());
		else
			lefttime_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_restTimeDay), lefttime/60/60/24)->getCString());
		
		
		
		CCScale9Sprite* tipBack = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
		tipBack->setPosition(ccp(main_case->getContentSize().width*0.655f, main_case->getContentSize().height+2-23.5f));
		main_case->addChild(tipBack, 2);
		
		CCSprite* title_back = CCSprite::create("tabbutton_up.png");
		title_back->setPosition(ccp(main_case->getContentSize().width*0.417f, main_case->getContentSize().height+2-25));
		main_case->addChild(title_back, 3);
		
		tipBack->setContentSize(CCSizeMake(278, 26));
		KSLabelTTF* tipLbl = KSLabelTTF::create("TIP", mySGD->getFont().c_str(), 14.f);
		tipLbl->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		
		tipLbl->getTexture()->setAliasTexParameters();
		//tipLbl->setColor(ccc3(255, 155, 0));
		title_back->addChild(tipLbl, 4);
		tipLbl->setPosition(ccpFromSize(title_back->getContentSize()) / 2.f + ccp(0,1));
		
		CCRect mission_size = CCRectMake(0, 0, 225, 22);
		CCPoint mission_position = ccp(main_case->getContentSize().width/2.f+97, main_case->getContentSize().height+2-23.5f);
		LabelTTFMarquee* tipMaquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), mission_size.size.width, mission_size.size.height, "");
		tipMaquee->addText(myLoc->getLocalForKey(LK::kMyLocalKey_rankNewMarquee1));
		tipMaquee->addText(myLoc->getLocalForKey(LK::kMyLocalKey_rankNewMarquee2));
		tipMaquee->addText(myLoc->getLocalForKey(LK::kMyLocalKey_rankNewMarquee3));
		tipMaquee->setPosition(mission_position);
		tipMaquee->startMarquee();
		tipMaquee->setFontSize(12.f);
		tipMaquee->setAnchorPoint(ccp(0.5f, 0.5f));
		main_case->addChild(tipMaquee, 2);
		setFormSetter(tipBack);
		setFormSetter(tipMaquee);
		
		
		
		//		CCSprite* temp_mission = CCSprite::create("whitePaper.png", mission_size);
		//		temp_mission->setOpacity(100);
		//		temp_mission->setPosition(mission_position);
		//		main_case->addChild(temp_mission);
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//		CCLabelTTF* all_user_label = CCLabelTTF::create(CCString::createWithFormat("/%d", alluser)->getCString(), mySGD->getFont().c_str(), 10);
		//		all_user_label->setColor(ccc3(255, 50, 50));
		//		all_user_label->setAnchorPoint(ccp(1,0.5));
		//		all_user_label->setPosition(ccp(main_case->getContentSize().width-30, 210));
		//		rankBack->addChild(all_user_label, kZ_CP_img);
		//
		//		CCLabelTTF* my_rank_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_myrankValue), myrank)->getCString(), mySGD->getFont().c_str(), 10);
		//		my_rank_label->setAnchorPoint(ccp(1,0.5));
		//		my_rank_label->setPosition(ccp(all_user_label->getPositionX()-all_user_label->getContentSize().width, all_user_label->getPositionY()));
		//		rankBack->addChild(my_rank_label, kZ_CP_img);
		
		float rank_percent = alluser == 0 ? 1.f : 1.f * myrank/alluser;
		
		CCSprite* rank_percent_case = CCSprite::create("gameresult_rank_percent.png");
		rank_percent_case->setAnchorPoint(ccp(0.5,1));
		rank_percent_case->setPosition(ccp(477.f, 210.5f));
		
		rankBack->addChild(rank_percent_case, kZ_CP_img);
		
		KSLabelTTF* percent_label = KSLabelTTF::create("100%", mySGD->getFont().c_str(), 13);
		addChild(KSGradualValue<float>::create(100.f, rank_percent*100.f,
																					 2.f * (1.f - rank_percent), [=](float t){
																						 percent_label->setString(ccsf("%.0f%%", t));
																					 }, [=](float t){
																						 percent_label->setString(ccsf("%.0f%%", t));
																					 }));
		percent_label->enableOuterStroke(ccc3(50, 25, 0), 1);
		percent_label->setPosition(ccp(rank_percent_case->getContentSize().width/2.f+1,
																	 rank_percent_case->getContentSize().height/2.f - 4));
		rank_percent_case->addChild(percent_label, kZ_CP_img);
		setFormSetter(rank_percent_case);
		
		CCMoveTo* t_move = CCMoveTo::create(2.f*(1.f-rank_percent), ccp(244 + (477 - 244)*rank_percent, rank_percent_case->getPositionY()));
		rank_percent_case->runAction(t_move);
		
		rank_data = result_data;
		
		
		cell_action_list.clear();
		
		rank_table->reloadData();
		
		reward_table->reloadData();
		
		
		
		{
			string mynick = myDSH->getStringForKey(kDSH_Key_nick);
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create("mainpopup_purple.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			list_cell_case->setAnchorPoint(ccp(0,0));
			list_cell_case->setContentSize(CCSizeMake(260, 31));
			list_cell_case->setPosition(ccp(231.0, 150.5f));
			rankBack->addChild(list_cell_case, kZ_CP_img);
			
			CCPoint rank_position = ccp(20,18 - 2.f);
			
			int i = rank_data.get("myrank", 999).asInt()-1;
			
			if(i == 0)
			{
				CCSprite* gold_medal = CCSprite::create("rank_gold.png");
				gold_medal->setPosition(rank_position);
				list_cell_case->addChild(gold_medal);
			}
			else if(i == 1)
			{
				CCSprite* silver_medal = CCSprite::create("rank_silver.png");
				silver_medal->setPosition(rank_position);
				list_cell_case->addChild(silver_medal);
			}
			else if(i == 2)
			{
				CCSprite* bronze_medal = CCSprite::create("rank_bronze.png");
				bronze_medal->setPosition(rank_position);
				list_cell_case->addChild(bronze_medal);
			}
			else
			{
				KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 10);
				rank_label->enableOuterStroke(ccBLACK, 1);
				rank_label->setPosition(ccp(37.75f,rank_position.y+7.5f));
				list_cell_case->addChild(rank_label);
			}
			
			string flag = myDSH->getStringForKey(kDSH_Key_flag);
			CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
			selectedFlagSpr->setScale(0.8f);
			if(i >= 3)
				selectedFlagSpr->setPosition(ccp(37.75f,11.5f));
			else
				selectedFlagSpr->setPosition(ccp(50,16));
			list_cell_case->addChild(selectedFlagSpr);
			
			
			CCLabelTTF* t_nick_size = CCLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12);
			if(t_nick_size->getContentSize().width > 80)
			{
				LabelTTFMarquee* nick_marquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 80, 15, "");
				nick_marquee->setSpace(30);
				nick_marquee->addText(("<font color=#352990 strokesize=0.3 strokecolor=#321900 strokeopacity=50>"+mynick+"</font>").c_str());
				nick_marquee->startMarquee();
				nick_marquee->setFontSize(12.f);
				nick_marquee->setAnchorPoint(ccp(0,0.5f));
				nick_marquee->setPosition(ccp(70-3.5f,15));
				list_cell_case->addChild(nick_marquee);
			}
			else
			{
				KSLabelTTF* nick_label = KSLabelTTF::create(mynick.c_str(), mySGD->getFont().c_str(), 12); // user_list[i]["nick"].asString().c_str()
				nick_label->setAnchorPoint(ccp(0,0.5f));
				nick_label->enableOuterStroke(ccc3(50, 25, 0), 0.3f, 50, true);
				nick_label->setPosition(ccp(70-3.5f,15));
				list_cell_case->addChild(nick_label);
			}
			
//			CCNode* character_node = CCNode::create();
//			character_node->setScale(0.8f);
//			character_node->setPosition(ccp(238, 2));
//			list_cell_case->addChild(character_node);
//			
//			auto character_ccb = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, mySGD->getSelectedCharacterHistory().characterIndex.getV()) + ".ccbi");
//			CCSprite* character_img = character_ccb.first;
//			character_img->setPosition(ccp(0,0));
//			character_node->addChild(character_img);
//			
//			character_ccb.second->runAnimationsForSequenceNamed("move_down");
			
			KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",rank_data["myscore"].asInt())->getCString()).c_str(), mySGD->getFont().c_str(), 12);
			score_label->setColor(ccc3(39, 6, 132));
			score_label->setAnchorPoint(ccp(1,0.5f));
			score_label->disableOuterStroke();
			score_label->setPosition(ccp(215 + 20.f + 15, 15));
			list_cell_case->addChild(score_label);
			
			if(rankBack->getTag()!=1){
				CCPoint original_position = list_cell_case->getPosition();
				list_cell_case->setPosition(ccpAdd(original_position, ccp(-500, 0)));
				cell_action_list.push_back([=](){
					delay_index = 0;
					CCDelayTime* t_delay = CCDelayTime::create(delay_index*0.2f);
					CCMoveTo* t_move = CCMoveTo::create(0.5f, original_position);
					CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
					list_cell_case->runAction(t_seq);
					rankBack->setTag(1);
					setFormSetter(list_cell_case);
				});
			}
			
			for(int i=0;i<cell_action_list.size();i++)
				cell_action_list[i]();
		}
	}
	else
	{
		CCLabelTTF* fail_label = CCLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_failCheckRanking), mySGD->getFont().c_str(), 12);
		fail_label->setPosition(loading_img->getPosition());
		rankBack->addChild(fail_label, kZ_CP_img);
	}
	
	//	for(int i=0;i<cell_action_list.size();i++)
	//		cell_action_list[i]();
	
	loading_img->removeFromParent();
}


RankNewPopup::~RankNewPopup()
{
	hspConnector::get()->removeTarget(this);
}

void RankNewPopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
}

void RankNewPopup::showPopup()
{
	
	CommonAnimation::openPopup(this, main_case, gray, [=](){
		
	}, bind(&RankNewPopup::endShowPopup, this));
}

void RankNewPopup::endShowPopup()
{
	is_menu_enable = true;
}

void RankNewPopup::hidePopup()
{
	is_menu_enable = false;
	
	
	CommonAnimation::closePopup(this, main_case, gray, [=](){
		
	}, bind(&RankNewPopup::endHidePopup, this));
}

void RankNewPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void RankNewPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	hidePopup();
}


CCTableViewCell* RankNewPopup::rankTableCellAtIndex(CCTableView *table, unsigned int idx)
{
	
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	int alluser = rank_data["alluser"].asInt();
	int myrank = rank_data["myrank"].asInt();
	
	Json::Value user_list = rank_data["list"];
	
	delay_index = 0;
	int limit_count = 3;
	int i = rank_data["list"].size()-idx-1;
	
	string case_name;
	if(myrank == i+1)
	{
		case_name = "mainpopup_purple.png";
		limit_count++;
	}
	else
	{
		case_name = "rank_normal2.png";
	}
	
	CCScale9Sprite* list_cell_case = CCScale9Sprite::create(case_name.c_str(), CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
	list_cell_case->setAnchorPoint(ccp(0,0));
	list_cell_case->setContentSize(CCSizeMake(260, 31));
	list_cell_case->setPosition(ccp(10,0));
	cell->addChild(list_cell_case, kZ_CP_img);
	
	CCPoint rank_position = ccp(20, 15);
	if(i == 0)
	{
		CCSprite* gold_medal = CCSprite::create("rank_gold.png");
		//gold_medal->setScale(0.75f);
		gold_medal->setPosition(rank_position);
		list_cell_case->addChild(gold_medal);
	}
	else if(i == 1)
	{
		CCSprite* silver_medal = CCSprite::create("rank_silver.png");
		//silver_medal->setScale(0.75f);
		silver_medal->setPosition(rank_position);
		list_cell_case->addChild(silver_medal);
	}
	else if(i == 2)
	{
		CCSprite* bronze_medal = CCSprite::create("rank_bronze.png");
		//bronze_medal->setScale(0.75f);
		bronze_medal->setPosition(rank_position);
		list_cell_case->addChild(bronze_medal);
	}
	else
	{
		KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 14);
		rank_label->enableOuterStroke(ccBLACK, 1);
		rank_label->setPosition(rank_position);//ccp(45.75f,rank_position.y+8));
		list_cell_case->addChild(rank_label);
		setFormSetter(rank_label);
	}
	
	Json::Reader reader;
	Json::Value read_data;
	reader.parse(user_list[i].get("data", Json::Value()).asString(), read_data);
	
	if(myrank != i+1)
	{
		int character_number = read_data.get("character", 1).asInt();
		int character_count = NSDS_GI(kSDS_GI_characterCount_i);
		int found_index = -1;
		for(int i=0;found_index == -1 && i<character_count;i++)
		{
			if(NSDS_GI(kSDS_GI_characterInfo_int1_no_i, i+1) == character_number)
			{
				found_index = i+1;
			}
		}
		
		if(found_index != -1)
		{
			CCNode* character_node = CCNode::create();
			character_node->setScale(0.8f);
			character_node->setPosition(ccp(50, 6));
			list_cell_case->addChild(character_node);
			
			auto character_ccb = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, found_index) + ".ccbi");
			CCSprite* character_img = character_ccb.first;
			character_img->setPosition(ccp(0,0));
			character_node->addChild(character_img);
			
			character_ccb.second->runAnimationsForSequenceNamed("move_down");
		}
	}
	
	string flag = user_list[i].get("flag", "kr").asString().c_str();
	
	CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
	if(myrank == i+1)
	{
		selectedFlagSpr->setPosition(ccp(50, 16));
		selectedFlagSpr->setScale(0.8f);
	}
	else
	{
		selectedFlagSpr->setPosition(ccp(50, 7));
		selectedFlagSpr->setScale(0.5f);
	}
	list_cell_case->addChild(selectedFlagSpr);
	setFormSetter(selectedFlagSpr);
	
	CCLabelTTF* t_nick_size = CCLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12);
	if(t_nick_size->getContentSize().width > 80)
	{
		LabelTTFMarquee* nick_marquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 80, 15, "");
		nick_marquee->setSpace(30);
		nick_marquee->addText(("<font color=#352990 strokesize=0.3 strokecolor=#321900 strokeopacity=50>"+user_list[i].get("nick", Json::Value()).asString()+"</font>").c_str());
		nick_marquee->startMarquee();
		nick_marquee->setFontSize(12.f);
		nick_marquee->setAnchorPoint(ccp(0,0.5f));
		nick_marquee->setPosition(ccp(70-3.5f,15));
		list_cell_case->addChild(nick_marquee);
	}
	else
	{
		KSLabelTTF* nick_label = KSLabelTTF::create(user_list[i].get("nick", Json::Value()).asString().c_str(), mySGD->getFont().c_str(), 12); // user_list[i]["nick"].asString().c_str()
		nick_label->setAnchorPoint(ccp(0,0.5f));
		nick_label->enableOuterStroke(ccc3(50, 25, 0), 0.3f, 50, true);
		nick_label->setPosition(ccp(70-3.5f,15));
		list_cell_case->addChild(nick_label);
		setFormSetter(nick_label);
	}
	
	KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",user_list[i]["score"].asInt())->getCString()).c_str(), mySGD->getFont().c_str(), 12);
	score_label->setColor(ccc3(39, 6, 132));
	score_label->setAnchorPoint(ccp(1,0.5f));
	score_label->disableOuterStroke();
	score_label->setPosition(ccp(215 + 35,15));
	list_cell_case->addChild(score_label);
	
	if(rankBack->getTag()!=1){
		CCPoint original_position = list_cell_case->getPosition();
		list_cell_case->setPosition(ccpAdd(original_position, ccp(-500, 0)));
		cell_action_list.push_back([=](){
			delay_index = i + 1;
			CCDelayTime* t_delay = CCDelayTime::create(delay_index*0.2f);
			CCMoveTo* t_move = CCMoveTo::create(0.5f, original_position);
			CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
			list_cell_case->runAction(t_seq);
			rankBack->setTag(1);
		});
	}
	
	return cell;
}
CCTableViewCell* RankNewPopup::rewardTableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	CCLog("idx = %d", idx);
	CCNode* A, *B, *C;
	A = B = C = nullptr;
	
	CCScale9Sprite* rewardBack2 = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	rewardBack2->setPosition(ccp(0.f, 0.f));
	rewardBack2->setAnchorPoint(ccp(0.f, 0.0f));
	rewardBack2->setContentSize(CCSizeMake(278 / 2.f, 68 / 2.f));
	//			rewardBack2->setPosition(105,175);
	setFormSetter(rewardBack2);
	cell->addChild(rewardBack2);
	
	auto titleHanger = [=, &A](){
		std::string left1 = rank_data["rewardInfo"][idx]["left1"].asString();
		std::string left2 = rank_data["rewardInfo"][idx]["left2"].asString();
		//		left1 = KS::replaceAll(left1, "%", "%");
		//		left2 = KS::replaceAll(left2, "%", "%");
		StyledLabelTTF* reward2Title = StyledLabelTTF::create(
																													ccsf("<font color=#FFAA14 newline=11>%s</font><font color=#FFAA14>%s</font>",
																															 left1.c_str(), left2.c_str()),
																													mySGD->getFont().c_str(), 8,
																													999, StyledAlignment::kCenterAlignment);
		reward2Title->setAnchorPoint(ccp(0.5f, 0.5f));
		
		A = reward2Title;
		rewardBack2->addChild(reward2Title);
		setFormSetter(reward2Title);
	};
	if(rank_data["rewardInfo"][idx]["left1"].asString() == "TOP")
	{
		int r = rank_data["rewardInfo"][idx]["left2"].asInt();
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
	
	std::string titleStr = rank_data["rewardInfo"][idx]["title"].asString();
	std::string contentStr = rank_data["rewardInfo"][idx]["content"].asString();
	
	//	contentStr = KS::replaceAll(contentStr, "%", "%");
	StyledLabelTTF* reward2Lbl = StyledLabelTTF::create(
																											ccsf("<font color=#FFAA14 size=11 newline=13>%s</font>"
																													 "<font color=#FFFFFF size=9 strokesize=0.5 strokeopacity=150 strokecolor=#000000>%s</font>",
																													 titleStr.c_str(), contentStr.c_str()),
																											mySGD->getFont().c_str(),
																											15.f, 999, StyledAlignment::kLeftAlignment);
	rewardBack2->addChild(reward2Lbl);
	int rubyCount = rank_data["rewardInfo"][idx]["reward"][0]["count"].asInt();
	std::string rType = rank_data["rewardInfo"][idx]["reward"][0]["type"].asString();
	std::string rCnt =ccsf("x %d", rubyCount);
	std::string rubyImgFileName;
	float rScale= 1.f;
	if(rType=="r" || rType=="fr" || rType=="pr"){
		rScale= 0.5f;
		if(rubyCount >= 300)rubyImgFileName = "shop_ruby6.png";
		else if(rubyCount >= 200)rubyImgFileName = "shop_ruby5.png";
		else if(rubyCount >= 100)rubyImgFileName = "shop_ruby4.png";
		else if(rubyCount >= 30)rubyImgFileName = "shop_ruby3.png";
		else if(rubyCount >= 5)rubyImgFileName = "shop_ruby2.png";
		else if(rubyCount >= 1)rubyImgFileName = "shop_ruby1.png";
	}else if(rType=="g"){
		rScale= 0.5f;
		if(rubyCount >= 10000)rubyImgFileName = "shop_gold6.png";
		else if(rubyCount >= 5000)rubyImgFileName = "shop_gold5.png";
		else if(rubyCount >= 1000)rubyImgFileName = "shop_gold4.png";
		else if(rubyCount >= 500)rubyImgFileName = "shop_gold3.png";
		else if(rubyCount >= 200)rubyImgFileName = "shop_gold2.png";
		else if(rubyCount >= 1)rubyImgFileName = "shop_gold1.png";
	}else if(rType=="p1"){
		rScale= 0.5f;
		if(rubyCount >= 100)rubyImgFileName = "shop_p1_6.png";
		else if(rubyCount >= 50)rubyImgFileName = "shop_p1_5.png";
		else if(rubyCount >= 30)rubyImgFileName = "shop_p1_4.png";
		else if(rubyCount >= 10)rubyImgFileName = "shop_p1_3.png";
		else if(rubyCount >= 5)rubyImgFileName = "shop_p1_2.png";
		else if(rubyCount >= 1)rubyImgFileName = "shop_p1_1.png";
	}else if(rType=="h"){
		rScale= 0.5f;
		if(rubyCount >= 300)rubyImgFileName = "shop_coin6.png";
		else if(rubyCount >= 200)rubyImgFileName = "shop_coin5.png";
		else if(rubyCount >= 100)rubyImgFileName = "shop_coin4.png";
		else if(rubyCount >= 30)rubyImgFileName = "shop_coin3.png";
		else if(rubyCount >= 5)rubyImgFileName = "shop_coin2.png";
		else if(rubyCount >= 1)rubyImgFileName = "shop_coin1.png";
	}else{
		if(rType=="cd")rCnt=ccsf("No.%d", rubyCount);
		rScale= 1.f;
		rubyImgFileName = CCString::createWithFormat("icon_%s.png",rType.c_str())->getCString();
	}
	
	CCSprite* reward2Img = CCSprite::create(rubyImgFileName.c_str());
	reward2Img->setScale(rScale);
	B = reward2Lbl;
	setFormSetter(reward2Lbl);
	rewardBack2->addChild(reward2Img);
	KSLabelTTF* reward2Detail = KSLabelTTF::create(rCnt.c_str(), mySGD->getFont().c_str(), 10.f);
	reward2Detail->setScale(1/rScale);
	reward2Detail->setPosition(ccpFromSize(reward2Img->getContentSize()) / 2.f + ccp(0, -20 + 8));
	setFormSetter(reward2Img);
	C = reward2Img;
	reward2Detail->setColor(ccc3(255, 255, 255));
	reward2Detail->enableOuterStroke(ccc3(0, 0, 0), 1.f);
	reward2Img->addChild(reward2Detail);
	
	if(A)
	{
		A->setPosition(ccp(18.5, 18.f));
	}
	if(B)
	{
		B->setPosition(ccp(36.5, 29.5)); 				// dt (9.5,-6.0)
	}
	if(C)
	{
		C->setPosition(ccp(119, 12));
	}
	return cell;
}
CCTableViewCell* RankNewPopup::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	if(table == rank_table)
	{
		return rankTableCellAtIndex(table, idx);
	}
	else if(table == reward_table)
	{
		return rewardTableCellAtIndex(table, idx);
	}
	
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	return cell;
}



void  RankNewPopup::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	
}

CCSize  RankNewPopup::cellSizeForTable(CCTableView *table)
{
	if(table == rank_table)
		return CCSizeMake(230, 33);
	else if(table == reward_table)
		return CCSizeMake(171, 37);
	
	return CCSizeMake(230, 34);
}
unsigned int  RankNewPopup::numberOfCellsInTableView(CCTableView *table)
{
	if(table == rank_table)
	{
		Json::Value user_list = rank_data["list"];
		return user_list.size();
	}
	else if(table == reward_table)
	{
		return rank_data["rewardInfo"].size();
	}
	else
	{
		return 0;
	}
}


void RankNewPopup::scrollViewDidScroll(CCScrollView* view)
{
	if(m_scrollBar)
	{
		m_scrollBar->setBarRefresh();
	}
	
	if(m_rewardScrollBar)
	{
		m_rewardScrollBar->setBarRefresh();
	}
}
void RankNewPopup::scrollViewDidZoom(CCScrollView* view){}
