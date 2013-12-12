// SearchEye.h
//

#ifndef LZZ_SearchEye_h
#define LZZ_SearchEye_h

#include "cocos2d.h"
#include "GameData.h"
#include "DataStorageHub.h"

USING_NS_CC;
#define LZZ_INLINE inline
class SearchEye : public CCNode
{
public:
	static SearchEye * create ();
	void startSearch ();
private:
	CCSprite* search_eye_arrow;
	CCSprite* search_eye_boss;
	CCSize frame_size;
	int ing_frame;
	void mainCumberSearching ();
	void myInit ();
};
#undef LZZ_INLINE
#endif
