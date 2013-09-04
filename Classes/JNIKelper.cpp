#include "JNIKelper.h"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void JNIKelper::setJNI(string _className)
{
	CCLog("call getActivity");
	className = _className;
	if(JniHelper::getStaticMethodInfo(minfo, className.c_str(), "getActivity", "()Ljava/lang/Object;"))
	{
		jobj = minfo.env->NewGlobalRef(minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID));
		minfo.env->DeleteLocalRef(minfo.classID);
	}
	CCLog("call getActivity end");
}
void JNIKelper::releaseJNI()
{
	//CCLog("nonono");
	minfo.env->DeleteGlobalRef(jobj);
}

void JNIKelper::callJava_buyItem(string itemID, CCObject* target, RecvInAppSelType selector)
{
	CCLog("call jnikelper");
	JniMethodInfo __minfo;
	__minfo.classID = 0;
	__minfo.env = 0;
	__minfo.methodID = 0;
	if(JniHelper::getMethodInfo(__minfo, className.c_str(), "buyItem", "(Ljava/lang/String;I)V")) // itemID, delekey
	{		
		int getKey = InAppDelegator::getInstance()->addDeleSel(target, selector);
		__minfo.env->CallVoidMethod(jobj, __minfo.methodID, __minfo.env->NewStringUTF(itemID.c_str()), getKey);
		__minfo.env->DeleteLocalRef(__minfo.classID);
	}
}

string JNIKelper::callJava_getPrice(string itemID)
{
	CCLog("call jnikelper");
	JniMethodInfo __minfo;
	__minfo.classID = 0;
	__minfo.env = 0;
	__minfo.methodID = 0;
	string p;
	if(JniHelper::getMethodInfo(__minfo, className.c_str(), "getPrice", "(Ljava/lang/String;)Ljava/lang/String;"))
	{
		jstring jstrTitle = (jstring)__minfo.env->CallObjectMethod(jobj, __minfo.methodID, __minfo.env->NewStringUTF(itemID.c_str()));
		//__minfo.env->CallO
		//__minfo.env->CallVoidMethod(jobj, __minfo.methodID);
		if(jstrTitle)
		{

			char* pszTitle = (char*)__minfo.env->GetStringUTFChars(jstrTitle, JNI_FALSE);

			p = pszTitle;

			__minfo.env->ReleaseStringUTFChars(jstrTitle, pszTitle);
			__minfo.env->DeleteLocalRef(jstrTitle);

		}
		
		__minfo.env->DeleteLocalRef(__minfo.classID);
	}
	if(p == "")
		return "0";
	else
		return p;
}
void JNIKelper::callJava_addWebView(string utf8url, int x, int y, int sx, int sy, 
	CCObject* target, RecvSelType selector)
{
	CCLog("call jnikelper");
	JniMethodInfo __minfo;
	__minfo.classID = 0;
	__minfo.env = 0;
	__minfo.methodID = 0;
	if(JniHelper::getMethodInfo(__minfo, className.c_str(), "addWebView", "(Ljava/lang/String;IIIII)V"))
	{
		int getKey = URLDelegator::getInstance()->addDeleSel(target, selector);
		__minfo.env->CallVoidMethod(jobj, __minfo.methodID, __minfo.env->NewStringUTF(utf8url.c_str()), 
			x, y, sx, sy, getKey);
		__minfo.env->DeleteLocalRef(__minfo.classID);
	}
}
void JNIKelper::callJava_simple(string functionName)
{
	CCLog("call jnikelper");
	JniMethodInfo __minfo;
	__minfo.classID = 0;
	__minfo.env = 0;
	__minfo.methodID = 0;
	if(JniHelper::getMethodInfo(__minfo, className.c_str(), functionName.c_str(), "()V"))
	{
		__minfo.env->CallVoidMethod(jobj, __minfo.methodID);
		__minfo.env->DeleteLocalRef(__minfo.classID);
	}
}
void JNIKelper::callJava_moveWebView(int x, int y)
{
	CCLog("call jnikelper");
	JniMethodInfo __minfo;

	if(JniHelper::getMethodInfo(__minfo, className.c_str(), "moveWebView", "(II)V"))
	{
		__minfo.env->CallVoidMethod(jobj, __minfo.methodID, x, y);
		__minfo.env->DeleteLocalRef(__minfo.classID);
	}
}

void JNIKelper::callJava_getDataFromURL(string utf8url, CCObject* target, RecvSelType selector)
{
	CCLog("call jnikelper");
	JniMethodInfo __minfo;

	if(JniHelper::getMethodInfo(__minfo, className.c_str(), "getDataFromURL", "(Ljava/lang/String;I)V"))
	{
		int getKey = URLDelegator::getInstance()->addDeleSel(target, selector);
		__minfo.env->CallVoidMethod(jobj, __minfo.methodID, __minfo.env->NewStringUTF(utf8url.c_str()), 
			getKey);
		__minfo.env->DeleteLocalRef(__minfo.classID);
	}
}

void JNIKelper::callJava_openApp(string app_name)
{
	CCLog("call jnikelper");
	JniMethodInfo __minfo;
	
	if(JniHelper::getMethodInfo(__minfo, className.c_str(), "openApp", "(Ljava/lang/String;)V"))
	{
		__minfo.env->CallVoidMethod(jobj, __minfo.methodID, __minfo.env->NewStringUTF(app_name.c_str()));
		__minfo.env->DeleteLocalRef(__minfo.classID);
	}
}
#endif