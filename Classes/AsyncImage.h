//
//  AsyncImage.h
//  DGproto
//
//  Created by 사원3 on 2014. 10. 12..
//
//

#ifndef __DGproto__AsyncImage__
#define __DGproto__AsyncImage__

#include "cocos2d.h"
#include "GraySprite.h"

USING_NS_CC;
using namespace std;

class AsyncImage : public GraySprite
{
public:
	static AsyncImage* create(string t_filename, string t_before_filename);
	
	AsyncImage() : is_loaded(false),
	is_loading(false)
	{}
	virtual ~AsyncImage();
	
	bool is_loading;
	string m_filename;
	
	void loadedImage(CCTexture2D* t_texture, bool is_removeUnusedTexture = false);
	
private:
	bool is_loaded;
	
	void myInit(string t_filename)
	{
		m_filename = t_filename;
	}
};

#endif /* defined(__DGproto__AsyncImage__) */
