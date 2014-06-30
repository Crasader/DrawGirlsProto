//
//  FormSetter.h
//  DGproto
//
//  Created by LitQoo on 2014. 3. 24..
//
//

#ifndef __DGproto__FormSetter__
#define __DGproto__FormSetter__

#include <iostream>
#include "GraphDog.h"
#include "KSUtil.h"
#include "CommonButton.h"
#include "KSLabelTTF.h"
#include "KSUtil.h"
#include <vector>
#include <stdio.h>
#define startFormSetter(obj) FormSetter::get()->start(obj,__FILE__)
#define setFormSetter(name) name->setStringData(#name);
#define setFormSetterGuide(filename) FormSetter::get()->setGuideImage(filename);
/*******************************************************
오브젝트 배치하기의 슈퍼초 울트라 캡 혁신
 *******************************************************
 
 2.0v - 자체 리모컨으로
 1.조절할 오브젝트추가 setFormSetter(obj);
 2.init함수에 리모콘코드 추가 startFormSetter(this);
 3.실행후 해당씬에서 디스플레이 오른쪽 아래를 터치하면 리모콘이 뜬다. 조절후 log버튼을 눌러보시라
 4.조절끝나면 소스코드삭제도 필요없음 나중에 FormSetter::get()->setEnabledRemocon(false) 한번만 호출해주면 있는 리모콘 모두 꺼짐.
 
 
 1.5v - 서버연동 버전 사용법2
 1.조절할 오브젝트에 이름추가 obj->setStringData("objName");
 2.init함수 마지막에 서버호출코드 추가  FormSetter::get()->requestFormData();
 3.웹어드민에 1번에서 추가한 이름으로 등록하여 사용가능.
 4.조절끝나면 2번 코드만 삭제하면 끝
 

 1.0v  - 서버연동 버전 사용법

이제 오브젝트 배치한다고 포지션잡고 실행하고 포지션 잡고 실행해보고.. 의 무한 반복 작업을 하지 맙시다~!

배치할 페이지를 켜놓고 웹에서 위치를 수정하면 실시간으로 오브젝트의 위치,스케일 등이 수정됩니다.

 
 
 
간단사용법
 1.웹어드민에 id를 등록하고 대충 x,y,scale값을 일단 대충 적습니다.
 2.조절할 오브젝트를 추가해줍니다.
	FormSetter::get()->addObject("id",object); //웹어드민에 등록한 id , 조절할 오브젝트(CCNode*)
 3.오브젝트가 제거될땐(씬전환시) FormSetter::get()->removeObject("id"); 로 제거해준다.
 4.이제 실행해서 해당페이지 펼쳐놓고 웹어드민 설정하면서 최적화된 값을 찾습니다.
 5.값을 찾았으면 2번의 소스를 지우고 setPosition이용해 코딩합니다.
 
 
 
 
복잡사용법 - 애니메이션 연출이 들어갈경우 시작위치와 끝 위치 등을 정해줘야 할때가 있다. 그럴땐...
 
 //뭔가 init함수가 있다고 치자.
 bool init(){
 
	 …
	 
	 //포지션 갱신간격을 10초로 늘린다.
	 FormSetter::get()->setDelay(10.f);
	 
	 //ttt오브젝트를 추가하고 포지션갱신후 실행할 func를 설정한다.
	 FormSetter::get()->addObject("ttt",ttt,[this](Json::Value p){
		 //포지션이 갱신되었을때 애니메이션을 재생한다, 즉 10초에 한번 애니메이션 연출을 시작하는거임
		 this->startAnimation();
	 });
	 
	 …
 
 }
 
 //애니메이션 시작함수
 void startAnimation(){
	 //가장 최근에 받아온 “ttt”의 데이터를 받아온다
	 Json::Value p = FormSetter::get()->getFormData(“ttt”);
	 
	 //미리설정해놓은 애니메이션 위치 (endPosition)을 받아온다
	 float endPo = p.get(“endPosition”,0).asFloat();
	 
	 //받아온값만큼 움직인다.
	 ttt->runAction(CCMoveTo::create(ccp(0,endPo),5.f);
 }
 
 
한번만 값 불러오고싶을땐
 
 FormSetter::get()->requestFormDataOnce([](){
	Json::Value v =  FormSetter::get()->getFormData("bustmorphing");
	Json::FastWriter fw;
	string a = fw.write(v);
	CCLOG("ttt data is %s",a.c_str());
 });
 
*/
class TouchCancelLayer : public CCLayer{
public:
	bool init(){
		if(!CCLayer::init()){
			return false;
		}
		
		return true;
	}
	
	static TouchCancelLayer* create(){
		TouchCancelLayer* obj = new TouchCancelLayer();
		obj->init();
		obj->autorelease();
		return obj;
	}
	
	bool ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
	{
		//CCLog("sw touch");
		return true;
	}
	
	
	void ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
	{
		
		
	}
	
	void ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
	{
		
	}
	
	void ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
	{
		
	}
	
	void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, (INT_MAX-10)*-1, true);
	}
	
};
class FormSetter : public CCLayer{
public:
	CCNode* remocon;
	
	struct FormSetterData{
		Json::Value data;
	//	CCNode* obj;
		vector<CCNode*> objects;
		vector<std::function<void(Json::Value)>> funcs;
	};
	
	struct FormSetterObj{
		CCNode* obj;
		bool isEdited;
		Json::Value originalData;
	};
	
	bool m_is_sch;
	bool m_is_once;
	int m_mode;
	int m_selectedObjNumber;
	CCPoint m_startPosition;
	vector<FormSetterObj> m_objList;
	map<string,FormSetterData> m_list;
	float m_delay;
	std::function<void(void)> m_funcAtReceived;
	std::function<void(void)> m_funcAtReceivedOnce;
	KSLabelTTF* m_objName;
	KSLabelTTF* m_objInfo;
	CommonButton* m_modeBtn;
	CommonButton* m_resetBtn;
	CommonButton* m_pauseBtn;
	TouchCancelLayer* m_swLayer;
	CCSprite* m_guideLine;
	CCSprite* m_guideImage;
	bool m_isEnabledRemocon;
	static FormSetter* get()
	{
		static FormSetter* _ins = 0;
		if(_ins == 0)
		{
			_ins = new FormSetter();
			_ins->retain();
		}
		
		return _ins;
	}
	
	
	FormSetter(){
		m_guideImage=nullptr;
		m_is_sch=false;
		m_delay = 1.f;
		m_funcAtReceived = nullptr;
		m_funcAtReceivedOnce = nullptr;
		m_mode=0;
		remocon = CCNode::create();
		remocon->retain();
		m_objList.clear();
		m_selectedObjNumber=-1;
		m_isEnabledRemocon = false;
		CommonButton* next = CommonButton::create("next", 13, CCSizeMake(50, 50), CommonButtonOrange, (INT_MAX-3)*-1);
		next->setFunction([this](CCObject *){
				if(m_selectedObjNumber>=0){
				m_selectedObjNumber++;
				if(m_selectedObjNumber>=m_objList.size())m_selectedObjNumber=0;
				CCNode* selectedObj = (CCNode*)m_objList[m_selectedObjNumber].obj;
				selectedObj->runAction(CCBlink::create(0.5f, 3));
				setInfomation();
			}
		});
		next->setPosition(50,0);
		CommonButton* prev = CommonButton::create("prev", 13, CCSizeMake(50, 50), CommonButtonOrange, (INT_MAX-3)*-1);
		prev->setFunction([this](CCObject *){
			if(m_selectedObjNumber>=0){
				m_selectedObjNumber--;
				if(m_selectedObjNumber<0)m_selectedObjNumber = m_objList.size()-1;
				CCNode* selectedObj = (CCNode*)m_objList[m_selectedObjNumber].obj;
				selectedObj->runAction(CCBlink::create(0.5f, 3));
				setInfomation();
			}
		});
		prev->setPosition(0,0);
		
		CommonButton* showSetting = CommonButton::create("log", 13, CCSizeMake(50, 50), CommonButtonOrange, (INT_MAX-3)*-1);
		showSetting->setFunction([this](CCObject *){
			if(m_selectedObjNumber<0)return;
			logFormSetting();
			if(!m_objList[m_selectedObjNumber].isEdited)return;
			
			CCLog("################ FormSetter Selected : %s ##############START##",m_filename.c_str());
			logOnce(m_selectedObjNumber);
			CCLog("");
			CCLog("################ FormSetter Selected : %s ################END##",m_filename.c_str());
		});
		showSetting->setPosition(50,80);
		
		
		CommonButton* exit = CommonButton::create("exit", 13, CCSizeMake(50, 50), CommonButtonOrange, (INT_MAX-3)*-1);
		exit->setFunction([this](CCObject *){
			remocon->setVisible(false);
			m_guideLine->setVisible(false);
			m_swLayer->setTouchEnabled(false);
			if(m_guideImage)m_guideImage->setVisible(false);
			if(m_selectedObjNumber<0)return;
			
			logFormSetting();
			m_selectedObjNumber=-1;
		});
		
		exit->setPosition(-50,80);
		
		m_pauseBtn = CommonButton::create("||", 13, CCSizeMake(50, 50), CommonButtonOrange, (INT_MAX-3)*-1);
		m_pauseBtn->setFunction([this](CCObject *){
			if(CCDirector::sharedDirector()->isPaused()){
				CCDirector::sharedDirector()->resume();
				//cocos2d::CCDirector::sharedDirector()->startAnimation();
			}else{
				CCDirector::sharedDirector()->pause();
				//cocos2d::CCDirector::sharedDirector()->stopAnimation();
			}
			
		});
		
		m_pauseBtn->setPosition(100,80);

		
		m_modeBtn = CommonButton::create("mode", 13, CCSizeMake(50, 50), CommonButtonOrange, (INT_MAX-3)*-1);
		m_modeBtn->setFunction([this](CCObject *){
			if(m_selectedObjNumber<0)return;
			
			int mode = m_modeBtn->getTag();
			mode++;
			if(mode>5)mode=0;
			m_modeBtn->setTag(mode);
			
			setInfomation();
		});
		
		m_modeBtn->setPosition(0,40);
		m_modeBtn->setTag(0);
		
		m_resetBtn = CommonButton::create("reset", 13, CCSizeMake(50, 50), CommonButtonOrange, (INT_MAX-3)*-1);
		m_resetBtn->setFunction([this](CCObject *){
			if(m_selectedObjNumber<0)return;
			CCNode* obj =  m_objList[m_selectedObjNumber].obj;
			obj->setPosition(m_objList[m_selectedObjNumber].originalData["x"].asFloat(),m_objList[m_selectedObjNumber].originalData["y"].asFloat());
			obj->setContentSize(CCSizeMake(m_objList[m_selectedObjNumber].originalData["w"].asFloat(),m_objList[m_selectedObjNumber].originalData["h"].asFloat()));
			obj->setAnchorPoint(CCSizeMake(m_objList[m_selectedObjNumber].originalData["ax"].asFloat(),m_objList[m_selectedObjNumber].originalData["ay"].asFloat()));

			obj->setScaleX(m_objList[m_selectedObjNumber].originalData["sx"].asFloat());
			obj->setScaleY(m_objList[m_selectedObjNumber].originalData["sy"].asFloat());
			if(CCLabelTTF* checkobj = dynamic_cast<CCLabelTTF*>(obj)){
				checkobj->setFontSize(m_objList[m_selectedObjNumber].originalData["fontsize"].asFloat());
			}
			m_objList[m_selectedObjNumber].isEdited=false;
			setInfomation();
		});
		
		m_resetBtn->setBackgroundTypeForDisabled(CommonButtonGray);
		
		m_resetBtn->setPosition(100,40);
		
		m_objName = KSLabelTTF::create("title", 	mySGD->getFont().c_str(), 13, CCSizeMake(100, 20), kCCTextAlignmentLeft);
		m_objName->setPosition(ccp(30,-30));
		m_objName->enableOuterStroke(ccc3(0,0,0), 1);
		m_objName->setColor(ccc3(255,0,0));
		
		m_objInfo = KSLabelTTF::create("info", 	mySGD->getFont().c_str(), 13, CCSizeMake(100, 80), kCCTextAlignmentLeft);
		m_objInfo->setVerticalAlignment(kCCVerticalTextAlignmentTop);
		m_objInfo->setPosition(ccp(30,-75));
		m_objInfo->enableOuterStroke(ccc3(0,0,0), 1);
		
		
		remocon->addChild(m_objName,10);
		remocon->addChild(m_objInfo,10);
		remocon->addChild(m_modeBtn,10);
		remocon->addChild(m_resetBtn,10);
		remocon->addChild(next,10);
		remocon->addChild(prev,10);
		remocon->addChild(exit,10);
		remocon->addChild(showSetting,10);
		remocon->addChild(m_pauseBtn,10);
		
		remocon->setPosition(240,160);
		this->addChild(remocon, 100);
		remocon->setVisible(false);
		
		m_swLayer = TouchCancelLayer::create();
		this->addChild(m_swLayer);
		m_swLayer->setTouchEnabled(false);
		
		m_guideLine = CCSprite::create("formsetter_guide.png");
		m_guideLine->setPosition(ccp(-240,160));
		this->addChild(m_guideLine,1);
		m_guideLine->setVisible(false);
	}
	
	void logFormSetting(){
		if(m_selectedObjNumber<0)return;
		CCLog("");
		CCLog("");
		CCLog("");
		CCLog("");
		CCLog("################ FormSetter Result : %s ################START##",m_filename.c_str());
		for(int i=0;i<m_objList.size();i++){
			logOnce(i);
		}
		CCLog("");
		CCLog("################ FormSetter Result : %s ##################END##",m_filename.c_str());
	}
	
	void logOnce(int i){
		CCNode* obj = m_objList[i].obj;
		if(!m_objList[i].isEdited)return;
		CCLog("");
		
		if(m_objList[i].originalData["x"].asFloat()!=obj->getPosition().x || m_objList[i].originalData["y"].asFloat()!=obj->getPosition().y)
			CCLog("%s->setPosition(ccp(%.1f, %.1f)); \t\t\t// dt (%.1f, %.1f)",
						obj->getStringData().c_str(),
						obj->getPosition().x,
						obj->getPosition().y,
						obj->getPosition().x-m_objList[i].originalData["x"].asFloat(),
						obj->getPosition().y-m_objList[i].originalData["y"].asFloat()
						);
		if(m_objList[i].originalData["sx"].asFloat()!=obj->getScaleX() || m_objList[i].originalData["sy"].asFloat()!=obj->getScaleY()){
			if(obj->getScaleX()==obj->getScaleY())
				CCLog("%s->setScale(%.1f); \t\t\t// dt (%.1f,%.1f)",
							obj->getStringData().c_str(),
							(int)(obj->getScale()*100)/100.f,
							(int)(obj->getScale()*100)/100.f-m_objList[i].originalData["sx"].asFloat(),
							(int)(obj->getScale()*100)/100.f-m_objList[i].originalData["sy"].asFloat()
							);
			else{
				if(m_objList[i].originalData["sx"].asFloat()!=obj->getScaleX())
					CCLog("%s->setScaleX(%.1f); \t\t\t// dt %.1f",
								obj->getStringData().c_str(),
								(int)(obj->getScaleX()*100)/100.f,
								(int)(obj->getScaleX()*100)/100.f-m_objList[i].originalData["sx"].asFloat()
								);
				if(m_objList[i].originalData["sy"].asFloat()!=obj->getScaleY())
					CCLog("%s->setScaleY(%.1f); \t\t\t// dt %.1f",
								obj->getStringData().c_str(),
								(int)(obj->getScaleY()*100)/100.f,
								(int)(obj->getScaleY()*100)/100.f-m_objList[i].originalData["sy"].asFloat()
								);
			}
				
		}
		if(m_objList[i].originalData["w"].asFloat()!=obj->getContentSize().width || m_objList[i].originalData["h"].asFloat()!=obj->getContentSize().height)
			CCLog("%s->setContentSize(CCSizeMake(%.1f, %.1f)); \t\t\t// dt (%.1f, %.1f)",
						obj->getStringData().c_str(),
						obj->getContentSize().width,
						obj->getContentSize().height,
						obj->getContentSize().width-m_objList[i].originalData["w"].asFloat(),
						obj->getContentSize().height-m_objList[i].originalData["h"].asFloat()
						);
		
		if(CCLabelTTF* checkobj = dynamic_cast<CCLabelTTF*>(obj)){
			if(m_objList[i].originalData["fontsize"].asFloat()!=checkobj->getFontSize())
				CCLog("%s->setFontSize(%.1f); \t\t\t// dt %.1f",
							obj->getStringData().c_str(),
							(int)(checkobj->getFontSize()*10)/10.f,
							(int)(checkobj->getFontSize()*10)/10.f-m_objList[i].originalData["fontsize"].asFloat()
							);
		}
		
		if(CCNodeRGBA* checkobj = dynamic_cast<CCNodeRGBA*>(obj)){
			if(m_objList[i].originalData["opacity"].asInt()!=checkobj->getOpacity())
				CCLog("%s->setOpacity(%d); \t\t\t// dt %d",
							obj->getStringData().c_str(),
							checkobj->getOpacity(),
							checkobj->getOpacity()-m_objList[i].originalData["opacity"].asInt()
							);
		}
		if(m_objList[i].originalData["ax"].asFloat()!=obj->getAnchorPoint().x || m_objList[i].originalData["ay"].asFloat()!=obj->getAnchorPoint().y)
			CCLog("%s->setAnchorPoint(ccp(%.1f, %.1f)); \t\t\t// dt (%.1f, %.1f)",
						 obj->getStringData().c_str(),
						 obj->getAnchorPoint().x,
						 obj->getAnchorPoint().y,
						 obj->getAnchorPoint().x-m_objList[i].originalData["ax"].asFloat(),
						 obj->getAnchorPoint().y-m_objList[i].originalData["ay"].asFloat()
						 );
	}
	
	void setGuideImage(string filename){
		if(m_guideImage){
			m_guideImage->removeFromParentAndCleanup(true);
		}
		m_guideImage = CCSprite::create(filename.c_str());
		if(m_guideImage==nullptr)return;
		setFormSetter(m_guideImage);
		this->addChild(m_guideImage);
		m_guideImage->setPosition(ccp(240,160));
		m_guideImage->setOpacity(80);
		m_guideImage->setVisible(false);
	}
	void setEnabledRemocon(bool _is){
		this->m_isEnabledRemocon=_is;
	}
	
	void start(){
		this->start("","");
	}
	void start(string filename){
		this->start(filename,"");
	}
	
	void start(CCNode* obj,string filename, string startName){
		if(!m_isEnabledRemocon)return;
		obj->addChild(KSTimer::create(0.3f,[this,filename,startName](){this->start(filename,startName);}));
	}
	
	void start(CCNode* obj){
		this->start(obj,"","");
	}
	void start(CCNode* obj,string filename){
		this->start(obj,filename,"");
	}
	
	string m_startObjName;
	string m_filename;
	void start(string filename, string startName);
	
	void addObjectInObjList(CCNode* obj){
		bool addnew=true;
		for(int i=0;i<m_objList.size();i++){
			if(obj->getStringData()==m_objList[i].obj->getStringData()){
				addnew = false;
			}
		}
		
		FormSetterObj newobj;
		newobj.obj = obj;
		newobj.isEdited = false;
		newobj.originalData["x"]= obj->getPosition().x;
		newobj.originalData["y"]= obj->getPosition().y;
		newobj.originalData["w"]= obj->getContentSize().width;
		newobj.originalData["h"]= obj->getContentSize().height;
		newobj.originalData["sx"]=obj->getScaleX();
		newobj.originalData["sy"]=obj->getScaleY();
		newobj.originalData["ax"]=obj->getAnchorPoint().x;
		newobj.originalData["ay"]=obj->getAnchorPoint().y;
		
		if(CCNodeRGBA* checkobj = dynamic_cast<CCNodeRGBA*>(obj)){
			newobj.originalData["opacity"]=checkobj->getOpacity();
		}
		if(CCLabelTTF* checkobj = dynamic_cast<CCLabelTTF*>(obj)){
			newobj.originalData["fontsize"]=checkobj->getFontSize();
		}
		
		if(addnew)m_objList.push_back(newobj);
	}
	void findObject(CCNode* obj){
		if(!obj)return;
		if(obj->getStringData()!=""){
			this->addObjectInObjList(obj);
		}
		
		CCArray* children = obj->getChildren();
		if(children==nullptr)return;
		
		for(int i=0;i<children->count();i++){
			CCNode* child = (CCNode*)children->objectAtIndex(i);
			findObject(child);
			//if(childchild!=nullptr){m_objList->addObject(childchild);}
		}
		
		//	return nullptr;
	}
	
	
	bool ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent );
	
	
	void setInfomation(){
		if(m_selectedObjNumber<0)return;
		
		CCNode* selectedObj = (CCNode*)m_objList[m_selectedObjNumber].obj;
		m_objName->setString(selectedObj->getStringData().c_str());
		if(m_modeBtn->getTag()==0){
			m_modeBtn->setTitle("position");
			m_objInfo->setString(CCString::createWithFormat("x:%f\ny:%f",selectedObj->getPosition().x,selectedObj->getPosition().y)->getCString());
		}else if(m_modeBtn->getTag()==1){
			m_modeBtn->setTitle("scale");
			m_objInfo->setString(CCString::createWithFormat("scaleX:%f\nscaleY:%f",(int)(selectedObj->getScaleX()*100)/100.f,(int)(selectedObj->getScaleY()*100)/100.f)->getCString());
		}else if(m_modeBtn->getTag()==2){
			m_modeBtn->setTitle("size");
			m_objInfo->setString(CCString::createWithFormat("width:%f\nheight:%f",selectedObj->getContentSize().width,selectedObj->getContentSize().height)->getCString());
		}else if(m_modeBtn->getTag()==3){
			m_modeBtn->setTitle("fotnsize");
			if(CCLabelTTF* checkobj = dynamic_cast<CCLabelTTF*>(selectedObj)){
				m_objInfo->setString(CCString::createWithFormat("size:%f",checkobj->getFontSize())->getCString());
			}else{
				m_objInfo->setString("do not support");
			}
		}else if(m_modeBtn->getTag()==4){
			m_modeBtn->setTitle("opacity");
			if(CCNodeRGBA* checkobj = dynamic_cast<CCNodeRGBA*>(selectedObj)){
				m_objInfo->setString(CCString::createWithFormat("opacity:%d",checkobj->getOpacity())->getCString());
			}else{
				m_objInfo->setString("do not support");
			}
		}else if(m_modeBtn->getTag()==5){
			m_modeBtn->setTitle("anchorpoint");
			m_objInfo->setString(CCString::createWithFormat("x:%f\ny:%f",selectedObj->getAnchorPoint().x,selectedObj->getAnchorPoint().y)->getCString());

		}else{
			m_modeBtn->setTitle("none");
		}
		
		m_resetBtn->setEnabled(m_objList[m_selectedObjNumber].isEdited);
	}

	
	void ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
	{
		
		bool utilMove = false;
		if(remocon->isVisible()){
			if(pTouch->getLocation().y<20 || pTouch->getLocation().x<20){
				CCPoint dtPos = ccpMult(ccpSub(pTouch->getLocation(),m_startPosition),0.5f);
				dtPos.x = ((int)(dtPos.x*10*2)/(int)10)/2.f;
				dtPos.y = ((int)(dtPos.y*10*2)/(int)10)/2.f;
				remocon->setPosition(ccpAdd(remocon->getPosition(),dtPos));
				utilMove=true;
			}else if(pTouch->getLocation().y>300 || pTouch->getLocation().x>460){
				CCPoint dtPos = ccpMult(ccpSub(pTouch->getLocation(),m_startPosition),0.5f);
				dtPos.x = ((int)(dtPos.x*10*2)/(int)10)/2.f;
				dtPos.y = ((int)(dtPos.y*10*2)/(int)10)/2.f;
				m_guideLine->setPosition(ccpAdd(m_guideLine->getPosition(),dtPos));
				utilMove=true;
			}
		}
		
		if(m_selectedObjNumber>=0 && utilMove==false){
			
			CCNode* selectedObj = (CCNode*)m_objList[m_selectedObjNumber].obj;
			m_objList[m_selectedObjNumber].isEdited = true;
			if(m_modeBtn->getTag()==0){
				CCPoint dtPos = ccpMult(ccpSub(pTouch->getLocation(),m_startPosition),0.1f);
				dtPos.x = ((int)(dtPos.x*10*2)/(int)10)/2.f;
				dtPos.y = ((int)(dtPos.y*10*2)/(int)10)/2.f;
				selectedObj->setPosition(ccpAdd(selectedObj->getPosition(),dtPos));
			}else if(m_modeBtn->getTag()==1){
				CCPoint dtPos = ccpMult(ccpSub(pTouch->getLocation(),m_startPosition),0.1f);
				dtPos.x = ((int)(dtPos.x*10*10)/(int)10)/(double)500;
				dtPos.y = ((int)(dtPos.y*10*10)/(int)10)/(double)500;
				selectedObj->setScaleX(selectedObj->getScaleX()+dtPos.x);
				selectedObj->setScaleY(selectedObj->getScaleY()+dtPos.y);
			}else if(m_modeBtn->getTag()==2){
				CCPoint dtPos = ccpMult(ccpSub(pTouch->getLocation(),m_startPosition),0.1f);
				dtPos.x = ((int)(dtPos.x*10*2)/(int)10)/2.f+selectedObj->getContentSize().width;
				dtPos.y = ((int)(dtPos.y*10*2)/(int)10)/2.f+selectedObj->getContentSize().height;
				
				selectedObj->setContentSize(CCSizeMake(dtPos.x, dtPos.y));
			}else if(m_modeBtn->getTag()==3){
				if(CCLabelTTF* checkobj = dynamic_cast<CCLabelTTF*>(selectedObj)){
					CCPoint dtPos = ccpMult(ccpSub(pTouch->getLocation(),m_startPosition),0.1f);
					dtPos.y = (int)(dtPos.y*10)/10.f;
					checkobj->setFontSize(checkobj->getFontSize()+dtPos.y);
				}
			}else if(m_modeBtn->getTag()==4){
				if(CCNodeRGBA* checkobj = dynamic_cast<CCNodeRGBA*>(selectedObj)){
					CCPoint dtPos = ccpMult(ccpSub(pTouch->getLocation(),m_startPosition),1.f);
					int newopa = checkobj->getOpacity()+(int)dtPos.y;
					if(newopa>255)newopa=255;
					if(newopa<0)newopa=0;
					checkobj->setOpacity(newopa);
				}
			}else if(m_modeBtn->getTag()==5){
				CCPoint dtPos = ccpMult(ccpSub(pTouch->getLocation(),m_startPosition),0.1f);
				dtPos.x = ((int)(dtPos.x*10*10)/(int)10)/(double)500;
				dtPos.y = ((int)(dtPos.y*10*10)/(int)10)/(double)500;
				selectedObj->setAnchorPoint(ccpAdd(selectedObj->getAnchorPoint(),dtPos));
			}
			
			setInfomation();
		}
		
		m_startPosition = pTouch->getLocation();
	}
	
	double round( double value, int pos )
	{
		double temp;
		temp = value * pow( 10, pos );  // 원하는 소수점 자리수만큼 10의 누승을 함
		temp = floor( temp + 0.5 );          // 0.5를 더한후 버림하면 반올림이 됨
		temp *= pow( 10, -pos );           // 다시 원래 소수점 자리수로
		return temp;
	}
	
	void ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
	{
		
	}
	
	void ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
	{
		
	}
	
	void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, (INT_MAX-5)*-1, false);
	}
	
	
	
	
	//정보를 갱신할 오브젝트를 등록한다.  이름, 오브젝트, 갱신시부를 펑션
	void addObject(string name,CCNode* obj,std::function<void(Json::Value)> func){
		
		
		map<string,FormSetterData>::iterator it;
		it = m_list.find(name);
		
		//받아온 데이터와 등록되어있는 오브젝트중 매칭이 되어있으면
		if(it != m_list.end()){
			(it->second).funcs.push_back(func);
			(it->second).objects.push_back(obj);
		}else{
			FormSetterData newObj;
			newObj.funcs.push_back(func);
			newObj.data = 0;
			newObj.objects.push_back(obj);
			m_list[name] = newObj;
		}
			
			

		
		if(m_is_sch==false)requestFormData();
	}
	
	//정보를 갱신할 오브젝트를 등록한다. 조금더 단순하게 이름과 오브젝트만 등록
	void addObject(string name,CCNode* obj){
		this->addObject(name,obj,nullptr);
	}
	
	//오브젝트가 사라질때 반드시 제거해줘야 오류를 피할수있다.
	void removeObj(string name){
		m_list.erase(name);
	}
	
	//오브젝트가 사라질때 반드시 제거해줘야 오류를 피할수있다.
	void removeObject(string name){
		m_list.erase(name);
	}
	
	//해당 이름의 최신데이터를 받아온다.
	Json::Value getFormData(string name){
		if(m_list.find(name)!=m_list.end()){
				return m_list[name].data;
		}
		
		return NULL;
	}
	
	//해당이름의 오브젝트를 받아온다. (하나만)
	CCNode* getFormObject(string name){
		if(m_list.find(name)!=m_list.end()){
			return *(m_list[name].objects.begin());
		}
		return NULL;
	}
	
	FormSetterData* getFormSetterData(string name){
		if(m_list.find(name)!=m_list.end()){
			return &m_list[name];
		}
		return NULL;
	}
	
	//데이터 갱신주기를 설정한다. 미설정시 1(1초)이다.
	void setDelay(float delay){
		m_delay = delay;
	}
	
	//갱신이 일어날때 부를 함수
	void setFuncAtReceived(std::function<void(void)> func){
		m_funcAtReceived=func;
	}
	
	
	

	
	//서버의 응답을 받아 등록된 오브젝트의 포지션과 스케일을 조절하고 펑션이 등록되어있다면 호출한다.
	void receivedFormData(Json::Value p){
		
		this->matchReceivedData(p);
		//갱신후 부를 함수가 설정되어있다면 콜한다.
		if(m_funcAtReceived)m_funcAtReceived();

		//설정된 딜레이후 다시 데이터를 받아온다.
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
																															 schedule_selector(FormSetter::requestFormData),
																															 this,
																															 1.f,
																															 0,
																															 this->m_delay,
																															 false);
	}
	
	//한번만 부른다.
	void receivedFormDataOnce(Json::Value p){
		this->matchReceivedData(p);
		if(m_funcAtReceivedOnce){
				m_funcAtReceivedOnce();
				m_funcAtReceivedOnce=nullptr;
		}
	}
	
	CCNode* findObjectByString(CCNode* obj,string strID){
		if(!obj)return nullptr;
		if(obj->getStringData()==strID)return obj;
		
		CCArray* children = obj->getChildren();
		if(children==nullptr)return nullptr;
		
		for(int i=0;i<children->count();i++){
			CCNode* child = (CCNode*)children->objectAtIndex(i);
			if(child->getStringData()==strID)return child;
			
			CCNode* childchild = findObjectByString(child, strID);
			if(childchild!=nullptr)return childchild;
		}
		
		return nullptr;
	}
	
	void matchReceivedData(Json::Value p){
		//update data
		Json::Value::Members m = p.getMemberNames();
		
		//받아온 데이터를 한번 훑어보자
		for(auto iter=m.begin();iter!=m.end();++iter){
			map<string,FormSetterData>::iterator it;
			it = m_list.find(*iter);
			
			CCNode* findObject = findObjectByString(CCDirector::sharedDirector()->getRunningScene(), *iter); // ->getChildren()->objectAtIndex(0)), *iter);
			if(findObject){
				CCNode* obj = findObject;
				Json::Value fData = p.get((*iter).c_str(),Json::Value());
				if(obj && !fData["height"].isNull())
					obj->setContentSize(ccp(fData.get("width", 0).asFloat(),fData.get("height", 0).asFloat()));
				
				if(obj && !fData["x"].isNull())
					obj->setPositionX(fData.get("x", 0).asFloat());
				
				if(obj &&!fData["y"].isNull())
					obj->setPositionY(fData.get("y", 0).asFloat());
				
				if(obj &&!fData["scale"].isNull() && fData["scale"].asFloat()>0)
					obj->setScale(fData.get("scale", 1).asFloat());
				
				if(obj && !fData["anchorX"].isNull())
					obj->setAnchorPoint(ccp(fData.get("anchorX", 0).asFloat(),fData.get("anchorY", 0).asFloat()));
				
				if(obj && !fData["w"].isNull())
					obj->setContentSize(CCSizeMake(fData.get("w", 0).asFloat(),obj->getContentSize().height));
				
				if(obj && !fData["h"].isNull())
					obj->setContentSize(CCSizeMake(obj->getContentSize().width, fData.get("h", 0).asFloat()));
			}
			
			//받아온 데이터와 등록되어있는 오브젝트중 매칭이 되어있으면
			if(it != m_list.end()){
				//포지션과 스케일을 조절해준다.
				(it->second).data = p[(*iter).c_str()];
				
				
				
				
				vector<CCNode*>::iterator it2;
				for (it2=(it->second).objects.begin();it2!=(it->second).objects.end();it2++) {
					CCNode* obj = *it2;
					if(obj && !(it->second).data["height"].isNull())
						obj->setContentSize(ccp((it->second).data.get("width", 0).asFloat(),(it->second).data.get("height", 0).asFloat()));
					
					if(obj && !(it->second).data["x"].isNull())
						obj->setPositionX((it->second).data.get("x", 0).asFloat());
					
					if(obj &&!(it->second).data["y"].isNull())
						obj->setPositionY((it->second).data.get("y", 0).asFloat());
					
					if(obj &&!(it->second).data["scale"].isNull() && (it->second).data["scale"].asFloat()>0)
						obj->setScale((it->second).data.get("scale", 1).asFloat());
					
					if(obj && !(it->second).data["anchorX"].isNull())
						obj->setAnchorPoint(ccp((it->second).data.get("anchorX", 0).asFloat(),(it->second).data.get("anchorY", 0).asFloat()));
					
					if(obj && !(it->second).data["w"].isNull())
						obj->setContentSize(CCSizeMake((it->second).data.get("w", 0).asFloat(),obj->getContentSize().height));
											   
					if(obj && !(it->second).data["h"].isNull())
						obj->setContentSize(CCSizeMake(obj->getContentSize().width, (it->second).data.get("h", 0).asFloat()));
				}
				
				
				vector<std::function<void(Json::Value)>>::iterator it3;
				for (it3=(it->second).funcs.begin();it3!=(it->second).funcs.end();it3++) {
					if((*it3))(*it3)(p[*iter]);
				}
				
//				
//				if((it->second).obj && !(it->second).data["x"].isNull())
//					((it->second).obj)->setPositionX((it->second).data.get("x", 0).asFloat());
//				
//				if((it->second).obj &&!(it->second).data["y"].isNull())
//					((it->second).obj)->setPositionY((it->second).data.get("y", 0).asFloat());
//				
//				if((it->second).obj &&!(it->second).data["scale"].isNull())
//					((it->second).obj)->setScale((it->second).data.get("scale", 1).asFloat());
//				
//				if((it->second).obj && !(it->second).data["anchorX"].isNull())
//					((it->second).obj)->setAnchorPoint(ccp((it->second).data.get("anchorX", 0).asFloat(),(it->second).data.get("anchorY", 0).asFloat()));
//				
				
				

				//없으면
			}else{
				FormSetterData newObj;
				newObj.data = p[(*iter).c_str()];
			//	newObj.func = nullptr;
			//	newObj.obj = nullptr;
				
				m_list[(*iter).c_str()] = newObj;
			}
		}
	}
	

	
	//서버로 데이터를 요청한다.
	void requestFormData(){
		m_is_sch = true;
		Json::Value param;
		graphdog->command("getformsetter", param, this, json_selector(this, FormSetter::receivedFormData));
	}
	
	//서버로 데이터를 요청한다. 한번만
	void requestFormDataOnce(std::function<void(void)> receivedFunc){
		m_funcAtReceivedOnce = receivedFunc;
		
		Json::Value param;
		graphdog->command("getformsetter", param, this, json_selector(this, FormSetter::receivedFormDataOnce));
	}
	
};
#endif /* defined(__DGproto__FormSetter__) */
