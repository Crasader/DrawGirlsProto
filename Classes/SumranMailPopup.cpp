
// SumranMailPopup.cp180

#include "SumranMailPopup.h"

#include "hspConnector.h"

#include "DataStorageHub.h"
#include "GDWebSprite.h"
#include "ServerDataSave.h"
#include "CCMenuLambda.h"

#include "KSUtil.h"
#include "GDWebSprite.h"
#include "EnumDefine.h"
#include "ScrollBar.h"
#include "KSAlertView.h"
#include "StageInfoDown.h"
#include "PuzzleMapScene.h"
#include "CardCase.h"
#include "StageImgLoader.h"
#include "HatGacha.h"
#include "ASPopupView.h"

#include "UnknownFriends.h"
#include "KnownFriend.h"

#include "StarGoldData.h"
#include "GachaPurchase.h"
#include "StartSettingScene.h"
#include "KHAlertView.h"
#include "KSLabelTTF.h"
#include "FormSetter.h"
#include "MyLocalization.h"
#include "StyledLabelTTF.h"
#include "CommonAnimation.h"
#include "KSProtect.h"
#include "LabelTTFMarquee.h"
#define LZZ_INLINE inline

using namespace std;
namespace
{
  CCSize mailCellSize = CCSizeMake(213, 44);
}
SumranMailPopup * SumranMailPopup::create (CCObject * t_close, SEL_CallFunc d_close, std::function<void(void)> heartRefresh)
{
	SumranMailPopup* t_rp = new SumranMailPopup();
	t_rp->myInit(t_close, d_close, heartRefresh);
	t_rp->autorelease();
	return t_rp;
}
//void SumranMailPopup::finishedOpen ()
//{
//	loadMail();
//}
//void SumranMailPopup::finishedClose ()
//{
//	
//	(target_close->*delegate_close)();
//	removeFromParent();
//}
void SumranMailPopup::myInit (CCObject * t_close, SEL_CallFunc d_close, std::function<void(void)> heartRefresh)
{
	
	CCLayer::init();
	
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kMP_Z_gray);

	target_final = t_close;
	delegate_final = d_close;
	main_case = CCSprite::create("mainpopup2_back.png");
	//main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-14));
//	addChild(main_case, 0);
	
	CCSprite* title_back = CCSprite::create("title_tab.png");
	title_back->setPosition(ccp(60,main_case->getContentSize().height-13));
	main_case->addChild(title_back);

	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_giftbox), mySGD->getFont().c_str(), 15);
	title_label->disableOuterStroke();
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f));
	setFormSetter(title_label);
	title_back->addChild(title_label);
	
	KSLabelTTF* title_label_shadow = KSLabelTTF::create(title_label->getString(), mySGD->getFont().c_str(), 15);
	title_label_shadow->setColor(ccBLACK);
	title_label_shadow->enableOuterStroke(ccBLACK, 1, 60/255, true);
	title_label_shadow->setPosition(ccpFromSize(title_label->getContentSize()/2.f) + ccp(0,-1));
	title_label->addChild(title_label_shadow, -1);
	
	m_nothingMessage = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_nogift), mySGD->getFont().c_str(), 14);
	m_nothingMessage->setPosition(ccpMult(main_case->getContentSize(),0.5));
	main_case->addChild(m_nothingMessage, 1);
	setFormSetter(m_nothingMessage);
	m_nothingMessage->setVisible(false);	
	
	CCScale9Sprite* main_inner = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	main_inner->setContentSize(CCSizeMake(424, 204));
	main_inner->setPosition(main_case->getContentSize().width/2.f, main_case->getContentSize().height*0.45f + 5);
	setFormSetter(main_inner);
	main_case->addChild(main_inner);

	addChild(main_case, kMP_Z_back);
	
	
	startFormSetter(this);
//	target_close = t_close;
//	delegate_close = d_close;
	m_heartRefresh = heartRefresh;
	m_mailFilter = SumranMailFilter::kTotal;
	
	t_suction = TouchSuctionLayer::create(-999999);
	t_suction->setTouchEnabled(false);
	main_case->addChild(t_suction,1000);
	CommonAnimation::openPopup(this, main_case, gray, [=](){
		
	}, [=](){
		//		end_func(); removeFromParent();
	});
#if 0 // 싹 지우는 루틴..
	Json::Value p;
	p["memberID"] = hspConnector::get()->getSocialID();
	p["type"] = 0;
	hspConnector::get()->command("removeallmessage",p,this,[](Json::Value r){});
#endif

	m_popupState = SumranPostBoxState::kNoMenu;
	setTouchEnabled(true);
	isLoaded = false;
	
	CCMenuLambda* _menu = CCMenuLambda::create();
	_menu->setTouchPriority(-200);

	
	allReceive = CommonButton::create(CCSprite::create("subbutton_pink.png"), -200);
	allReceive->setTitleSize(12.f);
	allReceive->setTitle(myLoc->getLocalForKey(kMyLocalKey_allAccept));
	allReceive->getTitleLabel()->disableOuterStroke();
//	allReceive->setBackgroundTypeForDisabled(CommonButtonGray);
	allReceive->setTitleColor(ccc3(255, 255, 255));
	allReceive->setTitleColorForDisable(ccc3(90, 60, 30));
	//allReceive->setBackgroundTypeForDisabled(CommonButtonGray);
	allReceive->setFunction([=](CCObject*){
		
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
		
		t_suction->setTouchEnabled(true);
		t_suction->setVisible(true);
		Json::Value p;
		p["memberID"] = myHSP->getMemberID();
		
		
		myHSP->command("confirmallgiftboxhistory",p,[=](Json::Value r){
			if(r["result"]["code"].asInt()==GDSUCCESS){
				
				{
					rewardDown(r["list"],[=](bool isSuccess){
						//테이블 리로드
						m_mailList.clear();
						this->filterWithMailFilter();
						this->mailTableView->reloadData();
						mySGD->saveChangeGoodsTransaction(r);
						takedCheck(r["list"],[=](){
							t_suction->setTouchEnabled(false);
							t_suction->setVisible(false);
							
						});
					});
				}
			}
		});
	});
	//FormSetter::get()->addObject("testksoo2", allReceive);
	allReceive->setPosition(ccp(395.0, 13.5));
	allReceive->setEnabled(false);
	setFormSetter(allReceive);
	main_case->addChild(allReceive, 1);

	auto giftBoxAlertBox = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
	giftBoxAlertBox->setPosition(ccp(326.0, 246.5));
	giftBoxAlertBox->setContentSize(CCSizeMake(224.5, 19.5));
	main_case->addChild(giftBoxAlertBox);

	
	CCLabelTTF* giftboxAlert = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_giftboxAlert), mySGD->getFont().c_str(), 12);
	setFormSetter(giftboxAlert);
	setFormSetter(giftBoxAlertBox);
	giftboxAlert->setAnchorPoint(ccp(0,0.5));
	giftboxAlert->setPosition(ccpFromSize(giftBoxAlertBox->getContentSize()) / 2.f + ccp(-94.5, 1));
	giftBoxAlertBox->addChild(giftboxAlert, 1);
	
	
	CommonButton* giftFilter = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_ticketBox), 12, CCSizeMake(65,38), CommonButtonGray, -200); // ? 티켓함?이 맞음? From YH To KS
	CommonButton* helpFilter = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_helpBox), 12, CCSizeMake(65,38), CommonButtonGray, -200);
	CommonButton* ticketFilter = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_ticketBox), 12, CCSizeMake(65,38), CommonButtonGray, -200);
	CommonButton* challengeFilter = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_challengeBox), 12, CCSizeMake(65,38), CommonButtonGray, -200);
	CommonButton* coinFilter = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_heartBox), 12, CCSizeMake(65,38), CommonButtonGray, -200);
	CommonButton* totalFilter = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_allView), 12, CCSizeMake(65,38), CommonButtonGray, -200);
	
	// 모든 필터를 감춤. 섬란 버전에서는.
	giftFilter->setVisible(false);
	helpFilter->setVisible(false);
	ticketFilter->setVisible(false);
	challengeFilter->setVisible(false);
	totalFilter->setVisible(false);
	coinFilter->setVisible(false);
	
	loadMail();
	
	auto allInvisible = [=]()
	{
		giftFilter->setEnabled(true);
		helpFilter->setEnabled(true);
		ticketFilter->setEnabled(true);
		challengeFilter->setEnabled(true);
		coinFilter->setEnabled(true);
		totalFilter->setEnabled(true);

	};

	
	totalFilter->setBackgroundTypeForDisabled(CommonButtonYellow);
	totalFilter->setTitleColor(ccc3(200, 200, 200));
	totalFilter->setTitleColorForDisable(ccc3(20, 0, 0));
	totalFilter->setFunction([=](CCObject*){
		if(!this->isLoaded)return;
		m_mailFilter = SumranMailFilter::kTotal;
		filterWithMailFilter();
		this->mailTableView->reloadData();
		allInvisible();
		totalFilter->setEnabled(false);
		allReceive->setEnabled(false);
	});
	totalFilter->setPosition(ccp(53, 255));
	this->addChild(totalFilter, 1);
	
	coinFilter->setBackgroundTypeForDisabled(CommonButtonYellow);
	coinFilter->setTitleColor(ccc3(200, 200, 200));
	coinFilter->setTitleColorForDisable(ccc3(20, 0, 0));
	coinFilter->setFunction([=](CCObject*){
		if(!this->isLoaded)return;
		m_mailFilter = SumranMailFilter::kHeart;
		filterWithMailFilter();
		this->mailTableView->reloadData();
		allInvisible();
		coinFilter->setEnabled(false);
		allReceive->setEnabled(true);

	});
	coinFilter->setPosition(ccp(118, 255));
	this->addChild(coinFilter, 1);
	
	
	
	
	

	challengeFilter->setBackgroundTypeForDisabled(CommonButtonYellow);
	challengeFilter->setTitleColor(ccc3(200, 200, 200));
	challengeFilter->setTitleColorForDisable(ccc3(20, 0, 0));
	challengeFilter->setFunction([=](CCObject*){
		if(!this->isLoaded)return;
		m_mailFilter = SumranMailFilter::kChallenge;
		filterWithMailFilter();
		this->mailTableView->reloadData();
		allInvisible();
		challengeFilter->setEnabled(false);
		allReceive->setEnabled(false);

	});
	challengeFilter->setPosition(ccp(182, 255));
	this->addChild(challengeFilter, 1);
	
	
	ticketFilter->setBackgroundTypeForDisabled(CommonButtonYellow);
	ticketFilter->setTitleColor(ccc3(200, 200, 200));
	ticketFilter->setTitleColorForDisable(ccc3(20, 0, 0));
	ticketFilter->setFunction([=](CCObject*){
		if(!this->isLoaded)return;
		m_mailFilter = SumranMailFilter::kTicket;
		filterWithMailFilter();
		this->mailTableView->reloadData();
		allInvisible();
		ticketFilter->setEnabled(false);
		allReceive->setEnabled(true);

	});
	ticketFilter->setPosition(ccp(247, 255));
	this->addChild(ticketFilter, 1);
	
	
	
	
	helpFilter->setBackgroundTypeForDisabled(CommonButtonYellow);
	helpFilter->setTitleColor(ccc3(200, 200, 200));
	helpFilter->setTitleColorForDisable(ccc3(20, 0, 0));
	helpFilter->setFunction([=](CCObject*){
		if(!this->isLoaded)return;
		m_mailFilter = SumranMailFilter::kHelp;
		filterWithMailFilter();
		this->mailTableView->reloadData();
		allInvisible();
		helpFilter->setEnabled(false);
	});
	helpFilter->setPosition(ccp(312, 255));
	this->addChild(helpFilter, 1);
	
	
		
	giftFilter->setBackgroundTypeForDisabled(CommonButtonYellow);
	giftFilter->setTitleColor(ccc3(200, 200, 200));
	giftFilter->setTitleColorForDisable(ccc3(20, 0, 0));
	giftFilter->setFunction([=](CCObject*){
		if(!this->isLoaded)return;
		m_mailFilter = SumranMailFilter::kHelp;
		filterWithMailFilter();
		this->mailTableView->reloadData();
		allInvisible();
		giftFilter->setEnabled(false);
	});
	giftFilter->setPosition(ccp(377, 255));
	this->addChild(giftFilter, 1);
	
	CommonButton* closeBtn = CommonButton::createCloseButton(-200);
	closeBtn->setFunction([=](CCObject*){
		hspConnector::get()->removeTarget(this);
		
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
		CommonAnimation::closePopup(this, main_case, gray, [=](){
			
		}, [=](){
			(target_final->*delegate_final)();
			removeFromParent();
			
		});
//		this->hidePopup();
	});
	closeBtn->setPosition(ccpFromSize(main_case->getContentSize()) + ccp(-20,-12));
	setFormSetter(closeBtn);
	main_case->addChild(closeBtn);
	
	
	allInvisible();
	totalFilter->setEnabled(false);
	
	//_menu->addChild(closeBtn, kMP_Z_close);
	
	loading_circle = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
	loading_circle->setPosition(ccp(main_case->getContentSize().width/2.f, main_case->getContentSize().height/2.f));
	t_suction->addChild(loading_circle);
	
	
}
void SumranMailPopup::loadMail ()
{
	Json::Value p;
	p["memberID"]=hspConnector::get()->getSocialID();
	// 0 이 아니면 해당하는 타입의 메시지가 들어옴.
	
	hspConnector::get()->command("getgiftboxhistory",p,this,[this](Json::Value r)
															 {
																 GraphDogLib::JsonToLog("getgiftboxhistory", r);
																 if(r["result"]["code"].asInt() != GDSUCCESS)
																	 return;
																 this->drawMail(r);
															 });
}
void SumranMailPopup::drawMail (Json::Value obj)
{
	
	t_suction->setTouchEnabled(false);
	t_suction->setVisible(false);
	//loading_circle->setVisible(false);
	
	m_mailList = obj["list"];
//	auto app_friends = fInfo["app_friends_info"];
	std::map<std::string, FriendData> userIdKeyValue;
	// m_mailList 와 app_friends 를 합쳐야됨.
	//
	if(m_mailList.isArray() && m_mailList.size()>0){
		allReceive->setEnabled(true);
	}
	
	for(auto it : KnownFriends::getInstance()->getFriends())
	{
		userIdKeyValue[it.userId] = it;
	}
	
//	for(int i=0; i<m_mailList.size(); i++)
//	{
//		std::string user_id = m_mailList[i]["friendID"].asString();
//		m_mailList[i]["nickname"] = userIdKeyValue[user_id].nick;
//		m_mailList[i]["profile_image_url"] = userIdKeyValue[user_id].profileUrl;
//	}
	
	filterWithMailFilter();
	//테이블 뷰 생성 시작 /////////////////////////////////////////////////////////////////////////////////////////
	
	CCScale9Sprite* barBack = CCScale9Sprite::create("cardsetting_scroll.png", CCRectMake(0, 0, 7, 13), CCRectMake(3, 6, 1, 1));
	barBack->setContentSize(CCSizeMake(7, 160.f));
	barBack->setPosition(ccp(451, 140));
	barBack->setVisible(false);
	setFormSetter(barBack);
//	FormSetter::get()->addObject("testksoo", barBack);
	main_case->addChild(barBack, kMP_Z_mailTable - 1);
	//320x320 테이블 뷰 생성

	mailTableView = CCTableView::create(this, CCSizeMake(455.f, 174.f));
	setFormSetter(mailTableView);	
//	CCScale9Sprite* bar = CCScale9Sprite::create("postbox_bar.png");
	CCScale9Sprite* scrollBar = CCScale9Sprite::create("cardsetting_scrollbutton.png",
																										 CCRect(0, 0, 12, 33), CCRectMake(5, 5, 3, 20));
	m_scrollBar = ScrollBar::createScrollbar(mailTableView, -18, NULL, scrollBar, -191);
	m_scrollBar->setDynamicScrollSize(true);
	m_scrollBar->setVisible(false);
	mailTableView->setAnchorPoint(CCPointZero);
	setFormSetter(m_scrollBar);
	//kCCScrollViewDirectionVertical : 세로 스크롤, kCCScrollViewDirectionHorizontal : 가로 스크롤
	mailTableView->setDirection(kCCScrollViewDirectionVertical);
	setFormSetter(this);
	//추가시 정렬 기준 설정 kCCTableViewFillTopDown : 아래부분으로 추가됨, kCCTableViewFillBottomUp : 위에서 부터 추가됨.
	mailTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	
	//기준점 0,0
	// 좌표 수동으로 잡느라 이리 됨
	//FormSetter::get()->addObject("testksoo", mailTableView);
	mailTableView->setPosition(ccp(33.0, 56.0));
	
	//데이터를 가져오고나 터치 이벤트를 반환해줄 대리자를 이 클래스로 설정.
	mailTableView->setDelegate(this);
//	this->addChild(mailTableView, kMP_Z_mailTable);
	main_case->addChild(mailTableView, kMP_Z_mailTable);
	mailTableView->setTouchPriority(-200);

	isLoaded=true;
	
	//테이블 뷰 생성 끝/////////////////////////////////////////////////////////////////////////////////////////
	
	// 필터한 json 을 생성함. m_mailFilter 에 따라 필터함.
	// atIndex 에서 그릴 때, m_filteredMailList 에 따라 돌아가야 될 듯.
	//
	//if(m_mailFilter == SumranMailFilter::kTotal)
	//{
	//}
	//else 
		//if(m_mailFilter == SumranMailFilter::kTotal)
}
//void SumranMailPopup::closePopup (CCControlButton * obj, CCControlEvent event)
//{
//	this->runAction(CCSequence::create(CCMoveBy::create(0.5, CCPoint(0,-400)),CCCallFunc::create(this, callfunc_selector(SumranMailPopup::finishedClose)),NULL));
//}
void SumranMailPopup::removeMail (CCObject * _obj)
{
	
	
}
CCTableViewCell * SumranMailPopup::tableCellAtIndex (CCTableView * table, unsigned int idx)
{
	CCTableViewCell* realCell = new CCTableViewCell();
	realCell->init();
	realCell->autorelease();
	auto createCCNodeFromIdx = [=](int idx)->CCNode*
	{

		KSLabelTTF* title;
//		CCMenuItemLambda* sendBtn;
		CommonButton* btnReceive;
		KSLabelTTF* score;
		//Json::Reader reader;
		//Json::Value contentObj;

		const Json::Value& mail = m_filteredMailList[idx]; //hspConnector::get()->getMailByIndex(idx);
		//reader.parse(mail["content"].asString(), contentObj);


		KS::KSLog("%", mail);
		
		//KS::KSLog("%", contentObj);
		//	KS::KSLog("%", contentObj["puzzlenumber"].asInt());

		CCNode* cell = CCNode::create();
		setFormSetter(cell);
		std::string cellBackFile = "rank_normal2.png";

		CCScale9Sprite* listCellCase = CCScale9Sprite::create(cellBackFile.c_str(), CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
		listCellCase->setContentSize(CCSizeMake(201, 41));
		setFormSetter(listCellCase);
		CCScale9Sprite* bg = listCellCase;
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,0);

		auto profileImg = CCScale9Sprite::create("common_purple.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
		profileImg->setContentSize(CCSizeMake(45.0, 32.0));
		profileImg->setAnchorPoint(ccp(0.5, 0.5));
		profileImg->setTag(kMP_MT_profileImg);
		profileImg->setPosition(ccp(27.0, 20.5));
		CCSprite* presentIcon;
		
		
		if(mail["reward"].isArray() && mail["reward"].size()==1){
			string rType = mail["reward"][0]["type"].asString();
			if(rType=="fr" || rType=="pr")rType="r";
			presentIcon= CCSprite::create(CCString::createWithFormat("icon_%s.png",rType.c_str())->getCString());
		}else{
			presentIcon = CCSprite::create("postbox_present.png");
		}
		profileImg->addChild(presentIcon);
		presentIcon->setPosition(ccpFromSize(profileImg->getContentSize()) / 2.f);
		//profileImg->setScale(30.f / profileImg->getContentSize().width);
		cell->addChild(profileImg, kMP_Z_profileImg);
		setFormSetter(profileImg);

		CCMenuLambda* _menu = CCMenuLambda::create();
		_menu->setPosition(ccp(0, 0));
		_menu->setTouchPriority(-200);
		_menu->setTag(kMP_MT_send);
		cell->addChild(_menu, kMP_MT_getheart);



		title = KSLabelTTF::create(mail.get("content","Gift").asString().c_str(), mySGD->getFont().c_str(), 13); // "님의"
		title->setPosition(ccp(53.5, 19.5));
		title->setColor(ccc3(255, 255, 255));
		title->setAnchorPoint(CCPointZero);
		title->setTag(kMP_MT_title);
		setFormSetter(title);
		cell->addChild(title,2);
		if(title->getContentSize().width>100){
			title->removeFromParent();
			LabelTTFMarquee *title2 = LabelTTFMarquee::create(ccc4(255, 255, 255, 255), 100, 13, "");
			title2->addText("<font size=13 color=#ffffff>"+mail.get("content","Gift").asString()+"</font>");
			title2->setAnchorPoint(CCPointZero);
			title2->setTag(kMP_MT_title);
			title2->setSpace(50);
			title2->startMarquee();
			title2->setPosition(ccp(53.5, 19.5));
			cell->addChild(title2,2);
			setFormSetter(title2);
		}
		
		std::string comment;
		int type = kGift;
		
		switch(type)
		{
			case kGift:
				
				comment = GraphDog::get()->dateFormat("m/d h:i",mail.get("regDate","Event").asString().c_str());
				
				
				btnReceive = CommonButton::create(CCSprite::create("subbutton_purple.png"), -200);
				setFormSetter(btnReceive);
				btnReceive->setTitle(myLoc->getLocalForKey(kMyLocalKey_take));
				btnReceive->setTitleSize(12.f);
				btnReceive->setTouchPriority(-200);
				btnReceive->setTitleColor(ccc3(255, 255, 255));
				btnReceive->getTitleLabel()->disableOuterStroke();
				btnReceive->setFunction([=](CCObject*)
																{
																	//						 CCMenuItemLambda* obj = dynamic_cast<CCMenuItemLambda*>(sender);
																	//						 int idx = (int)obj->getUserData();
																	
																	
																	
																	/*
																	 mail["list"][N]["type"]==cd 인게 있으면 이미지 다운로드후 표시할것.
																	*/
																	
																	t_suction->setTouchEnabled(true);
																	t_suction->setVisible(true);
																	
																	rewardDown(mail["reward"],[=](bool isSuccess){
																	////////////////////////////////////////////////////////////////////
																		
																		
																		t_suction->setTouchEnabled(false);
																		t_suction->setVisible(false);
																		
																		
																		if(isSuccess==false){
																			ASPopupView *alert = ASPopupView::getCommonNoti(-9999,"실패", "카드정보 로드를 실패했습니다.",[=](){
																				AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																			});
																			this->addChild(alert, 2000);
																			return;
																		}
																		
																		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		
																		CCNode* itemlist = CCNode::create();
																		setFormSetter(itemlist);
																		if(mail["reward"].isArray()){
																			for(int i=0;i<mail["reward"].size();i++){
																				string rewardType = mail["reward"][i].get("type","box").asString();
																				if(rewardType=="fr" || rewardType=="pr")rewardType="r";
																				int rewardCount = mail["reward"][i].get("count",1).asInt();
																				
																				CCScale9Sprite* back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
																				back->setContentSize(CCSizeMake(70, 70));
																				back->setPosition(ccp(i*75-(mail["reward"].size()-1)/2.f*75,0));
																				
																				
																				CCSprite* spr;
																				KSLabelTTF* count = KSLabelTTF::create(CCString::createWithFormat("%d",rewardCount)->getCString(), mySGD->getFont().c_str(), 13);
																				if(rewardType=="cd"){
																					spr = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png",rewardCount)->getCString());
																					spr->setScale(0.12);
																					count->setString("");
																				}else{
																					spr= CCSprite::create(CCString::createWithFormat("icon_%s.png",rewardType.c_str())->getCString());
																				}
																				
																				
																				
																				count->setPosition(ccp(back->getContentSize().width/2.f,16));
																				spr->setPosition(ccp(back->getContentSize().width/2.f,back->getContentSize().height/2.f));
																				
																				setFormSetter(back);
																				setFormSetter(spr);
																				setFormSetter(count);
																				back->addChild(count);
																				back->addChild(spr);
																				itemlist->addChild(back);
																				
																			}
																		}else if(mail["reward"].isObject()){
																			
																		}
																		
																		
																		//KSLabelTTF* ment = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_getgift), mySGD->getFont().c_str(), 13);
																		//	ment->setPosition(ccp(itemlist->getContentSize().width/2.f,-83));
																		//itemlist->addChild(ment);
																		//setFormSetter(ment);
																		
																		
																		//																	StyledLabelTTF* lbl = StyledLabelTTF::create(rewardList.c_str(), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
																		//																	setFormSetter(lbl);
																		//																	lbl->setOldAnchorPoint();
																		//																	lbl->setPosition(ccp(0,-55));
																		//																	itemlist->addChild(lbl);
																		itemlist->setContentSize(CCSizeMake(mail["reward"].size()*80, 100));
																		
																		
																		
																		
																		
																		
																		ASPopupView *alert = ASPopupView::getCommonNoti(-9999,mail.get("content","Gfit").asString(), itemlist,[=](){
																			
																			AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																			
																			Json::Value p;
																			int mailNo = mail["no"].asInt();
																			
																			p["giftBoxNo"] = mailNo;
																			p["memberID"] = mail["memberID"].asInt64();
																			
																			t_suction->setTouchEnabled(true);
																			t_suction->setVisible(true);
																			//삭제요청
																			this->removeMessage (mailNo, mail["memberID"].asInt64(),
																													 [=](Json::Value r)
																													 {
																														 mySGD->saveChangeGoodsTransaction(r);
																														 takedCheck(r["list"],[=](){
																															 
																															 t_suction->setTouchEnabled(false);
																															 t_suction->setVisible(false);
																															 
																															 
																														 });
																													 });
																			
																		});
																		
																		this->addChild(alert, 2000);
																		
																		
																	///////////////////////////////////////////////////////////////////
																	});
																	
																	
																	
																	
																	
																});
				cell->addChild(btnReceive, kMP_MT_getheart);
				break;

		}

		score = KSLabelTTF::create(comment.c_str(),mySGD->getFont().c_str(), 9);
		score->setColor(ccc3(54, 36, 148));
		score->disableOuterStroke();
		score->setPosition(ccp(54.0, 7.5));
		score->setAnchorPoint(CCPointZero);
		score->setTag(kMP_MT_score);
		setFormSetter(score);
		cell->addChild(score,2);
		btnReceive->setUserData((void*)idx);
		btnReceive->setPosition(ccp(178.5, 21.0));
//		sendBtn->setUserData((void *)idx);
//		sendBtn->setPosition(ccp(174.5,23.5));
//		setFormSetter(sendBtn);
		return cell;
	};
	bool odd = false;
	if(numberOfCellsInTableView(mailTableView) - 1 == idx) // last 조건
	{
		if(m_filteredMailList.size() % 2 == 1)
			odd = true;
	}

	if(odd)
	{
		CCNode* cell1 = createCCNodeFromIdx(idx * 2);	
		realCell->addChild(cell1);	
	}
	else
	{
		CCNode* cell1 = createCCNodeFromIdx(idx * 2);	
		realCell->addChild(cell1);	
		CCNode* cell2= createCCNodeFromIdx(idx * 2 + 1);	
		realCell->addChild(cell2);	
		cell2->setPosition(ccp(211, 0));
	}
		
	return realCell;
}
void SumranMailPopup::scrollViewDidScroll (CCScrollView * view)
{
	if(m_scrollBar)
	{
		m_scrollBar->setBarRefresh();
	}
}
void SumranMailPopup::scrollViewDidZoom (CCScrollView * view)
{
	
}
void SumranMailPopup::tableCellTouched (CCTableView * table, CCTableViewCell * cell)
{
	
	CCLOG("touch!!");
	
}
CCSize SumranMailPopup::cellSizeForTable (CCTableView * table)
{
	return mailCellSize;
}
unsigned int SumranMailPopup::numberOfCellsInTableView (CCTableView * table)
{
	return ceilf(m_filteredMailList.size() / 2.f);
}
void SumranMailPopup::touchFriend (CCObject * a, CCControlEvent b)
{
	
	
}
//CCPoint SumranMailPopup::getContentPosition (int t_tag)
//{
	//CCPoint return_value;
	
	//if(t_tag == kMP_MT_invite)					return_value = ccp(240,43);
	//else if(t_tag == kMP_MT_close)				return_value = ccp(345,283);
	//else if(t_tag == kMP_MT_send)				return_value = ccp(240,230);
	//else if(t_tag == kMP_MT_send_close)			return_value = ccp(193,59);
	//else if(t_tag == kMP_MT_invite_close)		return_value = ccp(345,283);
	//else if(t_tag == kMP_MT_invite_rank)		return_value = ccp(240,43);
	//else if(t_tag == kMP_MT_invite_send)		return_value = ccp(240,230);
	//else if(t_tag == kMP_MT_invite_send_close)	return_value = ccp(193,59);
	
	//return return_value;
//}
bool SumranMailPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	return true;
}
void SumranMailPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void SumranMailPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void SumranMailPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
}
void SumranMailPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}

void SumranMailPopup::rewardDown(Json::Value reward, std::function<void(bool)> func){
	
	//여기서 reward체크하고 다운받기~
	
	if(reward.isArray()){
		for(int i=0;i<reward.size();i++){
			if(reward[i].get("type","box").asString()=="cd"){
				cardDown(reward[i]["count"].asInt(),func);
				return;
			}
		}
	}
	
	func(true);
}

void SumranMailPopup::takedCheck(Json::Value reward, std::function<void(void)> func){
	if(reward.isArray()){
		for(int i=0;i<reward.size();i++){
			if(reward[i].get("type","box").asString()=="cd"){
				takedCard(reward[i]["count"].asInt(),func);
				return;
			}
		}
	}
	
	func();
}

void SumranMailPopup::cardDown(int cardNo,std::function<void(bool)>func){
	//카드정보 다운로드하기, 다운성공시 func(true); , 실패시 func(false) 호출
	
	// 카드 정보 받기
	card_download_list.clear();
	card_reduction_list.clear();
	download_card_no = cardNo;
	end_func_download_card = func;
	Json::Value card_param;
	card_param["noList"][0] = cardNo;
	myHSP->command("getcardlist", card_param, json_selector(this, SumranMailPopup::resultGetCardInfo));
}

void SumranMailPopup::resultGetCardInfo(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		
		Json::Value cards = result_data["list"];
		for(int i=0;i<cards.size();i++)
		{
			Json::Value t_card = cards[i];
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
			
			NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
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
				card_download_list.push_back(t_df);
				// ================================
				
				CopyFile t_cf;
				t_cf.from_filename = t_df.filename.c_str();
				t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
				card_reduction_list.push_back(t_cf);
				
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
					card_download_list.push_back(t_df);
					// ================================
				}
				
				if(is_add_cf)
				{
					CopyFile t_cf = card_reduction_list.back();
					card_reduction_list.pop_back();
					t_cf.is_ani = true;
					t_cf.cut_width = t_detail["cutWidth"].asInt();
					t_cf.cut_height = t_detail["cutHeight"].asInt();
					t_cf.position_x = t_detail["positionX"].asInt();
					t_cf.position_y = t_detail["positionY"].asInt();
					t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
					card_reduction_list.push_back(t_cf);
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
				NSDS_SS(kSDS_CI_int1_soundType_int1_s, t_card["no"].asInt(), j, t_card["sound"][j-1].asString(), false);
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
					card_download_list.push_back(t_df);
					// ================================
				}
			}
		}
		
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		if(0 >= card_download_list.size())
		{
			end_func_download_card(true);
		}
		else
		{
			ing_card_download = 1;
			startCardDownload();
		}
	}
	else
	{
		mySGD->network_check_cnt++;
		
		if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
		{
			mySGD->network_check_cnt = 0;
			
			ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(kMyLocalKey_reConnect), myLoc->getLocalForKey(kMyLocalKey_reConnectAlert4),[=](){
				Json::Value card_param;
				card_param["noList"][0] = download_card_no.getV();
				myHSP->command("getcardlist", card_param, json_selector(this, SumranMailPopup::resultGetCardInfo));
			});
			((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
									 {
										 Json::Value card_param;
										 card_param["noList"][0] = download_card_no.getV();
										 myHSP->command("getcardlist", card_param, json_selector(this, SumranMailPopup::resultGetCardInfo));
									 }));
		}
	}
}

void SumranMailPopup::startCardDownload()
{
	mySGD->network_check_cnt = 0;
	if(card_download_list.size() > 0 && ing_card_download <= card_download_list.size())
	{
		CCLOG("%d : %s", ing_download_cnt, card_download_list[ing_card_download-1].filename.c_str());
		mySIL->downloadImg(card_download_list[ing_card_download-1].img,
													  card_download_list[ing_card_download-1].size,
													  card_download_list[ing_card_download-1].filename,
													  this, callfunc_selector(SumranMailPopup::successCardDownload), this, callfunc_selector(SumranMailPopup::failCardDownload));
	}
	else
	{
		end_func_download_card(true);
	}
}

void SumranMailPopup::successCardDownload()
{
	if(ing_download_cnt < card_download_list.size())
	{
		SDS_SS(kSDF_cardInfo, card_download_list[ing_card_download-1].key,
			   card_download_list[ing_card_download-1].img, false);
		ing_download_cnt++;
		startCardDownload();
	}
	else if(ing_download_cnt == card_download_list.size())
	{
		SDS_SS(kSDF_cardInfo, card_download_list[ing_card_download-1].key,
			   card_download_list[ing_card_download-1].img, false);
		for(int i=0;i<card_reduction_list.size();i++)
		{
			CCSprite* target_img = CCSprite::createWithTexture(mySIL->addImage(card_reduction_list[i].from_filename.c_str()));
			target_img->setAnchorPoint(ccp(0,0));
			
			if(card_reduction_list[i].is_ani)
			{
				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(card_reduction_list[i].ani_filename.c_str()),
																CCRectMake(0, 0, card_reduction_list[i].cut_width, card_reduction_list[i].cut_height));
				ani_img->setPosition(ccp(card_reduction_list[i].position_x, card_reduction_list[i].position_y));
				target_img->addChild(ani_img);
			}
			
			target_img->setScale(0.2f);
			
			CCRenderTexture* t_texture = CCRenderTexture::create(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY());
			t_texture->setSprite(target_img);
			t_texture->begin();
			t_texture->getSprite()->visit();
			t_texture->end();
			
			t_texture->saveToFile(card_reduction_list[i].to_filename.c_str(), kCCImageFormatPNG);
		}
		
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		ing_download_cnt++;
		end_func_download_card(true);
	}
	else
	{
		end_func_download_card(true);
	}
}

void SumranMailPopup::failCardDownload()
{
	mySGD->network_check_cnt++;
	
	if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
	{
		mySGD->network_check_cnt = 0;
		
		ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(kMyLocalKey_reConnect), myLoc->getLocalForKey(kMyLocalKey_reConnectAlert4),[=](){
			startCardDownload();
		});
		((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
	}
	else
	{
		addChild(KSTimer::create(0.5f, [=]()
								 {
									 startCardDownload();
								 }));
	}
}

void SumranMailPopup::takedCard(int cardNo, function<void()> t_end_func){
	// 클라이언트의 카드정보업데이트 using getcardhistory?
	Json::Value card_param;
	card_param["memberID"] = hspConnector::get()->getSocialID();
	myHSP->command("getCardHistory", card_param, [=](Json::Value result_data)
	{
		if(result_data["result"]["code"] == GDSUCCESS)
		{
			mySGD->total_card_cnt = result_data["lastCardNo"].asInt();
			
			mySGD->network_check_cnt = 0;
			
			vector<int> card_data_load_list;
			card_data_load_list.clear();
			
			mySGD->initTakeCardInfo(result_data["list"], card_data_load_list);
			
//			if(card_data_load_list.size() > 0)
//			{
//				Json::Value card_param;
//				for(int i=0;i<card_data_load_list.size();i++)
//					card_param["noList"][i] = card_data_load_list[i];
//				command_list.push_back(CommandParam("getcardlist", card_param, json_selector(this, TitleRenewalScene::resultLoadedCardData)));
//			}
//			else
//			{
				mySGD->resetHasGottenCards();
//			}
			
			t_end_func();
		}
		else
		{
			mySGD->network_check_cnt++;
			
			if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
			{
				mySGD->network_check_cnt = 0;
				
				ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(kMyLocalKey_reConnect), myLoc->getLocalForKey(kMyLocalKey_reConnectAlert4),[=](){
					takedCard(cardNo, t_end_func);
				});
				((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
			}
			else
			{
				addChild(KSTimer::create(0.5f, [=]()
										 {
											 takedCard(cardNo, t_end_func);
										 }));
			}
		}
	});
}

void SumranMailPopup::onReceiveStageSuccess()
{
	// 성공시 게임창...
	CCLOG("sec");
//	PuzzleMapScene* pms = dynamic_cast<PuzzleMapScene*>(target_close);
//	CCAssert(pms, "!!");
//	(target_close->*delegate_close)();
//	removeFromParent();
//	pms->showAcceptStageSettingPopup();
	
	CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
	
//	PuzzleMapScene::showAcceptStageSettingPopup();
	
}
void SumranMailPopup::onReceiveStageFail()
{
	mySGD->setRemoveMessageMemberId(0);
	mySGD->setRemoveMessageMailNo(0);
//	removeFromParent();
	CCLOG("fail");
}
// 도움 수락을 누른 상태.
void SumranMailPopup::iHelpYou(int stage, long long user_id, const std::string& nick, Json::Value removeInfo)
{

	//삭제요청 하는 방법임. 삭제해야할 정보는 removeInfo 에 들어있음 신경 ㄴㄴ해.
#if 0
	hspConnector::get()->command
	(
	 "removemessage", removeInfo,
	 [=](Json::Value r)
	 {
		 if(r.get("state","fail").asString() == "ok"){
		 }
	 });
#endif
	
	
}

CCNode* SumranMailPopup::addCardImg (int t_card_number, int t_card_level, string t_passive)
{
	CCSprite* card_img = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png", t_card_number)->getCString());
	card_img->setScale(0.34f);
	
	if(NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, t_card_number))
	{
		CCSize ani_size = CCSizeMake(NSDS_GI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card_number));
		CCSprite* t_ani = mySIL->getLoadedImg(CCString::createWithFormat("card%d_animation.png", t_card_number)->getCString(),
											  CCRectMake(0, 0, ani_size.width, ani_size.height));
		t_ani->setPosition(ccp(NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card_number)));
		card_img->addChild(t_ani);
	}
	
	CardCase* t_case = CardCase::create(t_card_number, t_card_level, t_passive);
	t_case->setPosition(CCPointZero);
	card_img->addChild(t_case);
	
	CCLabelTTF* recent_durability_label = t_case->getRecentDurabilityLabel();
	recent_durability_label->setString(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_durability_i, t_card_number))->getCString());
//	recent_durability_label->setPosition(ccpAdd(recent_durability_label->getPosition(), ccp(6,-1)));
	recent_durability_label->setFontSize(recent_durability_label->getFontSize()+3);
	
//	t_case->getTotalDurabilityLabel()->removeFromParent();
	
	return card_img;
}

void SumranMailPopup::resultLoadedCardInfo (Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value cards = result_data["list"];
		for(int i=0;i<cards.size();i++)
		{
			Json::Value t_card = cards[i];
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
			
			NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
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
				NSDS_SS(kSDS_CI_int1_soundType_int1_s, t_card["no"].asInt(), j, t_card["sound"][j-1].asString(), false);
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
			mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		}
		
		if(mySGD->isHasGottenCards(download_card_number) == 0)
		{
			mySGD->addHasGottenCardNumber(download_card_number);
		}
		
		(target_final->*callfunc_selector(PuzzleMapScene::resetPuzzle))();
		
		if(df_list.size() > 0) // need download
		{
			startDownloadCardImage();
		}
		else
		{
			CCNode* loading_parent = loading_card_img->getParent();
			CCPoint loading_position = loading_card_img->getPosition();
			
			loading_card_img->removeFromParent();
			
			loading_parent->addChild(addCardImg(download_card_number, -1, "-1"));
		}
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		CCNode* loading_parent = loading_card_img->getParent();
		CCPoint loading_position = loading_card_img->getPosition();
		
		loading_card_img->removeFromParent();
		
		loading_parent->addChild(addCardImg(download_card_number, -1, "-1"));
	}
	else
	{
		failAction();
	}
}

void SumranMailPopup::successAction ()
{
	SDS_SS(kSDF_cardInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img);
	
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
		
		df_list.clear();
		cf_list.clear();
		
		CCNode* loading_parent = loading_card_img->getParent();
		CCPoint loading_position = loading_card_img->getPosition();
		
		loading_card_img->removeFromParent();
		
		loading_parent->addChild(addCardImg(download_card_number, -1, "-1"));
	}
	else
	{
		ing_download_cnt++;
		startDownload();
	}
}
void SumranMailPopup::failAction ()
{
	CCNode* loading_parent = loading_card_img->getParent();
	CCPoint loading_position = loading_card_img->getPosition();
	
	loading_card_img->removeFromParent();
	
	CCSprite* card_img = CCSprite::create("ending_take_card_back.png");
	card_img->setScale(0.34f);
	card_img->setPosition(ccp(99.f,156.f));
	
	CCLabelTTF* t_label = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_failLoadInfo), mySGD->getFont().c_str(), 20);
	t_label->setColor(ccBLACK);
	t_label->setPosition(ccp(160,215));
	card_img->addChild(t_label);
	
	loading_parent->addChild(card_img);
}
void SumranMailPopup::startDownloadCardImage ()
{
	CCLOG("start download card img");
	ing_download_cnt = 1;
	startDownload();
}
void SumranMailPopup::startDownload ()
{
	CCLOG("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
	
	StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename,
												  this, callfunc_selector(SumranMailPopup::successAction), this, callfunc_selector(SumranMailPopup::failAction));
}

void SumranMailPopup::removeMessage(int mailNo, long long memberID, std::function<void(Json::Value)> userFunction)
{
	Json::Value p;
	p["giftBoxNo"] = mailNo;
	p["memberID"] = memberID;
	// 도전장 삭제요청
	hspConnector::get()->command
	(
	 "confirmgiftboxhistory",p,
	 
	 [=](Json::Value r)
	 {
		 Json::Value newMailList;
		 if(r["result"]["code"].asInt() != GDSUCCESS)
		 {
			 if(userFunction != nullptr)
				 userFunction(r);
			 return;
		 }
		 

		 {
			 //테이블에서 없어진것 없애기
			 for(int i=0;i<m_mailList.size();i++){
				 if(m_mailList[i]["no"].asInt() != mailNo){
					 newMailList.append(m_mailList[i]);
				 }
			 }
			 //테이블 리로드
			 m_mailList=newMailList;
			 this->filterWithMailFilter();
			 this->mailTableView->reloadData();
			 
		 }
		 if(userFunction != nullptr)
			 userFunction(r);
	 }
	 );
}

void SumranMailPopup::removeMessageByList(vector<int> mailNo, long long memberID, std::function<void(Json::Value)> userFunction)
{
	Json::Value p;
	for(auto no : mailNo)
	{
		p["noList"].append(no);
	}
	p["memberID"] = memberID;
	// 도전장 삭제요청
	hspConnector::get()->command
		(
		 "removemessagebylist",p,
		 [=](Json::Value r)
		 {
			 Json::Value newMailList;
			 if(r["result"]["code"].asInt() != GDSUCCESS){
				 if(userFunction != nullptr)
					 userFunction(r);
				 return;
			 }
			 //테이블에서 없어진것 없애기
			 for(int i=0;i<m_mailList.size();i++){
				 int tempNo = m_mailList[i]["no"].asInt();
				 if(std::find(mailNo.begin(), mailNo.end(), tempNo) == mailNo.end()){
					 newMailList.append(m_mailList[i]);
				 }
			 }
			 //테이블 리로드
			 m_mailList = newMailList;
			 this->filterWithMailFilter();
			 this->mailTableView->reloadData();


			 if(userFunction != nullptr){
				 userFunction(r);
			 }
		 }
	);
}
void SumranMailPopup::filterWithMailFilter()
{
	KS::KSLog("%", m_mailList);
	m_filteredMailList.clear();
	if(m_mailFilter == SumranMailFilter::kTotal)
	{
		m_filteredMailList = m_mailList;
	}
	else
	{
		for(int i=0; i<m_mailList.size(); i++)
		{
			int mailType = m_mailList[i]["type"].asInt();
			if(m_mailFilter == SumranMailFilter::kHeart)
			{
				if(mailType == 1)
				{
					m_filteredMailList.append(m_mailList[i]);
				}	
			}
			else if(m_mailFilter == SumranMailFilter::kChallenge)
			{
				if(mailType == 2 || mailType == 3)
				{
					m_filteredMailList.append(m_mailList[i]);
				}	
			}
			else if(m_mailFilter == SumranMailFilter::kHelp)
			{
				if(mailType == 4 || mailType == 5)
				{
					m_filteredMailList.append(m_mailList[i]);
				}	
			}
			else if(m_mailFilter == SumranMailFilter::kTicket)
			{
				if(mailType == 6 || mailType == 7)
				{
					m_filteredMailList.append(m_mailList[i]);
				}	
			}
			else if(m_mailFilter == SumranMailFilter::kInvite)
			{
				if(mailType == 8)
				{
					m_filteredMailList.append(m_mailList[i]);
				}	
			}
			else if(m_mailFilter == SumranMailFilter::kNews)
			{
				if(mailType == 9)
				{
					m_filteredMailList.append(m_mailList[i]);
				}	
			}
			else if(m_mailFilter == SumranMailFilter::kUnknownFriendRequest)
			{
				if(mailType == 10)
				{
					m_filteredMailList.append(m_mailList[i]);
				}	
			}
		}
	}

	if(m_mailFilter == SumranMailFilter::kTotal)
	{
		KS::KSLog("%", m_mailList);
	}
	m_nothingMessage->setVisible(m_filteredMailList.size() == 0);
	allReceive->setEnabled(m_filteredMailList.size() != 0);
}
#undef LZZ_INLINE
