//
//  NewStageInfoView.h
//  DGproto
//
//  Created by 사원3 on 2014. 2. 7..
//
//

#ifndef __DGproto__NewStageInfoView__
#define __DGproto__NewStageInfoView__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "DataStorageHub.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"
#include "StarGoldData.h"
#include "hspConnector.h"
#include "GDWebSprite.h"
#include "KSUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum NewStageInfoView_MenuTag{
	kNewStageInfoView_MenuTag_reward = 1,
	kNewStageInfoView_MenuTag_rank
};

enum NewStageInfoView_Mode{
	kNewStageInfoView_Mode_reward = 0,
	kNewStageInfoView_Mode_rank
};

class NewStageInfoView : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
{
public:
	static NewStageInfoView* create(int t_touch_priority)
	{
		NewStageInfoView* t_nsiv = new NewStageInfoView();
		t_nsiv->myInit(t_touch_priority);
		t_nsiv->autorelease();
		return t_nsiv;
	}
	
	void setClickedStage(int t_stage)
	{
		if(recent_stage != t_stage)
		{
			recent_stage = t_stage;
			setContentNode();
		}
	}
	
private:
	int touch_priority;
	
	CCSprite* back_img;
	CCNode* content_node;
	CCMenu* reward_menu;
	CCMenu* rank_menu;
	
	NewStageInfoView_Mode recent_mode;
	
	bool is_show;
	
	int recent_stage;
	bool is_menu_enable;
	
	vector<RankFriendInfo> friend_list;
	CCTableView* rank_table;
	int selected_friend_idx;
	
	void setContentNode()
	{
		if(recent_mode == kNewStageInfoView_Mode_reward)
		{
			content_node->removeAllChildren();
			
			bool is_have_card_list[3] = {false,};
			int step_card_number = NSDS_GI(recent_stage, kSDS_SI_level_int1_card_i, 1);
			is_have_card_list[0] = myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, step_card_number) > 0;
			CCSprite* card_img1;
			if(is_have_card_list[0])
			{
				card_img1 = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png", step_card_number)->getCString()));
				card_img1->setScale(52.f/card_img1->getContentSize().width);
				content_node->addChild(card_img1);
			}
			else
			{
				card_img1 = CCSprite::create("puzzle_reward_cardback.png");
				card_img1->setScale(0.9f);
				content_node->addChild(card_img1);
				
				CCSprite* condition = CCSprite::create("puzzle_reward_condition1.png");
				condition->setPosition(ccp(card_img1->getContentSize().width/2.f, card_img1->getContentSize().height/2.f));
				card_img1->addChild(condition);
			}
			card_img1->setPosition(ccp(-31, 39.5));
			
			
			step_card_number = NSDS_GI(recent_stage, kSDS_SI_level_int1_card_i, 2);
			is_have_card_list[1] = myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, step_card_number) > 0;
			CCSprite* card_img2;
			CCSprite* card_img3;
			if(is_have_card_list[1])
			{
				card_img2 = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png", step_card_number)->getCString()));
				card_img2->setScale(52.f/card_img2->getContentSize().width);
				content_node->addChild(card_img2);
				
				card_img3 = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png", step_card_number)->getCString()));
				card_img3->setScale(52.f/card_img3->getContentSize().width);
				content_node->addChild(card_img3);
			}
			else
			{
				card_img2 = CCSprite::create("puzzle_reward_cardback.png");
				card_img2->setScale(0.9f);
				content_node->addChild(card_img2);
				
				CCSprite* condition = CCSprite::create("puzzle_reward_condition2.png");
				condition->setPosition(ccp(card_img2->getContentSize().width/2.f, card_img2->getContentSize().height/2.f));
				content_node->addChild(condition);
				
				card_img3 = CCSprite::create("puzzle_reward_cardback.png");
				card_img3->setScale(0.9f);
				content_node->addChild(card_img3);
				
				CCSprite* condition2 = CCSprite::create("puzzle_reward_condition3.png");
				condition2->setPosition(ccp(card_img3->getContentSize().width/2.f, card_img3->getContentSize().height/2.f));
				card_img3->addChild(condition2);
			}
			card_img2->setPosition(ccp(31, 39.5));
			card_img3->setPosition(ccp(-31, -55.5));
			
			
			step_card_number = NSDS_GI(recent_stage, kSDS_SI_level_int1_card_i, 3);
			is_have_card_list[2] = myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, step_card_number) > 0;
			CCSprite* card_img4;
			if(is_have_card_list[2])
			{
				card_img4 = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png", step_card_number)->getCString()));
				card_img4->setScale(52.f/card_img4->getContentSize().width);
				content_node->addChild(card_img4);
			}
			else
			{
				card_img4 = CCSprite::create("puzzle_reward_cardback.png");
				card_img4->setScale(0.9f);
				content_node->addChild(card_img4);
				
				CCSprite* condition = CCSprite::create("puzzle_reward_condition4.png");
				condition->setPosition(ccp(card_img4->getContentSize().width/2.f, card_img4->getContentSize().height/2.f));
				card_img4->addChild(condition);
			}
			card_img4->setPosition(ccp(31, -55.5));
			
			
			
			CCSprite* reward_back = CCSprite::create("stageinfoview_card_frame.png");
			reward_back->setPosition(ccp(0,-15));
			content_node->addChild(reward_back);
			
			
			
			step_card_number = NSDS_GI(recent_stage, kSDS_SI_level_int1_card_i, 1);
			CCLabelTTF* step_reward1 = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_reward_i, step_card_number))->getCString(), mySGD->getFont().c_str(), 8);
			step_reward1->setPosition(ccpAdd(card_img1->getPosition(), ccp(14, -47)));
			content_node->addChild(step_reward1);
			
			if(is_have_card_list[0])
			{
				CCSprite* t_complete1 = CCSprite::create("stageinfo_complete.png");
				t_complete1->setPosition(ccpAdd(card_img1->getPosition(), ccp(-12, 19)));
				content_node->addChild(t_complete1);
			}
			
			
			step_card_number = NSDS_GI(recent_stage, kSDS_SI_level_int1_card_i, 2);
			CCLabelTTF* step_reward2 = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_reward_i, step_card_number))->getCString(), mySGD->getFont().c_str(), 8);
			step_reward2->setPosition(ccpAdd(card_img2->getPosition(), ccp(14, -47)));
			content_node->addChild(step_reward2);
			
			CCLabelTTF* step_reward3 = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_reward_i, step_card_number))->getCString(), mySGD->getFont().c_str(), 8);
			step_reward3->setPosition(ccpAdd(card_img3->getPosition(), ccp(14, -47)));
			content_node->addChild(step_reward3);
			
			if(is_have_card_list[1])
			{
				CCSprite* t_complete2 = CCSprite::create("stageinfo_complete.png");
				t_complete2->setPosition(ccpAdd(card_img2->getPosition(), ccp(-12, 19)));
				content_node->addChild(t_complete2);
				
				CCSprite* t_complete3 = CCSprite::create("stageinfo_complete.png");
				t_complete3->setPosition(ccpAdd(card_img3->getPosition(), ccp(-12, 19)));
				content_node->addChild(t_complete3);
			}
			
			
			step_card_number = NSDS_GI(recent_stage, kSDS_SI_level_int1_card_i, 3);
			CCLabelTTF* step_reward4 = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_reward_i, step_card_number))->getCString(), mySGD->getFont().c_str(), 8);
			step_reward4->setPosition(ccpAdd(card_img4->getPosition(), ccp(14, -47)));
			content_node->addChild(step_reward4);
			
			if(is_have_card_list[2])
			{
				CCSprite* t_complete4 = CCSprite::create("stageinfo_complete.png");
				t_complete4->setPosition(ccpAdd(card_img4->getPosition(), ccp(-12, 19)));
				content_node->addChild(t_complete4);
			}
		}
		else if(recent_mode == kNewStageInfoView_Mode_rank)
		{
			selected_friend_idx = -1;
			friend_list.clear();
			content_node->removeAllChildren();
			rank_table = NULL;
			
			Json::Value p;
			Json::Value my_info = hspConnector::get()->myKakaoInfo;
			
			RankFriendInfo t_my_info;
			t_my_info.nickname = my_info["nickname"].asString();
			t_my_info.img_url = my_info["profile_image_url"].asString();
			t_my_info.user_id = my_info["user_id"].asString();
			t_my_info.score = 0;
			t_my_info.is_play = false;
			t_my_info.is_message_blocked = my_info["message_blocked"].asBool();
			friend_list.push_back(t_my_info);
			
			p["memberIDList"].append(t_my_info.user_id);
			
			for(auto i : KnownFriends::getInstance()->getFriends())
			{
				RankFriendInfo t_friend_info;
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
				RankFriendInfo fInfo;
				fInfo.nickname = i.nick;
				fInfo.img_url = "";
				fInfo.user_id = i.userId;
				fInfo.score = 0;
				fInfo.is_play = false;
				fInfo.is_message_blocked = false;
				friend_list.push_back(fInfo);
				
				p["memberIDList"].append(i.userId);
			}
			
			p["stageNo"] = recent_stage;
			hspConnector::get()->command("getstagescorelist",p,json_selector(this, NewStageInfoView::resultGetStageScoreList));
		}
	}
	
	void resultGetStageScoreList(Json::Value result_data)
	{
		if(result_data["result"]["code"].asInt() == GDSUCCESS && result_data["param"]["stageNo"].asInt() == recent_stage && !rank_table)
		{
			Json::Value score_list = result_data["list"];
			for(int i=0;i<score_list.size();i++)
			{
				vector<RankFriendInfo>::iterator iter = find(friend_list.begin(), friend_list.end(), score_list[i]["memberID"].asString());
				if(iter != friend_list.end())
				{
					(*iter).score = score_list[i]["score"].asFloat();
					(*iter).is_play = true;
				}
				else
					CCLog("not found friend memberID : %s", score_list[i]["memberID"].asString().c_str());
			}
			
			auto beginIter = std::remove_if(friend_list.begin(), friend_list.end(), [=](RankFriendInfo t_info)
											{
												return !t_info.is_play;
											});
			friend_list.erase(beginIter, friend_list.end());
			
			struct t_FriendSort{
				bool operator() (const RankFriendInfo& a, const RankFriendInfo& b)
				{
					return a.score > b.score;
				}
			} pred;
			
			sort(friend_list.begin(), friend_list.end(), pred);
			
			for(int i=0;i<friend_list.size();i++)
				friend_list[i].rank = i+1;
			
			mySGD->save_stage_rank_stageNumber = recent_stage;
			mySGD->save_stage_rank_list = friend_list;
			
			
			selected_friend_idx = -1;
			
			CCSize table_size = CCSizeMake(130, 190);
			
			CCSprite* temp_table = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
			temp_table->setAnchorPoint(CCPointZero);
			temp_table->setPosition(ccp(-table_size.width/2.f, -table_size.height/2.f-15));
			temp_table->setOpacity(100);
			content_node->addChild(temp_table);
			
			rank_table = CCTableView::create(this, table_size);
			rank_table->setAnchorPoint(CCPointZero);
			rank_table->setDirection(kCCScrollViewDirectionVertical);
			rank_table->setVerticalFillOrder(kCCTableViewFillTopDown);
			rank_table->setPosition(ccp(-table_size.width/2.f, -table_size.height/2.f-15));
			
			rank_table->setDelegate(this);
			content_node->addChild(rank_table);
			rank_table->setTouchPriority(touch_priority-1);
		}
	}
	
	virtual CCTableViewCell* tableCellAtIndex( CCTableView *table, unsigned int idx )
	{
		RankFriendInfo* member = &friend_list[idx];
		CCTableViewCell* cell = new CCTableViewCell();
		cell->init();
		cell->autorelease();
		
		string my_id = hspConnector::get()->myKakaoInfo["user_id"].asString();
		string cell_id = (*member).user_id;
		
		CCSprite* back_img;
		if(my_id == cell_id)
			back_img = CCSprite::create("puzzle_right_ranklist_me.png");
		else if(idx == 0)
			back_img = CCSprite::create("puzzle_right_ranklist_gold.png");
		else if(idx == 1)
			back_img = CCSprite::create("puzzle_right_ranklist_silver.png");
		else if(idx == 2)
			back_img = CCSprite::create("puzzle_right_ranklist_bronze.png");
		else
			back_img = CCSprite::create("puzzle_right_ranklist_normal.png");
		back_img->setPosition(CCPointZero);
		back_img->setAnchorPoint(CCPointZero);
		cell->addChild(back_img);
		
		CCSprite* profileImg = GDWebSprite::create((*member).img_url, "ending_noimg.png");
		profileImg->setAnchorPoint(ccp(0.5, 0.5));
		profileImg->setPosition(ccp(21, 21));
		cell->addChild(profileImg, -1);
		
		if(my_id != cell_id && KnownFriends::getInstance()->findById(cell_id) != nullptr)
		{
			CCSprite* kakao_sign = CCSprite::create("puzzle_right_rank_kakao.png");
			kakao_sign->setPosition(ccp(10,28));
			cell->addChild(kakao_sign);
		}
		
		CCLabelTTF* nickname_label = CCLabelTTF::create((*member).nickname.c_str(), mySGD->getFont().c_str(), 10);
		nickname_label->setPosition(ccp(89,27));
		cell->addChild(nickname_label);
		
		CCLabelTTF* score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", (*member).score)->getCString(), mySGD->getFont().c_str(), 8);
		score_label->setColor(ccBLACK);
		score_label->setPosition(ccp(89,12));
		cell->addChild(score_label);
		
		if(idx == 0)
		{
			CCSprite* medal_img = CCSprite::create("puzzle_right_rank_gold.png");
			medal_img->setPosition(ccp(50,20));
			cell->addChild(medal_img);
		}
		else if(idx == 1)
		{
			CCSprite* medal_img = CCSprite::create("puzzle_right_rank_silver.png");
			medal_img->setPosition(ccp(50,20));
			cell->addChild(medal_img);
		}
		else if(idx == 2)
		{
			CCSprite* medal_img = CCSprite::create("puzzle_right_rank_bronze.png");
			medal_img->setPosition(ccp(50,20));
			cell->addChild(medal_img);
		}
		else
		{
			CCLabelTTF* rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", (*member).rank)->getCString(), mySGD->getFont().c_str(), 14);
			rank_label->setPosition(ccp(50,20));
			cell->addChild(rank_label);
		}
		
		if(selected_friend_idx == idx)
		{
			CCSprite* selected_img = CCSprite::create("puzzle_right_rank_selected.png");
			selected_img->setPosition(CCPointZero);
			selected_img->setAnchorPoint(CCPointZero);
			cell->addChild(selected_img);
		}
		
		return cell;
	}
	
	virtual void scrollViewDidScroll(CCScrollView* view){}
	virtual void scrollViewDidZoom(CCScrollView* view){}
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell)
	{
		CCLog("touched cell idx : %d", cell->getIdx());
		
		string touched_id = friend_list[cell->getIdx()].user_id;
		string my_id = hspConnector::get()->myKakaoInfo["user_id"].asString();
		
		if(touched_id != my_id)
		{
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
	}
	virtual CCSize cellSizeForTable(CCTableView *table)
	{
		return CCSizeMake(110, 40);
	}
	virtual unsigned int numberOfCellsInTableView(CCTableView *table)
	{
		return friend_list.size();
	}
	
	void myInit(int t_touch_priority)
	{
		is_show = true;
		opacity_value = 255;
		is_menu_enable = false;
		touch_priority = t_touch_priority;
		setTouchEnabled(true);
		
		recent_mode = kNewStageInfoView_Mode_reward;
		
		back_img = CCSprite::create("stageinfoview_back.png");
		back_img->setPosition(ccp(410,170));
		addChild(back_img);
		
		content_node = CCNode::create();
		content_node->setPosition(ccp(back_img->getContentSize().width/2.f, back_img->getContentSize().height/2.f));
		back_img->addChild(content_node);
		
		reward_menu = NULL;
		setRewardMenu();
		
		rank_menu = NULL;
		setRankMenu();
		
		recent_stage = 0;
		is_menu_enable = true;
	}
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable || !is_show)
			return;
		
		is_menu_enable = false;
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kNewStageInfoView_MenuTag_reward)
		{
			if(recent_mode != kNewStageInfoView_Mode_reward)
			{
				recent_mode = kNewStageInfoView_Mode_reward;
				setContentNode();
			}
			setRewardMenu();
			setRankMenu();
			
			is_menu_enable = true;
		}
		else if(tag == kNewStageInfoView_MenuTag_rank)
		{
			if(recent_mode != kNewStageInfoView_Mode_rank)
			{
				recent_mode = kNewStageInfoView_Mode_rank;
				setContentNode();
			}
			setRewardMenu();
			setRankMenu();
			
			is_menu_enable = true;
		}
	}
	
	void setRewardMenu()
	{
		if(recent_mode == kNewStageInfoView_Mode_reward && reward_menu)
		{
			reward_menu->removeFromParent();
			reward_menu = NULL;
		}
		else if(recent_mode == kNewStageInfoView_Mode_rank && !reward_menu)
		{
			CCSprite* n_reward = CCSprite::create("puzzle_right_reward_off.png");
			CCSprite* s_reward = CCSprite::create("puzzle_right_reward_off.png");
			s_reward->setColor(ccGRAY);
			
			CCMenuItem* reward_item = CCMenuItemSprite::create(n_reward, s_reward, this, menu_selector(NewStageInfoView::menuAction));
			reward_item->setTag(kNewStageInfoView_MenuTag_reward);
			
			reward_menu = CCMenu::createWithItem(reward_item);
			reward_menu->setPosition(ccp(back_img->getContentSize().width/2.f - 33, back_img->getContentSize().height/2.f + 96));
			back_img->addChild(reward_menu);
			
			reward_menu->setTouchPriority(touch_priority-2);
		}
	}
	
	void setRankMenu()
	{
		if(recent_mode == kNewStageInfoView_Mode_rank && rank_menu)
		{
			rank_menu->removeFromParent();
			rank_menu = NULL;
		}
		else if(recent_mode == kNewStageInfoView_Mode_reward && !rank_menu)
		{
			CCSprite* n_rank = CCSprite::create("puzzle_right_rank_off.png");
			CCSprite* s_rank = CCSprite::create("puzzle_right_rank_off.png");
			s_rank->setColor(ccGRAY);
			
			CCMenuItem* rank_item = CCMenuItemSprite::create(n_rank, s_rank, this, menu_selector(NewStageInfoView::menuAction));
			rank_item->setTag(kNewStageInfoView_MenuTag_rank);
			
			rank_menu = CCMenu::createWithItem(rank_item);
			rank_menu->setPosition(ccp(back_img->getContentSize().width/2.f + 32, back_img->getContentSize().height/2.f + 96));
			back_img->addChild(rank_menu);
			
			rank_menu->setTouchPriority(touch_priority-2);
		}
	}
	
	void startHideAction()
	{
		is_show = false;
		unschedule(schedule_selector(NewStageInfoView::showing));
		schedule(schedule_selector(NewStageInfoView::hiding));
	}
	void hiding()
	{
		KS::setOpacity(back_img, opacity_value);
		
		if(opacity_value <= 30)
		{
			endHideAction();
		}
		else
			opacity_value-=5;
	}
	void endHideAction()
	{
		unschedule(schedule_selector(NewStageInfoView::hiding));
	}
	
	void startShowAction()
	{
		is_show = true;
		unschedule(schedule_selector(NewStageInfoView::hiding));
		schedule(schedule_selector(NewStageInfoView::showing));
	}
	void showing()
	{
		KS::setOpacity(back_img, opacity_value);
		
		if(opacity_value == 255)
		{
			endShowAction();
		}
		else
			opacity_value+=5;
	}
	void endShowAction()
	{
		unschedule(schedule_selector(NewStageInfoView::showing));
	}
	
	int opacity_value;
	vector<int> keep_touch_list;
	
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
	{
		CCPoint touchLocation = pTouch->getLocation();
		CCPoint local = back_img->convertToNodeSpace(touchLocation);
		
		if(is_show && !back_img->boundingBox().containsPoint(local))
		{
			keep_touch_list.push_back((int)pTouch);
			startHideAction();
		}
		
		return true;
	}
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
	{
		
	}
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
	{
		vector<int>::iterator iter = find(keep_touch_list.begin(), keep_touch_list.end(), (int)pTouch);
		if(iter != keep_touch_list.end())
		{
			keep_touch_list.erase(iter);
			if(keep_touch_list.size() <= 0)
				startShowAction();
		}
	}
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
	{
		vector<int>::iterator iter = find(keep_touch_list.begin(), keep_touch_list.end(), (int)pTouch);
		if(iter != keep_touch_list.end())
		{
			keep_touch_list.erase(iter);
			if(keep_touch_list.size() <= 0)
				startShowAction();
		}
	}
	virtual void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touch_priority, false);
	}
};


#endif /* defined(__DGproto__NewStageInfoView__) */
