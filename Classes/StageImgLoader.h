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
	static StageImgLoader* sharedInstance();
	
	CCSprite* getLoadedImg(string filename);
	CCSprite* getLoadedImg(string filename, CCRect t_rect);
	
	void downloadImg(string t_url, int t_size, string t_down_filename, CCObject* t_success, SEL_CallFunc d_success, CCObject* t_fail, SEL_CallFunc d_fail);
	
	void removeTD();
	
	bool isLoadedImg(string filename);
	
	float getDownloadPercentage();
	
	string writeable_path;
	string down_filename;
	
	CCTexture2D* addImage(const char * path);
	void addImageAsync(const char *path, CCObject *target, SEL_CallFuncO selector);
	
	string getDocumentPath();
	
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
	
	void startDownload(string t_url, int t_size);
	
	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
	static void* t_function(void *data);
	
	void myInit();
};

#endif /* defined(__DrawingJack__StageImgLoader__) */
