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
            GDWebSpriteManager::get()->chunks[downI]=chunk;
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GDWebSprite::finishDownload), (GDWebSprite *)(GDWebSpriteManager::get()->webImages->objectAtIndex(GDWebSpriteManager::get()->downloadIndex)), 0, false, 0, 0);
            GDWebSpriteManager::get()->downloadIndex++;
        }else{
					chunk.url="";
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

CCSprite* GDWebSprite::create(string imgUrl, CCNode *defaultNode, string imageName,CCSize size, CCObject* t_final, SEL_CallFunc d_final){
	
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
			if(size.height!=0)spr->setContentSize(size);
			CCLOG("GDWebSprite find %s -> %s",info["url"].asString().c_str(),info["filename"].asString().c_str());
			
			if(t_final && d_final)
			{
				(t_final->*d_final)();
			}
			
			if(spr)return spr;
		}
	}
	
	CCLOG("dont find");

	
	for(auto iter = GDWebSpriteManager::get()->chunks.begin();iter!=GDWebSpriteManager::get()->chunks.end();iter++){
		if(iter->second.url==imgUrl){
			
			CCImage* img = new CCImage;
			
			CCTexture2D* texture = new CCTexture2D();
			img->initWithImageData(iter->second.memory, (long)iter->second.size, CCImage::kFmtPng);
			texture->initWithImage(img);
			CCSprite *n =  CCSprite::createWithTexture(texture);
			img->release();
			texture->release();
			
			if(t_final && d_final)
			{
				(t_final->*d_final)();
			}
			return n;
		
		}
	}
	
	
	
//
//    //1. webImages 검사해서 값있으면 그냥 리턴
//    for(int i=0;i<GDWebSpriteManager::get()->webImages->count();i++){
//        GDWebSprite* image = (GDWebSprite*)GDWebSpriteManager::get()->webImages->objectAtIndex(i);
//        if(image->imageUrl==imgUrl || (image->imageName==imageName && imageName!="")){
//            if(image->webSprite!=NULL){
//  //              CCNode *newimg = CCNode::create();
//							auto chunk_index = GDWebSpriteManager::get()->chunks.begin();
//							for(auto iter = GDWebSpriteManager::get()->chunks.begin();iter!=GDWebSpriteManager::get()->chunks.end();iter++){
//								if(iter->second.url==imgUrl){
//									chunk_index = iter;
//								}
//							}
//							
//                CCSprite* ret = CCSprite::createWithTexture(image->webSprite->getTexture(),image->webSprite->getTextureRect());
//                
////                //ret->setScaleX(image->xScale/2.f);
////                ret->setScaleY(image->yScale/2.f);
////                
////                newimg->addChild(ret);
////                ret->setAnchorPoint(CCPointZero);
////                ret->setPosition(CCPointZero);
////                ret->setContentSize(CCSizeMake(ret->getContentSize().width*image->xScale/2.f,ret->getContentSize().height*image->yScale/2.f));
////                newimg->setContentSize(ret->getContentSize());
////								if(t_final && d_final){
////									(t_final->*d_final)();
////								}
//								//newimg->setScale(0.5);
//                return ret;
//            }
//        }
//    }
  
    
    //2. 새로운 이미지이면 webImages에 값넣고 스프라잇
    GDWebSprite* _ws = new GDWebSprite;
    _ws->init(imgUrl, defaultNode, imageName,size, t_final, d_final);
    _ws->autorelease();
    
    GDWebSprite::startDownload(_ws);
    
    return (CCSprite*)_ws;
}


CCSprite* GDWebSprite::create(string imgUrl, string defaultImg,CCSize size, CCObject* t_final, SEL_CallFunc d_final){
	CCNode *defalutNode = CCSprite::create(defaultImg.c_str());
	if(!defalutNode)
		defalutNode = CCNode::create();
	
	return GDWebSprite::create(imgUrl,defalutNode,"",size, t_final, d_final);
}

CCSprite* GDWebSprite::create(string imgUrl, CCNode *defaultNode,  CCSize size,CCObject* t_final, SEL_CallFunc d_final){
	return GDWebSprite::create(imgUrl,defaultNode,"",size, t_final, d_final);
}

CCSprite* GDWebSprite::create(string imgUrl, string defaultImg,CCObject* t_final, SEL_CallFunc d_final){
	CCNode *defalutNode = CCSprite::create(defaultImg.c_str());
	if(!defalutNode)
		defalutNode = CCNode::create();

	return GDWebSprite::create(imgUrl,defalutNode,"",CCSizeMake(0, 0), t_final, d_final);
}


CCSprite* GDWebSprite::create(string imgUrl, CCNode* defaultNode, CCObject* t_final, SEL_CallFunc d_final){
    return GDWebSprite::create(imgUrl,defaultNode,"",CCSizeMake(0, 0), t_final, d_final);
}

CCSprite* GDWebSprite::create(string imgUrl, string defaultImg, string imageName, CCObject* t_final, SEL_CallFunc d_final){
    CCNode *defalutNode = CCSprite::create(defaultImg.c_str());
    return GDWebSprite::create(imgUrl,defalutNode,imageName,CCSizeMake(0, 0), t_final, d_final);
}


bool GDWebSprite::init(string imgUrl, string defaultImg, CCObject* t_final, SEL_CallFunc d_final){
    return this->init(imgUrl,defaultSprite,"",CCSizeMake(0, 0), t_final, d_final);
}

bool GDWebSprite::init(string imgUrl, CCNode* defaultNode, CCObject* t_final, SEL_CallFunc d_final){
    return this->init(imgUrl,defaultNode,"",CCSizeMake(0, 0), t_final, d_final);
}

bool GDWebSprite::init(string imgUrl, string defaultImg, string imageName, CCObject* t_final, SEL_CallFunc d_final){
    defaultSprite = CCSprite::create(defaultImg.c_str());
    return this->init(imgUrl,defaultSprite,imageName,CCSizeMake(0, 0), t_final, d_final);
}

bool GDWebSprite::init(string imgUrl, CCNode *defaultNode, string imageName,CCSize size, CCObject* t_final, SEL_CallFunc d_final){
    
    if(!CCNode::init()){
        return false;
    }
    
	final_target = t_final;
	final_delegate = d_final;
    defaultSprite = defaultNode;
    this->addChild(defaultSprite,1);
	
	if(size.width==0 && size.height==0)
		this->setContentSize(CCSize(defaultSprite->getContentSize().width*defaultSprite->getScaleX(),defaultSprite->getContentSize().height*defaultSprite->getScaleY()));
	else
		this->setContentSize(size);
  
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
			
			auto chunk_index = GDWebSpriteManager::get()->chunks.begin();
      
			for(auto iter = GDWebSpriteManager::get()->chunks.begin();iter!=GDWebSpriteManager::get()->chunks.end();iter++){
				if(iter->second.url==this->imageUrl){
					chunk_index = iter;
				}
			}
			
			//GDWebSpriteManager::get()->chunks.pop();
      
        
        try {
            
            if(img->initWithImageData(chunk_index->second.memory, (long)chunk_index->second.size, CCImage::kFmtPng) == false)
                throw "..";
          
						string filename = "gdw"+GraphDogLib::random_string(10)+".png";
						string filecache = CCUserDefault::sharedUserDefault()->getStringForKey("gdwebspritelist", "[]");
						Json::Value list = filecache;
						Json::Value newfile;
						newfile["url"]=chunk_index->second.url;
						newfile["filename"]=filename;
						
						if(img->saveToFile((mySIL->getDocumentPath().c_str()+filename).c_str(),false)){
							
							list.append(newfile);
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
            
            GDWebSprite *_sprite = (GDWebSprite *)GDWebSpriteManager::get()->webImages->objectAtIndex(chunk_index->first);
            
            _sprite->changeWebSprite(texture);
            
            //if(chunk_indexfirst.memory)
              //  free(chunk_index.first.memory);
            
            
            texture->release();
            delete img;// in cocos2d-x 1.x
        } catch (...) {
            //if(chunk_index.first.memory)
            //    free(chunk_index.first.memory);
            
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
		CCSize s = getContentSize();

//		this->webSprite->setPosition(s);
//    this->xScale = s.width/this->webSprite->getContentSize().width*this->getScaleX();
//		this->yScale = s.height/this->webSprite->getContentSize().height*this->getScaleY();
  
	
	
		CCSprite *n=CCSprite::createWithTexture(pTexture);
		n->setAnchorPoint(this->getAnchorPoint());
    n->setPosition(this->getPosition());
		this->getParent()->addChild(n,this->getZOrder());
	this->removeFromParent();
		//GDWebSpriteManager::get()->webImages->removeObject(this);
//	this->setVisible(false);//(false);
//		this->release();
//		this->setContentSize(CCSizeMake(this->webSprite->getContentSize().width/2.f,this->webSprite->getContentSize().height/2.f));
//		this->setScaleY(this->webSprite->getScaleY());
//		this->setScaleX(this->webSprite->getScaleX());
//	
//	//this->webSprite->setScale(2);
//		//this->webSprite->setScaleX(xScale);
//    //this->webSprite->setScaleY(yScale);
//		
//		this->addChild(this->webSprite,2);
//    this->isDown=true;
//  
//	this->removeChild(this->defaultSprite);
//    this->defaultSprite=NULL;
}

void GDWebSprite::printCache(){
	
	string filecache = CCUserDefault::sharedUserDefault()->getStringForKey("gdwebspritelist", "[]");
	CCLOG("gdwebsprite cache %s",filecache.c_str());
	
}

void GDWebSprite::removeCache(){
	GDWebSpriteManager::get()->webImages->removeAllObjects();
	GDWebSpriteManager::get()->chunks.clear();
	GDWebSpriteManager::get()->downloadIndex=0;
	string filecache = CCUserDefault::sharedUserDefault()->getStringForKey("gdwebspritelist", "[]");
	Json::Value list = filecache;
	
	for(int i=0; i<filecache.size(); i++)
	{
		Json::Value rFileInfo =list[i];
		string rFilename =rFileInfo.get("filename", "fn").asString();
		remove((mySIL->getDocumentPath()+rFilename).c_str());
	}
	CCUserDefault::sharedUserDefault()->setStringForKey("gdwebspritelist", "[]");
	CCUserDefault::sharedUserDefault()->flush();
}
