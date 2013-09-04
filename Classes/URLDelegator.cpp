#include "URLDelegator.h"
#include "InAppDelegator.h"

#ifdef __cplusplus
extern "C" {
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	void Java_com_litqoo_lib_FBConnectorBase_sendWebViewData(JNIEnv *env, jobject thiz, jstring datas, int delekey) // 자바에서 데이터 받음!!
	{
		CCLog("call urldelegator");
		jboolean isCopy = JNI_FALSE;
		const char* revStr = env->GetStringUTFChars(datas, &isCopy); // UTF-8 문자를 얻어옴.
		string throwData = revStr;
		URLDelegator::DeleSel delesel = URLDelegator::getInstance()->getDeleSel(delekey);
		URLDelegator::getInstance()->removeDeleSel(delekey);
		if(delesel.target != 0 && delesel.selector != 0)
		{
			((delesel.target)->*(delesel.selector))(throwData);
		}
		
		env->ReleaseStringUTFChars(datas, revStr);
	}
	void Java_com_litqoo_lib_FBConnectorBase_sendInAppData(JNIEnv *env, jobject thiz, jstring datas, int resultcode, int delekey) // 자바에서 데이터 받음!!
	{
		CCLog("call urldelegator");
		jboolean isCopy = JNI_FALSE;
		const char* revStr = env->GetStringUTFChars(datas, &isCopy); // UTF-8 문자를 얻어옴.
		string throwData = revStr;
		InAppDelegator::DeleSel delesel = InAppDelegator::getInstance()->getDeleSel(delekey);
		InAppDelegator::getInstance()->removeDeleSel(delekey);
		if(delesel.target != 0 && delesel.selector != 0)
		{
			((delesel.target)->*(delesel.selector))(throwData, resultcode);
		}
		
		env->ReleaseStringUTFChars(datas, revStr);
	}
	void Java_com_litqoo_lib_FBConnectorBase_sendDataFromURL(JNIEnv *env, jobject thiz, jstring datas, int delekey) // 자바에서 데이터 받음!!
	{
		CCLog("call urldelegator");
		jboolean isCopy = JNI_FALSE;
		const char* revStr = env->GetStringUTFChars(datas, &isCopy); // UTF-8 문자를 얻어옴.
		//CCLog("nativeCall!! urldata" );
		string throwData = revStr;
		URLDelegator::DeleSel delesel = URLDelegator::getInstance()->getDeleSel(delekey);
		URLDelegator::getInstance()->removeDeleSel(delekey);
		if(delesel.target != 0 && delesel.selector != 0)
		{
			((delesel.target)->*(delesel.selector))(throwData);
		}
		//void (CCLayer::*_selector)() = new void (CCLayer::
		env->ReleaseStringUTFChars(datas, revStr);
	}
#endif
#ifdef __cplusplus
}
#endif