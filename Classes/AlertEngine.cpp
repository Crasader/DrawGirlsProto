


#include "AlertEngine.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include <Foundation/Foundation.h>
#include "AppController.h"
#include "RootViewController.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "JNIKelper.h"
#endif

void AlertEngine::addDoubleAlert(const char *t_title, const char *t_message, const char *t_leftButtonText, const char *t_rightButtonText, int t_tag, std::function<void(int,int)> fc)
{
	func = fc;
	addDoubleAlert(t_title, t_message, t_leftButtonText, t_rightButtonText, t_tag, nullptr, nullptr);
	
}
void AlertEngine::addDoubleAlert(const char *t_title, const char *t_message, const char *t_leftButtonText, const char *t_rightButtonText, int t_tag, CCObject* t_target, SEL_AlertFuncII t_selector)
{
//	if(addable)
//	{
//		addable = false;
		target = t_target;
		selector = t_selector;
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		id _v = [[UIApplication sharedApplication] delegate];
		UIAlertView* dlg = [[UIAlertView alloc] initWithTitle:[NSString stringWithCString:t_title encoding:NSUTF8StringEncoding]
													  message:[NSString stringWithCString:t_message encoding:NSUTF8StringEncoding]
													 delegate:[(AppController*)_v viewController]
											cancelButtonTitle:[NSString stringWithCString:t_leftButtonText encoding:NSUTF8StringEncoding]
											otherButtonTitles:[NSString stringWithCString:t_rightButtonText encoding:NSUTF8StringEncoding], nil];
		[dlg setTag:t_tag];
		[dlg show];
		[dlg release];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	CCLOG("double dialog in cocos2d-x");
		JniMethodInfo __minfo;
		if(JniHelper::getMethodInfo(__minfo, JNIKelper::getInstance()->getClassName().c_str(), "addDoubleDialog", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V"))
		{
			jstring param1 = __minfo.env->NewStringUTF(t_title);
			jstring param2 = __minfo.env->NewStringUTF(t_message);
			jstring param3 = __minfo.env->NewStringUTF(t_leftButtonText);
			jstring param4 = __minfo.env->NewStringUTF(t_rightButtonText);
			__minfo.env->CallVoidMethod(JNIKelper::getInstance()->getJobj(), __minfo.methodID, param1, param2, param3, param4, t_tag);
			__minfo.env->DeleteLocalRef(__minfo.classID);
			__minfo.env->DeleteLocalRef(param1);
			__minfo.env->DeleteLocalRef(param2);
			__minfo.env->DeleteLocalRef(param3);
			__minfo.env->DeleteLocalRef(param4);
		}
#endif
//	}
}

void AlertEngine::addSingleAlert(const char* t_title, const char* t_message, const char* t_checkButtonText, int t_tag, CCObject* t_target, SEL_AlertFuncII t_selector)
{
//	if(addable)
//	{
//		addable = false;
		target = t_target;
		selector = t_selector;
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		id _v = [[UIApplication sharedApplication] delegate];
		UIAlertView* dlg = [[UIAlertView alloc] initWithTitle:[NSString stringWithCString:t_title encoding:NSUTF8StringEncoding]
													  message:[NSString stringWithCString:t_message encoding:NSUTF8StringEncoding]
													 delegate:[(AppController*)_v viewController]
											cancelButtonTitle:[NSString stringWithCString:t_checkButtonText encoding:NSUTF8StringEncoding]
											otherButtonTitles:nil];
		[dlg setTag:t_tag];
		[dlg show];
		[dlg release];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		JniMethodInfo __minfo;
		if(JniHelper::getMethodInfo(__minfo, JNIKelper::getInstance()->getClassName().c_str(), "addSingleDialog", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V"))
		{
			jstring param1 = __minfo.env->NewStringUTF(t_title);
			jstring param2 = __minfo.env->NewStringUTF(t_message);
			jstring param3 = __minfo.env->NewStringUTF(t_checkButtonText);
			__minfo.env->CallVoidMethod(JNIKelper::getInstance()->getJobj(), __minfo.methodID, param1, param2, param3, t_tag);
			__minfo.env->DeleteLocalRef(__minfo.classID);
			__minfo.env->DeleteLocalRef(param1);
			__minfo.env->DeleteLocalRef(param2);
			__minfo.env->DeleteLocalRef(param3);
		}
#endif
//	}
}

void AlertEngine::doAction(int t_alertNumber, int t_buttonNumber)
{
//	addable = true;
	CCLOG("seo reaction");
	if(target && selector)(target->*selector)(t_alertNumber, t_buttonNumber);
	if(func)func(t_alertNumber,t_buttonNumber);
	CCLOG("seo reaction end");
}

AlertEngine* AlertEngine::sharedInstance()
{
	static AlertEngine* myAlert = NULL;
	if(myAlert == NULL)
	{
		myAlert = new AlertEngine();
		myAlert->myInit();
	}
	return myAlert;
}


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#ifdef __cplusplus
extern "C" {
#endif
	void Java_com_litqoo_lib_KSActivityBase_alertAction(JNIEnv *env, jobject thiz, int t_alertNumber, int t_buttonNumber)
	{
		AlertEngine::sharedInstance()->doAction(t_alertNumber, t_buttonNumber);
	}
#ifdef __cplusplus
}
#endif

#endif
