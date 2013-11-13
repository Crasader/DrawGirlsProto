
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
#import "HSPKakao.h"
#import "Kakao.h"
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import "KALocalUser.h"
#import "KAAuth.h"
#import "KakaoSDKViewController.h"
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

string hspConnector::getKakaoID(){
	return this->myKakaoInfo["user_id"].asString();
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
		
		
		
		hspConnector::get()->kLoadLocalUser([this,delekey,obj](Json::Value r){
			hspConnector::get()->myKakaoInfo = r;
			graphdog->setKakaoMemberID(hspConnector::get()->getKakaoID());
			
			jsonDelegator::DeleSel delsel = jsonDelegator::get()->load(delekey);
			if(delsel.func)delsel.func(obj);
			jsonDelegator::get()->remove(delekey);
			
		});
		
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

void hspConnector::loadMyInfo(Json::Value obj){
	GraphDogLib::JsonToLog("myinfo", obj);
}
/*
 로그인
  param
    -
  reutrn
    longlongint memberNo : 한게임회원번호
    string nickname : 닉네임
    bool isValid
    bool exposeOnline
    int recentPlayedGameNo : 최근실행 게임번호
    string lastLoginDate : 마지막 로그인 date
    dict error : 에러정보
 */

//void hspConnector::loadMyProfile(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    [HSPMyProfile loadMyProfileWithCompletionHandler:^( HSPMyProfile* profile, HSPError *error) {
//
//     
//     NSMutableDictionary *resultDict;
//     
//     if ( [error isSuccess] == YES )
//     {
//         resultDict = (NSMutableDictionary *)profileToDict(profile);
//     }else{
//         resultDict = [NSMutableDictionary dictionary];
//     }
//        
//     addErrorInResult(resultDict, error);
//     callFuncMainQueue(param,callbackParam,target,selector,resultDict);
//
//     }];
//#endif
//    
//}

/*
 유저데이터저장
 param
    array gameUserData : 저장할 데이터
 reutrn
    dict error : 에러정보
*/
void hspConnector::reportGameUserData(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    Json::FastWriter writer;
    NSString *gData=[NSString stringWithUTF8String:writer.write(param.get("gameUserData", "")).c_str()];
    
   [HSPMyProfile loadMyProfileWithCompletionHandler:^(HSPMyProfile *profile, HSPError *error) {
        [profile reportGameUserData:gData completionHandler:^(HSPError *error2) {
            NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
            addErrorInResult(resultDict, error2);
            callFuncMainQueue(param,callbackParam,target,selector,resultDict);
         }];
   }];
#endif
}

/*
 디테일데이터 읽어오기
 param
    -
 reutrn
    array gameUserData : 유저데이터
    dict error : 에러정보
*/
void hspConnector::loadMyDetailedProfile(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    [HSPMyProfile loadMyProfileWithCompletionHandler:^( HSPMyProfile* profile, HSPError *error) {
        NSMutableDictionary *resultDict;
        if ( [error isSuccess] != YES ){
            resultDict = [NSMutableDictionary dictionary];
            addErrorInResult(resultDict, error);
            callFuncMainQueue(param,callbackParam,target,selector,resultDict);

        }else{
            resultDict = (NSMutableDictionary *)profileToDict(profile);
            [profile loadDetailedProfileWithCompletionHandler:^(HSPDetailedProfile *detailedProfile, HSPError *error2) {
                
                NSMutableDictionary *df;
                if([error2 isSuccess]==YES)
                    df=(NSMutableDictionary *)detailedProfileToDict(detailedProfile);
                else
                    df=[NSMutableDictionary dictionary];
                
                addErrorInResult(df, error2);
                [resultDict setObject:df forKey:@"detailed"];
                
                
                addErrorInResult(resultDict, error);
                callFuncMainQueue(param,callbackParam,target,selector,resultDict);
            }];
        }
        
    }];
#endif
}


void hspConnector::loadServiceProperties(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    HSPServiceProperties *hpts = [HSPCore sharedHSPCore].serviceProperties;
    if ( [hpts servicePropertiesDictionary] != nil ){
        NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
        [resultDict setObject:[NSNumber numberWithInt:[hpts launchingState]] forKey:@"launchingState"];
        callFuncMainQueue(param,callbackParam,target,selector,resultDict);
    }else{
        [hpts loadServicePropertiesWithCompletionHandler:^(HSPError *error) {
            NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
            [resultDict setObject:[NSNumber numberWithInt:[hpts launchingState]] forKey:@"launchingState"];
            
            addErrorInResult(resultDict, error);
            callFuncMainQueue(param,callbackParam,target,selector,resultDict);
        }];
    }
#endif
}






void hspConnector::loadProfiles(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

    Json::FastWriter wr;
    CCLog("loadprofile param string %s",wr.write(param).c_str());
    NSString *jsonString = [NSString stringWithUTF8String:wr.write(param).c_str()];
    NSData *jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *parameters = [NSJSONSerialization JSONObjectWithData:jsonData options:kNilOptions error:nil];
    
    NSLog(@"loadprofile param %@",parameters);
    [HSPProfile loadProfilesWithMemberNos:[parameters objectForKey:@"memberNos"]
                        completionHandler:^(NSArray *profiles, HSPError *error) {
                            
                            NSMutableDictionary* resultDict = [NSMutableDictionary dictionary];
                            
                            if([error isSuccess]==YES){
                                NSMutableArray *resultArray=[NSMutableArray array];
                                for(HSPProfile* profile in profiles){
                                    [resultArray addObject:(NSMutableDictionary *)profileToDict(profile)];
                                }
                                [resultDict setObject:resultArray forKey:@"profiles"];
                            }
                            
                            addErrorInResult(resultDict, error);
                            callFuncMainQueue(param,callbackParam,target,selector,resultDict);
                            
    }];
#endif
}

void hspConnector::loadDetailedProfiles(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

    Json::FastWriter wr;
    CCLog("loadprofile param string %s",wr.write(param).c_str());
    NSString *jsonString = [NSString stringWithUTF8String:wr.write(param).c_str()];
    NSData *jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *parameters = [NSJSONSerialization JSONObjectWithData:jsonData options:kNilOptions error:nil];
    
    NSLog(@"loadprofile param %@",parameters);
    [HSPProfile loadProfilesWithMemberNos:[parameters objectForKey:@"memberNos"]
                        completionHandler:^(NSArray *profiles, HSPError *error) {
                            
                            NSMutableDictionary* resultDict = [NSMutableDictionary dictionary];
                            
                            
                            if([error isSuccess]==YES){
                                NSMutableArray *resultArray=[NSMutableArray array];
                                for(int i=0;i<[profiles count];i++){
                                    HSPProfile* profile = [profiles objectAtIndex:i];
                                    NSMutableDictionary *profileDict = (NSMutableDictionary *)profileToDict(profile);
                                    
                                    [profile loadDetailedProfileWithCompletionHandler:^(HSPDetailedProfile *detailedProfile, HSPError *error2) {
                                        
                                        //디테일뽑아오기
                                        NSMutableDictionary *df;
                                        if([error2 isSuccess]==YES)
                                            df=(NSMutableDictionary *)detailedProfileToDict(detailedProfile);
                                        else
                                            df=[NSMutableDictionary dictionary];
                                        
                                        addErrorInResult(df, error2);
                                        [profileDict setObject:df forKey:@"detailed"];
                                        
                                        [resultArray addObject:profileDict];
                                        
                                        [resultDict setObject:resultArray forKey:@"profiles"];
                                        
                                        addErrorInResult(resultDict, error);
                                        
                                        //마지막 이라면 callFunc!!
                                        if(i==[profiles count]-1)
                                            callFuncMainQueue(param,callbackParam,target,selector,resultDict);
                                        
                                    }];
                                    
                                }
                            }else{
                                addErrorInResult(resultDict, error);
                                callFuncMainQueue(param,callbackParam,target,selector,resultDict);
                            }
                            
                            //addErrorInResult(resultDict, error);
                            //callFuncMainQueue(param,callbackParam,target,selector,resultDict);
                            
                        }];
#endif
}

void hspConnector::saveMemberData(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

    NSMutableDictionary *pDict = (NSMutableDictionary *)jsonToDict(param);
    
    [HSPMemberDataStorage saveMemberDataWithDataDictionary:[pDict objectForKey:@"data"] completionHandler:^(HSPError *error) {
        NSMutableDictionary* resultDict = [NSMutableDictionary dictionary];
        addErrorInResult(resultDict, error);
        callFuncMainQueue(param,callbackParam,target,selector,resultDict);
    }];
#endif
    
}

void hspConnector::loadMemberData(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

    NSMutableDictionary *pDict = (NSMutableDictionary *)jsonToDict(param);
    
    [HSPMemberDataStorage loadMemberDataWithKeyArray:[pDict objectForKey:@"keys"] completionHandler:^(NSDictionary *dataDic, HSPError *error) {
        NSMutableDictionary* resultDict = [NSMutableDictionary dictionary];
        if ([error isSuccess])
            [resultDict setObject:dataDic forKey:@"data"];
        addErrorInResult(resultDict, error);
        callFuncMainQueue(param,callbackParam,target,selector,resultDict);
    }];
#endif
}

void hspConnector::removeMemberData(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

    NSMutableDictionary *pDict = (NSMutableDictionary *)jsonToDict(param);
    
    [HSPMemberDataStorage removeMemberDataWithKeyArray:[pDict objectForKey:@"keys"] completionHandler:^(HSPError *error) {
        NSMutableDictionary* resultDict = [NSMutableDictionary dictionary];
        addErrorInResult(resultDict, error);
        callFuncMainQueue(param,callbackParam,target,selector,resultDict);
    }];
#endif
}



void hspConnector::loadRankings(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

    NSMutableDictionary *pDict = (NSMutableDictionary *)jsonToDict(param);

    
    [HSPRanking loadRankingWithGameNo:[[pDict objectForKey:@"gameNo"] intValue] factor:[[pDict objectForKey:@"factor"] intValue] completionHandler:^(HSPRanking *ranking, HSPError *error) {
        
        
        if([error isSuccess]==YES){
        [ranking queryScoresByScope:(HSPRankingScope)[[pDict objectForKey:@"scope"] intValue]
                             period:(HSPRankingPeriod)[[pDict objectForKey:@"period"] intValue]
                              index:[[pDict objectForKey:@"index"] intValue]
                              count:[[pDict objectForKey:@"count"] intValue]
                  completionHandler:^(HSPScore *myScore, NSArray *scores, HSPError *error2) {
                      
                      NSMutableDictionary* resultDict = [NSMutableDictionary dictionary];
                      NSMutableArray* rankList = [NSMutableArray array];
                      if([error2 isSuccess]==YES){
                          for(HSPScore *score in scores){
                              //score, memberNo, grade, changeGrade, extraData
                              NSMutableDictionary* mRank = [NSMutableDictionary dictionary];
                              [mRank setObject:[NSNumber numberWithDouble:score.score] forKey:@"score"];
                              [mRank setObject:[NSNumber numberWithLongLong:score.memberNo] forKey:@"memberNo"];
                              [mRank setObject:[NSNumber numberWithInt:score.grade] forKey:@"grade"];
                              [mRank setObject:[NSNumber numberWithInt:score.changeGrade] forKey:@"changeGrade"];
                              if([score.extraData length]>0)[mRank setObject:score.extraData forKey:@"extraData"];
                              [rankList addObject:mRank];
                          }
                          
                          //내랭킹
                          NSMutableDictionary* myRank = [NSMutableDictionary dictionary];
                          [myRank setObject:[NSNumber numberWithDouble:myScore.score] forKey:@"score"];
                          [myRank setObject:[NSNumber numberWithLongLong:myScore.memberNo] forKey:@"memberNo"];
                          [myRank setObject:[NSNumber numberWithInt:myScore.grade] forKey:@"grade"];
                          [myRank setObject:[NSNumber numberWithInt:myScore.changeGrade] forKey:@"changeGrade"];
                          if([myScore.extraData length]>0)[myRank setObject:myScore.extraData forKey:@"extraData"];
                          
                          [resultDict setObject:rankList forKey:@"myRanking"];

                          
                          [resultDict setObject:rankList forKey:@"ranking"];
                      }
                      
                      addErrorInResult(resultDict, error2);
                      callFuncMainQueue(param,callbackParam,target,selector,resultDict);
                      
                  }];
        }else{
            //error처리
             NSMutableDictionary* resultDict = [NSMutableDictionary dictionary];
            addErrorInResult(resultDict, error);
            callFuncMainQueue(param,callbackParam,target,selector,resultDict);
        }
        
    }];
#endif
}

void hspConnector::reportRanking(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

    NSMutableDictionary *pDict = (NSMutableDictionary *)jsonToDict(param);
    if(![pDict objectForKey:@"extraData"])[pDict setObject:@"" forKey:@"extraData"];
    
    [HSPRanking reportRankingScore:[[pDict objectForKey:@"score"] doubleValue]
                            factor:[[pDict objectForKey:@"factor"] intValue]
                         extraData:[[pDict objectForKey:@"extraData"] stringValue]
                 completionHandler:^(HSPError *error) {
                     NSMutableDictionary* resultDict = [NSMutableDictionary dictionary];
                     addErrorInResult(resultDict, error);
                     callFuncMainQueue(param,callbackParam,target,selector,resultDict);
    }];
#endif
}




void hspConnector::queryHSPMemberNos(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

    NSMutableDictionary *pDict = (NSMutableDictionary *)jsonToDict(param);
    [HSPKakao queryHSPMemberNosWithKakaoIds:[pDict objectForKey:@"kakaoIds"] completionHandler:^(NSDictionary *memberNoMap, HSPError *error) {
        
        NSMutableDictionary* resultDict = [NSMutableDictionary dictionary];
        [resultDict setObject:memberNoMap forKey:@"list"];
        addErrorInResult(resultDict, error);
        callFuncMainQueue(param,callbackParam,target,selector,resultDict);
        
    }];
#endif
}

void hspConnector::queryKakaoIds(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

    
    NSMutableDictionary *pDict = (NSMutableDictionary *)jsonToDict(param);
    [HSPKakao queryKakaoIdsWithMemberNos:[pDict objectForKey:@"hspNos"] completionHandler:^(NSDictionary *kakaoIdDictionary, HSPError *error) {
        
        
        NSArray* keylist = [kakaoIdDictionary allKeys];
        NSMutableDictionary *kDict = [NSMutableDictionary dictionary];
        for(int i=0;i<[keylist count];i++){
            [kDict setObject:[keylist objectAtIndex:i] forKey:[kakaoIdDictionary objectForKey:[keylist objectAtIndex:i]]];
        }
        
        NSMutableDictionary* resultDict = [NSMutableDictionary dictionary];
        [resultDict setObject:kDict forKey:@"list"];
        
        
        addErrorInResult(resultDict, error);
        callFuncMainQueue(param,callbackParam,target,selector,resultDict);
        
    }];
#endif
}


void hspConnector::kLoadFriends(CCObject* target,hspSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

    // kakao친구 로드! -> 앱사용자는 querykakaoHSP이용하여 hsp detailedProfile 로드! -> 로드한 정보 connector.friends에 저장.
    //               -> 미사용자는 friends에 저장
    // kakao전체친구 로드!
    int _key =  hspDelegator::get()->add(target,selector,0,0);
    
    [[KALocalUser localUser] loadFriendsWithCompletionHandler:^(NSDictionary *response, NSError *error) {
        
        //response ->  app_friends_info{}, friends_info[{friend_nickname,nickname,hashed_talk_user_id,user_id,supported_device,profile_image_url}] , friends_count(int)
       
        //에러처리하기
        
        NSLog(@"appfriends %@",[response objectForKey:@"app_friends_info"]);
        //addErrorInResult(response, error);
        Json::Value param;
        Json::Value friendInfo = dictToJson(response);
        
        //appfriends틀 만들기
        
        hspConnector::get()->appFriends.empty();
        
        NSArray *afrd = [response objectForKey:@"app_friends_info"];
        
        for (unsigned int i=0; i<[afrd count]; i++) {
            NSDictionary *fInfo = [afrd objectAtIndex:i];
            string _key = [[fInfo objectForKey:@"user_id"] UTF8String];
            appFriends[_key]["nickname"]=[[fInfo objectForKey:@"nickname"] UTF8String];
            appFriends[_key]["hashed_talk_user_id"]=[[fInfo objectForKey:@"hashed_talk_user_id"] UTF8String];
            appFriends[_key]["message_blocked"]=[[fInfo objectForKey:@"message_blocked"] boolValue];
            appFriends[_key]["profile_image_url"]=[[fInfo objectForKey:@"profile_image_url"] UTF8String];
            appFriends[_key]["supported_device"]=[[fInfo objectForKey:@"supported_device"] boolValue];
            appFriends[_key]["kId"]=[[fInfo objectForKey:@"user_id"] UTF8String];
            
            param["kakaoIds"].append([[fInfo objectForKey:@"user_id"] UTF8String]);
        }
        
        Json::StyledWriter wr;
        CCLog("step1 %s", wr.write(appFriends).c_str());
        param["delekey"]=_key;
        this->queryHSPMemberNos(param, 0, this, hsp_selector(hspConnector::kLoadFriendsStep2));
        
        //hspConnector::get()->kLoadFriendsStep2(param,_key);
        
    }];
#endif

};

//void hspConnector::kLoadFriendsStep2(Json::Value kakaoIds,int delekey){
//    //친구없을때 처리하기
//    //에러처리하기
//    //멤버정보를 이용 step3로 이동해서 
//    
//    
//    Json::Value param;
//    param["kakaoIds"]=kakaoIds;
//    param["delekey"]=delekey;
//    
//    
//    this->queryHSPMemberNos(param, 0, this, hsp_selector(hspConnector::kLoadFriendsStep3));
//    
//    
//    
//    
////    
////    if(param["kakaoIds"].size()>0){
////        NSMutableDictionary *pDict = (NSMutableDictionary *)jsonToDict(param);
////        [HSPKakao queryHSPMemberNosWithKakaoIds:[pDict objectForKey:@"kakaoIds"] completionHandler:^(NSDictionary *memberNoMap, HSPError *error) {
////            
////            
////            NSArray* keylist = [memberNoMap allKeys];
////            for(int i=0;i<[keylist count];i++){
////                // [keylist objectAtIndex:i]  ---> kIds
////                // [memberNoMap objectForKey:[keylist objectAtIndex:i]]  ---> hNo
////                
////                appFriends[[[keylist objectAtIndex:i] UTF8String]]["hNo"]=[[memberNoMap objectForKey:[keylist objectAtIndex:i]] longLongValue];
////                //[kDict setObject:[keylist objectAtIndex:i] forKey:[memberNoMap objectForKey:[keylist objectAtIndex:i]]];
////            }
////            
////            
////            
////            
////            
//    
//            
//            
//            
//            
////            NSMutableDictionary* resultDict = [NSMutableDictionary dictionary];
////            [resultDict setObject:memberNoMap forKey:@"list"];
////            addErrorInResult(resultDict, error);
////            
////            Json::StyledWriter wr;
////            CCLog("step2 %s", wr.write(appFriends).c_str());
////            
////            
////            
////            hspConnector::get()->kLoadFriendsStep3(param, friendInfo, target, selector);
////            
////            //callFuncMainQueue(param,friendInfo,target,selector,resultDict);
//            
////        }];
////    }
//}


void hspConnector::kLoadFriendsStep2(Json::Value obj){
    

    Json::Value param;
    Json::Value::Members m=obj["list"].getMemberNames();
    
    for(auto iter = m.begin();iter!=m.end();++iter){
        appFriends[*iter]["hNo"]=obj["list"][*iter].asInt64();
        param["memberNos"].append(obj["list"][*iter].asInt64());
    }
    
    Json::StyledWriter wr;
    CCLog("step0 %s", wr.write(obj).c_str());
    CCLog("step2 %s", wr.write(appFriends).c_str());
    
    param["delekey"]=obj["param"]["delekey"].asInt();
    
    this->loadDetailedProfiles(param, 0, this, hsp_selector(hspConnector::kLoadFriendsStep3));
    
//    //appfriends에서 hno로 detailed 뽑아오기
//    //appfriends에서 hno모아서 친구등록하기
//    
//    Json::Value hnolist;
//    Json::Value::Members m = appFriends.getMemberNames();
//    for (auto iter = m.begin(); iter!=m.end(); ++iter) {
//        string key = (string)*iter;
//        hnolist.append(appFriends[key]["hNo"].asInt64());
//    }
//    
//    Json::StyledWriter wr;
//    CCLog("step3 %s", wr.write(hnolist).c_str());
//    
    
    
    
}
void hspConnector::kLoadFriendsStep3(Json::Value obj){
    
    Json::Value::Members m = appFriends.getMemberNames();
    
    for(auto iter=m.begin();iter!=m.end();++iter){
        for(int i=0;i<obj["profiles"].size();i++){
            if(obj["profiles"][i]["memberNo"].asInt64()==appFriends[*iter]["hNo"].asInt64()){
                appFriends[*iter]["detailed"]=obj["profiles"][i]["detailed"];
                appFriends[*iter]["exposeOnline"]=appFriends[*iter]["hNo"].asBool();
                appFriends[*iter]["isValid"]=appFriends[*iter]["isValid"].asBool();
                appFriends[*iter]["lastLoginDate"]=appFriends[*iter]["lastLoginDate"].asString();
                appFriends[*iter]["recentPlayedGameNo"]=appFriends[*iter]["recentPlayedGameNo"].asInt();
                break;
            }
        }
    }
    
    int delkey = obj["param"]["delekey"].asInt();
    hspDelegator::DeleSel delesel = hspDelegator::get()->load(delkey);
    ((delesel.target)->*(delesel.selector))(0);
    hspDelegator::get()->remove(delkey);
}

void hspConnector::kSendMessage(){
    // kakaoID로 connector.friends 에서 프로필빼옴 -> 카카오메세지보내기 -> 성공시 앱사용자일경우 hsp쪽지or메일 보내기 -> 완료
    //                                                         -> 성공시 앱미사용자일경우  그냥 완료
    //                                                         -> 실패시실패처리
    //
}


void hspConnector::kLoadRanking(){
    // hspranking 으로 랭킹불러오기 -> hspMemberNo로 connector.friends에서 detaileProfie 뽑아보내주기 ->완료
}


void queryFollowingMembers(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
    
}
void queryFollowers(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
    
}
void followMembers(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
    
}
void blockMembers(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
    
}
void queryMembersRecommended(Json::Value param,Json::Value callbackParam,CCObject* target,hspSelType selector){
    
}







void hspConnector::loadMyProfile(Json::Value param,Json::Value callbackParam,jsonSelType func){
    //1. hspprofile 로드
    //2. kakao loadlocaluser
    
    Json::Value p;
    p["api"]="GetProfileDetail2";
    p["memberNo"]=this->getHSPMemberNo();
    p["gameNo"]=this->hspNo;
    p["requesterMemberNo"]=this->getHSPMemberNo();
    
    CCLog("loadMyProfile start");
    hspConnector::get()->command("httpgateway",p,[=](Json::Value hProfile){
        CCLog("loadMyProfile step1");
        jsonLog("loadMyProfile", hProfile);
        
        
        hspConnector::get()->kLoadLocalUser([=](Json::Value kProfile){
            CCLog("loadMyProfile step2");
            
            kProfile["hsp"]=hProfile;
            func(kProfile);
        });
    });

}

void hspConnector::loadFriendsProfile(Json::Value param,Json::Value callbackParam,jsonSelType func){
    
    
    // 10분 체크해서 10분 안넘었으면 있는값 리턴시켜버려?
    // 에러처리들..
    
    //카카오 친구들 불러온다.
    hspConnector::get()->kLoadFriends([=](Json::Value kFriends){
        Json::Value p;
        
        p["api"]="GetMemberNoListByOauthIdList";
        p["serviceDomain"]="KAKAOGAME";
        p["oauthProvider"]="kakao";
        p["gameNo"]=this->hspNo;
        
        int i=0;
        Json::Value appFriends;
        for(auto frd : kFriends["app_friends_info"]){
            appFriends[frd["user_id"].asString()] = frd;
            p["oauthIdList"][i++] = frd["user_id"];
        }
        jsonLog("param", p );
        
        //hsp 번호 불러온다.
        hspConnector::get()->command("httpgateway", p, [=](Json::Value hspNoList){
            Json::Value appFriends2=appFriends;
            Json::Value p2;
            p2["api"]="GetProfileList";
            p2["requesterMemberNo"]=hspConnector::get()->getHSPMemberNo();

            Json::Value::Members m=hspNoList["memberNoMap"].getMemberNames();
            
            for(auto iter = m.begin();iter!=m.end();++iter){
                appFriends2[*iter]["hNo"]=hspNoList["memberNoMap"][*iter].asInt64();
                p2["memberNoList"].append(hspNoList["memberNoMap"][*iter].asInt64());
            }
            
             //hsp번호목록으로 hsp정보 가져와서 카카오친구에 붙인다.
             hspConnector::get()->command("httpgateway", p2, [=](Json::Value hMembers){
                 Json::Value appFriends3 = appFriends2;
                  jsonLog("complete", hMembers);
                 Json::Value::Members m=hMembers["profileMap"].getMemberNames();
                 Json::Value::Members m2=appFriends3.getMemberNames();
                 
                 for(auto iter = m.begin();iter!=m.end();++iter){
                     for(auto iter2 = m2.begin();iter2!=m2.end();++iter2){
                         if(appFriends3[*iter2]["hNo"].asUInt64()==hMembers["profileMap"][*iter]["memberNo"].asUInt64()){
                            appFriends3[*iter2]["hsp"]=hMembers["profileMap"][*iter];
                             break;
                         }
                         
                     }
                     
                 }
                 
                 jsonLog("complete", appFriends3);
                 func(appFriends3);
                 
                 
                 
                 //그냥확인용친구불러오기
                 Json::Value p4;
                 p4["api"]="GetFolloweesInOption2";
                 p4["memberNo"]=this->getHSPMemberNo();
                 p4["filterOption"]="all";
                 p4["sortOption"]=0;
                 p4["offset"]=1;
                 p4["count"]=500;
                 hspConnector::get()->command("httpgateway", p4, [](Json::Value obj){
                     jsonLog("result yeh~", obj);
                 });
             });
        });
    });
}























void hspConnector::kLoadLocalUser(Json::Value callbackParam, jsonSelType func){
    CCLog("kLoadLocalUser function");
    Json::Value param;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    [[KALocalUser localUser] loadLocalUserWithCompletionHandler:^(NSDictionary *response, NSError *error) {
     NSDictionary *resultDict = response;
     if(error){
     resultDict = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:error.code],@"status",[error.userInfo objectForKey:@"NSLocalizedDescription"],@"message",nil];
     }
     
     callFuncMainQueue2(param,callbackParam,func,resultDict);
     
     }];
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "kLoadLocalUser", "(I)V")) {
        int _key =  jsonDelegator::get()->add(func,param,callbackParam);
    	t.env->CallStaticObjectMethod(t.classID, t.methodID,_key);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}



void hspConnector::kLoadLocalUser(jsonSelType func){
    Json::Value callbackParam;
    this->kLoadLocalUser(callbackParam,func);
}



void hspConnector::kLoadFriends(Json::Value callbackParam,jsonSelType func){
    Json::Value param;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    [[KALocalUser localUser] loadFriendsWithCompletionHandler:^(NSDictionary *response, NSError *error) {
     
     NSDictionary *resultDict = response;
     if(error){
     resultDict = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:error.code],@"status",[error.userInfo objectForKey:@"NSLocalizedDescription"],@"message",nil];
     }
     callFuncMainQueue2(param,callbackParam,func,resultDict);
     }];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "kLoadFriends", "(I)V")) {
        int _key =  jsonDelegator::get()->add(func,param,callbackParam);
    	t.env->CallStaticObjectMethod(t.classID, t.methodID,_key);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void hspConnector::kLoadFriends(jsonSelType func){
    Json::Value callbackParam;
    this->kLoadFriends(callbackParam,func);
}





void hspConnector::kSendMessage(Json::Value param,Json::Value callbackParam,jsonSelType func){
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
    NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
    NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
    
    KALinkMessageRequest *request = nil;
    
    request = [KALinkMessageRequest requestWithReceiverID:receiverID
                                                  message:message
                                         executeURLString:executeURLString];
    
    [[KALocalUser localUser] sendLinkMessageWithRequest:request completionHandler:^(BOOL success, NSError *error) {
     
     
     NSDictionary *resultDict;
     if(error){
     resultDict = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:error.code],@"status",[error.userInfo objectForKey:@"NSLocalizedDescription"],@"message",nil];
     }else{
     resultDict = [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:0] forKey:@"status"];
     }
     callFuncMainQueue2(param,callbackParam,func,resultDict);
     }];
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "kSendMessage", "(ILjava/lang/String;)V")) {
        int _key =  jsonDelegator::get()->add(func,param,callbackParam);
        ostringstream oss;
        oss << param;
    	t.env->CallStaticObjectMethod(t.classID, t.methodID,_key, t.env->NewStringUTF(oss.str().c_str()));
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void hspConnector::kSendMessage(Json::Value param,jsonSelType func){
    Json::Value callbackParam;
    this->kSendMessage(param,callbackParam,func);
}

void hspConnector::kLogout(Json::Value callbackParam,jsonSelType func){
    Json::Value param;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    [[KALocalUser localUser] logoutWithCompletionHandler:^(BOOL success, NSError *error) {
     
     NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
     if (success==true) {
     [resultDict setObject:[NSNumber numberWithInt:0] forKey:@"status"];
     } else {
     [resultDict setObject:[NSNumber numberWithInt:-500] forKey:@"status"];
     [resultDict setObject:[error.userInfo objectForKey:@"NSLocalizedDescription"] forKey:@"message"];
     }
     callFuncMainQueue2(param,callbackParam,func,resultDict);
     }];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "kLogout", "(I)V")) {
        int _key =  jsonDelegator::get()->add(func,param,callbackParam);
        t.env->CallStaticObjectMethod(t.classID, t.methodID,_key);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
    
}

void hspConnector::kLogout(jsonSelType func){
    Json::Value callbackParam;
    this->kLogout(callbackParam,func);
}


void hspConnector::kUnregister(Json::Value callbackParam,jsonSelType func){
    Json::Value param;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    [[KALocalUser localUser] unregisterWithCompletionHandler:^(BOOL success, NSError *error) {
     NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
     if (success==true) {
     [resultDict setObject:[NSNumber numberWithInt:0] forKey:@"status"];
     } else {
     [resultDict setObject:[NSNumber numberWithInt:-500] forKey:@"status"];
     [resultDict setObject:[error.userInfo objectForKey:@"NSLocalizedDescription"] forKey:@"message"];
     }
     callFuncMainQueue2(param,callbackParam,func,resultDict);
     
     }];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/litqoo/lib/hspConnector", "kUnregister", "(I)V")) {
        int _key =  jsonDelegator::get()->add(func,param,callbackParam);
        t.env->CallStaticObjectMethod(t.classID, t.methodID,_key);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}


void hspConnector::kUnregister(jsonSelType func){
    Json::Value param;
    this->kUnregister(param,func);
}


