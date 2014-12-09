//
//  CharacterExpUp.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 10. 30..
//
//

#include "CharacterExpUp.h"
#include "StarGoldData.h"
#include "KSUtil.h"
#include "StageImgLoader.h"
#include "KsLocal.h"
#include "KSLabelTTF.h"
#include "ConvexGraph.h"

CharacterExpUp* CharacterExpUp::create(CharacterHistory t_before_history, CharacterHistory t_after_history, CCPoint t_base_position)
{
	CharacterExpUp* t_an = new CharacterExpUp();
	t_an->myInit(t_before_history, t_after_history, t_base_position);
	t_an->autorelease();
	return t_an;
}

void CharacterExpUp::myInit(CharacterHistory t_before_history, CharacterHistory t_after_history, CCPoint t_base_position)
{
	is_max_level = t_before_history.characterLevel.getV() == NSDS_GI(kSDS_GI_characterInfo_int1_maxLevel_i, t_before_history.characterIndex.getV());
	base_position = t_base_position;
	
	setPosition(base_position + ccp(0,27));
	CCScale9Sprite* back_img = CCScale9Sprite::create("cha_noti_back.png", CCRectMake(0, 0, 43, 43), CCRectMake(21, 21, 1, 1));
	back_img->setPosition(CCPointZero);
	addChild(back_img);
	
	back_img->setContentSize(CCSizeMake(200, 54));
	
	CCNode* char_node = CCNode::create();
	
	CCSprite* light_back = KS::loadCCBI<CCSprite*>(this, "hell_cha_back.ccbi").first;
	light_back->setPosition(ccp(0,0));
	char_node->addChild(light_back);
	
	auto t_pair = KS::loadCCBIForFullPath<CCSprite*>(this, StageImgLoader::sharedInstance()->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, t_before_history.characterIndex.getV()) + ".ccbi");
	CCSprite* char_img = t_pair.first;
	char_img->setPosition(ccp(0,0));
	char_node->addChild(char_img);
	
	char_node->setScale(0.5f);
	char_node->setPosition(ccp(30,23));
	back_img->addChild(char_node);
	
	level = t_before_history.characterLevel.getV();
	char_level = KSLabelTTF::create(ccsf(getLocal(LK::kMyLocalKey_levelValue), level), mySGD->getFont().c_str(), 12);
	char_level->setPosition(ccp(30,10));
	back_img->addChild(char_level);
	
	after_level = t_after_history.characterLevel.getV();
	
	KSLabelTTF* take_exp = KSLabelTTF::create(getLocal(LK::kMyLocalKey_expTake), mySGD->getFont().c_str(), 15);
	take_exp->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
	take_exp->enableOuterStroke(ccc3(60, 20, 0), 0.5f, 255, true);
	take_exp->setPosition(ccp(127,37));
	back_img->addChild(take_exp);
	
	progress_timer = ConvexGraph::create("cha_graph_center.png", CCRectMake(0, 0, 15, 15), CCRectMake(7, 7, 1, 1), CCSizeMake(120, 15), ConvexGraphType::width);
	progress_timer->setPosition(ccp(127,16));
	back_img->addChild(progress_timer);
	
	progress_timer->setCover("cha_graph_front.png", "cha_graph_center.png");
	progress_timer->setBack("cha_graph_back.png");
	
	exp_value = t_before_history.characterExp.getV();
	after_exp_value = t_after_history.characterExp.getV();
	after_max_exp_value = t_after_history.characterNextLevelExp.getV();
	
	int sub_base_value = t_before_history.characterNextLevelExp.getV() - t_before_history.characterCurrentLevelExp.getV();
	int sub_value = exp_value - t_before_history.characterCurrentLevelExp.getV();
	
	before_percentage = 100.f*sub_value/sub_base_value;
	if(is_max_level)
		progress_timer->setPercentage(100.f);
	else
		progress_timer->setPercentage(before_percentage);
	
	int after_sub_base_value = t_after_history.characterNextLevelExp.getV() - t_after_history.characterCurrentLevelExp.getV();
	int after_sub_value = after_exp_value - t_after_history.characterCurrentLevelExp.getV();
	after_percentage = 100.f*after_sub_value/after_sub_base_value;
	
	if(level < after_level)
		dis_time = 0.8f/(after_level-level);
	else
		dis_time = 0.f;
	
	float total_up_time = 0.3f + dis_time*(after_level-level);
	exp_per_time = (after_exp_value-exp_value)/total_up_time;
	
	before_exp = exp_value;
	
	exp_label = KSLabelTTF::create(ccsf(getLocal(LK::kMyLocalKey_expN_M), before_percentage/*KS::insert_separator(exp_value).c_str(), KS::insert_separator(after_max_exp_value).c_str()*/), mySGD->getFont().c_str(), 12);
	exp_label->enableOuterStroke(ccBLACK, 0.5f, 100, true);
	exp_label->setPosition(ccp(127,15));
	back_img->addChild(exp_label);
	
	if(is_max_level)
		exp_label->setString("MAX");
	
	setPosition(base_position + ccp(0,back_img->getContentSize().height/2.f));
	
	CCMoveTo* t_show = CCMoveTo::create(0.5f, base_position + ccp(0,-back_img->getContentSize().height/2.f));
	CCCallFunc* t_start = CCCallFunc::create(this, callfunc_selector(CharacterExpUp::startUpAnimation));
	CCSequence* t_seq = CCSequence::create(t_show, t_start, NULL);
	runAction(t_seq);
	
	end_func = [=]()
	{
		CCDelayTime* t_delay = CCDelayTime::create(1.f);
		CCMoveTo* t_hide = CCMoveTo::create(0.5f, base_position + ccp(0,back_img->getContentSize().height/2.f));
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(CharacterExpUp::removeFromParent));
		CCSequence* t_seq2 = CCSequence::create(t_delay, t_hide, t_call2, NULL);
		runAction(t_seq2);
	};
}

void CharacterExpUp::startUpAnimation()
{
	if(is_max_level)
	{
		end_func();
	}
	else
	{
		if(level == after_level)
		{
			addChild(KSGradualValue<float>::create(before_percentage, after_percentage, 0.3f, [=](float t_f)
												   {
													   progress_timer->setPercentage(t_f);
												   }, [=](float t_f)
												   {
													   progress_timer->setPercentage(t_f);
												   }));
			
			addChild(KSGradualValue<float>::create(/*before_exp, after_exp_value*/before_percentage, after_percentage, 0.3f, [=](float t_f)
												   {
													   exp_label->setString(ccsf(getLocal(LK::kMyLocalKey_expN_M), t_f/*KS::insert_separator(t_f, "%.0f").c_str(), KS::insert_separator(after_max_exp_value).c_str()*/));
												   }, [=](float t_f)
												   {
													   exp_label->setString(ccsf(getLocal(LK::kMyLocalKey_expN_M), t_f/*KS::insert_separator(t_f, "%.0f").c_str(), KS::insert_separator(after_max_exp_value).c_str()*/));
													   end_func();
												   }));
		}
		else if(level < after_level)
		{
			addChild(KSGradualValue<float>::create(before_percentage, 100.f, dis_time, [=](float t_f)
												   {
													   progress_timer->setPercentage(t_f);
												   }, [=](float t_f)
												   {
													   progress_timer->setPercentage(t_f);
													   before_percentage = 0.f;
												   }));
			
			addChild(KSGradualValue<float>::create(/*before_exp, before_exp + exp_per_time*dis_time*/before_percentage, 100.f, dis_time, [=](float t_f)
												   {
													   exp_label->setString(ccsf(getLocal(LK::kMyLocalKey_expN_M), t_f/*KS::insert_separator(t_f, "%.0f").c_str(), KS::insert_separator(after_max_exp_value).c_str()*/));
												   }, [=](float t_f)
												   {
													   exp_label->setString(ccsf(getLocal(LK::kMyLocalKey_expN_M), t_f/*KS::insert_separator(t_f, "%.0f").c_str(), KS::insert_separator(after_max_exp_value).c_str()*/));
													   //												 before_exp = t_f;
													   ++level;
													   char_level->setString(ccsf(getLocal(LK::kMyLocalKey_levelValue), level));
													   startUpAnimation();
												   }));
		}
	}
}



