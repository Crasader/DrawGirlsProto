//
//  StageImgLoader.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 10..
//
//

#include "StageImgLoader.h"
#include "SelectedMapData.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import <Foundation/Foundation.h>
#endif

CCSprite* StageImgLoader::getLoadedImg(string filename)
{
	CCTexture2D* texture = addImage(filename.c_str());
	return CCSprite::createWithTexture(texture);
}
CCSprite* StageImgLoader::getLoadedImg(string filename, CCRect t_rect)
{
	CCTexture2D* texture = addImage(filename.c_str());
	return CCSprite::createWithTexture(texture, t_rect);
}

CCTexture2D * StageImgLoader::addImage(const char * path)
{
	return CCTextureCache::sharedTextureCache()->addImage(path, true, getDocumentPath());
	
//	CCAssert(path != NULL, "TextureCache: fileimage MUST not be NULL");
//	
//	CCTexture2D * texture = NULL;
//	std::string fullpath = getDocumentPath()+path;
//	// all images are handled by UIImage except PVR extension that is handled by our own handler
//	do
//	{
//		CCImage::EImageFormat eImageFormat = CCImage::kFmtPng;
//		CCImage image;
//		unsigned long nSize = 0;
//		unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "rb", &nSize);
//		if (! image.initWithImageData((void*)pBuffer, nSize, eImageFormat))
//		{
//			CC_SAFE_DELETE_ARRAY(pBuffer);
//			break;
//		}
//		else
//		{
//			CC_SAFE_DELETE_ARRAY(pBuffer);
//		}
//		
//		texture = new CCTexture2D();
//		
//		if( texture &&
//		   texture->initWithImage(&image) )
//		{
//#if CC_ENABLE_CACHE_TEXTURE_DATA
//			// cache the texture file name
//			VolatileTexture::addImageTexture(texture, fullpath.c_str(), eImageFormat);
//#endif
//		}
//		else
//		{
//			CCLOG("cocos2d: Couldn't add image:%s in CCTextureCache", path);
//		}
//	} while (0);
//	
//	//pthread_mutex_unlock(m_pDictLock);
//	return texture;
}

void StageImgLoader::addImageAsync(const char *path, cocos2d::CCObject *target, SEL_CallFuncO selector)
{
	CCTextureCache::sharedTextureCache()->addImageAsync(path, target, selector, true, getDocumentPath());
}

size_t StageImgLoader::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size*nmemb;
	LMemoryStruct *out=(LMemoryStruct *)userp;
    if(out && !out->stream) {
        /* open file for writing */
        out->stream = fopen(out->filename.c_str(), "wb");
        if(!out->stream)
            return -1; /* failure, can't open file to write */
    }
	out->size += realsize;
    return fwrite(contents, size, nmemb, out->stream);
}

string StageImgLoader::getDocumentPath()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return CCFileUtils::sharedFileUtils()->getWritablePath();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    std::string strRet = [documentsDirectory UTF8String];
    strRet.append("/");
    return strRet;
#endif
}

LMemoryStruct lchunk;
void* StageImgLoader::t_function(void *data)
{
	string document_path = StageImgLoader::sharedInstance()->writeable_path;
	lchunk.size = 0;
	lchunk.filename = (document_path+StageImgLoader::sharedInstance()->down_filename).c_str();
	lchunk.stream = NULL;
	
	do{
		CURL *curl_handle;
		CURLcode retCode = curl_global_init(CURL_GLOBAL_ALL); // _ALL -> _DEFAULT
		if(retCode != CURLE_OK)
		{
			StageImgLoader::sharedInstance()->isFail = true;
			break;
		}
		curl_handle = curl_easy_init();
		curl_easy_setopt(curl_handle, CURLOPT_URL, StageImgLoader::sharedInstance()->p_url.c_str());
		curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, true);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&lchunk);
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
//		curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
		
		if(curl_easy_perform(curl_handle) != CURLE_OK)
		{
			StageImgLoader::sharedInstance()->isFail = true;
			break;
		}
		curl_easy_cleanup(curl_handle);
		
		if(lchunk.stream)
			fclose(lchunk.stream);
		
		curl_global_cleanup();
		
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(StageImgLoader::successAction), StageImgLoader::sharedInstance(), 0, 0, 0, false);
		
		while (lchunk.size!=0) {
			usleep(10000);
		}
	}while (0);
	
	if(StageImgLoader::sharedInstance()->isFail)
	{
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(StageImgLoader::failAction), StageImgLoader::sharedInstance(), 0, 0, 0, false);
	}
	
	return NULL;
}

float StageImgLoader::getDownloadPercentage()
{
	if(lchunk.size != 0)
		return 1.f*lchunk.size/total_size;
	else
		return 0;
}

void StageImgLoader::successAction()
{
	unschedule(schedule_selector(StageImgLoader::successAction));
	
	lchunk.size = 0;
	
	if(target_success)
	{
//		my_savedata->setKeyValue(kSDF_downloadedInfo, down_filename, 1);
		(target_success->*delegate_success)();
	}
}

void StageImgLoader::failAction()
{
	unschedule(schedule_selector(StageImgLoader::failAction));
	lchunk.size = 0;
	
	if(target_fail)
		(target_fail->*delegate_fail)();
}