//
//  hspConnector.h
//  hspConnector
//
//  Created by LitQoo on 13. 10. 2..
//
//

#ifndef __hspConnector__hspConnector__
#define __hspConnector__hspConnector__

#include <iostream>
#include "jsoncpp/json.h"
#include "cocos2d.h"
#include "hspDelegator.h"
#include "GraphDog.h"

using namespace cocos2d;
using namespace std;


using namespace cocos2d;
using namespace std;
typedef void (CCObject::*kakaoSelType)(Json::Value);
#define kakao_selector(_SELECTOR) (kakaoSelType)(&_SELECTOR)

class jsonDelegator
{
    
public:
	
	struct DeleSel
	{
		CCObject* target;
		//@kakaoSelType selector;
        jsonSelType func;
        Json::Value param;
        Json::Value callbackParam;
	};
    
    
public:
	void removeTarget(CCObject* _obj)
	{
    	for(auto iter = DeleSelTable.begin(); iter != DeleSelTable.end(); iter++)
		{
			if(iter->second.target == _obj)
			{
				DeleSelTable.erase(iter);
			}
		}
	}
    
	int add(jsonSelType func,Json::Value param,Json::Value callbackParam)
	{
		key++;
		DeleSel temp;
		//temp.target = target;
		//temp.selector = selector;
        temp.func = func;
        temp.param = param;
        temp.callbackParam = callbackParam;
		DeleSelTable[key] = temp;
		return key;
	}
	void remove(int _key)
	{
		if(DeleSelTable.find(_key) != DeleSelTable.end())
			DeleSelTable.erase(_key);
	}
	DeleSel load(int _key)
	{
		if(DeleSelTable.find(_key) != DeleSelTable.end())
		{
			return DeleSelTable[_key];
		}
		else
		{
			DeleSel _t;
			//_t.target = 0;
			//_t.selector = 0;
			_t.func=NULL;
            return _t;
		}
	}
    
public:
	static jsonDelegator* get()
	{
		static jsonDelegator* _ins = 0;
		if(_ins == 0)
			_ins = new jsonDelegator();
		return _ins;
	}
    
private:
	jsonDelegator()
	{
		key = 0;
        buff="";
	}
	~jsonDelegator(){}
private:
	map<int, DeleSel> DeleSelTable;
	int key;
    
public:
    string buff;
};


class hspConnector:CCObject {
public:
    static hspConnector* get()
	{
		static hspConnector* _ins = 0;
		if(_ins == 0)
			_ins = new hspConnector();
		return _ins;
	}
    
    hspConnector(){
        
    }
    
    Json::Value frineds;
    Json::Value appFriends;
	Json::Value	idMap;
	Json::Value myKakaoInfo;
	
	Json::Value getFriendByKakaoIDCopy(string kakaoID){
		return appFriends[kakaoID];
	}
	
	Json::Value* getFriendByKakaoID(string kakaoID){
		return &appFriends[kakaoID];
	}
	
	Json::Value* getFriendByHSPNO(int64_t memberNo){
		Json::Value::Members m = appFriends.getMemberNames();
		for (auto iter = m.begin(); iter!=m.end(); ++iter) {
			string kakaoID = (string)*iter;
			if(appFriends[kakaoID]["profile"]["memberNo"].asInt64()==memberNo){
				return &appFriends[kakaoID];
			}
		}
		return 0;
	}

	Json::Value getFriendByHSPNOCopy(int64_t memberNo){
		Json::Value::Members m = appFriends.getMemberNames();
		for (auto iter = m.begin(); iter!=m.end(); ++iter) {
			string kakaoID = (string)*iter;
			if(appFriends[kakaoID]["profile"]["memberNo"].asInt64()==memberNo){
				return appFriends[kakaoID];
			}
		}
		return 0;
	}
	
	Json::Value* getFriendByIndex(unsigned int idx){
		Json::Value::Members m =this->appFriends.getMemberNames();
		string kakaoID=(string)m[idx];
		return &appFriends[kakaoID];
	}
	
    string hspID;
	
    int hspNo;
    
    void setup(string pHSPID, int pHSPNo, string pGraphdogVersion){
        hspID = pHSPID;
        hspNo = pHSPNo;
        graphdog->setup("12345678", pGraphdogVersion);
    }
    
    bool command(const std::vector<CommandParam>& params){
        return graphdog->command(params);
    }
    
    bool command(string action, const Json::Value param,function<void(Json::Value)> func){
        return graphdog->command(action,param,func);
    }
    
    long long int getHSPMemberNo();
	string getKakaoID();
    
    bool setupHSPonIOS(int hspGameNo,string hspGameID,string hspGameVersion,void* launchOptions);

    void login(Json::Value param,Json::Value callbackParam,jsonSelType func);
	void loadMyInfo(Json::Value obj);
    void loadMyProfile(Json::Value param,Json::Value callbackParam,jsonSelType func);
    void loadFriendsProfile(Json::Value param,Json::Value callbackParam,jsonSelType func);
    
    void kLoadLocalUser(jsonSelType func);
    void kLoadLocalUser(Json::Value callbackParam,jsonSelType func);
    
    void kLoadFriends(jsonSelType func);
    void kLoadFriends(Json::Value callbackParam,jsonSelType func);
    
    void kSendMessage(Json::Value param,jsonSelType func);
    void kSendMessage(Json::Value param,Json::Value callbackParam,jsonSelType func);
    
    void kLogout(jsonSelType func);
    void kLogout(Json::Value callbackParam,jsonSelType func);
    
    void kUnregister(jsonSelType func);
    void kUnregister(Json::Value callbackParam,jsonSelType func);
    
    
    // void callFuncMainQueue(Json::Value param,Json::Value callbackParam,CCObject *target,hspSelType selector,void*resultDict);
    void kLoadFriends(CCObject* target,hspSelType selector);
    // void kLoadFriendsStep2(Json::Value kakaoIds,int delekey);
    void kLoadFriendsStep2(Json::Value obj);
    void kLoadFriendsStep3(Json::Value obj);
    
    void kSendMessage();
    void kLoadRanking();
    void reportGameUserData(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void loadMyDetailedProfile(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void loadServiceProperties(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void loadProfiles(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void loadDetailedProfiles(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void saveMemberData(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void loadMemberData(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void removeMemberData(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void loadRankings(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void reportRanking(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void queryHSPMemberNos(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void queryKakaoIds(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void queryFollowingMembers(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void queryFollowers(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void followMembers(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void blockMembers(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    void queryMembersRecommended(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector);
    
    
};



#endif /* defined(__hspConnector__hspConnector__) */
