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
#include "GraphDog.h"

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

enum DownAfterScene{
	kDAS_Empty = 0,
	kDAS_ChapterSetting,
	kDAS_StageSetting,
	kDAS_Gallery,
	kDAS_WorldMapScene
};

enum SIL_Key{
	kSIL_Key_isLoadedGirlChapter_int1_Stage_int2 = 1
};

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
	
	CCSprite* getLoadedImg(int t_i1, int t_i2);
	
	void downloadImgForCS(int t_i1, int t_i2, CCObject* t_success, SEL_CallFunc d_success, CCObject* t_fail, SEL_CallFunc d_fail)
	{
		loading_chapter = t_i1;
		loading_stage = t_i2;
		target_success = t_success;
		delegate_success = d_success;
		target_fail = t_fail;
		delegate_fail = d_fail;
		
		JsonBox::Object param;
		
		param["limit"] = JsonBox::Value(1);
		param["category"] = JsonBox::Value("colorgirl");
		param["stage"] = JsonBox::Value(CCString::createWithFormat("%d-%d", t_i1, t_i2)->getCString());
		
		graphdog->command("getstages", &param, this, gd_selector(StageImgLoader::resultGetStages));
	}
	
	bool isLoadedForKey(SIL_Key t_key, int t_i1, int t_i2)
	{
		return myUD->getBoolForKey(CCString::createWithFormat(getRealKey(t_key).c_str(), t_i1, t_i2)->getCString());
	}
	
	float getDownloadPercentage();
	
	DownAfterScene getSuccessSceneCode()
	{
		DownAfterScene r_value = success_scene_code;
		success_scene_code = kDAS_Empty;
		fail_scene_code = kDAS_Empty;
		return r_value;
	}
	
	DownAfterScene getFailSceneCode()
	{
		DownAfterScene r_value = fail_scene_code;
		fail_scene_code = kDAS_Empty;
		success_scene_code = kDAS_Empty;
		return r_value;
	}
	
	void setAfterSceneCode(DownAfterScene t_success, DownAfterScene t_fail)
	{
		success_scene_code = t_success;
		fail_scene_code = t_fail;
	}
	
	string writeable_path;
	
private:
	CCUserDefault* myUD;
	int loading_chapter;
	int loading_stage;
	CCObject* target_success;
	SEL_CallFunc delegate_success;
	CCObject* target_fail;
	SEL_CallFunc delegate_fail;
	bool isSetted;
	
	DownAfterScene success_scene_code;
	DownAfterScene fail_scene_code;
	
	string p_url;
	size_t total_size;
	bool isFail;
	
	string getRealKey(SIL_Key t_key)
	{
		string r_key;
		if(t_key == kSIL_Key_isLoadedGirlChapter_int1_Stage_int2)		r_key = "bc%ds%d";
		
		return r_key;
	}
	
	void successAction();
	void failAction();
	
	void resultGetStages(JsonBox::Object result_data)
	{
		if(result_data["state"].getString() == "ok")
		{
			JsonBox::Array result_list = result_data["list"].getArray();
			JsonBox::Object t_data = result_list[0].getObject();
			JsonBox::Object recent_data = t_data["userdata"].getObject();
			p_url = recent_data["image"].getString();
			total_size = recent_data["size"].getInt()*1024;

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
			isSetted = true;
		}
		else
		{
			(target_fail->*delegate_fail)();
		}
	}
	
	CCTexture2D* addImage(const char * path);
	
	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
	static void* t_function(void *data);
	
	string getDocumentPath();
	
	void myInit()
	{
		myUD = CCUserDefault::sharedUserDefault();
		isSetted = false;
		writeable_path = getDocumentPath();
	}
};

#endif /* defined(__DrawingJack__StageImgLoader__) */
