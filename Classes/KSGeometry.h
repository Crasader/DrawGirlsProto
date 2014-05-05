// hsnks100@gmail.com 한경수 

#ifndef _KSGeometry_h
#define _KSGeometry_h

#include "cocos2d.h"

inline cocos2d::CCPoint operator-(const cocos2d::CCPoint& t)
{
	return ccp(-t.x, -t.y);
}


inline cocos2d::CCRect rtSetScale(const cocos2d::CCRect& t, float sc, cocos2d::CCPoint ap = ccp(0.5f, 0.5f))
{
	cocos2d::CCRect ret = t;
	cocos2d::CCPoint move = ccp(t.size.width * ap.x - t.size.width * sc * ap.x,
					   t.size.height * ap.y - t.size.height * sc * ap.y);
	ret.origin = ret.origin + move;
	ret.size = ret.size * sc;
	return ret;
}

#endif
