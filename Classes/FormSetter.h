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
/*******************************************************
오브젝트 배치하기의 슈퍼초 울트라 캡 혁신
 *******************************************************
 
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
	CCLog("ttt data is %s",a.c_str());
 });
 
*/
class FormSetter : public CCNode{
public:
	struct FormSetterData{
		Json::Value data;
	//	CCNode* obj;
		vector<CCNode*> objects;
		vector<std::function<void(Json::Value)>> funcs;
	};
	
	bool m_is_sch;
	bool m_is_once;
	map<string,FormSetterData> m_list;
	float m_delay;
	std::function<void(void)> m_funcAtReceived;
	std::function<void(void)> m_funcAtReceivedOnce;
	
	static FormSetter* get()
	{
		static FormSetter* _ins = 0;
		if(_ins == 0)
			_ins = new FormSetter();
		
		return _ins;
	}
	
	
	FormSetter(){
		m_is_sch=false;
		m_delay = 1.f;
		m_funcAtReceived = nullptr;
		m_funcAtReceivedOnce = nullptr;
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
		
		this->matchRecedvedData(p);
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
		this->matchRecedvedData(p);
		if(m_funcAtReceivedOnce){
				m_funcAtReceivedOnce();
				m_funcAtReceivedOnce=nullptr;
		}
	}
	
	void matchRecedvedData(Json::Value p){
		//update data
		Json::Value::Members m = p.getMemberNames();
		
		//받아온 데이터를 한번 훑어보자
		for(auto iter=m.begin();iter!=m.end();++iter){
			map<string,FormSetterData>::iterator it;
			it = m_list.find(*iter);
			
			//받아온 데이터와 등록되어있는 오브젝트중 매칭이 되어있으면
			if(it != m_list.end()){
				//포지션과 스케일을 조절해준다.
				(it->second).data = p[(*iter).c_str()];
				
				
				
				
				vector<CCNode*>::iterator it2;
				for (it2=(it->second).objects.begin();it2!=(it->second).objects.end();it2++) {
					
					if((*it2) && !(it->second).data["height"].isNull())
						(*it2)->setContentSize(ccp((it->second).data.get("width", 0).asFloat(),(it->second).data.get("height", 0).asFloat()));
					
					if((*it2) && !(it->second).data["x"].isNull())
						(*it2)->setPositionX((it->second).data.get("x", 0).asFloat());
					
					if((*it2) &&!(it->second).data["y"].isNull())
						(*it2)->setPositionY((it->second).data.get("y", 0).asFloat());
					
					if((*it2) &&!(it->second).data["scale"].isNull() && (it->second).data["scale"].asFloat()>0)
						(*it2)->setScale((it->second).data.get("scale", 1).asFloat());
					
					if((*it2) && !(it->second).data["anchorX"].isNull())
						(*it2)->setAnchorPoint(ccp((it->second).data.get("anchorX", 0).asFloat(),(it->second).data.get("anchorY", 0).asFloat()));
					
					if((*it2) && !(it->second).data["w"].isNull())
						(*it2)->setContentSize(CCSizeMake((it->second).data.get("w", 0).asFloat(),(*it2)->getContentSize().height));
											   
					if((*it2) && !(it->second).data["h"].isNull())
						(*it2)->setContentSize(CCSizeMake((*it2)->getContentSize().width, (it->second).data.get("h", 0).asFloat()));
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
