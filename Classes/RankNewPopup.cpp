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
	
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-14.f));
	setFormSetter(main_case);
	addChild(main_case, 1);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_weeklyranking), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setPosition(ccp(70,253));
	setFormSetter(title_label);
	main_case->addChild(title_label);
	
	CCScale9Sprite* main_inner_left = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_inner_left->setContentSize(CCSizeMake(187, 234));
	main_inner_left->setPosition(ccp(375.5f,124.0));
	setFormSetter(main_inner_left);
	main_case->addChild(main_inner_left);
	
	CCScale9Sprite* main_inner_right = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_inner_right->setContentSize(CCSizeMake(272,234));
	main_inner_right->setPosition(ccp(144.0,123.5)); 			// dt (0.0,-0.5)
	setFormSetter(main_inner_right);
	main_case->addChild(main_inner_right);
	
	KSLabelTTF* myRankLbl = KSLabelTTF::create("나의 순위", mySGD->getFont().c_str(), 13.f);
	main_inner_right->addChild(myRankLbl);
	myRankLbl->setPosition(ccp(39.0,214.5)); 			// dt (39.0,214.5)
	myRankLbl->setColor(ccc3(255, 170, 20));
	setFormSetter(myRankLbl);
	
	
	
	{
		reward_table = CCTableView::create(this, CCSizeMake(187, 195));
		reward_table->setDirection(kCCScrollViewDirectionVertical);
		
		//추가시 정렬 기준 설정 kCCTableViewFillTopDown : 아래부분으로 추가됨, kCCTableViewFillBottomUp : 위에서 부터 추가됨.
		reward_table->setVerticalFillOrder(kCCTableViewFillTopDown);
		
//		reward_table->setVerticalFillOrder(kCCTableViewFillTopDown);
		reward_table->setDirection(CCScrollViewDirection::kCCScrollViewDirectionVertical);
//		reward_table->setContentOffset(reward_table->minContainerOffset());
		reward_table->setPosition(ccp(7.5,12.5)); 			// dt
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
		
		KSLabelTTF* rewardTitle = KSLabelTTF::create("보상내용", mySGD->getFont().c_str(), 15.f);
		rewardTitle->setPosition(ccp(40.5f, 214.5f));
		rewardTitle->setColor(ccc3(255, 170, 22));
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
	rank_table = CCTableView::create(this, CCSizeMake(268, 118));
	rank_table->setPosition(ccp(-4.5,12.5)); 			// dt (-9.5,-1.5)
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
	close_menu->setPosition(ccp(main_case->getContentSize().width-29,main_case->getContentSize().height-27));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								menuAction(t_node);
							});
	setFormSetter(close_menu);
	main_case->addChild(close_menu);
	
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	loading_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
	loading_img->setPosition(ccp(main_inner_right->getContentSize().width/2.f,main_inner_right->getContentSize().height*0.4f));
	main_inner_right->addChild(loading_img);
	reader->release();
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["start"]=1;
	param["limit"]=50;
	
	hspConnector::get()->command("getweeklyrankbyalluser", param, json_selector(this, RankNewPopup::resultGetRank));
	
    return true;
}

#define kZ_CP_img 1000

void RankNewPopup::resultGetRank(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		//cell_action_list.clear();
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
		
		CCSprite* graph_back = CCSprite::create("ending_graph.png");
		graph_back->setPosition(ccp(359.5,213.5)); 			// dt (19.0,-1.5)
		rankBack->addChild(graph_back, kZ_CP_img);
		setFormSetter(graph_back);
//		KSLabelTTF* t_rank_a = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankA), mySGD->getFont().c_str(), 9);
//		t_rank_a->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_a->setPosition(ccp(25,8));
//		graph_back->addChild(t_rank_a);
//		
//		KSLabelTTF* t_rank_b = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankB), mySGD->getFont().c_str(), 9);
//		t_rank_b->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_b->setPosition(ccp(25+49,8));
//		graph_back->addChild(t_rank_b);
//		
//		KSLabelTTF* t_rank_c = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankC), mySGD->getFont().c_str(), 9);
//		t_rank_c->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_c->setPosition(ccp(25+98,8));
//		graph_back->addChild(t_rank_c);
//		
//		KSLabelTTF* t_rank_d = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankD), mySGD->getFont().c_str(), 9);
//		t_rank_d->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_d->setPosition(ccp(25+147,8));
//		graph_back->addChild(t_rank_d);
		
		int alluser = result_data["alluser"].asInt();
		int lefttime = result_data["remainTime"].asInt();
		int myrank = result_data["myrank"].asInt();
//		string lefttimestr;
//		if(lefttime>60*60*24){
//			lefttimestr=CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_dayAfterReset), lefttime/(60*60*24)+1)->getCString();
//		}else if(lefttime>60*60){
//			lefttimestr=CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_hourAfterReset), lefttime/(60*60)+1)->getCString();
//		}else{
//			lefttimestr=CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_secondAfterReset), lefttime/60+1)->getCString();
//		}

		CCScale9Sprite* rest_back = CCScale9Sprite::create("common_time.png", CCRectMake(0, 0, 22, 22), CCRectMake(10, 10, 2, 2));
		rest_back->setPosition(ccp(436.0,259.5)); 			// dt (9.5,3.5)
		rest_back->setContentSize(CCSizeMake(100, 22));
		rankBack->addChild(rest_back);
		KSLabelTTF* lefttime_label = KSLabelTTF::create("", mySGD->getFont().c_str(), 10);
		lefttime_label->setPosition(ccp(77.5,11.0)); 			// dt (77.5,11.0)
		lefttime_label->setColor(ccc3(255, 170, 20));
		lefttime_label->setAnchorPoint(ccp(1,0.5));
		rest_back->addChild(lefttime_label); // , kZ_CP_img);
		setFormSetter(rest_back);
		
		KSLabelTTF* rest_time_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_restTime), mySGD->getFont().c_str(), 11);
		rest_time_title->setColor(ccc3(255, 170, 0));
		rest_time_title->setAnchorPoint(ccp(0,0.5f));
		rest_time_title->setPosition(ccp(10, rest_back->getContentSize().height / 2.f));
		rest_back->addChild(rest_time_title);
		setFormSetter(rest_time_title);
		setFormSetter(lefttime_label);
		if(lefttime < 60)
			//			rest_time_value->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_restTimeSecond), remain_time)->getCString());
			lefttime_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_restTimeMinute), 0)->getCString());
		else if(lefttime < 60*60)
			lefttime_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_restTimeMinute), lefttime/60)->getCString());
		else if(lefttime < 60*60*24)
			lefttime_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_restTimeHour), lefttime/60/60)->getCString());
		else
			lefttime_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_restTimeDay), lefttime/60/60/24)->getCString());
		
		CCScale9Sprite* tipBack = CCScale9Sprite::create("common_time.png", CCRectMake(0, 0, 22, 22), CCRectMake(10, 10, 2, 2));
		tipBack->setPosition(ccp(173.5,254.5f)); 			// dt (173.5,246.0)
		main_case->addChild(tipBack, 2);
		tipBack->setContentSize(CCSizeMake(65, 25));
		KSLabelTTF* tipLbl = KSLabelTTF::create("Tip", mySGD->getFont().c_str(), 14.f);
		tipLbl->setColor(ccc3(255, 155, 0));
		tipBack->addChild(tipLbl);
		tipLbl->setPosition(ccpFromSize(tipBack->getContentSize()) / 2.f);
		
		LabelTTFMarquee* tipMaquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 210, 25,
																												 "<font size=15>무한모드 점수는 합산되지 않습니다. "
																												 "이번주 플레이 누적 점수입니다. "
																												 "</font>");
		tipMaquee->setPosition(ccp(199.5, 245.f));
		tipMaquee->startMarquee();
		main_case->addChild(tipMaquee, 1);
		setFormSetter(tipBack);
		setFormSetter(tipMaquee);
//		CCLabelTTF* all_user_label = CCLabelTTF::create(CCString::createWithFormat("/%d", alluser)->getCString(), mySGD->getFont().c_str(), 10);
//		all_user_label->setColor(ccc3(255, 50, 50));
//		all_user_label->setAnchorPoint(ccp(1,0.5));
//		all_user_label->setPosition(ccp(main_case->getContentSize().width-30, 210));
//		rankBack->addChild(all_user_label, kZ_CP_img);
//		
//		CCLabelTTF* my_rank_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_myrankValue), myrank)->getCString(), mySGD->getFont().c_str(), 10);
//		my_rank_label->setAnchorPoint(ccp(1,0.5));
//		my_rank_label->setPosition(ccp(all_user_label->getPositionX()-all_user_label->getContentSize().width, all_user_label->getPositionY()));
//		rankBack->addChild(my_rank_label, kZ_CP_img);
		
		float rank_percent = alluser == 0 ? 1.f : 1.f * myrank/alluser;
		
		CCSprite* rank_percent_case = CCSprite::create("gameresult_rank_percent.png");
		rank_percent_case->setAnchorPoint(ccp(0.5,0));
		rank_percent_case->setPosition(ccp(455.5f, 214.5)); 			// dt (-2.5,-15.5)
		
		rankBack->addChild(rank_percent_case, kZ_CP_img);
		
		KSLabelTTF* percent_label = KSLabelTTF::create("100%", mySGD->getFont().c_str(), 13);
		addChild(KSGradualValue<float>::create(100.f, rank_percent*100.f,
																					 2.f * (1.f - rank_percent), [=](float t){
																					 percent_label->setString(ccsf("%.0f%%", t));
		}, [=](float t){
			percent_label->setString(ccsf("%.0f%%", t));
		}));
		percent_label->setColor(ccc3(255, 170, 20));
		percent_label->enableOuterStroke(ccc3(50, 25, 0), 1);
		percent_label->setPosition(ccp(rank_percent_case->getContentSize().width/2.f+1, rank_percent_case->getContentSize().height/2.f+2));
		rank_percent_case->addChild(percent_label, kZ_CP_img);
		setFormSetter(rank_percent_case);

		CCMoveTo* t_move = CCMoveTo::create(2.f*(1.f-rank_percent), ccp(266 + (451 - 266)*rank_percent, 215.5f));
		rank_percent_case->runAction(t_move);
		
		rank_data = result_data;
		
		
		cell_action_list.clear();
		
		rank_table->reloadData();
		
		reward_table->reloadData();
		
		
		
		{
			string mynick = myDSH->getStringForKey(kDSH_Key_nick);
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
			list_cell_case->setAnchorPoint(ccp(0,0));
			list_cell_case->setContentSize(CCSizeMake(260, 37));
			list_cell_case->setPosition(ccp(231.0,168.0));
			rankBack->addChild(list_cell_case, kZ_CP_img);
			
			CCPoint rank_position = ccp(20,18);
			
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
				KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 12);
				rank_label->enableOuterStroke(ccBLACK, 1);
				rank_label->setPosition(rank_position);
				list_cell_case->addChild(rank_label);
			}
			
			string flag = myDSH->getStringForKey(kDSH_Key_flag);
			CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
			selectedFlagSpr->setPosition(ccp(50,18));
			list_cell_case->addChild(selectedFlagSpr);
			
			
			KSLabelTTF* nick_label = KSLabelTTF::create(mynick.c_str(), mySGD->getFont().c_str(), 12); // user_list[i]["nick"].asString().c_str()
			nick_label->setAnchorPoint(ccp(0,0.5f));
			nick_label->enableOuterStroke(ccc3(50, 25, 0), 1);
			nick_label->setPosition(ccp(70,19));
			list_cell_case->addChild(nick_label);
			
			KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",rank_data["myscore"].asInt())->getCString()).c_str(), mySGD->getFont().c_str(), 12);
			score_label->setColor(ccc3(255, 170, 20));
			score_label->setAnchorPoint(ccp(1,0.5f));
			score_label->enableOuterStroke(ccc3(50, 25, 0), 1.f);
			score_label->setPosition(ccp(215,18));
			list_cell_case->addChild(score_label);
			
			if(rankBack->getTag()!=1){
				CCPoint original_position = list_cell_case->getPosition();
				list_cell_case->setPosition(ccpAdd(original_position, ccp(500, 0)));
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
		CCLabelTTF* fail_label = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_failCheckRanking), mySGD->getFont().c_str(), 12);
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
			case_name = "mainpopup_pupple1.png";
			limit_count++;
		}
		else
		{
			case_name = "rank_normal.png";
		}
		
		CCScale9Sprite* list_cell_case = CCScale9Sprite::create(case_name.c_str(), CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
		list_cell_case->setAnchorPoint(ccp(0,0));
		list_cell_case->setContentSize(CCSizeMake(260, 37));
		list_cell_case->setPosition(ccp(10,-3));
		cell->addChild(list_cell_case, kZ_CP_img);
		
		CCPoint rank_position = ccp(20,18);
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
			KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 12);
			rank_label->enableOuterStroke(ccBLACK, 1);
			rank_label->setPosition(rank_position);
			list_cell_case->addChild(rank_label);
		}
		
		Json::Reader reader;
		Json::Value read_data;
		reader.parse(user_list[i].get("data", Json::Value()).asString(), read_data);
		string flag = read_data.get("flag", "kr").asString().c_str();

		CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
		selectedFlagSpr->setPosition(ccp(50,18));
		list_cell_case->addChild(selectedFlagSpr);
	
		KSLabelTTF* nick_label = KSLabelTTF::create(read_data.get("nick", Json::Value()).asString().c_str(), mySGD->getFont().c_str(), 12); // user_list[i]["nick"].asString().c_str()
		nick_label->setAnchorPoint(ccp(0,0.5f));
		nick_label->enableOuterStroke(ccc3(50, 25, 0), 1);
		nick_label->setPosition(ccp(70,19));
		list_cell_case->addChild(nick_label);
		
		KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",user_list[i]["score"].asInt())->getCString()).c_str(), mySGD->getFont().c_str(), 12);
		score_label->setColor(ccc3(255, 170, 20));
		score_label->setAnchorPoint(ccp(1,0.5f));
		score_label->enableOuterStroke(ccc3(50, 25, 0), 1.f);
		score_label->setPosition(ccp(215,18));
		list_cell_case->addChild(score_label);
	
	if(rankBack->getTag()!=1){
		CCPoint original_position = list_cell_case->getPosition();
		list_cell_case->setPosition(ccpAdd(original_position, ccp(500, 0)));
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
	
	CCScale9Sprite* rewardBack2 = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	rewardBack2->setPosition(ccp(0.f, 0.f));
	rewardBack2->setAnchorPoint(ccp(0.f, 0.0f));
	rewardBack2->setContentSize(CCSizeMake(171, 42));
	//			rewardBack2->setPosition(105,175);
	setFormSetter(rewardBack2);
	cell->addChild(rewardBack2);
	
	auto titleHanger = [=, &A](){
		std::string left1 = rank_data["rewardInfo"][idx]["left1"].asString();
		std::string left2 = rank_data["rewardInfo"][idx]["left2"].asString();
//		left1 = KS::replaceAll(left1, "%", "%");
//		left2 = KS::replaceAll(left2, "%", "%");
		StyledLabelTTF* reward2Title = StyledLabelTTF::create(
																													ccsf("<font color=#FFAA14 newline=15>%s</font><font color=#FFAA14>%s</font>",
																															 left1.c_str(), left2.c_str()),
																													mySGD->getFont().c_str(), 15,
																													999, StyledAlignment::kCenterAlignment);
		reward2Title->setAnchorPoint(ccp(0.5f, 0.5f));
		A = reward2Title;
		rewardBack2->addChild(reward2Title);
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
												ccsf("<font color=#FFAA14 size=11 newline=10>%s</font>"
																											"<font color=999 size=9>%s</font>",
														 titleStr.c_str(), contentStr.c_str()),
																											mySGD->getFont().c_str(),
																											15.f, 999, StyledAlignment::kLeftAlignment);
	rewardBack2->addChild(reward2Lbl);
	
	CCSprite* reward2Img = CCSprite::create("mainflow_event.png");
	B = reward2Lbl;
	rewardBack2->addChild(reward2Img);
	KSLabelTTF* reward2Detail = KSLabelTTF::create(ccsf("X %d", rank_data["rewardInfo"][idx]["reward"][0]["count"].asInt()), mySGD->getFont().c_str(), 12.f);
	reward2Detail->setPosition(ccpFromSize(reward2Img->getContentSize()) / 2.f + ccp(0, -20));
	C = reward2Img;
	reward2Detail->setColor(ccc3(255, 170, 22));
	reward2Img->addChild(reward2Detail);

	if(A)
	{
		A->setPosition(ccp(26.0,19.0)); 			// dt (26.0,19.0)
	}
	if(B)
	{
		
		B->setPosition(ccp(51.5,32.5)); 			// dt (9.5,-6.0)
	}
	if(C)
	{
		C->setPosition(ccp(148.0,27.5)); 			// dt (-0.5,0.5)
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
	CCTableViewCell* cell = new CCTableViewCell();
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
		return CCSizeMake(230, 30);
	else if(table == reward_table)
		return CCSizeMake(171, 42);
	
	return CCSizeMake(230, 30);
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
