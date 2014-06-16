#include "EndlessSettingPopup.h"
#include "DataStorageHub.h"
#include "FormSetter.h"
#include "KSLabelTTF.h"
#include "StyledLabelTTF.h"
#include "StarGoldData.h"
#include "MyLocalization.h"
#include "FlagSelector.h"
#include "CommonAnimation.h"

EndlessSettingPopup::EndlessSettingPopup()
{
	
}
EndlessSettingPopup::~EndlessSettingPopup()
{
}
//void EndlessSettingPopup::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool EndlessSettingPopup::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool EndlessSettingPopup::init()
{
	CCLayer::init();
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kEndlessStartSettingPopupZorder_main);
	
	gray->runAction(CCFadeTo::create(0.5f, 255));

	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-22.f));
	addChild(main_case, kEndlessStartSettingPopupZorder_main);
	
	CommonAnimation::openPopup(this, main_case, nullptr, [=](){
		
	}, [=](){
	});
	
	KSLabelTTF* lblTitle = KSLabelTTF::create("PVP", mySGD->getFont().c_str(), 15);
	lblTitle->setPosition(ccp(35, 256));
	lblTitle->setColor(ccc3(241, 155, 0));
	main_case->addChild(lblTitle);
	
	CCScale9Sprite* inner_left = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
	inner_left->setContentSize(CCSizeMake(220, 235));
	inner_left->setPosition(115, 125);
	main_case->addChild(inner_left);
	
	
	CCScale9Sprite* inner_right = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	inner_right->setContentSize(CCSizeMake(250, 235));
	inner_right->setPosition(ccp(347, 125));
	main_case->addChild(inner_right);
	
	
	CCScale9Sprite* star_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	star_back->setContentSize(CCSizeMake(210, 75));
	star_back->setPosition(110, 165);
	inner_left->addChild(star_back);
	
//
//	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
//	main_case->setContentSize(CCSizeMake(480, 280));
//	main_case->setPosition(ccp(240,160-450));
//	addChild(main_case, kAchievePopupZorder_back);
		
//	CCScale9Sprite* right_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
//	right_back->setContentSize(CCSizeMake(310, 232));
//	right_back->setPosition(main_case->getContentSize().width*0.66f, main_case->getContentSize().height*0.44f);
//	main_case->addChild(right_back);
	
	
	KSLabelTTF* lblMyInfo = KSLabelTTF::create("내정보", mySGD->getFont().c_str(), 18.f);
	lblMyInfo->setColor(ccc3(255, 170, 0));
	lblMyInfo->setPosition(ccp(110, 215));
	inner_left->addChild(lblMyInfo);
	
	CommonButton* back_button = CommonButton::createCloseButton(-9999);
	back_button->setPosition(ccp(main_case->getContentSize().width-28, main_case->getContentSize().height+40-65));
	back_button->setFunction([=](CCObject* sender)
													 {
//														 CCNode* t_node = CCNode::create();
//														 t_node->setTag(kStartSettingPopupMenuTag_back);
//														 menuAction(t_node);
													 });
	main_case->addChild(back_button);
	
	CommonButton* readyButton = CommonButton::create("준비하기", CCScale9Sprite::create("endless_ready.png"));
	readyButton->setTitleColor(ccc3(50, 30, 6));
	readyButton->setTitleSize(22.f);
	inner_right->addChild(readyButton, 2);
	FormSetter::get()->addObject("ready", readyButton); // , <#std::function<void (Json::Value)> func#>)
	
	addMyPosition(inner_left); // 내 위치 보여주는거 붙이는 거.
	addRankingTable(inner_right);
	
	return true;
}

void EndlessSettingPopup::addMyPosition(CCNode* main_case)
{
	CCSprite* graph_back = CCSprite::create("ending_graph.png");
	graph_back->setPosition(ccp(110, 155));
	main_case->addChild(graph_back);


	KSLabelTTF* t_rank_a = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankA), mySGD->getFont().c_str(), 9);
	t_rank_a->enableOuterStroke(ccc3(41, 41, 41), 1.f);
	t_rank_a->setPosition(ccp(25,8));
	graph_back->addChild(t_rank_a);
	
	KSLabelTTF* t_rank_b = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankB), mySGD->getFont().c_str(), 9);
	t_rank_b->enableOuterStroke(ccc3(41, 41, 41), 1.f);
	t_rank_b->setPosition(ccp(25+49,8));
	graph_back->addChild(t_rank_b);
	
	KSLabelTTF* t_rank_c = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankC), mySGD->getFont().c_str(), 9);
	t_rank_c->enableOuterStroke(ccc3(41, 41, 41), 1.f);
	t_rank_c->setPosition(ccp(25+98,8));
	graph_back->addChild(t_rank_c);
	
	KSLabelTTF* t_rank_d = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankD), mySGD->getFont().c_str(), 9);
	t_rank_d->enableOuterStroke(ccc3(41, 41, 41), 1.f);
	t_rank_d->setPosition(ccp(25+147,8));
	graph_back->addChild(t_rank_d);
	
	int alluser = 33; // result_data["alluser"].asInt();
	int lefttime = 33; // result_data["remainTime"].asInt();
	int myrank = 33; // result_data["myrank"].asInt();
	string lefttimestr;
	if(lefttime>60*60*24){
		lefttimestr=CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_dayAfterReset), lefttime/(60*60*24)+1)->getCString();
	}else if(lefttime>60*60){
		lefttimestr=CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_hourAfterReset), lefttime/(60*60)+1)->getCString();
	}else{
		lefttimestr=CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_secondAfterReset), lefttime/60+1)->getCString();
	}
	
	CCLabelTTF* lefttime_label = CCLabelTTF::create(lefttimestr.c_str(),mySGD->getFont().c_str(), 10);
	lefttime_label->setAnchorPoint(ccp(1,0.5));
	lefttime_label->setPosition(ccp(main_case->getContentSize().width-175, 210));
	main_case->addChild(lefttime_label);
	
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
	rank_percent_case->setPosition(ccp(0, 0));
	graph_back->addChild(rank_percent_case);
	
	KSLabelTTF* percent_label = KSLabelTTF::create(CCString::createWithFormat("%.0f%%", rank_percent*100.f)->getCString(), mySGD->getFont().c_str(), 13);
	percent_label->setColor(ccc3(255, 170, 20));
	percent_label->enableOuterStroke(ccc3(50, 25, 0), 1);
	percent_label->setPosition(ccp(rank_percent_case->getContentSize().width/2.f+1, rank_percent_case->getContentSize().height/2.f+2));
	graph_back->addChild(percent_label);
	
	CCMoveTo* t_move = CCMoveTo::create(2.f*(1.f-rank_percent), ccp(257 + 195.f*rank_percent,230));
	rank_percent_case->runAction(t_move);

	CCScale9Sprite* info_back1 = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	info_back1->setContentSize(CCSizeMake(210, 34));
	info_back1->setPosition(ccp(110, 110));
	main_case->addChild(info_back1);

	
	CCScale9Sprite* info_back2 = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	info_back2->setContentSize(CCSizeMake(210, 34));
	info_back2->setPosition(ccp(110, 80));
	main_case->addChild(info_back2);

	
	CCScale9Sprite* info_back3 = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	info_back3->setContentSize(CCSizeMake(210, 34));
	info_back3->setPosition(ccp(110, 50));
	main_case->addChild(info_back3);

	
	CCScale9Sprite* info_back4 = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	info_back4->setContentSize(CCSizeMake(210, 34));
	info_back4->setPosition(ccp(110, 20));
	main_case->addChild(info_back4);

	
	
	KSLabelTTF* lblHistory = KSLabelTTF::create("전적", mySGD->getFont().c_str(), 12.f);
	lblHistory->setColor(ccc3(255, 255, 255));
	lblHistory->enableOuterStroke(ccc3(0, 0, 0), 1.f);
	lblHistory->setAnchorPoint(ccp(0, 0.5f));
	lblHistory->setPosition(ccp(15, 18));
	info_back1->addChild(lblHistory);
	
	KSLabelTTF* lblHighScore = KSLabelTTF::create("최고점수", mySGD->getFont().c_str(), 12.f);
	lblHighScore->setColor(ccc3(255, 255, 255));
	lblHighScore->enableOuterStroke(ccc3(0, 0, 0), 1.f);
	lblHighScore->setAnchorPoint(ccp(0, 0.5f));
	lblHighScore->setPosition(ccp(15, -12));
	info_back1->addChild(lblHighScore);

	
	KSLabelTTF* lblChainWin = KSLabelTTF::create("최고연승", mySGD->getFont().c_str(), 12.f);
	lblChainWin->setColor(ccc3(255, 255, 255));
	lblChainWin->enableOuterStroke(ccc3(0, 0, 0), 1.f);
	lblChainWin->setAnchorPoint(ccp(0, 0.5f));
	lblChainWin->setPosition(ccp(15, -42));
	info_back1->addChild(lblChainWin);

	
	KSLabelTTF* lblMaxCombo = KSLabelTTF::create("최대콤보", mySGD->getFont().c_str(), 12.f);
	lblMaxCombo->setColor(ccc3(255, 255, 255));
	lblMaxCombo->enableOuterStroke(ccc3(0, 0, 0), 1.f);
	lblMaxCombo->setAnchorPoint(ccp(0, 0.5f));
	lblMaxCombo->setPosition(ccp(15, -72));
	info_back1->addChild(lblMaxCombo);

	
	{ // 위와 변수 이름은 같지만 값을 나타냄. 귀찮아서 블록 씌움.
		KSLabelTTF* lblHistory = KSLabelTTF::create("15승 ~~~~~~~~~", mySGD->getFont().c_str(), 12.f);
		lblHistory->setColor(ccc3(255, 255, 255));
		lblHistory->enableOuterStroke(ccc3(0, 0, 0), 1.f);
		lblHistory->setAnchorPoint(ccp(1, 0.5f));
		lblHistory->setPosition(ccp(198, 18));
		info_back1->addChild(lblHistory);

		
		KSLabelTTF* lblHighScore = KSLabelTTF::create("435435435", mySGD->getFont().c_str(), 12.f);
		lblHighScore->setColor(ccc3(255, 255, 255));
		lblHighScore->enableOuterStroke(ccc3(0, 0, 0), 1.f);
		lblHighScore->setAnchorPoint(ccp(1, 0.5f));
		lblHighScore->setPosition(ccp(198, -12));
		info_back1->addChild(lblHighScore);
//		FormSetter::get()->addObject("lbl2", lblHighScore); //
		
		KSLabelTTF* lblChainWin = KSLabelTTF::create("20 연승", mySGD->getFont().c_str(), 12.f);
		lblChainWin->setColor(ccc3(255, 255, 255));
		lblChainWin->enableOuterStroke(ccc3(0, 0, 0), 1.f);
		lblChainWin->setAnchorPoint(ccp(1, 0.5f));
		lblChainWin->setPosition(ccp(198, -42));
		info_back1->addChild(lblChainWin);
//		FormSetter::get()->addObject("lbl3", lblChainWin); //
		
		KSLabelTTF* lblMaxCombo = KSLabelTTF::create("150 콤보", mySGD->getFont().c_str(), 12.f);
		lblMaxCombo->setColor(ccc3(255, 255, 255));
		lblMaxCombo->enableOuterStroke(ccc3(0, 0, 0), 1.f);
		lblMaxCombo->setAnchorPoint(ccp(1, 0.5f));
		lblMaxCombo->setPosition(ccp(198, -72));
		info_back1->addChild(lblMaxCombo);
//		FormSetter::get()->addObject("lbl4", lblMaxCombo);
	}
}

void EndlessSettingPopup::addRankingTable(CCNode* node)
{
	CCScale9Sprite* barBack = CCScale9Sprite::create("cardsetting_scroll.png", CCRectMake(0, 0, 7, 13), CCRectMake(3, 6, 1, 1));
	barBack->setContentSize(CCSizeMake(7, 155));
	barBack->setPosition(ccp(235, 140));
	//	FormSetter::get()->addObject("testksoo", barBack);
	node->addChild(barBack);
	//320x320 테이블 뷰 생성
	
	m_rankingTable = CCTableView::create(this, CCSizeMake(245, 159));
	
	CCScale9Sprite* bar = CCScale9Sprite::create("cardsetting_scrollbutton.png",
																							 CCRect(0, 0, 12, 33), CCRectMake(5, 5, 3, 20));
	
	m_scrollBar = ScrollBar::createScrollbar(m_rankingTable, 2, NULL, bar);
	m_scrollBar->setDynamicScrollSize(true);
	m_scrollBar->setVisible(true);
	m_rankingTable->setAnchorPoint(CCPointZero);
	
	//kCCScrollViewDirectionVertical : 세로 스크롤, kCCScrollViewDirectionHorizontal : 가로 스크롤
	m_rankingTable->setDirection(kCCScrollViewDirectionVertical);
	
	//추가시 정렬 기준 설정 kCCTableViewFillTopDown : 아래부분으로 추가됨, kCCTableViewFillBottomUp : 위에서 부터 추가됨.
	m_rankingTable->setVerticalFillOrder(kCCTableViewFillTopDown);
	
	//기준점 0,0
	// 좌표 수동으로 잡느라 이리 됨
	//FormSetter::get()->addObject("testksoo", mailTableView);
//	m_rankingTable->setPosition(ccp(5, 56));
	m_rankingTable->setPosition(ccp(220, 68));
	//데이터를 가져오고나 터치 이벤트를 반환해줄 대리자를 이 클래스로 설정.
	m_rankingTable->setDelegate(this);
	addChild(m_rankingTable, 3);

	m_rankingTable->setTouchPriority(-200);
}

CCTableViewCell* EndlessSettingPopup::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	cell->setPosition(ccp(-5, 0));
	//		int alluser = rank_data["alluser"].asInt();
	//		int myrank = rank_data["myrank"].asInt();
	//
	//		Json::Value user_list = rank_data["list"];
	//
	//		delay_index = 0;
	int limit_count = 3;
	int i = 0;
	
	std::string case_name = "rank_normal.png";
	
	CCScale9Sprite* list_cell_case = CCScale9Sprite::create(case_name.c_str(), CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
	list_cell_case->setAnchorPoint(ccp(0,0));
	list_cell_case->setContentSize(CCSizeMake(225, 37));
	list_cell_case->setPosition(ccp(10,-3));
	cell->addChild(list_cell_case);
	
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
	
//	Json::Reader reader;
//	Json::Value read_data;
//	reader.parse(user_list[i].get("data", Json::Value()).asString(), read_data);
	string flag = "kr"; // read_data.get("flag", "kr").asString().c_str();
	
	CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
	selectedFlagSpr->setPosition(ccp(50,18));
	list_cell_case->addChild(selectedFlagSpr);
	
	KSLabelTTF* nick_label = KSLabelTTF::create("드로우걸스", mySGD->getFont().c_str(), 12); // user_list[i]["nick"].asString().c_str()
	nick_label->setAnchorPoint(ccp(0,0.5f));
	nick_label->enableOuterStroke(ccc3(50, 25, 0), 1);
	nick_label->setPosition(ccp(70,19));
	list_cell_case->addChild(nick_label);
	
	KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator("3458903").c_str(), mySGD->getFont().c_str(), 11);
	score_label->setColor(ccc3(255, 170, 20));
	score_label->setAnchorPoint(ccp(1,0.5f));
	score_label->enableOuterStroke(ccc3(50, 25, 0), 1.f);
	score_label->setPosition(ccp(185,18));
	list_cell_case->addChild(score_label);
	
	KSLabelTTF* chainwin_label = KSLabelTTF::create("22연승", mySGD->getFont().c_str(), 11);
	chainwin_label->setAnchorPoint(ccp(1,0.5f));
	chainwin_label->enableOuterStroke(ccc3(50, 25, 0), 1);
	
	chainwin_label->setPosition(ccp(215,18));
	list_cell_case->addChild(chainwin_label);
//	if(rankBack->getTag()!=1){
//		CCPoint original_position = list_cell_case->getPosition();
//		list_cell_case->setPosition(ccpAdd(original_position, ccp(500, 0)));
//		cell_action_list.push_back([=](){
//			delay_index = i;
//			CCDelayTime* t_delay = CCDelayTime::create(delay_index*0.2f);
//			CCMoveTo* t_move = CCMoveTo::create(0.5f, original_position);
//			CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
//			list_cell_case->runAction(t_seq);
//			rankBack->setTag(1);
//		});
//	}
	
	
	return cell;
	
}