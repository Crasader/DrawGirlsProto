
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
#import "HSPServiceProperties.h"
#import "HSPCgp.h"
#import "HSPUtil.h"
#import "AdXTracking.h"
#import "HSPMessage.h"
#import "AppController.h"
#import "HSPPayment.h"
#import "HSPUiLauncher.h"
#import "HSPItemDelivery.h"
#import "KakaoLinkCenter.h"
#import  <IgaworksAD/AdBrix.h>
#include "HSPEnums.h"
//#import "HSPUiReference.h"
//#import "HSPKakao.h"
//#import "Kakao.h"
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import "AppController.h"
#import <sys/types.h>
#import <sys/sysctl.h>
#import "TagAnalytics.h"
//#import "KALocalUser.h"
//#import "KAAuth.h"
//#import "KakaoSDKViewController.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "TAGAnalyticsCpp.h"
#include "platform/android/jni/JniHelper.h"
#include "jni.h"
#endif
#include <string>
#include <sstream>
#include <algorithm>
#include "AccountManagerPopup.h"
using namespace std;
USING_NS_CC;

#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
using namespace rapidxml;
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
		
		
		//env->ReleaseStringUTFChars(revStr2, revStr);
		
	}
	
	void Java_com_litqoo_lib_hspConnector_SendResultNative(JNIEnv *env, jobject thiz,int _key, jstring datas, bool isFinish)
	{
		CCLOG("sendresultnative1 %d", _key);
		jsonDelegator::DeleSel *delesel = jsonDelegator::get()->load(_key);
        if(delesel==nullptr)return;
		jboolean isCopy = JNI_FALSE;
		const char* revStr = env->GetStringUTFChars(datas, &isCopy);
		string throwData = revStr;
		
		//CCLOG("sendresultnative1 - %s",delesel->result.c_str());
		//CCLOG("sendresultnative2 - %s",throwData.c_str());
		
		if(throwData!=""){
			delesel->result=delesel->result+throwData;
		}
		
	//	CCLOG("sendresultnative3 %s",throwData.c_str());
		//CCLOG("sendresultnative3-1 %s",delesel->result.c_str());
		
		if(delesel->func!=NULL)
		{
			
			CCLOG("sendresultnative4");
			if(isFinish){
				
				//CCLOG("sendresultnative5 %s",delesel->result.c_str());
				Json::Value resultObj;
				Json::Reader rd;
				rd.parse(delesel->result.c_str(),resultObj);
				
				//
				CCLOG("sendresultnative6-1");
				resultObj["param"] = delesel->param;
				CCLOG("sendresultnative6-2");
				resultObj["callback"] = delesel->callbackParam;
				CCLOG("sendresultnative6-3");
				//((delesel.target)->*(delesel.selector))(resultObj);
				delesel->func(resultObj);
				
				
				
				CCLOG("sendresultnative7");
			}
		}
		
		if(isFinish) jsonDelegator::get()->remove(_key);
		
		CCLOG("sendresultnative8");
		env->ReleaseStringUTFChars(datas, revStr);
		
		CCLOG("sendresultnative9");
		return;
	}
	void Java_com_litqoo_lib_hspConnector_SendReactionNative(JNIEnv *env, jobject thiz,int _key, jstring datas, bool isFinish)
	{
		jsonDelegator::DeleSel *delesel = jsonDelegator::get()->load(_key);
        if(delesel==nullptr)return;
		jboolean isCopy = JNI_FALSE;
		const char* revStr = env->GetStringUTFChars(datas, &isCopy);
		string throwData = revStr;
		delesel->result.append(throwData);
		//CCLOG("sendresultnative3 %s",throwData.c_str());
		if(delesel->func!=NULL)
		{
			if(isFinish)
			{
			//	CCLOG("sendreactionnative5 %s",delesel->result.c_str());
				Json::Value resultObj;
				Json::Reader rd;
				rd.parse(delesel->result.c_str(),resultObj);
				resultObj["param"] = delesel->param;
				resultObj["callback"] = delesel->callbackParam;
				//((delesel.target)->*(delesel.selector))(resultObj);
				delesel->func(resultObj);
			}
		}
		
		// 반응은 계속적으로 받아야 되는것이기 때문에 딜리게이트를 삭제 안함. 수동으로 삭제 해야 함
//		if(isFinish)
//		{
//			jsonDelegator::get()->remove(_key);
//		}
		
		env->ReleaseStringUTFChars(datas, revStr);
		
		return;
	}

	int Java_com_nhnent_SKSUMRAN_DGproto_getUserState(JNIEnv *env, jobject thiz)
	{
		jboolean isCopy = JNI_FALSE;
//		hspConnector::get()->setup(_gameID,hspGameNo,_gameVersion);
		return getSavedOAuthType((int)HSPLogin::GUEST);
		
		//env->ReleaseStringUTFChars(revStr2, revStr);
		
	}
	int Java_com_nhnent_SKSUMRAN_NA_DGproto_getUserState(JNIEnv *env, jobject thiz)
	{
		jboolean isCopy = JNI_FALSE;
		//		hspConnector::get()->setup(_gameID,hspGameNo,_gameVersion);
		return getSavedOAuthType((int)HSPLogin::GUEST);
		
		//env->ReleaseStringUTFChars(revStr2, revStr);
		
	}
	void Java_com_litqoo_lib_hspConnector_requestItemDeliveryNative(JNIEnv *env, jobject thiz)
	{
		if(GraphDog::get()->getMemberID())
		{
			jboolean isCopy = JNI_FALSE;
			Json::Value param;
			param["memberID"] = hspConnector::get()->getMemberID();
			GraphDog::get()->command("requestItemDelivery", param, [=](Json::Value t){
				if(t["result"]["code"].asInt() == GDSUCCESS)
				{
				}
				else if(t["result"]["code"].asInt() == 2016) // GDNOTINGWORK
				{
					//				addChild(KSTimer::create(3.f, [=](){requestItemDelivery();}));
				}
				else
				{
					//				addChild(KSTimer::create(3.f, [=](){requestItemDelivery();}));
				}
			});
			
		}
	}
}
#endif


long long int hspConnector::getMemberID(){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return [[HSPCore sharedHSPCore] memberNo];
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

	CCLOG("call sendPushMessage");
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "sendPushMessage", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		CCLOG("result sendPushMessage");
		//		jlong result = t.env->CallStaticLongMethod(t.classID, t.methodID);
		//		t.env->DeleteLocalRef(t.classID);
		//		return (long long int)result;
	}
#endif
}

string hspConnector::getSocialID(){
	return graphdog->getSocialID();
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
	CCLOG("%s -- %s",tag.c_str(),wr.write(value).c_str());
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
	CCLOG("loadprofile param string %s",wr.write(param).c_str());
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


void callFuncMainQueue2(Json::Value param,Json::Value callbackParam,int _dkey,void*resultDict){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	TRACE();
	NSData *_jdata= [NSJSONSerialization dataWithJSONObject:(NSDictionary *)resultDict options:NSJSONWritingPrettyPrinted error:nil];
	NSString *jsonString = [[NSString alloc] initWithData:_jdata encoding:NSUTF8StringEncoding];
	Json::Reader reader;
	Json::Value resultObj;

	TRACE();
	reader.parse([jsonString cStringUsingEncoding:NSUTF8StringEncoding], resultObj);
	TRACE();
	jsonDelegator::DeleSel *delesel = jsonDelegator::get()->load(_dkey);
	TRACE();
	if(delesel==nullptr)return;
	if(!param.isNull())resultObj["param"] = param;
	if(!callbackParam.isNull())resultObj["callback"] = callbackParam;
	dispatch_async(dispatch_get_main_queue(),
								 ^{
									 TRACE();
									 if(delesel->func != NULL)
									 {
										 delesel->func(resultObj);
									 }
									 jsonDelegator::get()->remove(_dkey);
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

void hspConnector::mappingToAccount(jsonSelType func){
	int dkey = jsonDelegator::get()->add(func, 0, 0);
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	CCLOG("mapping try!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	[[HSPCore sharedHSPCore] requestMappingToAccountWithCompletionHandler:
	 ^(HSPError *error)
	 {
		 
		 CCLOG("mapping callback!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		 NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
		 addErrorInResult(resultDict, error);
		 callFuncMainQueue2(0,0,dkey,resultDict);
	 }
	 ];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "mappingToAccount", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, dkey);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}


string hspConnector::getCountryCode(){
	string r;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	NSLocale *currentLocale = [NSLocale currentLocale];  // get the current locale.
	NSString *countryCode = [currentLocale objectForKey:NSLocaleCountryCode];
	r = [countryCode UTF8String];

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "getCountryCode", "()Ljava/lang/String;")) {
		jstring result = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		
		jboolean isCopy = JNI_FALSE;
		const char* revStr = t.env->GetStringUTFChars(result, &isCopy);
		r = revStr;
	
		t.env->ReleaseStringUTFChars(result, revStr);
		t.env->DeleteLocalRef(t.classID);
		
	}
#endif

	std::transform(r.begin(), r.end(), r.begin(), towlower);
	
	if(r=="")r="kr";
	
	return r;
}
string hspConnector::getMarketCode(){
	string r;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	r = "KS";
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "getMarketCode", "()Ljava/lang/String;")) {
		jstring result = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		
		jboolean isCopy = JNI_FALSE;
		const char* revStr = t.env->GetStringUTFChars(result, &isCopy);
		r = revStr;
		
		t.env->ReleaseStringUTFChars(result, revStr);
		t.env->DeleteLocalRef(t.classID);
		
	}
#endif
	return r;
}


string hspConnector::getStoreID()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	//	return "TS";
	unsigned long ps;
	//	CCLOG("%s", CCFileUtils::sharedFileUtils()->fullPathForFilename("HSPConfiguration__.xml").c_str());
	unsigned char* tt = CCFileUtils::sharedFileUtils()->getFileData("HSPConfiguration.xml", "rt", &ps);
	std::string xml((char*)tt, ps);
	// key="HSP_PAYMENT_STORE_ID" value="TS"
	xml_document<> xmlDoc;
	xmlDoc.parse<0>( (char*)xml.c_str() );
	
	// root 포인터
	xml_node<  >* root = xmlDoc.first_node();
	xml_node<  >* Item;
	xml_node< >* SubItem;
	char* Name;
	char* Value;
	xml_attribute< >* Attr;
	// root 하위 Node 탐색
	Item = root->first_node();
	for(xml_node<>* node = Item; node; node = node->next_sibling())
	{
		Attr = node->first_attribute();
		//			CCLog("%s", Attr->name());
		Name = Attr->value();
		if(Name == std::string("HSP_PAYMENT_STORE_ID"))
		{
			Attr = Attr->next_attribute();
			Name  = Attr->name();	// Attribute 의 이름
			Value = Attr->value();	// Attribute 의 값
			return Value;
		}
	}
	
	return "";
#else
	return "AS";
#endif
}
string hspConnector::getHSPConfigurationProp(const std::string& prop)
{
	//	return "TS";
	unsigned long ps;
	//	CCLOG("%s", CCFileUtils::sharedFileUtils()->fullPathForFilename("HSPConfiguration__.xml").c_str());
	unsigned char* tt = CCFileUtils::sharedFileUtils()->getFileData("HSPConfiguration.xml", "rt", &ps);
	std::string xml((char*)tt, ps);
	// key="HSP_PAYMENT_STORE_ID" value="TS"
	xml_document<> xmlDoc;
	xmlDoc.parse<0>( (char*)xml.c_str() );
	
	// root 포인터
	xml_node<  >* root = xmlDoc.first_node();
	xml_node<  >* Item;
	xml_node< >* SubItem;
	char* Name;
	char* Value;
	xml_attribute< >* Attr;
	// root 하위 Node 탐색
	Item = root->first_node();
	for(xml_node<>* node = Item; node; node = node->next_sibling())
	{
		Attr = node->first_attribute();
		//			CCLog("%s", Attr->name());
		Name = Attr->value();
		if(Name == prop)
		{
			Attr = Attr->next_attribute();
			Name  = Attr->name();	// Attribute 의 이름
			Value = Attr->value();	// Attribute 의 값
			return Value;
		}
	}
	return "";
}
string hspConnector::getTimeZone(){
	
	string r;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	NSTimeZone *timeZone = [NSTimeZone localTimeZone];
	NSString *tzName = [timeZone name];
	r = [tzName cStringUsingEncoding:NSUTF8StringEncoding];
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "getTimeZone", "()Ljava/lang/String;")) {
		jstring result = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		
		jboolean isCopy = JNI_FALSE;
		const char* revStr = t.env->GetStringUTFChars(result, &isCopy);
		r = revStr;
		
		t.env->ReleaseStringUTFChars(result, revStr);
		t.env->DeleteLocalRef(t.classID);
		
	}
#endif
	
	
	
	//std::transform(r.begin(), r.end(), r.begin(), towlower);
	CCLOG("mytime zone is : %s",r.c_str());
	return r.c_str();
}


string hspConnector::getServerAddress(){
	string serverAddr;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	HSPServiceProperties* properties = [HSPCore sharedHSPCore].serviceProperties;
	NSString* gameServerAddress = [properties serverAddressFromName: HSP_SERVERNAME_GAMESVR];
	serverAddr = [gameServerAddress cStringUsingEncoding:NSUTF8StringEncoding];
	
	// plist 얻어와서 작업해야 하는데 일단은 그냥
	serverAddr = "http://182.162.201.147:10010";
	
//	serverAddr = "http://182.162.196.182:10080";

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "getServerAddress", "()Ljava/lang/String;")) {
		jstring result = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		
		jboolean isCopy = JNI_FALSE;
		const char* revStr = t.env->GetStringUTFChars(result, &isCopy);
		serverAddr = revStr;
		
		t.env->ReleaseStringUTFChars(result, revStr);
		t.env->DeleteLocalRef(t.classID);
	}
	serverAddr = "http://" + serverAddr;
	
	string isDevelop = getHSPConfigurationProp("LIT_DEVELOP_SERVER");
	if(isDevelop == "true")
	{
		serverAddr = "http://182.162.201.147:10010";
	}
	
	
#endif
	
	return serverAddr.c_str();
	//std::transform(r.begin(), r.end(), r.begin(), towlower);
	
	//return r;
}
void hspConnector::withdrawAccount(jsonSelType func)
{
		int _key =  jsonDelegator::get()->add(func,0,0);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[[HSPCore sharedHSPCore] withdrawAccountWithCompletionHandler:
	 ^(HSPError *error)
	 {
		 NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
		 addErrorInResult(resultDict, error);
		 callFuncMainQueue2(0,0,_key,resultDict);
	 }
	 ];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "withdrawAccount", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID,_key);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}

void hspConnector::logout(jsonSelType func){
		int _key =  jsonDelegator::get()->add(func,0,0);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[[HSPCore sharedHSPCore] logoutWithCompletionHandler:
		^(HSPError *error)
		{
			NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
			addErrorInResult(resultDict, error);
			callFuncMainQueue2(0,0,_key,resultDict);
		}
	];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "logout", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID,_key);
		t.env->DeleteLocalRef(t.classID);
	}
#endif

}

void hspConnector::login(Json::Value param,Json::Value callbackParam,jsonSelType func){
	int dkey = jsonDelegator::get()->add(func, 0, 0);
	TRACE();
	jsonSelType nextFunc = [dkey,this](Json::Value obj){
		TRACE();
		KS::KSLog("aaaa %", obj);
		int delekey = dkey;
		
		if(obj["error"]["isSuccess"].asBool()){
			graphdog->setMemberID(hspConnector::get()->getMemberID());
			GraphDog::get()->setServerURL(this->getServerAddress());
			string hspids = CCString::createWithFormat("%lld",hspConnector::get()->getMemberID())->getCString();
			CCLOG("member id is %s",hspids.c_str());
			graphdog->setSocialID(hspids);
		}
		
		jsonDelegator::DeleSel *delsel = jsonDelegator::get()->load(delekey);
        if(delsel==nullptr)return;
		if(delsel->func)delsel->func(obj);
		jsonDelegator::get()->remove(delekey);


		};


	TRACE();
	int _key =  jsonDelegator::get()->add(nextFunc,param,callbackParam);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#ifdef LQTEST	
	[[HSPCore sharedHSPCore] loginWithOAuthProvider:HSP_OAUTHPROVIDER_GUEST completionHandler:
	 ^(BOOL playable, HSPError* error) {
		 TRACE();
		 KS::KSLog("loginWithOAuthProvider callback, member id = %", myHSP->getMemberID());
		 // 로그인 응답 처리
		 NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
		 [resultDict setObject:[NSNumber numberWithBool:playable] forKey:@"playable"];
		 TRACE();
		 addErrorInResult(resultDict, error);
		 callFuncMainQueue2(param,callbackParam, _key,resultDict);
		 TRACE();
	 }];
#endif
#ifndef LQTEST
	HSPLoginTypeX loginType = (HSPLoginTypeX)myHSP->getLoginType();
	// 이미 로그인이 되어있으면 로그인을 따로 안함.
	if(loginType != HSPLoginTypeX::HSPLoginTypeUNKNOWN)
	{
		NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
//		[resultDict setObject:[NSNumber numberWithBool:playable] forKey:@"playable"];
		TRACE();
		NSMutableDictionary* error = [NSMutableDictionary dictionary];
		[error setObject:[NSNumber numberWithBool:YES] forKey:@"isSuccess"];
		[resultDict setObject:error forKey:@"error"];
		
//		addErrorInResult(resultDict, error);
		callFuncMainQueue2(param,callbackParam, _key,resultDict); 
	}
	else
	{
		int LoginType = (int)HSPLogin::GAMECENTER;
	 HSPOAuthProvider loginT = (HSPOAuthProvider)param.get("LoginType", LoginType).asInt();
	 CCLOG("loginWithOAuthProvider with (login type) : %d", (int)loginT);
		[[HSPCore sharedHSPCore] loginWithOAuthProvider:loginT completionHandler:
		 ^(BOOL playable, HSPError* error) {
			 TRACE();
			 KS::KSLog("loginWithOAuthProvider callback, member id = %", myHSP->getMemberID());
			 // 로그인 응답 처리
			 NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
			 [resultDict setObject:[NSNumber numberWithBool:playable] forKey:@"playable"];
			 TRACE();
			 addErrorInResult(resultDict, error);
			 callFuncMainQueue2(param,callbackParam, _key,resultDict);
			 TRACE();
		 }];
		
	}
	
#endif
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	bool ManualLogin =true;
	int LoginType = (int)HSPLogin::GUEST;
	if(param!=0 && param!=NULL){
		ManualLogin = param.get("ManualLogin",true).asBool();
		LoginType = param.get("LoginType", LoginType).asInt();
	}

	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "login", "(IZI)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID,_key,ManualLogin, LoginType);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
	 TRACE();
}

double hspConnector::getScreenRealWidth()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	jint ret = -1;
	JniMethodInfo t;
	CCLog("getScreenRealWidth");
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "getScreenRealWidth", "()D")) {
		//		int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
		ret = t.env->CallStaticDoubleMethod(t.classID, t.methodID);
		CCLog("ret = %d ", ret);
		t.env->DeleteLocalRef(t.classID);
	}
	return (double)ret;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	size_t size = 100;
    char *hw_machine = (char*)malloc(size);
    int name[] = {CTL_HW,HW_MACHINE};
    sysctl(name, 2, hw_machine, &size, NULL, 0);
    NSString *hardware = [NSString stringWithUTF8String:hw_machine];
    free(hw_machine);
    if ([hardware isEqualToString:@"iPhone1,1"])	return 2.912176;//return "iPhone 2G";
    if ([hardware isEqualToString:@"iPhone1,2"])	return 2.912176;//return "iPhone 3G";
    if ([hardware isEqualToString:@"iPhone2,1"])	return 2.912176;//return "iPhone 3GS";
    if ([hardware isEqualToString:@"iPhone3,1"])	return 2.912176;//return "iPhone 4";
    if ([hardware isEqualToString:@"iPhone3,2"])	return 2.912176;//return "iPhone 4";
    if ([hardware isEqualToString:@"iPhone3,3"])	return 2.912176;//return "iPhone 4 (CDMA)";
    if ([hardware isEqualToString:@"iPhone4,1"])	return 2.912176;//return "iPhone 4S"; 3.5inch 480:320 | 960:640 | 2.912176:1.94145
    if ([hardware isEqualToString:@"iPhone5,1"])	return 3.48499;//return "iPhone 5";
    if ([hardware isEqualToString:@"iPhone5,2"])	return 3.48499;//return "iPhone 5 (GSM+CDMA)";
	if ([hardware isEqualToString:@"iPhone5,3"])	return 3.48499;//return "iPhone 5C";
    if ([hardware isEqualToString:@"iPhone5,4"])	return 3.48499;//return "iPhone 5C";
	if ([hardware isEqualToString:@"iPhone6,1"])	return 3.48499;//return "iPhone 5S";
    if ([hardware isEqualToString:@"iPhone6,2"])	return 3.48499;//return "iPhone 5S"; 4inch 1136:640 | 3.48499:1.963375
	
    if ([hardware isEqualToString:@"iPod1,1"])		return 2.912176;//return "iPod Touch (1 Gen)";
    if ([hardware isEqualToString:@"iPod2,1"])		return 2.912176;//return "iPod Touch (2 Gen)";
    if ([hardware isEqualToString:@"iPod3,1"])		return 2.912176;//return "iPod Touch (3 Gen)";
    if ([hardware isEqualToString:@"iPod4,1"])		return 2.912176;//return "iPod Touch (4 Gen)"; 3.5inch 480:320
    if ([hardware isEqualToString:@"iPod5,1"])		return 3.48499;//return "iPod Touch (5 Gen)"; 4inch 1136:640
	
    if ([hardware isEqualToString:@"iPad1,1"])		return 7.76;//return "iPad";
    if ([hardware isEqualToString:@"iPad1,2"])		return 7.76;//return "iPad 3G";
    if ([hardware isEqualToString:@"iPad2,1"])		return 7.76;//return "iPad 2 (WiFi)";
    if ([hardware isEqualToString:@"iPad2,2"])		return 7.76;//return "iPad 2";
    if ([hardware isEqualToString:@"iPad2,3"])		return 7.76;//return "iPad 2 (CDMA)";
    if ([hardware isEqualToString:@"iPad2,4"])		return 7.76;//return "iPad 2";
    if ([hardware isEqualToString:@"iPad2,5"])		return 6.32;//return "iPad Mini (WiFi)";
    if ([hardware isEqualToString:@"iPad2,6"])		return 6.32;//return "iPad Mini";
    if ([hardware isEqualToString:@"iPad2,7"])		return 6.32;//return "iPad Mini (GSM+CDMA)";
    if ([hardware isEqualToString:@"iPad3,1"])		return 7.76;//return "iPad 3 (WiFi)";
    if ([hardware isEqualToString:@"iPad3,2"])		return 7.76;//return "iPad 3 (GSM+CDMA)";
    if ([hardware isEqualToString:@"iPad3,3"])		return 7.76;//return "iPad 3";
    if ([hardware isEqualToString:@"iPad3,4"])		return 7.76;//return "iPad 4 (WiFi)";
    if ([hardware isEqualToString:@"iPad3,5"])		return 7.76;//return "iPad 4";
    if ([hardware isEqualToString:@"iPad3,6"])		return 7.76;//return "iPad 4 (GSM+CDMA)";
	if ([hardware isEqualToString:@"iPad4,1"])		return 6.32;//return "iPad Air";
    if ([hardware isEqualToString:@"iPad4,2"])		return 6.32;//return "iPad Air (CDMA)";
	if ([hardware isEqualToString:@"iPad4,3"])		return 6.32;//return "iPad Air China";
	if ([hardware isEqualToString:@"iPad4,4"])		return 6.32;//return "iPad Mini 2G";
    if ([hardware isEqualToString:@"iPad4,5"])		return 6.32;//return "iPad Mini 2G (CDMA)"; // 그냥은 9.7inch | 7.76:5.82, mini는 7.9inch 1024:768 | 6.32:4.74
	
    if ([hardware isEqualToString:@"i386"])			return 3.48499;//return "Simulator";
    if ([hardware isEqualToString:@"x86_64"])		return 3.48499;//return "Simulator";
	
	return 3.48499;
	
	// not implementation
	//	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s",url.c_str()]]];
#endif
	
}

double hspConnector::getScreenRealHeight()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	jint ret = -1;
	JniMethodInfo t;
	CCLog("getScreenRealHeight");
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "getScreenRealHeight", "()D")) {
		//		int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
		ret = t.env->CallStaticDoubleMethod(t.classID, t.methodID);
		CCLog("ret = %d ", ret);
		t.env->DeleteLocalRef(t.classID);
	}
	return (double)ret;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	size_t size = 100;
    char *hw_machine = (char*)malloc(size);
    int name[] = {CTL_HW,HW_MACHINE};
    sysctl(name, 2, hw_machine, &size, NULL, 0);
    NSString *hardware = [NSString stringWithUTF8String:hw_machine];
    free(hw_machine);
    if ([hardware isEqualToString:@"iPhone1,1"])	return 1.94145;//return "iPhone 2G";
    if ([hardware isEqualToString:@"iPhone1,2"])	return 1.94145;//return "iPhone 3G";
    if ([hardware isEqualToString:@"iPhone2,1"])	return 1.94145;//return "iPhone 3GS";
    if ([hardware isEqualToString:@"iPhone3,1"])	return 1.94145;//return "iPhone 4";
    if ([hardware isEqualToString:@"iPhone3,2"])	return 1.94145;//return "iPhone 4";
    if ([hardware isEqualToString:@"iPhone3,3"])	return 1.94145;//return "iPhone 4 (CDMA)";
    if ([hardware isEqualToString:@"iPhone4,1"])	return 1.94145;//return "iPhone 4S"; 3.5inch 480:320 | 960:640 | 2.912176:1.94145
    if ([hardware isEqualToString:@"iPhone5,1"])	return 1.963375;//return "iPhone 5";
    if ([hardware isEqualToString:@"iPhone5,2"])	return 1.963375;//return "iPhone 5 (GSM+CDMA)";
	if ([hardware isEqualToString:@"iPhone5,3"])	return 1.963375;//return "iPhone 5C";
    if ([hardware isEqualToString:@"iPhone5,4"])	return 1.963375;//return "iPhone 5C";
	if ([hardware isEqualToString:@"iPhone6,1"])	return 1.963375;//return "iPhone 5S";
    if ([hardware isEqualToString:@"iPhone6,2"])	return 1.963375;//return "iPhone 5S"; 4inch 1136:640 | 3.48499:1.963375
	
    if ([hardware isEqualToString:@"iPod1,1"])		return 1.94145;//return "iPod Touch (1 Gen)";
    if ([hardware isEqualToString:@"iPod2,1"])		return 1.94145;//return "iPod Touch (2 Gen)";
    if ([hardware isEqualToString:@"iPod3,1"])		return 1.94145;//return "iPod Touch (3 Gen)";
    if ([hardware isEqualToString:@"iPod4,1"])		return 1.94145;//return "iPod Touch (4 Gen)"; 3.5inch 480:320
    if ([hardware isEqualToString:@"iPod5,1"])		return 1.963375;//return "iPod Touch (5 Gen)"; 4inch 1136:640
	
    if ([hardware isEqualToString:@"iPad1,1"])		return 5.82;//return "iPad";
    if ([hardware isEqualToString:@"iPad1,2"])		return 5.82;//return "iPad 3G";
    if ([hardware isEqualToString:@"iPad2,1"])		return 5.82;//return "iPad 2 (WiFi)";
    if ([hardware isEqualToString:@"iPad2,2"])		return 5.82;//return "iPad 2";
    if ([hardware isEqualToString:@"iPad2,3"])		return 5.82;//return "iPad 2 (CDMA)";
    if ([hardware isEqualToString:@"iPad2,4"])		return 5.82;//return "iPad 2";
    if ([hardware isEqualToString:@"iPad2,5"])		return 4.74;//return "iPad Mini (WiFi)";
    if ([hardware isEqualToString:@"iPad2,6"])		return 4.74;//return "iPad Mini";
    if ([hardware isEqualToString:@"iPad2,7"])		return 4.74;//return "iPad Mini (GSM+CDMA)";
    if ([hardware isEqualToString:@"iPad3,1"])		return 5.82;//return "iPad 3 (WiFi)";
    if ([hardware isEqualToString:@"iPad3,2"])		return 5.82;//return "iPad 3 (GSM+CDMA)";
    if ([hardware isEqualToString:@"iPad3,3"])		return 5.82;//return "iPad 3";
    if ([hardware isEqualToString:@"iPad3,4"])		return 5.82;//return "iPad 4 (WiFi)";
    if ([hardware isEqualToString:@"iPad3,5"])		return 5.82;//return "iPad 4";
    if ([hardware isEqualToString:@"iPad3,6"])		return 5.82;//return "iPad 4 (GSM+CDMA)";
	if ([hardware isEqualToString:@"iPad4,1"])		return 4.74;//return "iPad Air";
    if ([hardware isEqualToString:@"iPad4,2"])		return 4.74;//return "iPad Air (CDMA)";
	if ([hardware isEqualToString:@"iPad4,3"])		return 4.74;//return "iPad Air China";
	if ([hardware isEqualToString:@"iPad4,4"])		return 4.74;//return "iPad Mini 2G";
    if ([hardware isEqualToString:@"iPad4,5"])		return 4.74;//return "iPad Mini 2G (CDMA)"; // 그냥은 9.7inch | 7.76:5.82, mini는 7.9inch 1024:768 | 6.32:4.74
	
    if ([hardware isEqualToString:@"i386"])			return 1.963375;//return "Simulator";
    if ([hardware isEqualToString:@"x86_64"])		return 1.963375;//return "Simulator";
	
	return 1.963375;
	
	// not implementation
	//	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s",url.c_str()]]];
#endif
	
}

void hspConnector::checkCGP(Json::Value param,Json::Value callbackParam,jsonSelType func)
{
	int _key =  jsonDelegator::get()->add(func, param, callbackParam);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//	int dkey = jsonDelegator::get()->add(func, 0, 0);
//	jsonSelType nextFunc = [=](Json::Value obj){
//		int delekey = dkey;
//		jsonDelegator::DeleSel *delsel = jsonDelegator::get()->load(delekey);
//		if(delsel->func){
//			delsel->func(obj);
//		}
//		jsonDelegator::get()->remove(delekey);
//		
//	};
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "checkCGP", "(I)Z")) {
		
		t.env->CallStaticObjectMethod(t.classID, t.methodID, _key);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	if(mySGD->getIosMenuVisible() && (mySGD->getIosHideVer() != graphdog->getAppVersionString()) == false)
		return;
	[HSPCGP checkPromotionWithCompletionHandler:^(PromotionState promotionState,  HSPError* error)
	 {
		 NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
		 
		 if ([error isSuccess])
		 {
			 switch (promotionState)
			 {
				 case CGP_NONE:
				 {
					 [resultDict setObject:@"CGP_NONE" forKey:@"promotionstate"];
					 // 프로모션 없음
					 break;
				 }
				 case CGP_PROMOTION_EXISTS:
				 {
					 [resultDict setObject:@"CGP_PROMOTION_EXISTS" forKey:@"promotionstate"];
					 PromotionInfo *promoInfo = [HSPCGP getPromotionInfo];
					 switch(promoInfo->typeCode)
					 {
						 case 1:
							 [resultDict setObject:[NSString stringWithUTF8String:promoInfo->eventUrl] forKey:@"eventurl"];
							 [resultDict setObject:[NSString stringWithUTF8String:promoInfo->buttonUrl] forKey:@"buttonurl"];
							 [resultDict setObject:[NSString stringWithUTF8String:promoInfo->bubbleText] forKey:@"bubbletext"];
							 
							 break;
						 case 2:
							 [resultDict setObject:[NSString stringWithUTF8String:promoInfo->bannerLandUrl] forKey:@"bannerlandurl"];
							 [resultDict setObject:[NSString stringWithUTF8String:promoInfo->bannerPortUrl] forKey:@"bannerporturl"];
							 break;
					 }
					 
					 // 노출해야 할 프로모션 있음
					 break;
				 }
				 case CGP_REWARD_REQUIRED:
				 {
					 [resultDict setObject:@"CGP_REWARD_REQUIRED" forKey:@"promotionstate"];
					 PromotionInfo *promoInfo = [HSPCGP getPromotionInfo];
					 // 이용자에게 보상해야할 정보가 있음
					 
					 NSMutableArray* rewardArray = [NSMutableArray array];
					 NSMutableDictionary* rewardDict = [NSMutableDictionary dictionary];
					 [rewardDict setObject:[NSNumber numberWithInt:promoInfo->rewardValue] forKey:@"rewardvalue"];
					 [rewardDict setObject:[NSString stringWithUTF8String:promoInfo->rewardCode] forKey:@"rewardcode"];
					 [rewardDict setObject:[NSNumber numberWithInt:promoInfo->promotionId] forKey:@"promotiontype"];
					 
					 [rewardArray addObject:rewardDict];
					 [resultDict setObject:rewardArray forKey:@"rewards"];
					 break;
				 }
			 }
		 }
		 addErrorInResult(resultDict, error);
		 callFuncMainQueue2(0,0,_key,resultDict);
	 }];
#endif
}


void hspConnector::checkCGP(Json::Value param,Json::Value callbackParam, CCObject* target, jsonSelType func)
{
	GraphDog::get()->addTarget(target);
	function<void(Json::Value)> sFunc = [=](Json::Value value){
		CCLOG("checkDelegator sFunc call");
		if(GraphDog::get()->checkTarget(target))
			func(value);
	};
	checkCGP(param, callbackParam, sFunc);
}

/*
 {
 "issuccess":true,
 "info":[
 {
 "productid":"g_10331_001",
 "price":1100,
 "currency":"KRW",
 "productname":"젬 10"
 },
 {
 "productid":"g_10331_002",
 "price":4500,
 "currency":"KRW",
 "productname":"젬 50"
 },
 {
 "productid":"g_10331_003",
 "price":8000,
 "currency":"KRW",
 "productname":"젬 100"
 },
 {
 "productid":"g_10331_004",
 "price":21000,
 "currency":"KRW",
 "productname":"젬 300"
 },
 {
 "productid":"g_10331_005",
 "price":30000,
 "currency":"KRW",
 "productname":"젬 500"
 },
 {
 "productid":"g_10331_006",
 "price":50000,
 "currency":"KRW",
 "productname":"젬 1000"
 },
 {
 "productid":"g_10331_007",
 "price":4000,
 "currency":"KRW",
 "productname":"젬 100개(첫구매)"
 },
 {
 "productid":"g_10331_008",
 "price":2900,
 "currency":"KRW",
 "productname":"젬 30"
 },
 {
 "productid":"g_10331_009",
 "price":5200,
 "currency":"KRW",
 "productname":"젬 56"
 },
 {
 "productid":"g_10331_010",
 "price":9900,
 "currency":"KRW",
 "productname":"젬 112"
 },
 {
 "productid":"g_10331_011",
 "price":33200,
 "currency":"KRW",
 "productname":"젬 411"
 },
 {
 "productid":"g_10331_012",
 "price":54000,
 "currency":"KRW",
 "productname":"젬 696"
 },
 {
 "productid":"g_10331_013",
 "price":99700,
 "currency":"KRW",
 "productname":"젬 1439"
 },
 {
 "productid":"g_10331_014",
 "price":4450,
 "currency":"KRW",
 "productname":"젬 112"
 },
 {
 "productid":"g_10331_015",
 "price":2900,
 "currency":"KRW",
 "productname":"이벤트 젬 42"
 },
 {
 "productid":"g_10331_016",
 "price":5200,
 "currency":"KRW",
 "productname":"이벤트 젬 78"
 },
 {
 "productid":"g_10331_017",
 "price":9900,
 "currency":"KRW",
 "productname":"이벤트 젬 157"
 },
 {
 "productid":"g_10331_018",
 "price":33200,
 "currency":"KRW",
 "productname":"이벤트 젬 575"
 },
 {
 "productid":"g_10331_019",
 "price":54000,
 "currency":"KRW",
 "productname":"이벤트 젬 974"
 },
 {
 "productid":"g_10331_020",
 "price":99700,
 "currency":"KRW",
 "productname":"이벤트 젬 2015"
 },
 {
 "productid":"g_10331_021",
 "price":33200,
 "currency":"KRW",
 "productname":"스타터팩"
 }
 ]
 }
 */
void hspConnector::requestProductInfos(jsonSelType func)
{
	int _key = jsonDelegator::get()->add(func, 0, 0);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "requestProductInfos", "(I)V")) {
		//		int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, _key);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[HSPPayment requestProductInfosWithCompletionHandler:^(NSArray* productInfos, HSPError* error) {
		
		NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
		
		NSMutableArray* jArray = [NSMutableArray array];
		[resultDict setObject:[NSNumber numberWithInteger:[error isSuccess]] forKey:@"issuccess"];
		if ( [error isSuccess] == YES )
		{
			for ( HSPPaymentProductInfo* productInfo in productInfos )
			{
				NSMutableDictionary *arrayElement = [NSMutableDictionary dictionary];
				/*
				 arrayElement.put("productid", productInfo.getProductID());
				 arrayElement.put("productname", productInfo.getProductName());
				 arrayElement.put("currency", productInfo.getCurrency());
				 arrayElement.put("price", productInfo.getPrice());
				 jArray.put(arrayElement)
				 */
				[arrayElement setObject:[productInfo productID] forKey:@"productid"];
				[arrayElement setObject:[productInfo productName] forKey:@"productname"];
				[arrayElement setObject:[productInfo currency] forKey:@"currency"];
				[arrayElement setObject:[productInfo price] forKey:@"price"];
				[jArray addObject:arrayElement];
			}
			[resultDict setObject:jArray forKey:@"info"];
		}
		else
		{
			NSLog(@"Failed to load productInfo : %@", error);
		}
		callFuncMainQueue2(0,0,_key,resultDict);
	}];
#endif
}
void hspConnector::requestProductInfos(CCObject* target, jsonSelType func)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	GraphDog::get()->addTarget(target);
	function<void(Json::Value)> sFunc = [=](Json::Value value){
		CCLOG("checkDelegator sFunc call");
		if(GraphDog::get()->checkTarget(target))
			func(value);
	};
	requestProductInfos(sFunc);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
}

void hspConnector::completePromotion()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "completePromotion", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[HSPCGP completePromotion];
#endif
	
}
void hspConnector::completeInstallPromotion()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "completeInstallPromotion", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[HSPCGP completePromotionByInstall];
#endif
}
void hspConnector::purchaseProduct(Json::Value param,Json::Value callbackParam,jsonSelType func)
{
	
	int _key =  jsonDelegator::get()->add(func, param, callbackParam);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "purchaseProduct", "(ILjava/lang/String;)Z")) {
		jstring param1 = t.env->NewStringUTF(param.get("productid", "").asString().c_str());
		
		t.env->CallStaticObjectMethod(t.classID, t.methodID, _key, param1);
		t.env->DeleteLocalRef(param1);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[HSPPayment purchaseWithProductId:[NSString stringWithUTF8String:param.get("productid", "").asString().c_str()]
									completionHandler:^(HSPError* error)
	 {
		 NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
		 [resultDict setObject:[NSNumber numberWithBool:[error isSuccess]] forKey:@"issuccess"];
		 NSLog(@"HSPPayment purchase result : %@", error);
		 if ([error isSuccess] == YES)
		 {
			 callFuncMainQueue2(0,0,_key,resultDict);
		 }
		 else
		 {
			 callFuncMainQueue2(0,0,_key,resultDict);
		 }
	 }];
#endif
}
	 
int hspConnector::checkInspection(){

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	jint ret = -1;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "checkInspection", "()I")) {
			
			ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
    }
	
	return (int)ret;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	HSPServiceProperties* properties = [HSPCore sharedHSPCore].serviceProperties;
 
	HSPLaunchingState state = [properties launchingState];
 
	switch ( state )
	{
		case HSP_LAUNCHINGSTATE_CLIENT_VERSION_FAIL:
			return 2;
		case HSP_LAUNCHINGSTATE_HANGAME_INSPECTION:
		case HSP_LAUNCHINGSTATE_GAME_INSPECTION:
		case HSP_LAUNCHINGSTATE_PLATFORM_INSPECTION:
			return 1;
	}
	
	return 0;
	
#endif
	
}
	 
void hspConnector::purchaseProduct(Json::Value param,Json::Value callbackParam, CCObject* target, jsonSelType func)
{
	GraphDog::get()->addTarget(target);
	function<void(Json::Value)> sFunc = [=](Json::Value value){
		CCLOG("checkDelegator sFunc call");
		if(GraphDog::get()->checkTarget(target))
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
		jstring param1 = t.env->NewStringUTF(url.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, param1);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(param1);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s",url.c_str()]]];
#endif
}

void hspConnector::openHSPUrl(const std::string& url)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "openHSPUrl", "(Ljava/lang/String;)V")) {
		//		int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
		jstring param1 = t.env->NewStringUTF(url.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, param1);
		t.env->DeleteLocalRef(param1);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	HSPUri* uriMyProfile = [HSPUri uriWithString:[NSString stringWithFormat:@"%s",url.c_str()]];
	[[HSPUiLauncher sharedLauncher] launchWithUri:uriMyProfile delegate:nil animated:YES];
//	HSPUri* uriMyProfile = [HSPUri uriWithString:@"HSPUI://webview"];
//	[HSPUiFactory]
//	
//	
//	// 게임 위에 해당 HSPUri 인스턴스에 해당하는 HSP화면을 출력한다.
//	[[HSPUiLauncher sharedLauncher] launchWithUri:uriMyProfile delegate:nil animated:YES];

	
//	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s",url.c_str()]]];
#endif
}

void hspConnector::openCSCenter(const std::string& url)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "openCSCenter", "(Ljava/lang/String;)V")) {
		jstring param1 = t.env->NewStringUTF(url.c_str());
		//		int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, param1);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(param1);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	// 내 정보 URI(HSPUI_URI_PROFILE_MYPROFILE)으로 HSPUri 인스턴스를 생성한다.
	HSPUri* uriMyProfile = [HSPUri uriWithString:HSP_COMMON_URI_SUPPORT_CSCENTER];
	
	// 게임 위에 해당 HSPUri 인스턴스에 해당하는 HSP화면을 출력한다.
	[[HSPUiLauncher sharedLauncher] launchWithUri:uriMyProfile delegate:nil animated:YES];
	// not implementation
	//[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s",url.c_str()]]];
#endif

}
void hspConnector::mappingToAccount(int mt, bool force, jsonSelType func)
{
	int _key = jsonDelegator::get()->add(func, 0, 0);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "hspMappingToAccount", "(IIZ)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, _key, (int)mt, force);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//		^(BOOL playable, HSPError* error) {
//			// 로그인 응답 처리
//			NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
//			[resultDict setObject:[NSNumber numberWithBool:playable] forKey:@"playable"];
//
//			addErrorInResult(resultDict, error);
//			callFuncMainQueue2(param,callbackParam,nextFunc,resultDict);
//			
//		}
	
	
	
	[[HSPCore sharedHSPCore] requestMappingToAccountWithMappingType:(HSPMappingType)mt overwrite:force
			completionHandler:^(HSPError *error, int64_t memberNo) {
				CCLOG("mapping callback!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				KS::KSLog("memberNo!!!!!!!!!!!!!!!!!!!!!!!!!!!! %", memberNo);
				if([error code] == (int)HSP_ERR_DOMAIN_LOGINSERVICE && mt == (int)HSPMapping::kGAMECENTER)
				{
					TRACE();
					Json::Value obj;
					NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
					[resultDict setObject:[NSString stringWithFormat:@"<font color=#FFFFFF newline=14>게임센터로 로그인을 재 시도 하시려면</font>"
																 "<font color=#FFAA14 newline=14>앱을 종료 후 [설정->게임센터] 에서 로그인 후</font>"
																 "<font color=#FFAA14>재시도 해 주시기 바랍니다.</font>"] forKey:@"msg"];
					addErrorInResult(resultDict, error);
					callFuncMainQueue2(0,0,_key,resultDict);
					return;
				}
				else
				{
					TRACE();
					Json::Value obj;
					NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
					[resultDict setObject:[NSNumber numberWithLongLong:memberNo] forKey:@"prevMemberNo"];
					addErrorInResult(resultDict, error);
					callFuncMainQueue2(0,0,_key,resultDict);
				}
				
			}];
	
	
//	[[HSPCore sharedHSPCore] requestMappingToAccountWithCompletionHandler:^(HSPError* error) {
//		if ( [error isSuccess] == YES )
//		{
//			NSLog(@"계정 연동에 성공했습니다.");
//		}
//		else
//		{
//			NSLog(@"계정 연동 실패 (%@)", error);
//		}
//	}];
	
//	// not implementation
//	Json::Value test;
//	test["error"]["code"] = 0x0014006D;
//	func(test);
#endif
}
void hspConnector::getIsUsimKorean(jsonSelType func)
{
	int _key = jsonDelegator::get()->add(func, 0, 0);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "getIsUsimKorean", "(I)V")) {
		//		int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, _key);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[HSPUtil alertViewWithToastTermsWithCompletionHandler:^(BOOL agreed) {
		NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
		
//		addErrorInResult(resultDict, error);
		[resultDict setObject:[NSNumber numberWithInt:1] forKey:@"isSuccess"];
		[resultDict setObject:[NSNumber numberWithBool:!agreed] forKey:@"korean"];
		callFuncMainQueue2(0,0,_key,resultDict);
//		CCLOG("agreed : %d", agreed);
//		Json::Value dummy;
//		dummy["isSuccess"] = 1;
//		dummy["korean"] = !agreed;
//		func(dummy);
	}];
	// not implementation
	
#endif
	
}
string hspConnector::getUniqId()
{
	string r;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	r = "aes_key_";
	//NSLocale *currentLocale = [NSLocale currentLocale];  // get the current locale.
	//NSString *countryCode = [currentLocale objectForKey:NSLocaleCountryCode];
	//string r = [countryCode cStringUsingEncoding:NSUTF8StringEncoding];

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "getUniqId", "()Ljava/lang/String;")) {
		jstring result = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		
		jboolean isCopy = JNI_FALSE;
		const char* revStr = t.env->GetStringUTFChars(result, &isCopy);
		r = revStr;
		
		t.env->ReleaseStringUTFChars(result, revStr);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
	
	return r;
}
int hspConnector::getVersionCode()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
 jint ret = -1;
 JniMethodInfo t;
 if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "getVersionCode", "()I")) {
	 //		int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
	 ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
	 CCLog("ret = %d ", ret);
	 t.env->DeleteLocalRef(t.classID);
 }
 return (int)ret;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
 return 0;
 // not implementation
 //	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s",url.c_str()]]];
#endif
}
int hspConnector::getLoginType()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	jint ret = -1;
	JniMethodInfo t;
	CCLog("getLoginType");
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "getLoginType", "()I")) {
		//		int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
		ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
		CCLog("ret = %d ", ret);
		t.env->DeleteLocalRef(t.classID);
	}
	return (int)ret;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return [[HSPCore sharedHSPCore] loginType];
	// not implementation
//	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s",url.c_str()]]];
#endif
	
}


int hspConnector::getIsEnablePushNotification()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	jint ret = -1;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "getIsEnablePushNotification", "()I")) {
		//		int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
		ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
		CCLog("ret = %d ", ret);
		t.env->DeleteLocalRef(t.classID);
	}
	return (int)ret;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return [HSPMessage isEnablePushNotification];
	// not implementation
    //	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s",url.c_str()]]];
#endif
}
void hspConnector::setIsEnablePushNotification(int p)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//	jint ret = -1;
	JniMethodInfo t;
    
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "setIsEnablePushNotification", "(I)V")) {
		//		int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, p);
//		CCLog("ret = %d ", ret);
		t.env->DeleteLocalRef(t.classID);
	}
//	return (int)ret;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[HSPMessage setPushNotification:p completionHandler:^(HSPError *error) {
		
		
	}];
//	return 1;
	// not implementation
    //	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s",url.c_str()]]];
#endif
}

void hspConnector::openHSPNotice()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	// not implementation
//	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s",url.c_str()]]];
#endif
	
}
int hspConnector::openKakaoMsg()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	int r = 0;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "openKakaoMsg", "()I")) {
		//		int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
		r = t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}

	return r;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return [[UIApplication sharedApplication] openURL: [NSURL URLWithString:
						 @"kakaolink://sendurl?msg=돌아온 오락실의 제왕!!\n땅따먹기 리턴즈 with 섬란카구라 뉴웨이브&url=http://hgurl.me/am7&appid=com.nhnent.SKSUMRAN&appver=1.0&type=&appname=땅따먹기&apiver=&metainfo={os:\"ios\",executeurl:\"주소입니다.\"}"]];
	// not implementation
//	CCLog(url.c_str());
//	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s",url.c_str()]]];

#endif
}

int hspConnector::sendKakaoMsg(string title,string msg,string url){
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	int r = 0;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "sendKakaoMsg", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I")) {
		jstring param1 = t.env->NewStringUTF(title.c_str());
		jstring param2 = t.env->NewStringUTF(msg.c_str());
		jstring param3 = t.env->NewStringUTF(url.c_str());
		r = t.env->CallStaticIntMethod(t.classID, t.methodID, param1,param2,param3);
		t.env->DeleteLocalRef(param1);
		t.env->DeleteLocalRef(param2);
		t.env->DeleteLocalRef(param3);
		t.env->DeleteLocalRef(t.classID);
	}
	
	return r;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	if (![KakaoLinkCenter canOpenKakaoLink]) {
		return false;
	}
	
	NSMutableArray *metaInfoArray = [NSMutableArray array];
	NSDictionary *metaInfoIOS = [NSDictionary dictionaryWithObjectsAndKeys:
															 @"ios", @"os",
															 @"phone", @"devicetype",
															 @"http://itunes.apple.com/app/id362057947?mt=8", @"installurl",
															 @"example://example", @"executeurl",
															 nil];
	//	[metaInfoArray addObject:metaInfoAndroid];
	[metaInfoArray addObject:metaInfoIOS];
	
	string totalMsg = title + msg;
	return [KakaoLinkCenter openKakaoAppLinkWithMessage:[NSString stringWithUTF8String:totalMsg.c_str()]
																					 URL:@"http://link.kakao.com/?test-ios-app"
																	 appBundleID:[[NSBundle mainBundle] bundleIdentifier]
																		appVersion:[[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"]
																			 appName:[[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleName"]
																 metaInfoArray:metaInfoArray];
//	return [[UIApplication sharedApplication] openURL: [NSURL URLWithString:
//																							 @"kakaolink://sendurl?msg=&url=http://hgurl.me/am7&appid=com.nhnent.SKSUMRAN&appver=1.0&appname=땅따먹기}"]];
#endif
}

void hspConnector::getAdXConnectEventInstance(string event, string data, string currency)
{
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	int r = 0;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "getAdXConnectEventInstance",
																		 "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
		jstring param1 = t.env->NewStringUTF(event.c_str());
		jstring param2 = t.env->NewStringUTF(data.c_str());
		jstring param3 = t.env->NewStringUTF(currency.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, param1,param2,param3);
		t.env->DeleteLocalRef(param1);
		t.env->DeleteLocalRef(param2);
		t.env->DeleteLocalRef(param3);
		t.env->DeleteLocalRef(t.classID);
	}
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[tracker sendEvent:[NSString stringWithUTF8String:event.c_str()]
						withData:[NSString stringWithUTF8String:data.c_str()]
				 andCurrency:[NSString stringWithUTF8String:currency.c_str()]];
	
#endif
}
void hspConnector::IgawAdbrixBuy(string productName)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	int r = 0;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "IgawAdbrixBuy",
																		 "(Ljava/lang/String;)V")) {
		jstring param1 = t.env->NewStringUTF(productName.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, param1);
		t.env->DeleteLocalRef(param1);
		t.env->DeleteLocalRef(t.classID);
	}
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[AdBrix buy:[NSString stringWithUTF8String:productName.c_str()]];
//	[tracker sendEvent:[NSString stringWithUTF8String:event.c_str()]
//						withData:[NSString stringWithUTF8String:data.c_str()]
//				 andCurrency:[NSString stringWithUTF8String:currency.c_str()]];
	
#endif
}
 
 
void hspConnector::IgawAdbrixFirstTimeExperience(string msg)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	int r = 0;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "IgawAdbrixFirstTimeExperience",
																		 "(Ljava/lang/String;)V")) {
		jstring param1 = t.env->NewStringUTF(msg.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, param1);
		t.env->DeleteLocalRef(param1);
		t.env->DeleteLocalRef(t.classID);
	}
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	//	[tracker sendEvent:[NSString stringWithUTF8String:event.c_str()]
	//						withData:[NSString stringWithUTF8String:data.c_str()]
	//				 andCurrency:[NSString stringWithUTF8String:currency.c_str()]];
	[AdBrix firstTimeExperience:[NSString stringWithUTF8String:msg.c_str()]];
#endif
 
}
void hspConnector::IgawAdbrixRetention(string msg)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	int r = 0;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "IgawAdbrixRetention",
																		 "(Ljava/lang/String;)V")) {
		jstring param1 = t.env->NewStringUTF(msg.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, param1);
		t.env->DeleteLocalRef(param1);
		t.env->DeleteLocalRef(t.classID);
	}
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	//	[tracker sendEvent:[NSString stringWithUTF8String:event.c_str()]
	//						withData:[NSString stringWithUTF8String:data.c_str()]
	//				 andCurrency:[NSString stringWithUTF8String:currency.c_str()]];
	[AdBrix retention:[NSString stringWithUTF8String:msg.c_str()]];
#endif
	
}

void hspConnector::analyticsPurchase(string itemCode, float payment, float unitCost, string currency, int level)
{
	
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	toast::analytics::tracePurchase(itemCode, payment, unitCost, currency, level);
//	JniMethodInfo t;
//	int r = 0;
//	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "AnalyticsPurchase",
//																		 "(Ljava/lang/String;FFLjava/lang/String;I)V")) {
//		jstring param1 = t.env->NewStringUTF(itemCode.c_str());
//		jstring param2 = t.env->NewStringUTF(currency.c_str());
//		t.env->CallStaticVoidMethod(t.classID, t.methodID, param1, payment, unitCost, param2, level);
//		t.env->DeleteLocalRef(param1);
//		t.env->DeleteLocalRef(param2);
//		t.env->DeleteLocalRef(t.classID);
//	}
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[TAGAnalytics tracePurchase:[NSString stringWithUTF8String:itemCode.c_str()] payment:payment unitCost:unitCost
										 currency:[NSString stringWithUTF8String:currency.c_str()] level:level];
	
#endif
	
}
void hspConnector::analyticsTraceMoneyAcquisition(string itemCode, string type, int amount, int level)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	toast::analytics::traceMoneyAcquisition(itemCode, type, amount, level);
//	JniMethodInfo t;
//	int r = 0;
//	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "AnalyticsTraceMoneyAcquisition",
//																		 "(Ljava/lang/String;Ljava/lang/String;II)V")) {
//		jstring param1 = t.env->NewStringUTF(itemCode.c_str());
//		jstring param2 = t.env->NewStringUTF(type.c_str());
//		t.env->CallStaticVoidMethod(t.classID, t.methodID, param1, param2, amount, level);
//		t.env->DeleteLocalRef(param1);
//		t.env->DeleteLocalRef(param2);
//		t.env->DeleteLocalRef(t.classID);
//	}
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[TAGAnalytics traceMoneyAcquisition:[NSString stringWithUTF8String:itemCode.c_str()] type:[NSString stringWithUTF8String:type.c_str()]
										acquisitionAmount:amount level:level];
#endif


}
void hspConnector::analyticsTraceMoneyConsumption(string itemCode, string type, int amount, int level)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	toast::analytics::traceMoneyConsumption(itemCode, type, amount, level);
//	JniMethodInfo t;
//	int r = 0;
//	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "AnalyticsTraceMoneyConsumption",
//																		 "(Ljava/lang/String;Ljava/lang/String;II)V")) {
//		jstring param1 = t.env->NewStringUTF(itemCode.c_str());
//		jstring param2 = t.env->NewStringUTF(type.c_str());
//		t.env->CallStaticVoidMethod(t.classID, t.methodID, param1, param2, amount, level);
//		t.env->DeleteLocalRef(param1);
//		t.env->DeleteLocalRef(param2);
//		t.env->DeleteLocalRef(t.classID);
//	}
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[TAGAnalytics traceMoneyConsumption:[NSString stringWithUTF8String:itemCode.c_str()] type:[NSString stringWithUTF8String:type.c_str()]
										consumptionAmount:amount level:level];
#endif
}
void hspConnector::analyticsTraceLevelUp(int l)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	toast::analytics::traceLevelUp(l);
//	JniMethodInfo t;
//	int r = 0;
//	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "AnalyticsTraceLevelUp",
//																		 "(I)V")) {
//		t.env->CallStaticVoidMethod(t.classID, t.methodID, l);
//		t.env->DeleteLocalRef(t.classID);
//	}
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[TAGAnalytics traceLevelUp:l];
#endif
	
}
void hspConnector::analyticsTraceFriendCount(int f)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	toast::analytics::traceFriendCount(f);
//	JniMethodInfo t;
//	int r = 0;
//	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "AnalyticsTraceFriendCount",
//																		 "(I)V")) {
//		t.env->CallStaticVoidMethod(t.classID, t.methodID, f);
//		t.env->DeleteLocalRef(t.classID);
//	}
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[TAGAnalytics traceFriendCount:f];
#endif
	
}

void hspConnector::analyticsSetUserId(string userId, bool t)
{
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	toast::analytics::setUserId(userId);
//	JniMethodInfo t;
//	int r = 0;
//	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "AnalyticsTraceFriendCount",
//																		 "(I)V")) {
//		t.env->CallStaticVoidMethod(t.classID, t.methodID, f);
//		t.env->DeleteLocalRef(t.classID);
//	}
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[TAGAnalytics setUserId:[NSString stringWithUTF8String:userId.c_str()] useCampaignOrPromotion:t];
#endif
}


int hspConnector::registerGamePadCallback(jsonSelType func)
{
	int registeredKey = -1;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	int r = 0;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "registerGamePadCallback",
																		 "(I)V")) {
		int _key =  jsonDelegator::get()->add(func,0,0);
		registeredKey = _key;
		t.env->CallStaticVoidMethod(t.classID, t.methodID, _key);
		t.env->DeleteLocalRef(t.classID);
	} 
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
	return registeredKey;
}
void hspConnector::registerGamePadCallback(CCObject* target, jsonSelType func)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	GraphDog::get()->addTarget(target);
	function<void(Json::Value)> sFunc = [=](Json::Value value){
		CCLOG("checkDelegator sFunc call");
		if(GraphDog::get()->checkTarget(target))
			func(value);
	};
	registerGamePadCallback(sFunc);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
	

}
void hspConnector::unregisterGamePadCallback(int registeredKey)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	int r = 0;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "unregisterGamePadCallback",
																		 "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, registeredKey);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
}

void hspConnector::launchPromotion()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "launchPromotion", "()V")) {
		//		int _key =  jsonDelegator::get()->add(nextFunc, param, callbackParam);
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	// not implementation
	
	[HSPCGP launchPromotion];
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
//        CCLOG("step1 %s", wr.write(appFriends).c_str());
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
//    CCLOG("step0 %s", wr.write(obj).c_str());
//    CCLOG("step2 %s", wr.write(appFriends).c_str());
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
//    CCLOG("kLoadLocalUser function");
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


