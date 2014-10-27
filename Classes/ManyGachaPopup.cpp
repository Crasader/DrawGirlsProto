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

enum ManyGachaPopupMenu
{
	kClose = 0,
	kNormal,
	kPremium
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
	tipMarquee->addText(getLocal(LK::kMyLocalKey_gachaListAt4HourTimeAutoRefresh));
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
	is_opening = true;
	
	inner_node->removeAllChildren();
	
	CCSprite* table_img = CCSprite::create("gacha_table.png");
	table_img->setScaleX(423/table_img->getContentSize().width);
	table_img->setPosition(ccp(main_inner->getContentSize().width/2.f, table_img->getContentSize().height/2.f));
	inner_node->addChild(table_img);
	
	auto normal_ccb = KS::loadCCBI<CCSprite*>(this, "gacha_silverbox.ccbi");
	CCSprite* normal_img = normal_ccb.first;
	normal_img->setPosition(ccp(main_inner->getContentSize().width/2.f-50, 100));
	inner_node->addChild(normal_img);
	
	auto premium_ccb = KS::loadCCBI<CCSprite*>(this, "gacha_goldbox.ccbi");
	CCSprite* premium_img = premium_ccb.first;
	premium_img->setPosition(ccp(main_inner->getContentSize().width/2.f+50, 100));
	inner_node->addChild(premium_img);
	
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
	normal_item->setPosition(ccp(main_inner->getContentSize().width/2.f-50, 100));
	gacha_menu->addChild(normal_item);
	
	CCSprite* n_premium = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 90, 140));
	CCSprite* s_premium = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 90, 140));
//	s_premium->setColor(ccGRAY);
	
	CCMenuItem* premium_item = CCMenuItemSprite::create(n_premium, s_premium, this, menu_selector(ManyGachaPopup::menuAction));
	premium_item->setTag(ManyGachaPopupMenu::kPremium);
	premium_item->setPosition(ccp(main_inner->getContentSize().width/2.f+50, 100));
	gacha_menu->addChild(premium_item);
}

void ManyGachaPopup::setNormalGacha()
{
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
	
	CCPoint base_position = ccp(29, 20 + 48*3);
	
	string list_data = myDSH->getStringForKey(kDSH_Key_normalGachaList);
	
	json_list.clear();
	Json::Reader t_reader;
	t_reader.parse(list_data, json_list);
	
	reward_list.clear();
	enable_gacha_list.clear();
	
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
			
			CCSprite* t_button = CCSprite::create(back_type.c_str());
			t_button->setPosition(base_position + ccp(53*j,-50*i));
			left_back->addChild(t_button);
			
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
						KSLabelTTF* t_count_label = KSLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_name_s, k+1).c_str(), mySGD->getFont().c_str(), 10);
						t_count_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
						t_count_label->setPosition(ccpFromSize(t_button->getContentSize()/2.f) + ccp(0,-15));
						t_button->addChild(t_count_label);
					}
				}
			}
			else if(reward_type == "many")
			{
				CCSprite* t_img = CCSprite::create("icon_box.png");
				t_img->setPosition(ccpFromSize(t_button->getContentSize()/2.f) + ccp(0,0));
				t_button->addChild(t_img);
				
//				KSLabelTTF* t_count_label = KSLabelTTF::create(ccsf("+%d", reward_count), mySGD->getFont().c_str(), 10);
//				t_count_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
//				t_count_label->setPosition(ccpFromSize(t_button->getContentSize()/2.f) + ccp(0,-10));
//				t_button->addChild(t_count_label);
			}
			else
			{
				CCSprite* t_img = CCSprite::create(ccsf("icon_%s.png", reward_type.c_str()));
				t_img->setPosition(ccpFromSize(t_button->getContentSize()/2.f) + ccp(0,5));
				t_button->addChild(t_img);
				
				KSLabelTTF* t_count_label = KSLabelTTF::create(ccsf("+%d", reward_count), mySGD->getFont().c_str(), 10);
				t_count_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
				t_count_label->setPosition(ccpFromSize(t_button->getContentSize()/2.f) + ccp(0,-15));
				t_button->addChild(t_count_label);
			}
			
			if(t_data.get("isTake", false).asBool())
			{
				CCSprite* t_take_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, t_button->getContentSize().width, t_button->getContentSize().height));
				t_take_back->setColor(ccBLACK);
				t_take_back->setOpacity(100);
				t_take_back->setPosition(ccpFromSize(t_button->getContentSize()/2.f));
				t_button->addChild(t_take_back);
				
				KSLabelTTF* t_take_label = KSLabelTTF::create("획득", mySGD->getFont().c_str(), 12);
				t_take_label->setColor(ccYELLOW);
				t_take_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
				t_take_label->setPosition(ccpFromSize(t_take_back->getContentSize()/2.f));
				t_take_back->addChild(t_take_label);
			}
			else
			{
				reward_list.push_back(t_button);
				enable_gacha_list.push_back(j+i*4);
			}
		}
	}
	
	CCLabelTTF* t_gacha_label = CCLabelTTF::create();
	KSLabelTTF* normal_gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_normalGacha), mySGD->getFont().c_str(), 12);
	t_gacha_label->addChild(normal_gacha_label);
	
	gacha_button = CCControlButton::create(t_gacha_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
	gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ManyGachaPopup::normalAction), CCControlEventTouchUpInside);
	gacha_button->setPreferredSize(CCSizeMake(92,45));
	gacha_button->setPosition(ccp(main_inner->getContentSize().width/2.f+110, 27));
	inner_node->addChild(gacha_button);
	
	gacha_button->setTouchPriority(-181);
	
	normal_ccb = KS::loadCCBI<CCSprite*>(this, "gacha_silverbox.ccbi");
	CCSprite* normal_img = normal_ccb.first;
	normal_img->setPosition(ccp(main_inner->getContentSize().width/2.f+110, 135));
	inner_node->addChild(normal_img);
}

void ManyGachaPopup::setPremiumGacha()
{
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
	
	CCPoint base_position = ccp(29, 20 + 48*3);
	
	string list_data = myDSH->getStringForKey(kDSH_Key_premiumGachaList);
	
	json_list.clear();
	Json::Reader t_reader;
	t_reader.parse(list_data, json_list);
	
	reward_list.clear();
	enable_gacha_list.clear();
	
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
			
			CCSprite* t_button = CCSprite::create(back_type.c_str());
			t_button->setPosition(base_position + ccp(53*j,-50*i));
			left_back->addChild(t_button);
			
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
						KSLabelTTF* t_count_label = KSLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_name_s, k+1).c_str(), mySGD->getFont().c_str(), 10);
						t_count_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
						t_count_label->setPosition(ccpFromSize(t_button->getContentSize()/2.f) + ccp(0,-15));
						t_button->addChild(t_count_label);
					}
				}
			}
			else if(reward_type == "many")
			{
				CCSprite* t_img = CCSprite::create("icon_box.png");
				t_img->setPosition(ccpFromSize(t_button->getContentSize()/2.f) + ccp(0,0));
				t_button->addChild(t_img);
				
				//				KSLabelTTF* t_count_label = KSLabelTTF::create(ccsf("+%d", reward_count), mySGD->getFont().c_str(), 10);
				//				t_count_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
				//				t_count_label->setPosition(ccpFromSize(t_button->getContentSize()/2.f) + ccp(0,-10));
				//				t_button->addChild(t_count_label);
			}
			else
			{
				CCSprite* t_img = CCSprite::create(ccsf("icon_%s.png", reward_type.c_str()));
				t_img->setPosition(ccpFromSize(t_button->getContentSize()/2.f) + ccp(0,5));
				t_button->addChild(t_img);
				
				KSLabelTTF* t_count_label = KSLabelTTF::create(ccsf("+%d", reward_count), mySGD->getFont().c_str(), 10);
				t_count_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
				t_count_label->setPosition(ccpFromSize(t_button->getContentSize()/2.f) + ccp(0,-15));
				t_button->addChild(t_count_label);
			}
			
			if(t_data.get("isTake", false).asBool())
			{
				CCSprite* t_take_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, t_button->getContentSize().width, t_button->getContentSize().height));
				t_take_back->setColor(ccBLACK);
				t_take_back->setOpacity(100);
				t_take_back->setPosition(ccpFromSize(t_button->getContentSize()/2.f));
				t_button->addChild(t_take_back);
				
				KSLabelTTF* t_take_label = KSLabelTTF::create("획득", mySGD->getFont().c_str(), 12);
				t_take_label->setColor(ccYELLOW);
				t_take_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
				t_take_label->setPosition(ccpFromSize(t_take_back->getContentSize()/2.f));
				t_take_back->addChild(t_take_label);
			}
			else
			{
				reward_list.push_back(t_button);
				enable_gacha_list.push_back(j+i*4);
			}
		}
	}
	
	CCLabelTTF* t_gacha_label = CCLabelTTF::create();
	KSLabelTTF* premium_gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_premiumGacha), mySGD->getFont().c_str(), 12);
	t_gacha_label->addChild(premium_gacha_label);
	
	gacha_button = CCControlButton::create(t_gacha_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
	gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ManyGachaPopup::premiumAction), CCControlEventTouchUpInside);
	gacha_button->setPreferredSize(CCSizeMake(92,45));
	gacha_button->setPosition(ccp(main_inner->getContentSize().width/2.f+110, 27));
	inner_node->addChild(gacha_button);
	
	gacha_button->setTouchPriority(-181);
	
	premium_ccb = KS::loadCCBI<CCSprite*>(this, "gacha_goldbox.ccbi");
	CCSprite* premium_img = premium_ccb.first;
	premium_img->setPosition(ccp(main_inner->getContentSize().width/2.f+110, 135));
	inner_node->addChild(premium_img);
}

void ManyGachaPopup::normalAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	selected_index = rand()%int(enable_gacha_list.size());
	int selected_value = enable_gacha_list[selected_index];
	
	CCLog("gacha type : %s", json_list[selected_value]["reward"][0]["type"].asString().c_str());
	
	string exchange_id = json_list[selected_value]["exchangeID"].asString();
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["exchangeID"] = exchange_id;
	
	myHSP->command("exchange", param, json_selector(this, ManyGachaPopup::resultNormalExchange));
}

void ManyGachaPopup::resultNormalExchange(Json::Value result_data)
{
	CCLog("resultNormalExchange : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		
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
		
		json_list[selected_value]["isTake"] = true;
		
		CCSprite* t_button = reward_list[selected_index];
		CCSprite* t_take_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, t_button->getContentSize().width, t_button->getContentSize().height));
		t_take_back->setColor(ccBLACK);
		t_take_back->setOpacity(0);
		t_take_back->setPosition(ccpFromSize(t_button->getContentSize()/2.f));
		t_button->addChild(t_take_back);
		
		KSLabelTTF* t_take_label = KSLabelTTF::create("획득", mySGD->getFont().c_str(), 12);
		t_take_label->setColor(ccYELLOW);
		t_take_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
		t_take_label->setPosition(ccpFromSize(t_take_back->getContentSize()/2.f));
		t_take_label->setScale(0.f);
		t_take_back->addChild(t_take_label);
		
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
			
			t_take_back->addChild(KSGradualValue<int>::create(0, 100, 0.3f, [=](int t_i)
															  {
																  t_take_back->setOpacity(t_i);
															  }, [=](int t_i)
															  {
																  t_take_back->setOpacity(t_i);
															  }));
			
			t_take_label->addChild(KSGradualValue<float>::create(3.f, 1.f, 0.3f, [=](float t_f)
																 {
																	 t_take_label->setScale(t_f);
																 }, [=](float t_f)
																 {
																	 t_take_label->setScale(t_f);
																	 
																	 if(enable_gacha_list.empty())
																		{
																		 myDSH->setStringForKey(kDSH_Key_normalGachaList, "");
																		}
																	 else
																		{
																		 myDSH->setStringForKey(kDSH_Key_normalGachaList, GraphDogLib::JsonObjectToString(json_list));
																			gacha_button->setVisible(true);
																		}
																	 is_menu_enable = true;
																 }));
		};
		
		enable_gacha_list.erase(enable_gacha_list.begin()+selected_index);
		reward_list.erase(reward_list.begin()+selected_index);
		
		gacha_button->setVisible(false);
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
				
				myHSP->command("exchange", param, json_selector(this, ManyGachaPopup::resultNormalExchange));
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
										 
										 myHSP->command("exchange", param, json_selector(this, ManyGachaPopup::resultNormalExchange));
									 }));
		}
	}
}

void ManyGachaPopup::completedAnimationSequenceNamed(const char *name)
{
	if(string(name) == "roll")
	{
		TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-182);
		t_suction->setTouchEnabled(true);
		addChild(t_suction);
		
		t_suction->touch_began_func = [=]()
		{
			t_suction->is_on_touch_began_func = false;
			if(loading_type_is_normal)
				normal_ccb.second->runAnimationsForSequenceNamed("roll_end");
			else
				premium_ccb.second->runAnimationsForSequenceNamed("roll_end");
			
			t_suction->removeFromParent();
		};
		t_suction->is_on_touch_began_func = true;
	}
	else if(string(name) == "roll_end")
	{
		if(loading_type_is_normal)
		{
			normal_ccb.first->setPosition(ccp(main_inner->getContentSize().width/2.f+110, 135));
			normal_ccb.second->runAnimationsForSequenceNamed("Default Timeline");
		}
		else
		{
			premium_ccb.first->setPosition(ccp(main_inner->getContentSize().width/2.f+110, 135));
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
	
	selected_index = rand()%int(enable_gacha_list.size());
	int selected_value = enable_gacha_list[selected_index];
	
	CCLog("gacha type : %s", json_list[selected_value]["reward"][0]["type"].asString().c_str());
	
	string exchange_id = json_list[selected_value]["exchangeID"].asString();
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["exchangeID"] = exchange_id;
	
	myHSP->command("exchange", param, json_selector(this, ManyGachaPopup::resultPremiumExchange));
}

void ManyGachaPopup::resultPremiumExchange(Json::Value result_data)
{
	CCLog("resultPremiumExchange : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		
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
		
		json_list[selected_value]["isTake"] = true;
		
		CCSprite* t_button = reward_list[selected_index];
		CCSprite* t_take_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, t_button->getContentSize().width, t_button->getContentSize().height));
		t_take_back->setColor(ccBLACK);
		t_take_back->setOpacity(0);
		t_take_back->setPosition(ccpFromSize(t_button->getContentSize()/2.f));
		t_button->addChild(t_take_back);
		
		KSLabelTTF* t_take_label = KSLabelTTF::create("획득", mySGD->getFont().c_str(), 12);
		t_take_label->setColor(ccYELLOW);
		t_take_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
		t_take_label->setPosition(ccpFromSize(t_take_back->getContentSize()/2.f));
		t_take_label->setScale(0.f);
		t_take_back->addChild(t_take_label);
		
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
			
			t_take_back->addChild(KSGradualValue<int>::create(0, 100, 0.3f, [=](int t_i)
															  {
																  t_take_back->setOpacity(t_i);
															  }, [=](int t_i)
															  {
																  t_take_back->setOpacity(t_i);
															  }));
			
			t_take_label->addChild(KSGradualValue<float>::create(3.f, 1.f, 0.3f, [=](float t_f)
																 {
																	 t_take_label->setScale(t_f);
																 }, [=](float t_f)
																 {
																	 t_take_label->setScale(t_f);
																	 
																	 if(enable_gacha_list.empty())
																		{
																		 myDSH->setStringForKey(kDSH_Key_premiumGachaList, "");
																			setOpening();
																		}
																	 else
																		{
																		 myDSH->setStringForKey(kDSH_Key_premiumGachaList, GraphDogLib::JsonObjectToString(json_list));
																			gacha_button->setVisible(true);
																		}
																	 is_menu_enable = true;
																 }));
		};
		
		enable_gacha_list.erase(enable_gacha_list.begin()+selected_index);
		reward_list.erase(reward_list.begin()+selected_index);
		
		gacha_button->setVisible(false);
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
				
				myHSP->command("exchange", param, json_selector(this, ManyGachaPopup::resultPremiumExchange));
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
										 
										 myHSP->command("exchange", param, json_selector(this, ManyGachaPopup::resultPremiumExchange));
									 }));
		}
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
}

void ManyGachaPopup::resultGetGachaList(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		loading_layer->removeFromParent();
		if(loading_type_is_normal)
		{
			myDSH->setStringForKey(kDSH_Key_normalGachaList, GraphDogLib::JsonObjectToString(result_data["list"]));
			setNormalGacha();
			is_menu_enable = true;
		}
		else
		{
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