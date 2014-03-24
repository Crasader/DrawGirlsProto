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

class FormSetter : public CCNode{
public:
	struct FormSetterData{
		Json::Value data;
		CCNode* obj;
		std::function<void(Json::Value)> func;
	};
	
	bool m_is_sch;
	map<string,FormSetterData> m_list;
	
	
	
	static FormSetter* get()
	{
		static FormSetter* _ins = 0;
		if(_ins == 0)
			_ins = new FormSetter();
		
		return _ins;
	}
	
	
	FormSetter(){
		m_is_sch=false;
	}
	
	void addObject(string name,CCNode* obj,std::function<void(Json::Value)> func){

		FormSetterData newObj;
		newObj.obj = obj;
		newObj.func = func;
		
		m_list[name] = newObj;
		
		if(m_is_sch==false)requestFormData();
	}
	
	void addObject(string name,CCNode* obj){
		this->addObject(name,obj,nullptr);
	}
	
	void removeObj(string name){
		m_list.erase(name);
	}
	
	void requestFormData(){
		m_is_sch = true;
		Json::Value param;
		graphdog->command("getformsetter", param, this, json_selector(this, FormSetter::getFormData));
	}
	
	void getFormData(Json::Value p){
		//update data
		Json::Value::Members m = p.getMemberNames();
		
		for(auto iter=m.begin();iter!=m.end();++iter){
			map<string,FormSetterData>::iterator it;
			it = m_list.find(*iter);
			if(it != m_list.end()){
				(it->second).data = p[(*iter).c_str()];
				
				if(!(it->second).data["x"].isNull())
					((it->second).obj)->setPositionX((it->second).data.get("x", 0).asFloat());
				
				if(!(it->second).data["y"].isNull())
					((it->second).obj)->setPositionY((it->second).data.get("y", 0).asFloat());
				
				if(!(it->second).data["scale"].isNull())
				((it->second).obj)->setScale((it->second).data.get("scale", 1).asFloat());
				
				if((it->second).func)(it->second).func(p[*iter]);
				
			}
		}
		
		
		CCLog(" ---- set Form!! ---- ");
		

		
CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
																															 schedule_selector(FormSetter::requestFormData),
																															 this,
																															 1.f,
																															 0,
																															 1.f,
																															 false);
		
	}
	
	
};
#endif /* defined(__DGproto__FormSetter__) */
