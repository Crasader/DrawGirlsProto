//
//  AsyncImageManager.h
//  DGproto
//
//  Created by 사원3 on 2014. 10. 12..
//
//

#ifndef __DGproto__AsyncImageManager__
#define __DGproto__AsyncImageManager__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define myAIM AsyncImageManager::sharedInstance()

class AsyncImage;

class AsyncImageInfo : public CCObject
{
public:
	string load_filename;
	vector<AsyncImage*> m_ai_list;
	bool is_loading;
	
	AsyncImageInfo() : load_filename(""),
	is_loading(false)
	{
		m_ai_list.clear();
	}
    
    virtual ~AsyncImageInfo();
};

class AsyncImageManager : public CCObject
{
public:
	static AsyncImageManager* sharedInstance();
	
	void regiAsyncImage(AsyncImage* t_ai);
	void unregiAsyncImage(AsyncImage* t_ai);
	
	void asyncedImage(CCObject* t_obj);
	
	int loaded_cnt;
	
private:
	vector<AsyncImageInfo*> ready_ai_list;
	
	void myInit()
	{
		loaded_cnt = 0;
		ready_ai_list.clear();
	}
};

#endif /* defined(__DGproto__AsyncImageManager__) */
