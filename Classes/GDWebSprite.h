#pragma once

#include <iostream>
#include "cocos2d.h"
#include "curl/curl.h"
#include <pthread.h>
#include <queue>
#include "jsoncpp/json.h"
using namespace cocos2d;
using namespace std;





class GDWebSprite: public cocos2d::CCNode{
public:
    CCSprite *webSprite;
    CCNode *defaultSprite;
    string imageUrl;
    string imageName;
    bool isDown;
    float xScale;
    float yScale;
	
	CCObject* final_target;
	SEL_CallFunc final_delegate;

	static CCSprite* create(string imgUrl, string defaultImg, CCSize size, CCObject* t_final = NULL, SEL_CallFunc d_final = NULL);
	static CCSprite* create(string imgUrl, string defaultImg, CCObject* t_final = NULL, SEL_CallFunc d_final = NULL);
	static CCSprite* create(string imgUrl, string defaultImg, string imageName, CCObject* t_final = NULL, SEL_CallFunc d_final = NULL);
	static CCSprite* create(string imgUrl, CCNode *defaultNode, CCObject* t_final = NULL, SEL_CallFunc d_final = NULL);
	static CCSprite* create(string imgUrl, CCNode *defaultNode,  CCSize size,CCObject* t_final = NULL, SEL_CallFunc d_final = NULL);
  static CCSprite* create(string imgUrl, CCNode *defaultNode, string imageName, CCSize size,CCObject* t_final = NULL, SEL_CallFunc d_final = NULL);
    
    bool init(string imgUrl, string defaultImg, CCObject* t_final = NULL, SEL_CallFunc d_final = NULL);
    bool init(string imgUrl, CCNode *defaultNode, CCObject* t_final = NULL, SEL_CallFunc d_final = NULL);
    bool init(string imgUrl, string defaultImg, string imageName, CCObject* t_final = NULL, SEL_CallFunc d_final = NULL);
    bool init(string imgUrl, CCNode *defaultNode, string imageName, CCSize size,CCObject* t_final = NULL, SEL_CallFunc d_final = NULL);

    static void removeAllSprite(); //remove all sprite
    
    
    
    static void startDownload(GDWebSprite *newSprite);
    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static void* t_function(void * _caller);
    void finishDownload();
    void changeWebSprite(CCTexture2D *pTexture);
	static void removeCache();
	static void printCache();
};

struct GDWebSpriteMemoryStruct {
    char *memory;
    size_t size;
		string url;
};

class GDWebSpriteManager {
public:
    static GDWebSpriteManager* get()
    {
        
        static GDWebSpriteManager* _ins = 0;
        if(_ins == 0){
            _ins = new GDWebSpriteManager();
        }
        return _ins;
    }
    
    
    GDWebSpriteManager(){
        webImages = new CCArray;
        webImages->init();
        isDownloding=false;
        downloadIndex=0;
			
        
    }
    
    CCArray* webImages;
    bool isDownloding;
    int downloadIndex;
    std::map<int,GDWebSpriteMemoryStruct> chunks;
};


