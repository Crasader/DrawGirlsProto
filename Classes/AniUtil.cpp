#include "AniUtil.h"

std::pair<CCActionInterval*, CCSprite*> AniUtil::playAnimation( CCSpriteBatchNode* batchNode, float delay, int cols, int frames, int width_per_each, int height_per_each, bool isInfinite )
{
	CCSprite* Sprite = CCSprite::createWithTexture(batchNode->getTexture(), CCRectMake(0, 0, width_per_each, height_per_each));

	CCAnimation* Animation = CCAnimation::create();
	Animation->setDelayPerUnit(delay);

	for(int f = 1; f<=frames; f++) // 프레임 개수만큼.
	{
		int col = (f - 1 ) % cols;
		int row = (f - 1 ) / cols;
		Animation->addSpriteFrameWithTexture(batchNode->getTexture(), CCRectMake(width_per_each*col, height_per_each * row, width_per_each, height_per_each));
	}

	CCActionInterval* ac;
	if(isInfinite)
	{
		ac = CCRepeatForever::create(CCAnimate::create(Animation));
	}
	else
	{
		ac = CCAnimate::create(Animation);
	}

	batchNode->addChild(Sprite);
	return std::pair<CCActionInterval*, CCSprite*>(ac, Sprite);
}

std::pair<CCActionInterval*, CCSprite*> AniUtil::playAnimation( const char* spriteName, float delay, int cols, int frames, int width_per_each, int height_per_each, bool isInfinite )
{
	CCAnimation* Animation = CCAnimation::create();
	CCSprite* sprite = CCSprite::create(spriteName);
	Animation->setDelayPerUnit(delay);
	for(int f = 1; f<=frames; f++) // 프레임 개수만큼.
	{
		int col = (f - 1 ) % cols;
		int row = (f - 1 ) / cols;
		Animation->addSpriteFrameWithTexture(sprite->getTexture(), CCRectMake(width_per_each*col, height_per_each * row, width_per_each, height_per_each));

	}

	CCActionInterval* ac;
	if(isInfinite)
	{
		ac = CCRepeatForever::create(CCAnimate::create(Animation));
	}
	else
	{
		ac = CCAnimate::create(Animation);
	}

	CCSprite* ret = CCSprite::createWithTexture(sprite->getTexture(), CCRectMake(0, 0, width_per_each, height_per_each));
	return std::pair<CCActionInterval*, CCSprite*>(ac, ret);
}

std::pair<CCActionInterval*, CCSprite*> AniUtil::playAnimation( const char* spriteName, float delay, int cols, int frames, int width_per_each, int height_per_each, int repeat )
{
	CCAnimation* Animation = CCAnimation::create();
	CCSprite* sprite = CCSprite::create(spriteName);
	Animation->setDelayPerUnit(delay);
	for(int f = 1; f<=frames; f++) // 프레임 개수만큼.
	{
		int col = (f - 1 ) % cols;
		int row = (f - 1 ) / cols;
		Animation->addSpriteFrameWithTexture(sprite->getTexture(), CCRectMake(width_per_each*col, height_per_each * row, width_per_each, height_per_each));

	}

	CCActionInterval* ac = CCRepeat::create(CCAnimate::create(Animation), repeat);

	CCSprite* ret = CCSprite::createWithTexture(sprite->getTexture(), CCRectMake(0, 0, width_per_each, height_per_each));
	return std::pair<CCActionInterval*, CCSprite*>(ac, ret);
}
