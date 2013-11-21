// StartMapLuckyItem.h
//

#ifndef LZZ_StartMapLuckyItem_h
#define LZZ_StartMapLuckyItem_h
#include "cocos2d.h"
#include "GameData.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
#define LZZ_INLINE inline
enum StartMapLuckyItemZorder
{
	kStartMapLuckyItem_Z_img = 1,
	kStartMapLuckyItem_Z_particle,
	kStartMapLuckyItem_Z_fx
};
class StartMapLuckyItem : public CCNode
{
public:
	static StartMapLuckyItem * create (IntPoint t_point);
	void checkInnerRect ();
private:
	IntPoint my_point;
	CCSprite * main_img;
	void startMyAction ();
	void createParticle ();
	void speedUpAction ();
	void speedUp ();
	void startRemoveAction ();
	void myInit (IntPoint t_point);
};
#undef LZZ_INLINE
#endif
