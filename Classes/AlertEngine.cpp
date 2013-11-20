


#include "AlertEngine.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include <Foundation/Foundation.h>
#include "AppController.h"
#include "RootViewController.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "JNIKelper.h"
#endif

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
	CCLog("double dialog in cocos2d-x");
		JniMethodInfo __minfo;
		if(JniHelper::getMethodInfo(__minfo, JNIKelper::getInstance()->getClassName().c_str(), "addDoubleDialog", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V"))
		{
			__minfo.env->CallVoidMethod(JNIKelper::getInstance()->getJobj(), __minfo.methodID, __minfo.env->NewStringUTF(t_title), __minfo.env->NewStringUTF(t_message), __minfo.env->NewStringUTF(t_leftButtonText), __minfo.env->NewStringUTF(t_rightButtonText), t_tag);
			__minfo.env->DeleteLocalRef(__minfo.classID);
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
			__minfo.env->CallVoidMethod(JNIKelper::getInstance()->getJobj(), __minfo.methodID, __minfo.env->NewStringUTF(t_title), __minfo.env->NewStringUTF(t_message), __minfo.env->NewStringUTF(t_checkButtonText), t_tag);
			__minfo.env->DeleteLocalRef(__minfo.classID);
		}
#endif
//	}
}

void AlertEngine::doAction(int t_alertNumber, int t_buttonNumber)
{
//	addable = true;
	CCLog("seo reaction");
	(target->*selector)(t_alertNumber, t_buttonNumber);
	CCLog("seo reaction end");
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