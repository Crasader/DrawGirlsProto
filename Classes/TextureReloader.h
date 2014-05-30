//
//  TextureReloader.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 30..
//
//

#ifndef __DGproto__TextureReloader__
#define __DGproto__TextureReloader__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define myTR TextureReloader::sharedTextureReloader()

class AliveNode : public CCNode
{
public:
	static AliveNode* create(function<void()> t_reload_func);
	virtual ~AliveNode();
	void reloadAction();
	
private:
	function<void()> reload_func;
	void myInit(function<void()> t_reload_func);
};

class TextureReloader : public CCObject
{
public:
	static TextureReloader* sharedTextureReloader();
	void addAliveNode(CCNode* t_node, function<void()> t_reload_func);
	void removeAliveNode(AliveNode* t_an); // auto
	
	void reloadTexture();
	
private:
	vector<AliveNode*> alive_list;
	
	void myInit();
};

#endif /* defined(__DGproto__TextureReloader__) */
