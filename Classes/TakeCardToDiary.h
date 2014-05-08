//
//  TakeCardToDiary.h
//  DGproto
//
//  Created by 사원3 on 2014. 4. 17..
//
//

#ifndef __DGproto__TakeCardToDiary__
#define __DGproto__TakeCardToDiary__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"
#include "KSUtil.h"
#include <random>
#include "KSLabelTTF.h"
#include "MyLocalization.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum TakeCardToDiaryMenuTag
{
	kTCTD_MT_second = 1,
	kTCTD_MT_third,
	kTCTD_MT_forth
};

class TakeCardToDiary : public CCLayer
{
public:
	static TakeCardToDiary* create(int t_take_card_number, std::function<void()> t_end_func)
	{
		TakeCardToDiary* t_tctd = new TakeCardToDiary();
		t_tctd->myInit(t_take_card_number, t_end_func);
		t_tctd->autorelease();
		return t_tctd;
	}
	
private:
	
	int recent_card_number;
	std::function<void()> end_func;
	
	CCSprite* recent_left_img;
	CCSprite* recent_right_img;
	CCSprite* smaller_img;
	CCSprite* top_cover;
	CCSprite* bottom_cover;
	CCSprite* gray;
	
	void myInit(int t_take_card_number, std::function<void()> t_end_func)
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
		
		setLeftPage(recent_left_img, recent_card_number);
		
		
		recent_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
		recent_right_img->setAnchorPoint(ccp(0.f,0.5f));
		recent_right_img->setPosition(ccp(240,160));
		addChild(recent_right_img);
		
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
	
	float animation_angle;
	CCSprite* animation_img;
	int touch_direction;
	int touch_end_direction;
	SEL_CallFunc end_animation_delegate;
	
	void startPrePageFull()
	{
		animation_angle = 0;
		animation_img = recent_left_img;
		touch_direction = -1;
		touch_end_direction = -1;
		end_animation_delegate = callfunc_selector(TakeCardToDiary::startPrePage);
		
		schedule(schedule_selector(TakeCardToDiary::ingPageFull));
	}
	
	void ingPageFull()
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
	
	void startPrePage()
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

	void ingPage()
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
	
	void startHiding()
	{
		addChild(KSGradualValue<float>::create(1.f, 0.f, 0.5f, [=](float t)
											   {
												   gray->setOpacity(t*255);
												   KS::setOpacity(recent_left_img, t*255);
												   KS::setOpacity(recent_right_img, t*255);
											   }, [=](float t)
											   {
												   gray->setOpacity(0);
												   KS::setOpacity(recent_left_img, 0);
												   KS::setOpacity(recent_right_img, 0);
												   end_func();
												   removeFromParent();
											   }));
	}
	
	CCPoint getContentPosition(int t_tag)
	{
		CCPoint return_value;
		
		if(t_tag == kTCTD_MT_second)	return_value = ccp(50,225);
		else if(t_tag == kTCTD_MT_third)	return_value = ccp(110,225);
		else if(t_tag == kTCTD_MT_forth)	return_value = ccp(170,225);
		
		return return_value;
	}
	
	void setRightPage(CCNode *target, int card_number)
	{
		CCLabelTTF* r_stage_script = CCLabelTTF::create(NSDS_GS(kSDS_CI_int1_script_s, card_number).c_str(), mySGD->getFont().c_str(), 13, CCSizeMake(180, 60), kCCTextAlignmentLeft);
		r_stage_script->setPosition(ccp(25,152));
		r_stage_script->setColor(ccBLACK);
		r_stage_script->setVerticalAlignment(kCCVerticalTextAlignmentTop);
		r_stage_script->setAnchorPoint(ccp(0,1));
		target->addChild(r_stage_script);
		
		
		KSLabelTTF* r_stage_name = KSLabelTTF::create(NSDS_GS(kSDS_CI_int1_name_s, card_number).c_str(), mySGD->getFont().c_str(), 13, CCSizeMake(180, 60), kCCTextAlignmentLeft); //create(NSDS_GS(kSDS_CI_int1_name_s, card_number).c_str(), mySGD->getFont().c_str(), 13, CCSizeMake(180, 60), kCCTextAlignmentLeft);
		r_stage_name->setColor(ccWHITE);
		r_stage_name->enableOuterStroke(ccBLACK, 1);
		r_stage_name->setPosition(ccp(32,109));
		r_stage_name->setVerticalAlignment(kCCVerticalTextAlignmentTop);
		r_stage_name->setAnchorPoint(ccp(0,1));
		target->addChild(r_stage_name);
		
		CCLabelTTF* r_stage_profile = CCLabelTTF::create(NSDS_GS(kSDS_CI_int1_profile_s, card_number).c_str(), mySGD->getFont().c_str(), 9, CCSizeMake(180, 100), kCCTextAlignmentLeft);
		r_stage_profile->setPosition(ccp(32,95));
		r_stage_profile->setColor(ccBLACK);
		r_stage_profile->setVerticalAlignment(kCCVerticalTextAlignmentTop);
		r_stage_profile->setAnchorPoint(ccp(0,1));
		target->addChild(r_stage_profile);

		
		
		float mul_value = 0.88f;
		int stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
		int level_number = NSDS_GI(kSDS_CI_int1_grade_i, card_number);
		
		int position_index = 1;
		
		int stage_card_count = NSDS_GI(stage_number, kSDS_SI_cardCount_i);
		target->setTag(stage_card_count);
		
		random_device rd;
		default_random_engine e1(rd());
		uniform_int_distribution<int> uniform_dist(-10, 10);
		uniform_int_distribution<int> uniform_dist_x(8, 82);
		uniform_int_distribution<int> uniform_dist_y(8, 112);
		uniform_int_distribution<int> uniform_dist_cnt(1, 2);
		uniform_int_distribution<int> uniform_dist_type(1, 3);
		uniform_int_distribution<int> uniform_dist_rotate(-90,90);
		
		for(int i=1;i<=stage_card_count;i++)
		{
			if(i == level_number)
				continue;
			
			int check_card_number = NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, i);
			
			int rotation_value;
			CCPoint position_value;
			
			if(position_index == 1)
			{
				rotation_value = uniform_dist(e1);
				position_value = ccpAdd(getContentPosition(kTCTD_MT_second), ccp(0,uniform_dist(e1)));
			}
			else if(position_index == 2)
			{
				rotation_value = uniform_dist(e1);
				position_value = ccpAdd(getContentPosition(kTCTD_MT_third), ccp(0,uniform_dist(e1)));
			}
			else
			{
				rotation_value = uniform_dist(e1);
				position_value = ccpAdd(getContentPosition(kTCTD_MT_forth), ccp(0,uniform_dist(e1)));
			}
			
			if(mySGD->isHasGottenCards(check_card_number) != 0)
			{
				CCSprite* second_img = mySIL->getLoadedImg(CCString::createWithFormat("card%d_thumbnail.png", check_card_number)->getCString());
				second_img->setScale(mul_value);
				second_img->setRotation(rotation_value);
				second_img->setPosition(position_value);
				target->addChild(second_img);
				
				CCSprite* img_case = CCSprite::create("diary_frame.png");
				img_case->setPosition(ccp(second_img->getContentSize().width/2.f, second_img->getContentSize().height/2.f-4.f));
				second_img->addChild(img_case);
				
				int tape_cnt = uniform_dist_cnt(e1);
				for(int j=0;j<tape_cnt;j++)
				{
					int tape_type = uniform_dist_type(e1);
					CCSprite* tape_img = CCSprite::create(CCString::createWithFormat("diary_tape%d.png", tape_type)->getCString());
					bool is_x = uniform_dist_cnt(e1) == 1;
					bool is_first = uniform_dist_cnt(e1) == 1;
					if(is_x)
					{
						if(is_first)
							tape_img->setPosition(ccp(uniform_dist_x(e1), 8));
						else
							tape_img->setPosition(ccp(uniform_dist_x(e1), 112));
					}
					else
					{
						if(is_first)
							tape_img->setPosition(ccp(8, uniform_dist_y(e1)));
						else
							tape_img->setPosition(ccp(82, uniform_dist_y(e1)));
					}
					img_case->addChild(tape_img);
					tape_img->setRotation(uniform_dist_rotate(e1));
				}
			}
			else
			{
				CCSprite* no_img = CCSprite::create("diary_nophoto.png");
				
				KSLabelTTF* no_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_diaryNoImg), mySGD->getFont().c_str(), 8);
				no_label->setColor(ccc3(60, 60, 60));
				no_label->setPosition(ccp(no_img->getContentSize().width/2.f, no_img->getContentSize().height/2.f));
				no_img->addChild(no_label);
				
				no_img->setScale(mul_value);
				no_img->setRotation(rotation_value);
				no_img->setPosition(position_value);
				target->addChild(no_img);
				
				CCSprite* img_case = CCSprite::create("diary_frame.png");
				img_case->setPosition(ccp(no_img->getContentSize().width/2.f, no_img->getContentSize().height/2.f-4.f));
				no_img->addChild(img_case);
				
				int tape_cnt = uniform_dist_cnt(e1);
				for(int j=0;j<tape_cnt;j++)
				{
					int tape_type = uniform_dist_type(e1);
					CCSprite* tape_img = CCSprite::create(CCString::createWithFormat("diary_tape%d.png", tape_type)->getCString());
					bool is_x = uniform_dist_cnt(e1) == 1;
					bool is_first = uniform_dist_cnt(e1) == 1;
					if(is_x)
					{
						if(is_first)
							tape_img->setPosition(ccp(uniform_dist_x(e1), 8));
						else
							tape_img->setPosition(ccp(uniform_dist_x(e1), 112));
					}
					else
					{
						if(is_first)
							tape_img->setPosition(ccp(8, uniform_dist_y(e1)));
						else
							tape_img->setPosition(ccp(82, uniform_dist_y(e1)));
					}
					img_case->addChild(tape_img);
					tape_img->setRotation(uniform_dist_rotate(e1));
				}
			}
			position_index++;
		}
		
		CCLabelTTF* r_stage_label = CCLabelTTF::create(CCString::createWithFormat("STAGE %d", stage_number)->getCString(), mySGD->getFont().c_str(), 8);
		r_stage_label->setAnchorPoint(ccp(0,0.5f));
		r_stage_label->setPosition(ccp(138, 287));
		r_stage_label->setColor(ccBLACK);
		r_stage_label->setHorizontalAlignment(kCCTextAlignmentCenter);
		r_stage_label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		target->addChild(r_stage_label);
	}
	
	void setLeftPage(CCNode *target, int card_number)
	{
		CCSprite* r_card_img = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png", card_number)->getCString());
		r_card_img->setScale(1.5f/myDSH->screen_convert_rate);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		
		float screen_width = 480.f/myDSH->screen_convert_rate;
		float screen_height = screen_width*screen_size.height/screen_size.width;
		float img_height = 430*r_card_img->getScale();
		
		r_card_img->setPosition(ccp(240, screen_height-img_height/2.f));
		target->addChild(r_card_img);
		
		smaller_img = r_card_img;
		
//		if(NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, card_number))
//		{
//			CollectionAnimation* t_ca = CollectionAnimation::create(card_number);
//			r_card_img->addChild(t_ca);
//		}
		
		CCSprite* left_top_paper = CCSprite::create("diary_paper.png");
		left_top_paper->setPosition(ccp(46,277));
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
};


#endif /* defined(__DGproto__TakeCardToDiary__) */
