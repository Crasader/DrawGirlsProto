//
//  PuzzleScene.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 17..
//
//

#include "PuzzleScene.h"
#include "DataStorageHub.h"
#include "CountingBMLabel.h"
#include "StarGoldData.h"
#include "HeartTime.h"
#include "ShopPopup.h"
#include "ServerDataSave.h"
#include "MainFlowScene.h"
#include "StageImgLoader.h"
#include "UnknownFriends.h"
#include "GDWebSprite.h"
#include "CumberShowWindow.h"

CCScene* PuzzleScene::scene()
{
    CCScene *scene = CCScene::create();
    
    PuzzleScene *layer = PuzzleScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    scene->addChild(layer);
	
    return scene;
}

enum PuzzleZorder{
	kPuzzleZorder_back = 1,
	kPuzzleZorder_puzzle,
	kPuzzleZorder_top,
	kPuzzleZorder_right,
	kPuzzleZorder_popup
};

bool PuzzleScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	friend_point_popup = NULL;
	start_menu = NULL;
	challenge_menu = NULL;
	rank_table = NULL;
	
	CCSprite* back_img = CCSprite::create("mainflow_back_wall.png");
	back_img->setPosition(ccp(240,160));
	addChild(back_img, kPuzzleZorder_back);
	
	setPuzzle();
	
	setTop();
	setRight();
	
	is_menu_enable = true;
	
	return true;
}

void PuzzleScene::setPuzzle()
{
	
}

enum PuzzleMenuTag{
	kPuzzleMenuTag_cancel = 0,
	kPuzzleMenuTag_rubyShop,
	kPuzzleMenuTag_goldShop,
	kPuzzleMenuTag_heartShop,
	kPuzzleMenuTag_friendPointContent,
	kPuzzleMenuTag_friendPointClose,
	kPuzzleMenuTag_start,
	kPuzzleMenuTag_challenge,
	kPuzzleMenuTag_rightReward,
	kPuzzleMenuTag_rightRank,
	kPuzzleMenuTag_rightMonster
};

void PuzzleScene::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kPuzzleMenuTag_cancel)
	{
		CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
	}
	else if(tag == kPuzzleMenuTag_rubyShop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
		t_shop->targetHeartTime(heart_time);
		t_shop->setShopCode(kSC_ruby);
		t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
		addChild(t_shop, kPuzzleZorder_popup);
	}
	else if(tag == kPuzzleMenuTag_goldShop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
		t_shop->targetHeartTime(heart_time);
		t_shop->setShopCode(kSC_gold);
		t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
		addChild(t_shop, kPuzzleZorder_popup);
	}
	else if(tag == kPuzzleMenuTag_heartShop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
		t_shop->targetHeartTime(heart_time);
		t_shop->setShopCode(kSC_heart);
		t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
		addChild(t_shop, kPuzzleZorder_popup);
	}
	else if(tag == kPuzzleMenuTag_friendPointContent)
	{
		if(!friend_point_popup)
		{
			CCNode* menu_node = ((CCNode*)sender)->getParent();
			CCNode* top_node = menu_node->getParent();
			friend_point_popup = CCSprite::create("candy_popup.png");
			friend_point_popup->setAnchorPoint(ccp(0.5,1.f));
			friend_point_popup->setPosition(ccp(410,menu_node->getPositionY() + friend_point_popup->getContentSize().height));
			top_node->addChild(friend_point_popup, -1);
			
			CCSprite* n_close = CCSprite::create("candy_popup_close.png");
			CCSprite* s_close = CCSprite::create("candy_popup_close.png");
			s_close->setColor(ccGRAY);
			
			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(PuzzleScene::menuAction));
			close_item->setTag(kPuzzleMenuTag_friendPointClose);
			
			CCMenu* close_menu = CCMenu::createWithItem(close_item);
			close_menu->setPosition(ccp(friend_point_popup->getContentSize().width/2.f, 25));
			friend_point_popup->addChild(close_menu);
			
			CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(410,menu_node->getPositionY()-12));
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleScene::popupClose));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
			friend_point_popup->runAction(t_seq);
		}
		else
			is_menu_enable = true;
	}
	else if(tag == kPuzzleMenuTag_friendPointClose)
	{
		CCNode* menu_node = ((CCNode*)sender)->getParent();
		CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(410,menu_node->getPositionY() + friend_point_popup->getContentSize().height));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleScene::closeFriendPointPopup));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
		friend_point_popup->runAction(t_seq);
	}
	else if(tag == kPuzzleMenuTag_rightReward)
	{
		recent_right_code = kPuzzleRightCode_reward;
		setRightHead();
		setRightContent();
		is_menu_enable = true;
	}
	else if(tag == kPuzzleMenuTag_rightRank)
	{
		recent_right_code = kPuzzleRightCode_rank;
		setRightHead();
		setRightContent();
		is_menu_enable = true;
	}
	else if(tag == kPuzzleMenuTag_rightMonster)
	{
		recent_right_code = kPuzzleRightCode_monster;
		setRightHead();
		setRightContent();
		is_menu_enable = true;
	}
	else if(tag == kPuzzleMenuTag_start)
	{
		
	}
	else if(tag == kPuzzleMenuTag_challenge)
	{
		
	}
}

void PuzzleScene::setRight()
{
	int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	selected_stage_number = myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number);
	if(selected_stage_number == 0)
	{
		selected_stage_number = NSDS_GI(puzzle_number, kSDS_PZ_startStage_i);
		myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, selected_stage_number);
	}
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	right_case = CCNode::create();
	right_case->setPosition(ccp(480*screen_scale_x,160));
	addChild(right_case, kPuzzleZorder_right);
	
	CCSprite* right_body = CCSprite::create("puzzle_right_body.png");
	right_body->setPosition(ccp(-right_body->getContentSize().width/2.f-5, -10));
	right_case->addChild(right_body);
	
	recent_right_code = kPuzzleRightCode_reward;
	
	right_head = CCNode::create();
	right_head->setPosition(ccp(-right_body->getContentSize().width/2.f-5, right_body->getContentSize().height/2.f+35.f/2.f-10));
	right_case->addChild(right_head);
	setRightHead();
	
	reward_node = CCNode::create();
	reward_node->setPosition(ccp(right_body->getContentSize().width/2.f, right_body->getContentSize().height/2.f));
	reward_node->setTag(0);
	right_body->addChild(reward_node);
	reward_node->setVisible(false);
	
	rank_node = CCNode::create();
	rank_node->setPosition(ccp(right_body->getContentSize().width/2.f, right_body->getContentSize().height/2.f));
	rank_node->setTag(0);
	right_body->addChild(rank_node);
	rank_node->setVisible(false);
	
	monster_node = CCNode::create();
	monster_node->setPosition(ccp(right_body->getContentSize().width/2.f, right_body->getContentSize().height/2.f));
	monster_node->setTag(0);
	right_body->addChild(monster_node);
	monster_node->setVisible(false);
	setRightContent();
}

void PuzzleScene::setRightHead()
{
	right_head->removeAllChildren();
	if(recent_right_code == kPuzzleRightCode_reward)
	{
		CCSprite* n_monster = CCSprite::create("puzzle_right_monster_off.png");
		CCSprite* s_monster = CCSprite::create("puzzle_right_monster_off.png");
		s_monster->setColor(ccGRAY);
		
		CCMenuItem* monster_item = CCMenuItemSprite::create(n_monster, s_monster, this, menu_selector(PuzzleScene::menuAction));
		monster_item->setTag(kPuzzleMenuTag_rightMonster);
		
		CCMenu* monster_menu = CCMenu::createWithItem(monster_item);
		monster_menu->setPosition(ccp(n_monster->getContentSize().width-4, 0));
		right_head->addChild(monster_menu);
		
		
		CCSprite* n_rank = CCSprite::create("puzzle_right_rank_off.png");
		CCSprite* s_rank = CCSprite::create("puzzle_right_rank_off.png");
		s_rank->setColor(ccGRAY);
		
		CCMenuItem* rank_item = CCMenuItemSprite::create(n_rank, s_rank, this, menu_selector(PuzzleScene::menuAction));
		rank_item->setTag(kPuzzleMenuTag_rightRank);
		
		CCMenu* rank_menu = CCMenu::createWithItem(rank_item);
		rank_menu->setPosition(CCPointZero);
		right_head->addChild(rank_menu);
		
		
		CCSprite* reward_img = CCSprite::create("puzzle_right_reward_on.png");
		reward_img->setPosition(ccp(-reward_img->getContentSize().width+4, 0));
		right_head->addChild(reward_img);
	}
	else if(recent_right_code == kPuzzleRightCode_rank)
	{
		CCSprite* n_reward = CCSprite::create("puzzle_right_reward_off.png");
		CCSprite* s_reward = CCSprite::create("puzzle_right_reward_off.png");
		s_reward->setColor(ccGRAY);
		
		CCMenuItem* reward_item = CCMenuItemSprite::create(n_reward, s_reward, this, menu_selector(PuzzleScene::menuAction));
		reward_item->setTag(kPuzzleMenuTag_rightReward);
		
		CCMenu* reward_menu = CCMenu::createWithItem(reward_item);
		reward_menu->setPosition(ccp(-n_reward->getContentSize().width+4, 0));
		right_head->addChild(reward_menu);
		
		
		CCSprite* n_monster = CCSprite::create("puzzle_right_monster_off.png");
		CCSprite* s_monster = CCSprite::create("puzzle_right_monster_off.png");
		s_monster->setColor(ccGRAY);
		
		CCMenuItem* monster_item = CCMenuItemSprite::create(n_monster, s_monster, this, menu_selector(PuzzleScene::menuAction));
		monster_item->setTag(kPuzzleMenuTag_rightMonster);
		
		CCMenu* monster_menu = CCMenu::createWithItem(monster_item);
		monster_menu->setPosition(ccp(n_monster->getContentSize().width-4, 0));
		right_head->addChild(monster_menu);
		
		
		CCSprite* rank_img = CCSprite::create("puzzle_right_rank_on.png");
		rank_img->setPosition(CCPointZero);
		right_head->addChild(rank_img);
	}
	else if(recent_right_code == kPuzzleRightCode_monster)
	{
		CCSprite* n_reward = CCSprite::create("puzzle_right_reward_off.png");
		CCSprite* s_reward = CCSprite::create("puzzle_right_reward_off.png");
		s_reward->setColor(ccGRAY);
		
		CCMenuItem* reward_item = CCMenuItemSprite::create(n_reward, s_reward, this, menu_selector(PuzzleScene::menuAction));
		reward_item->setTag(kPuzzleMenuTag_rightReward);
		
		CCMenu* reward_menu = CCMenu::createWithItem(reward_item);
		reward_menu->setPosition(ccp(-n_reward->getContentSize().width+4, 0));
		right_head->addChild(reward_menu);
		
		
		CCSprite* n_rank = CCSprite::create("puzzle_right_rank_off.png");
		CCSprite* s_rank = CCSprite::create("puzzle_right_rank_off.png");
		s_rank->setColor(ccGRAY);
		
		CCMenuItem* rank_item = CCMenuItemSprite::create(n_rank, s_rank, this, menu_selector(PuzzleScene::menuAction));
		rank_item->setTag(kPuzzleMenuTag_rightRank);
		
		CCMenu* rank_menu = CCMenu::createWithItem(rank_item);
		rank_menu->setPosition(CCPointZero);
		right_head->addChild(rank_menu);
		
		
		CCSprite* monster_img = CCSprite::create("puzzle_right_monster_on.png");
		monster_img->setPosition(ccp(monster_img->getContentSize().width-4, 0));
		right_head->addChild(monster_img);
	}
}

void PuzzleScene::setRightContent()
{
	if(recent_right_code == kPuzzleRightCode_reward)
	{
		setReward();
		
		reward_node->setVisible(true);
		rank_node->setVisible(false);
		monster_node->setVisible(false);
	}
	else if(recent_right_code == kPuzzleRightCode_rank)
	{
		setRank();
		
		reward_node->setVisible(false);
		rank_node->setVisible(true);
		monster_node->setVisible(false);
	}
	else if(recent_right_code == kPuzzleRightCode_monster)
	{
		setMonster();
		
		reward_node->setVisible(false);
		rank_node->setVisible(false);
		monster_node->setVisible(true);
	}
	if(start_menu)
		start_menu->setTouchEnabled(reward_node->isVisible());
	if(challenge_menu)
		challenge_menu->setTouchEnabled(rank_node->isVisible());
	if(rank_table)
		rank_table->setTouchEnabled(rank_node->isVisible());
	if(monster_start_menu)
		monster_start_menu->setTouchEnabled(monster_node->isVisible());
}

void PuzzleScene::setReward()
{
	if(reward_node->getTag() != selected_stage_number)
	{
		reward_node->removeAllChildren();
		
		CCSprite* reward_back = CCSprite::create("puzzle_reward_back.png");
		reward_back->setPosition(ccp(0,0));
		reward_node->addChild(reward_back);
		
		for(int i=0;i<3;i++)
		{
			int step_card_number = NSDS_GI(selected_stage_number, kSDS_SI_level_int1_card_i, i+1);
			
			CCSprite* step_card_img = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", selected_stage_number, i+1)->getCString()));
			step_card_img->setScale(0.73f*0.7f);
			step_card_img->setPosition(ccp(-10, 63-i*58));
			reward_node->addChild(step_card_img);
			
			CCSprite* no_img = CCSprite::create("cardsetting_noimg.png");
			no_img->setScale(0.8f*0.7f);
			no_img->setPosition(ccp(-10, 63-i*58));
			reward_node->addChild(no_img);
			
			CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
			mini_rank->setPosition(ccp(17,17));
			no_img->addChild(mini_rank);
			
			CCLabelTTF* step_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, step_card_number))->getCString(), mySGD->getFont().c_str(), 8);
			step_rank->setColor(ccBLACK);
			step_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f,mini_rank->getContentSize().height/2.f));
			mini_rank->addChild(step_rank);
			
			CCLabelTTF* step_reward = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_reward_i, step_card_number))->getCString(), mySGD->getFont().c_str(), 15);
			step_reward->setPosition(ccp(50,60-i*58));
			reward_node->addChild(step_reward);
			
			if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, step_card_number) > 0)
			{
				CCSprite* t_complete = CCSprite::create("stageinfo_complete.png");
				t_complete->setPosition(ccpAdd(step_reward->getPosition(), ccp(-15, 5)));
				reward_node->addChild(t_complete);
			}
		}
		
		CCSprite* n_start = CCSprite::create("puzzle_right_start.png");
		CCSprite* s_start = CCSprite::create("puzzle_right_start.png");
		s_start->setColor(ccGRAY);
		
		CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(PuzzleScene::menuAction));
		start_item->setTag(kPuzzleMenuTag_start);
		
		start_menu = CCMenu::createWithItem(start_item);
		start_menu->setPosition(ccp(0, -120));
		start_menu->setTouchEnabled(false);
		reward_node->addChild(start_menu);
		
		reward_node->setTag(selected_stage_number);
	}
}

void PuzzleScene::setRank()
{
	if(rank_node->getTag() != selected_stage_number)
	{
		rank_node->removeAllChildren();
		
		
		Json::Value p;
		Json::Value my_info = hspConnector::get()->myKakaoInfo;
		
		PuzzleRankFriendInfo t_my_info;
		t_my_info.nickname = my_info["nickname"].asString();
		t_my_info.img_url = my_info["profile_image_url"].asString();
		t_my_info.user_id = my_info["user_id"].asInt64();
		t_my_info.score = 0;
		t_my_info.is_play = false;
		t_my_info.is_message_blocked = my_info["message_blocked"].asBool();
		friend_list.push_back(t_my_info);
		
		p["memberIDList"].append(t_my_info.user_id);
		
		for(auto i : KnownFriends::getInstance()->getFriends())
		{
			PuzzleRankFriendInfo t_friend_info;
			t_friend_info.nickname = i.nick;
			t_friend_info.img_url = i.profileUrl;
			t_friend_info.user_id = i.userId;
			t_friend_info.score = 0;
			t_friend_info.is_play = false;
			t_friend_info.is_message_blocked = i.messageBlocked;
			friend_list.push_back(t_friend_info);
			
			p["memberIDList"].append(i.userId);
		}
		
		for(auto i : UnknownFriends::getInstance()->getFriends())
		{
			PuzzleRankFriendInfo fInfo;
			fInfo.nickname = i.nick + "[unknown]";
			fInfo.img_url = "";
			fInfo.user_id = i.userId;
			fInfo.score = 0;
			fInfo.is_play = false;
			fInfo.is_message_blocked = false;
			friend_list.push_back(fInfo);
			
			p["memberIDList"].append(i.userId);
		}
		
		p["stageNo"] = selected_stage_number;
		hspConnector::get()->command("getstagescorelist",p,json_selector(this, PuzzleScene::resultGetStageScoreList));
		
		
		
		CCSprite* n_challenge = CCSprite::create("puzzle_right_challenge.png");
		CCSprite* s_challenge = CCSprite::create("puzzle_right_challenge.png");
		s_challenge->setColor(ccGRAY);
		
		CCMenuItem* challenge_item = CCMenuItemSprite::create(n_challenge, s_challenge, this, menu_selector(PuzzleScene::menuAction));
		challenge_item->setTag(kPuzzleMenuTag_challenge);
		
		challenge_menu = CCMenu::createWithItem(challenge_item);
		challenge_menu->setPosition(ccp(0, -120));
		challenge_menu->setTouchEnabled(false);
		rank_node->addChild(challenge_menu);
		
		rank_node->setTag(selected_stage_number);
	}
}

void PuzzleScene::resultGetStageScoreList(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value score_list = result_data["list"];
		for(int i=0;i<score_list.size();i++)
		{
			vector<PuzzleRankFriendInfo>::iterator iter = find(friend_list.begin(), friend_list.end(), score_list[i]["memberID"].asInt64());
			if(iter != friend_list.end())
			{
				(*iter).score = score_list[i]["score"].asFloat();
				(*iter).is_play = true;
			}
			else
				CCLog("not found friend memberID : %s", score_list[i]["memberID"].asString().c_str());
		}
		
		struct t_FriendSort{
			bool operator() (const PuzzleRankFriendInfo& a, const PuzzleRankFriendInfo& b)
			{
				return a.score > b.score;
			}
		} pred;
		
		sort(friend_list.begin(), friend_list.end(), pred);
		
		for(int i=0;i<friend_list.size();i++)
			friend_list[i].rank = i+1;
		
		float my_score = 0.f;
		
		vector<PuzzleRankFriendInfo>::iterator iter = find(friend_list.begin(), friend_list.end(), hspConnector::get()->myKakaoInfo["user_id"].asInt64());
		if(iter != friend_list.end())
		{
			my_score = (*iter).score;
			friend_list.erase(iter);
		}
		else
			CCLog("not found my info");
		
		
		CCLabelTTF* my_score_title = CCLabelTTF::create("내 기록", mySGD->getFont().c_str(), 10);
		my_score_title->setPosition(ccp(0,85));
		rank_node->addChild(my_score_title);
		
		CCLabelTTF* my_score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", my_score)->getCString(), mySGD->getFont().c_str(), 15);
		my_score_label->setPosition(ccp(0,71));
		rank_node->addChild(my_score_label);
		
		
		selected_friend_idx = -1;
		
		CCSize table_size = CCSizeMake(110, 147);
		
		rank_table = CCTableView::create(this, table_size);
		rank_table->setAnchorPoint(CCPointZero);
		rank_table->setDirection(kCCScrollViewDirectionVertical);
		rank_table->setVerticalFillOrder(kCCTableViewFillTopDown);
		rank_table->setPosition(ccp(-table_size.width/2.f, -7-table_size.height/2.f));
		
		rank_table->setDelegate(this);
		rank_node->addChild(rank_table);
		rank_table->setTouchPriority(kCCMenuHandlerPriority+1);
		rank_table->setTouchEnabled(rank_node->isVisible());
	}
}

CCTableViewCell* PuzzleScene::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	PuzzleRankFriendInfo* member = &friend_list[idx];
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CCSprite* back_img = CCSprite::create("puzzle_right_rank_back.png");
	back_img->setPosition(CCPointZero);
	back_img->setAnchorPoint(CCPointZero);
	cell->addChild(back_img);
	
	CCSprite* profileImg = GDWebSprite::create((*member).img_url, "ending_noimg.png");
	profileImg->setAnchorPoint(ccp(0.5, 0.5));
	profileImg->setPosition(ccp(20, 19));
	profileImg->setScale(25.f / profileImg->getContentSize().width);
	cell->addChild(profileImg);
	
	CCLabelTTF* nickname_label = CCLabelTTF::create((*member).nickname.c_str(), mySGD->getFont().c_str(), 10);
	nickname_label->setPosition(ccp(38,20));
	nickname_label->setAnchorPoint(CCPointZero);
	cell->addChild(nickname_label);
	
	CCLabelTTF* score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", (*member).score)->getCString(), mySGD->getFont().c_str(), 15);
	score_label->setPosition(ccp(38,2));
	score_label->setAnchorPoint(CCPointZero);
	cell->addChild(score_label);
	
	CCLabelTTF* rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", (*member).rank)->getCString(), mySGD->getFont().c_str(), 10);
	rank_label->setColor(ccBLACK);
	rank_label->setPosition(ccp(11,9));
	rank_label->setAnchorPoint(ccp(0.5,0.5));
	cell->addChild(rank_label);
	
	if(selected_friend_idx == idx)
	{
		CCSprite* selected_img = CCSprite::create("puzzle_right_rank_selected.png");
		selected_img->setPosition(CCPointZero);
		selected_img->setAnchorPoint(CCPointZero);
		cell->addChild(selected_img);
	}
	
	return cell;
}

void PuzzleScene::scrollViewDidScroll(CCScrollView* view){}
void PuzzleScene::scrollViewDidZoom(CCScrollView* view){}
void PuzzleScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	CCLog("touched cell idx : %d", cell->getIdx());
	if(selected_friend_idx == -1)
	{
		selected_friend_idx = cell->getIdx();
		table->updateCellAtIndex(selected_friend_idx);
	}
	else if (cell->getIdx() != selected_friend_idx)
	{
		int keep_idx = selected_friend_idx;
		selected_friend_idx = cell->getIdx();
		table->updateCellAtIndex(keep_idx);
		table->updateCellAtIndex(selected_friend_idx);
	}
	else
	{
		int keep_idx = selected_friend_idx;
		selected_friend_idx = -1;
		table->updateCellAtIndex(keep_idx);
	}
}
CCSize PuzzleScene::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(110, 37);
}
unsigned int PuzzleScene::numberOfCellsInTableView(CCTableView *table)
{
	return friend_list.size();
}

void PuzzleScene::setMonster()
{
	if(monster_node->getTag() != selected_stage_number)
	{
		monster_node->removeAllChildren();
		
		CCSprite* monster_back = CCSprite::create("puzzle_monster_back.png");
		monster_back->setPosition(ccp(0,0));
		monster_node->addChild(monster_back);
		
		CumberShowWindow* monster_img = CumberShowWindow::create(selected_stage_number);
		monster_img->setPosition(ccp(0,0));
		monster_node->addChild(monster_img);
		
		CCSprite* n_start = CCSprite::create("puzzle_right_start.png");
		CCSprite* s_start = CCSprite::create("puzzle_right_start.png");
		s_start->setColor(ccGRAY);
		
		CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(PuzzleScene::menuAction));
		start_item->setTag(kPuzzleMenuTag_start);
		
		monster_start_menu = CCMenu::createWithItem(start_item);
		monster_start_menu->setPosition(ccp(0, -120));
		monster_start_menu->setTouchEnabled(false);
		monster_node->addChild(monster_start_menu);
		
		monster_node->setTag(selected_stage_number);
	}
}

void PuzzleScene::setTop()
{
	CCSprite* top_case = CCSprite::create("puzzle_top.png");
	top_case->setAnchorPoint(ccp(0.5f,1.f));
	top_case->setPosition(ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f));
	addChild(top_case, kPuzzleZorder_top);
	
	CCSprite* n_cancel = CCSprite::create("puzzle_cancel.png");
	CCSprite* s_cancel = CCSprite::create("puzzle_cancel.png");
	s_cancel->setColor(ccGRAY);
	
	CCMenuItem* cancel_item = CCMenuItemSprite::create(n_cancel, s_cancel, this, menu_selector(PuzzleScene::menuAction));
	cancel_item->setTag(kPuzzleMenuTag_cancel);
	
	CCMenu* cancel_menu = CCMenu::createWithItem(cancel_item);
	cancel_menu->setPosition(ccp(80,top_case->getContentSize().height/2.f));
	top_case->addChild(cancel_menu);
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(130,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_time);
	
	CCSprite* n_heart = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_heart = CCSprite::create("mainflow_top_shop.png");
	s_heart->setColor(ccGRAY);
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(PuzzleScene::menuAction));
	heart_item->setTag(kPuzzleMenuTag_heartShop);
	
	CCMenu* heart_menu = CCMenu::createWithItem(heart_item);
	heart_menu->setPosition(ccp(243,top_case->getContentSize().height/2.f-2));
	top_case->addChild(heart_menu);
	
	gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGold())->getCString(), "etc_font.fnt", 0.3f, "%d");
	gold_label->setPosition(ccp(302,top_case->getContentSize().height/2.f));
	top_case->addChild(gold_label);
	
	mySGD->setGoldLabel(gold_label);
	
	CCSprite* n_gold = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_gold = CCSprite::create("mainflow_top_shop.png");
	s_gold->setColor(ccGRAY);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(PuzzleScene::menuAction));
	gold_item->setTag(kPuzzleMenuTag_goldShop);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(ccp(336,top_case->getContentSize().height/2.f-2));
	top_case->addChild(gold_menu);
	
	ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getStar())->getCString(), "etc_font.fnt", 0.3f, "%d");
	ruby_label->setPosition(ccp(391,top_case->getContentSize().height/2.f));
	top_case->addChild(ruby_label);
	
	mySGD->setStarLabel(ruby_label);
	
	CCSprite* n_ruby = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_ruby = CCSprite::create("mainflow_top_shop.png");
	s_ruby->setColor(ccGRAY);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(PuzzleScene::menuAction));
	ruby_item->setTag(kPuzzleMenuTag_rubyShop);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(ccp(421,top_case->getContentSize().height/2.f-2));
	top_case->addChild(ruby_menu);
	
	friend_point_label =  CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getFriendPoint())->getCString(), "etc_font.fnt", 0.3f, "%d");
	friend_point_label->setPosition(ccp(475,top_case->getContentSize().height/2.f));
	top_case->addChild(friend_point_label);
	
	mySGD->setFriendPointLabel(friend_point_label);
	
	CCSprite* n_friend_point = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_friend_point = CCSprite::create("mainflow_top_shop.png");
	s_friend_point->setColor(ccGRAY);
	
	CCMenuItem* friend_point_item = CCMenuItemSprite::create(n_friend_point, s_friend_point, this, menu_selector(PuzzleScene::menuAction));
	friend_point_item->setTag(kPuzzleMenuTag_friendPointContent);
	
	CCMenu* friend_point_menu = CCMenu::createWithItem(friend_point_item);
	friend_point_menu->setPosition(ccp(505,top_case->getContentSize().height/2.f-2));
	top_case->addChild(friend_point_menu);
}

void PuzzleScene::popupClose()
{
	is_menu_enable = true;
}

void PuzzleScene::closeFriendPointPopup()
{
	friend_point_popup->removeFromParent();
	friend_point_popup = NULL;
	is_menu_enable = true;
}