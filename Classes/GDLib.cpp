#include "GDLib.h"
#include <sstream>
#include "BaseXX.h"
#include "CipherUtils.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "jni.h"
#if COCOS2D_VERSION<0x00020000
#include "android/jni/JniHelper.h"
#include "android/jni/SystemInfoJni.h"
#else
#include "platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include "platform/android/jni/JniHelper.h"
#endif
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import <UIKit/UIKit.h>
#import <StoreKit/SKStoreProductViewController.h>
#import "AppController.h"

//@interface AppController(GraphDog) <SKStoreProductViewControllerDelegate>
//-(void)productViewControllerDidFinish:(SKStoreProductViewController *)viewController;
//@end
//
//@implementation AppController(GraphDog)
//-(void)productViewControllerDidFinish:(SKStoreProductViewController *)viewController
//{
//    [viewController dismissViewControllerAnimated:YES completion:nil];
//}
//@end

#endif



namespace GraphDogLib {

	
	
	std::string IntToString (long long int number)
	{
		std::ostringstream convStream;
		
		convStream << number;
		
		return convStream.str();
	}
	
	std::string trim(std::string str){
		if(str == "")return str;
		std::remove(str.begin(), str.end(), ' ');
		return str;
	}
	
	void ReplaceString(std::string & strCallId, const char * pszBefore, const char * pszAfter )
	{
		size_t iPos = strCallId.find( pszBefore );
		size_t iBeforeLen = strlen( pszBefore );
		while( iPos < std::string::npos )
		{
			strCallId.replace( iPos, iBeforeLen, pszAfter );
			iPos = strCallId.find( pszBefore, iPos );
		}
	}
	
	string dateFormat(string format, string datestring){
		if(datestring.length()<14)datestring = "19870620120000";
		string y = datestring.substr(0,4);
		string sy = datestring.substr(2,2);
		string m = datestring.substr(4,2);
		string d = datestring.substr(6,2);
		string h = datestring.substr(8,2);
		string sh = IntToString(atoi(h.c_str())%12);
		string i = datestring.substr(10,2);
		string s = datestring.substr(12,2);
		
		ReplaceString(format,"y",sy.c_str());
		ReplaceString(format,"Y",y.c_str());
		ReplaceString(format,"m",m.c_str());
		ReplaceString(format,"d",d.c_str());
		ReplaceString(format,"h",sh.c_str());
		ReplaceString(format,"H",h.c_str());
		ReplaceString(format,"i",i.c_str());
		ReplaceString(format,"s",s.c_str());
		
		return format;
	}
	
	string dateFormat(string format, long long int datestring){
		return dateFormat(format,IntToString(datestring));
	}

	
	int match(const char *string, const char *pattern)
	{
		int    status;
		regex_t    re;
		
		
		if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
			return(0);      /* Report error. */
		}
		status = regexec(&re, string, (size_t) 0, NULL, 0);
		regfree(&re);
		if (status != 0) {
			return(0);      /* Report error. */
		}
		return(1);
	}


std::string GDCreateToken(string auID,string udid,string flag,string lang,string nick,string email,string platform,string cTime,string secretKey,string dInfo){
    string msg = auID;
    msg.append("||");
    msg.append(udid);
    msg.append("||");
    msg.append(flag);
    msg.append("||");
    msg.append(lang);
    msg.append("||");
    msg.append(nick);
    msg.append("||");
    msg.append(email);
    msg.append("||");
    msg.append(platform);
    msg.append("||");
    msg.append(cTime);
    msg.append("||");
    msg.append(dInfo);
    msg.append("||");
	return CipherUtils::encryptDESBASE64(secretKey.c_str(), msg.c_str());
}
    
void replaceString( std::string & strCallId, const char * pszBefore, const char * pszAfter )
{
    size_t iPos = strCallId.find( pszBefore );
    size_t iBeforeLen = strlen( pszBefore );
    while( iPos < std::string::npos )
    {
            strCallId.replace( iPos, iBeforeLen, pszAfter );
            iPos = strCallId.find( pszBefore, iPos );
    }
}
	
std::string random_string( size_t length )
{
	auto randchar = []() -> char
	{
		const char charset[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[ rand() % max_index ];
	};
	std::string str(length,0);
	std::generate_n( str.begin(), length, randchar );
	return str;
}


//@ string JsonObjectToString(JsonBox::Object _obj){
//    ostringstream oss;
//    oss << _obj;
//    return oss.str();
//}
    
//@ JsonBox::Object StringToJsonObject(string _str){
//    //명령문자열 json::value 로 변환
//    JsonBox::Value result;
//    result.loadFromString(_str.c_str());
//    
//    //명령문자열 json::object 로 변환
//    JsonBox::Object resultobj = result.getObject();
//    return resultobj;
//}
    
string JsonObjectToString(Json::Value _obj){
    Json::StyledWriter writer;
    return writer.write(_obj);
}

	
void JsonToLog(string tag,Json::Value _obj){
	Json::StyledWriter writer;
	CCLOG("%s - %s",tag.c_str(),writer.write(_obj).c_str());
}
    
Json::Value StringToJsonObject(string _str){
	Json::Value root;
	Json::Reader reader;
	if(reader.parse(_str, root)){
    return root;
	}else{
		root = "";
		return root;
	}
}

	
string getLocalCode()
{
	CCLOG("call getLocalCode");
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	{
		const char* tempCode;
		NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
		NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
		NSString *currentLanguage = [languages objectAtIndex:0];
		
		// get the current language code.(such as English is "en", Chinese is "zh" and so on)
		NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:currentLanguage];
		NSString * languageCode = [temp objectForKey:NSLocaleLanguageCode];
		
		tempCode = [languageCode cStringUsingEncoding:NSASCIIStringEncoding];
		return tempCode;
	}
	
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	{
//		string tempCode = getCurrentLanguageJNI();
//		return tempCode;
		return "";
	}
#endif
	
}
    
    void openReview(string appid)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		NSString *urlstring = [NSString stringWithFormat:@"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=%s&onlyLatestVersion=true&pageNumber=0&sortOrdering=1&type=Purple+Software",appid.c_str()];
		NSURL *url = [NSURL URLWithString:urlstring];
		[[UIApplication sharedApplication] openURL:url];
#endif
	}
	void openUpdate(string appid)
	{
		#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		NSString *urlstring = [NSString stringWithFormat:@"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewSoftware?id=%s",appid.c_str()];
		NSURL *url = [NSURL URLWithString:urlstring];
		[[UIApplication sharedApplication] openURL:url];
		#endif
	}

    void openAppStore(string appid){
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//        if(NSClassFromString(@"SKStoreProductViewController")) { // iOS6 이상인지 체크
//            AppController *appDelegate = (AppController *)[[UIApplication sharedApplication]delegate];
//			
//            SKStoreProductViewController *storeViewController = [[SKStoreProductViewController alloc] init];
//            storeViewController.delegate = appDelegate;
//			
//            NSDictionary *parameters = @{SKStoreProductParameterITunesItemIdentifier:[NSNumber numberWithInt:atoi(appid.c_str())]};
//            
//            [storeViewController loadProductWithParameters:parameters completionBlock:^(BOOL result, NSError *error) {
//                if (result){
//					[appDelegate.viewController presentViewController:storeViewController animated:YES completion:nil];
//                }
//			}];
//        }else{
//            NSString *urlstring = [NSString stringWithFormat:@"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewSoftware?id=%s",appid.c_str()];
//            NSURL *url = [NSURL URLWithString:urlstring];
//            [[UIApplication sharedApplication] openURL:url];
//        }
//#endif
    }

    
}
