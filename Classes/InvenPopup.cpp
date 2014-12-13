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

	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p8), goodsToImageFile(kGoodsType_pass8), mySGD->getGoodsValue(kGoodsType_pass8)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p1), goodsToImageFile(kGoodsType_pass1), mySGD->getGoodsValue(kGoodsType_pass1)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p1), goodsToImageFile(kGoodsType_pass1), mySGD->getGoodsValue(kGoodsType_pass1)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p1), goodsToImageFile(kGoodsType_pass1), mySGD->getGoodsValue(kGoodsType_pass1)));
	m_itemList.push_back(HasItem(getLocal(LK::kMyLocalKey_p1), goodsToImageFile(kGoodsType_pass1), mySGD->getGoodsValue(kGoodsType_pass1)));
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
	m_infoList.push_back(MyInfo(string("<font color=#FFFFFF>") + getLocal(LK::kMyLocalKey_highStage) + "</font>", "<font color=#FF8c8c>" + boost::lexical_cast<std::string>(mySGD->mySGD->getUserdataHighPiece()) + "</font>"));
	m_infoList.push_back(MyInfo(string("<font color=#FFFFFF>") + getLocal(LK::kMyLocalKey_playCount) + "</font>", "<font color=#FF8c8c>" + boost::lexical_cast<std::string>(playCount) + "</font>"));
	m_infoList.push_back(MyInfo(string("<font color=#FFFFFF>") + getLocal(LK::kMyLocalKey_pvpHighVictory) + "</font>", "<font color=#FF8c8c>" + boost::lexical_cast<std::string>(mySGD->getUserdataEndlessVictory()) + "</font>"));
	m_infoList.push_back(MyInfo(string("<font color=#FFFFFF>") + getLocal(LK::kMyLocalKey_pvpHighScore) + "</font>", "<font color=#FF8c8c>" + boost::lexical_cast<std::string>(mySGD->getUserdataEndlessScore()) + "</font>"));
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
		addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){
			main_case->setScaleY(t);
		}, [=](float t){
			main_case->setScaleY(1.2f);
			addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){
				main_case->setScaleY(t);
			}, [=](float t){
				main_case->setScaleY(0.f);
				CharacterSelectPopup* t_popup = CharacterSelectPopup::create();
				t_popup->setHideFinalAction(this, callfunc_selector(InvenPopup::reOpenPopup));
				addChild(t_popup, 9);
			}));
		}));
	}
	else if(tag == myCard)
	{
		int zorder = getZOrder();
		auto parent = getParent();
		auto tf = target_final;
		auto df = delegate_final;
		
		hidePopup();
		
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
		t_popup->setHideFinalAction(target_final, callfunc_selector(MainFlowScene::showInvenPopup));
		getParent()->addChild(t_popup, kMainFlowZorder_popup);
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
		cellBack->addChild(descImg);
		descImg->setPosition(ccpFromSize(cellBack->getContentSize()) / 2.f + ccp(0, 2.5));
		setFormSetter(descImg);
		
		KSLabelTTF* cntLabel = KSLabelTTF::create(boost::lexical_cast<std::string>(m_itemList[i].itemCount).c_str(),
																							mySGD->getFont().c_str(),
																							11.f);
		cellBack->addChild(cntLabel);
		cntLabel->setPosition(ccpFromSize(cellBack->getContentSize()) / 2.f + ccp(0, -19.0));
		setFormSetter(cntLabel);
	
		
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


