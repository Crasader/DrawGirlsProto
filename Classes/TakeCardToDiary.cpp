//
//  TakeCardToDiary.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 4. 17..
//
//

#include "TakeCardToDiary.h"
#include "MyLocalization.h"
#include "EffectSprite.h"
TakeCardToDiary* TakeCardToDiary::create(int t_take_card_number, std::function<void()> t_end_func)
{
	TakeCardToDiary* t_tctd = new TakeCardToDiary();
	t_tctd->myInit(t_take_card_number, t_end_func);
	t_tctd->autorelease();
	return t_tctd;
}


void TakeCardToDiary::myInit(int t_take_card_number, std::function<void()> t_end_func)
{
	recent_card_number = t_take_card_number;
	end_func = t_end_func;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(255);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray);
	
	recent_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
	recent_left_img->setAnchorPoint(ccp(1.f,0.5f));
	recent_left_img->setPosition(ccp(240,160));
	addChild(recent_left_img,2);
	
	CCSprite* left_cover = CCSprite::create("diary_back_cover.png", CCRectMake(0, 0, 240, 320));
	left_cover->setAnchorPoint(ccp(1.f,0.5f));
	left_cover->setPosition(ccp(240,160));
	recent_left_img->addChild(left_cover, -1);
	
	setLeftPage(recent_left_img, recent_card_number);
	
	
	recent_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
	recent_right_img->setAnchorPoint(ccp(0.f,0.5f));
	recent_right_img->setPosition(ccp(240,160));
	addChild(recent_right_img);
	
	CCSprite* right_cover = CCSprite::create("diary_back_cover.png", CCRectMake(240, 0, 240, 320));
	right_cover->setAnchorPoint(ccp(0,0.5f));
	right_cover->setPosition(ccp(0,160));
	recent_right_img->addChild(right_cover, -1);
	
	setRightPage(recent_right_img, recent_card_number);
	
	//		CCScale9Sprite* text_case = CCScale9Sprite::create("diary_text_box.png", CCRectMake(0,0,35,35), CCRectMake(17,17,1,1));
	//		text_case->setContentSize(CCSizeMake(190, 50));
	//		text_case->setPosition(ccp(112,95));
	//		recent_right_img->addChild(text_case);
	
	AudioEngine::sharedInstance()->playEffect("se_cardget.mp3", false);
	
	CCScaleTo* t_scale = CCScaleTo::create(0.5f, 0.57f);
	CCMoveTo* t_move = CCMoveTo::create(0.5f, ccp(129,161));
	CCSpawn* t_spawn = CCSpawn::create(t_scale, t_move, NULL);
	
	smaller_img->runAction(t_spawn);
	
	CCDelayTime* t_delay1 = CCDelayTime::create(0.3f);
	CCFadeTo* t_fade1 = CCFadeTo::create(0.3f, 255);
	CCSequence* t_seq1 = CCSequence::create(t_delay1, t_fade1, NULL);
	
	top_cover->runAction(t_seq1);
	
	CCDelayTime* t_delay2 = CCDelayTime::create(0.3f);
	CCFadeTo* t_fade2 = CCFadeTo::create(0.3f, 255);
	CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(TakeCardToDiary::startPrePageFull));
	CCSequence* t_seq2 = CCSequence::create(t_delay2, t_fade2, t_call2, NULL);
	
	bottom_cover->runAction(t_seq2);
}


void TakeCardToDiary::startPrePageFull()
{
	animation_angle = 0;
	animation_img = recent_left_img;
	touch_direction = -1;
	touch_end_direction = -1;
	end_animation_delegate = callfunc_selector(TakeCardToDiary::startPrePage);
	
	schedule(schedule_selector(TakeCardToDiary::ingPageFull));
}


void TakeCardToDiary::ingPageFull()
{
	animation_angle += 3.f;
	
	animation_img->setSkewY(-touch_direction*touch_end_direction*animation_angle/4.f);
	animation_img->setScaleX(cosf(animation_angle/180.f*M_PI));
	
	if(animation_angle >= 90.f)
	{
		unschedule(schedule_selector(TakeCardToDiary::ingPageFull));
		animation_img = NULL;
		(this->*end_animation_delegate)();
	}
}


void TakeCardToDiary::startPrePage()
{
	recent_left_img->removeFromParent();
	recent_left_img = CCSprite::create("diary_cover_right.png");
	recent_left_img->setAnchorPoint(ccp(0.f,0.5f));
	recent_left_img->setPosition(ccp(240,160));
	addChild(recent_left_img, 2);
	
	animation_angle = 90.f;
	
	recent_left_img->setSkewY(animation_angle/4.f);
	recent_left_img->setScaleX(cosf(animation_angle/180.f*M_PI));
	
	animation_img = recent_left_img;
	
	touch_end_direction = 1;
	
	schedule(schedule_selector(TakeCardToDiary::ingPage));
}

void TakeCardToDiary::ingPage()
{
	animation_angle -= 3.f;
	
	if(animation_angle < 0.f)
		animation_angle = 0.f;
	
	animation_img->setSkewY(-touch_direction*touch_end_direction*animation_angle/4.f);
	animation_img->setScaleX(cosf(animation_angle/180.f*M_PI));
	
	if(animation_angle <= 0.f)
	{
		unschedule(schedule_selector(TakeCardToDiary::ingPage));
		animation_img = NULL;
		startHiding();
	}
}


void TakeCardToDiary::startHiding()
{
	recent_right_img->removeFromParent();
	
	addChild(KSGradualValue<float>::create(1.f, 0.f, 0.5f, [=](float t)
										   {
											   gray->setOpacity(t*255);
											   KS::setOpacity(recent_left_img, t*255);
//											   KS::setOpacity(recent_right_img, t*255);
										   }, [=](float t)
										   {
											   gray->setOpacity(0);
											   KS::setOpacity(recent_left_img, 0);
//											   KS::setOpacity(recent_right_img, 0);
											   end_func();
											   removeFromParent();
										   }));
}


CCPoint TakeCardToDiary::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kTCTD_MT_second)	return_value = ccp(50,225);
	else if(t_tag == kTCTD_MT_third)	return_value = ccp(110,225);
	else if(t_tag == kTCTD_MT_forth)	return_value = ccp(170,225);
	
	return return_value;
}

void TakeCardToDiary::setRightPage(CCNode *target, int card_number)
{
	KSLabelTTF* r_stage_name = KSLabelTTF::create(NSDS_GS(kSDS_CI_int1_name_s, card_number).c_str(), mySGD->getFont().c_str(), 10);
	r_stage_name->setPosition(ccp(32,273));
	r_stage_name->setAnchorPoint(ccp(0,0.5f));
	target->addChild(r_stage_name);
	
	
	CCLabelTTF* r_stage_script = CCLabelTTF::create(NSDS_GS(kSDS_CI_int1_script_s, card_number).c_str(), mySGD->getFont().c_str(), 9, CCSizeMake(175, 35), kCCTextAlignmentLeft);
	r_stage_script->setPosition(ccp(25,253));
	r_stage_script->setColor(ccBLACK);
	r_stage_script->setVerticalAlignment(kCCVerticalTextAlignmentTop);
	r_stage_script->setAnchorPoint(ccp(0,1));
	target->addChild(r_stage_script);
	
	
	KSLabelTTF* r_profile_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_profile), mySGD->getFont().c_str(), 10);
	r_profile_label->setPosition(ccp(32,208));
	r_profile_label->setAnchorPoint(ccp(0,0.5f));
	target->addChild(r_profile_label);
	
	CCLabelTTF* r_stage_profile = CCLabelTTF::create(NSDS_GS(kSDS_CI_int1_profile_s, card_number).c_str(), mySGD->getFont().c_str(), 10, CCSizeMake(175, 50), kCCTextAlignmentLeft);
	r_stage_profile->setPosition(ccp(25,189));
	r_stage_profile->setColor(ccBLACK);
	r_stage_profile->setVerticalAlignment(kCCVerticalTextAlignmentTop);
	r_stage_profile->setAnchorPoint(ccp(0,1));
	target->addChild(r_stage_profile);
	
	
	KSLabelTTF* r_card_elemental_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_cardElemental), mySGD->getFont().c_str(), 11);
	r_card_elemental_label->enableOuterStroke(ccBLACK, 1, int(255*0.8), true);
	r_card_elemental_label->setPosition(ccp(48,111));
	target->addChild(r_card_elemental_label);
	
	string elemental_str, elemental_filename;
	int card_type = NSDS_GI(kSDS_CI_int1_type_i, card_number);
	
	if(card_type == 0)
	{
		elemental_str = "음";
		elemental_filename = "diary_icon_light.png";
	}
	else if(card_type == 1)
	{
		elemental_str = "양";
		elemental_filename = "diary_icon_shaded.png";
	}
	else if(card_type == 2)
	{
		elemental_str = "섬";
		elemental_filename = "diary_icon_sun.png";
	}
	else if(card_type == 3)
	{
		elemental_str = "음란";
		elemental_filename = "diary_icon_sun.png";
	}
	else if(card_type == 4)
	{
		elemental_str = "양란";
		elemental_filename = "diary_icon_sun.png";
	}
	else if(card_type == 5)
	{
		elemental_str = "섬란";
		elemental_filename = "diary_icon_sun.png";
	}
	
	
//	KSLabelTTF* r_elemental = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_elementalValue), elemental_str.c_str()), mySGD->getFont().c_str(), 9);
//	r_elemental->setColor(ccBLACK);
//	r_elemental->setPosition(ccp(45,95));
//	target->addChild(r_elemental);
	
	CCSprite* elemental_img = CCSprite::create(elemental_filename.c_str());
	elemental_img->setPosition(ccp(48,91));
	target->addChild(elemental_img);

	
	KSLabelTTF* r_card_number_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_cardNumber), mySGD->getFont().c_str(), 11);
	r_card_number_label->enableOuterStroke(ccBLACK, 1, int(255*0.8), true);
	r_card_number_label->setPosition(ccp(110,111));
	target->addChild(r_card_number_label);
	
	KSLabelTTF* r_card_number_value = KSLabelTTF::create(ccsf("%d", /*NSDS_GI(kSDS_CI_int1_serial_i, */card_number/*)*/), mySGD->getFont().c_str(), 20);
	r_card_number_value->setColor(ccc3(255, 95, 95));
	r_card_number_value->enableOuterStroke(ccBLACK, 1, int(255*0.8), true);
	r_card_number_value->setPosition(ccp(110,88));
	target->addChild(r_card_number_value);
	
	
	KSLabelTTF* r_take_count_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_cardTakeCnt), mySGD->getFont().c_str(), 11);
	r_take_count_label->enableOuterStroke(ccBLACK, 1, int(255*0.8), true);
	r_take_count_label->setPosition(ccp(172,111));
	target->addChild(r_take_count_label);
	
	CardSortInfo t_card_info = mySGD->getHasGottenCardDataForCardNumber(card_number);
	KSLabelTTF* r_take_count_value = KSLabelTTF::create(ccsf("%d", t_card_info.count.getV()), mySGD->getFont().c_str(), 20);
	r_take_count_value->setColor(ccc3(95, 200, 255));
	r_take_count_value->enableOuterStroke(ccBLACK, 1, int(255*0.8), true);
	r_take_count_value->setPosition(ccp(172,88));
	target->addChild(r_take_count_value);
	
//	KSLabelTTF* r_touch = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_touch), mySGD->getFont().c_str(), 9);
//	r_touch->setColor(ccBLACK);
//	r_touch->setPosition(ccp(90,95));
//	target->addChild(r_touch);
//	
//	if(mySGD->isCardMorphing(card_number))
//	{
//		CCSprite* touch_img = CCSprite::create("diary_icon_touch.png");
//		touch_img->setPosition(ccp(90,74));
//		target->addChild(touch_img);
//	}
//	else
//	{
//		CCSprite* not_touch_img = CCSprite::create("diary_icon_lock.png");
//		not_touch_img->setPosition(ccp(90,74));
//		target->addChild(not_touch_img);
//	}
//	
//	if(NSDS_GI(kSDS_CI_int1_soundCnt_i, card_number) > 0)
//	{
//		KSLabelTTF* r_sound = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_sound), mySGD->getFont().c_str(), 9);
//		r_sound->setColor(ccBLACK);
//		r_sound->setPosition(ccp(135,95));
//		target->addChild(r_sound);
//		
//		if(mySGD->isCardMorphing(card_number))
//		{
//			CCSprite* sound_img = CCSprite::create("diary_icon_sound.png");
//			sound_img->setPosition(ccp(135,74));
//			target->addChild(sound_img);
//		}
//		else
//		{
//			CCSprite* not_sound_img = CCSprite::create("diary_icon_lock.png");
//			not_sound_img->setPosition(ccp(135,74));
//			target->addChild(not_sound_img);
//		}
//	}
	
	string card_category = NSDS_GS(kSDS_CI_int1_category_s, card_number);
	if(card_category == "")
		card_category = "normal";
	if(card_category == "nPuzzle" || card_category == "ePuzzle")
	{
		int stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
		int puzzle_number = NSDS_GI(stage_number, kSDS_SI_puzzle_i);
		
		CCLabelTTF* r_stage_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_puzzleValue), puzzle_number)->getCString(), mySGD->getFont().c_str(), 8);
		r_stage_label->setAnchorPoint(ccp(1,0.5f));
		r_stage_label->setPosition(ccp(167, 288));
		r_stage_label->setColor(ccBLACK);
		target->addChild(r_stage_label);
	}
	else
	{
		int stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
		
		CCLabelTTF* r_stage_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_stageValue), stage_number)->getCString(), mySGD->getFont().c_str(), 8);
		r_stage_label->setAnchorPoint(ccp(1,0.5f));
		r_stage_label->setPosition(ccp(167, 288));
		r_stage_label->setColor(ccBLACK);
		target->addChild(r_stage_label);
	}
}

void TakeCardToDiary::setLeftPage(CCNode *target, int card_number)
{
//	KSLabelTTF* take_cnt_label = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_cardTakeCnt), mySGD->getHasGottenCardDataForCardNumber(card_number).count.getV()), mySGD->getFont().c_str(), 9);
//	take_cnt_label->setColor(ccBLACK);
//	take_cnt_label->disableOuterStroke();
//	take_cnt_label->setAnchorPoint(ccp(0,0.5f));
//	take_cnt_label->setPosition(ccp(38,28));
//	target->addChild(take_cnt_label);
	
//	KSLabelTTF* card_number_label = KSLabelTTF::create(ccsf("No.%d", card_number), mySGD->getFont().c_str(), 9);
//	card_number_label->setColor(ccBLACK);
//	card_number_label->disableOuterStroke();
//	card_number_label->setAnchorPoint(ccp(0,0.5f));
//	card_number_label->setPosition(ccp(38,294));
//	target->addChild(card_number_label);
	
	CCSprite* r_card_img = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png", card_number)->getCString());
	r_card_img->setScale(1.5f/myDSH->screen_convert_rate);
	
//	if(NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, card_number))
//	{
//		CCSprite* ccb_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, card_number)).first;
//		ccb_img->setPosition(ccp(160,215));
//		r_card_img->addChild(ccb_img);
//	}
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	
	float screen_width = 480.f/myDSH->screen_convert_rate;
	float screen_height = screen_width*screen_size.height/screen_size.width;
	float img_height = 430*r_card_img->getScale();
	
	r_card_img->setPosition(ccp(240, myDSH->ui_center_y));
	target->addChild(r_card_img);
	
	smaller_img = r_card_img;
	
	if(mySGD->is_safety_mode)
	{
		EffectSprite* safety_img = EffectSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()));
		safety_img->setSilhouetteConvert(0);
		safety_img->setPosition(ccpFromSize(r_card_img->getContentSize()/2.f));
		r_card_img->addChild(safety_img);
	}
	
	//		if(NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, card_number))
	//		{
	//			CollectionAnimation* t_ca = CollectionAnimation::create(card_number);
	//			r_card_img->addChild(t_ca);
	//		}
	
	CCSprite* left_top_paper = CCSprite::create("diary_paper.png");
	left_top_paper->setPosition(ccp(46,278));
	target->addChild(left_top_paper);
	left_top_paper->setOpacity(0);
	
	top_cover = left_top_paper;
	
	CCSprite* right_bottom_paper = CCSprite::create("diary_paper.png");
	right_bottom_paper->setRotation(180);
	right_bottom_paper->setPosition(ccp(213,44));
	target->addChild(right_bottom_paper);
	bottom_cover = right_bottom_paper;
	right_bottom_paper->setOpacity(0);
}