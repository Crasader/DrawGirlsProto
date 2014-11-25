//
//  RivalSelectPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 31..
//
//

#include "RivalSelectPopup.h"
#include "CCMenuLambda.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "DataStorageHub.h"
#include "ASPopupView.h"
#include "MyLocalization.h"
#include "AudioEngine.h"
#include "FormSetter.h"
#include "FlagSelector.h"
#include "CommonAnimation.h"

RivalSelectPopup* RivalSelectPopup::create(int t_touch_priority, function<void()> t_cancel_func, function<void()> t_selected_func)
{
	RivalSelectPopup* t_mup = new RivalSelectPopup();
	t_mup->myInit(t_touch_priority, t_cancel_func, t_selected_func);
	t_mup->autorelease();
	return t_mup;
}

void RivalSelectPopup::myInit(int t_touch_priority, function<void()> t_cancel_func, function<void()> t_selected_func)
{
	startFormSetter(this);
	
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	cancel_func = t_cancel_func;
	selected_func = t_selected_func;
	
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
	
	CCScale9Sprite* title_tag = CCScale9Sprite::create("title_tag.png", CCRectMake(0, 0, 135, 30), CCRectMake(45, 14, 45, 2));
	title_tag->setContentSize(CCSizeMake(160, 30));
	title_tag->setPosition(ccp(80, back_case->getContentSize().height-35));
	back_case->addChild(title_tag);
	
	title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rivalSearch), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(-75,back_case->getContentSize().height/2.f-35));
	m_container->addChild(title_label);
	
	select_menu = CCMenuLambda::create();
	select_menu->setPosition(ccpFromSize(back_case->getContentSize()/2.f));
	back_case->addChild(select_menu);
	select_menu->setTouchPriority(touch_priority);
	select_menu->setEnabled(false);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width-25, back_case->getContentSize().height-25));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
									  return;
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  
									CommonAnimation::closePopup(this, m_container, gray, [=](){
										
									}, [=](){
										cancel_func(); removeFromParent();
									});
							  });
	back_case->addChild(close_button);
	
	//if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
		close_button->setVisible(false);
	
	left_case = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	left_case->setContentSize(CCSizeMake(80, 70));
	left_case->setPosition(ccp(back_case->getContentSize().width/2.f-90, back_case->getContentSize().height/2.f+5));
	back_case->addChild(left_case);
	
	left_node = CCNode::create();
	left_node->setPosition(ccp(left_case->getContentSize().width/2.f, left_case->getContentSize().height/2.f));
	left_case->addChild(left_node);
	
	{
		CCSprite* t_question = CCSprite::create("startsetting_question.png");
		t_question->setPosition(CCPointZero);
		left_node->addChild(t_question);
	}
	
	
	center_case = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	center_case->setContentSize(CCSizeMake(80, 70));
	center_case->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f+5));
	back_case->addChild(center_case);
	
	center_node = CCNode::create();
	center_node->setPosition(ccp(center_case->getContentSize().width/2.f, center_case->getContentSize().height/2.f));
	center_case->addChild(center_node);
	
	{
		CCSprite* t_question = CCSprite::create("startsetting_question.png");
		t_question->setPosition(CCPointZero);
		center_node->addChild(t_question);
	}
	
	right_case = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	right_case->setContentSize(CCSizeMake(80, 70));
	right_case->setPosition(ccp(back_case->getContentSize().width/2.f+90, back_case->getContentSize().height/2.f+5));
	back_case->addChild(right_case);
	
	right_node = CCNode::create();
	right_node->setPosition(ccp(right_case->getContentSize().width/2.f, right_case->getContentSize().height/2.f));
	right_case->addChild(right_node);
	
	{
		CCSprite* t_question = CCSprite::create("startsetting_question.png");
		t_question->setPosition(CCPointZero);
		right_node->addChild(t_question);
	}
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		startRivalAnimation();
	});
}

void RivalSelectPopup::startRivalAnimation()
{
	function<void(CCNode*, function<void()>)> slim_animation = [=](CCNode* target_node, function<void()> end_func){
		addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
									  {
										  target_node->setScaleX(1.f - t);
									  }, [=](float t)
									  {
										  target_node->setScaleX(0.f);
										  end_func();
									  }));
	};
	
	function<void(CCNode*, function<void()>)> wide_animation = [=](CCNode* target_node, function<void()> end_func){
		addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
											   {
												   target_node->setScaleX(t);
											   }, [=](float t)
											   {
												   target_node->setScaleX(1.f);
												   end_func();
											   }));
	};
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
	
	slim_animation(left_case, [=]()
	{
		wide_animation(left_case, [=]()
		{
			slim_animation(left_case, [=]()
			{
				wide_animation(left_case, [=]()
				{
					slim_animation(left_case, [=]()
					{
						wide_animation(left_case, [=]()
						{
							slim_animation(left_case, [=]()
							{
								// set
								left_node->removeAllChildren();
								
								CCSprite* left_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(mySGD->dummy_list[0]["flag"].asString()).c_str());
								left_flag->setScale(1.f);
								left_flag->setPosition(ccp(0, 18));
								left_node->addChild(left_flag);
								
								KSLabelTTF* left_nick = KSLabelTTF::create(mySGD->dummy_list[0]["nick"].asString().c_str(), mySGD->getFont().c_str(), 10);
								left_nick->setPosition(ccp(0, -3));
								left_node->addChild(left_nick);
								
//								KSLabelTTF* left_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rivalLevel), mySGD->dummy_list[0]["level"].asInt())->getCString(), mySGD->getFont().c_str(), 12);
//								left_level->setPosition(ccp(0, -10));
//								left_node->addChild(left_level);
								
								KSLabelTTF* left_ing_win = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rivalIngWin), mySGD->dummy_list[0]["victory"].asInt())->getCString(), mySGD->getFont().c_str(), 15);
								left_ing_win->setColor(ccc3(255,170,20));
								left_ing_win->setPosition(ccp(0, -19));
								left_node->addChild(left_ing_win);
								
								wide_animation(left_case, [=]()
								{
									slim_animation(left_case, [=]()
									{
										wide_animation(left_case, [=]()
										{
											
										});
									});
								});
							});
						});
						
						
						slim_animation(center_case, [=]()
						{
							wide_animation(center_case, [=]()
							{
								slim_animation(center_case, [=]()
								{
									wide_animation(center_case, [=]()
									{
										slim_animation(center_case, [=]()
										{
											wide_animation(center_case, [=]()
											{
												slim_animation(center_case, [=]()
												{
													// set
													center_node->removeAllChildren();
													
													CCSprite* center_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(mySGD->dummy_list[1]["flag"].asString()).c_str());
													center_flag->setScale(1.f);
													center_flag->setPosition(ccp(0, 18));
													center_node->addChild(center_flag);
													
													KSLabelTTF* center_nick = KSLabelTTF::create(mySGD->dummy_list[1]["nick"].asString().c_str(), mySGD->getFont().c_str(), 10);
													center_nick->setPosition(ccp(0, -3));
													center_node->addChild(center_nick);
													
//													KSLabelTTF* center_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rivalLevel), mySGD->dummy_list[1]["level"].asInt())->getCString(), mySGD->getFont().c_str(), 12);
//													center_level->setPosition(ccp(0, -10));
//													center_node->addChild(center_level);
													
													KSLabelTTF* center_ing_win = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rivalIngWin), mySGD->dummy_list[1]["victory"].asInt())->getCString(), mySGD->getFont().c_str(), 15);
													center_ing_win->setColor(ccc3(255,170,20));
													center_ing_win->setPosition(ccp(0, -19));
													center_node->addChild(center_ing_win);
													
													wide_animation(center_case, [=]()
													{
														slim_animation(center_case, [=]()
														{
															wide_animation(center_case, [=]()
															{
																
															});
														});
													});
												});
											});
											
											slim_animation(right_case, [=]()
											{
												wide_animation(right_case, [=]()
												{
													slim_animation(right_case, [=]()
													{
														wide_animation(right_case, [=]()
														{
															slim_animation(right_case, [=]()
															{
																wide_animation(right_case, [=]()
																{
																	slim_animation(right_case, [=]()
																	{
																		// set
																		right_node->removeAllChildren();
																		
																		CCSprite* right_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(mySGD->dummy_list[2]["flag"].asString()).c_str());
																		right_flag->setScale(1.f);
																		right_flag->setPosition(ccp(0, 18));
																		right_node->addChild(right_flag);
																		
																		KSLabelTTF* right_nick = KSLabelTTF::create(mySGD->dummy_list[2]["nick"].asString().c_str(), mySGD->getFont().c_str(), 10);
																		right_nick->setPosition(ccp(0, -3));
																		right_node->addChild(right_nick);
																		
//																		KSLabelTTF* right_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rivalLevel), mySGD->dummy_list[2]["level"].asInt())->getCString(), mySGD->getFont().c_str(), 12);
//																		right_level->setPosition(ccp(0, -10));
//																		right_node->addChild(right_level);
																		
																		KSLabelTTF* right_ing_win = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rivalIngWin), mySGD->dummy_list[2]["victory"].asInt())->getCString(), mySGD->getFont().c_str(), 15);
																		right_ing_win->setColor(ccc3(255,170,20));
																		right_ing_win->setPosition(ccp(0, -19));
																		right_node->addChild(right_ing_win);
																		
																		wide_animation(right_case, [=]()
																		{
																			slim_animation(right_case, [=]()
																			{
																				wide_animation(right_case, [=]()
																				{
																					addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
																														   {
																															   title_label->setOpacity(255-t*255);
																														   }, [=](float t)
																														   {
																															   title_label->setOpacity(0);
																															   title_label->setString(myLoc->getLocalForKey(LK::kMyLocalKey_rivalSelect));
																															   addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
																																									  {
																																										  title_label->setOpacity(t*255);
																																									  }, [=](float t)
																																									  {
																																										  title_label->setOpacity(255);
																																										  
																																										  CCSprite* n_left = CCSprite::create("subbutton_purple4.png");
																																										  KSLabelTTF* n_left_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_choice), mySGD->getFont().c_str(), 12);
																																										  n_left_label->setPosition(ccpFromSize(n_left->getContentSize()/2.f));
																																										  n_left->addChild(n_left_label);
																																										  CCSprite* s_left = CCSprite::create("subbutton_purple4.png");
																																										  s_left->setColor(ccGRAY);
																																										  KSLabelTTF* s_left_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_choice), mySGD->getFont().c_str(), 12);
																																										  s_left_label->setPosition(ccpFromSize(s_left->getContentSize()/2.f));
																																										  s_left->addChild(s_left_label);
																																										  
																																										  
																																										  CCMenuItemLambda* left_item = CCMenuItemSpriteLambda::create(n_left, s_left, [=](CCObject* sender){selectRival(0);});
																																										  left_item->setPosition(ccp(-90,-52));
																																										  select_menu->addChild(left_item);
																																										  
																																										  
																																										  CCSprite* n_center = CCSprite::create("subbutton_purple4.png");
																																										  KSLabelTTF* n_center_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_choice), mySGD->getFont().c_str(), 12);
																																										  n_center_label->setPosition(ccpFromSize(n_center->getContentSize()/2.f));
																																										  n_center->addChild(n_center_label);
																																										  CCSprite* s_center = CCSprite::create("subbutton_purple4.png");
																																										  s_center->setColor(ccGRAY);
																																										  KSLabelTTF* s_center_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_choice), mySGD->getFont().c_str(), 12);
																																										  s_center_label->setPosition(ccpFromSize(s_center->getContentSize()/2.f));
																																										  s_center->addChild(s_center_label);
																																										  
																																										  CCMenuItemLambda* center_item = CCMenuItemSpriteLambda::create(n_center, s_center, [=](CCObject* sender){selectRival(1);});
																																										  center_item->setPosition(ccp(0,-52));
																																										  select_menu->addChild(center_item);
																																										  
																																										  
																																										  CCSprite* n_right = CCSprite::create("subbutton_purple4.png");
																																										  KSLabelTTF* n_right_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_choice), mySGD->getFont().c_str(), 12);
																																										  n_right_label->setPosition(ccpFromSize(n_right->getContentSize()/2.f));
																																										  n_right->addChild(n_right_label);
																																										  CCSprite* s_right = CCSprite::create("subbutton_purple4.png");
																																										  s_right->setColor(ccGRAY);
																																										  KSLabelTTF* s_right_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_choice), mySGD->getFont().c_str(), 12);
																																										  s_right_label->setPosition(ccpFromSize(s_right->getContentSize()/2.f));
																																										  s_right->addChild(s_right_label);
																																										  
																																										  CCMenuItemLambda* right_item = CCMenuItemSpriteLambda::create(n_right, s_right, [=](CCObject* sender){selectRival(2);});
																																										  right_item->setPosition(ccp(90,-52));
																																										  select_menu->addChild(right_item);
																																										  
																																										  select_menu->setEnabled(true);
																																										  
																																										  is_menu_enable = true;
																																									  }));
																														   }));
																				});
																			});
																		});
																	});
																});
																
																
															});
														});
													});
												});
											});
										});
									});
								});
							});
						});
						
						
					});
				});
			});
		});
	});
}

void RivalSelectPopup::selectRival(int t_index)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	select_menu->setEnabled(false);
	select_menu->removeAllChildren();
	
	mySGD->endless_nick = mySGD->dummy_list[t_index]["nick"].asString();
	mySGD->endless_flag = mySGD->dummy_list[t_index]["flag"].asString();
	mySGD->endless_victory = mySGD->dummy_list[t_index]["victory"].asInt();
	mySGD->endless_autoLevel = mySGD->dummy_list[t_index]["autoLevel"].asInt();
	mySGD->endless_level = mySGD->dummy_list[t_index]["level"].asInt();
	mySGD->endless_character_no = mySGD->dummy_list[t_index].get("charNo", 1).asInt();
	
	CommonAnimation::closePopup(this, m_container, gray, [=](){
		
	}, [=](){
		selected_func(); removeFromParent();
	});
}


