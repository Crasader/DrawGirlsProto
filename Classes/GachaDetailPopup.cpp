//
//  GachaDetailPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 10. 28..
//
//

#include "GachaDetailPopup.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "MyLocalization.h"
#include "AudioEngine.h"
#include "StyledLabelTTF.h"
#include "CommonButton.h"
#include "CommonAnimation.h"
#include "StageImgLoader.h"
#include "GachaData.h"

GachaDetailPopup* GachaDetailPopup::create(int t_touch_priority, GachaData* t_goods_info, function<void()> t_end_func)
{
	GachaDetailPopup* t_mup = new GachaDetailPopup();
	t_mup->myInit(t_touch_priority, t_goods_info, t_end_func);
	t_mup->autorelease();
	return t_mup;
}

void GachaDetailPopup::myInit(int t_touch_priority, GachaData* t_goods_info, function<void()> t_end_func)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	goods_info = t_goods_info;
	
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
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	back_case = CCSprite::create("popup_small_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(251, 106));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-12));
	back_case->addChild(back_in);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_goodsScript), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f, 0.5f));
	title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
	m_container->addChild(title_label);
	
	
	if(goods_info->reward_list.size() > 1)
	{
		// many
		KSLabelTTF* sub_title = KSLabelTTF::create(getLocal(LK::kMyLocalKey_attendanceGoodsTypeMany), mySGD->getFont().c_str(), 18);
		sub_title->enableOuterStroke(ccBLACK, 2.f, int(255*0.7f), true);
		sub_title->setPosition(ccp(back_in->getContentSize().width/2.f, back_in->getContentSize().height-20));
		back_in->addChild(sub_title);
		
		CCSprite* box_img = CCSprite::create("icon_box.png");
		box_img->setScale(2.5f);
		box_img->setPosition(ccpFromSize(back_in->getContentSize()/2.f));
		back_in->addChild(box_img);
		
		StyledLabelTTF* count_label = StyledLabelTTF::create(ccsf(getLocal(LK::kMyLocalKey_attendanceSpecialGoodsTypeMany), int(goods_info->reward_list.size())), mySGD->getFont().c_str(), 14, 999, StyledAlignment::kCenterAlignment);
		count_label->setAnchorPoint(ccp(0.5f,0.5f));
		count_label->setPosition(ccp(back_in->getContentSize().width/2.f, 20));
		back_in->addChild(count_label);
		
	}
	else if(goods_info->reward_list[0].type.getV() == "gncd")
	{
		KSLabelTTF* sub_title = KSLabelTTF::create(ccsf(getLocal(LK::kMyLocalKey_cardTake), goods_info->reward_list[0].count.getV()), mySGD->getFont().c_str(), 18);
		sub_title->enableOuterStroke(ccBLACK, 1.f, int(255*0.7f), true);
		sub_title->setPosition(ccp(back_in->getContentSize().width/2.f, back_in->getContentSize().height-20));
		back_in->addChild(sub_title);
		
//		CCSprite* box_img = mySIL->getLoadedImg(ccsf("card%d_visible.png", keep_card_number));
//		box_img->setScale(0.23f);
//		box_img->setPosition(ccp(65, detail_back->getContentSize().height/2.f));
//		detail_back->addChild(box_img);
//		
//		KSLabelTTF* count_label = KSLabelTTF::create(NSDS_GS(kSDS_CI_int1_profile_s, keep_card_number).c_str(), mySGD->getFont().c_str(), 13, CCSizeMake(145, 90), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//		count_label->setPosition(ccp(detail_back->getContentSize().width/2.f + 50,detail_back->getContentSize().height/2.f-10));
//		detail_back->addChild(count_label);
	}
	else if(goods_info->reward_list[0].type.getV() == "dhcd")
	{
		KSLabelTTF* sub_title = KSLabelTTF::create((getLocal(LK::kMyLocalKey_dontHave) + string(" ") + ccsf(getLocal(LK::kMyLocalKey_cardTake), goods_info->reward_list[0].count.getV())).c_str(), mySGD->getFont().c_str(), 18);
		sub_title->enableOuterStroke(ccBLACK, 1.f, int(255*0.7f), true);
		sub_title->setPosition(ccp(back_in->getContentSize().width/2.f, back_in->getContentSize().height-20));
		back_in->addChild(sub_title);
	}
	else if(goods_info->reward_list[0].type.getV() == "cp")
	{
		CCSprite* light_back = KS::loadCCBI<CCSprite*>(this, "hell_cha_back.ccbi").first;
		light_back->setPosition(ccp(65, back_in->getContentSize().height/2.f-15));
		back_in->addChild(light_back);
		
		int found_index = -1;
		int found_level = 1;
		int character_count = NSDS_GI(kSDS_GI_characterCount_i);
		int character_no = goods_info->reward_list[0].count.getV();
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
		character_img->setPosition(ccp(65, back_in->getContentSize().height/2.f-15));
		back_in->addChild(character_img);
		
		CCSprite* character_name_back = CCSprite::create("startsetting_levelbox.png");
		character_name_back->setScale(1.f);
		character_name_back->setPosition(ccp(185, back_in->getContentSize().height/2.f + 30));
		back_in->addChild(character_name_back);
		
		KSLabelTTF* char_name_title = KSLabelTTF::create(getLocal(LK::kMyLocalKey_characterName), mySGD->getFont().c_str(), 11);
		char_name_title->setPosition(character_name_back->getPosition() + ccp(-31, 0));
		back_in->addChild(char_name_title);
		
		KSLabelTTF* char_name_label = KSLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_name_s, found_index).c_str(), mySGD->getFont().c_str(), 11);
		char_name_label->setPosition(character_name_back->getPosition() + ccp(26, 0));
		back_in->addChild(char_name_label);
		
		StyledLabelTTF* comment_label = StyledLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_comment_int2_s, found_index, found_level).c_str(), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kLeftAlignment);
		comment_label->setAnchorPoint(ccp(0.5f,0.5f));
		comment_label->setPosition(ccp(back_in->getContentSize().width/2.f + 50,42));
		back_in->addChild(comment_label);
	}
	else
	{
		string t_type = goods_info->reward_list[0].type.getV();
		string sub_string, count_string;
		
		if(t_type == "r")
		{
			sub_string = getLocal(LK::kMyLocalKey_gem);
			count_string = ccsf(getLocal(LK::kMyLocalKey_gemCount), goods_info->reward_list[0].count.getV());
		}
		else if(t_type == "g")
		{
			sub_string = getLocal(LK::kMyLocalKey_gold);
			count_string = ccsf(getLocal(LK::kMyLocalKey_goldCount), goods_info->reward_list[0].count.getV());
		}
		else if(t_type == "h")
		{
			sub_string = getLocal(LK::kMyLocalKey_heart);
			count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), goods_info->reward_list[0].count.getV());
		}
		else if(t_type == "i6")
		{
			sub_string = getLocal(LK::kMyLocalKey_doubleItem);
			count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), goods_info->reward_list[0].count.getV());
		}
		else if(t_type == "i9")
		{
			sub_string = getLocal(LK::kMyLocalKey_baseSpeedUpItem);
			count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), goods_info->reward_list[0].count.getV());
		}
		else if(t_type == "i11")
		{
			sub_string = getLocal(LK::kMyLocalKey_magneticItem);
			count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), goods_info->reward_list[0].count.getV());
		}
		else if(t_type == "p1")
		{
			sub_string = getLocal(LK::kMyLocalKey_p1);
			count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), goods_info->reward_list[0].count.getV());
		}
		else if(t_type == "p2")
		{
			sub_string = getLocal(LK::kMyLocalKey_p2);
			count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), goods_info->reward_list[0].count.getV());
		}
		else if(t_type == "p3")
		{
			sub_string = getLocal(LK::kMyLocalKey_p3);
			count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), goods_info->reward_list[0].count.getV());
		}
		else if(t_type == "p4")
		{
			sub_string = getLocal(LK::kMyLocalKey_p4);
			count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), goods_info->reward_list[0].count.getV());
		}
		else if(t_type == "p5")
		{
			sub_string = getLocal(LK::kMyLocalKey_p5);
			count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), goods_info->reward_list[0].count.getV());
		}
		else if(t_type == "p6")
		{
			sub_string = getLocal(LK::kMyLocalKey_p6);
			count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), goods_info->reward_list[0].count.getV());
		}
		else if(t_type == "p7")
		{
			sub_string = getLocal(LK::kMyLocalKey_p7);
			count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), goods_info->reward_list[0].count.getV());
		}
		else if(t_type == "p8")
		{
			sub_string = getLocal(LK::kMyLocalKey_p8);
			count_string = ccsf(getLocal(LK::kMyLocalKey_propertyCount), goods_info->reward_list[0].count.getV());
		}
		
		KSLabelTTF* sub_title = KSLabelTTF::create(sub_string.c_str(), mySGD->getFont().c_str(), 18);
		sub_title->enableOuterStroke(ccBLACK, 1.f, int(255*0.7f), true);
		sub_title->setPosition(ccp(back_in->getContentSize().width/2.f, back_in->getContentSize().height-18));
		back_in->addChild(sub_title);
		
		CCSprite* box_img = CCSprite::create(ccsf("icon_%s.png", t_type.c_str()));
		box_img->setScale(2.5f);
		box_img->setPosition(ccpFromSize(back_in->getContentSize()/2.f));
		back_in->addChild(box_img);
		
		KSLabelTTF* count_label = KSLabelTTF::create(ccsf(count_string.c_str(), int(goods_info->reward_list.size())), mySGD->getFont().c_str(), 16);
		count_label->setPosition(ccp(back_in->getContentSize().width/2.f, 17));
		back_in->addChild(count_label);
	}
	
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width/2.f-25,back_case->getContentSize().height/2.f-22));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
								  
								  CommonAnimation::closePopup(this, m_container, gray, [=](){
									  
								  }, [=](){
									  end_func(); removeFromParent();
								  });
							  });
	m_container->addChild(close_button);
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
}
