
//
//  hspConnector.cpp
//  hspConnector
//
//  Created by LitQoo on 13. 10. 2..
//
//

#include "hspConnector.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import "HSPCore.h"
#import "HSPUri.h"
#import "HSPMyProfile.h"
#import "HSPMemberDataStorage.h"
#import "HSPRanking.h"
//#import "HSPKakao.h"
//#import "Kakao.h"
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//#import "KALocalUser.h"
//#import "KAAuth.h"
//#import "KakaoSDKViewController.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include "jni.h"
#endif
#include <string>
#include <sstream>
using namespace std;
USING_NS_CC;


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
extern "C"{
void Java_com_litqoo_lib_hspConnector_SetupOnAndroid(JNIEnv *env, jobject thiz,int hspGameNo, jstring hspGameID, jstring hspGameVersion)
{
	jboolean isCopy = JNI_FALSE;
	const char* revStr = env->GetStringUTFChars(hspGameID, &isCopy);
	string _gameID = revStr;

	jboolean isCopy2 = JNI_FALSE;
	const char* revStr2 = env->GetStringUTFChars(hspGameVersion, &isCopy2);
	string _gameVersion = revStr2;

	hspConnector::get()->setup(_gameID,hspGameNo,_gameVersion);

}

void Java_com_litqoo_lib_hspConnector_SendResultNative(JNIEnv *env, jobject thiz,int _key, jstring datas, bool isFinish)
{
	CCLOG("sendresultnative1 %d", _key);
	jsonDelegator::DeleSel delesel = jsonDelegator::get()->load(_key);
	jboolean isCopy = JNI_FALSE;
	const char* revStr = env->GetStringUTFChars(datas, &isCopy);
	string throwData = revStr;

	CCLOG("sendresultnative1");
	jsonDelegator::get()->buff.append(throwData);


	CCLOG("sendresultnative3");
	if(delesel.func!=NULL)
	{

		CCLOG("sendresultnative4");
		if(isFinish){

			CCLOG("sendresultnative5");
			Json::Value resultData;

			Json::Value resultObj;
			Json::Reader rd;
			rd.parse(jsonDelegator::get()->buff.c_str(),resultObj);

			//
			CCLOG("sendresultnative6");
			resultObj["param"] = delesel.param;
			resultObj["callback"] = delesel.callbackParam;
			//((delesel.target)->*(delesel.selector))(resultObj);
			delesel.func(resultObj);


			jsonDelegator::get()->buff="";

			CCLOG("sendresultnative7");
		}
	}

	if(isFinish) jsonDelegator::get()->remove(_key);

	CCLOG("sendresultnative8");
	env->ReleaseStringUTFChars(datas, revStr);

	CCLOG("sendresultnative9");
	return;
}
}
#endif


long long int hspConnector::getHSPMemberNo(){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return [HSPCore sharedHSPCore].memberNo;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "getHSPMemberNo", "()J")) {
		jlong result = t.env->CallStaticLongMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		return (long long int)result;
	}
#endif
}

void hspConnector::sendPushMessage(){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

	CCLog("call sendPushMessage");
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "sendPushMessage", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		CCLog("result sendPushMessage");
		//		jlong result = t.env->CallStaticLongMethod(t.classID, t.methodID);
		//		t.env->DeleteLocalRef(t.classID);
		//		return (long long int)result;
	}
#endif
}

string hspConnector::getKakaoID(){
	return graphdog->getKakaoMemberID();
	//return this->myKakaoInfo.get("user_id", "").asString();
}

string hspConnector::getKakaoProfileURL(){
	return this->myKakaoInfo.get("profile_image_url", "").asString();
}

string hspConnector::getKakaoNickname(){
	return this->myKakaoInfo.get("nickname", "").asString();
}


#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
bool hspConnector::setupHSPonIOS(int hspGameNo,string hspGameID,string hspGameVersion,void* launchOptions){

	this->setup(hspGameID, hspGameNo, hspGameVersion);
	bool r=[HSPCore
		createHSPCoreWithGameNo:hspGameNo
		gameID:[NSString stringWithUTF8String:hspGameID.c_str()]
		gameVersion:[NSString stringWithUTF8String:hspGameVersion.c_str()]
		launchOptions:(NSDictionary*)launchOptions];

	return r;
}
#endif


void jsonLog(string tag,Json::Value value){
	Json::StyledWriter wr;
	CCLog("%s -- %s",tag.c_str(),wr.write(value).c_str());
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
void* detailedProfileToDict(void* _profile){

	HSPDetailedProfile *detailedProfile = (HSPDetailedProfile *) _profile;
	NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
	//디테일정보
	if(![detailedProfile.gameUserData isKindOfClass:[NSNull class]] && [detailedProfile.gameUserData length]!=0){
		[resultDict setObject:[NSJSONSerialization JSONObjectWithData: [detailedProfile.gameUserData dataUsingEncoding:NSUTF8StringEncoding]
			options: NSJSONReadingMutableContainers
			error: nil] forKey:@"gameUserData"];
	}

	[resultDict setObject:[NSNumber numberWithInt:detailedProfile.age] forKey:@"age"];
	[resultDict setObject:[NSNumber numberWithInt:detailedProfile.gender] forKey:@"gender"];
	[resultDict setObject:[NSNumber numberWithBool:detailedProfile.isAdmin] forKey:@"isAdmin"];
	[resultDict setObject:[NSNumber numberWithInt:detailedProfile.relationTypeFromMe] forKey:@"relationTypeFromMe"];
	[resultDict setObject:[NSNumber numberWithInt:detailedProfile.relationTypeToMe] forKey:@"relationTypeToMe"];

	[resultDict setObject:detailedProfile.todayWords forKey:@"todayWords"];
	return resultDict;

}

void* profileToDict(void* _profile){
	HSPProfile *profile = (HSPProfile *)_profile;
	NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];

	//기본정보
	[resultDict setObject:[NSNumber numberWithLongLong:profile.memberNo] forKey:@"memberNo"];
	if([profile.nickname length]>0)[resultDict setObject:profile.nickname forKey:@"nickname"];
	else [resultDict setObject:@"noname" forKey:@"nickname"];
	[resultDict setObject:[NSNumber numberWithBool:profile.isValid] forKey:@"isValid"];
	[resultDict setObject:[NSNumber numberWithBool:profile.exposeOnline] forKey:@"exposeOnline"];
	[resultDict setObject:[NSNumber numberWithInt:profile.recentPlayedGameNo] forKey:@"recentPlayedGameNo"];

	if(profile.lastLoginDate){
		NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
		[formatter setDateFormat:@"yyyyMMddHHmmss"];
		NSString *lastLogin = [formatter stringFromDate:profile.lastLoginDate];
		[resultDict setObject:lastLogin forKey:@"lastLoginDate"];
		[formatter release];
	}

	return resultDict;
}

void addErrorInResult(void*resultDict,void*error){
	NSMutableDictionary *r = (NSMutableDictionary *)resultDict;

	if(error!=NULL){
		HSPError *e = (HSPError *)error;
		NSMutableDictionary *hspError = [NSMutableDictionary dictionary];
		[hspError setObject:[NSNumber numberWithBool:[e isSuccess]] forKey:@"isSuccess"];
		[hspError setObject:[NSNumber numberWithInt:[e code]] forKey:@"code"];
		[hspError setObject:[e localizedDescription] forKey:@"localizedDescription"];
		[r setObject:hspError forKey:@"error"];
	}else{
		NSMutableDictionary *hspError = [NSMutableDictionary dictionary];
		[hspError setObject:[NSNumber numberWithBool:true] forKey:@"isSuccess"];
		[hspError setObject:[NSNumber numberWithInt:0] forKey:@"code"];
		[hspError setObject:@"" forKey:@"localizedDescription"];
		[r setObject:hspError forKey:@"error"];
	}
}

void* jsonToDict(Json::Value param){
	Json::FastWriter wr;
	CCLog("loadprofile param string %s",wr.write(param).c_str());
	NSString *jsonString = [NSString stringWithUTF8String:wr.write(param).c_str()];
	NSData *jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
	NSDictionary *parameters = [NSJSONSerialization JSONObjectWithData:jsonData options:kNilOptions error:nil];
	return parameters;
}

Json::Value arrayToJson(void*array){
	NSData *_jdata= [NSJSONSerialization dataWithJSONObject:(NSArray *)array options:NSJSONWritingPrettyPrinted error:nil];
	NSString *jsonString = [[NSString alloc] initWithData:_jdata encoding:NSUTF8StringEncoding];
	Json::Reader reader;
	Json::Value resultObj;

	reader.parse([jsonString cStringUsingEncoding:NSUTF8StringEncoding], resultObj);
	return resultObj;
}

Json::Value dictToJson(void*dict){
	NSData *_jdata= [NSJSONSerialization dataWithJSONObject:(NSDictionary *)dict options:NSJSONWritingPrettyPrinted error:nil];
	NSString *jsonString = [[NSString alloc] initWithData:_jdata encoding:NSUTF8StringEncoding];
	Json::Reader reader;
	Json::Value resultObj;

	reader.parse([jsonString cStringUsingEncoding:NSUTF8StringEncoding], resultObj);
	return resultObj;
}

#endif

void callFuncMainQueue(Json::Value param,Json::Value callbackParam,CCObject *target,hspSelType selector,void*resultDict){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	NSData *_jdata= [NSJSONSerialization dataWithJSONObject:(NSDictionary *)resultDict options:NSJSONWritingPrettyPrinted error:nil];
	NSString *jsonString = [[NSString alloc] initWithData:_jdata encoding:NSUTF8StringEncoding];
	Json::Reader reader;
	Json::Value resultObj;

	reader.parse([jsonString cStringUsingEncoding:NSUTF8StringEncoding], resultObj);


	if(!param.isNull())resultObj["param"] = param;
	if(!callbackParam.isNull())resultObj["callback"] = callbackParam;
	dispatch_async(dispatch_get_main_queue(),
								 ^{
									 if(target!=NULL && selector!=NULL)(target->*(selector))(resultObj);
								 }
								);
#endif
}


void callFuncMainQueue2(Json::Value param,Json::Value callbackParam,jsonSelType func,void*resultDict){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	NSData *_jdata= [NSJSONSerialization dataWithJSONObject:(NSDictionary *)resultDict options:NSJSONWritingPrettyPrinted error:nil];
	NSString *jsonString = [[NSString alloc] initWithData:_jdata encoding:NSUTF8StringEncoding];
	Json::Reader reader;
	Json::Value resultObj;

	reader.parse([jsonString cStringUsingEncoding:NSUTF8StringEncoding], resultObj);


	if(!param.isNull())resultObj["param"] = param;
	if(!callbackParam.isNull())resultObj["callback"] = callbackParam;
	dispatch_async(dispatch_get_main_queue(),
								 ^{
									 if(func != NULL)func(resultObj);
								 }
								);
#endif
}




/*
	 로그인
	 param
	 Bool ManualLogin : 자동로그인설정, 그냥 무조건 YES로.
	 reutrn
	 bool playable : 게임 실행 가능 여부.
	 dict error : 에러정보.
 */

void hspConnector::logout(jsonSelType func){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[[HSPCore sharedHSPCore] logoutWithCompletionHandler:
		^(HSPError *error)
		{
			NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
			addErrorInResult(resultDict, error);
			callFuncMainQueue2(0,0,func,resultDict);
		}
	];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "logout", "(I)V")) {
		int _key =  jsonDelegator::get()->add(func,0,0);
		t.env->CallStaticObjectMethod(t.classID, t.methodID,_key);
		t.env->DeleteLocalRef(t.classID);
	}
#endif

}
void hspConnector::login(Json::Value param,Json::Value callbackParam,jsonSelType func){
	bool ManualLogin=true;
	if(param!=0 && param!=NULL){
		ManualLogin= param.get("ManualLogin",true).asBool();
	}

	int dkey = jsonDelegator::get()->add(func, 0, 0);

	jsonSelType nextFunc = [dkey,this](Json::Value obj){

		int delekey = dkey;
		graphdog->setHSPMemberNo(hspConnector::get()->getHSPMemberNo());

		string hspids = CCString::createWithFormat("%lld",hspConnector::get()->getHSPMemberNo())->getCString();
		CCLog("member id is %s",hspids.c_str());
		graphdog->setKakaoMemberID(hspids);

		jsonDelegator::DeleSel delsel = jsonDelegator::get()->load(delekey);
		if(delsel.func)delsel.func(obj);
		jsonDelegator::get()->remove(delekey);

		//		});

};

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

[[HSPCore sharedHSPCore] loginWithManualLogin:ManualLogin completionHandler:
^(BOOL playable, HSPError *error)
{

	NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
	[resultDict setObject:[NSNumber numberWithBool:playable] forKey:@"playable"];

	addErrorInResult(resultDict, error);
	callFuncMainQueue2(param,callbackParam,nextFunc,resultDict);
}
];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
JniMethodInfo t;
if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "login", "(IZ)V")) {
	int _key =  jsonDelegator::get()->add(nextFunc,param,callbackParam);
	t.env->CallStaticObjectMethod(t.classID, t.methodID,_key,ManualLogin);
	t.env->DeleteLocalRef(t.classID);
}
#endif
}

void hspConnector::checkCGP(Json::Value param,Json::Value callbackParam,jsonSelType func)
{
	int dkey = jsonDelegator::get()->add(func, 0, 0);
	jsonSelType nextFunc = [=](Json::Value obj){
		int delekey = dkey;
		jsonDelegator::DeleSel delsel = jsonDelegator::get()->load(delekey);
		if(delsel.func){
			delsel.func(obj);
		}
		jsonDelegator::get()->remove(delekey);

	};
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
JniMethodInfo t;
if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "checkCGP", "(I)Z")) {
	int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
	t.env->CallStaticObjectMethod(t.classID, t.methodID, _key);
	t.env->DeleteLocalRef(t.classID);
}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
// not implementation

#endif
}

void hspConnector::checkCGP(Json::Value param,Json::Value callbackParam, CCObject* target, jsonSelType func)
{
	GraphDog::get()->addTarget(target);
	function<void(Json::Value)> sFunc = [=](Json::Value value){
		CCLog("checkDelegator sFunc call");
		if(GraphDog::get()->cehckTarget(target))
			func(value);
	};
	checkCGP(param, callbackParam, sFunc);
}

void hspConnector::purchaseProduct(Json::Value param,Json::Value callbackParam,jsonSelType func)
{
	int dkey = jsonDelegator::get()->add(func, 0, 0);
	jsonSelType nextFunc = [=](Json::Value obj){
		int delekey = dkey;
		jsonDelegator::DeleSel delsel = jsonDelegator::get()->load(delekey);
		if(delsel.func){
			delsel.func(obj);
		}
		jsonDelegator::get()->remove(delekey);

	};
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
JniMethodInfo t;
if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "purchaseProduct", "(ILjava/lang/String;)Z")) {
	int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
	t.env->CallStaticObjectMethod(t.classID, t.methodID, _key, t.env->NewStringUTF(param.get("productid", "").asString().c_str()));
	t.env->DeleteLocalRef(t.classID);
}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
// not implementation

#endif
}
void hspConnector::purchaseProduct(Json::Value param,Json::Value callbackParam, CCObject* target, jsonSelType func)
{
	GraphDog::get()->addTarget(target);
	function<void(Json::Value)> sFunc = [=](Json::Value value){
		CCLog("checkDelegator sFunc call");
		if(GraphDog::get()->cehckTarget(target))
			func(value);
	};
	purchaseProduct(param, callbackParam, sFunc);
}
void hspConnector::openUrl(const std::string& url)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "openUrl", "(Ljava/lang/String;)V")) {
//		int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
		t.env->CallStaticObjectMethod(t.classID, t.methodID, t.env->NewStringUTF(url.c_str()));
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	// not implementation
	
#endif
}

void hspConnector::launchPromotion()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "launchPromotion", "()V")) {
		//		int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
		t.env->CallStaticObjectMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	// not implementation
	
#endif
}
//
//void hspConnector::kLoadFriends(CCObject* target,hspSelType selector){
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//
//    // kakao친구 로드! -> 앱사용자는 querykakaoHSP이용하여 hsp detailedProfile 로드! -> 로드한 정보 connector.friends에 저장.
//    //               -> 미사용자는 friends에 저장
//    // kakao전체친구 로드!
//    int _key =  hspDelegator::get()->add(target,selector,0,0);
//    
//    [[KALocalUser localUser] loadFriendsWithCompletionHandler:^(NSDictionary *response, NSError *error) {
//        
//        //response ->  app_friends_info{}, friends_info[{friend_nickname,nickname,hashed_talk_user_id,user_id,supported_device,profile_image_url}] , friends_count(int)
//       
//        //에러처리하기
//        
//        NSLog(@"appfriends %@",[response objectForKey:@"app_friends_info"]);
//        //addErrorInResult(response, error);
//        Json::Value param;
//        Json::Value friendInfo = dictToJson(response);
//        
//        //appfriends틀 만들기
//        
//        hspConnector::get()->appFriends.empty();
//        
//        NSArray *afrd = [response objectForKey:@"app_friends_info"];
//        
//        for (unsigned int i=0; i<[afrd count]; i++) {
//            NSDictionary *fInfo = [afrd objectAtIndex:i];
//            string _key = [[fInfo objectForKey:@"user_id"] UTF8String];
//            appFriends[_key]["nickname"]=[[fInfo objectForKey:@"nickname"] UTF8String];
//            appFriends[_key]["hashed_talk_user_id"]=[[fInfo objectForKey:@"hashed_talk_user_id"] UTF8String];
//            appFriends[_key]["message_blocked"]=[[fInfo objectForKey:@"message_blocked"] boolValue];
//            appFriends[_key]["profile_image_url"]=[[fInfo objectForKey:@"profile_image_url"] UTF8String];
//            appFriends[_key]["supported_device"]=[[fInfo objectForKey:@"supported_device"] boolValue];
//            appFriends[_key]["kId"]=[[fInfo objectForKey:@"user_id"] UTF8String];
//            
//            param["kakaoIds"].append([[fInfo objectForKey:@"user_id"] UTF8String]);
//        }
//        
//        Json::StyledWriter wr;
//        CCLog("step1 %s", wr.write(appFriends).c_str());
//        param["delekey"]=_key;
//        this->queryHSPMemberNos(param, 0, this, hsp_selector(hspConnector::kLoadFriendsStep2));
//        
//        //hspConnector::get()->kLoadFriendsStep2(param,_key);
//        
//    }];
//#endif
//
//};



//void hspConnector::kLoadFriendsStep2(Json::Value obj){
//    
//
//    Json::Value param;
//    Json::Value::Members m=obj["list"].getMemberNames();
//    
//    for(auto iter = m.begin();iter!=m.end();++iter){
//        appFriends[*iter]["hNo"]=obj["list"][*iter].asInt64();
//        param["memberNos"].append(obj["list"][*iter].asInt64());
//    }
//    
//    Json::StyledWriter wr;
//    CCLog("step0 %s", wr.write(obj).c_str());
//    CCLog("step2 %s", wr.write(appFriends).c_str());
//    
//    param["delekey"]=obj["param"]["delekey"].asInt();
//    
//    this->loadDetailedProfiles(param, 0, this, hsp_selector(hspConnector::kLoadFriendsStep3));
//  
//    
//}


//void hspConnector::kLoadFriendsStep3(Json::Value obj){
//    
//    Json::Value::Members m = appFriends.getMemberNames();
//    
//    for(auto iter=m.begin();iter!=m.end();++iter){
//        for(int i=0;i<obj["profiles"].size();i++){
//            if(obj["profiles"][i]["memberNo"].asInt64()==appFriends[*iter]["hNo"].asInt64()){
//                appFriends[*iter]["detailed"]=obj["profiles"][i]["detailed"];
//                appFriends[*iter]["exposeOnline"]=appFriends[*iter]["hNo"].asBool();
//                appFriends[*iter]["isValid"]=appFriends[*iter]["isValid"].asBool();
//                appFriends[*iter]["lastLoginDate"]=appFriends[*iter]["lastLoginDate"].asString();
//                appFriends[*iter]["recentPlayedGameNo"]=appFriends[*iter]["recentPlayedGameNo"].asInt();
//                break;
//            }
//        }
//    }
//    
//    int delkey = obj["param"]["delekey"].asInt();
//    hspDelegator::DeleSel delesel = hspDelegator::get()->load(delkey);
//    ((delesel.target)->*(delesel.selector))(0);
//    hspDelegator::get()->remove(delkey);
//}



// kakao only
//void hspConnector::kLoadLocalUser(Json::Value callbackParam, jsonSelType func){
//    CCLog("kLoadLocalUser function");
//    Json::Value param;
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    [[KALocalUser localUser] loadLocalUserWithCompletionHandler:^(NSDictionary *response, NSError *error) {
//     NSDictionary *resultDict = response;
//     if(error){
//     resultDict = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:error.code],@"status",[error.userInfo objectForKey:@"NSLocalizedDescription"],@"message",nil];
//     }
//     
//     callFuncMainQueue2(param,callbackParam,func,resultDict);
//     
//     }];
//    
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    JniMethodInfo t;
//    if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "kLoadLocalUser", "(I)V")) {
//        int _key =  jsonDelegator::get()->add(func,param,callbackParam);
//    	t.env->CallStaticObjectMethod(t.classID, t.methodID,_key);
//        t.env->DeleteLocalRef(t.classID);
//    }
//#endif
//}



//void hspConnector::kLoadLocalUser(jsonSelType func){
//    Json::Value callbackParam;
//    this->kLoadLocalUser(callbackParam,func);
//}



//void hspConnector::kLoadFriends(Json::Value callbackParam,jsonSelType func){
//    Json::Value param;
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    [[KALocalUser localUser] loadFriendsWithCompletionHandler:^(NSDictionary *response, NSError *error) {
//     
//     NSDictionary *resultDict = response;
//     if(error){
//     resultDict = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:error.code],@"status",[error.userInfo objectForKey:@"NSLocalizedDescription"],@"message",nil];
//     }
//     callFuncMainQueue2(param,callbackParam,func,resultDict);
//     }];
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    JniMethodInfo t;
//    if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "kLoadFriends", "(I)V")) {
//        int _key =  jsonDelegator::get()->add(func,param,callbackParam);
//    	t.env->CallStaticObjectMethod(t.classID, t.methodID,_key);
//        t.env->DeleteLocalRef(t.classID);
//    }
//#endif
//}
//
//void hspConnector::kLoadFriends(jsonSelType func){
//    Json::Value callbackParam;
//    this->kLoadFriends(callbackParam,func);
//}




//
//void hspConnector::kSendMessage(Json::Value param,Json::Value callbackParam,jsonSelType func){
//    
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
//    NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
//    NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
//    
//    KALinkMessageRequest *request = nil;
//    
//    request = [KALinkMessageRequest requestWithReceiverID:receiverID
//                                                  message:message
//                                         executeURLString:executeURLString];
//    
//    [[KALocalUser localUser] sendLinkMessageWithRequest:request completionHandler:^(BOOL success, NSError *error) {
//     
//     
//     NSDictionary *resultDict;
//     if(error){
//     resultDict = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:error.code],@"status",[error.userInfo objectForKey:@"NSLocalizedDescription"],@"message",nil];
//     }else{
//     resultDict = [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:0] forKey:@"status"];
//     }
//     callFuncMainQueue2(param,callbackParam,func,resultDict);
//     }];
//    
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    JniMethodInfo t;
//    if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "kSendMessage", "(ILjava/lang/String;)V")) {
//        int _key =  jsonDelegator::get()->add(func,param,callbackParam);
//        ostringstream oss;
//        oss << param;
//    	t.env->CallStaticObjectMethod(t.classID, t.methodID,_key, t.env->NewStringUTF(oss.str().c_str()));
//        t.env->DeleteLocalRef(t.classID);
//    }
//#endif
//}
//
//void hspConnector::kSendMessage(Json::Value param,jsonSelType func){
//    Json::Value callbackParam;
//    this->kSendMessage(param,callbackParam,func);
//}
//
//void hspConnector::kLogout(Json::Value callbackParam,jsonSelType func){
//    Json::Value param;
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    [[KALocalUser localUser] logoutWithCompletionHandler:^(BOOL success, NSError *error) {
//     
//     NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
//     if (success==true) {
//     [resultDict setObject:[NSNumber numberWithInt:0] forKey:@"status"];
//     } else {
//     [resultDict setObject:[NSNumber numberWithInt:-500] forKey:@"status"];
//     [resultDict setObject:[error.userInfo objectForKey:@"NSLocalizedDescription"] forKey:@"message"];
//     }
//     callFuncMainQueue2(param,callbackParam,func,resultDict);
//     }];
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    JniMethodInfo t;
//    if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "kLogout", "(I)V")) {
//        int _key =  jsonDelegator::get()->add(func,param,callbackParam);
//        t.env->CallStaticObjectMethod(t.classID, t.methodID,_key);
//        t.env->DeleteLocalRef(t.classID);
//    }
//#endif
//    
//}
//
//void hspConnector::kLogout(jsonSelType func){
//    Json::Value callbackParam;
//    this->kLogout(callbackParam,func);
//}
//
//
//void hspConnector::kUnregister(Json::Value callbackParam,jsonSelType func){
//    Json::Value param;
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    [[KALocalUser localUser] unregisterWithCompletionHandler:^(BOOL success, NSError *error) {
//     NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
//     if (success==true) {
//     [resultDict setObject:[NSNumber numberWithInt:0] forKey:@"status"];
//     } else {
//     [resultDict setObject:[NSNumber numberWithInt:-500] forKey:@"status"];
//     [resultDict setObject:[error.userInfo objectForKey:@"NSLocalizedDescription"] forKey:@"message"];
//     }
//     callFuncMainQueue2(param,callbackParam,func,resultDict);
//     
//     }];
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    JniMethodInfo t;
//    if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "kUnregister", "(I)V")) {
//        int _key =  jsonDelegator::get()->add(func,param,callbackParam);
//        t.env->CallStaticObjectMethod(t.classID, t.methodID,_key);
//        t.env->DeleteLocalRef(t.classID);
//    }
//#endif
//}
//
//
//void hspConnector::kUnregister(jsonSelType func){
//    Json::Value param;
//    this->kUnregister(param,func);
//}


