//
//  CharacterDetailPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 10. 31..
//
//

#include "CharacterDetailPopup.h"
#include "TouchSuctionLayer.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "CommonButton.h"
#include "CommonAnimation.h"
#include "KSLabelTTF.h"
#include "KsLocal.h"
#include "AudioEngine.h"
#include "StageImgLoader.h"
#include "ConvexGraph.h"
#include "KSUtil.h"
#include "StyledLabelTTF.h"

CharacterDetailPopup* CharacterDetailPopup::create(int t_touch_priority, int t_cha_idx, function<void()> t_end_func, function<void()> t_select_func)
{
	CharacterDetailPopup* t_mup = new CharacterDetailPopup();
	t_mup->myInit(t_touch_priority, t_cha_idx, t_end_func, t_select_func);
	t_mup->autorelease();
	return t_mup;
}

void CharacterDetailPopup::myInit(int t_touch_priority, int t_cha_idx, function<void()> t_end_func, function<void()> t_select_func)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	select_func = t_select_func;
	cha_idx = t_cha_idx;
	
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
	
	back_case = CCSprite::create("popup_large_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(251,137));
	back_in->setPosition(ccpFromSize(back_case->getContentSize()/2.f) + ccp(0,6));
	back_case->addChild(back_in);
	
	KSLabelTTF* title_label = KSLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_name_s, cha_idx).c_str(), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccpFromSize(back_case->getContentSize()/2.f) + ccp(-85, back_case->getContentSize().height/2.f-35));
	back_case->addChild(title_label);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width-25,back_case->getContentSize().height-22));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  
								  CommonAnimation::closePopup(this, m_container, gray, [=](){
									  
								  }, [=](){
									  end_func();
									  removeFromParent();
								  });
							  });
	back_case->addChild(close_button);
	
	CCNode* character_node = CCNode::create();
	character_node->setScale(0.8f);
	character_node->setPosition(ccp(70, 83));
	back_in->addChild(character_node);
	
	CCSprite* light_back = KS::loadCCBI<CCSprite*>(this, "hell_cha_back.ccbi").first;
	light_back->setPosition(ccp(0,0));
	character_node->addChild(light_back);
	
	CCSprite* character_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, cha_idx) + ".ccbi").first;
	character_img->setPosition(ccp(0,0));
	character_node->addChild(character_img);
	
	int character_no = NSDS_GI(kSDS_GI_characterInfo_int1_no_i, cha_idx);
	int history_size = mySGD->getCharacterHistorySize();
	bool is_found = false;
	CharacterHistory cha_history;
	for(int i=0;!is_found && i<history_size;i++)
	{
		CharacterHistory t_history = mySGD->getCharacterHistory(i);
		if(t_history.characterNo.getV() == character_no)
		{
			is_found = true;
			cha_history = t_history;
		}
	}
	
	int cha_level = cha_history.characterLevel.getV();
	
	KSLabelTTF* level_label = KSLabelTTF::create(ccsf(getLocal(LK::kMyLocalKey_levelValue), cha_level), mySGD->getFont().c_str(), 11);
	level_label->setAnchorPoint(ccp(1,0.5f));
	level_label->setPosition(ccp(128,68));
	back_in->addChild(level_label);
	
	ConvexGraph* exp_graph = ConvexGraph::create("cha_graph_center.png", CCRectMake(0, 0, 15, 15), CCRectMake(7, 7, 1, 1), CCSizeMake(120, 15), ConvexGraphType::width);
	exp_graph->setPosition(ccp(70,53));
	back_in->addChild(exp_graph);
	
	exp_graph->setCover("cha_graph_front.png", "cha_graph_center.png");
	exp_graph->setBack("cha_graph_back.png");
	
	int sub_base_value = cha_history.characterNextLevelExp.getV() - cha_history.characterCurrentLevelExp.getV();
	int sub_value = cha_history.characterExp.getV() - cha_history.characterCurrentLevelExp.getV();
	float t_percent = 100.f*sub_value/sub_base_value;
	if(cha_history.characterLevel.getV() == NSDS_GI(kSDS_GI_characterInfo_int1_maxLevel_i, cha_idx))
	{
		t_percent = 100.f;
	}
	
	exp_graph->setPercentage(t_percent);
	
	KSLabelTTF* exp_label = KSLabelTTF::create(ccsf(getLocal(LK::kMyLocalKey_expN_M), t_percent/*KS::insert_separator(cha_history.characterExp.getV()).c_str(), KS::insert_separator(cha_history.characterNextLevelExp.getV()).c_str()*/), mySGD->getFont().c_str(), 11);
	exp_label->enableOuterStroke(ccBLACK, 0.5f, 255, true);
	exp_label->setPosition(ccp(70,52));
	back_in->addChild(exp_label);
	
	
	CCSprite* base_back = CCSprite::create("cha_box.png");
	base_back->setPosition(ccp(190, 90));
	back_in->addChild(base_back);
	
//	CCSprite* base_white = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 99, 80));
//	base_white->setOpacity(100);
//	base_white->setPosition(ccpFromSize(base_back->getContentSize()/2.f) + ccp(3, 0));
//	base_back->addChild(base_white);
	
	string base_str = "";
	
	double power_value = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_power_d, cha_idx, cha_level);
	double speed_value = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_speed_d, cha_idx, cha_level);
	double magnet_value = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_magnetic_d, cha_idx, cha_level);
	double missile_value = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_percent_d, cha_idx, cha_level);
	double rewind_value = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_rewindSpd_d, cha_idx, cha_level);
	double gold_value = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_gold_d, cha_idx, cha_level);
	double score_value = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_score_d, cha_idx, cha_level);
	double wave_value = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_monsterWave_d, cha_idx, cha_level);
	double ms_wave_value = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_missileWave_d, cha_idx, cha_level);
	
	int power_int = round((power_value - 1.0)*100.0);
//	if(power_int > 0)
	{
		base_str += ccsf("<font size=8.5 color=#550A1E strokesize=0.5 strokecolor=#FFFFFF strokeopacity=102>%s </font>", getLocal(LK::kMyLocalKey_power));
		base_str += ccsf("<font newline=10 size=9.5 color=#FFFFFF strokesize=1 strokecolor=#000000 strokeopacity=153>+%d%%</font>", power_int);
	}
	
	int speed_int = round((speed_value - 1.1)*100.0);
//	if(speed_int > 0)
	{
		base_str += ccsf("<font size=8.5 color=#550A1E strokesize=0.5 strokecolor=#FFFFFF strokeopacity=102>%s </font>", getLocal(LK::kMyLocalKey_speed));
		base_str += ccsf("<font newline=10 size=9.5 color=#FFFFFF strokesize=1 strokecolor=#000000 strokeopacity=153>+%d%%</font>", speed_int);
	}
	
	int magnet_int = round(magnet_value);
//	if(magnet_int > 0)
	{
		base_str += ccsf("<font size=8.5 color=#550A1E strokesize=0.5 strokecolor=#FFFFFF strokeopacity=102>%s </font>", getLocal(LK::kMyLocalKey_magnetEffect));
		base_str += ccsf("<font newline=10 size=9.5 color=#FFFFFF strokesize=1 strokecolor=#000000 strokeopacity=153>+%d</font>", magnet_int);
	}
	
	int missile_int = round((2.0 - missile_value)/2.0*100.0);
//	if(missile_int > 0)
	{
		base_str += ccsf("<font size=8.5 color=#550A1E strokesize=0.5 strokecolor=#FFFFFF strokeopacity=102>%s </font>", getLocal(LK::kMyLocalKey_missileCreateValue));
		base_str += ccsf("<font newline=10 size=9.5 color=#FFFFFF strokesize=1 strokecolor=#000000 strokeopacity=153>+%d%%</font>", missile_int);
	}
	
	int rewind_int = round((rewind_value - 2.0)/2.0*100.0);
//	if(rewind_int > 0)
	{
		base_str += ccsf("<font size=8.5 color=#550A1E strokesize=0.5 strokecolor=#FFFFFF strokeopacity=102>%s </font>", getLocal(LK::kMyLocalKey_cancelSpeed));
		base_str += ccsf("<font newline=10 size=9.5 color=#FFFFFF strokesize=1 strokecolor=#000000 strokeopacity=153>+%d%%</font>", rewind_int);
	}
	
	int gold_int = round((gold_value - 1.0)*100.0);
//	if(gold_int > 0)
	{
		base_str += ccsf("<font size=8.5 color=#550A1E strokesize=0.5 strokecolor=#FFFFFF strokeopacity=102>%s </font>", getLocal(LK::kMyLocalKey_goldBonus));
		base_str += ccsf("<font newline=10 size=9.5 color=#FFFFFF strokesize=1 strokecolor=#000000 strokeopacity=153>+%d%%</font>", gold_int);
	}
	
	int score_int = round((score_value - 1.0)*100.0);
//	if(score_int > 0)
	{
		base_str += ccsf("<font size=8.5 color=#550A1E strokesize=0.5 strokecolor=#FFFFFF strokeopacity=102>%s </font>", getLocal(LK::kMyLocalKey_scoreBonus));
		base_str += ccsf("<font newline=10 size=9.5 color=#FFFFFF strokesize=1 strokecolor=#000000 strokeopacity=153>+%d%%</font>", score_int);
	}
	
	int wave_int = round(wave_value);
//	if(wave_int > 0)
	{
		base_str += ccsf("<font size=8.5 color=#550A1E strokesize=0.5 strokecolor=#FFFFFF strokeopacity=102>%s </font>", getLocal(LK::kMyLocalKey_shockRegi));
		base_str += ccsf("<font newline=10 size=9.5 color=#FFFFFF strokesize=1 strokecolor=#000000 strokeopacity=153>+%d%%</font>", wave_int);
	}
	
	int ms_wave_int = round(ms_wave_value);
//	if(ms_wave_int > 0)
	{
		base_str += ccsf("<font size=8.5 color=#550A1E strokesize=0.5 strokecolor=#FFFFFF strokeopacity=102>%s </font>", getLocal(LK::kMyLocalKey_missileFireRegi));
		base_str += ccsf("<font newline=10 size=9.5 color=#FFFFFF strokesize=1 strokecolor=#000000 strokeopacity=153>+%d%%</font>", ms_wave_int);
	}
	
	StyledLabelTTF* base_label = StyledLabelTTF::create(base_str.c_str(), mySGD->getFont().c_str(), 8, 999, StyledAlignment::kLeftAlignment);
	base_label->setAnchorPoint(ccp(0.5f,0.5f));
	
	CCScrollView* base_scroll = CCScrollView::create(CCSizeMake(99, 80));
	base_scroll->setDirection(CCScrollViewDirection::kCCScrollViewDirectionVertical);
	base_scroll->setContainer(base_label);
	base_scroll->setPosition(ccpFromSize(base_back->getContentSize()/2.f) + ccp(3, 0) - ccpMult(ccp(99,80), 0.5f));
	base_scroll->setContentOffset(ccp(0, base_scroll->minContainerOffset().y));
	base_back->addChild(base_scroll);
	base_scroll->setTouchPriority(touch_priority);
	
	CCScale9Sprite* special_back = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	special_back->setContentSize(CCSizeMake(back_in->getContentSize().width-7, 38));
	special_back->setPosition(ccp(back_in->getContentSize().width/2.f, 23));
	back_in->addChild(special_back);
	
	CCSprite* special_node = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 0, 0));
	StyledLabelTTF* special_label = StyledLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_comment_int2_s, cha_idx, cha_level).c_str(), mySGD->getFont().c_str(), 10, 999, StyledAlignment::kLeftAlignment);
	special_label->setAnchorPoint(ccp(0.f,1.f));
	special_node->setPosition(ccp(0,0));
	special_node->addChild(special_label);
	special_node->setTextureRect(CCRectMake(0, 0, special_label->getContentSize().width, special_label->getContentSize().height));
	special_label->setPosition(ccp(0,special_node->getContentSize().height));
	
	if(NSDS_GI(kSDS_GI_characterInfo_int1_maxLevel_i, cha_idx) != cha_level)
	{
		StyledLabelTTF* next_special_label = StyledLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_comment_int2_s, cha_idx, cha_level+1).c_str(), mySGD->getFont().c_str(), 10, 999, StyledAlignment::kLeftAlignment);
		next_special_label->setColorPos(ccGRAY);
		next_special_label->enableOuterStrokePos(ccGRAY, 0, 0, true);
		next_special_label->setAnchorPoint(ccp(0.f,1.f));
		special_node->setTextureRect(CCRectMake(0, 0, MAX(special_label->getContentSize().width, next_special_label->getContentSize().width), special_label->getContentSize().height + next_special_label->getContentSize().height));
		special_label->setPosition(ccp(0,special_node->getContentSize().height));
		
		KSLabelTTF* next_level = KSLabelTTF::create(getLocal(LK::kMyLocalKey_nextLevel), mySGD->getFont().c_str(), 10);
		next_level->setColor(ccGRAY);
		next_level->setAnchorPoint(ccp(0,0.5f));
		next_level->setPosition(ccp(0,next_special_label->getContentSize().height/2.f));
		next_special_label->setPosition(ccp(next_level->getContentSize().width,next_special_label->getContentSize().height));
		special_node->addChild(next_level);
		special_node->addChild(next_special_label);
	}
	
	CCScrollView* special_scroll = CCScrollView::create(CCSizeMake(special_back->getContentSize().width-6, special_back->getContentSize().height-4));
	special_scroll->setDirection(CCScrollViewDirection::kCCScrollViewDirectionVertical);
	special_scroll->setContainer(special_node);
	special_scroll->setPosition(ccpFromSize(special_back->getContentSize()/2.f) - ccpMult(ccp(special_back->getContentSize().width-6, special_back->getContentSize().height-4), 0.5f));
	special_scroll->setContentOffset(ccp(0, special_scroll->minContainerOffset().y));
	special_back->addChild(special_scroll);
	special_scroll->setTouchPriority(touch_priority);
	
	if(mySGD->getUserdataSelectedCharNO() != character_no)
	{
		CCLabelTTF* t_label = CCLabelTTF::create();
		KSLabelTTF* select_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_choice), mySGD->getFont().c_str(), 12);
		t_label->addChild(select_label);
		
		select_button = CCControlButton::create(t_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
		select_button->addTargetWithActionForControlEvents(this, cccontrol_selector(CharacterDetailPopup::selectAction), CCControlEventTouchUpInside);
		select_button->setPreferredSize(CCSizeMake(92, 45));
		select_button->setPosition(ccp(back_case->getContentSize().width/2.f, 43));
		back_case->addChild(select_button);
		select_button->setTouchPriority(touch_priority);
	}
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
	}, [=](){
		is_menu_enable = true;
	});
}

void CharacterDetailPopup::selectAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	CommonAnimation::closePopup(this, m_container, gray, [=](){
		
	}, [=](){
		select_func();
		removeFromParent();
	});
}

