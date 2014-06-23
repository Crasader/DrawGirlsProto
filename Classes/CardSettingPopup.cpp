//
//  CardSettingPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 20..
//
//

#include "CardSettingPopup.h"
#include "MyLocalization.h"
#include "StageImgLoader.h"
//#include "CollectionBook.h"
#include "CollectionBookPopup.h"
#include "PuzzleMapScene.h"
#include "MainFlowScene.h"
#include "CardCase.h"
#include "GameData.h"
#include "DataStorageHub.h"
#include "CardListViewer.h"
#include "StageSettingPopup.h"
#include "CardStrengthPopup.h"
#include "TutorialFlowStep.h"
#include "TouchSuctionLayer.h"
#include "CommonButton.h"
#include "KSLabelTTF.h"
#include "ScrollBar.h"
#include "CommonAnimation.h"
#include "CCMenuLambda.h"

void CardSettingPopup::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool CardSettingPopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
//	if(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber) > 10000)
//	{
//		server_puzzle_list_count = NSDS_GI(kSDS_GI_eventListCount_i);
//		
//		server_puzzle_list_no.push_back(0);
//		server_puzzle_stage_count.push_back(0);
//		server_puzzle_start_stage.push_back(0);
//		for(int i=1;i<=server_puzzle_list_count;i++)
//		{
//			server_puzzle_list_no.push_back(NSDS_GI(kSDS_GI_eventList_int1_no_i, i));
//			server_puzzle_stage_count.push_back(NSDS_GI(server_puzzle_list_no[i], kSDS_PZ_stageCount_i));
//			server_puzzle_start_stage.push_back(NSDS_GI(server_puzzle_list_no[i], kSDS_PZ_startStage_i));
//		}
//	}
//	else
//	{
		server_puzzle_list_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
	
		default_align_number_of_cell = 0;
		default_cell_info.clear();
		special_card_list.clear();
		
		int not_event_puzzle_count = 0;
		for(int i=1;i<=server_puzzle_list_count;i++)
		{
			int t_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i);
			if(!NSDS_GB(t_puzzle_number, kSDS_PZ_isEvent_b))
			{
				not_event_puzzle_count++;
				server_puzzle_list_no.push_back(t_puzzle_number);
				int t_stage_count = NSDS_GI(t_puzzle_number, kSDS_PZ_stageCount_i);
				int t_start_stage = NSDS_GI(t_puzzle_number, kSDS_PZ_startStage_i);
				server_puzzle_stage_count.push_back(t_stage_count);
				server_puzzle_start_stage.push_back(t_start_stage);
				
				int cell_count = ((t_stage_count+1)+1)/2 + 1; // inner +1 : puzzle card
				
				DefaultCardCellInfo t_info;
				t_info.m_type = DefaultCardCellType::kTitle;
				t_info.puzzle_number = t_puzzle_number;
				default_cell_info.push_back(t_info);
				
				int ing_stage_count = 0;
				for(int j=0;j<cell_count-1;j++)
				{
					DefaultCardCellInfo t_info;
					t_info.m_type = DefaultCardCellType::kStage;
					t_info.puzzle_number = t_puzzle_number;
					
					if(ing_stage_count < t_stage_count)
					{
						t_info.first_cell_in_cell_type = t_start_stage + ing_stage_count;
						ing_stage_count++;
					}
					else if(ing_stage_count == t_stage_count)
					{
						t_info.first_cell_in_cell_type = DefaultCardCellInCellType::kPuzzle;
						ing_stage_count++;
					}
					else
					{
						t_info.first_cell_in_cell_type = DefaultCardCellInCellType::kEmpty;
					}
					
					if(ing_stage_count < t_stage_count)
					{
						t_info.second_cell_in_cell_type = t_start_stage + ing_stage_count;
						ing_stage_count++;
					}
					else if(ing_stage_count == t_stage_count)
					{
						t_info.second_cell_in_cell_type = DefaultCardCellInCellType::kPuzzle;
						ing_stage_count++;
					}
					else
					{
						t_info.second_cell_in_cell_type = DefaultCardCellInCellType::kEmpty;
					}
					
					default_cell_info.push_back(t_info);
				}
				
				default_align_number_of_cell += cell_count;
			}
		}
	
	for(int i=0;i<mySGD->getHasGottenCardsSize();i++)
	{
		int t_card_number = mySGD->getHasGottenCardsDataCardNumber(i);
		string t_card_type = NSDS_GS(kSDS_CI_int1_type_s, t_card_number);
		if(t_card_type == "")
			t_card_type = "normal";
		
		if(t_card_type != "normal" && t_card_type != "nPuzzle")
		{
			special_card_list.push_back(mySGD->getHasGottenCardData(i));
		}
	}
	
		server_puzzle_list_count = not_event_puzzle_count;
	
	for(int i=0;i<mySGD->getHasGottenCardsSize();i++)
	{
		CardSortInfo t_info = mySGD->getHasGottenCardData(i);
		string card_type = NSDS_GS(kSDS_CI_int1_type_s, t_info.card_number.getV());
		if(card_type == "")
			card_type = "normal";
		
		if(card_type == "normal")
		{
			not_default_card_list.push_back(t_info);
		}
	}
//	}
	is_special_align_reverse = false;
	
	recent_sort_type = myDSH->getIntegerForKey(kDSH_Key_cardSortType);
	
	changeSortType(CardSortType(recent_sort_type));
	
	recent_selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	
	is_menu_enable = false;
	
	setTouchEnabled(true);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kCSS_Z_gray);
	
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kCSS_Z_back);
	
	title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_mycard), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setAnchorPoint(ccp(0,0.5f));
	title_label->setPosition(ccp(25,256));
	main_case->addChild(title_label);
	
	CCScale9Sprite* main_inner = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_inner->setContentSize(CCSizeMake(465, 200));
	main_inner->setPosition(main_case->getContentSize().width/2.f, 140);
	main_case->addChild(main_inner);
	
	
//	int puzzle_count = server_puzzle_list_count;
//	int total_stage_cnt = 0;
//	
//	if(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber) > 10000)
//	{
//		for(int i=1;i<=puzzle_count;i++)
//		{
//			int puzzle_number = NSDS_GI(kSDS_GI_eventList_int1_no_i, i);
//			int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
//			
//			total_stage_cnt += stage_count;
//		}
//	}
//	else
//	{
//		for(int i=1;i<=puzzle_count;i++)
//		{
//			int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i);
//			int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
//			
//			total_stage_cnt += stage_count;
//		}
//	}
	
	is_normal_table = true;
	
	CCSize table_size = CCSizeMake(457, 175);
	CCPoint table_position = ccp(4, 13);
	
//	CCSprite* temp_table = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_table->setAnchorPoint(CCPointZero);
//	temp_table->setOpacity(100);
//	temp_table->setPosition(table_position);
//	main_inner->addChild(temp_table);
	
	
	card_table = CCTableView::create(this, table_size);
	card_table->setAnchorPoint(CCPointZero);
	card_table->setDirection(kCCScrollViewDirectionVertical);
	card_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	card_table->setPosition(table_position);
	card_table->setDelegate(this);
	main_inner->addChild(card_table);
	card_table->setTouchPriority(-180-5);
	
	TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-184);
	t_suction->setNotSwallowRect(CCRectMake(table_position.x, table_position.y, table_size.width, table_size.height));
	t_suction->setTouchEnabled(true);
	addChild(t_suction);
	
	
	CommonButton* close_menu = CommonButton::createCloseButton(-200);
	close_menu->setPosition(getContentPosition(kCSS_MT_close));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								t_node->setTag(kCSS_MT_close);
								menuAction(t_node);
							});
	main_case->addChild(close_menu, kCSS_Z_content);
	
	
//	CommonButton* strength_menu = CommonButton::create("카드강화", 12, CCSizeMake(100,40), CommonButtonYellow, -185);
//	strength_menu->setTitleColor(ccBLACK);
//	strength_menu->setPosition(ccp(78,31));
//	main_case->addChild(strength_menu, kCSS_Z_content);
//	strength_menu->setFunction([=](CCObject* sender)
//							   {
//								   CCNode* t_node = CCNode::create();
//								   t_node->setTag(kCSS_MT_strength);
//								   menuAction(t_node);
//							   });
	
	string button_label_string;
	if(recent_sort_type == kCST_default)
		button_label_string = myLoc->getLocalForKey(kMyLocalKey_defaultSort);
	else if(recent_sort_type == kCST_gradeDown)
		button_label_string = string(myLoc->getLocalForKey(kMyLocalKey_gradeOrder)) + "▲";
	else if(recent_sort_type == kCST_gradeUp)
		button_label_string = string(myLoc->getLocalForKey(kMyLocalKey_gradeOrder)) + "▼";
	else if(recent_sort_type == kCST_take)
		button_label_string = string(myLoc->getLocalForKey(kMyLocalKey_takeOrder)) + "▲";
	else if(recent_sort_type == kCST_takeReverse)
		button_label_string = string(myLoc->getLocalForKey(kMyLocalKey_takeOrder)) + "▼";
	
	align_default_menu = CommonButton::create(button_label_string.c_str(), 12, CCSizeMake(70,37), CommonButtonYellowUp, -185);
	align_default_menu->setTitleColor(ccc3(50, 20, 0));
	align_default_menu->setPosition(ccp(400,256));
	main_case->addChild(align_default_menu, kCSS_Z_alignButton);
	
	function<void(CCObject*)> align_func = [=](CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		TouchSuctionLayer* tt_suction = TouchSuctionLayer::create(-200);
		tt_suction->setTouchEnabled(true);
		addChild(tt_suction);
		
		float t_cell_height = 30;
		
		CCSprite* t_stencil = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 80, t_cell_height*4 + 6));
		t_stencil->setAnchorPoint(ccp(0.5f,1.f));
		t_stencil->setPosition(ccp(0,-7));
		
		CCClippingNode* t_clipping = CCClippingNode::create(t_stencil);
		t_clipping->setPosition(ccp(400,256));
		main_case->addChild(t_clipping, kCSS_Z_alignList);
		
		CCScale9Sprite* t_case = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
		t_case->setContentSize(CCSizeMake(67, t_cell_height*4 + 6));
		t_case->setAnchorPoint(ccp(0.5f,0));
		t_case->setPosition(ccp(0,0));
		t_clipping->addChild(t_case);
		
		for(int i=1;i<=4-1;i++)
		{
			CCSprite* t_line = CCSprite::create("cardsetting_line.png");
			t_line->setScaleX(55/t_line->getContentSize().width);
			t_line->setPosition(ccp(t_case->getContentSize().width/2.f, 3+t_cell_height*i));
			t_case->addChild(t_line);
		}
		
		CCMenuLambda* t_menu = CCMenuLambda::create();
		t_menu->setPosition(ccp(t_case->getContentSize().width/2.f,t_case->getContentSize().height));
		t_case->addChild(t_menu);
		t_menu->setTouchPriority(-201);
		
		vector<string> title_list;
		title_list.clear();
		
		vector<function<void()>> lambda_list;
		lambda_list.clear();
		
		int t_recent_sort_type = myDSH->getIntegerForKey(kDSH_Key_cardSortType);
		if(t_recent_sort_type != kCST_default)
		{
			title_list.push_back(myLoc->getLocalForKey(kMyLocalKey_defaultSort));
			lambda_list.push_back([=]()
								  {
									  tt_suction->is_on_touch_began_func = false;
									  tt_suction->touch_began_func = [](){};
									  
									  align_default_menu->setTitle(myLoc->getLocalForKey(kMyLocalKey_defaultSort));
									  this->changeSortType(kCST_default);
									  card_table->reloadData();
									  
									  addChild(KSGradualValue<float>::create(1.f, 0.f, 0.3f, [=](float t)
																			 {
																				 t_case->setPosition(ccp(0,(-t_cell_height*4-6-7)*t));
																			 }, [=](float t)
																			 {
																				 t_case->setPosition(ccp(0,(-t_cell_height*4-6-7)*t));
																				 tt_suction->removeFromParent();
																				 is_menu_enable = true;
																			 }));
									  t_menu->setTouchEnabled(false);
									  t_menu->removeFromParent();
								  });
		}
		
		if(t_recent_sort_type != kCST_gradeDown)
		{
			title_list.push_back(string(myLoc->getLocalForKey(kMyLocalKey_gradeOrder)) + "▲");
			lambda_list.push_back([=]()
								  {
									  tt_suction->is_on_touch_began_func = false;
									  tt_suction->touch_began_func = [](){};
									  
									  align_default_menu->setTitle(string(myLoc->getLocalForKey(kMyLocalKey_gradeOrder)) + "▲");
									  this->changeSortType(kCST_gradeDown);
									  card_table->reloadData();
									  
									  addChild(KSGradualValue<float>::create(1.f, 0.f, 0.3f, [=](float t)
																			 {
																				 t_case->setPosition(ccp(0,(-t_cell_height*4-6-7)*t));
																			 }, [=](float t)
																			 {
																				 t_case->setPosition(ccp(0,(-t_cell_height*4-6-7)*t));
																				 tt_suction->removeFromParent();
																				 is_menu_enable = true;
																			 }));
									  t_menu->setTouchEnabled(false);
									  t_menu->removeFromParent();
								  });
		}
		
		if(t_recent_sort_type != kCST_gradeUp)
		{
			title_list.push_back(string(myLoc->getLocalForKey(kMyLocalKey_gradeOrder)) + "▼");
			lambda_list.push_back([=]()
								  {
									  tt_suction->is_on_touch_began_func = false;
									  tt_suction->touch_began_func = [](){};
									  
									  align_default_menu->setTitle(string(myLoc->getLocalForKey(kMyLocalKey_gradeOrder)) + "▼");
									  this->changeSortType(kCST_gradeUp);
									  card_table->reloadData();
									  
									  addChild(KSGradualValue<float>::create(1.f, 0.f, 0.3f, [=](float t)
																			 {
																				 t_case->setPosition(ccp(0,(-t_cell_height*4-6-7)*t));
																			 }, [=](float t)
																			 {
																				 t_case->setPosition(ccp(0,(-t_cell_height*4-6-7)*t));
																				 tt_suction->removeFromParent();
																				 is_menu_enable = true;
																			 }));
									  t_menu->setTouchEnabled(false);
									  t_menu->removeFromParent();
								  });
		}
		
		if(t_recent_sort_type != kCST_take)
		{
			title_list.push_back(string(myLoc->getLocalForKey(kMyLocalKey_takeOrder)) + "▲");
			lambda_list.push_back([=]()
								  {
									  tt_suction->is_on_touch_began_func = false;
									  tt_suction->touch_began_func = [](){};
									  
									  align_default_menu->setTitle(string(myLoc->getLocalForKey(kMyLocalKey_takeOrder)) + "▲");
									  this->changeSortType(kCST_take);
									  card_table->reloadData();
									  
									  addChild(KSGradualValue<float>::create(1.f, 0.f, 0.3f, [=](float t)
																			 {
																				 t_case->setPosition(ccp(0,(-t_cell_height*4-6-7)*t));
																			 }, [=](float t)
																			 {
																				 t_case->setPosition(ccp(0,(-t_cell_height*4-6-7)*t));
																				 tt_suction->removeFromParent();
																				 is_menu_enable = true;
																			 }));
									  t_menu->setTouchEnabled(false);
									  t_menu->removeFromParent();
								  });
		}
		
		if(t_recent_sort_type != kCST_takeReverse)
		{
			title_list.push_back(string(myLoc->getLocalForKey(kMyLocalKey_takeOrder)) + "▼");
			lambda_list.push_back([=]()
								  {
									  tt_suction->is_on_touch_began_func = false;
									  tt_suction->touch_began_func = [](){};
									  
									  align_default_menu->setTitle(string(myLoc->getLocalForKey(kMyLocalKey_takeOrder)) + "▼");
									  this->changeSortType(kCST_takeReverse);
									  card_table->reloadData();
									  
									  addChild(KSGradualValue<float>::create(1.f, 0.f, 0.3f, [=](float t)
																			 {
																				 t_case->setPosition(ccp(0,(-t_cell_height*4-6-7)*t));
																			 }, [=](float t)
																			 {
																				 t_case->setPosition(ccp(0,(-t_cell_height*4-6-7)*t));
																				 tt_suction->removeFromParent();
																				 is_menu_enable = true;
																			 }));
									  t_menu->setTouchEnabled(false);
									  t_menu->removeFromParent();
								  });
		}
		
		for(int i=0;i<title_list.size();i++)
		{
			KSLabelTTF* t_label = KSLabelTTF::create(title_list[i].c_str(), mySGD->getFont().c_str(), 10);
			t_label->setPosition(ccp(t_case->getContentSize().width/2.f,t_case->getContentSize().height) + ccp(0,-3-t_cell_height/2.f - i*t_cell_height));
			t_case->addChild(t_label);
			
			CCSprite* n_sprite = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 67, t_cell_height));
			CCSprite* s_sprite = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 67, t_cell_height));
			
			CCMenuItemSpriteLambda* t_item = CCMenuItemSpriteLambda::create(n_sprite, s_sprite, [=](CCObject* sender)
																			{
																				lambda_list[i]();
																			});
			t_item->setPosition(ccp(0,-3-t_cell_height/2.f - i*t_cell_height));
			t_menu->addChild(t_item);
		}
		
		t_menu->setEnabled(false);
		addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
											   {
												   t_case->setPosition(ccp(0,(-t_cell_height*4-6-7)*t));
											   }, [=](float t)
											   {
												   t_case->setPosition(ccp(0,(-t_cell_height*4-6-7)*t));
												   
												   tt_suction->touch_began_func = [=]()
												   {
													   tt_suction->is_on_touch_began_func = false;
													   tt_suction->touch_began_func = [](){};
													   addChild(KSGradualValue<float>::create(1.f, 0.f, 0.3f, [=](float t)
																							  {
																								  t_case->setPosition(ccp(0,(-t_cell_height*4-6-7)*t));
																							  }, [=](float t)
																							  {
																								  t_case->setPosition(ccp(0,(-t_cell_height*4-6-7)*t));
																								  tt_suction->removeFromParent();
																								  is_menu_enable = true;
																							  }));
												   };
												   tt_suction->is_on_touch_began_func = true;
												   t_menu->setEnabled(true);
											   }));
		
		
		
		//									 CCNode* t_node = CCNode::create();
		//									 t_node->setTag(kCSS_MT_alignDefault);
		//									 menuAction(t_node);
	};
	
	align_default_menu->setFunction(align_func);
	align_default_menu->setBackgroundTypeForDisabled(CommonButtonYellowDown);
	
	
//	string sign_str;
//
//	if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) == kCST_take)
//		sign_str = "▼";
//	else
//		sign_str = "▲";
//
//	align_take_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_takeOrder) + sign_str, 12, CCSizeMake(65,37), CommonButtonYellowUp, -185);
//	align_take_menu->setTitleColor(ccc3(50, 20, 0));
//	align_take_menu->setPosition(ccp(280,256));
//	main_case->addChild(align_take_menu, kCSS_Z_alignButton);
//	align_take_menu->setFunction([=](CCObject* sender)
//									{
//										CCNode* t_node = CCNode::create();
//										t_node->setTag(kCSS_MT_alignTake);
//										menuAction(t_node);
//									});
//	align_take_menu->setBackgroundTypeForDisabled(CommonButtonYellowDown);
//
//
//	if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) == kCST_gradeUp)
//		sign_str = "▼";
//	else
//		sign_str = "▲";
//
//	align_rank_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_gradeOrder) + sign_str, 12, CCSizeMake(65,37), CommonButtonYellowUp, -185);
//	align_rank_menu->setTitleColor(ccc3(50, 20, 0));
//	align_rank_menu->setPosition(ccp(340,256));
//	main_case->addChild(align_rank_menu, kCSS_Z_alignButton);
//	align_rank_menu->setFunction([=](CCObject* sender)
//								 {
//									 CCNode* t_node = CCNode::create();
//									 t_node->setTag(kCSS_MT_alignRank);
//									 menuAction(t_node);
//								 });
//	align_rank_menu->setBackgroundTypeForDisabled(CommonButtonYellowDown);
	
	
//	KSLabelTTF* take_card_ment = KSLabelTTF::create("획득한 카드 수", mySGD->getFont().c_str(), 12);
//	take_card_ment->setPosition(ccp(110,255));
//	main_case->addChild(take_card_ment, kCSS_Z_content);
	
//	int open_stage_card_count = 0;
	int all_stage_card_count = 0;
	for(int i=0;i<server_puzzle_list_count;i++)
	{
		int t_start_stage = server_puzzle_start_stage[i];
		int t_stage_count = server_puzzle_stage_count[i];
		
//		bool is_puzzle_open = myDSH->getBoolForKey(kDSH_Key_openPuzzleCnt)+1 >= i;
		
		for(int j=t_start_stage;j<t_start_stage+t_stage_count;j++)
		{
			int t_stage_card_count = 4;
			
//			int condition_gold = NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, j);
//			bool is_on_condition_gold = condition_gold == 0;
//			
//			int condition_stage = NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, j);
//			bool is_on_condition_stage = (condition_stage == 0) || (mySGD->isClearPiece(condition_stage));
//			
//			if(is_puzzle_open && ((is_on_condition_gold && is_on_condition_stage) || mySGD->isClearPiece(j))) // open stage
//				open_stage_card_count += t_stage_card_count;
			all_stage_card_count += t_stage_card_count;
		}
	}
	
	take_count_back = CCScale9Sprite::create("common_time.png", CCRectMake(0, 0, 22, 22), CCRectMake(10, 10, 2, 2));
	take_count_back->setContentSize(CCSizeMake(90, 22));
	take_count_back->setPosition(ccp(112,256));
	main_case->addChild(take_count_back, kCSS_Z_content);
	
	
	KSLabelTTF* total_card_count = KSLabelTTF::create(ccsf("/%d", all_stage_card_count), mySGD->getFont().c_str(), 10);
	total_card_count->setAnchorPoint(ccp(0.5f,0));
	take_count_back->addChild(total_card_count);
	
	KSLabelTTF* take_card_count = KSLabelTTF::create(ccsf("%d", mySGD->getHasGottenCardsSize()), mySGD->getFont().c_str(), 13);
	take_card_count->setColor(ccc3(255, 170, 20));
	take_card_count->setAnchorPoint(ccp(0.5f,0.f));
	take_count_back->addChild(take_card_count);
	
	total_card_count->setPosition(ccp(take_count_back->getContentSize().width/2.f + take_card_count->getContentSize().width/2.f, 4));
	take_card_count->setPosition(ccp(take_count_back->getContentSize().width/2.f - total_card_count->getContentSize().width/2.f, 4));
	
	
	title_content = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_cardSettingMyCardContent), mySGD->getFont().c_str(), 12);
	title_content->setAnchorPoint(ccp(0,0.5f));
	title_content->setPosition(ccp(160,256));
	main_case->addChild(title_content, kCSS_Z_content);
	
	
	if(mySGD->getHasGottenCardsSize() > 0)
	{
		CCSprite* n_diary = CCSprite::create("cardsetting_diary.png");
		CCSprite* s_diary = CCSprite::create("cardsetting_diary.png");
		s_diary->setColor(ccGRAY);
		
		CCMenuItem* diary_item = CCMenuItemSprite::create(n_diary, s_diary, this, menu_selector(CardSettingPopup::menuAction));
		diary_item->setTag(kCSS_MT_diary);
		
		CCMenu* diary_menu = CCMenu::createWithItem(diary_item);
		diary_menu->setPosition(ccp(340, 256));
		main_case->addChild(diary_menu, kCSS_Z_content);
		diary_menu->setTouchPriority(-185);
	}
	
	CCMenuLambda* change_mode_menu = CCMenuLambda::create();
	change_mode_menu->setPosition(ccp(main_case->getContentSize().width/2.f, 24.5f));
	main_case->addChild(change_mode_menu, kCSS_Z_content);
	change_mode_menu->setTouchPriority(-185);
	
	CCSprite* n_gold_button_img = CCSprite::create("cardsetting_goldbutton.png");
	
	n_special_show_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_cardSettingMyCardToSpecial), mySGD->getFont().c_str(), 13);
	n_special_show_label->enableOuterStroke(ccBLACK, 1.f);
	n_special_show_label->setPosition(ccpFromSize(n_gold_button_img->getContentSize()/2.f));
	n_gold_button_img->addChild(n_special_show_label);
	
	CCSprite* s_gold_button_img = CCSprite::create("cardsetting_goldbutton.png");
	s_gold_button_img->setColor(ccGRAY);
	
	s_special_show_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_cardSettingMyCardToSpecial), mySGD->getFont().c_str(), 13);
	s_special_show_label->enableOuterStroke(ccBLACK, 1.f);
	s_special_show_label->setPosition(ccpFromSize(n_gold_button_img->getContentSize()/2.f));
	s_gold_button_img->addChild(s_special_show_label);
	
	CCMenuItemSpriteLambda* gold_button_item = CCMenuItemSpriteLambda::create(n_gold_button_img, s_gold_button_img, [=](CCObject* sender)
																			  {
																				  if(!is_menu_enable)
																					  return;
																				  
																				  is_menu_enable = false;
																				  
																				  if(is_normal_table)
																				  {
																					  title_label->setString(myLoc->getLocalForKey(kMyLocalKey_cardSettingSpecialCardTitle));
																					  take_count_back->setVisible(false);
																					  title_content->setString(myLoc->getLocalForKey(kMyLocalKey_cardSettingSpecialCardContent));
																					  title_content->setPositionX(100);
																					  
																					  n_special_show_label->setString(myLoc->getLocalForKey(kMyLocalKey_cardSettingSpecialCardToMyCard));
																					  s_special_show_label->setString(myLoc->getLocalForKey(kMyLocalKey_cardSettingSpecialCardToMyCard));
																					  
																					  string t_button_label_string;
																					  if(is_special_align_reverse)
																						  t_button_label_string = string(myLoc->getLocalForKey(kMyLocalKey_takeOrder)) + "▼";
																					  else
																						  t_button_label_string = string(myLoc->getLocalForKey(kMyLocalKey_takeOrder)) + "▲";
																					  
																					  align_default_menu->setTitle(t_button_label_string.c_str());
																					  align_default_menu->setFunction([=](CCObject* sender)
																													  {
																														  if(!is_menu_enable)
																															  return;
																														  
																														  is_menu_enable = false;
																														  
																														  this->specialChangeSortType(!is_special_align_reverse);
																														  card_table->reloadData();
																														  
																														  is_menu_enable = true;
																													  });
																					  
																					  is_normal_table = false;
																					  card_table->reloadData();
																					  
																					  is_menu_enable = true;
																				  }
																				  else
																				  {
																					  title_label->setString(myLoc->getLocalForKey(kMyLocalKey_mycard));
																					  take_count_back->setVisible(true);
																					  title_content->setString(myLoc->getLocalForKey(kMyLocalKey_cardSettingMyCardContent));
																					  title_content->setPositionX(160);
																					  
																					  n_special_show_label->setString(myLoc->getLocalForKey(kMyLocalKey_cardSettingMyCardToSpecial));
																					  s_special_show_label->setString(myLoc->getLocalForKey(kMyLocalKey_cardSettingMyCardToSpecial));
																					  
																					  
																					  string t_button_label_string;
																					  if(recent_sort_type == kCST_default)
																						  t_button_label_string = myLoc->getLocalForKey(kMyLocalKey_defaultSort);
																					  else if(recent_sort_type == kCST_gradeDown)
																						  t_button_label_string = string(myLoc->getLocalForKey(kMyLocalKey_gradeOrder)) + "▲";
																					  else if(recent_sort_type == kCST_gradeUp)
																						  t_button_label_string = string(myLoc->getLocalForKey(kMyLocalKey_gradeOrder)) + "▼";
																					  else if(recent_sort_type == kCST_take)
																						  t_button_label_string = string(myLoc->getLocalForKey(kMyLocalKey_takeOrder)) + "▲";
																					  else if(recent_sort_type == kCST_takeReverse)
																						  t_button_label_string = string(myLoc->getLocalForKey(kMyLocalKey_takeOrder)) + "▼";
																					  
																					  align_default_menu->setTitle(t_button_label_string.c_str());
																					  align_default_menu->setFunction(align_func);
																					  
																					  is_normal_table = true;
																					  card_table->reloadData();
																					  
																					  is_menu_enable = true;
																				  }
																			  });
	change_mode_menu->addChild(gold_button_item);
	
	
	
//	CommonButton* event_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_event), 12, CCSizeMake(65,37), CommonButtonLightPupple, -185);
//	event_menu->setPosition(ccp(400,256));
//	main_case->addChild(event_menu, kCSS_Z_alignButton);
//	event_menu->setFunction([=](CCObject* sender)
//								 {
//									 CCNode* t_node = CCNode::create();
//									 t_node->setTag(kCSS_MT_event);
//									 menuAction(t_node);
//								 });
	
	
//	CCSprite* n_tip = CCSprite::create("mainflow_tip.png");
//	CCSprite* s_tip = CCSprite::create("mainflow_tip.png");
//	s_tip->setColor(ccGRAY);
//	
//	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(CardSettingPopup::menuAction));
//	tip_item->setTag(kCSS_MT_tip);
//	
//	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
//	tip_menu->setPosition(ccp(465,(myDSH->puzzle_ui_top-320.f)/2.f+320.f-3 -13));
//	addChild(tip_menu, kCSS_Z_content);
//	
//	tip_menu->setTouchPriority(-185);
	
	
    return true;
}

void CardSettingPopup::specialChangeSortType(bool is_reverse)
{
	is_special_align_reverse = is_reverse;
	
	if(!is_special_align_reverse)
	{
		align_default_menu->setTitle((string(myLoc->getLocalForKey(kMyLocalKey_takeOrder)) + "▼").c_str());
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number.getV() > b.take_number.getV();
			}
		} pred;
		
		sort(special_card_list.begin(), special_card_list.end(), pred);
	}
	else
	{
		align_default_menu->setTitle((string(myLoc->getLocalForKey(kMyLocalKey_takeOrder)) + "▲").c_str());
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number.getV() < b.take_number.getV();
			}
		} pred;
		
		sort(special_card_list.begin(), special_card_list.end(), pred);
	}
}

void CardSettingPopup::changeSortType( CardSortType t_type )
{
	recent_sort_type = t_type;
	myDSH->setIntegerForKey(kDSH_Key_cardSortType, t_type);
	mySGD->changeSortType(t_type);
	
	if(t_type == kCST_default)
	{
		struct t_CardSortDefault{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.card_number.getV() < b.card_number.getV();
			}
		} pred;
		
		sort(not_default_card_list.begin(), not_default_card_list.end(), pred);
	}
	else if(t_type == kCST_take)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number.getV() > b.take_number.getV();
			}
		} pred;
		
		sort(not_default_card_list.begin(), not_default_card_list.end(), pred);
	}
	else if(t_type == kCST_takeReverse)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number.getV() < b.take_number.getV();
			}
		} pred;
		
		sort(not_default_card_list.begin(), not_default_card_list.end(), pred);
	}
	else if(t_type == kCST_gradeUp) // rank
	{
		struct t_CardSortGradeUp{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank.getV() > b.rank.getV();
			}
		} pred;
		
		sort(not_default_card_list.begin(), not_default_card_list.end(), pred);
	}
	else if(t_type == kCST_gradeDown) // rank
	{
		struct t_CardSortGradeDown{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank.getV() < b.rank.getV();
			}
		} pred;
		
		sort(not_default_card_list.begin(), not_default_card_list.end(), pred);
	}
}

void CardSettingPopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
}

void CardSettingPopup::showPopup()
{
	CommonAnimation::openPopup(this, main_case, gray, [=](){
		
	}, bind(&CardSettingPopup::endShowPopup, this));
//	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
//	gray->runAction(gray_fade);
//	
//	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-14.f));
//	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(CardSettingPopup::endShowPopup));
//	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
//	main_case->runAction(main_seq);
}

void CardSettingPopup::endShowPopup()
{
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_upgradeClick)
	{
		TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
		t_tutorial->initStep(kTutorialFlowStep_upgradeClick);
		addChild(t_tutorial, kCSS_Z_popup);
		
		tutorial_node = t_tutorial;
		
		card_table->setTouchEnabled(false);
	}
	else
	{
//		if(!myDSH->getBoolForKey(kDSH_Key_was_opened_tutorial_dimed_cardsetting))
//		{
//			myDSH->setBoolForKey(kDSH_Key_was_opened_tutorial_dimed_cardsetting, true);
//			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
//			t_suction->target_touch_began = t_suction;
//			t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
//			t_suction->setTouchEnabled(true);
//			
//			CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_cardsetting.png");
//			dimed_tip->setPosition(ccp(240,160));
//			t_suction->addChild(dimed_tip);
//			
//			addChild(t_suction, kCSS_Z_popup);
//		}
	}
	
	is_menu_enable = true;
}

void CardSettingPopup::hidePopup()
{
	is_menu_enable = false;
		
	
	CommonAnimation::closePopup(this, main_case, gray, [=](){
		
	}, bind(&CardSettingPopup::endHidePopup, this));
}

void CardSettingPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

CCPoint CardSettingPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kCSS_MT_close)			return_value = ccp(452,256);
	else if(t_tag == kCSS_MT_cardBase)	return_value = ccp(245,210);
	
	return return_value;
}

void CardSettingPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_upgradeClick)
	{
		int tag = ((CCNode*)pSender)->getTag();
		
		if(tag == kCSS_MT_strength)
		{
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_upgradeScript);
			is_menu_enable = false;
			CardStrengthPopup* t_popup = CardStrengthPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kMainFlowZorder_popup);
			
			target_final = NULL;
			hidePopup();
		}
	}
	else
	{
		is_menu_enable = false;
		int tag = ((CCNode*)pSender)->getTag();
		
		if(tag == kCSS_MT_close)
		{
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_cardsInfo);
			save_userdata_list.push_back(kSaveUserData_Key_selectedCard);
			
			myDSH->saveUserData(save_userdata_list, nullptr);
			
			
			if(mySGD->before_cardsetting == kSceneCode_PuzzleMapScene)
				hidePopup();
			else if(mySGD->before_cardsetting == kSceneCode_StageSetting)
			{
				StageSettingPopup* t_popup = StageSettingPopup::create();
				t_popup->setHideFinalAction(target_final, delegate_final);
				getParent()->addChild(t_popup, kMainFlowZorder_popup);
				
				target_final = NULL;
				hidePopup();
			}
		}
		else if(tag == kCSS_MT_alignDefault)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_default)
			{
				changeSortType(kCST_default);
				alignChange();
			}
			
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_alignRank)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_gradeDown)
			{
				changeSortType(kCST_gradeDown);
				alignChange();
			}
			else
			{
				changeSortType(kCST_gradeUp);
				alignChange();
			}
			
			string sign_str;
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) == kCST_gradeUp)
				sign_str = "▼";
			else
				sign_str = "▲";
			
			align_rank_menu->setTitle(myLoc->getLocalForKey(kMyLocalKey_gradeOrder) + sign_str);
			
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_alignTake)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_take)
			{
				changeSortType(kCST_take);
				alignChange();
			}
			else
			{
				changeSortType(kCST_takeReverse);
				alignChange();
			}
			
			string sign_str;
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) == kCST_take)
				sign_str = "▼";
			else
				sign_str = "▲";
			
			align_take_menu->setTitle(myLoc->getLocalForKey(kMyLocalKey_takeOrder) + sign_str);
			
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_diary)
		{
			mySGD->selected_collectionbook = mySGD->getHasGottenCardsDataCardNumber(mySGD->getHasGottenCardsSize()-1);
			
			CollectionBookPopup* t_popup = CollectionBookPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kMainFlowZorder_popup);
			
			target_final = NULL;
			hidePopup();
		}
		else if(tag == kCSS_MT_event)
		{
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_strength)
		{
			mySGD->setStrengthTargetCardNumber(myDSH->getIntegerForKey(kDSH_Key_selectedCard));
			mySGD->setCardStrengthBefore(kCardStrengthBefore_cardSetting);
			CardStrengthPopup* t_popup = CardStrengthPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kMainFlowZorder_popup);
			
			target_final = NULL;
			hidePopup();
		}
		else if(tag == kCSS_MT_tip)
		{
			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
			t_suction->target_touch_began = t_suction;
			t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
			t_suction->setTouchEnabled(true);
			
			CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_cardsetting.png");
			dimed_tip->setPosition(ccp(240,160));
			t_suction->addChild(dimed_tip);
			
			addChild(t_suction, kCSS_Z_popup);
			
			is_menu_enable = true;
		}
		else if(tag >= kCSS_MT_cardMenuBase && tag < kCSS_MT_noCardBase)
		{
			int clicked_card_number = tag-kCSS_MT_cardMenuBase;
			
			if(clicked_card_number != 0)
			{
				mySGD->selected_collectionbook = clicked_card_number;
				
				CollectionBookPopup* t_popup = CollectionBookPopup::create();
				t_popup->setHideFinalAction(target_final, delegate_final);
				getParent()->addChild(t_popup, kMainFlowZorder_popup);
				
				target_final = NULL;
				hidePopup();
				
				//			CCDirector::sharedDirector()->replaceScene(CollectionBook::scene());
			}
			else
			{
				is_menu_enable = true;
			}
		}
		else if(tag >= kCSS_MT_noCardBase)
		{
			is_menu_enable = true;
		}
	}
}


void CardSettingPopup::alignChange()
{
	card_table->reloadData();
}

void CardSettingPopup::cellAction( CCObject* sender )
{
	
}

void CardSettingPopup::addDefaultAlignCard(int t_card_number, CCPoint t_position, CCNode* t_node, string t_ment)
{
	if(mySGD->isHasGottenCards(t_card_number))
	{
		GraySprite* n_card_img = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																										  t_card_number)->getCString()));
		if(!n_card_img)
		{
			n_card_img = GraySprite::create("cardsetting_blank.png");
		}
		n_card_img->setScale(0.6f);
		
		string card_type = NSDS_GS(kSDS_CI_int1_type_s, t_card_number);
		if(card_type == "")
			card_type = "normal";
		
		CCSprite* n_case_img = CCSprite::create(CCString::createWithFormat("cardsetting_minicase_%s.png", card_type.c_str())->getCString());
		n_case_img->setPosition(ccp(n_card_img->getContentSize().width/2.f, n_card_img->getContentSize().height/2.f));
		n_card_img->addChild(n_case_img);
		
		CCSprite* n_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, n_card_img->getContentSize().width*n_card_img->getScale(), n_card_img->getContentSize().height*n_card_img->getScale()));
		n_node->setOpacity(0);
		n_card_img->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
		n_node->addChild(n_card_img);
		
		
		GraySprite* s_card_img = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																										  t_card_number)->getCString()));
		if(!s_card_img)
		{
			s_card_img = GraySprite::create("cardsetting_blank.png");
		}
		s_card_img->setScale(0.6f);
		
		CCSprite* s_case_img = CCSprite::create(CCString::createWithFormat("cardsetting_minicase_%s.png", card_type.c_str())->getCString());
		s_case_img->setPosition(ccp(s_card_img->getContentSize().width/2.f, s_card_img->getContentSize().height/2.f));
		s_card_img->addChild(s_case_img);
		s_card_img->setColor(ccGRAY);
		
		CCSprite* s_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, s_card_img->getContentSize().width*s_card_img->getScale(), s_card_img->getContentSize().height*s_card_img->getScale()));
		s_node->setOpacity(0);
		s_card_img->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
		s_node->addChild(s_card_img);
		
		CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
		t_card_item->setTag(kCSS_MT_cardMenuBase+t_card_number);
		
		ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
		t_card_menu->setPosition(t_position);
		t_node->addChild(t_card_menu);
		t_card_menu->setTouchPriority(-180-3);
	}
	else
	{
		CCSprite* n_card_img = CCSprite::create("cardsetting_blank.png");
		n_card_img->setScale(0.6f);
		
		string condition_text = t_ment;
		
		KSLabelTTF* n_condition = KSLabelTTF::create(condition_text.c_str(), mySGD->getFont().c_str(), 14);
		n_condition->enableOuterStroke(ccBLACK, 1.f);
		n_condition->setPosition(ccp(n_card_img->getContentSize().width/2.f, n_card_img->getContentSize().height/2.f));
		n_card_img->addChild(n_condition);
		
		string card_type = NSDS_GS(kSDS_CI_int1_type_s, t_card_number);
		if(card_type == "")
			card_type = "normal";
		
		CCSprite* n_case_img = CCSprite::create(CCString::createWithFormat("cardsetting_minicase_%s.png", card_type.c_str())->getCString());
		n_case_img->setPosition(ccp(n_card_img->getContentSize().width/2.f, n_card_img->getContentSize().height/2.f));
		n_card_img->addChild(n_case_img);
		
		CCSprite* n_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, n_card_img->getContentSize().width*n_card_img->getScale(), n_card_img->getContentSize().height*n_card_img->getScale()));
		n_node->setOpacity(0);
		n_card_img->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
		n_node->addChild(n_card_img);
		
		
		CCSprite* s_card_img = CCSprite::create("cardsetting_blank.png");
		s_card_img->setScale(0.6f);
		
		KSLabelTTF* s_condition = KSLabelTTF::create(condition_text.c_str(), mySGD->getFont().c_str(), 14);
		s_condition->enableOuterStroke(ccBLACK, 1.f);
		s_condition->setPosition(ccp(s_card_img->getContentSize().width/2.f, s_card_img->getContentSize().height/2.f));
		s_card_img->addChild(s_condition);
		
		CCSprite* s_case_img = CCSprite::create(CCString::createWithFormat("cardsetting_minicase_%s.png", card_type.c_str())->getCString());
		s_case_img->setPosition(ccp(s_card_img->getContentSize().width/2.f, s_card_img->getContentSize().height/2.f));
		s_card_img->addChild(s_case_img);
		s_card_img->setColor(ccGRAY);
		
		CCSprite* s_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, s_card_img->getContentSize().width*s_card_img->getScale(), s_card_img->getContentSize().height*s_card_img->getScale()));
		s_node->setOpacity(0);
		s_card_img->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
		s_node->addChild(s_card_img);
		
		CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
		t_card_item->setTag(kCSS_MT_noCardBase+t_card_number);
		
		ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
		t_card_menu->setPosition(t_position);
		t_node->addChild(t_card_menu);
		t_card_menu->setTouchPriority(-180-3);
	}
}

CCTableViewCell* CardSettingPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	if(is_normal_table)
	{
		CardSortType sort_type = (CardSortType)recent_sort_type;
		
		if(sort_type == kCST_default)
		{
			DefaultCardCellInfo t_cell_info = default_cell_info[idx];
			
			if(t_cell_info.m_type == DefaultCardCellType::kTitle)
			{
				CCSprite* title_back = CCSprite::create("cardsetting_title.png");
				title_back->setPosition(ccp(70,tableCellSizeForIndex(table, idx).height/2.f));
				cell->addChild(title_back);
				
				KSLabelTTF* title_label = KSLabelTTF::create(NSDS_GS(t_cell_info.puzzle_number, kSDS_PZ_title_s).c_str(), mySGD->getFont().c_str(), 12);
				title_label->enableOuterStroke(ccBLACK, 1.f);
				title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,1));
				title_back->addChild(title_label);
				
				if(idx != 0)
				{
					title_back->setPosition(ccp(70,tableCellSizeForIndex(table, idx).height/2.f - 8));
					
					CCSprite* line_img = CCSprite::create("cardsetting_line.png");
					line_img->setScaleX(450/line_img->getContentSize().width);
					line_img->setPosition(ccpFromSize(tableCellSizeForIndex(table, idx)/2.f) + ccp(0,8));
					cell->addChild(line_img);
				}
			}
			else if(t_cell_info.m_type == DefaultCardCellType::kStage)
			{
				CCPoint distance_position = ccp(45,0);
				CCPoint base_position = ccp(23, cellSizeForTable(table).height/2.f);
				if(t_cell_info.first_cell_in_cell_type == DefaultCardCellInCellType::kPuzzle)
				{
					// puzzle
					
					KSLabelTTF* number_label = KSLabelTTF::create(ccsf("%d", t_cell_info.puzzle_number), mySGD->getFont().c_str(), 13);
					number_label->enableOuterStroke(ccBLACK, 1.f);
					number_label->setColor(ccc3(255, 170, 20));
					number_label->setPosition(base_position + ccp(0, 8));
					cell->addChild(number_label);
					
					KSLabelTTF* unit_label = KSLabelTTF::create("PUZZLE", mySGD->getFont().c_str(), 12);
					unit_label->enableOuterStroke(ccBLACK, 1.f);
					unit_label->setPosition(base_position + ccp(0,-7));
					cell->addChild(unit_label);
					
					addDefaultAlignCard(NSDS_GI(t_cell_info.puzzle_number, kSDS_PZ_clearCard_i), base_position + distance_position, cell, myLoc->getLocalForKey(kMyLocalKey_cardSettingClearCardMent));
					addDefaultAlignCard(NSDS_GI(t_cell_info.puzzle_number, kSDS_PZ_perfectCard_i), base_position + distance_position*2, cell, myLoc->getLocalForKey(kMyLocalKey_cardSettingPerfectCardMent));
				}
				else if(t_cell_info.first_cell_in_cell_type != DefaultCardCellInCellType::kEmpty)
				{
					// stage
					
					KSLabelTTF* number_label = KSLabelTTF::create(ccsf("%d", t_cell_info.first_cell_in_cell_type), mySGD->getFont().c_str(), 13);
					number_label->enableOuterStroke(ccBLACK, 1.f);
					number_label->setColor(ccc3(255, 170, 20));
					number_label->setPosition(base_position + ccp(0, 8));
					cell->addChild(number_label);
					
					KSLabelTTF* unit_label = KSLabelTTF::create("STAGE", mySGD->getFont().c_str(), 12);
					unit_label->enableOuterStroke(ccBLACK, 1.f);
					unit_label->setPosition(base_position + ccp(0,-7));
					cell->addChild(unit_label);
					
					int stage_card_count = 4;
					
					for(int i=1;i<=stage_card_count;i++)
					{
						string t_condition_text;
						if(i == 1)
							t_condition_text = myLoc->getLocalForKey(kMyLocalKey_conditionTwoLine1);
						else if(i == 2)
							t_condition_text = myLoc->getLocalForKey(kMyLocalKey_conditionTwoLine2);
						else if(i == 3)
							t_condition_text = myLoc->getLocalForKey(kMyLocalKey_conditionTwoLine3);
						else
							t_condition_text = myLoc->getLocalForKey(kMyLocalKey_conditionTwoLine4);
						
						addDefaultAlignCard(NSDS_GI(t_cell_info.first_cell_in_cell_type, kSDS_SI_level_int1_card_i, i), base_position + distance_position*i, cell, t_condition_text);
					}
				}
				
				base_position = ccpFromSize(cellSizeForTable(table)/2.f) + ccp(23,0);
				if(t_cell_info.second_cell_in_cell_type == DefaultCardCellInCellType::kPuzzle)
				{
					// puzzle
					
					KSLabelTTF* number_label = KSLabelTTF::create(ccsf("%d", t_cell_info.puzzle_number), mySGD->getFont().c_str(), 13);
					number_label->enableOuterStroke(ccBLACK, 1.f);
					number_label->setColor(ccc3(255, 170, 20));
					number_label->setPosition(base_position + ccp(0, 8));
					cell->addChild(number_label);
					
					KSLabelTTF* unit_label = KSLabelTTF::create("PUZZLE", mySGD->getFont().c_str(), 12);
					unit_label->enableOuterStroke(ccBLACK, 1.f);
					unit_label->setPosition(base_position + ccp(0,-7));
					cell->addChild(unit_label);
					
					addDefaultAlignCard(NSDS_GI(t_cell_info.puzzle_number, kSDS_PZ_clearCard_i), base_position + distance_position, cell, myLoc->getLocalForKey(kMyLocalKey_cardSettingClearCardMent));
					addDefaultAlignCard(NSDS_GI(t_cell_info.puzzle_number, kSDS_PZ_perfectCard_i), base_position + distance_position*2, cell, myLoc->getLocalForKey(kMyLocalKey_cardSettingPerfectCardMent));
				}
				else if(t_cell_info.second_cell_in_cell_type != DefaultCardCellInCellType::kEmpty)
				{
					// stage
					
					KSLabelTTF* number_label = KSLabelTTF::create(ccsf("%d", t_cell_info.second_cell_in_cell_type), mySGD->getFont().c_str(), 13);
					number_label->enableOuterStroke(ccBLACK, 1.f);
					number_label->setColor(ccc3(255, 170, 20));
					number_label->setPosition(base_position + ccp(0, 8));
					cell->addChild(number_label);
					
					KSLabelTTF* unit_label = KSLabelTTF::create("STAGE", mySGD->getFont().c_str(), 12);
					unit_label->enableOuterStroke(ccBLACK, 1.f);
					unit_label->setPosition(base_position + ccp(0,-7));
					cell->addChild(unit_label);
					
					int stage_card_count = 4;
					
					for(int i=1;i<=stage_card_count;i++)
					{
						string t_condition_text;
						if(i == 1)
							t_condition_text = myLoc->getLocalForKey(kMyLocalKey_conditionTwoLine1);
						else if(i == 2)
							t_condition_text = myLoc->getLocalForKey(kMyLocalKey_conditionTwoLine2);
						else if(i == 3)
							t_condition_text = myLoc->getLocalForKey(kMyLocalKey_conditionTwoLine3);
						else
							t_condition_text = myLoc->getLocalForKey(kMyLocalKey_conditionTwoLine4);
						
						addDefaultAlignCard(NSDS_GI(t_cell_info.second_cell_in_cell_type, kSDS_SI_level_int1_card_i, i), base_position + distance_position*i, cell, t_condition_text);
					}
				}
			}
		}
		else
		{
			for(int i=idx*9;i<idx*9+9 && i<not_default_card_list.size();i++)
			{
				int card_number = not_default_card_list[i].card_number.getV();
				CCPoint card_position = ccp(30.f + (i-idx*9)*(48.f), cellSizeForTable(table).height/2.f);
				
				string card_type = NSDS_GS(kSDS_CI_int1_type_s, card_number);
				if(card_type == "")
					card_type = "normal";
				
				GraySprite* n_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																											  card_number)->getCString()));
				n_card->setScale(0.6f);
				CCSprite* n_case = CCSprite::create(CCString::createWithFormat("cardsetting_minicase_%s.png", card_type.c_str())->getCString());
				n_case->setPosition(ccp(n_card->getContentSize().width/2.f, n_card->getContentSize().height/2.f));
				n_card->addChild(n_case);
				
				CCSprite* n_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, n_card->getContentSize().width*n_card->getScale(), n_card->getContentSize().height*n_card->getScale()));
				n_node->setOpacity(0);
				n_card->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
				n_node->addChild(n_card);
				
				GraySprite* s_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																											  card_number)->getCString()));
				s_card->setScale(0.6f);
				s_card->setColor(ccGRAY);
				CCSprite* s_case = CCSprite::create(CCString::createWithFormat("cardsetting_minicase_%s.png", card_type.c_str())->getCString());
				s_case->setPosition(ccp(s_card->getContentSize().width/2.f, s_card->getContentSize().height/2.f));
				s_card->addChild(s_case);
				
				CCSprite* s_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, s_card->getContentSize().width*s_card->getScale(), s_card->getContentSize().height*s_card->getScale()));
				s_node->setOpacity(0);
				s_card->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
				s_node->addChild(s_card);
				
				CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
				t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
				
				ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
				t_card_menu->setPosition(card_position);
				cell->addChild(t_card_menu);
				t_card_menu->setTouchPriority(-180-3);
				
				if(mySGD->isCardMorphing(card_number))
				{
					CCSprite* morphing_mark = KS::loadCCBI<CCSprite*>(this, "morphing_card.ccbi").first;
					morphing_mark->setPosition(card_position + ccp(n_card->getContentSize().width/2.f*n_card->getScale()-10, -n_card->getContentSize().height/2.f*n_card->getScale()+10));
					cell->addChild(morphing_mark);
				}
				
				if(recent_selected_card_number == card_number)
				{
					CCSprite* select_img = CCSprite::create("card_check.png");
					select_img->setPosition(card_position);
					cell->addChild(select_img);
				}
			}
		}
	}
	else
	{
		for(int i=idx*5;i<idx*5+5 && i<special_card_list.size();i++)
		{
			int card_number = special_card_list[i].card_number.getV();
			CCPoint card_position = ccp(45.f + (i-idx*5)*(60.f), cellSizeForTable(table).height/2.f);
			
			string card_type = NSDS_GS(kSDS_CI_int1_type_s, card_number);
			if(card_type == "")
				card_type = "normal";
			
			GraySprite* n_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																										  card_number)->getCString()));
//			n_card->setScale(0.6f);
			CCSprite* n_case = CCSprite::create(CCString::createWithFormat("cardsetting_minicase_%s.png", card_type.c_str())->getCString());
			n_case->setPosition(ccp(n_card->getContentSize().width/2.f, n_card->getContentSize().height/2.f));
			n_card->addChild(n_case);
			
			CCSprite* n_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, n_card->getContentSize().width*n_card->getScale(), n_card->getContentSize().height*n_card->getScale()));
			n_node->setOpacity(0);
			n_card->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
			n_node->addChild(n_card);
			
			GraySprite* s_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																										  card_number)->getCString()));
//			s_card->setScale(0.6f);
			s_card->setColor(ccGRAY);
			CCSprite* s_case = CCSprite::create(CCString::createWithFormat("cardsetting_minicase_%s.png", card_type.c_str())->getCString());
			s_case->setPosition(ccp(s_card->getContentSize().width/2.f, s_card->getContentSize().height/2.f));
			s_card->addChild(s_case);
			
			CCSprite* s_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, s_card->getContentSize().width*s_card->getScale(), s_card->getContentSize().height*s_card->getScale()));
			s_node->setOpacity(0);
			s_card->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
			s_node->addChild(s_card);
			
			CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
			t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
			
			ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
			t_card_menu->setPosition(card_position);
			cell->addChild(t_card_menu);
			t_card_menu->setTouchPriority(-180-3);
			
			if(mySGD->isCardMorphing(card_number))
			{
				CCSprite* morphing_mark = KS::loadCCBI<CCSprite*>(this, "morphing_card.ccbi").first;
				morphing_mark->setPosition(card_position + ccp(n_card->getContentSize().width/2.f*n_card->getScale()-10, -n_card->getContentSize().height/2.f*n_card->getScale()+10));
				cell->addChild(morphing_mark);
			}
			
			if(recent_selected_card_number == card_number)
			{
				CCSprite* select_img = CCSprite::create("card_check.png");
				select_img->setPosition(card_position);
				cell->addChild(select_img);
			}
		}
	}
	
	return cell;
}

void CardSettingPopup::scrollViewDidScroll( CCScrollView* view )
{
	if(m_scrollBar)
	{
		m_scrollBar->setBarRefresh();
	}
}

void CardSettingPopup::scrollViewDidZoom( CCScrollView* view )
{
	
}

void CardSettingPopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	
}

CCSize CardSettingPopup::cellSizeForTable( CCTableView *table )
{
	if(is_normal_table)
		return CCSizeMake(457, 60);
	else
		return CCSizeMake(457, 100);
}

CCSize CardSettingPopup::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	if(is_normal_table)
	{
		CardSortType sort_type = (CardSortType)recent_sort_type;
		
		if(sort_type == kCST_default)
		{
			DefaultCardCellInfo t_cell_info = default_cell_info[idx];
			
			if(t_cell_info.m_type == DefaultCardCellType::kTitle)
			{
				if(idx == 0)
				{
					return CCSizeMake(457, 25);
				}
				else
				{
					return CCSizeMake(457, 40);
				}
			}
			else
				return cellSizeForTable(table);
		}
		else
			return cellSizeForTable(table);
	}
	else
		return cellSizeForTable(table);
}

unsigned int CardSettingPopup::numberOfCellsInTableView( CCTableView *table )
{
	CardSortType sort_type = CardSortType(recent_sort_type);
	
	if(is_normal_table)
	{
		if(sort_type == kCST_default)
		{
			return default_align_number_of_cell;
		}
		else
		{
			int have_card_count = not_default_card_list.size();
			if(have_card_count == 0)
				return 0;
			else
				return (have_card_count-1)/9+1;
		}
	}
	else
	{
		if(special_card_list.empty())
			return 0;
		else
			return (special_card_list.size()-1)/5+1;
	}
}

bool CardSettingPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	CCLOG("touch swallow!!");
	
	return true;
}
void CardSettingPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardSettingPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardSettingPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardSettingPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
