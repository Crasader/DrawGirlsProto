
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
#include "KSLocal.h"
#include "LoadingLayer.h"
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
	m_touchPriority = -200;
	
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

	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_giftbox), mySGD->getFont().c_str(), 15);
	title_label->disableOuterStroke();
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0, 2));
	setFormSetter(title_label);
	title_back->addChild(title_label);
	
	CommonAnimation::applyShadow(title_label);

	
	m_nothingMessage = CCLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_nogift), mySGD->getFont().c_str(), 14);
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
	_menu->setPosition(CCPointZero);
	_menu->setTouchPriority(m_touchPriority);
	main_case->addChild(_menu, 4);
	CCSprite* n_allReward_img = CCSprite::create("subbutton_pink.png");
	KSLabelTTF* n_allReward_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_allAccept), mySGD->getFont().c_str(), 12.5f);
	n_allReward_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	n_allReward_label->setPosition(ccpFromSize(n_allReward_img->getContentSize()/2.f) + ccp(0,-1));
	n_allReward_img->addChild(n_allReward_label);
	
	CCSprite* s_allReward_img = CCSprite::create("subbutton_pink.png");
	s_allReward_img->setColor(ccGRAY);
	KSLabelTTF* s_allReward_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_allAccept), mySGD->getFont().c_str(), 12.5f);
	s_allReward_label->setColor(ccGRAY);
	s_allReward_label->disableOuterStroke();
	s_allReward_label->setPosition(ccpFromSize(s_allReward_img->getContentSize()/2.f) + ccp(0,-1));
	s_allReward_img->addChild(s_allReward_label);
	
	CCSprite* d_allReward_img = GraySprite::create("subbutton_pink.png");
	((GraySprite*)d_allReward_img)->setGray(true);
	KSLabelTTF* d_allReward_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_allAccept), mySGD->getFont().c_str(), 12.5f);
	d_allReward_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	d_allReward_label->setPosition(ccpFromSize(d_allReward_img->getContentSize()/2.f) + ccp(0,-1));
	d_allReward_img->addChild(d_allReward_label);
	
	
	allReceive = CCMenuItemSpriteLambda::create(n_allReward_img, s_allReward_img, d_allReward_img,
																							std::bind(&ThisClassType::takeAllReward, this, std::placeholders::_1));
//	allReceive->setTag(kAchievePopupMenuTag_allReward);
	allReceive->setPosition(ccp(395,16));
	_menu->addChild(allReceive);
//	allReceive = CommonButton::create(CCSprite::create("subbutton_pink.png"), m_touchPriority);
//	
//	allReceive->setTitleSize(12.f);
//	allReceive->setTitle(myLoc->getLocalForKey(LK::kMyLocalKey_allAccept));
//	allReceive->getTitleLabel()->enableOuterStroke(ccBLACK, 0.5f, 150, true);
////	allReceive->setBackgroundTypeForDisabled(CommonButtonGray);
//	allReceive->setTitleColor(ccc3(255, 255, 255));
//	allReceive->setTitleColorForDisable(ccc3(90, 60, 30));
//	//allReceive->setBackgroundTypeForDisabled(CommonButtonGray);
	
	
//	allReceive->setFunction([=](CCObject*));
	//FormSetter::get()->addObject("testksoo2", allReceive);
	
//	allReceive->setPosition(ccp(395.0, 13.5));
	allReceive->setEnabled(false);
	setFormSetter(allReceive);
//	main_case->addChild(allReceive, 1);

	auto giftBoxAlertBox = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
	giftBoxAlertBox->setPosition(ccp(326.0, 246.5));
	giftBoxAlertBox->setContentSize(CCSizeMake(224.5, 19.5));
	main_case->addChild(giftBoxAlertBox);

	
	KSLabelTTF* giftboxAlert = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_giftboxAlert), mySGD->getFont().c_str(), 12);
	giftboxAlert->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	setFormSetter(giftboxAlert);
	setFormSetter(giftBoxAlertBox);
	giftboxAlert->setAnchorPoint(ccp(0,0.5));
	giftboxAlert->setPosition(ccpFromSize(giftBoxAlertBox->getContentSize()) / 2.f + ccp(-94.5, 1));
	giftBoxAlertBox->addChild(giftboxAlert, 1);
	
	
	CommonButton* giftFilter = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_ticketBox), 12, CCSizeMake(65,38), CommonButtonGray, m_touchPriority); // ? 티켓함?이 맞음? From YH To KS
	CommonButton* helpFilter = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_helpBox), 12, CCSizeMake(65,38), CommonButtonGray, m_touchPriority);
	CommonButton* ticketFilter = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_ticketBox), 12, CCSizeMake(65,38), CommonButtonGray, m_touchPriority);
	CommonButton* challengeFilter = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_challengeBox), 12, CCSizeMake(65,38), CommonButtonGray, m_touchPriority);
	CommonButton* coinFilter = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_heartBox), 12, CCSizeMake(65,38), CommonButtonGray, m_touchPriority);
	CommonButton* totalFilter = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_allView), 12, CCSizeMake(65,38), CommonButtonGray, m_touchPriority);
	
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
	
	CommonButton* closeBtn = CommonButton::createCloseButton(m_touchPriority);
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
void SumranMailPopup::takeAllReward(CCObject* sender)
{
	{
		
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
						
						Json::Value newList = Json::Value(Json::arrayValue);
						
						for(int i=0;i<m_mailList.size();i++){
							Json::Value info = m_mailList[i];
							if(info.get("reward","").asString()=="" || info.get("exchangeList","").asString()!=""){
								newList.append(info);
							}
						}
						m_mailList.swap(newList);
						
						
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
	}
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

	mailTableView = CCTableView::create(this, CCSizeMake(455.f, 203.f));
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
	mailTableView->setPosition(ccp(33.0, 30.0));
	
	//데이터를 가져오고나 터치 이벤트를 반환해줄 대리자를 이 클래스로 설정.
	mailTableView->setDelegate(this);
//	this->addChild(mailTableView, kMP_Z_mailTable);
	main_case->addChild(mailTableView, kMP_Z_mailTable);
	mailTableView->setTouchPriority(m_touchPriority);

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
	CCTableViewCell* realCell = table->dequeueCell();
	
	realCell = new CCTableViewCell();
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
		}else if(mail["reward"].isArray() && mail["reward"].size()>1){
			presentIcon = CCSprite::create("postbox_present.png");
		}else{
			presentIcon = CCSprite::create("postbox_friendinvite.png");
		}
		profileImg->addChild(presentIcon);
		presentIcon->setPosition(ccpFromSize(profileImg->getContentSize()) / 2.f);
		//profileImg->setScale(30.f / profileImg->getContentSize().width);
		cell->addChild(profileImg, kMP_Z_profileImg);
		setFormSetter(profileImg);

		CCMenuLambda* _menu = CCMenuLambda::create();
		_menu->setPosition(ccp(0, 0));
		_menu->setTouchPriority(m_touchPriority);
		_menu->setTag(kMP_MT_send);
		cell->addChild(_menu, kMP_MT_getheart);


		
		string titlestr = mail.get("content","Gift").asString();
		ReplaceString(titlestr,"\n"," ");
		title = KSLabelTTF::create(titlestr.c_str(), mySGD->getFont().c_str(), 13); // "님의"
		title->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		title->setPosition(ccp(53.5, 19.5));
		title->setAnchorPoint(CCPointZero);
		title->setTag(kMP_MT_title);
		setFormSetter(title);
		cell->addChild(title,2);
		if(title->getContentSize().width>100){
			title->removeFromParent();
			LabelTTFMarquee *title2 = LabelTTFMarquee::create(ccc4(255, 255, 255, 255), 100, 13, "");
			title2->addText("<font size=13 color=#ffffff strokecolor=#000000 strokesize=0.5 strokeopacity=200>"+titlestr+"</font>");
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
				
				comment = GraphDogLib::dateFormat("m/d H:i",mail.get("regDate","Event").asString().c_str());
				
				
				btnReceive = CommonButton::create(CCSprite::create("subbutton_purple.png"), m_touchPriority);
				setFormSetter(btnReceive);
				btnReceive->setTitle(myLoc->getLocalForKey(LK::kMyLocalKey_take));
				btnReceive->setTitleSize(12.f);
				btnReceive->setTouchPriority(m_touchPriority);
				btnReceive->setTitleColor(ccc3(255, 255, 255));
				btnReceive->getTitleLabel()->enableOuterStroke(ccBLACK, 0.5f, 150, true);
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
																			ASPopupView *alert = ASPopupView::getCommonNoti(-9999,myLoc->getLocalForKey(LK::kMyLocalKey_fail), myLoc->getLocalForKey(LK::kMyLocalKey_failedConnect),[=](){
																				AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																			});
																			this->addChild(alert, 2000);
																			return;
																		}
																		
																		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		
																		CCNode* itemlist = CCNode::create();
																		CCScrollView* rewardScroll = CCScrollView::create();
																		setFormSetter(itemlist);
																		
																		string from = CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_giftboxContent),
																																						 mail.get("content","Gift").asString().c_str(),
																																						// mail.get("sender","GM").asString().c_str(),
																																						 GraphDogLib::dateFormat("m/d H:i",mail.get("regDate","Unkown Date").asString().c_str()).c_str()
																																						 )->getCString();
																		StyledLabelTTF* lbl;
																		
																		if(mail["reward"].isArray()){
																			for(int i=0;i<mail["reward"].size();i++){
																				string rewardType = mail["reward"][i].get("type","box").asString();
																				if(rewardType=="fr" || rewardType=="pr")rewardType="r";
																				int rewardCount = mail["reward"][i].get("count",1).asInt();
																				
																				CCScale9Sprite* back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
																				back->setContentSize(CCSizeMake(70, 70));
																				back->setPosition(ccp(i*75+75/2.f,0));
																				
																				CCLOG("position : %f",back->getPositionX());
																				
																				CCSprite* spr;
																				KSLabelTTF* rewardName = KSLabelTTF::create("",mySGD->getFont().c_str(), 13);
																				if(rewardType=="fr" || rewardType=="pr" || rewardType=="r")rewardName->setString("젬");
																				else if(rewardType=="g")rewardName->setString("골드");
																				else if(rewardType=="h")rewardName->setString("하트");
																				else if(rewardType=="i6")rewardName->setString("더블");
																				else if(rewardType=="i9")rewardName->setString("신발");
																				else if(rewardType=="i11")rewardName->setString("자석");
																				else if(rewardType=="p1")rewardName->setString("부활석");
																				else if(rewardType=="p2")rewardName->setString("맵뽑기권");
																				else if(rewardType=="p3")rewardName->setString("업그레이드권");
																				else if(rewardType=="p4")rewardName->setString("아이템뽑기권");
																				else if(rewardType=="p5")rewardName->setString("99프로뽑기권");
																				else if(rewardType=="p6")rewardName->setString("생명의돌");
																				else if(rewardType=="p7")rewardName->setString("일반뽑기권");
																				else if(rewardType=="p8")rewardName->setString("고급뽑기권");
																				else if(rewardType=="cd")rewardName->setString("");
																				else rewardName->setString("??");
																				
																				KSLabelTTF* count = KSLabelTTF::create(CCString::createWithFormat("x%d",rewardCount)->getCString(), mySGD->getFont().c_str(), 13);
																				if(rewardType=="cd"){
																					spr = mySIL->getLoadedImg(ccsf("card%d_visible.png", rewardCount));// mySIL->getUnsafeLoadedImg(CCString::createWithFormat("card%d_thumbnail.png",rewardCount)->getCString());
																					spr->setScale(0.4f*0.3f);
																					count->setString("");
																					spr->setPosition(ccp(back->getContentSize().width/2.f,back->getContentSize().height/2.f));
																				}else{
																					spr= CCSprite::create(CCString::createWithFormat("icon_%s.png",rewardType.c_str())->getCString());
                                                                                    
                                                                                    if(!spr)spr = CCSprite::create("icon_box.png");
                                                                                    
																					spr->setPosition(ccp(back->getContentSize().width/2.f,back->getContentSize().height/2.f));
																				}
																				
																				
																				
																				count->setPosition(ccp(back->getContentSize().width/2.f,16));
																				rewardName->setPosition(ccp(back->getContentSize().width/2.f,53));
																				setFormSetter(back);
																				setFormSetter(spr);
																				setFormSetter(count);
																				back->addChild(spr);
																				back->addChild(count);
																				back->addChild(rewardName);
																				itemlist->addChild(back);
																				
																			}
																			
																			itemlist->setContentSize(CCSizeMake((mail["reward"].size()*75)-2.5f/2.f, 50));
																			lbl  = StyledLabelTTF::create(from.c_str(), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
																			
																			if(itemlist->getContentSize().width<251){
																				rewardScroll->setTouchEnabled(false);
																			}else{
																				rewardScroll->setDirection(CCScrollViewDirection::kCCScrollViewDirectionHorizontal);
																			}
																			
																		}else{ //if(mail["reward"].isObject())
																			//일반텍스트일때
																			KSLabelTTF* label = KSLabelTTF::create(mail["content"].asString().c_str(), mySGD->getFont().c_str(), 13);
																			
																			itemlist->addChild(label);
																			
																			from = CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_giftboxContent),
																																				"     ",
																																				GraphDogLib::dateFormat("m/d H:i",mail.get("regDate","Unkown Date").asString().c_str()).c_str()
																																				)->getCString();
																			
																			lbl  = StyledLabelTTF::create(from.c_str(), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
																			
																			itemlist->setContentSize(CCSizeMake((mail["reward"].size()*75)-2.5f/2.f, label->getContentSize().height));
																			
																			
																			if(itemlist->getContentSize().height>60){
																				rewardScroll->setDirection(CCScrollViewDirection::kCCScrollViewDirectionVertical);
																			}else{
																				rewardScroll->setTouchEnabled(false);
																			}
																		}
																		
																		CCLOG("contentsize is %f",itemlist->getContentSize().width);
																		//itemlist->setPosition(ccp(0,40));
																		//itemlist->setAnchorPoint(ccp(0,0));
																		
																		CCSize size = itemlist->getContentSize();
																		rewardScroll->setContentSize(size);
																		rewardScroll->setViewSize(CCSizeMake(251, 80));
																		rewardScroll->setContainer(itemlist);
																		rewardScroll->setAnchorPoint(ccp(0,0));
																		rewardScroll->setTouchPriority(-999999);
																		rewardScroll->setPosition(ccp(0,53));
																		
																		CCLOG("contentoffset1 is %f,%f",rewardScroll->getContentOffset().x,rewardScroll->getContentOffset().y);
																		
																		rewardScroll->setContentOffset(ccp(251.f/2.f-itemlist->getContentSize().width/2.f-5.f/2.f,40));
																		
																		
																		CCLOG("contentoffset2 is %f,%f",rewardScroll->getContentOffset().x,rewardScroll->getContentOffset().y);
																		
																		setFormSetter(rewardScroll);
																		
																		
																		
																		
																		ASPopupView* managerPopup = ASPopupView::createDimmed(-9999);
																		managerPopup->getDimmedSprite()->setVisible(false);
																		addChild(managerPopup);
																		
																		
																		//	auto back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
																		auto back = CCSprite::create("popup_large_back.png");
																		
																		auto front = CCScale9Sprite::create("common_grayblue.png",
																																				CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
																		CommonAnimation::openPopup(managerPopup, back, managerPopup->getDimmedSprite(), nullptr, nullptr);
												
																		managerPopup->setContainerNode(back);
																		front->setContentSize(CCSizeMake(251, 370 / 2.f-50));
																		
																		
																		back->addChild(front);
																		
																		front->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(0,8));
																		
																		front->addChild(rewardScroll);
																		setFormSetter(front);
																		
																		setFormSetter(itemlist);
																		KSLabelTTF* titleLbl = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_confirmGift), mySGD->getFont().c_str(), 12.f);
																		titleLbl->setAnchorPoint(ccp(0.5f,0.5f));
																		titleLbl->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(-85, back->getContentSize().height/2.f-35));
																		back->addChild(titleLbl);
																		setFormSetter(titleLbl);
																		
																		
																		
																		
																		lbl->setPosition(ccp(front->getContentSize().width/2.f,48));
																		front->addChild(lbl);
																		setFormSetter(lbl);
																		
																		
																		Json::Value btnInfo = this->getButton(mail);
																		
																		
																		float width = (btnInfo.size()+1)*110-10;
																		float x = -width/2.f;
																		for (int buttonIndex=0; buttonIndex<btnInfo.size(); buttonIndex++) {
																			
																			x += 110;
																			
																			CommonButton* close_button = CommonButton::create(btnInfo[buttonIndex].asString().c_str(), 12, CCSizeMake(101, 44), CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0, 0, 101, 44), CCRectMake(50, 21, 1, 2)), managerPopup->getTouchPriority()-5);
																			close_button->setPosition(ccp(x+150, 47.0));
																			back->addChild(close_button);
																			
																			
																			close_button->setFunction([=](CCObject* sender)
																																{
																																	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																																	CommonAnimation::closePopup(managerPopup, back,
																																															managerPopup->getDimmedSprite(), nullptr,
																																															[=](){
																																																
																																																///////////////////////////////
																																																
																																																
																																																
																																																
																																																
																																																AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																																																
																																																Json::Value p;
																																																int mailNo = mail["no"].asInt();
																																																
																																																p["giftBoxNo"] = mailNo;
																																																p["memberID"] = mail["memberID"].asInt64();
																																																
																																																t_suction->setTouchEnabled(true);
																																																t_suction->setVisible(true);
																																																//삭제요청
																																																this->removeMessage (mailNo, mail["memberID"].asInt64(),[=](Json::Value r)
																																																										 {
																																																											 this->confirmMessage(buttonIndex, mail);
																																																											 
																																																											 mySGD->saveChangeGoodsTransaction(r);
																																																											 takedCheck(r["list"],[=](){
																																																												 
																																																												 t_suction->setTouchEnabled(false);
																																																												 t_suction->setVisible(false);
																																																												 
																																																												 
																																																											 });
																																																										 });
																																																
																																																
																																																
																																																
																																																
																																																//////////////////////////////
																																																
																																																
																																																
																																																
																																																
																																																
																																																
																																																
																																																managerPopup->removeFromParent();
																																															});
																																	
																																	
																																	
																																	
																																	
																																});
																		}
																		
																		
																		
																		//managerPopup->setContentSize(managerPopup->getContentSize() + CCSizeMake(0, -53));
																		//managerPopup->setPositionY(managerPopup->getPositionY() + 22.5f);
																		
																		
//																		
//																		
//																		ASPopupView *alert = ASPopupView::getCommonNoti(-9999,mail.get("content","Gfit").asString(), itemlist,[=](){
//																			
//																			AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
//																			
//																			Json::Value p;
//																			int mailNo = mail["no"].asInt();
//																			
//																			p["giftBoxNo"] = mailNo;
//																			p["memberID"] = mail["memberID"].asInt64();
//																			
//																			t_suction->setTouchEnabled(true);
//																			t_suction->setVisible(true);
//																			//삭제요청
//																			this->removeMessage (mailNo, mail["memberID"].asInt64(),
//																													 [=](Json::Value r)
//																													 {
//																														 mySGD->saveChangeGoodsTransaction(r);
//																														 takedCheck(r["list"],[=](){
//																															 
//																															 t_suction->setTouchEnabled(false);
//																															 t_suction->setVisible(false);
//																															 
//																															 
//																														 });
//																													 });
//																			
//																		});
//																		
//																		this->addChild(alert, 2000);
//																		
//																		
//																	///////////////////////////////////////////////////////////////////
//																	});
																	
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
		Json::Value card_param;
		int cnt = -1;
		for(int i=0;i<reward.size();i++){
			if(reward[i].get("type","box").asString()=="cd"){
				cnt++;
				card_param["noList"][cnt] =reward[i]["count"].asInt();
			}
		}
	
	
		if(cnt>-1){
			card_download_list.clear();
//			card_reduction_list.clear();
			download_card_no = card_param["noList"][0].asInt();
			end_func_download_card = func;
			myHSP->command("getcardlist", card_param, json_selector(this, SumranMailPopup::resultGetCardInfo));
			return;
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

//void SumranMailPopup::cardDown(int cardNo,std::function<void(bool)>func){
//	//카드정보 다운로드하기, 다운성공시 func(true); , 실패시 func(false) 호출
//	
//	// 카드 정보 받기
//	card_download_list.clear();
//	card_reduction_list.clear();
//	download_card_no = cardNo;
//	end_func_download_card = func;
//	Json::Value card_param;
//	card_param["noList"][0] = cardNo;
//	myHSP->command("getcardlist", card_param, json_selector(this, SumranMailPopup::resultGetCardInfo));
//}

void SumranMailPopup::resultGetCardInfo(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		
		Json::Value cards = result_data["list"];
		for(int i=0;i<cards.size();i++)
		{
			Json::Value t_card = cards[i];
			if(NSDS_GI(kSDS_CI_int1_version_i, t_card["no"].asInt()) >= t_card["version"].asInt())
				continue;
			NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
			NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
			
//			NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
			NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
			
//			Json::Value t_card_missile = t_card["missile"];
//			NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str(), false);
//			NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt(), false);
//			NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble(), false);
//			
//			NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str(), false);
//			
//			Json::Value t_ability = t_card["ability"];
//			NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), int(t_ability.size()), false);
//			for(int j=0;j<t_ability.size();j++)
//			{
//				Json::Value t_abil = t_ability[j];
//				NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt(), false);
//				
//				Json::Value t_option;
//				if(!t_abil["option"].isObject())                    t_option["key"]="value";
//				else												t_option =t_abil["option"];
//				
//				if(t_abil["type"].asInt() == kIC_attack)
//					NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_addTime)
//					NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_fast)
//					NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_silence)
//					NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_doubleItem)
//					NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_longTime)
//					NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_baseSpeedUp)
//					NSDS_SI(kSDS_CI_int1_abilityBaseSpeedUpOptionUnit_i, t_card["no"].asInt(), t_option["unit"].asInt(), false);
//			}
			
			NSDS_SB(kSDS_CI_int1_haveAdult_b, t_card["no"].asInt(), t_card["haveAdult"].asBool(), false);
			NSDS_SI(kSDS_CI_int1_exp_i, t_card["no"].asInt(), t_card["exp"].asInt(), false);
			
			Json::Value t_imgInfo = t_card["imgInfo"];
			
//			bool is_add_cf = false;
			
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
				
//				CopyFile t_cf;
//				t_cf.from_filename = t_df.filename.c_str();
//				t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//				card_reduction_list.push_back(t_cf);
//				
//				is_add_cf = true;
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
				
//				if(is_add_cf)
//				{
//					CopyFile t_cf = card_reduction_list.back();
//					card_reduction_list.pop_back();
//					t_cf.is_ani = true;
//					t_cf.cut_width = t_detail["cutWidth"].asInt();
//					t_cf.cut_height = t_detail["cutHeight"].asInt();
//					t_cf.position_x = t_detail["positionX"].asInt();
//					t_cf.position_y = t_detail["positionY"].asInt();
//					t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
//					card_reduction_list.push_back(t_cf);
//				}
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
			
			Json::Value t_faceInfo = t_card["faceInfo"];
			if(!t_faceInfo.isNull() && t_faceInfo.asString() != "" && NSDS_GS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt()) != (t_faceInfo["ccbiID"].asString() + ".ccbi"))
			{
				NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
//				NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
				
				DownloadFile t_df1;
				t_df1.size = t_faceInfo["size"].asInt();
				t_df1.img = t_faceInfo["ccbi"].asString().c_str();
				t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
				t_df1.key = mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str();
				card_download_list.push_back(t_df1);
				
				DownloadFile t_df2;
				t_df2.size = t_faceInfo["size"].asInt();
				t_df2.img = t_faceInfo["plist"].asString().c_str();
				t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
				t_df2.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str();
				card_download_list.push_back(t_df2);
				
				DownloadFile t_df3;
				t_df3.size = t_faceInfo["size"].asInt();
				t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
				t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
				t_df3.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str();
				card_download_list.push_back(t_df3);
				
//				if(!is_add_cf)
//				{
//					CopyFile t_cf;
//					t_cf.from_filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
//					t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//					card_reduction_list.push_back(t_cf);
//					
//					is_add_cf = true;
//				}
//				
//				CopyFile t_cf = card_reduction_list.back();
//				card_reduction_list.pop_back();
//				t_cf.is_ccb = true;
//				t_cf.ccb_filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
//				
//				card_reduction_list.push_back(t_cf);
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
			
			ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
				Json::Value card_param;
				card_param["noList"][0] = download_card_no.getV();
				myHSP->command("getcardlist", card_param, json_selector(this, SumranMailPopup::resultGetCardInfo));
			}, 1);
			if(alert)
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
		CCLOG("%d : %s", ing_card_download, card_download_list[ing_card_download-1].filename.c_str());
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
	if(ing_card_download < card_download_list.size())
	{
		SDS_SS(kSDF_cardInfo, card_download_list[ing_card_download-1].key,
			   card_download_list[ing_card_download-1].img, false);
		ing_card_download++;
		startCardDownload();
	}
	else if(ing_card_download == card_download_list.size())
	{
		SDS_SS(kSDF_cardInfo, card_download_list[ing_card_download-1].key,
			   card_download_list[ing_card_download-1].img, false);
//		for(int i=0;i<card_reduction_list.size();i++)
//		{
//			mySIL->removeTextureCache(card_reduction_list[i].from_filename);
//			mySIL->removeTextureCache(card_reduction_list[i].to_filename);
//			
//			CCSprite* target_img = new CCSprite();
//			target_img->initWithTexture(mySIL->addImage(card_reduction_list[i].from_filename.c_str()));
//			target_img->setAnchorPoint(ccp(0,0));
//			
////			if(card_reduction_list[i].is_ani)
////			{
////				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(card_reduction_list[i].ani_filename.c_str()),
////																CCRectMake(0, 0, card_reduction_list[i].cut_width, card_reduction_list[i].cut_height));
////				ani_img->setPosition(ccp(card_reduction_list[i].position_x, card_reduction_list[i].position_y));
////				target_img->addChild(ani_img);
////			}
////			
////			if(card_reduction_list[i].is_ccb)
////			{
////				CCSprite* face_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + card_reduction_list[i].ccb_filename.c_str()).first;
////				face_img->setPosition(ccpFromSize(target_img->getContentSize()/2.f));
////				target_img->addChild(face_img);
////			}
//			
//			target_img->setScale(0.4f);
//			
//			CCRenderTexture* t_texture = new CCRenderTexture();
//			t_texture->initWithWidthAndHeight(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY(), kCCTexture2DPixelFormat_RGBA8888, 0);
//			t_texture->setSprite(target_img);
//			t_texture->beginWithClear(0, 0, 0, 0);
//			t_texture->getSprite()->visit();
//			t_texture->end();
//			
//			if(!(t_texture->saveToFileNoAlpha(card_reduction_list[i].to_filename.c_str(), kCCImageFormatPNG)))
//                CCLOG("failed!!! card reduce : %s", card_reduction_list[i].to_filename.c_str());
//			
//			t_texture->release();
//			target_img->release();
//			
//			if(i % 3 == 0)
//			{
//				CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//			}
//		}
		
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		ing_card_download++;
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
		
		ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
			startCardDownload();
		}, 1);
		if(alert)
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
				
				ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
					takedCard(cardNo, t_end_func);
				}, 1);
				if(alert)
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
	
//	if(NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, t_card_number))
//	{
//		CCSprite* ccb_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, t_card_number)).first;
//		ccb_img->setPosition(ccp(160,215));
//		card_img->addChild(ccb_img);
//	}
	
	CardCase* t_case = CardCase::create(t_card_number, t_card_level, t_passive);
	t_case->setPosition(CCPointZero);
	card_img->addChild(t_case);
	
	CCLabelTTF* recent_durability_label = t_case->getRecentDurabilityLabel();
//	recent_durability_label->setString(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_durability_i, t_card_number))->getCString());
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
			if(NSDS_GI(kSDS_CI_int1_version_i, t_card["no"].asInt()) >= t_card["version"].asInt())
				continue;
			NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
			NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
			
//			NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
			NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
			
//			Json::Value t_card_missile = t_card["missile"];
//			NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str(), false);
//			NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt(), false);
//			NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble(), false);
//			
//			NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str(), false);
//			
//			Json::Value t_ability = t_card["ability"];
//			NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), int(t_ability.size()), false);
//			for(int j=0;j<t_ability.size();j++)
//			{
//				Json::Value t_abil = t_ability[j];
//				NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt(), false);
//				
//				Json::Value t_option;
//				if(!t_abil["option"].isObject())                    t_option["key"]="value";
//				else												t_option =t_abil["option"];
//				
//				if(t_abil["type"].asInt() == kIC_attack)
//					NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_addTime)
//					NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_fast)
//					NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_silence)
//					NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_doubleItem)
//					NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_longTime)
//					NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_baseSpeedUp)
//					NSDS_SI(kSDS_CI_int1_abilityBaseSpeedUpOptionUnit_i, t_card["no"].asInt(), t_option["unit"].asInt(), false);
//			}
			
			NSDS_SB(kSDS_CI_int1_haveAdult_b, t_card["no"].asInt(), t_card["haveAdult"].asBool(), false);
			NSDS_SI(kSDS_CI_int1_exp_i, t_card["no"].asInt(), t_card["exp"].asInt(), false);
			
			Json::Value t_imgInfo = t_card["imgInfo"];
			
//			bool is_add_cf = false;
			
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
				
//				CopyFile t_cf;
//				t_cf.from_filename = t_df.filename.c_str();
//				t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//				cf_list.push_back(t_cf);
//				
//				is_add_cf = true;
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
				
//				if(is_add_cf)
//				{
//					CopyFile t_cf = cf_list.back();
//					cf_list.pop_back();
//					t_cf.is_ani = true;
//					t_cf.cut_width = t_detail["cutWidth"].asInt();
//					t_cf.cut_height = t_detail["cutHeight"].asInt();
//					t_cf.position_x = t_detail["positionX"].asInt();
//					t_cf.position_y = t_detail["positionY"].asInt();
//					
//					t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
//					
//					cf_list.push_back(t_cf);
//				}
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
			
			Json::Value t_faceInfo = t_card["faceInfo"];
			if(!t_faceInfo.isNull() && t_faceInfo.asString() != "" && NSDS_GS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt()) != (t_faceInfo["ccbiID"].asString() + ".ccbi"))
			{
				NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
//				NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
				
				DownloadFile t_df1;
				t_df1.size = t_faceInfo["size"].asInt();
				t_df1.img = t_faceInfo["ccbi"].asString().c_str();
				t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
				t_df1.key = mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str();
				df_list.push_back(t_df1);
				
				DownloadFile t_df2;
				t_df2.size = t_faceInfo["size"].asInt();
				t_df2.img = t_faceInfo["plist"].asString().c_str();
				t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
				t_df2.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str();
				df_list.push_back(t_df2);
				
				DownloadFile t_df3;
				t_df3.size = t_faceInfo["size"].asInt();
				t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
				t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
				t_df3.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str();
				df_list.push_back(t_df3);
				
//				if(!is_add_cf)
//				{
//					CopyFile t_cf;
//					t_cf.from_filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
//					t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//					cf_list.push_back(t_cf);
//					
//					is_add_cf = true;
//				}
//				
//				CopyFile t_cf = cf_list.back();
//				cf_list.pop_back();
//				t_cf.is_ccb = true;
//				t_cf.ccb_filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
//				
//				cf_list.push_back(t_cf);
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
//		for(int i=0;i<cf_list.size();i++)
//		{
//			mySIL->removeTextureCache(cf_list[i].from_filename);
//			mySIL->removeTextureCache(cf_list[i].to_filename);
//			
//			CCSprite* target_img = new CCSprite();
//			target_img->initWithTexture(mySIL->addImage(cf_list[i].from_filename.c_str()));
//			target_img->setAnchorPoint(ccp(0,0));
//			
////			if(cf_list[i].is_ani)
////			{
////				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].ani_filename.c_str()), CCRectMake(0, 0, cf_list[i].cut_width, cf_list[i].cut_height));
////				ani_img->setPosition(ccp(cf_list[i].position_x, cf_list[i].position_y));
////				target_img->addChild(ani_img);
////			}
////			
////			if(cf_list[i].is_ccb)
////			{
////				CCSprite* face_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + cf_list[i].ccb_filename.c_str()).first;
////				face_img->setPosition(ccpFromSize(target_img->getContentSize()/2.f));
////				target_img->addChild(face_img);
////			}
//			
//			target_img->setScale(0.4f);
//			
//			CCRenderTexture* t_texture = new CCRenderTexture();
//			t_texture->initWithWidthAndHeight(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY(), kCCTexture2DPixelFormat_RGBA8888, 0);
//			t_texture->setSprite(target_img);
//			t_texture->begin();
//			t_texture->getSprite()->visit();
//			t_texture->end();
//			
//			if(!(t_texture->saveToFileNoAlpha(card_reduction_list[i].to_filename.c_str(), kCCImageFormatPNG)))
//                CCLOG("failed!!! card reduce : %s", card_reduction_list[i].to_filename.c_str());
//			
//			t_texture->release();
//			target_img->release();
//			
//			if(i % 3 == 0)
//			{
//				CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//			}
//		}
		
		df_list.clear();
//		cf_list.clear();
		
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
	
	CCLabelTTF* t_label = CCLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_failLoadInfo), mySGD->getFont().c_str(), 20);
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

Json::Value SumranMailPopup::getButton(Json::Value mail){
	Json::Value btn;
	
	if(mail["data"].get("friendreq", 0).asInt())
	{
		
		btn[0] = getLocal(LK::kFriendMailReject);
		btn[1] = getLocal(LK::kFriendMailAccept);
	}
	else
	{
		btn[0] = myLoc->getLocalForKey(LK::kMyLocalKey_ok);
	}
	
	return btn;
}

void SumranMailPopup::confirmMessage(int btnIndex,Json::Value mail){
	CCLOG("test->> %s",mail.asString().c_str());
	if(mail["data"].get("friendreq", 0).asInt())
	{
		if(btnIndex == 1)
		{
			// add friend !
			Json::Value param;
			param["memberID"] = myHSP->getMemberID();
			param["friendID"] = mail["data"].get("from", "0").asString();
			LoadingLayer* ll = LoadingLayer::create(m_touchPriority - 100);
			addChild(ll, 100);
			myHSP->command("addfriend", param, [=](Json::Value v){
				ll->removeFromParent();
				if(v["result"]["code"] != GDSUCCESS)
					return;
				if(v["result"]["code"] == GDFRIENDMAX)
				{
					std::string resultMessage = v["result"]["message"].asString();
					// 내가 50명 넘음
					if(resultMessage == "me")
					{
						auto popup = ASPopupView::getCommonNoti(m_touchPriority - 1, "", getLocal(LK::kFriendAddLimitMe), nullptr);
						popup->getDimmedSprite()->setVisible(false);
						addChild(popup); 
					}
					// 상대방이 50명 넘음
					else if (resultMessage == "you")
					{
						auto popup = ASPopupView::getCommonNoti(m_touchPriority - 1, "", getLocal(LK::kFriendAddLimitYou), nullptr);
						popup->getDimmedSprite()->setVisible(false);
						addChild(popup);
					}
				}
				
				
				
				
			});
			
		}
	}
	else
	{
		
	}
	
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
