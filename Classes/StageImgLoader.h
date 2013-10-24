//
//  StageImgLoader.h
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 10..
//
//

#ifndef __DrawingJack__StageImgLoader__
#define __DrawingJack__StageImgLoader__

#include "cocos2d.h"
#include <pthread.h>
#include "curl/curl.h"
#include "SaveData.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/CCFileUtils.h"
#endif

using namespace cocos2d;
using namespace std;

struct LMemoryStruct {
    size_t size;
	string filename;
	FILE* stream;
	string writeable_path;
};

#define mySIL StageImgLoader::sharedInstance()

class StageImgLoader : public CCNode
{
public:
	static StageImgLoader* sharedInstance()
	{
		static StageImgLoader* t_loader = NULL;
		if(t_loader == NULL)
		{
			t_loader = new StageImgLoader();
			t_loader->myInit();
		}
		return t_loader;
	}
	
	CCSprite* getLoadedImg(string filename);
	CCSprite* getLoadedImg(string filename, CCRect t_rect);
	
	void downloadImg(string t_url, int t_size, string t_down_filename, CCObject* t_success, SEL_CallFunc d_success, CCObject* t_fail, SEL_CallFunc d_fail)
	{
		target_success = t_success;
		delegate_success = d_success;
		target_fail = t_fail;
		delegate_fail = d_fail;
		down_filename = t_down_filename;
		
		startDownload(t_url, t_size);
	}
	
	void removeTD()
	{
		target_success = NULL;
		target_fail = NULL;
	}
	
	bool isLoadedImg(string filename)
	{
//		return my_savedata->getValue(kSDF_downloadedInfo, filename, 0) == 1;
		return false;
	}
	
	float getDownloadPercentage();
	
	string writeable_path;
	string down_filename;
	
	CCTexture2D* addImage(const char * path);
	
private:
	SaveData* my_savedata;
	
	CCObject* target_success;
	SEL_CallFunc delegate_success;
	CCObject* target_fail;
	SEL_CallFunc delegate_fail;
	
	string p_url;
	size_t total_size;
	bool isFail;
	
	void successAction();
	void failAction();
	
	void startDownload(string t_url, int t_size)
	{
		p_url = t_url;
		total_size = t_size*1024;
		
		isFail = false;
		pthread_t p_thread;
		int thr_id;
		// 쓰레드 생성 아규먼트로 1 을 넘긴다.
		thr_id = pthread_create(&p_thread, NULL, t_function, NULL);
		if (thr_id < 0)
		{
			perror("thread create error : ");
			exit(0);
		}
	}
	
	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
	static void* t_function(void *data);
	
	string getDocumentPath();
	
	void myInit()
	{
		my_savedata = SaveData::sharedObject();
		my_savedata->createJSON(kSDF_downloadedInfo);
		writeable_path = getDocumentPath();
	}
};

#endif /* defined(__DrawingJack__StageImgLoader__) */
