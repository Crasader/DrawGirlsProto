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
	
	back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_case->setContentSize(CCSizeMake(350,180));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	setFormSetter(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-10, back_case->getContentSize().height-46));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-17));
	back_case->addChild(back_in);
	
	setFormSetter(back_in);
	
	
	title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rivalSearch), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(0,back_case->getContentSize().height/2.f-25));
	m_container->addChild(title_label);
	
	select_menu = CCMenuLambda::create();
	select_menu->setPosition(ccp(back_in->getContentSize().width/2.f, back_in->getContentSize().height/2.f));
	back_in->addChild(select_menu);
	select_menu->setTouchPriority(touch_priority);
	select_menu->setEnabled(false);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width-25, back_case->getContentSize().height-25));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  select_menu->setEnabled(false);
								  select_menu->removeAllChildren();
								  
								  addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
									  addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
								  
								  addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
																	   {
																		   gray->setOpacity(t);
																		   KS::setOpacity(m_container, t);
																	   }, [=](int t)
																	   {
																		   gray->setOpacity(0);
																		   KS::setOpacity(m_container, 0); cancel_func(); removeFromParent();
																	   }));
							  });
	back_case->addChild(close_button);
	
	left_case = CCScale9Sprite::create("mainpopup_pupple4.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_case->setContentSize(CCSizeMake(100, back_in->getContentSize().height-20));
	left_case->setPosition(ccp(back_in->getContentSize().width/2.f-110, back_in->getContentSize().height/2.f));
	back_in->addChild(left_case);
	
	setFormSetter(left_case);
	
	left_node = CCNode::create();
	left_node->setPosition(ccp(left_case->getContentSize().width/2.f, left_case->getContentSize().height/2.f));
	left_case->addChild(left_node);
	
	{
		CCSprite* t_question = CCSprite::create("startsetting_question.png");
		t_question->setPosition(CCPointZero);
		left_node->addChild(t_question);
	}
	
	
	center_case = CCScale9Sprite::create("mainpopup_pupple4.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	center_case->setContentSize(CCSizeMake(100, back_in->getContentSize().height-20));
	center_case->setPosition(ccp(back_in->getContentSize().width/2.f, back_in->getContentSize().height/2.f));
	back_in->addChild(center_case);
	
	setFormSetter(center_case);
	
	center_node = CCNode::create();
	center_node->setPosition(ccp(center_case->getContentSize().width/2.f, center_case->getContentSize().height/2.f));
	center_case->addChild(center_node);
	
	{
		CCSprite* t_question = CCSprite::create("startsetting_question.png");
		t_question->setPosition(CCPointZero);
		center_node->addChild(t_question);
	}
	
	right_case = CCScale9Sprite::create("mainpopup_pupple4.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	right_case->setContentSize(CCSizeMake(100, back_in->getContentSize().height-20));
	right_case->setPosition(ccp(back_in->getContentSize().width/2.f+110, back_in->getContentSize().height/2.f));
	back_in->addChild(right_case);
	
	setFormSetter(right_case);
	
	right_node = CCNode::create();
	right_node->setPosition(ccp(right_case->getContentSize().width/2.f, right_case->getContentSize().height/2.f));
	right_case->addChild(right_node);
	
	{
		CCSprite* t_question = CCSprite::create("startsetting_question.png");
		t_question->setPosition(CCPointZero);
		right_node->addChild(t_question);
	}
	
	m_container->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t)
										 {
											 gray->setOpacity(t);
											 KS::setOpacity(m_container, t);
										 }, [=](int t)
										 {
											 gray->setOpacity(255);
											 KS::setOpacity(m_container, 255);
											 startRivalAnimation();
//											 is_menu_enable = true;
										 }));
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
								left_flag->setScale(1.2f);
								left_flag->setPosition(ccp(0, 30));
								left_node->addChild(left_flag);
								
								KSLabelTTF* left_nick = KSLabelTTF::create(mySGD->dummy_list[0]["nick"].asString().c_str(), mySGD->getFont().c_str(), 14);
								left_nick->setPosition(ccp(0, 5));
								left_node->addChild(left_nick);
								
								KSLabelTTF* left_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rivalLevel), mySGD->dummy_list[0]["level"].asInt())->getCString(), mySGD->getFont().c_str(), 12);
								left_level->setPosition(ccp(0, -10));
								left_node->addChild(left_level);
								
								KSLabelTTF* left_ing_win = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rivalIngWin), mySGD->dummy_list[0]["victory"].asInt())->getCString(), mySGD->getFont().c_str(), 15);
								left_ing_win->setColor(ccc3(255,170,20));
								left_ing_win->setPosition(ccp(0, -30));
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
													center_flag->setScale(1.2f);
													center_flag->setPosition(ccp(0, 30));
													center_node->addChild(center_flag);
													
													KSLabelTTF* center_nick = KSLabelTTF::create(mySGD->dummy_list[1]["nick"].asString().c_str(), mySGD->getFont().c_str(), 14);
													center_nick->setPosition(ccp(0, 5));
													center_node->addChild(center_nick);
													
													KSLabelTTF* center_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rivalLevel), mySGD->dummy_list[1]["level"].asInt())->getCString(), mySGD->getFont().c_str(), 12);
													center_level->setPosition(ccp(0, -10));
													center_node->addChild(center_level);
													
													KSLabelTTF* center_ing_win = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rivalIngWin), mySGD->dummy_list[1]["victory"].asInt())->getCString(), mySGD->getFont().c_str(), 15);
													center_ing_win->setColor(ccc3(255,170,20));
													center_ing_win->setPosition(ccp(0, -30));
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
																		right_flag->setScale(1.2f);
																		right_flag->setPosition(ccp(0, 30));
																		right_node->addChild(right_flag);
																		
																		KSLabelTTF* right_nick = KSLabelTTF::create(mySGD->dummy_list[2]["nick"].asString().c_str(), mySGD->getFont().c_str(), 14);
																		right_nick->setPosition(ccp(0, 5));
																		right_node->addChild(right_nick);
																		
																		KSLabelTTF* right_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rivalLevel), mySGD->dummy_list[2]["level"].asInt())->getCString(), mySGD->getFont().c_str(), 12);
																		right_level->setPosition(ccp(0, -10));
																		right_node->addChild(right_level);
																		
																		KSLabelTTF* right_ing_win = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rivalIngWin), mySGD->dummy_list[2]["victory"].asInt())->getCString(), mySGD->getFont().c_str(), 15);
																		right_ing_win->setColor(ccc3(255,170,20));
																		right_ing_win->setPosition(ccp(0, -30));
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
																															   title_label->setString(myLoc->getLocalForKey(kMyLocalKey_rivalSelect));
																															   addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
																																									  {
																																										  title_label->setOpacity(t*255);
																																									  }, [=](float t)
																																									  {
																																										  title_label->setOpacity(255);
																																										  CCSprite* n_left = CCSprite::create("whitePaper.png", CCRectMake(0, 0, left_case->getContentSize().width-10, left_case->getContentSize().height-10));
																																										  n_left->setOpacity(0);
																																										  CCSprite* s_left = CCSprite::create("whitePaper.png", CCRectMake(0, 0, left_case->getContentSize().width-10, left_case->getContentSize().height-10));
																																										  s_left->setOpacity(0);
																																										  
																																										  CCMenuItemLambda* left_item = CCMenuItemSpriteLambda::create(n_left, s_left, [=](CCObject* sender){selectRival(0);});
																																										  left_item->setPosition(ccp(-110,0));
																																										  select_menu->addChild(left_item);
																																										  
																																										  
																																										  CCSprite* n_center = CCSprite::create("whitePaper.png", CCRectMake(0, 0, center_case->getContentSize().width-10, center_case->getContentSize().height-10));
																																										  n_center->setOpacity(0);
																																										  CCSprite* s_center = CCSprite::create("whitePaper.png", CCRectMake(0, 0, center_case->getContentSize().width-10, center_case->getContentSize().height-10));
																																										  s_center->setOpacity(0);
																																										  
																																										  CCMenuItemLambda* center_item = CCMenuItemSpriteLambda::create(n_center, s_center, [=](CCObject* sender){selectRival(1);});
																																										  center_item->setPosition(ccp(0,0));
																																										  select_menu->addChild(center_item);
																																										  
																																										  
																																										  CCSprite* n_right = CCSprite::create("whitePaper.png", CCRectMake(0, 0, right_case->getContentSize().width-10, right_case->getContentSize().height-10));
																																										  n_right->setOpacity(0);
																																										  CCSprite* s_right = CCSprite::create("whitePaper.png", CCRectMake(0, 0, right_case->getContentSize().width-10, right_case->getContentSize().height-10));
																																										  s_right->setOpacity(0);
																																										  
																																										  CCMenuItemLambda* right_item = CCMenuItemSpriteLambda::create(n_right, s_right, [=](CCObject* sender){selectRival(2);});
																																										  right_item->setPosition(ccp(110,0));
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
	select_menu->setEnabled(false);
	select_menu->removeAllChildren();
	
	mySGD->endless_nick = mySGD->dummy_list[t_index]["nick"].asString();
	mySGD->endless_flag = mySGD->dummy_list[t_index]["flag"].asString();
	mySGD->endless_victory = mySGD->dummy_list[t_index]["victory"].asInt();
	mySGD->endless_autoLevel = mySGD->dummy_list[t_index]["autoLevel"].asInt();
	mySGD->endless_level = mySGD->dummy_list[t_index]["level"].asInt();
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
										 {
											 gray->setOpacity(t);
											 KS::setOpacity(m_container, t);
										 }, [=](int t)
										 {
											 gray->setOpacity(0);
											 KS::setOpacity(m_container, 0); selected_func(); removeFromParent();
										 }));
}

