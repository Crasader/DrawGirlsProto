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
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


class StoryLayer : public CCLayer{
public:
	string storyID;
	CCLayerColor *back;
	CCLayerColor *top;
	CCLayerColor *bottom;
	TouchSuctionLayer *suction;
	static StoryLayer* create(string storyID)
	{
		StoryLayer* obj = new StoryLayer();
		obj->init(storyID);
		obj->autorelease();
		return obj;
	}
	
	bool init(string storyID){
		if(!CCLayer::init()){
			return false;
		}
		
		suction = TouchSuctionLayer::create(-10000);
		addChild(suction);
		
		suction->setTouchEnabled(true);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		

		top = CCLayerColor::create(ccc4(0,0,0,255), screen_size.width,50);
		top->setAnchorPoint(ccp(0,0));
		top->setPosition(ccp(0,myDSH->ui_top));
		addChild(top);
		
		bottom = CCLayerColor::create(ccc4(0,0,0,255), screen_size.width,50);
		bottom->setAnchorPoint(ccp(0,0));
		bottom->setPosition(ccp(0,0-50));
		addChild(bottom);
		
		this->addChild(KSGradualValue<float>::create(0.f,50.f,0.5f,[=](float t){
			bottom->setPositionY(t-50);
			top->setPosition(ccp(0,myDSH->ui_top-t));
			
		},[](float t){}));
		
				
		back = CCLayerColor::create(ccc4(0,0,0,0));
		addChild(back);
		
		
		return true;
	}
	
	enum BoxType{
		kBoxBig,
		kBoxMiddle,
		kBoxSmall
	};
	

	
	enum ObjType{
		kObjTypeNone=0,
		kObjTypeActor=1,
		kObjTypeText=2,
		kObjTypeTitle=4,
		kObjTypeSpot=8,
		kObjTypeAll=15
	};
	
	enum scriptType{
		kScriptTypeTalk = 1,
		kScriptTypeTitle = 2,
		kScriptTypeSpot = 3
	};
	
	Json::Value storyData;
	int storySeq;
	function<void(void)> callbackFunc;
	static void startStory(string storyID,function<void(void)> endFunc){
		
		StoryLayer *sl  = StoryLayer::create(storyID);
		((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(sl,9999999);
		sl->storyID = storyID;
		sl->storySeq=0;
		sl->m_sss=0;
		sl->callbackFunc = endFunc;
		sl->storyData = Json::Value(Json::arrayValue);
		sl->loadStory();
		sl->addChild(KSGradualValue<float>::create(0, 150, 1, [=](int a){
			sl->back->setOpacity(a);
		}, [=](int a){
			sl->play();
		}));
	}
	
	void loadStory(){
		// 인물 / 대사 / 인물위치 / 박스종류 / 폰트사이즈
		
		addTitle("ko","puzzle1","<font color=#ffff00 size=30 strokesize=1 strokecolor=#ffffff>1장 어둠의 그림자</font>",kObjTypeAll);
		addSpot("ko","puzzle1","testobj",kObjTypeNone);
		addScript("ko","puzzle1","kt_cha_hibari_1.png",myLoc->getLocalForKey(kMyLocalKey_scenarioMent10),kCCTextAlignmentRight,kBoxBig,kObjTypeAll);
		addScript("ko","puzzle1","",myLoc->getLocalForKey(kMyLocalKey_scenarioMent11),kCCTextAlignmentRight,kBoxBig,kObjTypeAll);
		addScript("ko","puzzle1","kt_cha_yagyu_1.png",myLoc->getLocalForKey(kMyLocalKey_scenarioMent12),kCCTextAlignmentLeft,kBoxBig,kObjTypeAll);
		addScript("ko","puzzle1","kt_cha_asuka_1.png",myLoc->getLocalForKey(kMyLocalKey_scenarioMent13),kCCTextAlignmentRight,kBoxBig,kObjTypeAll);
		addScript("ko","puzzle1","kt_cha_hibari_1.png",myLoc->getLocalForKey(kMyLocalKey_scenarioMent14),kCCTextAlignmentLeft,kBoxBig,kObjTypeAll);
	}
	
	void addTitle(string lang,string storyID,string title,ObjType isClear=kObjTypeAll){
		if(this->storyID == storyID && lang == MyLocal::sharedInstance()->getLocalCode()->getCString()){
			Json::Value scriptInfo;
			scriptInfo["type"]=kScriptTypeTitle;
			scriptInfo["title"]=title;
			scriptInfo["clear"]=(int)isClear;
			storyData.append(scriptInfo);
		}
	}
	
	void addSpot(string lang,string storyID,string stringdata,ObjType isClear=kObjTypeAll){
		if(this->storyID == storyID && lang == MyLocal::sharedInstance()->getLocalCode()->getCString()){
			Json::Value scriptInfo;
			scriptInfo["type"]=kScriptTypeSpot;
			scriptInfo["stringdata"]=stringdata;
			scriptInfo["clear"]=(int)isClear;
			storyData.append(scriptInfo);
		}
	}
	void addScript(string lang,string storyID,string image,string script,CCTextAlignment align,BoxType bt=kBoxBig,ObjType isClear=kObjTypeNone){
		if(this->storyID == storyID && lang == MyLocal::sharedInstance()->getLocalCode()->getCString()){
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
		
	void scriptTypeTalk(Json::Value script){
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		
		if(script["image"].asString()!=""){
			CCSprite* actor = CCSprite::create(script["image"].asString().c_str());
			actor->setTag(kObjTypeActor);
			actor->setStringData(GraphDogLib::random_string(5));
			CCLOG("addChild actor %s", actor->getStringData().c_str());
			back->addChild(actor, 10);
			
			if(script["align"].asInt()==kCCTextAlignmentLeft){
				actor->setAnchorPoint(ccp(0,0));
				actor->setPosition(ccp(240-240*screen_scale_x-actor->getContentSize().width , 160-160*screen_scale_y));
			}
			else {
				actor->setAnchorPoint(ccp(1,0));
				actor->setPosition(ccp(240+240*screen_scale_x+actor->getContentSize().width , 160-160*screen_scale_y));
			}
			
			back->addChild(KSGradualValue<float>::create(0.f,1.f,0.3f,[=](float t){
				if(script["align"].asInt()==kCCTextAlignmentLeft)
					actor->setPositionX(240-240*screen_scale_x-actor->getContentSize().width + actor->getContentSize().width*2.f/3.f*t);
				else
					actor->setPositionX(240+240*screen_scale_x+actor->getContentSize().width - actor->getContentSize().width*2.f/3.f*t);
				
			},[=](float t){
				
				CCLog("action pos is %f",actor->getPositionX());
				
			}));
			
		}
		
		
		TypingBox* typing_box;
		
		if(script["box"].asInt()==kBoxBig){
			if(script["align"].asInt()==kCCTextAlignmentLeft){
				typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(241, 78));
			}else{
				typing_box = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 60), ccp(239, 96));
			}
		}else{
			typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(241, 78));
		}
		typing_box->setTag(kObjTypeText);
		back->addChild(typing_box, 11);
		
		typing_box->startTyping(script["script"].asString().c_str(),[=](){
			this->clear(script);
			this->play();
		});
		typing_box->setTouchOffScrollAndButton();
	
	}
	
	void scriptTypeTitle(Json::Value script){
		
		StyledLabelTTF* titlelbl = StyledLabelTTF::create(script["title"].asString().c_str(), mySGD->getFont().c_str(), 20, 000, StyledAlignment::kCenterAlignment);
		titlelbl->setPosition(ccp(240+480,160));
		titlelbl->setTag(kObjTypeTitle);
		back->addChild(titlelbl);
		
		
		addChild(KSGradualValue<float>::create(0, 460, 0.5, [=](float a){
			titlelbl->setPositionX(240+480-a);
		}, [=](int a){
			addChild(KSGradualValue<float>::create(0, 40, 1.5, [=](float a){
				titlelbl->setPositionX(240+480-460-a);
			}, [=](int a){
				addChild(KSGradualValue<float>::create(0, 460, 0.5, [=](float a){
					titlelbl->setPositionX(240+480-460-40-titlelbl->getPositionX()-a);
				}, [=](int a){
					this->clear(script);
					this->play();
				}));
			}));
		}));
	}
	
	void scriptTypeSpot(Json::Value script){
		CCNode* obj =StoryLayer::findObject(CCDirector::sharedDirector()->getRunningScene(),script["stringdata"].asString());
		CCPoint pos =  obj->getParent()->convertToWorldSpace(obj->getPosition());
		if(obj)obj->runAction(CCBlink::create(0.5f, 2));
		
		CCDrawNode * dot = CCDrawNode::create();
		dot->drawDot(pos,obj->getContentSize().width/2.f, ccc4f (1, 1, 1, 1));
		back->addChild(dot);
		dot->setBlendFunc(ccBlendFunc{GL_DST_COLOR, GL_ONE});
		dot->setAnchorPoint(ccp(0.5f,0.5f));
		dot->setTag(kObjTypeSpot);
		
		addChild(KSTimer::create(0.3,[=](){
			this->clear(script);
			this->play();
		}));
	}

	
	int m_sss;
	void play(){
		if(storySeq>=storyData.size()){
			
			CCLOG("clear last");
			
			addChild(KSGradualValue<float>::create(0, 50, 0.5, [=](int a){
				bottom->setPosition(ccp(0,-a));
				top->setPosition(ccp(0,myDSH->ui_top-50+a));
				back->setOpacity(150-a*3.f);
			}, [=](int a){
				CCLOG("all finish");
				if(callbackFunc)callbackFunc();
				this->removeFromParent();
				
			}));
			return;
		}
		
		
		Json::Value script = storyData[storySeq];
		storySeq++;
		
		if(script["type"].asInt()==kScriptTypeTalk){
			scriptTypeTalk(script);
		}else if(script["type"].asInt()==kScriptTypeTitle){
			scriptTypeTitle(script);
		}else if(script["type"].asInt()==kScriptTypeSpot){
			scriptTypeSpot(script);
		}
	}
	

	void clear(Json::Value script){
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCArray *cr = back->getChildren();
		
		
		for(int i=0;i<cr->count();i++){
			CCNode *obj = (CCNode*)cr->objectAtIndex(i);
			
			if(obj->getTag()==kObjTypeText && script["clear"].asInt()&kObjTypeText){
				CCLOG("remove text");
				obj->removeFromParent();
				
			}else if(obj->getTag()==kObjTypeActor && script["clear"].asInt()&kObjTypeActor){
				
				CCLOG("move actor %s", obj->getStringData().c_str());
				back->addChild(KSGradualValue<float>::create(1.f,0.f,0.3f,[=](float t){
					CCNode *myobj = obj;
					if(myobj->getAnchorPoint().x==0.f){
						myobj->setPositionX(240-240*screen_scale_x-obj->getContentSize().width + obj->getContentSize().width*2.f/3.f*t);
					}else{
						myobj->setPositionX(240+240*screen_scale_x+obj->getContentSize().width - obj->getContentSize().width*2.f/3.f*t);
					}
					
				},[=](float t){
						CCNode *myobj = obj;
						CCLOG("remove actor %s",myobj->getStringData().c_str());
						myobj->removeFromParent();
				}));
			}else if(obj->getTag()==kObjTypeTitle && script["clear"].asInt()&kObjTypeTitle){
				CCLOG("remove title");
				obj->removeFromParent();
			}else if(obj->getTag()==kObjTypeSpot && script["clear"].asInt()&kObjTypeSpot){
				CCLOG("remove spot");
				obj->removeFromParent();
			}
				
			
		}
		
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
