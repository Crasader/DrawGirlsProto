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
#include "HSPEnums.h"

using namespace cocos2d;
using namespace std;

typedef void (CCObject::*kakaoSelType)(Json::Value);
#define kakao_selector(_SELECTOR) (kakaoSelType)(&_SELECTOR)

#define myHSP hspConnector::get()

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
		string result;
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
		temp.param = (param==0)?Json::Value():param;
		temp.callbackParam = (callbackParam==0)?Json::Value():callbackParam;
		temp.result = "";
		DeleSelTable[key] = temp;
		return key;
	}
	void remove(int _key)
	{
		if(DeleSelTable.find(_key) != DeleSelTable.end())
			DeleSelTable.erase(_key);
	}
	DeleSel* load(int _key)
	{
		if(DeleSelTable.find(_key) != DeleSelTable.end())
		{
			return &(DeleSelTable[_key]);
		}
		else
		{
//			DeleSel _t;
//			//_t.target = 0;
//			//_t.selector = 0;
//			_t.func=NULL;
			return nullptr;
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
	}
	~jsonDelegator(){}
private:
	map<int, DeleSel> DeleSelTable;
	int key;

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
		string bt;
	}
	
	Json::Value frineds;
	Json::Value appFriends;
	Json::Value	idMap;
	Json::Value myKakaoInfo;
	Json::Value mailData;
	
	Json::Value getMailByIndexCopy(int idx){
		return mailData["mailList"][idx];
	}
	
	Json::Value* getMailByIndex(int idx){
		return &mailData["mailList"][idx];
	}
	
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
		GraphDog::get()->setup("drawgirls_tstore", "12345678","", pGraphdogVersion);
	}
	
	bool command(const std::vector<CommandParam>& params, int errorCnt=-1){
		return graphdog->command(params,errorCnt);
	}
	
	bool command(const char* action, const Json::Value param,function<void(Json::Value)> func, int errorCnt=-1){
		return graphdog->command(action,param,func,errorCnt);
	}
	bool command(const char* action, const Json::Value param,CCObject* obj,function<void(Json::Value)> func){
		return graphdog->command(action,param,obj,func);
	}
	
	bool command(const char* action, const char* param,function<void(Json::Value)> func){
		Json::Reader r;
		Json::Value p;
		r.parse(param, p);
		return graphdog->command(action,p,func);
	}
	
	void removeTarget(CCObject* obj){
		graphdog->removeTarget(obj);
	}
	
	long long int getLastTimestampOnServer(){
		return graphdog->timestamp;
	}
	
	long long int getLastTimestampOnLocal(){
		return graphdog->localTimestamp;
	}
	
	long long int getCurrentTimestampOnLocal(){
		return graphdog->getTime();
	}
	
	long long int getMemberID();
	string getSocialID();
	string getKakaoProfileURL();
	string getKakaoNickname();
	bool setupHSPonIOS(int hspGameNo,string hspGameID,string hspGameVersion,void* launchOptions);
	
	void login(Json::Value param,Json::Value callbackParam,jsonSelType func);
	void logout(jsonSelType func);
	void withdrawAccount(jsonSelType func);
	void mappingToAccount(jsonSelType func);
	string getCountryCode();
	string getMarketCode();
	string getStoreID();
	string getHSPConfigurationProp(const std::string& prop);
	string getTimeZone();
	string getServerAddress();
	void sendPushMessage();
	
	void checkCGP(Json::Value param,Json::Value callbackParam,jsonSelType func);
	void checkCGP(Json::Value param,Json::Value callbackParam, CCObject* target, jsonSelType func);
	void requestProductInfos(jsonSelType func);
	void requestProductInfos(CCObject* target, jsonSelType func);
	void completePromotion();
	void completeInstallPromotion();
	void purchaseProduct(Json::Value param,Json::Value callbackParam, jsonSelType func);
	void purchaseProduct(Json::Value param,Json::Value callbackParam, CCObject* target, jsonSelType func);
	void openUrl(const std::string& url);
	void openHSPUrl(const std::string& url);
	void openCSCenter(const std::string& url);
	void mappingToAccount(int mt, bool force, jsonSelType func);
	void getIsUsimKorean(jsonSelType func); // value["korean"] 이 1이면 약관 페이지 띄워야 함.
	
	string getUniqId();
	int getVersionCode();
	int getLoginType();
    int getIsEnablePushNotification();
    void setIsEnablePushNotification(int p);
	void openHSPNotice();
	void launchPromotion();
	int openKakaoMsg();
	int sendKakaoMsg(string title,string msg,string url);
	void getAdXConnectEventInstance(string event, string data, string currency);
	void IgawAdbrixBuy(string productName);
	void IgawAdbrixFirstTimeExperience(string msg);
	void IgawAdbrixRetention(string msg);
	int registerGamePadCallback(jsonSelType func);
	int checkInspection();
	
	void analyticsPurchase(string itemCode, float payment, float unitCost, string currency, int level);
	void analyticsTraceMoneyAcquisition(string itemCode, string type, int amount, int level);
	void analyticsTraceMoneyConsumption(string itemCode, string type, int amount, int level);
	void analyticsTraceLevelUp(int l);
	void analyticsTraceFriendCount(int f);
	void analyticsSetUserId(string userId, bool t); // t = true (userid)
	
protected:
	void registerGamePadCallback(CCObject* target, jsonSelType func);
public:
	void unregisterGamePadCallback(int registeredKey);
	
	double getScreenRealWidth();
	double getScreenRealHeight();
	
	//void loadMyInfo(Json::Value obj);
	//void loadMyProfile(Json::Value param,Json::Value callbackParam,jsonSelType func);
	//void loadFriendsProfile(Json::Value param,Json::Value callbackParam,jsonSelType func);
	
	//void kLoadLocalUser(jsonSelType func);
	//void kLoadLocalUser(Json::Value callbackParam,jsonSelType func);
	
	//void kLoadFriends(jsonSelType func);
	//void kLoadFriends(Json::Value callbackParam,jsonSelType func);
	
	//void kSendMessage(Json::Value param,jsonSelType func);
	//void kSendMessage(Json::Value param,Json::Value callbackParam,jsonSelType func);
	
	//void kLogout(jsonSelType func);
	//void kLogout(Json::Value callbackParam,jsonSelType func);
	
	//void kUnregister(jsonSelType func);
	//void kUnregister(Json::Value callbackParam,jsonSelType func);
	
	
	// void callFuncMainQueue(Json::Value param,Json::Value callbackParam,CCObject *target,hspSelType selector,void*resultDict);
	//void kLoadFriends(CCObject* target,hspSelType selector);
	// void kLoadFriendsStep2(Json::Value kakaoIds,int delekey);
	//void kLoadFriendsStep2(Json::Value obj);
	//void kLoadFriendsStep3(Json::Value obj);
	
	//void kSendMessage();
	//void kLoadRanking();
	
	
	
};



#endif /* defined(__hspConnector__hspConnector__) */
