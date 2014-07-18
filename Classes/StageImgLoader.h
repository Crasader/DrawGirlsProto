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

class DownloadImgInfo
{
public:
	string download_url;
	int download_size;
	string download_filename;
	function<void(string)> success_func;
	function<void(string)> fail_func;
	bool is_fail;
	
	bool operator== (const DownloadImgInfo a)
	{
		return download_filename == a.download_filename;
	}
	
	DownloadImgInfo(string t_filename) : download_filename(t_filename)
	{
		DownloadImgInfo();
	}
	
	DownloadImgInfo() :
	is_fail(false)
	{}
};

#define mySIL StageImgLoader::sharedInstance()

class StageImgLoader : public CCNode
{
public:
	static StageImgLoader* sharedInstance();
	
	CCSprite* getLoadedImg(string filename);
	CCSprite* getLoadedImg(string filename, CCRect t_rect);
	
	void downloadImg(string t_url, int t_size, string t_down_filename, CCObject* t_success, SEL_CallFunc d_success, CCObject* t_fail, SEL_CallFunc d_fail);
	void downloadImgSet(vector<DownloadImgInfo> t_list);
	
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
	
	void successActionSet1();
	void successActionSet2();
	void successActionSet3();
	void successActionSet4();
	void successActionSet5();
	void failActionSet1();
	void failActionSet2();
	void failActionSet3();
	void failActionSet4();
	void failActionSet5();
	
	void startDownload(string t_url, int t_size);
	int end_cnt;
	void startDownloadSet();
	
	void endDownloadSet();
	
	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
	static void* t_function(void *data);
	static void* t_function2(void *data);
	
	void myInit();
};

#endif /* defined(__DrawingJack__StageImgLoader__) */
