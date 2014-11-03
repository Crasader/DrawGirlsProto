//
//  ScenarioScene.h
//  DGproto
//
//  Created by LitQoo on 2014. 9. 15..
//
//

#ifndef __DGproto__ScenarioScene__
#define __DGproto__ScenarioScene__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "GraphDog.h"
#include "MyLocalization.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "TypingBox.h"
#include "KSUtil.h"
#include "StyledLabelTTF.h"
#include "TouchSuctionLayer.h";
#include "EasingAction.h";
#include "CCMenuLambda.h";
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


class StoryLayer : public CCLayer{
public:
	string storyID;
	CCLayerColor *black;
	CCLayer *back;
	CCLayerColor *top;
	CCLayerColor *bottom;
	TouchSuctionLayer *suction;
	Json::Value storyData;
	int storySeq;
	function<void(void)> callbackFunc;
	bool isAdd;
    
	static StoryLayer* create(string _storyID)
	{
		StoryLayer* obj = new StoryLayer();
		obj->init(_storyID);
		obj->autorelease();
		return obj;
	}
    
	bool init(string _storyID){
		if(!CCLayer::init()){
			return false;
		}
		
		isAdd=false;
		suction = TouchSuctionLayer::create(-9998);
		addChild(suction);
		
		suction->setTouchEnabled(true);
		this->storyID=_storyID;
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		float ui_top = myDSH->ui_top;
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
       // CCLOG("init storyLayer %f,%f,%f,%f,%f,%f,%f",screen_size.width,screen_size.height,myDSH->ui_top);
		top = CCLayerColor::create(ccc4(0,0,0,255), screen_scale_x*480,50);
		top->setAnchorPoint(ccp(0.5,0));
        top->ignoreAnchorPointForPosition(false);
		top->setPosition(ccp(240,320+(screen_scale_y*(320)-320)/2.f));
		addChild(top);
		
		bottom = CCLayerColor::create(ccc4(0,0,0,255), screen_scale_x*480,50);
		bottom->setAnchorPoint(ccp(0.5,0));
        bottom->ignoreAnchorPointForPosition(false);
		bottom->setPosition(ccp(240,-(screen_scale_y*320-320)/2.f-50));
		addChild(bottom);
		
		this->addChild(KSGradualValue<float>::create(0.f,50.f,0.5f,[this,screen_scale_y](float t){
			bottom->setPositionY(-(screen_scale_y*(320)-320)/2.f-50+t);
			top->setPositionY(320+(screen_scale_y*(320)-320)/2.f-t);
			
		},[](float t){}));
		
		
		black = CCLayerColor::create(ccc4(0,0,0,0), 480, 320);
        
        black->setScaleX(screen_scale_x);
        black->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
        

        addChild(black);
        
		back = CCLayer::create();
		addChild(back);
		
		CCSprite* n_skip = CCSprite::create("kt_skip.png");
		CCSprite* s_skip = CCSprite::create("kt_skip.png");
		s_skip->setColor(ccGRAY);
		
		CCMenuLambda* skip_menu = CCMenuLambda::create();
		skip_menu->setPosition(ccp(240-240*screen_scale_x + 35, 160+160*screen_scale_y - 25 + 150 - 150));
		skip_menu->setTouchPriority(-19999);
		skip_menu->setEnabled(true );
		
		CCMenuItemLambda* skip_item = CCMenuItemSpriteLambda::create(n_skip, s_skip, [skip_menu,this](CCObject* sender)
																	 {
																		 skip_menu->setEnabled(false);
                                                                         this->storySeq=this->storyData.size();
                                                                         this->playScript();
                                                                     });
        skip_menu->addChild(skip_item);
        
		//this->storyData = Json::Value(Json::arrayValue);
        this->storySeq=0;
		this->m_sss=0;
		this->addChild(skip_menu, 999);
        
        

		return true;
	}
	
	enum BoxType{
		kBoxBig,
		kBoxMiddle,
		kBoxSmall
	};
	

	
	enum ObjType{
		kObjTypeNone=0x00,
		kObjTypeActorLeft=0x01,
		kObjTypeActorRight=0x02,
		kObjTypeActorCenter=0x04,
		kObjTypeActor=0x07,
		kObjTypeText=0x08,
		kObjTypeTitle=0x10,
		kObjTypeSpot=0x20,
		kObjTypeImage=0x40,
		kObjTypeAll=0xff
	};
	
	enum scriptType{
		kScriptTypeTalk = 1,
		kScriptTypeTitle = 2,
		kScriptTypeSpot = 3,
		kScriptTypeImage = 4
	};
	
	static void startStory(CCLayer* obj,string _storyID,function<void(void)> _endFunc=nullptr){
		
		if(!obj){
			obj=(CCLayer *)(CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0));
		}
		StoryLayer *sl  = StoryLayer::create(_storyID);
        TRACE();
        sl->callbackFunc = _endFunc;
		TRACE();
        obj->addChild(sl,9999999);
        
		TRACE();
        sl->loadStory();
		TRACE();
        sl->addChild(KSGradualValue<float>::create(0, 150, 1, [sl](int a){
			sl->black->setOpacity(a);
		}, [sl](int a){
            CCLOG("play %s",sl->storyID.c_str());
			sl->playScript();
		}));
        TRACE();
	}
	
	
	void loadStory(){
		
        CCLOG("load story");
		
		beginScene("puzzle1");
		addScript("",getLocal(LK::kStory_1_1),kCCTextAlignmentCenter,kObjTypeText,kBoxBig);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_1_2),kCCTextAlignmentLeft,kObjTypeText,kBoxBig);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_1_3),kCCTextAlignmentLeft,kObjTypeText,kBoxBig);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_1_4),kCCTextAlignmentRight,kObjTypeText,kBoxBig);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_1_5),kCCTextAlignmentLeft,kObjTypeText,kBoxBig);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_1_6),kCCTextAlignmentRight,kObjTypeText,kBoxBig);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_1_7),kCCTextAlignmentLeft,kObjTypeAll,kBoxBig);
		
		////////////////////////////////////
		
		beginScene("puzzle2");
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_2_1),kCCTextAlignmentLeft,kObjTypeText|kObjTypeActorRight);
		addScript("kt_cha_hibari_1.png",getLocal(LK::kStory_2_2),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_2_3),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_2_4),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_2_5),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_hibari_1.png",getLocal(LK::kStory_2_6),kCCTextAlignmentRight,kObjTypeAll);
		addScript("kt_cha_yagyu_1.png",getLocal(LK::kStory_2_7),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_hibari_1.png",getLocal(LK::kStory_2_8),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_yagyu_1.png",getLocal(LK::kStory_2_9),kCCTextAlignmentLeft,kObjTypeAll);
		
		////////////////////////////////////
		
		beginScene("puzzle3");
		addScript("kt_cha_hibari_1.png",getLocal(LK::kStory_3_1),kCCTextAlignmentLeft,kObjTypeText,kBoxBig);
		addScript("kt_cha_yagyu_1.png",getLocal(LK::kStory_3_2),kCCTextAlignmentRight,kObjTypeText,kBoxBig);
		addScript("kt_cha_hibari_1.png",getLocal(LK::kStory_3_3),kCCTextAlignmentLeft,kObjTypeText,kBoxBig);
		addScript("kt_cha_yagyu_1.png",getLocal(LK::kStory_3_4),kCCTextAlignmentRight,kObjTypeText,kBoxBig);
		addScript("kt_cha_hibari_1.png",getLocal(LK::kStory_3_5),kCCTextAlignmentLeft,kObjTypeAll);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_3_6),kCCTextAlignmentRight,kObjTypeText,kBoxBig);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_3_7),kCCTextAlignmentLeft,kObjTypeText,kBoxBig);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_3_8),kCCTextAlignmentRight,kObjTypeText,kBoxBig);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_3_9),kCCTextAlignmentRight,kObjTypeText,kBoxBig);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_3_10),kCCTextAlignmentLeft,kObjTypeText,kBoxBig);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_3_11),kCCTextAlignmentRight,kObjTypeAll);
		
		////////////////////////////////////
		beginScene("puzzle4");
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_4_1),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_4_2),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_4_3),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_4_4),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_4_5),kCCTextAlignmentRight,kObjTypeAll);
		
		////////////////////////////////////
		beginScene("puzzle5");
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_5_1),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_5_2),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_5_3),kCCTextAlignmentRight,kObjTypeActorLeft|kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_5_4),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_5_5),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_5_6),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_5_7),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_5_8),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_5_9),kCCTextAlignmentLeft,kObjTypeAll);
		
		////////////////////////////////////////
		
		beginScene("puzzle6");
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_6_1),kCCTextAlignmentLeft,kObjTypeAll);
		addScript("",getLocal(LK::kStory_6_2),kCCTextAlignmentCenter,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_6_3),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_6_4),kCCTextAlignmentLeft,kObjTypeAll);
		addScript("",getLocal(LK::kStory_6_5),kCCTextAlignmentCenter,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_6_6),kCCTextAlignmentLeft,kObjTypeAll);
		
		/////////////////////////////////////////
		
		beginScene("puzzle7");
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_7_1),kCCTextAlignmentRight,kObjTypeAll);
		addScript("",getLocal(LK::kStory_7_2),kCCTextAlignmentCenter,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_7_3),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_7_4),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kStory_7_5),kCCTextAlignmentRight,kObjTypeAll);
		
		/////////////////////////////////////////
		
		beginScene("puzzle8");
		addScript("kt_cha_hibari_1.png",getLocal(LK::kStory_8_1),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_yagyu_1.png",getLocal(LK::kStory_8_2),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_hibari_1.png",getLocal(LK::kStory_8_3),kCCTextAlignmentLeft,kObjTypeAll);
		addScript("",getLocal(LK::kStory_8_4),kCCTextAlignmentCenter,kObjTypeText);
		addScript("kt_cha_yagyu_1.png",getLocal(LK::kStory_8_5),kCCTextAlignmentRight,kObjTypeAll);
		addScript("",getLocal(LK::kStory_8_6),kCCTextAlignmentCenter,kObjTypeText);
		addScript("kt_cha_yagyu_1.png",getLocal(LK::kStory_8_7),kCCTextAlignmentRight,kObjTypeAll);
		addScript("",getLocal(LK::kStory_8_8),kCCTextAlignmentCenter,kObjTypeText);
		
		//////////////////////////////////////////
		
		beginScene("puzzle9");
		addScript("kt_cha_yagyu_1.png",getLocal(LK::kStory_9_1),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_hibari_1.png",getLocal(LK::kStory_9_2),kCCTextAlignmentLeft,kObjTypeAll);
		addScript("",getLocal(LK::kStory_9_3),kCCTextAlignmentCenter,kObjTypeText);
		addScript("kt_cha_hibari_1.png",getLocal(LK::kStory_9_4),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_yagyu_1.png",getLocal(LK::kStory_9_5),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_hibari_1.png",getLocal(LK::kStory_9_6),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_yagyu_1.png",getLocal(LK::kStory_9_7),kCCTextAlignmentRight,kObjTypeAll);
		
		/////////////////////////////////////////////
		
		beginScene("puzzle10");
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_10_1),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_10_2),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_10_3),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_10_4),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_10_5),kCCTextAlignmentRight,kObjTypeAll);
		
		//////////////////////////////////////////
		beginScene("puzzle11");
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_11_1),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_11_2),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_11_3),kCCTextAlignmentRight,kObjTypeAll);

		////////////////////////////////////
		beginScene("puzzle12");
		addScript("kt_cha_hibari_1.png",getLocal(LK::kStory_12_1),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_hibari_1.png",getLocal(LK::kStory_12_2),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_12_3),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_12_4),kCCTextAlignmentRight,kObjTypeAll);
		
		
		
		////////////////////////////////////
		beginScene("puzzle13");
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_13_1),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_13_2),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_13_3),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_13_4),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_13_5),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_13_6),kCCTextAlignmentLeft,kObjTypeAll);
		
		////////////////////////////////////
		beginScene("puzzle14");
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_14_1),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_14_2),kCCTextAlignmentLeft,kObjTypeAll);
		addScript("",getLocal(LK::kStory_14_3),kCCTextAlignmentCenter,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_14_4),kCCTextAlignmentLeft,kObjTypeText);
		
		////////////////////////////////////
		beginScene("puzzle15");
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_15_1),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_homura_1.png",getLocal(LK::kStory_15_2),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_15_3),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_homura_1.png",getLocal(LK::kStory_15_4),kCCTextAlignmentRight,kObjTypeAll);
		
		
		////////////////////////////////////
		
		beginScene("puzzle16");
		addScript("kt_cha_homura_1.png",getLocal(LK::kStory_16_1),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_16_2),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_homura_1.png",getLocal(LK::kStory_16_3),kCCTextAlignmentRight,kObjTypeAll);
		
		////////////////////////////////////
		
		beginScene("puzzle17");
		addScript("kt_cha_homura_1.png",getLocal(LK::kStory_17_1),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_17_2),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_homura_1.png",getLocal(LK::kStory_17_3),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_17_4),kCCTextAlignmentLeft,kObjTypeAll);
		
		////////////////////////////////////
		
		beginScene("puzzle18");
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_18_1),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kStory_18_2),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_yomi_1.png",getLocal(LK::kStory_18_3),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_18_4),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_yomi_1.png",getLocal(LK::kStory_18_5),kCCTextAlignmentLeft,kObjTypeAll);
		
		////////////////////////////////////
		
		beginScene("puzzle19");

		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_19_1),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_yomi_1.png",getLocal(LK::kStory_19_2),kCCTextAlignmentLeft,kObjTypeAll);
		
		////////////////////////////////////
		beginScene("puzzle20");
		
		addScript("kt_cha_yomi_1.png",getLocal(LK::kStory_20_1),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kStory_20_2),kCCTextAlignmentLeft,kObjTypeAll);
		
		
		
		
		//2부하몹잡기
		beginScene("mission2");
		//addSpot("missionPannel","rect",0.5f,kObjTypeNone);
		addScript("kt_cha_yagyu_1.png",getLocal(LK::kMyLocalKey_kindTutorial11),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_hibari_1.png",getLocal(LK::kMyLocalKey_kindTutorial12),kCCTextAlignmentRight,kObjTypeText);
		
		//4수집가
		beginScene("mission4");
		//addSpot("missionPannel","rect",0.5f,kObjTypeNone);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kMyLocalKey_kindTutorial13),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial14),kCCTextAlignmentRight,kObjTypeText);
		
		//7비지니스맨
		beginScene("mission7");
		//addSpot("missionPannel","rect",0.5f,kObjTypeNone);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial15),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kMyLocalKey_kindTutorial16),kCCTextAlignmentRight,kObjTypeText);
		
		//8헬모드
		beginScene("mission8");
		//addSpot("missionPannel","rect",0.5f,kObjTypeNone);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial33),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kMyLocalKey_kindTutorial34),kCCTextAlignmentRight,kObjTypeText);
		
		//9퍼센트
		beginScene("mission9");
		//addSpot("missionPannel","rect",0.5f,kObjTypeNone);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial23),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kMyLocalKey_kindTutorial24),kCCTextAlignmentRight,kObjTypeText);
		
		//10점수
		beginScene("mission10");
		//addSpot("missionPannel","rect",0.5f,kObjTypeNone);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial25),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kMyLocalKey_kindTutorial26),kCCTextAlignmentRight,kObjTypeText);
		
		//11콤보
		beginScene("mission11");
		//addSpot("missionPannel","rect",0.5f,kObjTypeNone);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial27),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kMyLocalKey_kindTutorial28),kCCTextAlignmentRight,kObjTypeText);
		
		//12골드
		beginScene("mission12");
		//addSpot("missionPannel","rect",0.5f,kObjTypeNone);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial29),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kMyLocalKey_kindTutorial30),kCCTextAlignmentRight,kObjTypeText);
		
		//13턴수
		beginScene("mission13");
		//addSpot("missionPannel","rect",0.5f,kObjTypeNone);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial31),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kMyLocalKey_kindTutorial32),kCCTextAlignmentRight,kObjTypeText);
		
		
		//랭킹팝업 튜토리얼
		beginScene("menu_rank");
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial6),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial7),kCCTextAlignmentLeft,kObjTypeText);
		
		//상점
		//beginScene("menu_shop");
		//addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial6),kCCTextAlignmentLeft,kObjTypeText);
		//addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial7),kCCTextAlignmentRight,kObjTypeText);

		//뽑기
		//beginScene("menu_gacha");
		//addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial6),kCCTextAlignmentLeft,kObjTypeText);
		//addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial7),kCCTextAlignmentRight,kObjTypeText);

		//내카드
		beginScene("menu_mycard");
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial3),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial4),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial5),kCCTextAlignmentLeft,kObjTypeText);

		//오늘의임무
		//beginScene("menu_todaymission");
		//addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial6),kCCTextAlignmentLeft,kObjTypeText);
		//addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial7),kCCTextAlignmentRight,kObjTypeText);

		//친구
		beginScene("menu_friend");
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial38),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial39),kCCTextAlignmentLeft,kObjTypeText);
		
		//선물함
		//beginScene("menu_msgbox");
		//addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial6),kCCTextAlignmentLeft,kObjTypeText);
		//addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial7),kCCTextAlignmentRight,kObjTypeText);

		//업적
		beginScene("menu_archivement");
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial1),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial2),kCCTextAlignmentLeft,kObjTypeText);

		//pvp
		//beginScene("menu_pvp");
		//addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial6),kCCTextAlignmentLeft,kObjTypeText);
		//addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial7),kCCTextAlignmentRight,kObjTypeText);

		//헬모드
		beginScene("menu_hellmode");
		addScript("kt_cha_katsuragi_1.png",getLocal(LK::kMyLocalKey_kindTutorial35),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial36),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial37),kCCTextAlignmentRight,kObjTypeText);
		
		//신발아이템
		beginScene("item_9");
		//addSpot("itemPannel","rect",1.f,kObjTypeNone);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kMyLocalKey_kindTutorial17),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial18),kCCTextAlignmentRight,kObjTypeText);

		
		//더블아이템
		beginScene("item_6");
		//addSpot("itemPannel","rect",1.f,kObjTypeNone);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kMyLocalKey_kindTutorial19),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial20),kCCTextAlignmentRight,kObjTypeText);
		
		
		//자석아이템
		beginScene("item_11");
		//addSpot("itemPannel","rect",1.f,kObjTypeNone);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kMyLocalKey_kindTutorial21),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial22),kCCTextAlignmentRight,kObjTypeText);
		
		//랜덤아이템
		beginScene("item_random");
		//addSpot("item_random","rect",1.f,kObjTypeNone);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial8),kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_asuka_1.png",getLocal(LK::kMyLocalKey_kindTutorial9),kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png",getLocal(LK::kMyLocalKey_kindTutorial10),kCCTextAlignmentRight,kObjTypeText);
		

	}
	
	void beginScene(string _storyID){
		//string cliLang = MyLocal::sharedInstance()->getLocalCode()->getCString();
		//CCLOG("Scene %s,",this->storyID.c_str());
		if(this->storyID == _storyID){
			isAdd=true;
		}else{
			isAdd=false;
		}
	}
	
	void addTitle(string title,ObjType isClear=kObjTypeAll){
		if(isAdd){
			Json::Value scriptInfo;
			scriptInfo["type"]=kScriptTypeTitle;
			scriptInfo["title"]=title;
			scriptInfo["clear"]=(int)isClear;
			storyData.append(scriptInfo);
		}
	}
	
	void addSpot(string stringdata,string rectType,float delay=0.5f,ObjType isClear=kObjTypeAll){
		if(isAdd){
			Json::Value scriptInfo;
			scriptInfo["type"]=kScriptTypeSpot;
			scriptInfo["stringdata"]=stringdata;
			scriptInfo["clear"]=(int)isClear;
			scriptInfo["spotType"]=rectType;
			scriptInfo["delay"]=delay;
			storyData.append(scriptInfo);
		}
	}
	void addScript(string image,string script,CCTextAlignment align,int isClear=kObjTypeNone,BoxType bt=kBoxBig){
		 //CCLOG("%s\t%s",image.c_str(),script.c_str());
		if(isAdd){
           // CCLOG("add script %s",script.c_str());
			Json::Value scriptInfo;
			scriptInfo["type"]=kScriptTypeTalk;
			scriptInfo["image"]=image;
			scriptInfo["script"]=script;
			scriptInfo["align"]=(int)align;
			
			scriptInfo["box"]=(int)bt;
			scriptInfo["clear"]=(int)isClear;
			storyData.append(scriptInfo);
		}
	}
	
	void addImage(string image,CCTextAlignment align,int isClear=kObjTypeNone,BoxType bt=kBoxBig){
		if(isAdd){
			Json::Value scriptInfo;
			scriptInfo["type"]=kScriptTypeImage;
			scriptInfo["image"]=image;
			scriptInfo["align"]=(int)align;
			
			scriptInfo["box"]=(int)bt;
			scriptInfo["clear"]=(int)isClear;
			storyData.append(scriptInfo);
		}
	}
		
	void scriptTypeTalk(Json::Value script){
        
        TRACE();
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
        TRACE();
		
		if(script["image"].asString()!=""){
			CCSprite* actor=nullptr;
			CCArray* ch = back->getChildren();
            TRACE();
			bool isOldObj = false;
			if(ch){
				for(int i=0;i<ch->count();i++){
					CCNode* n = (CCNode*)ch->objectAtIndex(i);
					if(n->getStringData()==script["image"].asString()){
						actor = (CCSprite*)n;
						isOldObj=true;
					}
				}
			}
            TRACE();
			if(!actor){
				actor = CCSprite::create(script["image"].asString().c_str());
				actor->setStringData(script["image"].asString().c_str());
				back->addChild(actor, 10);
				CCLOG("addChild actor %s", actor->getStringData().c_str());
			}
			
			
            TRACE();
			if(!isOldObj){
				if(script["align"].asInt()==kCCTextAlignmentLeft){
					actor->setAnchorPoint(ccp(0,0));
					actor->setPosition(ccp(240-240*screen_scale_x-actor->getContentSize().width , 160-160*screen_scale_y));
					actor->setTag(kObjTypeActorLeft);
				}
				else if(script["align"].asInt()==kCCTextAlignmentRight){
					actor->setAnchorPoint(ccp(1,0));
					actor->setPosition(ccp(240+240*screen_scale_x+actor->getContentSize().width , 160-160*screen_scale_y));
					actor->setTag(kObjTypeActorRight);
				}else{
					actor->setAnchorPoint(ccp(1,0));
					actor->setPosition(ccp(240, 160-160*screen_scale_y-500));
					actor->setTag(kObjTypeActorCenter);
				}
				
                TRACE();
				
				back->addChild(KSGradualValue<float>::create(0.f,1.f,0.3f,[actor,screen_scale_x,screen_scale_y,script](float t){
					if(script["align"].asInt()==kCCTextAlignmentLeft)
						actor->setPositionX(240-240*screen_scale_x-actor->getContentSize().width + actor->getContentSize().width*2.f/3.f*t);
					else if(script["align"].asInt()==kCCTextAlignmentRight)
						actor->setPositionX(240+240*screen_scale_x+actor->getContentSize().width - actor->getContentSize().width*2.f/3.f*t);
					else
						actor->setPosition(ccp(240, 160-160*screen_scale_y-500+500*t));
					
				},[actor](float t){
					
					CCLog("action pos is %f",actor->getPositionX());
					
				}));
			}
			
            TRACE();
		}
		
		
		TypingBox* typing_box;
		
        TRACE();
        CCLOG("test1 %d",this->storySeq);
		if(script["box"].asInt()==kBoxBig){
			if(script["align"].asInt()==kCCTextAlignmentLeft){
				typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(241, 78));
			}else if(script["align"].asInt()==kCCTextAlignmentRight){
				typing_box = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 60), ccp(239, 96));
			}else{
				typing_box = TypingBox::create(-9999, "kt_talkbox_orange.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(240, 60), ccp(240, 96));
				
//				TypeingBox::create(int t_touch_priority, string t_box_filename, cocos2d::CCRect t_box_9_out, cocos2d::CCRect t_box_9_in, cocos2d::CCRect t_box_in_label, cocos2d::CCSize t_box_in_size, cocos2d::CCPoint t_box_position)
			}
		}else{
			typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(7, 10, 23, 64), CCSizeMake(210, 60), ccp(241, 78));
		}
		
		//TypingBox::create(int t_touch_priority, string t_box_filename, cocos2d::CCRect t_box_9_out, cocos2d::CCRect t_box_9_in, cocos2d::CCRect t_box_in_label, cocos2d::CCSize t_box_in_size, cocos2d::CCPoint t_box_position)
        TRACE();
		typing_box->setTag(kObjTypeText);
		back->addChild(typing_box, 11);
		
        TRACE();
        CCLOG("%s",script.toStyledString().c_str());
		typing_box->startTyping(script["script"].asString().c_str(),[this](){

            this->addChild(KSTimer::create(0.1,[this](){
                TRACE();
                CCLOG("test2-0 %d",this->storySeq);
                this->clearScript();
                CCLOG("test2-1 %d",this->storySeq);
                TRACE();
                
                CCLOG("test3-1 %d",this->storySeq);
                this->playScript();
                CCLOG("test3-2 %d",this->storySeq);
                TRACE();
                TRACE();
                CCLOG("test2-2 %d",this->storySeq);

            }));
           		});
        TRACE();
		typing_box->setTouchOffScrollAndButton();
	
	}
	
	void scriptTypeTitle(Json::Value script){
		
		StyledLabelTTF* titlelbl = StyledLabelTTF::create(script["title"].asString().c_str(), mySGD->getFont().c_str(), 20, 000, StyledAlignment::kCenterAlignment);
		titlelbl->setPosition(ccp(240+480,160));
		titlelbl->setTag(kObjTypeTitle);
		back->addChild(titlelbl);
		
		
		addChild(KSGradualValue<float>::create(0, 460, 0.5, [&titlelbl,this](float a){
			titlelbl->setPositionX(240+480-a);
		}, [&titlelbl,this](int a){
			addChild(KSGradualValue<float>::create(0, 40, 1.5, [&titlelbl,this](float a){
				titlelbl->setPositionX(240+480-460-a);
			}, [&titlelbl,this](int a){
				addChild(KSGradualValue<float>::create(0, 460, 0.5, [&titlelbl,this](float a){
					titlelbl->setPositionX(240+480-460-40-titlelbl->getPositionX()-a);
				}, [&titlelbl,this](int a){
					this->clearScript();
				}));
			}));
		}));
	}
	
	void scriptTypeSpot(Json::Value script){
		CCNode* obj =StoryLayer::findObject(CCDirector::sharedDirector()->getRunningScene(),script["stringdata"].asString());
		CCPoint pos =  obj->getParent()->convertToWorldSpace(obj->getPosition());
		CCSprite* spotObj;
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		float uitop = myDSH->ui_top;
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;

		
		
		if(script.get("spotType","circle").asString()=="circle"){
			CCDrawNode* dot = CCDrawNode::create();
			
			dot->drawDot(ccp(pos.x+screen_scale_x/2.f,pos.y*(320.f/uitop)),obj->getContentSize().width/2.f, ccc4f (1, 1, 1, 1));
			back->addChild(dot);
			dot->setBlendFunc(ccBlendFunc{GL_DST_COLOR, GL_ONE});
			dot->setAnchorPoint(ccp(0.5f,0.5f));
			dot->setTag(kObjTypeSpot);
			
			spotObj = (CCSprite*)dot;
		}else if(script.get("spotType","circle").asString()=="rect"){
			//CCDrawNode * dot = CCDrawNode::create();
			//dot->draw // drawDot(pos,obj->getContentSize().width/2.f, ccc4f (1, 1, 1, 1));
			
			
			CCSprite* dot = CCSprite::create("whitePaper.png", CCRectMake(0, 0, obj->getContentSize().width, obj->getContentSize().height));
			dot->setPosition(ccp(pos.x/screen_scale_x + (screen_size.width-960.f)/2.f,pos.y*(320.f/uitop)));
			back->addChild(dot);
			dot->setBlendFunc(ccBlendFunc{GL_DST_COLOR, GL_ONE});
			dot->setAnchorPoint(ccp(0.5f,0.5f));
			dot->setTag(kObjTypeSpot);
			
			spotObj = (CCSprite*)dot;
			//spotObj->setOpacity(0);
		}
		
		spotObj->setScale(0.1);
		float delay = script.get("delay",0.5).asFloat();
		addChild(KSGradualValue<float>::create(0.1, 1, 0.3, [this,spotObj](float a){
			spotObj->setScale(a);
			//spotObj->setOpacity(a*255);
		},[this,delay](float a){
			addChild(KSTimer::create(delay,[this](){
				this->clearScript();
				this->playScript();
			}));
		}));
		

	}
	
	void scriptTypeImage(Json::Value script){
		
		
		CCSprite* img = CCSprite::create(script["image"].asString().c_str());
		img->setTag(kObjTypeImage);
		img->setScale(0.1);
		img->setOpacity(0);
		
		
		float xPo=240;
		if(script["align"].asInt()==kCCTextAlignmentLeft){
			xPo=120;
		}else if(script["align"].asInt()==kCCTextAlignmentLeft){
			xPo=360;
		}
		img->setPosition(ccp(xPo,myDSH->ui_top/2.f));
		back->addChild(img);
		
		
		addChild(KSGradualValue<float>::create(0.1, 1, 0.3, [this,img](float a){
			img->setScale(a);
			img->setOpacity(a*255);
		},[this,img](float a){
			addChild(KSTimer::create(0.5,[this](){
				this->clearScript();
				this->playScript();
			}));
		}));
		

		
	}
	

	
	
	int m_sss;
	void playScript(){
        TRACE();
        CCLOG("test3-1 %d",this->storySeq);
        TRACE();
		if(storySeq>=storyData.size()){
			
            TRACE();
			CCLOG("clear last1 %s",this->storyID.c_str());
            TRACE();
			addChild(KSGradualValue<float>::create(0, 50, 0.5, [this](int a){
                CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
                float screen_scale_x = screen_size.width/screen_size.height/1.5f;
                if(screen_scale_x < 1.f)
                    screen_scale_x = 1.f;
                float ui_top = myDSH->ui_top;
                float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
                
                bottom->setPositionY(-(screen_scale_y*(320)-320)/2.f-a);
                top->setPositionY(320+(screen_scale_y*(320)-320)/2.f-50+a);
				black->setOpacity(150-a*3.f);
			}, [this](int a){
				CCLOG("all finish");
				if(callbackFunc)callbackFunc();
				this->removeFromParent();
				
			}));
            TRACE();
			CCLOG("clear last2 %s",this->storyID.c_str());
            TRACE();
			return;
		}
		
		
        CCLOG("get script");
		Json::Value script = storyData[storySeq];
		
		
		if(script["type"].asInt()==kScriptTypeTalk){
			scriptTypeTalk(script);
		}else if(script["type"].asInt()==kScriptTypeTitle){
			scriptTypeTitle(script);
		}else if(script["type"].asInt()==kScriptTypeSpot){
			scriptTypeSpot(script);
		}else if(script["type"].asInt()==kScriptTypeImage){
			scriptTypeImage(script);
		}
	}
	

	void clearScript(){
        int nowSeq = this->storySeq;
        CCLOG("test4-1 %d",nowSeq);
		Json::Value script = storyData[nowSeq];
        CCLOG("test4-2 %d",nowSeq);
        
        TRACE();
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
        CCLOG("test4-3 %d",nowSeq);
        TRACE();
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCArray *cr = back->getChildren();
		
        CCLOG("test4-4 %d",nowSeq);
		
        TRACE();
		for(int i=0;i<cr->count();i++){
			CCNode *obj = (CCNode*)cr->objectAtIndex(i);
			
            TRACE();
			if(obj->getTag()==kObjTypeText && script["clear"].asInt()&kObjTypeText){
				CCLOG("remove text");
                CCLOG("test4-5 %d",nowSeq);
				obj->removeFromParent();
				
			}else if((obj->getTag()==kObjTypeActorLeft && script["clear"].asInt()&kObjTypeActorLeft) || (obj->getTag()==kObjTypeActorRight && script["clear"].asInt()&kObjTypeActorRight)){
                
                
                CCLOG("test4-6 %d",nowSeq);
				
				CCLOG("move actor %s", obj->getStringData().c_str());
				back->addChild(KSGradualValue<float>::create(1.f,0.f,0.3f,[this,obj,screen_scale_x](float t){
					CCNode *myobj = obj;
					if(myobj->getAnchorPoint().x==0.f){
						myobj->setPositionX(240-240*screen_scale_x-obj->getContentSize().width + obj->getContentSize().width*2.f/3.f*t);
					}else{
						myobj->setPositionX(240+240*screen_scale_x+obj->getContentSize().width - obj->getContentSize().width*2.f/3.f*t);
					}
					
				},[this,obj](float t){
						CCNode *myobj = obj;
						CCLOG("remove actor %s",myobj->getStringData().c_str());
						myobj->removeFromParent();
				}));
			}else if(obj->getTag()==kObjTypeTitle && script["clear"].asInt()&kObjTypeTitle){
                
                CCLOG("test4-7 %d",nowSeq);
				CCLOG("remove title");
				obj->removeFromParent();
			}else if(obj->getTag()==kObjTypeSpot && script["clear"].asInt()&kObjTypeSpot){
				
                CCLOG("test4-8 %d",nowSeq);
                CCLOG("remove spot");
				obj->removeFromParent();
			}else if(obj->getTag()==kObjTypeImage && script["clear"].asInt()&kObjTypeImage){
				
				CCLOG("test4-8 %d",nowSeq);
				CCLOG("remove spot");
				obj->removeFromParent();
			}
			
            TRACE();
			
		}
        
        CCLOG("test4-9 %d",nowSeq);
        this->storySeq++;
        TRACE();
		
	}
	
	static CCNode* findObject(CCNode* obj,string tagname){
		if(!obj || tagname=="")return nullptr;
		
		if(obj->getStringData()==tagname){
			return obj;
		}
		
		CCArray* children = obj->getChildren();
		if(children==nullptr)return nullptr;
		
		for(int i=0;i<children->count();i++){
			CCNode* child = (CCNode*)children->objectAtIndex(i);
			CCNode* objs = StoryLayer::findObject(child,tagname);
			if(objs!=nullptr)return objs;
		}
		
		return nullptr;
	}
	
};


#endif /* defined(__DGproto__ScenarioScene__) */
