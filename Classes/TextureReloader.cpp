//
//  TextureReloader.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 30..
//
//

#include "TextureReloader.h"

AliveNode* AliveNode::create(function<void()> t_reload_func)
{
	AliveNode* t_an = new AliveNode();
	t_an->myInit(t_reload_func);
	t_an->autorelease();
	return t_an;
}

void AliveNode::myInit(function<void()> t_reload_func)
{
	reload_func = t_reload_func;
}

AliveNode::~AliveNode()
{
	myTR->removeAliveNode(this);
}

void AliveNode::reloadAction()
{
	reload_func();
}

void TextureReloader::reloadTexture()
{
	for(auto iter = alive_list.begin();iter != alive_list.end();iter++)
	{
		(*iter)->reloadAction();
	}
}

void TextureReloader::addAliveNode(CCNode *t_node, function<void()> t_reload_func)
{
	AliveNode* t_an = AliveNode::create(t_reload_func);
	alive_list.push_back(t_an);
	t_node->addChild(t_an);
}

void TextureReloader::removeAliveNode(AliveNode *t_an)
{
	for(auto iter = alive_list.begin();iter != alive_list.end();iter++)
	{
		if(t_an == *iter)
		{
			alive_list.erase(iter);
			return;
		}
	}
}

TextureReloader* TextureReloader::sharedTextureReloader()
{
	static TextureReloader* t_TR = NULL;
	if(t_TR == NULL)
	{
		t_TR = new TextureReloader();
		t_TR->myInit();
	}
	return t_TR;
}

void TextureReloader::myInit()
{
	alive_list.clear();
}