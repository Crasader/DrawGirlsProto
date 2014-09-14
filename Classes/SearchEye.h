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
	static SearchEye * create (CCNode* t_boss, vector<SearchEye*>* sv);
	virtual ~SearchEye()
	{
		CCLOG("~SEARCH EYE");
	}
	void startSearch ();
private:
	CCNode* target_boss;
	CCSprite* search_eye_arrow;
	CCSprite* search_eye_boss;
	CCSize frame_size;
	int ing_frame;
	void mainCumberSearching ();
	vector<SearchEye*>* search_eye_vector;
	void myInit (CCNode* t_boss, vector<SearchEye*>* sv);
};
#undef LZZ_INLINE
#endif
