//
//  InvenPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 3. 27..
//
//

#include "InvenPopup.h"
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
#include "CardSettingPopup.h"
#include "MainFlowScene.h"
#include "CharacterSelectPopup.h"
#include "CardGachaPopup.h"
#include "CardComposePopup.h"
#include "LoadingLayer.h"
#include "ConvexGraph.h"
#include "ASPopupView.h"

#include <boost/lexical_cast.hpp>
void InvenPopup::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}



// on "init" you need to initialize your instance
bool InvenPopup::init()
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
	
	KSLabelTTF* title_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_inventory), mySGD->getFont().c_str(), 12);
	title_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0, 2));
	title_back->addChild(title_label);
	
	CommonAnimation::applyShadow(title_label);
	
	
	CCScale9Sprite* main_inner_left = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	
	main_inner_left->setContentSize(CCSizeMake(524.f / 2.f, 394.f / 2.f));
	main_inner_left->setPosition(ccp(321.5, 129.5));
	setFormSetter(main_inner_left);
	main_case->addChild(main_inner_left);
	
	CCScale9Sprite* main_inner_right = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	m_main_inner_right = main_inner_right;
	main_inner_right->setContentSize(CCSizeMake(312.f / 2.f, 394.f / 2.f));
	main_inner_right->setPosition(ccp(107.0, 129.5));
	setFormSetter(main_inner_right);
	main_case->addChild(main_inner_right);
	
	
	CommonButton* close_menu = CommonButton::createCloseButton(-185);
	close_menu->setPosition(ccpFromSize(main_case->getContentSize()) + ccp(-20,-12));
	close_menu->setFunction([=](CCObject* sender)
													{
														if(!is_menu_enable)
															return;
														
														is_menu_enable = false;
														AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
														hidePopup();
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
	
	
	CCScale9Sprite* tipBack = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	tipBack->setPosition(ccp(main_case->getContentSize().width*0.655f, main_case->getContentSize().height+2-23.5f));
	main_case->addChild(tipBack, 2);
	
	CCSprite* title_back2 = CCSprite::create("tabbutton_up.png");
	title_back2->setPosition(ccp(main_case->getContentSize().width*0.417f, main_case->getContentSize().height+2-25));
	main_case->addChild(title_back2, 3);
	
	tipBack->setContentSize(CCSizeMake(278, 26));
	KSLabelTTF* tipLbl = KSLabelTTF::create("TIP", mySGD->getFont().c_str(), 14.f);
	tipLbl->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	
	tipLbl->getTexture()->setAliasTexParameters();
	//tipLbl->setColor(ccc3(255, 155, 0));
	title_back2->addChild(tipLbl, 4);
	tipLbl->setPosition(ccpFromSize(title_back2->getContentSize()) / 2.f + ccp(0,1));
	
	CCRect mission_size = CCRectMake(0, 0, 225, 22);
	CCPoint mission_position = ccp(main_case->getContentSize().width/2.f+97, main_case->getContentSize().height+2-23.5f);
	LabelTTFMarquee* tipMaquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), mission_size.size.width, mission_size.size.height, "");
	tipMaquee->addText(getLocal(LK::kMyLocalKey_haveItemDetailShow));
	tipMaquee->setPosition(mission_position);
	tipMaquee->startMarquee();
	tipMaquee->setFontSize(12.f);
	tipMaquee->setAnchorPoint(ccp(0.5f, 0.5f));
	main_case->addChild(tipMaquee, 2);
	setFormSetter(tipBack);
	setFormSetter(tipMaquee);
	
	
	CCScale9Sprite* rightPurple = CCScale9Sprite::create("mainpopup_purple.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
	rightPurple->setContentSize(CCSizeMake(302 / 2.f, 382 / 2.f));
	rightPurple->setPosition(ccpFromSize(main_inner_right->getContentSize()) / 2.f);
	main_inner_right->addChild(rightPurple);
	
	
	

	
	// getUserData 로 가입일시 , pvp 최고연승, pvp 최고점수
	/*
	 obj[LK::kMyLocalKey_p1] = "부활석";
	 obj[LK::kMyLocalKey_p2] = "맵뽑기권";
	 obj[LK::kMyLocalKey_p3] = "업그레이드권";
	 obj[LK::kMyLocalKey_p4] = "아이템뽑기권";
	 obj[LK::kMyLocalKey_p5] = "99프로뽑기권";
	 obj[LK::kMyLocalKey_p6] = "생명의돌";
	 obj[LK::kMyLocalKey_p7] = "일반뽑기권";
	 obj[LK::kMyLocalKey_p8] = "고급뽑기권";
	 */
	// 순서는 9 6, 11 , p1 p2 ~p8
	// item9, item6, item11 의 개수는
	mySGD->getGoodsValue(GoodsType::kGoodsType_begin);
	
	int item_cnt = mySGD->getGoodsValue(mySGD->getItemCodeToGoodsType(ITEM_CODE::kIC_addTime)); // 이게 개수임...
	// 그림파일은 CCSprite* AttendancePopup::getGoodsType(string t_type) 이거 참고...
	
	m_itemTable = CCTableView::create(this, CCSizeMake(550 / 2.f, 394 / 2.f));
	m_itemTable->setPosition(ccp(6.0, -1.0));
	m_itemTable->setAnchorPoint(CCPointZero);
	m_itemTable->setDirection(kCCScrollViewDirectionVertical);
	m_itemTable->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_itemTable->setTouchPriority(-181);
	m_itemTable->setDelegate(this);
	main_inner_left->addChild(m_itemTable);
	setFormSetter(m_itemTable);
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_baseSpeedUpItem), goodsToImageFile(kGoodsType_item9), mySGD->getGoodsValue(kGoodsType_item9)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_doubleItem), goodsToImageFile(kGoodsType_item6), mySGD->getGoodsValue(kGoodsType_item6)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_magneticItem), goodsToImageFile(kGoodsType_item11), mySGD->getGoodsValue(kGoodsType_item11)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p1), goodsToImageFile(kGoodsType_pass1), mySGD->getGoodsValue(kGoodsType_pass1)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p2), goodsToImageFile(kGoodsType_pass2), mySGD->getGoodsValue(kGoodsType_pass2)));

	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p3), goodsToImageFile(kGoodsType_pass3), mySGD->getGoodsValue(kGoodsType_pass3)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p4), goodsToImageFile(kGoodsType_pass4), mySGD->getGoodsValue(kGoodsType_pass4)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p5), goodsToImageFile(kGoodsType_pass5), mySGD->getGoodsValue(kGoodsType_pass5)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p6), goodsToImageFile(kGoodsType_pass6), mySGD->getGoodsValue(kGoodsType_pass6)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p7), goodsToImageFile(kGoodsType_pass7), mySGD->getGoodsValue(kGoodsType_pass7)));

	/*
	 obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass9] = "<font newline=15>카드뽑기권을 %d개를 드립니다.";
	 obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass10] = "<font newline=15>캐릭강화권을 %d개를 드립니다.";
	 obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass11] = "<font newline=15>카드환생권을 %d개를 드립니다.";
	 obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass12] = "<font newline=15>쩌리권을 %d개를 드립니다.";
	 */
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p8), goodsToImageFile(kGoodsType_pass8), mySGD->getGoodsValue(kGoodsType_pass8)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p9), goodsToImageFile(kGoodsType_pass9), mySGD->getGoodsValue(kGoodsType_pass9)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p10), goodsToImageFile(kGoodsType_pass10), mySGD->getGoodsValue(kGoodsType_pass10)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p11), goodsToImageFile(kGoodsType_pass11), mySGD->getGoodsValue(kGoodsType_pass11)));
	m_itemList.push_back(HasItem("", "", mySGD->getGoodsValue(kGoodsType_pass1)));
	m_itemTable->reloadData();
	
	
	///////////////////
	
	auto left_character_ccb = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, mySGD->getSelectedCharacterHistory().characterIndex.getV()) + ".ccbi");
	CCSprite* left_character_img = left_character_ccb.first;
	left_character_img->setScale(0.4f);
	left_character_img->setPosition(ccp(23.5, 171.5));
	main_inner_right->addChild(left_character_img);
	
	//	left_character_ccb.second->runAnimationsForSequenceNamed("move_down");
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
	
	CCSprite* left_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(myDSH->getStringForKey(kDSH_Key_flag)).c_str());
	left_flag->setScale(0.7f);
	main_inner_right->addChild(left_flag);
	left_flag->setPosition(ccp(52.0, 177.5));
	
	KSLabelTTF* left_nick = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 13);
	main_inner_right->addChild(left_nick);
	left_nick->setPosition(ccp(65.0, 176.5));
	left_nick->setAnchorPoint(ccp(0.0, 0.5));
	setFormSetter(left_character_img);
	setFormSetter(left_flag);
	setFormSetter(left_nick);
	///////////////////////////////
	
	CCScale9Sprite* darkPurple = CCScale9Sprite::create("mainpopup_darkpurple.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
	darkPurple->setContentSize(CCSizeMake(286 / 2.f, 308 / 2.f));
	rightPurple->addChild(darkPurple);
	darkPurple->setPosition(ccpFromSize(rightPurple->getContentSize()) / 2.f + ccp(0, -14));
	setFormSetter(darkPurple);
	
	m_infoTable = CCTableView::create(this, CCSizeMake(286 / 2.f, 305 / 2.f));
	m_infoTable->setPosition(ccp(10.5, 7.5));
	m_infoTable->setAnchorPoint(CCPointZero);
	m_infoTable->setDirection(kCCScrollViewDirectionVertical);
	m_infoTable->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_infoTable->setTouchPriority(-181);
	m_infoTable->setDelegate(this);
	main_inner_right->addChild(m_infoTable, 2);
	setFormSetter(m_infoTable);
	
	//	mySGD->getPieceHistorySize();
	//	mySGD->getPieceHistoryForIndex(i); // 돌면서 플레이 판수 얻어올 수 있음.
	int playCount = 0;
	for(int i=0; i<mySGD->getPieceHistorySize(); i++)
	{
		playCount += mySGD->getPieceHistoryForIndex(i).try_count.getV();
	}
		
	
	m_infoList.push_back(MyInfo(string("<font color=#FFFFFF>") + getLocal(LK::kMyLocalKey_joinDate) + "</font>", "<font color=#FF8c8c>" + std::string(GraphDogLib::dateFormat(getLocal(LK::kMyLocalKey_yearMonthDay), mySGD->join_date)) + "</font>"));
	m_infoList.push_back(MyInfo(string("<font color=#FFFFFF>") + getLocal(LK::kMyLocalKey_highStage) + "</font>", "<font color=#FF8c8c>" + KS::insert_separator(mySGD->mySGD->getUserdataHighPiece()) + "</font>"));
	m_infoList.push_back(MyInfo(string("<font color=#FFFFFF>") + getLocal(LK::kMyLocalKey_playCount) + "</font>", "<font color=#FF8c8c>" + KS::insert_separator(playCount) + "</font>"));
	m_infoList.push_back(MyInfo(string("<font color=#FFFFFF>") + getLocal(LK::kMyLocalKey_pvpHighVictory) + "</font>", "<font color=#FF8c8c>" + KS::insert_separator(mySGD->getUserdataEndlessVictory()) + "</font>"));
	m_infoList.push_back(MyInfo(string("<font color=#FFFFFF>") + getLocal(LK::kMyLocalKey_pvpHighScore) + "</font>", "<font color=#FF8c8c>" + KS::insert_separator(mySGD->getUserdataEndlessScore()) + "</font>"));
	m_infoTable->reloadData();

	//////////////////////////
	CCMenu* tab_menu = CCMenu::create();
	tab_menu->setPosition(CCPointZero);
	main_case->addChild(tab_menu, 1);
	tab_menu->setTouchPriority(-182);
	
	CCSprite* n_mychar_img = CCSprite::create("subbutton_pink.png");
	KSLabelTTF* n_mychar_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_mycharacter), mySGD->getFont().c_str(), 12.5f);
	n_mychar_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_mychar_label->setPosition(ccpFromSize(n_mychar_img->getContentSize()/2.f) + ccp(0,-1));
	n_mychar_img->addChild(n_mychar_label);
	
	CCSprite* s_mychar_img = CCSprite::create("subbutton_pink.png");
	s_mychar_img->setColor(ccGRAY);
	KSLabelTTF* s_mychar_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_mycharacter), mySGD->getFont().c_str(), 12.5f);
	s_mychar_label->setColor(ccGRAY);
	s_mychar_label->disableOuterStroke();
	s_mychar_label->setPosition(ccpFromSize(s_mychar_img->getContentSize()/2.f) + ccp(0,-1));
	s_mychar_img->addChild(s_mychar_label);
	
	CCSprite* d_mychar_img = CCSprite::create("subbutton_pink.png");
	d_mychar_img->setColor(ccGRAY);
	KSLabelTTF* d_mychar_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_mycharacter), mySGD->getFont().c_str(), 12.5f);
	d_mychar_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_mychar_label->setPosition(ccpFromSize(d_mychar_img->getContentSize()/2.f) + ccp(0,-1));
	d_mychar_img->addChild(d_mychar_label);
	
	
	CCMenuItem* mychar_item = CCMenuItemSprite::create(n_mychar_img, s_mychar_img, d_mychar_img, this, menu_selector(ThisClassType::menuAction));
	mychar_item->setTag(myChar);
	mychar_item->setPosition(ccp(395,16));
	tab_menu->addChild(mychar_item);
	
	
	CCSprite* n_mycard_img = CCSprite::create("subbutton_pink.png");
	KSLabelTTF* n_mycard_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_mycard), mySGD->getFont().c_str(), 12.5f);
	n_mycard_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_mycard_label->setPosition(ccpFromSize(n_mycard_img->getContentSize()/2.f) + ccp(0,-1));
	n_mycard_img->addChild(n_mycard_label);
	
	CCSprite* s_mycard_img = CCSprite::create("subbutton_pink.png");
	s_mycard_img->setColor(ccGRAY);
	KSLabelTTF* s_mycard_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_mycard), mySGD->getFont().c_str(), 12.5f);
	s_mycard_label->setColor(ccGRAY);
	s_mycard_label->disableOuterStroke();
	s_mycard_label->setPosition(ccpFromSize(s_mycard_img->getContentSize()/2.f) + ccp(0,-1));
	s_mycard_img->addChild(s_mycard_label);
	
	CCSprite* d_mycard_img = CCSprite::create("subbutton_pink.png");
	d_mycard_img->setColor(ccGRAY);
	KSLabelTTF* d_mycard_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_mycard), mySGD->getFont().c_str(), 12.5f);
	d_mycard_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_mycard_label->setPosition(ccpFromSize(d_mycard_img->getContentSize()/2.f) + ccp(0,-1));
	d_mycard_img->addChild(d_mycard_label);
	
	
	CCMenuItem* mycard_item = CCMenuItemSprite::create(n_mycard_img, s_mycard_img, d_mycard_img, this, menu_selector(ThisClassType::menuAction));
	mycard_item->setTag(myCard);
	mycard_item->setPosition(ccp(293,16));//395
	tab_menu->addChild(mycard_item);
	
	
	CCSprite* n_gacha_img = CCSprite::create("subbutton_violet.png");
	KSLabelTTF* n_gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_cardGacha), mySGD->getFont().c_str(), 12.5f);
	n_gacha_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_gacha_label->setPosition(ccpFromSize(n_gacha_img->getContentSize()/2.f) + ccp(0,-1));
	n_gacha_img->addChild(n_gacha_label);
	
	CCSprite* s_gacha_img = CCSprite::create("subbutton_violet.png");
	s_gacha_img->setColor(ccGRAY);
	KSLabelTTF* s_gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_cardGacha), mySGD->getFont().c_str(), 12.5f);
	s_gacha_label->setColor(ccGRAY);
	s_gacha_label->disableOuterStroke();
	s_gacha_label->setPosition(ccpFromSize(s_gacha_img->getContentSize()/2.f) + ccp(0,-1));
	s_gacha_img->addChild(s_gacha_label);
	
	CCSprite* d_gacha_img = CCSprite::create("subbutton_violet.png");
	d_gacha_img->setColor(ccGRAY);
	KSLabelTTF* d_gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_cardGacha), mySGD->getFont().c_str(), 12.5f);
	d_gacha_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_gacha_label->setPosition(ccpFromSize(d_gacha_img->getContentSize()/2.f) + ccp(0,-1));
	d_gacha_img->addChild(d_gacha_label);
	
	
	CCMenuItem* gacha_item = CCMenuItemSprite::create(n_gacha_img, s_gacha_img, d_gacha_img, this, menu_selector(ThisClassType::menuAction));
	gacha_item->setTag(TagNumber::gacha);
	gacha_item->setPosition(ccp(75,16));
	tab_menu->addChild(gacha_item);
	
	
	CCSprite* n_compose_img = CCSprite::create("subbutton_violet.png");
	KSLabelTTF* n_compose_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_cardCompose), mySGD->getFont().c_str(), 12.5f);
	n_compose_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_compose_label->setPosition(ccpFromSize(n_compose_img->getContentSize()/2.f) + ccp(0,-1));
	n_compose_img->addChild(n_compose_label);
	
	CCSprite* s_compose_img = CCSprite::create("subbutton_violet.png");
	s_compose_img->setColor(ccGRAY);
	KSLabelTTF* s_compose_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_cardCompose), mySGD->getFont().c_str(), 12.5f);
	s_compose_label->setColor(ccGRAY);
	s_compose_label->disableOuterStroke();
	s_compose_label->setPosition(ccpFromSize(s_compose_img->getContentSize()/2.f) + ccp(0,-1));
	s_compose_img->addChild(s_compose_label);
	
	CCSprite* d_compose_img = CCSprite::create("subbutton_violet.png");
	d_compose_img->setColor(ccGRAY);
	KSLabelTTF* d_compose_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_cardCompose), mySGD->getFont().c_str(), 12.5f);
	d_compose_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_compose_label->setPosition(ccpFromSize(d_compose_img->getContentSize()/2.f) + ccp(0,-1));
	d_compose_img->addChild(d_compose_label);
	
	
	CCMenuItem* compose_item = CCMenuItemSprite::create(n_compose_img, s_compose_img, d_compose_img, this, menu_selector(ThisClassType::menuAction));
	compose_item->setTag(TagNumber::compose);
	compose_item->setPosition(ccp(177,16));//395
	tab_menu->addChild(compose_item);
	
	
	return true;
}
#define kZ_CP_img 1000



InvenPopup::~InvenPopup()
{
	hspConnector::get()->removeTarget(this);
}

void InvenPopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
}

void InvenPopup::showPopup()
{
	
	CommonAnimation::openPopup(this, main_case, gray, [=](){
		
	}, bind(&InvenPopup::endShowPopup, this));
}

void InvenPopup::endShowPopup()
{
	is_menu_enable = true;
}

void InvenPopup::hidePopup()
{
	is_menu_enable = false;
	
	
	CommonAnimation::closePopup(this, main_case, gray, [=](){
		
	}, bind(&InvenPopup::endHidePopup, this));
}

void InvenPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}
void InvenPopup::reOpenPopup()
{
	main_case->setScaleY(0.f);
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){
		main_case->setScaleY(t);
	}, [=](float t){ // finish
		main_case->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){
			main_case->setScaleY(t);
		}, [=](float t){ // finish
			main_case->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){
				main_case->setScaleY(t);
			}, [=](float t){ // finish
				main_case->setScaleY(1.f);
				is_menu_enable = true;
			}));}));}));
}
void InvenPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == myChar)
	{
//		addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){
//			main_case->setScaleY(t);
//		}, [=](float t){
//			main_case->setScaleY(1.2f);
//			addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){
//				main_case->setScaleY(t);
//			}, [=](float t){
//				main_case->setScaleY(0.f);
		CharacterSelectPopup* t_popup = CharacterSelectPopup::create();
		t_popup->setHideFinalAction(target_final, delegate_final);//this, callfunc_selector(InvenPopup::reOpenPopup));
		getParent()->addChild(t_popup, getZOrder()); // 이전에는 그냥 addChild 에 z 9 였음
		
		target_final = NULL;
		hidePopup();
//			}));
//		}));
	}
	else if(tag == myCard)
	{
		int zorder = getZOrder();
		auto parent = getParent();
		auto tf = target_final;
		auto df = delegate_final;
		
		mySGD->before_cardsetting = kSceneCode_PuzzleMapScene;
		CardSettingPopup* t_popup = CardSettingPopup::create();
//		t_popup->setHideFinalAction(this, callfunc_selector(ThisClassType::reOpenPopup));
#if 0 // 이것은 가상의 예상 코드.
		t_popup->setHideFinalAction([=](){
			InvenPopup* t_popup = InvenPopup::create();
			parent->addChild(t_popup, zorder);
			t_popup->setHideFinalAction(tf, df);
		});
#endif
		t_popup->setHideFinalAction(target_final, delegate_final);//callfunc_selector(MainFlowScene::showInvenPopup));
		getParent()->addChild(t_popup, kMainFlowZorder_popup);
		
		target_final = NULL;
		hidePopup();
	}
	else if(tag == TagNumber::gacha)
	{
		card_loading = LoadingLayer::create(-9999);
		addChild(card_loading, 9999);
		card_loading->startLoading();
		
		myHSP->command("getcardgachalist", Json::Value(), [=](Json::Value result_data)
					   {
						   if(result_data["result"]["code"].asInt() == GDSUCCESS)
						   {
							   mySGD->card_gacha_no = result_data["no"].asInt();
							   mySGD->card_gacha_msg = result_data["msg"].asString();
							   mySGD->card_gacha_list.clear();
							   card_download_list.clear();
							   
							   Json::Value cards = result_data["cardlist"];
							   for(int i=0;i<cards.size();i++)
							   {
								   Json::Value t_card = cards[i];
								   KSProtectVar<int> t_gacha_card_no;
								   t_gacha_card_no = t_card["no"].asInt();
								   mySGD->card_gacha_list.push_back(t_gacha_card_no);
								   NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
								   NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
								   if(NSDS_GI(kSDS_CI_int1_version_i, t_card["no"].asInt()) >= t_card["version"].asInt())
									   continue;
								   NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
								   NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
								   NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
								   NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
								   NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
								   
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
									   
									   auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
									   if(iter == card_download_list.end())
									   {
										   card_download_list.push_back(t_df);
										   // ================================
									   }
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
										   
										   auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
										   if(iter == card_download_list.end())
											   card_download_list.push_back(t_df);
										   // ================================
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
										   
										   auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
										   if(iter == card_download_list.end())
											   card_download_list.push_back(t_df);
										   // ================================
									   }
								   }
								   
								   Json::Value t_faceInfo = t_card["faceInfo"];
								   if(!t_faceInfo.isNull() && t_faceInfo.asString() != "" && t_faceInfo.asString() != " ")
								   {
									   NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
									   NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
									   
									   if(NSDS_GS(kSDS_CI_int1_faceInfoCcbi_s, t_card["no"].asInt()) != (t_faceInfo["ccbiID"].asString() + ".ccbi"))
									   {
										   DownloadFile t_df1;
										   t_df1.size = t_faceInfo["size"].asInt();
										   t_df1.img = t_faceInfo["ccbi"].asString().c_str();
										   t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
										   t_df1.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str(), t_card["no"].asInt());
										   card_download_list.push_back(t_df1);
									   }
									   
									   if(NSDS_GS(kSDS_CI_int1_faceInfoPlist_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".plist"))
									   {
										   DownloadFile t_df2;
										   t_df2.size = t_faceInfo["size"].asInt();
										   t_df2.img = t_faceInfo["plist"].asString().c_str();
										   t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
										   t_df2.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str(), t_card["no"].asInt());
										   card_download_list.push_back(t_df2);
									   }
									   
									   if(NSDS_GS(kSDS_CI_int1_faceInfoPvrccz_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".pvr.ccz"))
									   {
										   DownloadFile t_df3;
										   t_df3.size = t_faceInfo["size"].asInt();
										   t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
										   t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
										   t_df3.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str(), t_card["no"].asInt());
										   card_download_list.push_back(t_df3);
									   }
								   }
								   else
								   {
									   NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), false, false);
								   }
								   mySDS->fFlush(t_card["piece"].asInt(), kSDS_SI_base);
							   }
							   
							   mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
							   
							   if(card_download_list.size() > 0)
							   {
								   card_progress_timer = ConvexGraph::create("loading_progress_front2.png", CCRectMake(0, 0, 13, 13), CCRectMake(6, 6, 1, 1), CCSizeMake(201, 13), ConvexGraphType::width);
								   card_progress_timer->setPosition(ccp(240,100));
								   card_loading->addChild(card_progress_timer);
								   
								   card_progress_timer->setCover("loading_progress_front1.png", "loading_progress_mask.png");
								   card_progress_timer->setBack("loading_progress_back.png");
								   
								   ing_download_cnt = 1;
								   success_download_cnt = 0;
								   
								   download_set.clear();
								   is_enable_index.clear();
								   for(int i=0;i<5;i++)
								   {
									   is_enable_index.push_back(i);
								   }
								   
								   download_type = TagNumber::gacha;
								   startFileDownloadSet();
							   }
							   else
							   {
								   card_loading->removeFromParent();
								   
								   CardGachaPopup* t_popup = CardGachaPopup::create(-200);
								   t_popup->setHideFinalAction(target_final, delegate_final);
								   getParent()->addChild(t_popup, getZOrder());
								   
								   target_final = NULL;
								   hidePopup();
							   }
						   }
						   else
						   {
							   is_menu_enable = true;
							   card_loading->removeFromParent();
							   addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_endlessServerError)), 999);
						   }
					   });
	}
	else if(tag == TagNumber::compose)
	{
		card_loading = LoadingLayer::create(-9999);
		addChild(card_loading, 9999);
		card_loading->startLoading();
		
		Json::Value param;
		param["version"] = NSDS_GI(kSDS_GI_cardCompose_version_i);
		
		myHSP->command("getcardcomposelist", param, [=](Json::Value result_data)
					   {
						   if(result_data["result"]["code"].asInt() == GDSUCCESS)
						   {
							   keep_version = result_data["version"].asInt();
							   
							   mySGD->card_compose_list.clear();
							   card_download_list.clear();
							   
							   Json::Value t_list = result_data["list"];
							   NSDS_SI(kSDS_GI_cardCompose_listCnt_i, t_list.size(), false);
							   for(int i=0;i<t_list.size();i++)
							   {
								   Json::Value t_info_json = t_list[i];
								   CardComposeInfo t_info;
								   t_info.compose_no = t_info_json["no"].asInt();
								   t_info.title = t_info_json["title"].asString();
								   t_info.msg = t_info_json["msg"].asString();
								   t_info.need_exp = t_info_json["needExp"].asInt();
								   t_info.compose_card_number = t_info_json["cardNo"].asInt();
								   t_info.need_stone = t_info_json["needStone"].asInt();
								   
								   NSDS_SI(kSDS_GI_cardCompose_list_int1_no_i, i+1, t_info_json["no"].asInt(), false);
								   NSDS_SS(kSDS_GI_cardCompose_list_int1_title_s, i+1, t_info_json["title"].asString(), false);
								   NSDS_SS(kSDS_GI_cardCompose_list_int1_msg_s, i+1, t_info_json["msg"].asString(), false);
								   NSDS_SI(kSDS_GI_cardCompose_list_int1_needExp_i, i+1, t_info_json["needExp"].asInt(), false);
								   NSDS_SI(kSDS_GI_cardCompose_list_int1_cardNo_i, i+1, t_info_json["cardNo"].asInt(), false);
								   NSDS_SI(kSDS_GI_cardCompose_list_int1_needStone_i, i+1, t_info_json["needStone"].asInt(), false);
								   
								   Json::Value material = t_info_json["materialCards"];
								   NSDS_SI(kSDS_GI_cardCompose_list_int1_materialCardsCnt_i, i+1, material.size(), false);
								   t_info.material_card_list.clear();
								   for(int j=0;j<material.size();j++)
								   {
									   KSProtectVar<int> t_p;
									   t_p = material[j].asInt();
									   t_info.material_card_list.push_back(t_p);
									   NSDS_SI(kSDS_GI_cardCompose_list_int1_materialCards_int2_no_i, i+1, j+1, material[j].asInt(), false);
								   }
								   
								   mySGD->card_compose_list.push_back(t_info);
								   
								   Json::Value t_card = t_info_json["cardInfo"];
								   NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
								   NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
								   if(NSDS_GI(kSDS_CI_int1_version_i, t_card["no"].asInt()) >= t_card["version"].asInt())
									   continue;
								   NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
								   NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
								   NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
								   NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
								   NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
								   
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
									   
									   auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
									   if(iter == card_download_list.end())
									   {
										   card_download_list.push_back(t_df);
										   // ================================
									   }
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
										   
										   auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
										   if(iter == card_download_list.end())
											   card_download_list.push_back(t_df);
										   // ================================
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
										   
										   auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
										   if(iter == card_download_list.end())
											   card_download_list.push_back(t_df);
										   // ================================
									   }
								   }
								   
								   Json::Value t_faceInfo = t_card["faceInfo"];
								   if(!t_faceInfo.isNull() && t_faceInfo.asString() != "" && t_faceInfo.asString() != " ")
								   {
									   NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
									   NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
									   
									   if(NSDS_GS(kSDS_CI_int1_faceInfoCcbi_s, t_card["no"].asInt()) != (t_faceInfo["ccbiID"].asString() + ".ccbi"))
									   {
											 if(t_faceInfo["ccbiID"].asString() == "")
											 {
												 CCLog("null");
											 }
											 
										   DownloadFile t_df1;
										   t_df1.size = t_faceInfo["size"].asInt();
										   t_df1.img = t_faceInfo["ccbi"].asString().c_str();
										   t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
										   t_df1.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str(), t_card["no"].asInt());
										   card_download_list.push_back(t_df1);
									   }
									   
									   if(NSDS_GS(kSDS_CI_int1_faceInfoPlist_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".plist"))
									   {
										   DownloadFile t_df2;
										   t_df2.size = t_faceInfo["size"].asInt();
										   t_df2.img = t_faceInfo["plist"].asString().c_str();
										   t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
										   t_df2.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str(), t_card["no"].asInt());
										   card_download_list.push_back(t_df2);
									   }
									   
									   if(NSDS_GS(kSDS_CI_int1_faceInfoPvrccz_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".pvr.ccz"))
									   {
										   DownloadFile t_df3;
										   t_df3.size = t_faceInfo["size"].asInt();
										   t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
										   t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
										   t_df3.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str(), t_card["no"].asInt());
										   card_download_list.push_back(t_df3);
									   }
								   }
								   else
								   {
									   NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), false, false);
								   }
								   mySDS->fFlush(t_card["piece"].asInt(), kSDS_SI_base);
							   }
							   
							   mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
							   mySDS->fFlush(kSDS_GI_base);
							   
							   if(card_download_list.size() > 0)
							   {
								   card_progress_timer = ConvexGraph::create("loading_progress_front2.png", CCRectMake(0, 0, 13, 13), CCRectMake(6, 6, 1, 1), CCSizeMake(201, 13), ConvexGraphType::width);
								   card_progress_timer->setPosition(ccp(240,100));
								   card_loading->addChild(card_progress_timer);
								   
								   card_progress_timer->setCover("loading_progress_front1.png", "loading_progress_mask.png");
								   card_progress_timer->setBack("loading_progress_back.png");
								   
								   ing_download_cnt = 1;
								   success_download_cnt = 0;
								   
								   download_set.clear();
								   is_enable_index.clear();
								   for(int i=0;i<5;i++)
								   {
									   is_enable_index.push_back(i);
								   }
								   
								   download_type = TagNumber::compose;
								   startFileDownloadSet();
							   }
							   else
							   {
								   NSDS_SI(kSDS_GI_cardCompose_version_i, keep_version);
								   
								   card_loading->removeFromParent();
								   
								   CardComposePopup* t_popup = CardComposePopup::create(-200);
								   t_popup->setHideFinalAction(target_final, delegate_final);
								   getParent()->addChild(t_popup, getZOrder());
								   
								   target_final = NULL;
								   hidePopup();
							   }
						   }
						   else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
						   {
							   mySGD->card_compose_list.clear();
							   
							   int list_cnt = NSDS_GI(kSDS_GI_cardCompose_listCnt_i);
							   for(int i=0;i<list_cnt;i++)
							   {
								   CardComposeInfo t_info;
								   t_info.compose_no = NSDS_GI(kSDS_GI_cardCompose_list_int1_no_i, i+1);
								   t_info.title = NSDS_GS(kSDS_GI_cardCompose_list_int1_title_s, i+1);
								   t_info.msg = NSDS_GS(kSDS_GI_cardCompose_list_int1_msg_s, i+1);
								   t_info.need_exp = NSDS_GI(kSDS_GI_cardCompose_list_int1_needExp_i, i+1);
								   t_info.compose_card_number = NSDS_GI(kSDS_GI_cardCompose_list_int1_cardNo_i, i+1);
								   t_info.need_stone = NSDS_GI(kSDS_GI_cardCompose_list_int1_needStone_i, i+1);
								   
								   int mate_cnt = NSDS_GI(kSDS_GI_cardCompose_list_int1_materialCardsCnt_i, i+1);
								   for(int j=0;j<mate_cnt;j++)
								   {
									   KSProtectVar<int> t_p;
									   t_p = NSDS_GI(kSDS_GI_cardCompose_list_int1_materialCards_int2_no_i, i+1, j+1);
									   t_info.material_card_list.push_back(t_p);
								   }
								   
								   mySGD->card_compose_list.push_back(t_info);
							   }
							   
							   card_loading->removeFromParent();
							   
							   CardComposePopup* t_popup = CardComposePopup::create(-200);
							   t_popup->setHideFinalAction(target_final, delegate_final);
							   getParent()->addChild(t_popup, getZOrder());
							   
							   target_final = NULL;
							   hidePopup();
						   }
						   else
						   {
							   is_menu_enable = true;
							   card_loading->removeFromParent();
							   addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_endlessServerError)), 999);
						   }
					   });
	}
}

void InvenPopup::startFileDownloadSet()
{
	int max_thread_cnt = 5;
	int total_download_cnt = card_download_list.size();
	int before_download_size = download_set.size();
	
	for(int i=download_set.size();ing_download_cnt <= total_download_cnt && i<max_thread_cnt;i++)
	{
		DownloadImgInfo t_info;
		if(card_download_list.size() > 0 && ing_download_cnt <= card_download_list.size())
		{
			t_info.download_url = card_download_list[ing_download_cnt-1].img;
			t_info.download_size = card_download_list[ing_download_cnt-1].size;
			t_info.download_filename = card_download_list[ing_download_cnt-1].filename;
		}
		else
		{
			break;
		}
		
		t_info.is_fail = false;
		t_info.is_end = false;
		download_set.push_back(t_info);
		
		ing_download_cnt++;
	}
	
	if(download_set.size() > 0)
	{
		for(int i=before_download_size;i<download_set.size();i++)
		{
			int will_download_index = is_enable_index.back();
			CCLOG("start download idx : %d / filename : %s", will_download_index, download_set[i].download_filename.c_str());
			mySIL->downloadImg(download_set[i], will_download_index);
			is_enable_index.pop_back();
		}
		if(!is_downloading)
		{
			is_downloading = true;
			schedule(schedule_selector(InvenPopup::checkDownloading));
		}
	}
	else
	{
		unschedule(schedule_selector(InvenPopup::checkDownloading));
		
		
		for(int i=0;i<card_download_list.size();i++)
		{
			SDS_SS(kSDF_cardInfo, card_download_list[i].key,
				   card_download_list[i].img, false);
		}
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		card_loading->removeFromParent();
		
		if(download_type == TagNumber::gacha)
		{
			CardGachaPopup* t_popup = CardGachaPopup::create(-200);
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, getZOrder());
		}
		else if(download_type == TagNumber::compose)
		{
			NSDS_SI(kSDS_GI_cardCompose_version_i, keep_version);
			
			CardComposePopup* t_popup = CardComposePopup::create(-200);
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, getZOrder());
		}
		
		target_final = NULL;
		hidePopup();
	}
}

void InvenPopup::checkDownloading()
{
	int max_thread_cnt = 5;
	bool is_successed = false;
	for(int i=0;i<max_thread_cnt;i++)
	{
		if(mySIL->downloading_list[i].is_end)
		{
			if(mySIL->downloading_list[i].is_fail)
			{
				CCLOG("fail download idx : %d / filename : %s", i, mySIL->downloading_list[i].download_filename.c_str());
				auto iter = find(download_set.begin(), download_set.end(), DownloadImgInfo(mySIL->downloading_list[i].download_filename));
				if(iter != download_set.end())
				{
					CCLOG("start download idx : %d / filename : %s", i, iter->download_filename.c_str());
					mySIL->downloadImg((*iter), i);
				}
				else
				{
					CCLOG("not found fail set");
				}
			}
			else
			{
				CCLOG("success download idx : %d / filename : %s", i, mySIL->downloading_list[i].download_filename.c_str());
				auto iter = find(download_set.begin(), download_set.end(), DownloadImgInfo(mySIL->downloading_list[i].download_filename));
				if(iter != download_set.end())
				{
					download_set.erase(iter);
				}
				else
				{
					CCLOG("not found success set");
				}
				is_enable_index.push_back(i);
				success_download_cnt++;
				is_successed = true;
			}
		}
	}
	
	float download_percent = 100.f*success_download_cnt/int(card_download_list.size());
	
	card_progress_timer->setPercentage(download_percent);
	
	if(is_successed || success_download_cnt >= card_download_list.size())
	{
		startFileDownloadSet();
	}
}

CCTableViewCell* InvenPopup::itemTableCellAtIndex(CCTableView *table, unsigned int idx)
{
	
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();

	
	// 0, 5, 10,
	int row = idx;
	
	int startIdx = row * 5;
	if(startIdx >= m_itemList.size())
		return cell;
	int endIdx = MIN(m_itemList.size(), startIdx + 4);
	int col = 0;
	for(int i=startIdx; i<=endIdx; i++, col++)
	{
		CCSprite* cellBack = CCSprite::create("inven_cell.png");
		cellBack->setPosition(ccp(col * cellBack->getContentSize().width, 0));
		cellBack->setAnchorPoint(CCPointZero);
		
		KSLabelTTF* desc = KSLabelTTF::create(m_itemList[i].itemDesc.c_str(), mySGD->getFont().c_str(), 8.f);
		cellBack->addChild(desc);
		desc->setPosition(ccpFromSize(cellBack->getContentSize()) / 2.f + ccp(0, 19.5));
		
		setFormSetter(desc);
		CCSprite* descImg = CCSprite::create(m_itemList[i].itemFile.c_str());
		if(descImg)
		{
			cellBack->addChild(descImg);
			descImg->setPosition(ccpFromSize(cellBack->getContentSize()) / 2.f + ccp(0, 2.5));
			setFormSetter(descImg);
			
			KSLabelTTF* cntLabel = KSLabelTTF::create(boost::lexical_cast<std::string>(m_itemList[i].itemCount).c_str(),
																								mySGD->getFont().c_str(),
																								11.f);
			cellBack->addChild(cntLabel);
			cntLabel->setPosition(ccpFromSize(cellBack->getContentSize()) / 2.f + ccp(0, -19.0));
			setFormSetter(cntLabel);
			
		}
		
		
	
		
		cell->addChild(cellBack);
	}
	
	
	CCLog("table!!");
	return cell;
}

CCTableViewCell* InvenPopup::infoTableCellAtIndex(CCTableView *table, unsigned int idx)
{
	
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CCSprite* center_line = CCSprite::create("common_line.png");
	center_line->setScaleX(270 / 2.f);
	center_line->setAnchorPoint(CCPointZero);

	StyledLabelTTF* title = StyledLabelTTF::create(m_infoList[idx].styledTitle.c_str(), mySGD->getFont().c_str(),
																								 9.f,
																								 999,
																								 StyledAlignment::kCenterAlignment);
	title->setAnchorPoint(ccp(0.5f, 0.0));
	title->setPosition(270 / 2.f / 2.f, 17);
	cell->addChild(center_line);
	cell->addChild(title);
//	title->setPosition(ccpFromSize(cell->getContentSize()) / 2.f + ccp(0, 0));
	
	StyledLabelTTF* con = StyledLabelTTF::create(m_infoList[idx].styledContent.c_str(), mySGD->getFont().c_str(),
																								 11.f,
																								 999,
																								 StyledAlignment::kCenterAlignment);
	con->setPosition(270 / 2.f / 2.f, 3.5);
	con->setAnchorPoint(ccp(0.5f, 0.0));
	cell->addChild(con);
//	con->setPosition(ccpFromSize(cell->getContentSize()) / 2.f + ccp(0, 0));
	
	setFormSetter(title);
	setFormSetter(con);
	
	
	
	return cell;
}
CCTableViewCell* InvenPopup::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	if(table == m_itemTable)
	{
		return itemTableCellAtIndex(table, idx);
	}
	else if(table == m_infoTable)
	{
		return infoTableCellAtIndex(table, idx);
	}
	
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	return cell;
}



void  InvenPopup::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	
}

CCSize  InvenPopup::cellSizeForTable(CCTableView *table)
{
	if(table == m_itemTable)
		return CCSizeMake(230, 64);
	else if(table == m_infoTable)
		return CCSizeMake(286 / 2.f, 315.f / 2.f / 5.f);
	
	return CCSizeMake(230, 34);
}
unsigned int  InvenPopup::numberOfCellsInTableView(CCTableView *table)
{
	if(table == m_itemTable)
	{
		return ceilf(m_itemList.size() / 5.f);
	}
	else if(table == m_infoTable)
	{
		return m_infoList.size();
	}
	else
	{
		return 0;
	}
}


void InvenPopup::scrollViewDidScroll(CCScrollView* view)
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
void InvenPopup::scrollViewDidZoom(CCScrollView* view){}


