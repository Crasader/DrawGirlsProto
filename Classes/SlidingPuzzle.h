#ifndef __SlidingPuzzle__
#define __SlidingPuzzle__

#include "cocos2d.h"
USING_NS_CC;
#include "DataStorageHub.h"
#include <random>
#include "CCMenuLambda.h"
enum class SlidingState
{
	kShuffleReady, // 셔플 준비.
	kMovingInShuffling, // 셔플의 움직임.
	kNormal, // 플레이 상태.
	kMovingInPlay // 플레이 중 움직임.
};
struct Coord
{
	int x, y;
	Coord(int _x, int _y) : x(_x), y(_y){}
	Coord(){}
	void swap(Coord& coord)
	{
		Coord temp = coord;
		coord = *this;
		x = temp.x;
		y = temp.y;
	}
	Coord operator+(const Coord& c) const
	{
		return Coord(x + c.x, y + c.y);
	}
	Coord operator-() const
	{
		return Coord(-x, -y);
	}
	bool operator==(const Coord& c) const
	{
		return x == c.x && y == c.y;
	}
	float length(const Coord& c) const
	{
		return sqrt((x-c.x)*(x-c.x) + (y-c.y)*(y-c.y));
	}
};

class PuzzlePieceSprite : public CCSprite
{
public:
	Coord m_originalCoord;
	
};
class SlidingPuzzle : public CCLayer
{
public:
	static CCScene* scene()
	{
		// 'scene' is an autorelease object
		CCScene *scene = CCScene::create();
		
		// 'layer' is an autorelease object
		SlidingPuzzle *layer = SlidingPuzzle::create(0, nullptr);
		layer->setAnchorPoint(ccp(0.5,0));
		layer->setScale(myDSH->screen_convert_rate);
		layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
		
		scene->addChild(layer);
		return scene;
	}

	SlidingPuzzle() : m_pieces(4, vector<CCMenuItemLambda*>(5)), // 4 by 5,
	PUZZLE_WIDTH(5),
	PUZZLE_HEIGHT(4),
	m_timer(0),
	m_remainTime(60*3)
	{
		
	}
	virtual ~SlidingPuzzle();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void startSchedule();
	virtual bool init(int priority, const std::function<void(CCObject*, SEL_CallFunc)>& hideFunction);
	static SlidingPuzzle* create(int priority, const std::function<void(CCObject*, SEL_CallFunc)>& hideFunction)
	{
    SlidingPuzzle* pRet = new SlidingPuzzle();
    if (pRet && pRet->init(priority, hideFunction))
    {
			pRet->autorelease();
			return pRet;
    }
    else
    {
			delete pRet;
			pRet = NULL;
			return NULL;
    }
	}
	void shuffle(float dt);
	void update(float dt);
	//virtual void registerWithTouchDispatcher();
	virtual void registerWithTouchDispatcher()
	{
		///		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_priority, true);
	}
	void movePiece(Coord piece, float t, const std::function<void(Coord)>& noti );
	bool isValidCoord(Coord coord){
		return 0 <= coord.x && coord.x < PUZZLE_WIDTH &&
		0 <= coord.y && coord.y < PUZZLE_HEIGHT;
	}
	void shufflePieces(int loop, Coord coord);
protected:
	float m_remainTime;
	std::function<void(CCObject*, SEL_CallFunc)> m_hideFunction;
	CCClippingNode* m_thiz;
	int m_priority;
	CCMenuLambda* m_menu;
	vector<vector<CCMenuItemLambda*> > m_pieces;
	Coord m_emptyCoord;
	const int PUZZLE_WIDTH, PUZZLE_HEIGHT;
	SlidingState m_state;
	std::mt19937 m_rEngine;
	CCLabelBMFont* m_timeFnt;
	float m_timer;
};

#endif
