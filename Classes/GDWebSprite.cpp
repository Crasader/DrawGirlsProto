#include "GDWebSprite.h"
#include "GDLib.h"
#include "StageImgLoader.h"

void GDWebSprite::removeAllSprite(){
    GDWebSpriteManager::get()->webImages->removeAllObjects();
}

void GDWebSprite::startDownload(GDWebSprite *newSprite){
    
    GDWebSpriteManager::get()->webImages->addObject(newSprite);
    //start thread
    
    if(GDWebSpriteManager::get()->isDownloding==false){
        GDWebSpriteManager::get()->isDownloding=true;
        pthread_t p_thread;
        int thr_id;
        // 쓰레드 생성 아규먼트로 1 을 넘긴다.
        thr_id = pthread_create(&p_thread, NULL, t_function, newSprite);
        if (thr_id < 0)
        {
            perror("thread create error : ");
            exit(0);
        }
    }
    
}

size_t GDWebSprite::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    
    size_t realsize = size * nmemb;
    struct GDWebSpriteMemoryStruct *mem = (struct GDWebSpriteMemoryStruct *)userp;
    
    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    
    if (mem->memory == NULL) {
        /* out of memory! */
        CCLOG("not enough memory (realloc returned NULL)");
        exit(EXIT_FAILURE);
    }
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

void* GDWebSprite::t_function(void * _caller)
{
    //FBConnector* caller = (FBConnector*)_caller;
    
    CURL *curl_handle;
    CURLcode retCode = curl_global_init(CURL_GLOBAL_ALL);
    if(retCode != CURLE_OK){
        GDWebSpriteManager::get()->isDownloding=false;
        return NULL;
    }
    curl_handle = curl_easy_init();
    int reDownCnt=0;
    while (1) {
        string pURL;
        if(GDWebSpriteManager::get()->webImages->count()<=GDWebSpriteManager::get()->downloadIndex){
            GDWebSpriteManager::get()->isDownloding=false;
            break;
        }else{
            pURL=((GDWebSprite *)GDWebSpriteManager::get()->webImages->objectAtIndex(GDWebSpriteManager::get()->downloadIndex))->imageUrl;
        }
        
        //CCLOG("start downloadIndex : %d", GDWebSpriteManager::get()->downloadIndex);
        
        GDWebSpriteMemoryStruct chunk = {(char*)malloc(1), 0, pURL};
        int downI = GDWebSpriteManager::get()->downloadIndex;
        curl_easy_setopt(curl_handle, CURLOPT_URL, pURL.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, true);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
				curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
				curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1L);
				curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT_MS, 20000);
				curl_easy_setopt(curl_handle, CURLOPT_TCP_KEEPALIVE, 1);
				curl_easy_setopt(curl_handle, CURLOPT_TCP_KEEPIDLE,5); //5초대기
				curl_easy_setopt(curl_handle, CURLOPT_TCP_KEEPINTVL,5); //5초대기
			
        if(curl_easy_perform(curl_handle) == CURLE_OK){
            //CCLOG("finish downloadIndex : %d", GDWebSpriteManager::get()->downloadIndex);
            GDWebSpriteManager::get()->chunks.push(pair<GDWebSpriteMemoryStruct, int>(chunk, downI));
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GDWebSprite::finishDownload), (GDWebSprite *)(GDWebSpriteManager::get()->webImages->objectAtIndex(GDWebSpriteManager::get()->downloadIndex)), 0, false, 0, 0);
            GDWebSpriteManager::get()->downloadIndex++;
        }else{
             free(chunk.memory);
            if(reDownCnt>3){
                reDownCnt=0;
                GDWebSpriteManager::get()->downloadIndex++;
            }else{
                reDownCnt++;
            }
        }
    }
    GDWebSpriteManager::get()->isDownloding=false;
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
    return NULL;
}

CCSprite* GDWebSprite::create(string imgUrl, CCNode *defaultNode, string imageName, CCObject* t_final, SEL_CallFunc d_final){
	
    if(imgUrl==""){
		if(t_final && d_final){
				(t_final->*d_final)();
		}
        
		return (CCSprite *)defaultNode;
    }
	
	
	string filecache = CCUserDefault::sharedUserDefault()->getStringForKey("gdwebspritelist", "[]");
	Json::Value list = filecache;
	
	for (int i=0; i<list.size(); i++) {
		Json::Value info = list[i];
		if(info["url"].asString()==imgUrl){
			string filename = info["filename"].asString();
			CCSprite* spr = mySIL->getUnsafeLoadedImg(filename);
			CCLOG("GDWebSprite find %s -> %s",info["url"].asString().c_str(),info["filename"].asString().c_str());
			if(spr)return spr;
		}
	}
	
  
    //1. webImages 검사해서 값있으면 그냥 리턴
    for(int i=0;i<GDWebSpriteManager::get()->webImages->count();i++){
        GDWebSprite* image = (GDWebSprite*)GDWebSpriteManager::get()->webImages->objectAtIndex(i);
        if(image->imageUrl==imgUrl || (image->imageName==imageName && imageName!="")){
            if(image->webSprite!=NULL){
                CCNode *newimg = CCNode::create();
                CCSprite* ret = CCSprite::createWithTexture(image->webSprite->getTexture(),image->webSprite->getTextureRect());
                
                ret->setScaleX(image->xScale);
                ret->setScaleY(image->yScale);
                
                newimg->addChild(ret);
                ret->setAnchorPoint(CCPointZero);
                ret->setPosition(CCPointZero);
                ret->setContentSize(CCSizeMake(ret->getContentSize().width*image->xScale,ret->getContentSize().height*image->yScale));
                newimg->setContentSize(ret->getContentSize());
				if(t_final && d_final){
					(t_final->*d_final)();
				}
                return (CCSprite *)newimg;
            }
        }
    }
    
    
    //2. 새로운 이미지이면 webImages에 값넣고 스프라잇
    GDWebSprite* _ws = new GDWebSprite;
    _ws->init(imgUrl, defaultNode, imageName, t_final, d_final);
    _ws->autorelease();
    
    GDWebSprite::startDownload(_ws);
    
    return (CCSprite*)_ws;
}

CCSprite* GDWebSprite::create(string imgUrl, string defaultImg, CCObject* t_final, SEL_CallFunc d_final){
	CCNode *defalutNode = CCSprite::create(defaultImg.c_str());
	if(!defalutNode)
		defalutNode = CCNode::create();

	return GDWebSprite::create(imgUrl,defalutNode,"", t_final, d_final);
}


CCSprite* GDWebSprite::create(string imgUrl, CCNode* defaultNode, CCObject* t_final, SEL_CallFunc d_final){
    return GDWebSprite::create(imgUrl,defaultNode,"", t_final, d_final);
}

CCSprite* GDWebSprite::create(string imgUrl, string defaultImg, string imageName, CCObject* t_final, SEL_CallFunc d_final){
    CCNode *defalutNode = CCSprite::create(defaultImg.c_str());
    return GDWebSprite::create(imgUrl,defalutNode,imageName, t_final, d_final);
}


bool GDWebSprite::init(string imgUrl, string defaultImg, CCObject* t_final, SEL_CallFunc d_final){
    return this->init(imgUrl,defaultSprite,"", t_final, d_final);
}

bool GDWebSprite::init(string imgUrl, CCNode* defaultNode, CCObject* t_final, SEL_CallFunc d_final){
    return this->init(imgUrl,defaultNode,"", t_final, d_final);
}

bool GDWebSprite::init(string imgUrl, string defaultImg, string imageName, CCObject* t_final, SEL_CallFunc d_final){
    defaultSprite = CCSprite::create(defaultImg.c_str());
    return this->init(imgUrl,defaultSprite,imageName, t_final, d_final);
}

bool GDWebSprite::init(string imgUrl, CCNode *defaultNode, string imageName, CCObject* t_final, SEL_CallFunc d_final){
    
    if(!CCNode::init()){
        return false;
    }
    
	final_target = t_final;
	final_delegate = d_final;
	
    defaultSprite = defaultNode;
    this->addChild(defaultSprite,1);
    this->setContentSize(CCSize(defaultSprite->getContentSize().width*defaultSprite->getScaleX(),defaultSprite->getContentSize().height*defaultSprite->getScaleY()));
    CCSize s = getContentSize();
    defaultSprite->setAnchorPoint(ccp(0.5,0.5));
    defaultSprite->setPosition(ccp(s.width / 2, s.height / 2));
    this->imageName=imageName;
    this->imageUrl=imgUrl;
    webSprite=NULL;
    isDown=false;
    xScale=1;
    yScale=1;
    return true;
}



void GDWebSprite::finishDownload(){
    if(!GDWebSpriteManager::get()->chunks.empty())
    {
        
        CCImage* img = new CCImage;
        
        CCTexture2D* texture = new CCTexture2D();
        auto chunk_index = GDWebSpriteManager::get()->chunks.front();
        GDWebSpriteManager::get()->chunks.pop();
      
        
        try {
            
            if(img->initWithImageData(chunk_index.first.memory, (long)chunk_index.first.size, CCImage::kFmtUnKnown) == false)
                throw "..";
          
						string filename = "gdw"+GraphDogLib::random_string(10)+".png";
						string filecache = CCUserDefault::sharedUserDefault()->getStringForKey("gdwebspritelist", "[]");
						Json::Value list = filecache;
						Json::Value newfile;
						newfile["url"]=chunk_index.first.url;
						newfile["filename"]=filename;
						list.append(newfile);
						if(img->saveToFile((mySIL->getDocumentPath().c_str()+filename).c_str())){
							
							CCLOG("GDWebSprite : save ok %s, %s",newfile["url"].asString().c_str(),(mySIL->getDocumentPath().c_str()+filename).c_str());

							if(list.size()>30){
								//CCLOG("removed1 -> %s",list.toStyledString().c_str());
								Json::Value rFileInfo =list[0];
								string rFilename =rFileInfo.get("filename", "fn").asString();
								CCLOG("remove file %s",(mySIL->getDocumentPath()+rFilename).c_str());

								remove((mySIL->getDocumentPath()+rFilename).c_str());
								CCLOG("removed1 -> %s",list.toStyledString().c_str());
								
								
								Json::Value newObject;
								int valueSize = list.size();
								for(int i=1; i<valueSize; i++)
								{
										newObject.append(list[i]);
								}
								
								CCLOG("removed2 -> %s",newObject.toStyledString().c_str());
								CCUserDefault::sharedUserDefault()->setStringForKey("gdwebspritelist",newObject.asString().c_str());
								//delete newimg;
							}else{
								CCUserDefault::sharedUserDefault()->setStringForKey("gdwebspritelist",list.asString().c_str());
							}
							
							
							CCUserDefault::sharedUserDefault()->flush();
						}else{
							CCLOG("GDWebSprite : save fail %s, %s",newfile["url"].asString().c_str(),newfile["filename"].asString().c_str());
						}
					
					
            if(texture->initWithImage(img) == false)
                throw "..";
            
            //CCLOG("showing downimage %d",chunk_index.second);
            
            GDWebSprite *_sprite = (GDWebSprite *)GDWebSpriteManager::get()->webImages->objectAtIndex(chunk_index.second);
            
            _sprite->changeWebSprite(texture);
            
            if(chunk_index.first.memory)
                free(chunk_index.first.memory);
            
            
            texture->release();
            delete img;// in cocos2d-x 1.x
        } catch (...) {
            if(chunk_index.first.memory)
                free(chunk_index.first.memory);
            
            texture->release();
            delete img;// in cocos2d-x 1.x
        }
	}
	
	if(final_target && final_delegate)
	{
		(final_target->*final_delegate)();
	}
}

void GDWebSprite::changeWebSprite(CCTexture2D *pTexture){
    if(this->isDown)return;
    this->webSprite=CCSprite::createWithTexture(pTexture);
    CCSize s = getContentSize();
    this->xScale = this->defaultSprite->getContentSize().width/this->webSprite->getContentSize().width*this->defaultSprite->getScaleX();
    this->yScale = this->defaultSprite->getContentSize().height/this->webSprite->getContentSize().height*this->defaultSprite->getScaleY();
    this->webSprite->setAnchorPoint(ccp(0.5,0.5));
    this->webSprite->setPosition(ccp(s.width / 2, s.height / 2));
    this->webSprite->setScaleX(xScale);
    this->webSprite->setScaleY(yScale);
    this->addChild(this->webSprite,2);
    this->isDown=true;
    this->removeChild(this->defaultSprite);
    this->defaultSprite=NULL;
}
