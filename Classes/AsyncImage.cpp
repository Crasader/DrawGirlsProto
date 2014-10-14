//
//  AsyncImage.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 10. 12..
//
//

#include "AsyncImage.h"
#include "AsyncImageManager.h"
#include "KSUtil.h"

AsyncImage::~AsyncImage()
{
	if(!is_loaded)
	{
		myAIM->unregiAsyncImage(this);
	}
}

AsyncImage* AsyncImage::create(string t_filename, string t_before_filename)
{
	AsyncImage* t_ai = new AsyncImage();
	t_ai->myInit(t_filename);
	CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->textureForKey(t_filename.c_str());
	if(t_texture)
	{
		t_ai->is_loaded = true;
		t_ai->initWithTexture(t_texture);
	}
	else
	{
		t_ai->initWithFile(t_before_filename.c_str());
		myAIM->regiAsyncImage(t_ai);
	}
	t_ai->autorelease();
	
	return t_ai;
}

void AsyncImage::loadedImage(CCTexture2D* t_texture, bool is_removeUnusedTexture)
{
	is_loaded = true;
    CCLOG("android setTexture test1 : %d , texture : %d", is_removeUnusedTexture, t_texture);
	setTexture(t_texture);
	CCLOG("android setTexture test2 : %d", is_removeUnusedTexture);
    if(is_removeUnusedTexture)
    {
        addChild(KSTimer::create(0.05f, [=]()
                                 {
                                     CCTextureCache::sharedTextureCache()->removeUnusedTextures();
                                 }));
    }
}