//
//  RankNewPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 3. 27..
//
//

#include "RankNewPopup.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "CommonButton.h"
#include "KSLabelTTF.h"

void RankNewPopup::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool RankNewPopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	
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
	addChild(gray);
	
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(290, 315));
	main_case->setPosition(ccp(240,160-450));
	addChild(main_case, 1);
	
	KSLabelTTF* title_label = KSLabelTTF::create("랭킹", mySGD->getFont().c_str(), 17);
	title_label->setPosition(ccp(40,288));
	main_case->addChild(title_label);
	
	CCScale9Sprite* main_inner = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_inner->setContentSize(CCSizeMake(265, 265));
	main_inner->setPosition(main_case->getContentSize().width/2.f, main_case->getContentSize().height*0.45f);
	main_case->addChild(main_inner);
	
	CommonButton* close_menu = CommonButton::createCloseButton(-185);
	close_menu->setPosition(ccp(main_case->getContentSize().width-29,main_case->getContentSize().height-27));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								menuAction(t_node);
							});
	main_case->addChild(close_menu);
	
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	loading_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
	loading_img->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.4f));
	main_case->addChild(loading_img);
	reader->release();
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getKakaoID();
	
	hspConnector::get()->command("getweeklyrankbyalluser", param, json_selector(this, RankNewPopup::resultGetRank));
	
    return true;
}

void RankNewPopup::resultGetRank(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
//		int alluser = result_data["alluser"].asInt();
		int myrank = result_data["myrank"].asInt();
		
		Json::Value user_list = result_data["list"];
		
		for(int i=0;i<user_list.size() && i < 7;i++)
		{
			string case_name;
			if(myrank == user_list[i]["rank"].asInt())
				case_name = "rankpopup_me.png";
			else
				case_name = "rankpopup_normal.png";
			
			CCSprite* list_cell_case = CCSprite::create(case_name.c_str());
			list_cell_case->setPosition(ccp(main_case->getContentSize().width/2.f,247-i*35));
			main_case->addChild(list_cell_case);
			
			if(myrank == user_list[i]["rank"].asInt())
				list_cell_case->setScale(1.03f);
			
			CCPoint rank_position = ccp(24,15);
			if(i == 0)
			{
				CCSprite* gold_medal = CCSprite::create("rank_gold.png");
				gold_medal->setPosition(rank_position);
				list_cell_case->addChild(gold_medal);
			}
			else if(i == 1)
			{
				CCSprite* silver_medal = CCSprite::create("rank_silver.png");
				silver_medal->setPosition(rank_position);
				list_cell_case->addChild(silver_medal);
			}
			else if(i == 2)
			{
				CCSprite* bronze_medal = CCSprite::create("rank_bronze.png");
				bronze_medal->setPosition(rank_position);
				list_cell_case->addChild(bronze_medal);
			}
			else
			{
				int rank_value = user_list[i]["rank"].asInt();
				if(rank_value <= 0)
					rank_value = result_data["alluser"].asInt()+1;
				
				KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", rank_value)->getCString(), mySGD->getFont().c_str(), 11);
				rank_label->enableOuterStroke(ccBLACK, 1);
				rank_label->setPosition(ccpAdd(rank_position, ccp(0,2)));
				list_cell_case->addChild(rank_label);
			}
			
			Json::Reader reader;
			Json::Value read_data;
			reader.parse(user_list[i].get("data", Json::Value()).asString(), read_data);
			
			KSLabelTTF* nick_label = KSLabelTTF::create(read_data.get("nick", Json::Value()).asString().c_str(), mySGD->getFont().c_str(), 11); // user_list[i]["nick"].asString().c_str()
			nick_label->enableOuterStroke(ccBLACK, 1);
			nick_label->setPosition(ccp(120,17));
			list_cell_case->addChild(nick_label);
			
			KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",user_list[i]["score"].asInt())->getCString()).c_str(), mySGD->getFont().c_str(), 12);
			score_label->enableOuterStroke(ccBLACK, 1);
			score_label->setColor(ccc3(255, 150, 80));
			score_label->setAnchorPoint(ccp(1.f,0.5f));
			score_label->setPosition(ccp(230,17));
			list_cell_case->addChild(score_label);
		}
	}
	else
	{
		CCLabelTTF* fail_label = CCLabelTTF::create("랭킹 정보 확인 실패", mySGD->getFont().c_str(), 12);
		fail_label->setPosition(loading_img->getPosition());
		main_case->addChild(fail_label);
	}
	
	loading_img->removeFromParent();
}

RankNewPopup::~RankNewPopup()
{
	hspConnector::get()->removeTarget(this);
}

void RankNewPopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
}

void RankNewPopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(RankNewPopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void RankNewPopup::endShowPopup()
{
	is_menu_enable = true;
}

void RankNewPopup::hidePopup()
{
	is_menu_enable = false;
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-450));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(RankNewPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void RankNewPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void RankNewPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	
	hidePopup();
}

bool RankNewPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	CCLog("touch swallow!!");
	
	return true;
}
void RankNewPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void RankNewPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void RankNewPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void RankNewPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}