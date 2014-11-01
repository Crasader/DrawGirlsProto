//
//  ManyGachaPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 10. 26..
//
//

#include "ManyGachaPopup.h"
#include "TouchSuctionLayer.h"
#include "DataStorageHub.h"
#include "KSLabelTTF.h"
#include "KsLocal.h"
#include "StarGoldData.h"
#include "CommonAnimation.h"
#include "CommonButton.h"
#include "AudioEngine.h"
#include "LabelTTFMarquee.h"
#include "LoadingLayer.h"
#include "ASPopupView.h"
#include "StageImgLoader.h"
#include "MainFlowScene.h"
#include "ShopPopup.h"
#include "DownloadFile.h"
#include "GachaDetailPopup.h"
#include "CCMenuLambda.h"

enum ManyGachaPopupMenu
{
	kClose = 0,
	kNormal,
	kPremium,
	kShop
};

void ManyGachaPopup::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

bool ManyGachaPopup::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	
	is_menu_enable = false;
	
	is_opening = false;
	
	loading_type_is_normal = false;
	
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
	addChild(main_case, 1);
	
	CCSprite* title_back = CCSprite::create("title_tab.png");
	title_back->setPosition(ccp(60,main_case->getContentSize().height-13));
	main_case->addChild(title_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_gacha), mySGD->getFont().c_str(), 15);
	title_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,2));
	title_back->addChild(title_label);
	
	CommonAnimation::applyShadow(title_label);
	
	
	CCScale9Sprite* tip_marquee_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	tip_marquee_back->setContentSize(CCSizeMake(278, 26));
	tip_marquee_back->setPosition(ccp(main_case->getContentSize().width*0.655f, main_case->getContentSize().height-21.5f));
	main_case->addChild(tip_marquee_back);
	
	LabelTTFMarquee* tipMarquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 278, 22, "");
	tipMarquee->addText(ccsf(getLocal(LK::kMyLocalKey_gachaListAt4HourTimeAutoRefresh), mySGD->getGachaRefreshTime()));
	tipMarquee->setPosition(ccpFromSize(tip_marquee_back->getContentSize()/2.f));
	tipMarquee->startMarquee();
	tipMarquee->setAnchorPoint(ccp(0.5f,0.5f));
	tip_marquee_back->addChild(tipMarquee);
	
	CCSprite* tipBack = CCSprite::create("tabbutton_up.png");
	tipBack->setPosition(ccp(main_case->getContentSize().width*0.417f, main_case->getContentSize().height-23));
	main_case->addChild(tipBack);
	
	KSLabelTTF* tipLbl = KSLabelTTF::create("TIP", mySGD->getFont().c_str(), 14.f);
	tipLbl->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	tipLbl->setPosition(ccpFromSize(tipBack->getContentSize()) / 2.f + ccp(0,1));
	tipBack->addChild(tipLbl);
	
	CommonButton* close_menu = CommonButton::createCloseButton(-185);
	close_menu->setPosition(ccpFromSize(main_case->getContentSize()) + ccp(-20,-12));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								t_node->setTag(ManyGachaPopupMenu::kClose);
								menuAction(t_node);
							});
	main_case->addChild(close_menu);
	
	main_inner = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	main_inner->setContentSize(CCSizeMake(424, 218));
	main_inner->setPosition(ccp(main_case->getContentSize().width/2.f, main_case->getContentSize().height*0.45f + 3));
	main_case->addChild(main_inner);
	
	inner_node = CCNode::create();
	inner_node->setPosition(ccp(0,0));
	main_inner->addChild(inner_node);

	setOpening();
	
	return true;
}

void ManyGachaPopup::setOpening()
{
	unschedule(schedule_selector(ManyGachaPopup::refreshTimeChecking));
	
	is_opening = true;
	
	inner_node->removeAllChildren();
	
	CCSprite* table_img = CCSprite::create("gacha_table.png");
	table_img->setScaleX(423/table_img->getContentSize().width);
	table_img->setPosition(ccp(main_inner->getContentSize().width/2.f, table_img->getContentSize().height/2.f));
	inner_node->addChild(table_img);
	
	auto normal_ccb = KS::loadCCBI<CCSprite*>(this, "gacha_silverbox.ccbi");
	CCSprite* normal_img = normal_ccb.first;
	normal_img->setPosition(ccp(main_inner->getContentSize().width/2.f-53, 120));
	inner_node->addChild(normal_img);
	
	KSLabelTTF* normal_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_normalGacha), mySGD->getFont().c_str(), 25);
	normal_label->setGradientColor(ccc4(255, 255, 255, 255), ccc4(230, 230, 230, 255), ccp(0,-1));
	normal_label->setPosition(ccp(main_inner->getContentSize().width/2.f-53, 40));
	inner_node->addChild(normal_label);
	
	KSLabelTTF* normal_back = KSLabelTTF::create(getLocal(LK::kMyLocalKey_normalGacha), mySGD->getFont().c_str(), 25);
	normal_back->setColor(ccc3(30, 15, 0));
	normal_back->enableOuterStroke(ccc3(0, 0, 0), 2, 255, true);
	normal_back->setPosition(ccpFromSize(normal_label->getContentSize()/2.f) + ccp(0,1));
	normal_label->addChild(normal_back, -1);
	
	auto premium_ccb = KS::loadCCBI<CCSprite*>(this, "gacha_goldbox.ccbi");
	CCSprite* premium_img = premium_ccb.first;
	premium_img->setPosition(ccp(main_inner->getContentSize().width/2.f+63, 120));
	inner_node->addChild(premium_img);
	
	KSLabelTTF* premium_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_premiumGacha), mySGD->getFont().c_str(), 25);
	premium_label->setGradientColor(ccc4(255, 255, 235, 255), ccc4(255, 205, 70, 255), ccp(0,-1));
	premium_label->setPosition(ccp(main_inner->getContentSize().width/2.f+63, 40));
	inner_node->addChild(premium_label);
	
	KSLabelTTF* premium_back = KSLabelTTF::create(getLocal(LK::kMyLocalKey_premiumGacha), mySGD->getFont().c_str(), 25);
	premium_back->setColor(ccc3(30, 15, 0));
	premium_back->enableOuterStroke(ccc3(0, 0, 0), 2, 255, true);
	premium_back->setPosition(ccpFromSize(premium_label->getContentSize()/2.f) + ccp(0,1));
	premium_label->addChild(premium_back, -1);
	
	CCSprite* left_char = CCSprite::create("gacha_cha1.png");
	left_char->setPosition(ccp(left_char->getContentSize().width/2.f - 9, left_char->getContentSize().height/2.f - 3));
	inner_node->addChild(left_char);
	
	CCSprite* right_char = CCSprite::create("gacha_cha2.png");
	right_char->setPosition(ccp(main_inner->getContentSize().width - right_char->getContentSize().width/2.f + 13, right_char->getContentSize().height/2.f - 3));
	inner_node->addChild(right_char);
	
	CCSprite* left_talkbox = CCSprite::create("gacha_talkbox.png");
	left_talkbox->setPosition(ccp(48,190));
	inner_node->addChild(left_talkbox);
	
	KSLabelTTF* left_talkment = KSLabelTTF::create(getLocal(LK::kMyLocalKey_goodLuckAyameaFighting), mySGD->getFont().c_str(), 11);
	left_talkment->setColor(ccBLACK);
	left_talkment->disableOuterStroke();
	left_talkment->setPosition(ccp(48,195));
	inner_node->addChild(left_talkment);
	
	CCSprite* right_talkbox = CCSprite::create("gacha_talkbox.png");
	right_talkbox->setPosition(ccp(main_inner->getContentSize().width - 40,185));
	right_talkbox->setScale(0.8f);
	right_talkbox->setFlipX(true);
	inner_node->addChild(right_talkbox);
	
	KSLabelTTF* right_talkment = KSLabelTTF::create(getLocal(LK::kMyLocalKey_brotherJustOne), mySGD->getFont().c_str(), 11);
	right_talkment->setColor(ccBLACK);
	right_talkment->disableOuterStroke();
	right_talkment->setPosition(ccp(main_inner->getContentSize().width - 40,190));
	inner_node->addChild(right_talkment);
	
	CCMenu* gacha_menu = CCMenu::create();
	gacha_menu->setPosition(ccp(0,0));
	inner_node->addChild(gacha_menu);
	gacha_menu->setTouchPriority(-181);
	
	CCSprite* n_normal = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 90, 140));
	CCSprite* s_normal = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 90, 140));
//	s_normal->setColor(ccGRAY);
	
	CCMenuItem* normal_item = CCMenuItemSprite::create(n_normal, s_normal, this, menu_selector(ManyGachaPopup::menuAction));
	normal_item->setTag(ManyGachaPopupMenu::kNormal);
	normal_item->setPosition(ccp(main_inner->getContentSize().width/2.f-53, 120));
	gacha_menu->addChild(normal_item);
	
	CCSprite* n_premium = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 90, 140));
	CCSprite* s_premium = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 90, 140));
//	s_premium->setColor(ccGRAY);
	
	CCMenuItem* premium_item = CCMenuItemSprite::create(n_premium, s_premium, this, menu_selector(ManyGachaPopup::menuAction));
	premium_item->setTag(ManyGachaPopupMenu::kPremium);
	premium_item->setPosition(ccp(main_inner->getContentSize().width/2.f+63, 120));
	gacha_menu->addChild(premium_item);
	
	
	CCSprite* n_shop_img = CCSprite::create("subbutton_pink.png");
	KSLabelTTF* n_shop_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_shop), mySGD->getFont().c_str(), 12.5f);
	n_shop_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_shop_label->setPosition(ccpFromSize(n_shop_img->getContentSize()/2.f) + ccp(0,-1));
	n_shop_img->addChild(n_shop_label);
	
	CCSprite* s_shop_img = CCSprite::create("subbutton_pink.png");
	s_shop_img->setColor(ccGRAY);
	KSLabelTTF* s_shop_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_shop), mySGD->getFont().c_str(), 12.5f);
	s_shop_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_shop_label->setPosition(ccpFromSize(s_shop_img->getContentSize()/2.f) + ccp(0,-1));
	s_shop_img->addChild(s_shop_label);
	
	
	CCMenuItem* shop_menu = CCMenuItemSprite::create(n_shop_img, s_shop_img, this, menu_selector(ManyGachaPopup::menuAction));
	shop_menu->setTag(ManyGachaPopupMenu::kShop);
	shop_menu->setPosition(ccp(380,10));
	gacha_menu->addChild(shop_menu);
}

void ManyGachaPopup::refreshTimeChecking()
{
	chrono::time_point<std::chrono::system_clock> now_time = chrono::system_clock::now();
	std::time_t now_time_t = chrono::system_clock::to_time_t(now_time);
	
	time_t sub_time = now_time_t;
	if(loading_type_is_normal)
		sub_time -= myDSH->getIntegerForKey(kDSH_Key_normalGachaDataTime);
	else
		sub_time -= myDSH->getIntegerForKey(kDSH_Key_premiumGachaDataTime);
		
	int expireSec = mySGD->getGachaRefreshTime()*60*60;
	
	int sub_value = expireSec - sub_time;
	if(sub_value < 0)
		sub_value = 0;
	
	refresh_time_label->setString(ccsf("%02d:%02d:%02d", sub_value/60/60, sub_value/60%60, sub_value%60));
}

void ManyGachaPopup::setNormalGacha()
{
	unschedule(schedule_selector(ManyGachaPopup::refreshTimeChecking));
	
	is_opening = false;
	
	loading_type_is_normal = true;
	
	inner_node->removeAllChildren();
	
	CCScale9Sprite* left_back = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	left_back->setContentSize(CCSizeMake(218, 208));
	left_back->setPosition(ccp(left_back->getContentSize().width/2.f+5, left_back->getContentSize().height/2.f+5));
	inner_node->addChild(left_back);
	
	KSLabelTTF* detail_script = KSLabelTTF::create(getLocal(LK::kMyLocalKey_goodsTouchDetailScript), mySGD->getFont().c_str(), 11);
	detail_script->setPosition(ccp(left_back->getContentSize().width/2.f, left_back->getContentSize().height - detail_script->getContentSize().height/2.f - 5));
	left_back->addChild(detail_script);
	
	
	CCScale9Sprite* rest_time_back = CCScale9Sprite::create("option_withdraw.png", CCRectMake(0, 0, 72, 34), CCRectMake(35, 16, 2, 2));
	rest_time_back->setContentSize(CCSizeMake(210, 42));
	rest_time_back->setPosition(ccp(left_back->getContentSize().width/2.f, 20));
	left_back->addChild(rest_time_back);
	
	KSLabelTTF* rest_time_ment = KSLabelTTF::create(getLocal(LK::kMyLocalKey_listRefreshRestTime), mySGD->getFont().c_str(), 9);
	rest_time_ment->setColor(ccBLACK);
	rest_time_ment->setPosition(ccp(50,rest_time_back->getContentSize().height-13));
	rest_time_back->addChild(rest_time_ment);
	
	CCScale9Sprite* time_label_back = CCScale9Sprite::create("attendance_gold_back.png", CCRectMake(0, 0, 43, 17), CCRectMake(21, 8, 1, 1));
	time_label_back->setContentSize(CCSizeMake(80, 17));
	time_label_back->setPosition(ccp(50,15));
	rest_time_back->addChild(time_label_back);
	
	refresh_time_label = KSLabelTTF::create("00:00:00", mySGD->getFont().c_str(), 10);
	refresh_time_label->setPosition(ccpFromSize(time_label_back->getContentSize()/2.f) + ccp(0,-1));
	time_label_back->addChild(refresh_time_label);
	
	refreshTimeChecking();
	schedule(schedule_selector(ManyGachaPopup::refreshTimeChecking), 1.f);
	
	CCLabelTTF* t_refresh_label_node = CCLabelTTF::create();
	CCSprite* t_refresh_icon = CCSprite::create("gacha_refresh.png");
	KSLabelTTF* t_refresh_label_ttf = KSLabelTTF::create(getLocal(LK::kMyLocalKey_nowRefresh), mySGD->getFont().c_str(), 12);
	t_refresh_label_ttf->setPosition(ccp(t_refresh_icon->getContentSize().width/2.f-2,0));
	t_refresh_label_node->addChild(t_refresh_label_ttf);
	t_refresh_icon->setPosition(ccp(-t_refresh_label_ttf->getContentSize().width/2.f-2,0));
	t_refresh_label_node->addChild(t_refresh_icon);
	
	CCControlButton* now_refresh_button = CCControlButton::create(t_refresh_label_node, CCScale9Sprite::create("endless_bt_up.png", CCRectMake(0, 0, 75, 30), CCRectMake(37, 14, 1, 2)));
	now_refresh_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ManyGachaPopup::normalRefresh), CCControlEventTouchUpInside);
	now_refresh_button->setPreferredSize(CCSizeMake(95, 30));
	now_refresh_button->setPosition(ccp(150, rest_time_back->getContentSize().height/2.f+1));
	now_refresh_button->setTouchPriority(-181);
	rest_time_back->addChild(now_refresh_button);
	
	CCPoint base_position = ccp(29, 20 + 48*3);
	
	string list_data = myDSH->getStringForKey(kDSH_Key_normalGachaList);
	
	json_list.clear();
	Json::Reader t_reader;
	t_reader.parse(list_data, json_list);
	
	reward_list.clear();
	enable_gacha_list.clear();
	
	CCMenuLambda* buttons_menu = CCMenuLambda::create();
	buttons_menu->setPosition(ccp(0,0));
	left_back->addChild(buttons_menu);
	
	buttons_menu->setTouchPriority(-181);
	
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
		{
			Json::Value t_data = json_list[j + i*4];
			string back_type;
			int back_type_value = t_data["level"].asInt();
			if(back_type_value == 1)
				back_type = "gacha_cell_silver.png";
			else if(back_type_value == 2)
				back_type = "gacha_cell_purple.png";
			else if(back_type_value == 3)
				back_type = "gacha_cell_pink.png";
			else if(back_type_value == 4)
				back_type = "gacha_cell_gold.png";
			
			CCSprite* n_button = CCSprite::create(back_type.c_str());
			CCSprite* s_button = CCSprite::create(back_type.c_str());
			s_button->setColor(ccGRAY);
			
			CCNode* t_button_node = CCNode::create();
			t_button_node->setContentSize(n_button->getContentSize());
			t_button_node->setPosition(ccpFromSize(t_button_node->getContentSize()/(-2.f)) + base_position + ccp(53*j,-50*i));
			left_back->addChild(t_button_node);
			
			CCMenuItemSpriteLambda* button_item = CCMenuItemSpriteLambda::create(n_button, s_button, [=](CCObject* sender)
																				 {
																					 if(!is_menu_enable)
																						 return;
																					 
																					 is_menu_enable = false;
																					 
																					 AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
																					 
																					 CCScale9Sprite* t_cover = CCScale9Sprite::create("common_select.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
																					 t_cover->setContentSize(CCSizeMake(55, 52));
																					 t_cover->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f));
																					 t_button_node->addChild(t_cover, 9);
																					 
																					 GachaDetailPopup* t_popup = GachaDetailPopup::create(-200, t_data, [=](){
																						 t_cover->removeFromParent();
																						 is_menu_enable = true;});
																					 addChild(t_popup, 999);
																				 });
			button_item->setPosition(base_position + ccp(53*j,-50*i));
			buttons_menu->addChild(button_item);
			
			string reward_type;
			int reward_count;
			if(t_data["reward"].size() > 1)
			{
				reward_type = "many";
				reward_count = t_data["reward"].size();
			}
			else
			{
				reward_type = t_data["reward"][0]["type"].asString();
				reward_count = t_data["reward"][0]["count"].asInt();
			}
			
			if(reward_type == "cp")
			{
//				CCSprite* t_img = CCSprite::create(ccsf("icon_%s.png", reward_type.c_str()));
//				t_img->setPosition(ccpFromSize(t_button->getContentSize()/2.f) + ccp(0,5));
//				t_button->addChild(t_img);
				
				bool is_found = false;
				int character_count = NSDS_GI(kSDS_GI_characterCount_i);
				for(int k=0;!is_found && k<character_count;k++)
				{
					if(reward_count == NSDS_GI(kSDS_GI_characterInfo_int1_no_i, k+1))
					{
						is_found = true;
						
						CCNode* char_node = CCNode::create();
						char_node->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f) + ccp(0,0));
						char_node->setScale(0.4f);
						auto t_pair = KS::loadCCBIForFullPath<CCSprite*>(this, StageImgLoader::sharedInstance()->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, k+1) + ".ccbi");
						CCSprite* char_img = t_pair.first;
						char_img->setPosition(ccp(0,0));
						char_node->addChild(char_img);
						t_button_node->addChild(char_node);
						
						KSLabelTTF* t_count_label = KSLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_name_s, k+1).c_str(), mySGD->getFont().c_str(), 10.5f);
						t_count_label->enableOuterStroke(ccBLACK, 1, 255, true);
						t_count_label->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f) + ccp(0,-15));
						t_button_node->addChild(t_count_label);
					}
				}
			}
			else if(reward_type == "gncd")
			{
				CCPoint base_position = ccpFromSize(t_button_node->getContentSize()/2.f);
				base_position = base_position + ccpMult(ccp(-11,0), (reward_count-1)/2.f);
				
				for(int k=0;k<reward_count;k++)
				{
					CCSprite* t_star = CCSprite::create("star_on.png");
					t_star->setPosition(base_position + ccpMult(ccp(11,0), k));
					t_button_node->addChild(t_star);
				}
				
				KSLabelTTF* t_count_label = KSLabelTTF::create(ccsf(getLocal(LK::kMyLocalKey_nGradeCard), reward_count), mySGD->getFont().c_str(), 10.5f);
				t_count_label->enableOuterStroke(ccBLACK, 1, 255, true);
				t_count_label->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f) + ccp(0,-10));
				t_button_node->addChild(t_count_label);

			}
			else if(reward_type == "many")
			{
				CCSprite* t_img = CCSprite::create("icon_box.png");
				t_img->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f) + ccp(0,0));
				t_button_node->addChild(t_img);
				
//				KSLabelTTF* t_count_label = KSLabelTTF::create(ccsf("+%d", reward_count), mySGD->getFont().c_str(), 10);
//				t_count_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
//				t_count_label->setPosition(ccpFromSize(t_button->getContentSize()/2.f) + ccp(0,-10));
//				t_button->addChild(t_count_label);
			}
			else
			{
				CCSprite* t_img = CCSprite::create(ccsf("icon_%s.png", reward_type.c_str()));
				t_img->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f) + ccp(0,5));
				t_button_node->addChild(t_img);
				
				KSLabelTTF* t_count_label = KSLabelTTF::create(ccsf("+%d", reward_count), mySGD->getFont().c_str(), 10.5f);
				t_count_label->enableOuterStroke(ccBLACK, 1, 255, true);
				t_count_label->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f) + ccp(0,-15));
				t_button_node->addChild(t_count_label);
			}
			
			if(t_data.get("isTake", false).asBool())
			{
				CCSprite* t_take_back = CCSprite::create("gacha_cell_gain.png");
				t_take_back->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f));
				t_button_node->addChild(t_take_back);
			}
			else
			{
				reward_list.push_back(t_button_node);
				enable_gacha_list.push_back(j+i*4);
			}
		}
	}
	
	CCLabelTTF* t_gacha_label = CCLabelTTF::create();
	KSLabelTTF* normal_gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_normalGacha), mySGD->getFont().c_str(), 12);
	t_gacha_label->addChild(normal_gacha_label);
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("common_shadowgray2.png", CCRectMake(0, 0, 23, 23), CCRectMake(11, 11, 1, 1));
	
	if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass7) > 0)
	{
		CCSprite* price_icon = CCSprite::create("icon_p7.png");
		KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(1).c_str(), mySGD->getFont().c_str(), 12);
		price_back->setContentSize(CCSizeMake(5+price_icon->getContentSize().width+price_label->getContentSize().width+5, 23));
		price_icon->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_label->getContentSize().width/2.f-5, 0));
		price_label->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_icon->getContentSize().width/2.f-5, 0));
		price_back->addChild(price_icon);
		price_back->addChild(price_label);
		t_gacha_label->addChild(price_back);
	}
	else
	{
		CCSprite* price_icon = CCSprite::create("icon_g.png");
		KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(mySGD->getNmlGc()).c_str(), mySGD->getFont().c_str(), 12);
		price_back->setContentSize(CCSizeMake(5+price_icon->getContentSize().width+price_label->getContentSize().width+5, 23));
		price_icon->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_label->getContentSize().width/2.f-5, 0));
		price_label->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_icon->getContentSize().width/2.f-5, 0));
		price_back->addChild(price_icon);
		price_back->addChild(price_label);
		t_gacha_label->addChild(price_back);
	}
	
	normal_gacha_label->setPosition(ccp(-price_back->getContentSize().width/2.f-3, 0));
	price_back->setPosition(ccp(normal_gacha_label->getContentSize().width/2.f+3, -1));
	
	gacha_button = CCControlButton::create(t_gacha_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
	gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ManyGachaPopup::normalAction), CCControlEventTouchUpInside);
	gacha_button->setPreferredSize(CCSizeMake(92+60/*15+normal_gacha_label->getContentSize().width+10+price_back->getContentSize().width+15*/,45));
	gacha_button->setPosition(ccp(main_inner->getContentSize().width/2.f+110, 27));
	inner_node->addChild(gacha_button);
	
	gacha_button->setTouchPriority(-181);
	
	normal_ccb = KS::loadCCBI<CCSprite*>(this, "gacha_silverbox.ccbi");
	CCSprite* normal_img = normal_ccb.first;
	normal_img->setPosition(ccp(main_inner->getContentSize().width/2.f+114, 143));
	inner_node->addChild(normal_img,2);
	
	gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_normalGacha), mySGD->getFont().c_str(), 25);
	gacha_label->setGradientColor(ccc4(255, 255, 255, 255), ccc4(230, 230, 230, 255), ccp(0,-1));
	gacha_label->setPosition(ccp(main_inner->getContentSize().width/2.f+110, 64));
	inner_node->addChild(gacha_label,3);
	
	KSLabelTTF* normal_back = KSLabelTTF::create(getLocal(LK::kMyLocalKey_normalGacha), mySGD->getFont().c_str(), 25);
	normal_back->setColor(ccc3(30, 15, 0));
	normal_back->enableOuterStroke(ccc3(0, 0, 0), 2, 255, true);
	normal_back->setPosition(ccpFromSize(gacha_label->getContentSize()/2.f) + ccp(0,1));
	gacha_label->addChild(normal_back, -1);
}

void ManyGachaPopup::setPremiumGacha()
{
	unschedule(schedule_selector(ManyGachaPopup::refreshTimeChecking));
	
	is_opening = false;
	
	loading_type_is_normal = false;
	
	inner_node->removeAllChildren();
	
	CCScale9Sprite* left_back = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	left_back->setContentSize(CCSizeMake(218, 208));
	left_back->setPosition(ccp(left_back->getContentSize().width/2.f+5, left_back->getContentSize().height/2.f+5));
	inner_node->addChild(left_back);
	
	KSLabelTTF* detail_script = KSLabelTTF::create(getLocal(LK::kMyLocalKey_goodsTouchDetailScript), mySGD->getFont().c_str(), 11);
	detail_script->setPosition(ccp(left_back->getContentSize().width/2.f, left_back->getContentSize().height - detail_script->getContentSize().height/2.f - 5));
	left_back->addChild(detail_script);
	
	CCScale9Sprite* rest_time_back = CCScale9Sprite::create("option_withdraw.png", CCRectMake(0, 0, 72, 34), CCRectMake(35, 16, 2, 2));
	rest_time_back->setContentSize(CCSizeMake(210, 42));
	rest_time_back->setPosition(ccp(left_back->getContentSize().width/2.f, 20));
	left_back->addChild(rest_time_back);
	
	KSLabelTTF* rest_time_ment = KSLabelTTF::create(getLocal(LK::kMyLocalKey_listRefreshRestTime), mySGD->getFont().c_str(), 9);
	rest_time_ment->setColor(ccBLACK);
	rest_time_ment->setPosition(ccp(50,rest_time_back->getContentSize().height-13));
	rest_time_back->addChild(rest_time_ment);
	
	CCScale9Sprite* time_label_back = CCScale9Sprite::create("attendance_gold_back.png", CCRectMake(0, 0, 43, 17), CCRectMake(21, 8, 1, 1));
	time_label_back->setContentSize(CCSizeMake(80, 17));
	time_label_back->setPosition(ccp(50,15));
	rest_time_back->addChild(time_label_back);
	
	refresh_time_label = KSLabelTTF::create("00:00:00", mySGD->getFont().c_str(), 10);
	refresh_time_label->setPosition(ccpFromSize(time_label_back->getContentSize()/2.f) + ccp(0,-1));
	time_label_back->addChild(refresh_time_label);
	
	refreshTimeChecking();
	schedule(schedule_selector(ManyGachaPopup::refreshTimeChecking), 1.f);
	
	CCLabelTTF* t_refresh_label_node = CCLabelTTF::create();
	CCSprite* t_refresh_icon = CCSprite::create("gacha_refresh.png");
	KSLabelTTF* t_refresh_label_ttf = KSLabelTTF::create(getLocal(LK::kMyLocalKey_nowRefresh), mySGD->getFont().c_str(), 12);
	t_refresh_label_ttf->setPosition(ccp(t_refresh_icon->getContentSize().width/2.f-2,0));
	t_refresh_label_node->addChild(t_refresh_label_ttf);
	t_refresh_icon->setPosition(ccp(-t_refresh_label_ttf->getContentSize().width/2.f-2,0));
	t_refresh_label_node->addChild(t_refresh_icon);
	
	CCControlButton* now_refresh_button = CCControlButton::create(t_refresh_label_node, CCScale9Sprite::create("endless_bt_up.png", CCRectMake(0, 0, 75, 30), CCRectMake(37, 14, 1, 2)));
	now_refresh_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ManyGachaPopup::premiumRefresh), CCControlEventTouchUpInside);
	now_refresh_button->setPreferredSize(CCSizeMake(95, 30));
	now_refresh_button->setPosition(ccp(150, rest_time_back->getContentSize().height/2.f+1));
	now_refresh_button->setTouchPriority(-181);
	rest_time_back->addChild(now_refresh_button);
	
	CCPoint base_position = ccp(29, 20 + 48*3);
	
	string list_data = myDSH->getStringForKey(kDSH_Key_premiumGachaList);
	
	json_list.clear();
	Json::Reader t_reader;
	t_reader.parse(list_data, json_list);
	
	reward_list.clear();
	enable_gacha_list.clear();
	
	CCMenuLambda* buttons_menu = CCMenuLambda::create();
	buttons_menu->setPosition(ccp(0,0));
	left_back->addChild(buttons_menu);
	
	buttons_menu->setTouchPriority(-181);
	
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
		{
			Json::Value t_data = json_list[j + i*4];
			string back_type;
			int back_type_value = t_data["level"].asInt();
			if(back_type_value == 1)
				back_type = "gacha_cell_silver.png";
			else if(back_type_value == 2)
				back_type = "gacha_cell_purple.png";
			else if(back_type_value == 3)
				back_type = "gacha_cell_pink.png";
			else if(back_type_value == 4)
				back_type = "gacha_cell_gold.png";
			
			CCSprite* n_button = CCSprite::create(back_type.c_str());
			CCSprite* s_button = CCSprite::create(back_type.c_str());
			s_button->setColor(ccGRAY);
			
			CCNode* t_button_node = CCNode::create();
			t_button_node->setContentSize(n_button->getContentSize());
			t_button_node->setPosition(ccpFromSize(t_button_node->getContentSize()/(-2.f)) + base_position + ccp(53*j,-50*i));
			left_back->addChild(t_button_node);
			
			CCMenuItemSpriteLambda* button_item = CCMenuItemSpriteLambda::create(n_button, s_button, [=](CCObject* sender)
																				 {
																					 if(!is_menu_enable)
																						 return;
																					 
																					 is_menu_enable = false;
																					 
																					 AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
																					 
																					 CCScale9Sprite* t_cover = CCScale9Sprite::create("common_select.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
																					 t_cover->setContentSize(CCSizeMake(55, 52));
																					 t_cover->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f));
																					 t_button_node->addChild(t_cover, 9);
																					 
																					 GachaDetailPopup* t_popup = GachaDetailPopup::create(-200, t_data, [=](){
																						 t_cover->removeFromParent();
																						 is_menu_enable = true;});
																					 addChild(t_popup, 999);
																				 });
			button_item->setPosition(base_position + ccp(53*j,-50*i));
			buttons_menu->addChild(button_item);
			
			string reward_type;
			int reward_count;
			if(t_data["reward"].size() > 1)
			{
				reward_type = "many";
				reward_count = t_data["reward"].size();
			}
			else
			{
				reward_type = t_data["reward"][0]["type"].asString();
				reward_count = t_data["reward"][0]["count"].asInt();
			}
			
			if(reward_type == "cp")
			{
				//				CCSprite* t_img = CCSprite::create(ccsf("icon_%s.png", reward_type.c_str()));
				//				t_img->setPosition(ccpFromSize(t_button->getContentSize()/2.f) + ccp(0,5));
				//				t_button->addChild(t_img);
				
				bool is_found = false;
				int character_count = NSDS_GI(kSDS_GI_characterCount_i);
				for(int k=0;!is_found && k<character_count;k++)
				{
					if(reward_count == NSDS_GI(kSDS_GI_characterInfo_int1_no_i, k+1))
					{
						is_found = true;
						
						CCNode* char_node = CCNode::create();
						char_node->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f) + ccp(0,0));
						char_node->setScale(0.4f);
						auto t_pair = KS::loadCCBIForFullPath<CCSprite*>(this, StageImgLoader::sharedInstance()->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, k+1) + ".ccbi");
						CCSprite* char_img = t_pair.first;
						char_img->setPosition(ccp(0,0));
						char_node->addChild(char_img);
						t_button_node->addChild(char_node);
						
						KSLabelTTF* t_count_label = KSLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_name_s, k+1).c_str(), mySGD->getFont().c_str(), 10.5f);
						t_count_label->enableOuterStroke(ccBLACK, 1, 255, true);
						t_count_label->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f) + ccp(0,-15));
						t_button_node->addChild(t_count_label);
					}
				}
			}
			else if(reward_type == "gncd")
			{
				CCPoint base_position = ccpFromSize(t_button_node->getContentSize()/2.f);
				base_position = base_position + ccpMult(ccp(-11,0), (reward_count-1)/2.f);
				
				for(int k=0;k<reward_count;k++)
				{
					CCSprite* t_star = CCSprite::create("star_on.png");
					t_star->setPosition(base_position + ccpMult(ccp(11,0), k));
					t_button_node->addChild(t_star);
				}
				
				KSLabelTTF* t_count_label = KSLabelTTF::create(ccsf(getLocal(LK::kMyLocalKey_nGradeCard), reward_count), mySGD->getFont().c_str(), 10.5f);
				t_count_label->enableOuterStroke(ccBLACK, 1, 255, true);
				t_count_label->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f) + ccp(0,-10));
				t_button_node->addChild(t_count_label);
				
			}
			else if(reward_type == "many")
			{
				CCSprite* t_img = CCSprite::create("icon_box.png");
				t_img->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f) + ccp(0,0));
				t_button_node->addChild(t_img);
				
				//				KSLabelTTF* t_count_label = KSLabelTTF::create(ccsf("+%d", reward_count), mySGD->getFont().c_str(), 10);
				//				t_count_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
				//				t_count_label->setPosition(ccpFromSize(t_button->getContentSize()/2.f) + ccp(0,-10));
				//				t_button->addChild(t_count_label);
			}
			else
			{
				CCSprite* t_img = CCSprite::create(ccsf("icon_%s.png", reward_type.c_str()));
				t_img->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f) + ccp(0,5));
				t_button_node->addChild(t_img);
				
				KSLabelTTF* t_count_label = KSLabelTTF::create(ccsf("+%d", reward_count), mySGD->getFont().c_str(), 10.5f);
				t_count_label->enableOuterStroke(ccBLACK, 1, 255, true);
				t_count_label->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f) + ccp(0,-15));
				t_button_node->addChild(t_count_label);
			}
			
			if(t_data.get("isTake", false).asBool())
			{
				CCSprite* t_take_back = CCSprite::create("gacha_cell_gain.png");
				t_take_back->setPosition(ccpFromSize(t_button_node->getContentSize()/2.f));
				t_button_node->addChild(t_take_back);
			}
			else
			{
				reward_list.push_back(t_button_node);
				enable_gacha_list.push_back(j+i*4);
			}
		}
	}
	
	CCLabelTTF* t_gacha_label = CCLabelTTF::create();
	KSLabelTTF* premium_gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_premiumGacha), mySGD->getFont().c_str(), 12);
	t_gacha_label->addChild(premium_gacha_label);
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("common_shadowgray2.png", CCRectMake(0, 0, 23, 23), CCRectMake(11, 11, 1, 1));
	
	if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass8) > 0)
	{
		CCSprite* price_icon = CCSprite::create("icon_p8.png");
		KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(1).c_str(), mySGD->getFont().c_str(), 12);
		price_back->setContentSize(CCSizeMake(5+price_icon->getContentSize().width+price_label->getContentSize().width+5, 23));
		price_icon->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_label->getContentSize().width/2.f-5, 0));
		price_label->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_icon->getContentSize().width/2.f-5, 0));
		price_back->addChild(price_icon);
		price_back->addChild(price_label);
		t_gacha_label->addChild(price_back);
	}
	else
	{
		CCSprite* price_icon = CCSprite::create("icon_r.png");
		KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(mySGD->getPrmGc()).c_str(), mySGD->getFont().c_str(), 12);
		price_back->setContentSize(CCSizeMake(5+price_icon->getContentSize().width+price_label->getContentSize().width+5, 23));
		price_icon->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_label->getContentSize().width/2.f-5, 0));
		price_label->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_icon->getContentSize().width/2.f-5, 0));
		price_back->addChild(price_icon);
		price_back->addChild(price_label);
		t_gacha_label->addChild(price_back);
	}
	
	premium_gacha_label->setPosition(ccp(-price_back->getContentSize().width/2.f-3, 0));
	price_back->setPosition(ccp(premium_gacha_label->getContentSize().width/2.f+3, -1));
	
	gacha_button = CCControlButton::create(t_gacha_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
	gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ManyGachaPopup::premiumAction), CCControlEventTouchUpInside);
	gacha_button->setPreferredSize(CCSizeMake(92+60/*15+premium_gacha_label->getContentSize().width+10+price_back->getContentSize().width+15*/,45));
	gacha_button->setPosition(ccp(main_inner->getContentSize().width/2.f+110, 27));
	inner_node->addChild(gacha_button);
	
	gacha_button->setTouchPriority(-181);
	
	premium_ccb = KS::loadCCBI<CCSprite*>(this, "gacha_goldbox.ccbi");
	CCSprite* premium_img = premium_ccb.first;
	premium_img->setPosition(ccp(main_inner->getContentSize().width/2.f+114, 143));
	inner_node->addChild(premium_img,2);
	
	gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_premiumGacha), mySGD->getFont().c_str(), 25);
	gacha_label->setGradientColor(ccc4(255, 255, 235, 255), ccc4(255, 205, 70, 255), ccp(0,-1));
	gacha_label->setPosition(ccp(main_inner->getContentSize().width/2.f+110, 64));
	inner_node->addChild(gacha_label,3);
	
	KSLabelTTF* premium_back = KSLabelTTF::create(getLocal(LK::kMyLocalKey_premiumGacha), mySGD->getFont().c_str(), 25);
	premium_back->setColor(ccc3(30, 15, 0));
	premium_back->enableOuterStroke(ccc3(0, 0, 0), 2, 255, true);
	premium_back->setPosition(ccpFromSize(gacha_label->getContentSize()/2.f) + ccp(0,1));
	gacha_label->addChild(premium_back, -1);
}

void ManyGachaPopup::normalRefresh(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	loading_layer = LoadingLayer::create(-182);
	addChild(loading_layer, 999);
	
	Json::Value param;
	param["isPremium"] = false;
	
	myHSP->command("getgachalist", param, json_selector(this, ManyGachaPopup::resultGetGachaList));
}
void ManyGachaPopup::premiumRefresh(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	loading_layer = LoadingLayer::create(-182);
	addChild(loading_layer, 999);
	
	Json::Value param;
	param["isPremium"] = true;
	
	myHSP->command("getgachalist", param, json_selector(this, ManyGachaPopup::resultGetGachaList));
}

void ManyGachaPopup::normalAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	if(mySGD->getGoodsValue(kGoodsType_pass7) <= 0 && mySGD->getNmlGc() > mySGD->getGoodsValue(kGoodsType_gold))
	{
		addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(-999, GoodsType::kGoodsType_gold, [=]()
		{
			target_final = NULL;
			delegate_final = NULL;
			hidePopup();
			((MainFlowScene*)getParent())->showShopPopup(kSC_gold);
		}, [=]()
															{
																is_menu_enable = true;
															}), 999);
		return;
	}
	
	loading_layer = LoadingLayer::create(-182);
	addChild(loading_layer, 999);
	loading_layer->startLoading();
	
	selected_index = rand()%int(enable_gacha_list.size());
	int selected_value = enable_gacha_list[selected_index];
	
//	CCLog("gacha type : %s", json_list[selected_value]["reward"][0]["type"].asString().c_str());
	
	if(mySGD->getGoodsValue(kGoodsType_pass7) > 0)
		mySGD->addChangeGoods("nml_gc_p7");
	else
		mySGD->addChangeGoods("nml_gc");
	
	string exchange_id = json_list[selected_value]["exchangeID"].asString();
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["exchangeID"] = exchange_id;
	
	vector<CommandParam> send_commend_list;
	send_commend_list.clear();
	send_commend_list.push_back(CommandParam("exchange", param, json_selector(this, ManyGachaPopup::resultNormalExchange)));
//	myHSP->command("exchange", param, json_selector(this, ManyGachaPopup::resultNormalExchange));
	mySGD->changeGoodsTransaction(send_commend_list, json_selector(this, ManyGachaPopup::resultNormalProperties));
}

void ManyGachaPopup::resultNormalProperties(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() != GDSUCCESS)
	{
		Json::Value ex1_result = result_data["list"][1];
		if(ex1_result["result"]["code"].asInt() == GDPROPERTYISMINUS)
		{
			mySGD->clearChangeGoods();
			addChild(KSTimer::create(0.1f, [=]()
									 {
										 loading_layer->removeFromParent();
										 
										 gacha_button->removeFromParent();
										 
										 CCLabelTTF* t_gacha_label = CCLabelTTF::create();
										 KSLabelTTF* normal_gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_normalGacha), mySGD->getFont().c_str(), 12);
										 t_gacha_label->addChild(normal_gacha_label);
										 
										 CCScale9Sprite* price_back = CCScale9Sprite::create("common_shadowgray2.png", CCRectMake(0, 0, 23, 23), CCRectMake(11, 11, 1, 1));
										 
										 if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass7) > 0)
										 {
											 CCSprite* price_icon = CCSprite::create("icon_p7.png");
											 KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(1).c_str(), mySGD->getFont().c_str(), 12);
											 price_back->setContentSize(CCSizeMake(5+price_icon->getContentSize().width+price_label->getContentSize().width+5, 23));
											 price_icon->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_label->getContentSize().width/2.f-5, 0));
											 price_label->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_icon->getContentSize().width/2.f-5, 0));
											 price_back->addChild(price_icon);
											 price_back->addChild(price_label);
											 t_gacha_label->addChild(price_back);
										 }
										 else
										 {
											 CCSprite* price_icon = CCSprite::create("icon_g.png");
											 KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(mySGD->getNmlGc()).c_str(), mySGD->getFont().c_str(), 12);
											 price_back->setContentSize(CCSizeMake(5+price_icon->getContentSize().width+price_label->getContentSize().width+5, 23));
											 price_icon->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_label->getContentSize().width/2.f-5, 0));
											 price_label->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_icon->getContentSize().width/2.f-5, 0));
											 price_back->addChild(price_icon);
											 price_back->addChild(price_label);
											 t_gacha_label->addChild(price_back);
										 }
										 
										 normal_gacha_label->setPosition(ccp(-price_back->getContentSize().width/2.f-3, 0));
										 price_back->setPosition(ccp(normal_gacha_label->getContentSize().width/2.f+3, -1));
										 
										 gacha_button = CCControlButton::create(t_gacha_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
										 gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ManyGachaPopup::normalAction), CCControlEventTouchUpInside);
										 gacha_button->setPreferredSize(CCSizeMake(92+60/*15+normal_gacha_label->getContentSize().width+10+price_back->getContentSize().width+15*/,45));
										 gacha_button->setPosition(ccp(main_inner->getContentSize().width/2.f+110, 27));
										 inner_node->addChild(gacha_button);
										 
										 gacha_button->setTouchPriority(-181);
										 
										 addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(-999, GoodsType::kGoodsType_gold, [=]()
																							 {
																								 target_final = NULL;
																								 delegate_final = NULL;
																								 hidePopup();
																								 ((MainFlowScene*)getParent())->showShopPopup(kSC_gold);
																							 }, [=]()
																							 {
																								 is_menu_enable = true;
																							 }), 999);
									 }));
		}
		else
		{
			mySGD->network_check_cnt++;
			
			if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
			{
				mySGD->network_check_cnt = 0;
				
				ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
					int selected_value = enable_gacha_list[selected_index];
					
					string exchange_id = json_list[selected_value]["exchangeID"].asString();
					
					Json::Value param;
					param["memberID"] = hspConnector::get()->getSocialID();
					param["exchangeID"] = exchange_id;
					
					vector<CommandParam> send_commend_list;
					send_commend_list.clear();
					send_commend_list.push_back(CommandParam("exchange", param, json_selector(this, ManyGachaPopup::resultNormalExchange)));
					//	myHSP->command("exchange", param, json_selector(this, ManyGachaPopup::resultNormalExchange));
					mySGD->changeGoodsTransaction(send_commend_list, nullptr);
				}, 1);
				if(alert)
					((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
			}
			else
			{
				addChild(KSTimer::create(0.5f, [=]()
										 {
											 int selected_value = enable_gacha_list[selected_index];
											 
											 string exchange_id = json_list[selected_value]["exchangeID"].asString();
											 
											 Json::Value param;
											 param["memberID"] = hspConnector::get()->getSocialID();
											 param["exchangeID"] = exchange_id;
											 
											 vector<CommandParam> send_commend_list;
											 send_commend_list.clear();
											 send_commend_list.push_back(CommandParam("exchange", param, json_selector(this, ManyGachaPopup::resultNormalExchange)));
											 //	myHSP->command("exchange", param, json_selector(this, ManyGachaPopup::resultNormalExchange));
											 mySGD->changeGoodsTransaction(send_commend_list, nullptr);
										 }));
			}
		}
	}
	else
	{
		mySGD->network_check_cnt = 0;
	}
}

void ManyGachaPopup::resultNormalExchange(Json::Value result_data)
{
//	CCLog("resultNormalExchange : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		loading_layer->removeFromParent();
		
		int selected_value = enable_gacha_list[selected_index];
		string ani_name;
		int selected_level = json_list[selected_value]["level"].asInt();
		if(selected_level == 1)
			ani_name = "bar_silver";
		else if(selected_level == 2)
			ani_name = "bar_purple";
		else if(selected_level == 3)
			ani_name = "bar_pink";
		else if(selected_level == 4)
			ani_name = "bar_gold";
		
		for(int i=0;i<json_list.size();i++)
		{
			if(!json_list[i].get("isTake", false).asBool() && json_list[i]["level"].asInt() == selected_level)
			{
				for(int j=0;j<enable_gacha_list.size();j++)
				{
					if(i == enable_gacha_list[j])
					{
						CCNode* t_button = reward_list[j];
						
						CCScale9Sprite* t_cover = CCScale9Sprite::create("common_select.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
						t_cover->setContentSize(CCSizeMake(55, 52));
						t_cover->setPosition(ccpFromSize(t_button->getContentSize()/2.f));
						t_button->addChild(t_cover);
						
						t_cover->setVisible(false);
						
						t_cover->runAction(CCSequence::create(CCDelayTime::create(1.9f), CCShow::create(), CCDelayTime::create(0.15f),
															  CCRepeat::create(CCSequence::create(CCHide::create(), CCDelayTime::create(0.15f), CCShow::create(), CCDelayTime::create(0.15f), NULL), 6),
															  CCCallFunc::create(t_cover, callfunc_selector(CCScale9Sprite::removeFromParent)), NULL));
						break;
					}
				}
			}
		}
		
		json_list[selected_value]["isTake"] = true;
		keep_value = selected_value;
		
		CCNode* t_button = reward_list[selected_index];
		
		CCSprite* t_take_back = CCSprite::create("gacha_cell_gain.png");
		t_take_back->setOpacity(0);
		t_take_back->setPosition(ccpFromSize(t_button->getContentSize()/2.f));
		t_button->addChild(t_take_back);
		
		success_func = [=]()
		{
			Json::Value property_list = result_data["list"];
			for(int i=0;i<property_list.size();i++)
			{
				Json::Value t_property = property_list[i];
				if(mySGD->getGoodsKeyToType(t_property["type"].asString()) != kGoodsType_end)
				{
					mySGD->refreshGoodsData(t_property["type"].asString(), t_property["count"].asInt());
				}
				else if(t_property["type"].asString() == "cp")
				{
					mySGD->addCharacterHistoryForGacha(t_property["result"]);
				}
				else if(t_property["type"].asString() == "gncd")
				{
					mySGD->addHasGottenCardNumber(t_property["result"]["cardInfo"]["no"].asInt());
				}
			}
			
			t_take_back->addChild(KSGradualValue<int>::create(0, 255, 0.3f, [=](int t_i)
															  {
																  t_take_back->setOpacity(t_i);
															  }, [=](int t_i)
															  {
																  t_take_back->setOpacity(t_i);
																  
																  if(enable_gacha_list.empty())
																		{
																		 myDSH->setStringForKey(kDSH_Key_normalGachaList, "");
																		}
																	 else
																		{
																		 myDSH->setStringForKey(kDSH_Key_normalGachaList, GraphDogLib::JsonObjectToString(json_list));
																			gacha_button->removeFromParent();
																			CCLabelTTF* t_gacha_label = CCLabelTTF::create();
																			KSLabelTTF* normal_gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_normalGacha), mySGD->getFont().c_str(), 12);
																			t_gacha_label->addChild(normal_gacha_label);
																			
																			CCScale9Sprite* price_back = CCScale9Sprite::create("common_shadowgray2.png", CCRectMake(0, 0, 23, 23), CCRectMake(11, 11, 1, 1));
																			
																			if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass7) > 0)
																			{
																				CCSprite* price_icon = CCSprite::create("icon_p7.png");
																				KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(1).c_str(), mySGD->getFont().c_str(), 12);
																				price_back->setContentSize(CCSizeMake(5+price_icon->getContentSize().width+price_label->getContentSize().width+5, 23));
																				price_icon->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_label->getContentSize().width/2.f-5, 0));
																				price_label->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_icon->getContentSize().width/2.f-5, 0));
																				price_back->addChild(price_icon);
																				price_back->addChild(price_label);
																				t_gacha_label->addChild(price_back);
																			}
																			else
																			{
																				CCSprite* price_icon = CCSprite::create("icon_g.png");
																				KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(mySGD->getNmlGc()).c_str(), mySGD->getFont().c_str(), 12);
																				price_back->setContentSize(CCSizeMake(5+price_icon->getContentSize().width+price_label->getContentSize().width+5, 23));
																				price_icon->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_label->getContentSize().width/2.f-5, 0));
																				price_label->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_icon->getContentSize().width/2.f-5, 0));
																				price_back->addChild(price_icon);
																				price_back->addChild(price_label);
																				t_gacha_label->addChild(price_back);
																			}
																			
																			normal_gacha_label->setPosition(ccp(-price_back->getContentSize().width/2.f-3, 0));
																			price_back->setPosition(ccp(normal_gacha_label->getContentSize().width/2.f+3, -1));
																			
																			gacha_button = CCControlButton::create(t_gacha_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
																			gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ManyGachaPopup::normalAction), CCControlEventTouchUpInside);
																			gacha_button->setPreferredSize(CCSizeMake(92+60/*15+normal_gacha_label->getContentSize().width+10+price_back->getContentSize().width+15*/,45));
																			gacha_button->setPosition(ccp(main_inner->getContentSize().width/2.f+110, 27));
																			inner_node->addChild(gacha_button);
																			
																			gacha_button->setTouchPriority(-181);
																			
																			gacha_label->setVisible(true);
																		}
																	 is_menu_enable = true;
															  }));
		};
		
		if(json_list[selected_value]["reward"][0]["type"].asString() == "gncd")
		{
			Json::Value t_card;
			
			Json::Value property_list = result_data["list"];
			bool is_found = false;
			for(int i=0;!is_found && i<property_list.size();i++)
			{
				Json::Value t_property = property_list[i];
				if(t_property["type"].asString() == "gncd")
				{
					is_found = true;
					t_card = t_property["result"]["cardInfo"];
					keep_card_number = t_property["result"]["cardInfo"]["no"].asInt();
				}
			}
			
			card_download_list.clear();
			
			NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
			NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
			
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
			NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
			
			NSDS_SB(kSDS_CI_int1_haveAdult_b, t_card["no"].asInt(), t_card["haveAdult"].asBool(), false);
			
			Json::Value t_imgInfo = t_card["imgInfo"];
			
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
			if(!t_faceInfo.isNull() && t_faceInfo.asString() != "")
			{
				NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
				NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
				
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
			}
			
			mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
			
			card_downloaded_func = [=]()
			{
				enable_gacha_list.erase(enable_gacha_list.begin()+selected_index);
				reward_list.erase(reward_list.begin()+selected_index);
				
				gacha_button->setVisible(false);
				gacha_label->setVisible(false);
				CCPoint before_position = normal_ccb.first->getPosition();
				normal_ccb.first->addChild(KSGradualValue<CCPoint>::create(before_position, ccpFromSize(main_inner->getContentSize()/2.f) + ccp(0,30), 0.3f, [=](CCPoint t_p)
																		   {
																			   normal_ccb.first->setPosition(t_p);
																		   }, [=](CCPoint t_p)
																		   {
																			   normal_ccb.first->setPosition(t_p);
																			   normal_ccb.second->setDelegate(this);
																			   normal_ccb.second->runAnimationsForSequenceNamed(ani_name.c_str());
																		   }));
			};
			
			if(0 >= card_download_list.size())
			{
				card_downloaded_func();
			}
			else
			{
				ing_card_download = 1;
				startCardDownload();
			}
		}
		else
		{
			enable_gacha_list.erase(enable_gacha_list.begin()+selected_index);
			reward_list.erase(reward_list.begin()+selected_index);
			
			gacha_button->setVisible(false);
			gacha_label->setVisible(false);
			CCPoint before_position = normal_ccb.first->getPosition();
			normal_ccb.first->addChild(KSGradualValue<CCPoint>::create(before_position, ccpFromSize(main_inner->getContentSize()/2.f) + ccp(0,30), 0.3f, [=](CCPoint t_p)
																	   {
																		   normal_ccb.first->setPosition(t_p);
																	   }, [=](CCPoint t_p)
																	   {
																		   normal_ccb.first->setPosition(t_p);
																		   normal_ccb.second->setDelegate(this);
																		   normal_ccb.second->runAnimationsForSequenceNamed(ani_name.c_str());
																	   }));
		}
	}
}

void ManyGachaPopup::startCardDownload()
{
	mySGD->network_check_cnt = 0;
	if(card_download_list.size() > 0 && ing_card_download <= card_download_list.size())
	{
//		CCLOG("%d : %s", ing_card_download, card_download_list[ing_card_download-1].filename.c_str());
		mySIL->downloadImg(card_download_list[ing_card_download-1].img,
						   card_download_list[ing_card_download-1].size,
						   card_download_list[ing_card_download-1].filename,
						   this, callfunc_selector(ManyGachaPopup::successCardDownload), this, callfunc_selector(ManyGachaPopup::failCardDownload));
	}
	else
	{
		card_downloaded_func();
	}
}

void ManyGachaPopup::successCardDownload()
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
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		ing_card_download++;
		card_downloaded_func();
	}
	else
	{
		card_downloaded_func();
	}
}

void ManyGachaPopup::failCardDownload()
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

void ManyGachaPopup::completedAnimationSequenceNamed(const char *name)
{
	if(string(name) == "roll")
	{
//		CCLog("gacha type : %s", json_list[selected_value]["reward"][0]["type"].asString().c_str());
		int selected_value = keep_value;
		
		CCNode* t_node = CCNode::create();
		t_node->setPosition(ccp(0,0));
		addChild(t_node, 10);
		
		KSLabelTTF* title_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_congratulation), mySGD->getFont().c_str(), 35);
		title_label->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
		title_label->enableOuterStroke(ccc3(60, 20, 0), 2.5f, 255, true);
		title_label->setOpacity(0);
		t_node->addChild(title_label);
		
		CCScale9Sprite* detail_back = CCScale9Sprite::create("common_deepgray.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
		detail_back->setContentSize(CCSizeMake(310, 126));
		detail_back->setPosition(ccp(240,113));
		detail_back->setOpacity(0);
		t_node->addChild(detail_back);
		
		if(json_list[selected_value]["reward"].size() > 1)
		{
			// many
			title_label->setPosition(ccp(240,220));
			
			KSLabelTTF* sub_title = KSLabelTTF::create(getLocal(LK::kMyLocalKey_attendanceGoodsTypeMany), mySGD->getFont().c_str(), 18);
			sub_title->enableOuterStroke(ccBLACK, 2.f, int(255*0.7f), true);
			sub_title->setPosition(ccp(detail_back->getContentSize().width/2.f, detail_back->getContentSize().height-20));
			detail_back->addChild(sub_title);
			
			CCSprite* box_img = CCSprite::create("icon_box.png");
			box_img->setScale(2.5f);
			box_img->setPosition(ccpFromSize(detail_back->getContentSize()/2.f));
			detail_back->addChild(box_img);
			
			StyledLabelTTF* count_label = StyledLabelTTF::create(ccsf(getLocal(LK::kMyLocalKey_attendanceSpecialGoodsTypeMany), int(json_list[selected_value]["reward"].size())), mySGD->getFont().c_str(), 14, 999, StyledAlignment::kCenterAlignment);
			count_label->setAnchorPoint(ccp(0.5f,0.5f));
			count_label->setPosition(ccp(detail_back->getContentSize().width/2.f, 20));
			detail_back->addChild(count_label);
			
		}
		else if(json_list[selected_value]["reward"][0]["type"].asString() == "gncd")
		{
			title_label->setPosition(ccp(240,240));
			
			KSLabelTTF* card_take = KSLabelTTF::create(getLocal(LK::kMyLocalKey_cardTake), mySGD->getFont().c_str(), 18);
			card_take->enableOuterStroke(ccBLACK, 2.f, int(255*0.7f), true);
			card_take->setPosition(ccp(240,200));
			card_take->setOpacity(0);
			t_node->addChild(card_take);
			
			KSLabelTTF* sub_title = KSLabelTTF::create((ccsf(getLocal(LK::kMyLocalKey_nGradeCard), NSDS_GI(kSDS_CI_int1_grade_i, keep_card_number)) + string("  ") + NSDS_GS(kSDS_CI_int1_name_s, keep_card_number)).c_str(), mySGD->getFont().c_str(), 18);
			sub_title->enableOuterStroke(ccBLACK, 2.f, int(255*0.7f), true);
			sub_title->setPosition(ccp(detail_back->getContentSize().width/2.f + 50, detail_back->getContentSize().height-20));
			detail_back->addChild(sub_title);
			
			CCSprite* box_img = mySIL->getLoadedImg(ccsf("card%d_visible.png", keep_card_number));
			box_img->setScale(0.23f);
			box_img->setPosition(ccp(65, detail_back->getContentSize().height/2.f));
			detail_back->addChild(box_img);
			
			KSLabelTTF* count_label = KSLabelTTF::create(NSDS_GS(kSDS_CI_int1_profile_s, keep_card_number).c_str(), mySGD->getFont().c_str(), 13, CCSizeMake(145, 90), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
			count_label->setPosition(ccp(detail_back->getContentSize().width/2.f + 50,detail_back->getContentSize().height/2.f-10));
			detail_back->addChild(count_label);
		}
		else if(json_list[selected_value]["reward"][0]["type"].asString() == "cp")
		{
			// character
			title_label->setPosition(ccp(240,240));
			
			KSLabelTTF* character_take = KSLabelTTF::create(getLocal(LK::kMyLocalKey_takeCharacterCard), mySGD->getFont().c_str(), 18);
			character_take->enableOuterStroke(ccBLACK, 2.f, int(255*0.7f), true);
			character_take->setPosition(ccp(240,200));
			character_take->setOpacity(0);
			t_node->addChild(character_take);
			
			CCSprite* light_back = KS::loadCCBI<CCSprite*>(this, "hell_cha_back.ccbi").first;
			light_back->setPosition(ccp(75, detail_back->getContentSize().height/2.f-15));
			detail_back->addChild(light_back);
			
			int found_index = -1;
			int found_level = 1;
			int character_count = NSDS_GI(kSDS_GI_characterCount_i);
			int character_no = json_list[selected_value]["reward"][0]["count"].asInt();
			for(int i=0;found_index == -1 && i<character_count;i++)
			{
				if(NSDS_GI(kSDS_GI_characterInfo_int1_no_i, i+1) == character_no)
				{
					found_index = i+1;
					int history_size = mySGD->getCharacterHistorySize();
					for(int j=0;j<history_size;j++)
					{
						CharacterHistory t_history = mySGD->getCharacterHistory(j);
						if(t_history.characterNo.getV() == character_no)
						{
							found_level = t_history.characterLevel.getV();
							break;
						}
					}
				}
			}
			
			CCSprite* character_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, found_index) + ".ccbi").first;
			character_img->setPosition(ccp(75, detail_back->getContentSize().height/2.f-15));
			detail_back->addChild(character_img);
			
			CCSprite* character_name_back = CCSprite::create("startsetting_levelbox.png");
			character_name_back->setScale(1.3f);
			character_name_back->setPosition(ccp(220, detail_back->getContentSize().height/2.f + 40));
			detail_back->addChild(character_name_back);
			
			KSLabelTTF* char_name_title = KSLabelTTF::create(getLocal(LK::kMyLocalKey_characterName), mySGD->getFont().c_str(), 12);
			char_name_title->setPosition(character_name_back->getPosition() + ccp(-40, 0));
			detail_back->addChild(char_name_title);
			
			KSLabelTTF* char_name_label = KSLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_name_s, found_index).c_str(), mySGD->getFont().c_str(), 12);
			char_name_label->setPosition(character_name_back->getPosition() + ccp(32, 0));
			detail_back->addChild(char_name_label);
			
			StyledLabelTTF* comment_label = StyledLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_comment_int2_s, found_index, found_level).c_str(), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kLeftAlignment);
			comment_label->setAnchorPoint(ccp(0.5f,0.5f));
			comment_label->setPosition(ccp(detail_back->getContentSize().width/2.f + 50,47));
			detail_back->addChild(comment_label);
		}
		else
		{
			title_label->setPosition(ccp(240,220));
			
			string t_type = json_list[selected_value]["reward"][0]["type"].asString();
			string sub_string, count_string;
			
			if(t_type == "r")
			{
				sub_string = getLocal(LK::kMyLocalKey_gemTake);
				count_string = ccsf(getLocal(LK::kMyLocalKey_gemCount), json_list[selected_value]["reward"][0]["count"].asInt());
			}
			else if(t_type == "g")
			{
				sub_string = getLocal(LK::kMyLocalKey_goldTake);
				count_string = ccsf(getLocal(LK::kMyLocalKey_goldCount), json_list[selected_value]["reward"][0]["count"].asInt());
			}
			else if(t_type == "h")
			{
				sub_string = getLocal(LK::kMyLocalKey_heartTake);
				count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), json_list[selected_value]["reward"][0]["count"].asInt());
			}
			else if(t_type == "i6")
			{
				sub_string = getLocal(LK::kMyLocalKey_doubleItemTake);
				count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), json_list[selected_value]["reward"][0]["count"].asInt());
			}
			else if(t_type == "i9")
			{
				sub_string = getLocal(LK::kMyLocalKey_baseSpeedUpItemTake);
				count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), json_list[selected_value]["reward"][0]["count"].asInt());
			}
			else if(t_type == "i11")
			{
				sub_string = getLocal(LK::kMyLocalKey_magneticItemTake);
				count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), json_list[selected_value]["reward"][0]["count"].asInt());
			}
			else if(t_type == "p1")
			{
				sub_string = getLocal(LK::kMyLocalKey_p1Take);
				count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), json_list[selected_value]["reward"][0]["count"].asInt());
			}
			else if(t_type == "p2")
			{
				sub_string = getLocal(LK::kMyLocalKey_p2Take);
				count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), json_list[selected_value]["reward"][0]["count"].asInt());
			}
			else if(t_type == "p3")
			{
				sub_string = getLocal(LK::kMyLocalKey_p3Take);
				count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), json_list[selected_value]["reward"][0]["count"].asInt());
			}
			else if(t_type == "p4")
			{
				sub_string = getLocal(LK::kMyLocalKey_p4Take);
				count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), json_list[selected_value]["reward"][0]["count"].asInt());
			}
			else if(t_type == "p5")
			{
				sub_string = getLocal(LK::kMyLocalKey_p5Take);
				count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), json_list[selected_value]["reward"][0]["count"].asInt());
			}
			else if(t_type == "p6")
			{
				sub_string = getLocal(LK::kMyLocalKey_p6Take);
				count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), json_list[selected_value]["reward"][0]["count"].asInt());
			}
			else if(t_type == "p7")
			{
				sub_string = getLocal(LK::kMyLocalKey_p7Take);
				count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), json_list[selected_value]["reward"][0]["count"].asInt());
			}
			else if(t_type == "p8")
			{
				sub_string = getLocal(LK::kMyLocalKey_p8Take);
				count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), json_list[selected_value]["reward"][0]["count"].asInt());
			}
			
			KSLabelTTF* sub_title = KSLabelTTF::create(sub_string.c_str(), mySGD->getFont().c_str(), 18);
			sub_title->enableOuterStroke(ccBLACK, 2.f, int(255*0.7f), true);
			sub_title->setPosition(ccp(detail_back->getContentSize().width/2.f, detail_back->getContentSize().height-20));
			detail_back->addChild(sub_title);
			
			CCSprite* box_img = CCSprite::create(ccsf("icon_%s.png", t_type.c_str()));
			box_img->setScale(2.5f);
			box_img->setPosition(ccpFromSize(detail_back->getContentSize()/2.f));
			detail_back->addChild(box_img);
			
			KSLabelTTF* count_label = KSLabelTTF::create(ccsf(count_string.c_str(), int(json_list[selected_value]["reward"].size())), mySGD->getFont().c_str(), 16);
			count_label->setPosition(ccp(detail_back->getContentSize().width/2.f, 20));
			detail_back->addChild(count_label);
		}
		
		t_node->addChild(KSGradualValue<int>::create(0, 255, 0.3f, [=](int t_i)
														  {
															  KS::setOpacity(t_node, t_i);
														  }, [=](int t_i)
														  {
															  KS::setOpacity(t_node, t_i);
															  
															  TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-182);
															  t_suction->setTouchEnabled(true);
															  addChild(t_suction);
															  
															  t_suction->touch_began_func = [=]()
															  {
																  t_suction->is_on_touch_began_func = false;
																  addChild(KSGradualValue<int>::create(255, 0, 0.3f, [=](int t_i)
																											   {
																												   KS::setOpacity(t_node, t_i);
																											   }, [=](int t_i)
																											   {
																												   KS::setOpacity(t_node, t_i);
																												   t_node->removeFromParent();
																												   if(loading_type_is_normal)
																													   normal_ccb.second->runAnimationsForSequenceNamed("roll_end");
																												   else
																													   premium_ccb.second->runAnimationsForSequenceNamed("roll_end");
																												   
																												   t_suction->removeFromParent();
																											   }));
															  };
															  t_suction->is_on_touch_began_func = true;
														  }));
	}
	else if(string(name) == "roll_end")
	{
		if(loading_type_is_normal)
		{
			normal_ccb.first->setPosition(ccp(main_inner->getContentSize().width/2.f+114, 143));
			normal_ccb.second->runAnimationsForSequenceNamed("Default Timeline");
		}
		else
		{
			premium_ccb.first->setPosition(ccp(main_inner->getContentSize().width/2.f+114, 143));
			premium_ccb.second->runAnimationsForSequenceNamed("Default Timeline");
		}
		
		success_func();
	}
}

void ManyGachaPopup::premiumAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	if(mySGD->getGoodsValue(kGoodsType_pass8) <= 0 && mySGD->getPrmGc() > mySGD->getGoodsValue(kGoodsType_ruby))
	{
		addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(-999, GoodsType::kGoodsType_ruby, [=]()
															{
																target_final = NULL;
																delegate_final = NULL;
																hidePopup();
																((MainFlowScene*)getParent())->showShopPopup(kSC_ruby);
															}, [=]()
															{
																is_menu_enable = true;
															}), 999);
		return;
	}
	
	loading_layer = LoadingLayer::create(-182);
	addChild(loading_layer, 999);
	loading_layer->startLoading();
	
	selected_index = rand()%int(enable_gacha_list.size());
	int selected_value = enable_gacha_list[selected_index];
	
//	CCLog("gacha type : %s", json_list[selected_value]["reward"][0]["type"].asString().c_str());
	
	if(mySGD->getGoodsValue(kGoodsType_pass8) > 0)
		mySGD->addChangeGoods("prm_gc_p8");
	else
		mySGD->addChangeGoods("prm_gc");
	
	string exchange_id = json_list[selected_value]["exchangeID"].asString();
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["exchangeID"] = exchange_id;
	
	vector<CommandParam> send_commend_list;
	send_commend_list.clear();
	send_commend_list.push_back(CommandParam("exchange", param, json_selector(this, ManyGachaPopup::resultPremiumExchange)));
//	myHSP->command("exchange", param, json_selector(this, ManyGachaPopup::resultPremiumExchange));
	mySGD->changeGoodsTransaction(send_commend_list, json_selector(this, ManyGachaPopup::resultPremiumProperties));
}

void ManyGachaPopup::resultPremiumProperties(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() != GDSUCCESS)
	{
		Json::Value ex1_result = result_data["list"][1];
		if(ex1_result["result"]["code"].asInt() == GDPROPERTYISMINUS)
		{
			mySGD->clearChangeGoods();
			addChild(KSTimer::create(0.1f, [=]()
									 {
										 loading_layer->removeFromParent();
										 
										 gacha_button->removeFromParent();
										 
										 CCLabelTTF* t_gacha_label = CCLabelTTF::create();
										 KSLabelTTF* premium_gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_premiumGacha), mySGD->getFont().c_str(), 12);
										 t_gacha_label->addChild(premium_gacha_label);
										 
										 CCScale9Sprite* price_back = CCScale9Sprite::create("common_shadowgray2.png", CCRectMake(0, 0, 23, 23), CCRectMake(11, 11, 1, 1));
										 
										 if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass8) > 0)
										 {
											 CCSprite* price_icon = CCSprite::create("icon_p8.png");
											 KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(1).c_str(), mySGD->getFont().c_str(), 12);
											 price_back->setContentSize(CCSizeMake(5+price_icon->getContentSize().width+price_label->getContentSize().width+5, 23));
											 price_icon->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_label->getContentSize().width/2.f-5, 0));
											 price_label->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_icon->getContentSize().width/2.f-5, 0));
											 price_back->addChild(price_icon);
											 price_back->addChild(price_label);
											 t_gacha_label->addChild(price_back);
										 }
										 else
										 {
											 CCSprite* price_icon = CCSprite::create("icon_r.png");
											 KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(mySGD->getPrmGc()).c_str(), mySGD->getFont().c_str(), 12);
											 price_back->setContentSize(CCSizeMake(5+price_icon->getContentSize().width+price_label->getContentSize().width+5, 23));
											 price_icon->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_label->getContentSize().width/2.f-5, 0));
											 price_label->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_icon->getContentSize().width/2.f-5, 0));
											 price_back->addChild(price_icon);
											 price_back->addChild(price_label);
											 t_gacha_label->addChild(price_back);
										 }
										 
										 premium_gacha_label->setPosition(ccp(-price_back->getContentSize().width/2.f-3, 0));
										 price_back->setPosition(ccp(premium_gacha_label->getContentSize().width/2.f+3, -1));
										 
										 gacha_button = CCControlButton::create(t_gacha_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
										 gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ManyGachaPopup::premiumAction), CCControlEventTouchUpInside);
										 gacha_button->setPreferredSize(CCSizeMake(92+60/*15+premium_gacha_label->getContentSize().width+10+price_back->getContentSize().width+15*/,45));
										 gacha_button->setPosition(ccp(main_inner->getContentSize().width/2.f+110, 27));
										 inner_node->addChild(gacha_button);
										 
										 gacha_button->setTouchPriority(-181);
										 
										 addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(-999, GoodsType::kGoodsType_ruby, [=]()
																							 {
																								 target_final = NULL;
																								 delegate_final = NULL;
																								 hidePopup();
																								 ((MainFlowScene*)getParent())->showShopPopup(kSC_ruby);
																							 }, [=]()
																							 {
																								 is_menu_enable = true;
																							 }), 999);
									 }));
		}
		else
		{
			mySGD->network_check_cnt++;
			
			if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
			{
				mySGD->network_check_cnt = 0;
				
				ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
					int selected_value = enable_gacha_list[selected_index];
					
					string exchange_id = json_list[selected_value]["exchangeID"].asString();
					
					Json::Value param;
					param["memberID"] = hspConnector::get()->getSocialID();
					param["exchangeID"] = exchange_id;
					
					vector<CommandParam> send_commend_list;
					send_commend_list.clear();
					send_commend_list.push_back(CommandParam("exchange", param, json_selector(this, ManyGachaPopup::resultPremiumExchange)));
					//	myHSP->command("exchange", param, json_selector(this, ManyGachaPopup::resultPremiumExchange));
					mySGD->changeGoodsTransaction(send_commend_list, nullptr);
				}, 1);
				if(alert)
					((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
			}
			else
			{
				addChild(KSTimer::create(0.5f, [=]()
										 {
											 int selected_value = enable_gacha_list[selected_index];
											 
											 string exchange_id = json_list[selected_value]["exchangeID"].asString();
											 
											 Json::Value param;
											 param["memberID"] = hspConnector::get()->getSocialID();
											 param["exchangeID"] = exchange_id;
											 
											 vector<CommandParam> send_commend_list;
											 send_commend_list.clear();
											 send_commend_list.push_back(CommandParam("exchange", param, json_selector(this, ManyGachaPopup::resultPremiumExchange)));
											 //	myHSP->command("exchange", param, json_selector(this, ManyGachaPopup::resultPremiumExchange));
											 mySGD->changeGoodsTransaction(send_commend_list, nullptr);
										 }));
			}
		}
	}
	else
	{
		mySGD->network_check_cnt = 0;
	}
}

void ManyGachaPopup::resultPremiumExchange(Json::Value result_data)
{
//	CCLog("resultPremiumExchange : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		loading_layer->removeFromParent();
		
		int selected_value = enable_gacha_list[selected_index];
		string ani_name;
		int selected_level = json_list[selected_value]["level"].asInt();
		if(selected_level == 1)
			ani_name = "bar_silver";
		else if(selected_level == 2)
			ani_name = "bar_purple";
		else if(selected_level == 3)
			ani_name = "bar_pink";
		else if(selected_level == 4)
			ani_name = "bar_gold";
		
		for(int i=0;i<json_list.size();i++)
		{
			if(!json_list[i].get("isTake", false).asBool() && json_list[i]["level"].asInt() == selected_level)
			{
				for(int j=0;j<enable_gacha_list.size();j++)
				{
					if(i == enable_gacha_list[j])
					{
						CCNode* t_button = reward_list[j];
						
						CCScale9Sprite* t_cover = CCScale9Sprite::create("common_select.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
						t_cover->setContentSize(CCSizeMake(55, 52));
						t_cover->setPosition(ccpFromSize(t_button->getContentSize()/2.f));
						t_button->addChild(t_cover);
						
						t_cover->setVisible(false);
						
						t_cover->runAction(CCSequence::create(CCDelayTime::create(1.9f), CCShow::create(), CCDelayTime::create(0.15f),
															  CCRepeat::create(CCSequence::create(CCHide::create(), CCDelayTime::create(0.15f), CCShow::create(), CCDelayTime::create(0.15f), NULL), 6),
															  CCCallFunc::create(t_cover, callfunc_selector(CCScale9Sprite::removeFromParent)), NULL));
						break;
					}
				}
			}
		}
		
		json_list[selected_value]["isTake"] = true;
		keep_value = selected_value;
		
		CCNode* t_button = reward_list[selected_index];
		CCSprite* t_take_back = CCSprite::create("gacha_cell_gain.png");
		t_take_back->setOpacity(0);
		t_take_back->setPosition(ccpFromSize(t_button->getContentSize()/2.f));
		t_button->addChild(t_take_back);
		
		success_func = [=]()
		{
			Json::Value property_list = result_data["list"];
			for(int i=0;i<property_list.size();i++)
			{
				Json::Value t_property = property_list[i];
				if(mySGD->getGoodsKeyToType(t_property["type"].asString()) != kGoodsType_end)
				{
					mySGD->refreshGoodsData(t_property["type"].asString(), t_property["count"].asInt());
				}
				else if(t_property["type"].asString() == "cp")
				{
					mySGD->addCharacterHistoryForGacha(t_property["result"]);
				}
			}
			
			t_take_back->addChild(KSGradualValue<int>::create(0, 255, 0.3f, [=](int t_i)
															  {
																  t_take_back->setOpacity(t_i);
															  }, [=](int t_i)
															  {
																  t_take_back->setOpacity(t_i);
																  
																  if(enable_gacha_list.empty())
																		{
																		 myDSH->setStringForKey(kDSH_Key_premiumGachaList, "");
																			setOpening();
																		}
																	 else
																		{
																		 myDSH->setStringForKey(kDSH_Key_premiumGachaList, GraphDogLib::JsonObjectToString(json_list));
																			gacha_button->removeFromParent();
																			
																			CCLabelTTF* t_gacha_label = CCLabelTTF::create();
																			KSLabelTTF* premium_gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_premiumGacha), mySGD->getFont().c_str(), 12);
																			t_gacha_label->addChild(premium_gacha_label);
																			
																			CCScale9Sprite* price_back = CCScale9Sprite::create("common_shadowgray2.png", CCRectMake(0, 0, 23, 23), CCRectMake(11, 11, 1, 1));
																			
																			if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass8) > 0)
																			{
																				CCSprite* price_icon = CCSprite::create("icon_p8.png");
																				KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(1).c_str(), mySGD->getFont().c_str(), 12);
																				price_back->setContentSize(CCSizeMake(5+price_icon->getContentSize().width+price_label->getContentSize().width+5, 23));
																				price_icon->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_label->getContentSize().width/2.f-5, 0));
																				price_label->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_icon->getContentSize().width/2.f-5, 0));
																				price_back->addChild(price_icon);
																				price_back->addChild(price_label);
																				t_gacha_label->addChild(price_back);
																			}
																			else
																			{
																				CCSprite* price_icon = CCSprite::create("icon_r.png");
																				KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(mySGD->getPrmGc()).c_str(), mySGD->getFont().c_str(), 12);
																				price_back->setContentSize(CCSizeMake(5+price_icon->getContentSize().width+price_label->getContentSize().width+5, 23));
																				price_icon->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_label->getContentSize().width/2.f-5, 0));
																				price_label->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_icon->getContentSize().width/2.f-5, 0));
																				price_back->addChild(price_icon);
																				price_back->addChild(price_label);
																				t_gacha_label->addChild(price_back);
																			}
																			
																			premium_gacha_label->setPosition(ccp(-price_back->getContentSize().width/2.f-3, 0));
																			price_back->setPosition(ccp(premium_gacha_label->getContentSize().width/2.f+3, -1));
																			
																			gacha_button = CCControlButton::create(t_gacha_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
																			gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ManyGachaPopup::premiumAction), CCControlEventTouchUpInside);
																			gacha_button->setPreferredSize(CCSizeMake(92+60/*15+premium_gacha_label->getContentSize().width+10+price_back->getContentSize().width+15*/,45));
																			gacha_button->setPosition(ccp(main_inner->getContentSize().width/2.f+110, 27));
																			inner_node->addChild(gacha_button);
																			
																			gacha_button->setTouchPriority(-181);
																			
																			gacha_label->setVisible(true);
																		}
																	 is_menu_enable = true;
															  }));
		};
		
		enable_gacha_list.erase(enable_gacha_list.begin()+selected_index);
		reward_list.erase(reward_list.begin()+selected_index);
		
		gacha_button->setVisible(false);
		gacha_label->setVisible(false);
		CCPoint before_position = premium_ccb.first->getPosition();
		premium_ccb.first->addChild(KSGradualValue<CCPoint>::create(before_position, ccpFromSize(main_inner->getContentSize()/2.f) + ccp(0,30), 0.3f, [=](CCPoint t_p)
																   {
																	   premium_ccb.first->setPosition(t_p);
																   }, [=](CCPoint t_p)
																   {
																	   premium_ccb.first->setPosition(t_p);
																	   premium_ccb.second->setDelegate(this);
																	   premium_ccb.second->runAnimationsForSequenceNamed(ani_name.c_str());
																   }));
	}
}

void ManyGachaPopup::onEnter()
{
	CCLayer::onEnter();
	showPopup();
}

void ManyGachaPopup::showPopup()
{
	CommonAnimation::openPopup(this, main_case, gray, [=](){}, bind(&ManyGachaPopup::endShowPopup, this));
}
void ManyGachaPopup::endShowPopup()
{
	is_menu_enable = true;
}
void ManyGachaPopup::hidePopup()
{
	is_menu_enable = false;
	
	CommonAnimation::closePopup(this, main_case, gray, [=](){}, bind(&ManyGachaPopup::endHidePopup, this));
}
void ManyGachaPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void ManyGachaPopup::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	int t_tag = ((CCNode*)sender)->getTag();
	
	if(t_tag == ManyGachaPopupMenu::kClose)
	{
		if(is_opening)
			hidePopup();
		else
		{
			setOpening();
			is_menu_enable = true;
		}
	}
	else if(t_tag == ManyGachaPopupMenu::kNormal)
	{
		chrono::time_point<std::chrono::system_clock> now_time = chrono::system_clock::now();
		std::time_t now_time_t = chrono::system_clock::to_time_t(now_time);
		
		time_t sub_time = now_time_t - myDSH->getIntegerForKey(kDSH_Key_normalGachaDataTime);
		int expireSec = mySGD->getGachaRefreshTime()*60*60;
		bool is_enable_time_normal_gacha = sub_time <= expireSec;
		
		if(!is_enable_time_normal_gacha)
		{
			myDSH->setIntegerForKey(kDSH_Key_normalGachaDataTime, 0);
			myDSH->setStringForKey(kDSH_Key_normalGachaList, "");
		}
		
		if(myDSH->getStringForKey(kDSH_Key_normalGachaList) == "")
		{
			loading_type_is_normal = true;
			
			loading_layer = LoadingLayer::create(-182);
			addChild(loading_layer, 999);
			
			Json::Value param;
			param["isPremium"] = false;
			
			myHSP->command("getgachalist", param, json_selector(this, ManyGachaPopup::resultGetGachaList));
		}
		else
		{
			setNormalGacha();
			is_menu_enable = true;
		}
	}
	else if(t_tag == ManyGachaPopupMenu::kPremium)
	{
		chrono::time_point<std::chrono::system_clock> now_time = chrono::system_clock::now();
		std::time_t now_time_t = chrono::system_clock::to_time_t(now_time);
		
		time_t sub_time = now_time_t - myDSH->getIntegerForKey(kDSH_Key_premiumGachaDataTime);
		int expireSec = mySGD->getGachaRefreshTime()*60*60;
		bool is_enable_time_premium_gacha = sub_time <= expireSec;
		
		if(!is_enable_time_premium_gacha)
		{
			myDSH->setIntegerForKey(kDSH_Key_premiumGachaDataTime, 0);
			myDSH->setStringForKey(kDSH_Key_premiumGachaList, "");
		}
		
		if(myDSH->getStringForKey(kDSH_Key_premiumGachaList) == "")
		{
			loading_type_is_normal = false;
			
			loading_layer = LoadingLayer::create(-182);
			addChild(loading_layer, 999);
			
			Json::Value param;
			param["isPremium"] = true;
			
			myHSP->command("getgachalist", param, json_selector(this, ManyGachaPopup::resultGetGachaList));
		}
		else
		{
			setPremiumGacha();
			is_menu_enable = true;
		}
	}
	else if(t_tag == ManyGachaPopupMenu::kShop)
	{
		target_final = NULL;
		delegate_final = NULL;
		hidePopup();
		
		chrono::time_point<std::chrono::system_clock> now_time = chrono::system_clock::now();
		std::time_t now_time_t = chrono::system_clock::to_time_t(now_time);
		///////////////////////// 스타트팩 보여줄 수 있는지 판단 /////////////////////////////////////
		time_t sub_time = now_time_t - myDSH->getIntegerForKey(kDSH_Key_savedStartPackFirstTime);
		
		int expireSec = NSDS_GI(kSDS_GI_shopStartPack_expireSec_i);
		
		bool is_on_time_startPack = sub_time <= expireSec;
		bool is_buyed_startPack = NSDS_GI(kSDS_GI_shopStartPack_no_i) == mySGD->getUserdataOnlyOneBuyPack();
		///////////////////////// 이벤트팩 보여줄 수 있는지 판단 ////////////////////////////////////
		bool is_have_eventPack = NSDS_GB(kSDS_GI_shopEventPack_isHave_b);
		bool is_just_one = NSDS_GB(kSDS_GI_shopEventPack_isJustOne_b);
		bool is_buyed_eventPack = NSDS_GI(kSDS_GI_shopEventPack_no_i) == mySGD->getUserdataOnlyOneBuyPack();
		bool is_on_time_eventPack = false;
		
		tm* now_tm = localtime(&now_time_t);
		string startDate = NSDS_GS(kSDS_GI_shopEventPack_startDate_s);
		string endDate = NSDS_GS(kSDS_GI_shopEventPack_endDate_s);
		
		int now_time_number = atoi((string("") + ccsf("%04d", now_tm->tm_year+1900) + ccsf("%02d", now_tm->tm_mon+1) + ccsf("%02d", now_tm->tm_mday)).c_str());
		int now_time_hms = atoi((string("") + ccsf("%02d", now_tm->tm_hour) + ccsf("%02d", now_tm->tm_min) + ccsf("%02d", now_tm->tm_sec)).c_str());
		
		if(atoi(startDate.substr(0,8).c_str()) <= now_time_number &&
		   atoi(endDate.substr(0,8).c_str()) >= now_time_number &&
		   NSDS_GI(kSDS_GI_shopEventPack_startTime_i) <= now_time_hms &&
		   NSDS_GI(kSDS_GI_shopEventPack_endTime_i) >= now_time_hms)
		{
			is_on_time_eventPack = true;
		}
		
		bool is_useable_eventPack = true;
		if(is_have_eventPack && is_on_time_eventPack)
		{
			if(is_just_one)
			{
				if(!is_buyed_eventPack)
					is_useable_eventPack = true;
				else
					is_useable_eventPack = false;
			}
			else
				is_useable_eventPack = true;
		}
		else
		{
			is_useable_eventPack = false;
		}
		
		if((!is_buyed_startPack && is_on_time_startPack) || is_useable_eventPack)
		{
			((MainFlowScene*)getParent())->showShopPopup(kSC_eventPack);
		}
		else
		{
			((MainFlowScene*)getParent())->showShopPopup(kSC_gold);
		}
	}
}

void ManyGachaPopup::resultGetGachaList(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		loading_layer->removeFromParent();
		
		chrono::time_point<std::chrono::system_clock> now_time = chrono::system_clock::now();
		std::time_t now_time_t = chrono::system_clock::to_time_t(now_time);
		
		if(loading_type_is_normal)
		{
			myDSH->setIntegerForKey(kDSH_Key_normalGachaDataTime, now_time_t);
			myDSH->setStringForKey(kDSH_Key_normalGachaList, GraphDogLib::JsonObjectToString(result_data["list"]));
			setNormalGacha();
			is_menu_enable = true;
		}
		else
		{
			myDSH->setIntegerForKey(kDSH_Key_premiumGachaDataTime, now_time_t);
			myDSH->setStringForKey(kDSH_Key_premiumGachaList, GraphDogLib::JsonObjectToString(result_data["list"]));
			setPremiumGacha();
			is_menu_enable = true;
		}
	}
	else
	{
		mySGD->network_check_cnt++;
		
		if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
		{
			mySGD->network_check_cnt = 0;
			
			ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
				Json::Value param;
				param["isPremium"] = !loading_type_is_normal;
				
				myHSP->command("getgachalist", param, json_selector(this, ManyGachaPopup::resultGetGachaList));
			}, 1);
			if(alert)
				((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
									 {
										 Json::Value param;
										 param["isPremium"] = !loading_type_is_normal;
										 
										 myHSP->command("getgachalist", param, json_selector(this, ManyGachaPopup::resultGetGachaList));
									 }));
		}
	}
}