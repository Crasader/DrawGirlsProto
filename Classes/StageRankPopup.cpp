//
//  StageRankPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 21..
//
//

#include "StageRankPopup.h"
#include "StarGoldData.h"
#include "GDWebSprite.h"
#include "SendMessageUtil.h"
#include "GraySprite.h"
#include "StarGoldData.h"
#include "UnknownFriends.h"

StageRankPopup* StageRankPopup::create( CCObject* t_close, SEL_CallFunc d_close, CCObject* t_challenge, SEL_CallFunc d_challenge, int t_stage_number )
{
	StageRankPopup* t_tnp = new StageRankPopup();
	t_tnp->myInit(t_close, d_close, t_challenge, d_challenge, t_stage_number);
	t_tnp->autorelease();
	return t_tnp;
}

void StageRankPopup::myInit( CCObject* t_close, SEL_CallFunc d_close, CCObject* t_challenge, SEL_CallFunc d_challenge, int t_stage_number )
{
	target_close = t_close;
	delegate_close = d_close;
	target_challenge = t_challenge;
	delegate_challenge = d_challenge;
	stage_number = t_stage_number;
	
	back_img = NULL;
	close_menu = NULL;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kSRP_Z_gray);
	
	setBack();
	
	touched_number = 0;
	is_menu_enable = false;
	setTouchEnabled(true);
}

void StageRankPopup::onEnter()
{
	CCLayer::onEnter();
	showPopup();
}

void StageRankPopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.3f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* back_move = CCMoveTo::create(0.5f, ccp(240, 160));
	CCCallFunc* back_call = CCCallFunc::create(this, callfunc_selector(StageRankPopup::endShowPopup));
	CCSequence* back_seq = CCSequence::createWithTwoActions(back_move, back_call);
	back_img->runAction(back_seq);
	
	resultLoadFriends(Json::Value());
}

void StageRankPopup::endShowPopup()
{
	is_menu_enable = true;
}

void StageRankPopup::hidePopup()
{
	is_menu_enable = false;
	CCFadeTo* gray_fade = CCFadeTo::create(0.3f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* back_move = CCMoveTo::create(0.5f, ccp(240, -160));
	CCCallFunc* back_call = CCCallFunc::create(this, callfunc_selector(StageRankPopup::endHidePopup));
	CCSequence* back_seq = CCSequence::createWithTwoActions(back_move, back_call);
	back_img->runAction(back_seq);
}

void StageRankPopup::endHidePopup()
{
	if(mySGD->getIsMeChallenge())
	{
		(target_challenge->*delegate_challenge)();
	}
	else
	{
		(target_close->*delegate_close)();
	}
	removeFromParent();
}

void StageRankPopup::setBack()
{
	back_img = CCSprite::create("stagerank_back.png");
	back_img->setPosition(ccp(240,-160));
	addChild(back_img, kSRP_Z_back);
	
	CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
	CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(StageRankPopup::menuAction));
	close_item->setTag(kSRP_MT_close);
	close_menu = CCMenu::createWithItem(close_item);
	close_menu->setVisible(false);
	close_menu->setPosition(getContentPosition(kSRP_MT_close));
	back_img->addChild(close_menu, kSRP_Z_content);
	
	loading_label = CCLabelTTF::create("Loading...", mySGD->getFont().c_str(), 20);
	loading_label->setPosition(ccp(back_img->getContentSize().width/2.f, back_img->getContentSize().height/2.f));
	back_img->addChild(loading_label, kSRP_Z_content);
}

void StageRankPopup::resultLoadFriends(Json::Value result_data)
{
	Json::Value p;
	Json::Value my_info = hspConnector::get()->myKakaoInfo;
	
	StageRankFriendInfo t_my_info;
	t_my_info.nickname = my_info["nickname"].asString();
	t_my_info.img_url = my_info["profile_image_url"].asString();
	t_my_info.user_id = my_info["user_id"].asString();
	t_my_info.score = 0;
	t_my_info.is_play = false;
	t_my_info.is_message_blocked = my_info["message_blocked"].asBool();
	
	p["memberIDList"].append(t_my_info.user_id);
	
	for(auto i : KnownFriends::getInstance()->getFriends())
	{
		StageRankFriendInfo t_friend_info;
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
		StageRankFriendInfo fInfo;
		fInfo.nickname = i.nick;
		fInfo.img_url = "";
		fInfo.user_id = i.userId;
		fInfo.score = 0;
		fInfo.is_play = false;
		fInfo.is_message_blocked = false;
		friend_list.push_back(fInfo);
		
		p["memberIDList"].append(i.userId);
	}
	
	p["stageNo"]=stage_number;
	hspConnector::get()->command("getstagescorelist",p,json_selector(this, StageRankPopup::resultGetStageScoreList));
}

void StageRankPopup::resultGetStageScoreList(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value score_list = result_data["list"];
		for(int i=0;i<score_list.size();i++)
		{
			vector<StageRankFriendInfo>::iterator iter = find(friend_list.begin(), friend_list.end(), score_list[i]["memberID"].asString().c_str());
			if(iter != friend_list.end())
			{
				(*iter).score = score_list[i]["score"].asFloat();
				(*iter).is_play = true;
			}
			else
				CCLog("not found friend memberID");
		}
		
//		auto beginIter = std::remove_if(friend_list.begin(), friend_list.end(), [=](StageRankFriendInfo t_info)
//										{
//											return !t_info.is_play;
//										});
//		friend_list.erase(beginIter, friend_list.end());
		
		struct t_FriendSort{
			bool operator() (const StageRankFriendInfo& a, const StageRankFriendInfo& b)
			{
				return a.score > b.score;
			}
		} pred;
		
		sort(friend_list.begin(), friend_list.end(), pred);
		
		// create cell
		
//		CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 244, 194));
//		temp_back->setAnchorPoint(CCPointZero);
//		temp_back->setOpacity(100);
//		temp_back->setPosition(ccp(19, 20));
//		back_img->addChild(temp_back, kSRP_Z_content);
		
		rankTableView = CCTableView::create(this, CCSizeMake(244, 194));
		
		rankTableView->setAnchorPoint(CCPointZero);
		rankTableView->setDirection(kCCScrollViewDirectionVertical);
		rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
		rankTableView->setPosition(ccp(19, 20));
		
		rankTableView->setDelegate(this);
		back_img->addChild(rankTableView, kSRP_Z_content);
		rankTableView->setTouchPriority(-171);
		
		//		int myPosition = rankTableView->minContainerOffset().y;
		//		for(int i=0; i<friend_list.size(); i++)
		//		{
		//			if(friend_list[i].user_id == hspConnector::get()->getKakaoID())
		//			{
		//				myPosition = friend_list.size() - i - 1;
		//				break;
		//			}
		//		}
		//		float yInitPosition = MAX(rankTableView->minContainerOffset().y, -cellSizeForTable(rankTableView).height*myPosition + rankTableView->getViewSize().height / 2.f);
		//		yInitPosition = MIN(0, yInitPosition);
		//		rankTableView->setContentOffsetInDuration(ccp(0, yInitPosition), 0.7f);
	}
	
	close_menu->setVisible(true);
	loading_label->removeFromParent();
}

void StageRankPopup::cellAction( CCObject* sender )
{
	int tag = ((CCNode*)sender)->getTag();
	tag -= kSRFC_T_menuBase;
	
	CCLog("challenge memberID : %s", friend_list[tag].user_id.c_str());
//	::setChallengeSendTime(friend_list[tag].user_id);
		mySGD->setIsMeChallenge(true);
	mySGD->setMeChallengeTarget(friend_list[tag].user_id, friend_list[tag].nickname, friend_list[tag].score, friend_list[tag].img_url);
	hidePopup();
}

CCTableViewCell* StageRankPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCLabelTTF* nickname_label;
	CCLabelTTF* score_label;
	CCLabelTTF* rank_label;
	StageRankFriendInfo* member = &friend_list[idx];
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CCSprite* profileImg = GDWebSprite::create((*member).img_url, "ending_noimg.png");
	profileImg->setAnchorPoint(ccp(0.5, 0.5));
	profileImg->setTag(kSRFC_T_img);
	profileImg->setPosition(ccp(64, 21));
	profileImg->setScale(38.f / profileImg->getContentSize().width);
	cell->addChild(profileImg, kSRFC_Z_img);
	
	CCSprite* bg = CCSprite::create("stagerank_cell_back.png");
	bg->setPosition(CCPointZero);
	bg->setAnchorPoint(CCPointZero);
	cell->addChild(bg,kSRFC_Z_case);
	
	nickname_label = CCLabelTTF::create((*member).nickname.c_str(), mySGD->getFont().c_str(), 12);
	nickname_label->setPosition(ccp(85,22));
	nickname_label->setAnchorPoint(CCPointZero);
	nickname_label->setTag(kSRFC_T_nickname);
	cell->addChild(nickname_label,kSRFC_Z_img);
	
	score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", (*member).score)->getCString(), mySGD->getFont().c_str(), 18);
	score_label->setPosition(ccp(85,0));
	score_label->setAnchorPoint(CCPointZero);
	score_label->setTag(kSRFC_T_score);
	cell->addChild(score_label,kSRFC_Z_img);
	
	rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 18);
	rank_label->setPosition(ccp(23,20));
	rank_label->setAnchorPoint(ccp(0.5,0.5));
	rank_label->setTag(kSRFC_T_rank);
	cell->addChild(rank_label,kSRFC_Z_img);
	
	if((*member).user_id == hspConnector::get()->getKakaoID())
	{
		
	}
	else
	{
		if(!(*member).is_message_blocked)
		{
			CCSprite* n_help;
			CCSprite* s_help;
			bool enable = true;
//			int rr = getIsNotChallangableUser((*member).user_id), mySGD->getChallengeCoolTime());

			if(getIsNotChallangableUser( (*member).user_id, StarGoldData::sharedInstance()->getChallengeCoolTime() ))
			{
				n_help = CCSprite::create("stagerank_cell_button_off.png");
				s_help = CCSprite::create("stagerank_cell_button_off.png");
				enable = false;
			}
			else
			{
				n_help = CCSprite::create("stagerank_cell_button_on.png");
				s_help = CCSprite::create("stagerank_cell_button_on.png");
				enable = true;
			}

			
			CCMenuItem* help_item = CCMenuItemSprite::create(n_help, s_help, this, menu_selector(StageRankPopup::cellAction));
			help_item->setTag(kSRFC_T_menuBase + idx);
			
			CCMenu* help_menu = CCMenu::createWithItem(help_item);
			help_menu->setPosition(ccp(211,21));
			cell->addChild(help_menu, kSRFC_Z_img, kSRFC_T_menuBase);
			help_menu->setTouchPriority(-172);
			
			help_item->setEnabled(enable);
			if(enable == false)
				help_item->setColor(ccc3(255, 0, 0));
		}
		else
		{
			CCSprite* not_help = CCSprite::create("stagerank_cell_button_off.png");
			not_help->setPosition(ccp(211,21));
			cell->addChild(not_help, kSRFC_Z_img);
		}
	}
	
	return cell;
}

void StageRankPopup::scrollViewDidScroll( CCScrollView* view )
{
	
}

void StageRankPopup::scrollViewDidZoom( CCScrollView* view )
{
	
}

void StageRankPopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	//		CCLog("%s", m_scoreList[cell->getIdx()]["user_id"].asString().c_str());
}

CCSize StageRankPopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(244, 45);
}

unsigned int StageRankPopup::numberOfCellsInTableView( CCTableView *table )
{
	return friend_list.size();
}

void StageRankPopup::menuAction(CCObject *sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kSRP_MT_close)
	{
		hidePopup();
	}
}

CCPoint StageRankPopup::getContentPosition( int t_tag )
{
	CCPoint return_value;
	
	if(t_tag == kSRP_MT_close)
		return_value = ccp(257, 248);
	
	return return_value;
}

bool StageRankPopup::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number != 0)		return true;
	if(close_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kSRP_MT_close;
	return true;
}

void StageRankPopup::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kSRP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
}

void StageRankPopup::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kSRP_MT_close){						close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
}

void StageRankPopup::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kSRP_MT_close){						close_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
}

void StageRankPopup::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}