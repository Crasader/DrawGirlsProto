#ifndef SportsWorldCupOriginal_SceneUtil_h
#define SportsWorldCupOriginal_SceneUtil_h

#include "cocos2d.h"
USING_NS_CC;
class AniUtil
{
public:
	static std::pair<CCActionInterval*, CCSprite*> playAnimation(CCSpriteBatchNode* batchNode, float delay, int cols, int frames, int width_per_each, int height_per_each, bool isInfinite);
	static std::pair<CCActionInterval*, CCSprite*> playAnimation(const char* spriteName, float delay, int cols, int frames, int width_per_each, int height_per_each, bool isInfinite);
	static std::pair<CCActionInterval*, CCSprite*> playAnimation(const char* spriteName, float delay, int cols, int frames, int width_per_each, int height_per_each, int repeat);
};

#endif
