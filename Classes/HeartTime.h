// HeartTime.h
//

#ifndef LZZ_HeartTime_h
#define LZZ_HeartTime_h
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define HEART_DISTANCE 11.5f
#define LZZ_INLINE inline
class HeartTime : public CCNode
{
public:
	static HeartTime * create ();
	bool startGame ();
	void backHeart ();
	bool isStartable ();
	void onLight();
	void refreshHeartTime();
	
	virtual ~HeartTime();
private:
	bool is_on_light;
	vector <CCSprite*> heart_list;
	CCLabelBMFont * state_label;
	bool is_checking;
	void checkingTime ();
	void myInit ();
};
#undef LZZ_INLINE
#endif
