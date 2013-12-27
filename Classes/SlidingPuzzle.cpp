#include "SlidingPuzzle.h"
#include "PuzzleCache.h"
#include "KSUtil.h"

SlidingPuzzle::~SlidingPuzzle()
{
}
//void SlidingPuzzle::registerWithTouchDispatcher()
//{
//	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
//}

//bool SlidingPuzzle::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool SlidingPuzzle::init()
{
	CCLayer::init();
	setTouchEnabled(true);
	std::random_device rd;
	m_rEngine.seed(rd());
	m_menu = CCMenuLambda::create();
	m_menu->setPosition(ccp(0, 0));
	m_menu->setPropaOnBegan(true);
	
	addChild(m_menu);
	
	
	m_timeFnt = CCLabelBMFont::create("", "etc_font.fnt");
	m_timeFnt->setPosition(ccp(390, 100));
	addChild(m_timeFnt);
	
	//퍼즐 파일명
	string filename=CCFileUtils::sharedFileUtils()->getWritablePath() + "puzzle1_original.png";
	
	//퍼즐이미지를 불러옵니다.
	CCImage *img = new CCImage;
	img->initWithImageFileThreadSafe(filename.c_str());
	
	//피스조각(가로형)을 불러옵니다.
	CCImage *st_w = new CCImage;
	st_w->initWithImageFile("puzzle_stencil_2_pw.png");
	
	//피스조각(세로형)을 불러옵니다.
	CCImage *st_h = new CCImage;
	st_h->initWithImageFile("puzzle_stencil_2_ph.png");
	
	
	//지금부터 퍼즐 조각 잘라 저장하기를 시작하겠습니다.
	
	for(int i=0;i<20;i++){
		//피스는 i가 짝수일때 st_w 이미지를 이용하여 자르고 홀수일때 st_h 이미지를 이용하여 자릅니다.
		CCImage *st = st_h;
		if(i%2==0)st=st_w;
		
		//피스의 좌표를 구합니다. 퍼즐은 5*4 개로 이루어져있습니다.
		int x = i%PUZZLE_WIDTH;
		int y = i/PUZZLE_WIDTH;
		
		//원본파일에서 자를 위치를 계산합니다.
		int cutx =x*116+94;
		int cuty =y*116+94;
		
		/* face cut
		 cutx = x*172+86;
		 cuty = y*172+86;
		 */
		
		//자르고 저장합니다.
		PuzzleCache::getInstance()->changeStencilByOrigin(st, img, {cutx,cuty}, true);
		
		//테스트로 한번 붙여봅니다.
		CCTexture2D* _texture = new CCTexture2D;
		_texture->initWithImage(st);
		CCSprite* pps = CCSprite::createWithTexture(_texture);
		_texture->release();
		
		_texture = new CCTexture2D;
		_texture->initWithImage(st);
		CCSprite* pps2 = CCSprite::createWithTexture(_texture);
	
		_texture->release();
		pps2->setColor(ccc3(166, 166, 166));
		CCMenuItemSpriteLambda* item = CCMenuItemSpriteLambda::create(pps, pps2,
																																	[=](CCObject* s)
																																	{
																																		Coord coord(-1, -1);
																																		for(int y=0; y<m_pieces.size(); y++)
																																		{
																																			for(int x=0; x<m_pieces[y].size(); x++)
																																			{
																																				if(m_pieces[y][x] == s)
																																				{
																																					coord.y = y;
																																					coord.x = x;
																																					break;
																																				}
																																			}
																																		}
																																		CCLog("%f ", coord.length(m_emptyCoord));
																																		if(0.9f <= coord.length(m_emptyCoord) &&
																																			 coord.length(m_emptyCoord) <= 1.1f)
																																		{
																																			if(m_state == SlidingState::kNormal)
																																			{
																																				m_state = SlidingState::kMovingInPlay;
																																				this->movePiece
																																				(coord, 0.2f,
																																				 [=](Coord prevEmptyCoord) // 다 움직였을 때 올바른가 검사.
																																				 {
																																					 m_state = SlidingState::kNormal;
																																					 CCLog("com");
																																					 Coord coord(-1, -1);
																																					 
																																					 bool valid = true;
																																					 for(int y=0; y<m_pieces.size(); y++)
																																					 {
																																						 for(int x=0; x<m_pieces[y].size(); x++)
																																						 {
																																							 CCMenuItemLambda* menuItem = m_pieces[y][x];
																																							 if(!(Coord(x, y) == Coord((int)menuItem->getUserData()%10, (int)menuItem->getUserData()/10)))
																																							 {
																																								 valid = false;
																																								 break;
																																							 }
																																						 }
																																					 }
																																					 if(valid)
																																					 {
																																						 CCLog("correct!!");
																																						 this->unscheduleUpdate();
																																						 m_timeFnt->setColor(ccc3(255, 0, 0));
																																						 addChild(KSTimer::create(3.f, [=]()
																																																			{
																																																				CCDirector::sharedDirector()->popScene();
																																																			}));
																																					 }
																																				 });
																																			}
																																		}
																																		
																																		
																																		
																																	});
		item->setUserData((void*)(y*10 + x));
		m_menu->addChild(item);
		
		m_pieces[y][x] = item;
		
		item->setPosition(ccp(cutx/2,cuty/2));
//		addChild(pps);
	}
	//가로세로 피스 메모리해제
	st_w->release();
	st_h->release();
	
	//이제 프레임들 불러와서 붙여보겠음다
	
#if 1
	//위쪽부터 잘라봅니다.
	{
		CCImage *st = new CCImage;
		st->initWithImageFile("puzzle_stencil_2_top.png");
		
		int cutx =652/2;
		int cuty =536-68/2;
		
		//자르고 저장합니다.
		PuzzleCache::getInstance()->changeStencilByOrigin(st, img, {cutx,cuty}, true);
		
		
		//테스트로 한번 붙여봅니다.
		CCTexture2D* _texture = new CCTexture2D;
		_texture->initWithImage(st);
		CCSprite *spr =  CCSprite::createWithTexture(_texture);
		_texture->release();
		spr->setAnchorPoint(ccp(0.5,0.5));
		spr->setPosition(ccp(cutx/2,cuty/2));
		addChild(spr,1000);
		
		
		//메모리해제
		st->release();
	}
	
	//아래쪽 잘라봅니다.
	{
		CCImage *st = new CCImage;
		st->initWithImageFile("puzzle_stencil_2_bottom.png");
		
		
		int cutx =652/2;
		int cuty =68/2;
		
		//자르고 저장합니다.
		PuzzleCache::getInstance()->changeStencilByOrigin(st, img, {cutx,cuty}, true);

		//테스트로 한번 붙여봅니다.
		CCTexture2D* _texture = new CCTexture2D;
		_texture->initWithImage(st);
		CCSprite *spr =  CCSprite::createWithTexture(_texture);
		_texture->release();
		spr->setAnchorPoint(ccp(0.5,0.5));
		spr->setPosition(ccp(cutx/2,cuty/2));
		addChild(spr,1000);
		//메모리해제
		st->release();
	}
	
	//왼쪽 잘라봅니다.
	{
		CCImage *st = new CCImage;
		st->initWithImageFile("puzzle_stencil_2_left.png");
		
		
		int cutx =68/2;
		int cuty =536/2;
		
		//자르고 저장합니다.
		PuzzleCache::getInstance()->changeStencilByOrigin(st, img, {cutx,cuty}, true);
		
		
		
		//테스트로 한번 붙여봅니다.
		CCTexture2D* _texture = new CCTexture2D;
		_texture->initWithImage(st);
		CCSprite *spr =  CCSprite::createWithTexture(_texture);
		_texture->release();
		spr->setAnchorPoint(ccp(0.5,0.5));
		spr->setPosition(ccp(cutx/2,cuty/2));
		addChild(spr,1000);
		//메모리해제
		st->release();
	}
	//오른쪽 잘라봅니다.
	{
		CCImage *st = new CCImage;
		st->initWithImageFile("puzzle_stencil_2_right.png");
		
		
		int cutx =652-68/2;
		int cuty =536/2;
		
		//자르고 저장합니다.
		PuzzleCache::getInstance()->changeStencilByOrigin(st, img, {cutx,cuty}, true);
		
		
		//테스트로 한번 붙여봅니다.
		CCTexture2D* _texture = new CCTexture2D;
		_texture->initWithImage(st);
		CCSprite *spr =  CCSprite::createWithTexture(_texture);
		_texture->release();
		spr->setAnchorPoint(ccp(0.5,0.5));
		spr->setPosition(ccp(cutx/2,cuty/2));
		addChild(spr,1000);
		
		//메모리해제
		st->release();
	}
#endif
	//메모리해제
	img->release();
	
	m_pieces[0][PUZZLE_WIDTH - 1]->setVisible(false);
	m_emptyCoord.y = 0;
	m_emptyCoord.x = PUZZLE_WIDTH - 1;
	schedule(schedule_selector(SlidingPuzzle::shuffle));
	m_state = SlidingState::kShuffleReady;
	
	Coord movingPiece;
	std::uniform_int_distribution<> dist(0, 3);
	while(1)
	{
		int r = dist(m_rEngine);
		switch(r)
		{
			case 0:
				movingPiece = m_emptyCoord + Coord(0, 1);
				break;
			case 1:
				movingPiece = m_emptyCoord + Coord(-1, 0);
				break;
			case 2:
				movingPiece = m_emptyCoord + Coord(0, -1);
				break;
			case 3:
				movingPiece = m_emptyCoord + Coord(1, 0);
				break;
		}
		if(isValidCoord(movingPiece))
			break;
	}
	
	m_state = SlidingState::kMovingInShuffling;
	shufflePieces(140, movingPiece);
//	movePiece(Coord(3, 0), [=]()
//						{
//							CCLog("%d %d", m_emptyCoord.x, m_emptyCoord.y);
//							movePiece(Coord(4, 0), [=]()
//												{
//													CCLog("%d %d", m_emptyCoord.x, m_emptyCoord.y);
//												});
//						});
	scheduleUpdate();
	return true;
}

void SlidingPuzzle::shuffle(float dt)
{
	
}

void SlidingPuzzle::update(float dt)
{
	m_timer += dt;
	
	m_timeFnt->setString(CCString::createWithFormat("%.1f", m_timer)->getCString());
}

void SlidingPuzzle::movePiece(Coord piece, float duration, const std::function<void(Coord)>& noti )
{
	Coord emptyCoord = m_emptyCoord;
	CCPoint a = m_pieces[piece.y][piece.x]->getPosition();
	CCPoint b = m_pieces[m_emptyCoord.y][m_emptyCoord.x]->getPosition();
	m_emptyCoord = piece;
	if(duration < 1/60.f)
	{
		m_pieces[piece.y][piece.x]->setPosition(b);
		m_pieces[emptyCoord.y][emptyCoord.x]->setPosition(a);
		std::swap(m_pieces[piece.y][piece.x], m_pieces[emptyCoord.y][emptyCoord.x]);
		noti(emptyCoord);
	}
	else
	{
		addChild(KSGradualValue<CCPoint>::create(a,
																						 b, duration,
																						 [=](CCPoint t)
																						 {
																							 m_pieces[piece.y][piece.x]->setPosition(t);
																						 },
																						 [=](CCPoint t)
																						 {
																							 m_pieces[emptyCoord.y][emptyCoord.x]->setPosition(a);
																							 std::swap(m_pieces[piece.y][piece.x], m_pieces[emptyCoord.y][emptyCoord.x]);
																							 noti(emptyCoord);
																						 }));
	}
}

void SlidingPuzzle::shufflePieces(int loop, Coord coord)
{
	if(loop != 0)
	{
		movePiece(coord, 1/60.f, [=](Coord prevEmptyCoord)
							{
								Coord movingPiece;
								std::uniform_int_distribution<> dist(0, 3);
								while(1)
								{
									int r = dist(m_rEngine);
									std::map<int, Coord> adder;
									adder[0] = Coord(0, 1);
									adder[1] = Coord(-1, 0);
									adder[2] = Coord(0, -1);
									adder[3] = Coord(1, 0);
									movingPiece = m_emptyCoord + adder[r];
									
									
									if(!isValidCoord(movingPiece) || prevEmptyCoord == movingPiece)
										continue;
									else
									{
										break;
									}
								}
								
								shufflePieces(loop - 1, movingPiece);
							});
	}
	else
	{
		movePiece(coord, 1/60.f, [=](Coord prevEmptyCoord)
							{
								if(m_emptyCoord == Coord(PUZZLE_WIDTH - 1, 0))
								{
									m_state = SlidingState::kNormal;
									return;
								}
								Coord movingPiece;
								std::uniform_int_distribution<> dist(2, 3);
								
								while(1)
								{
									int r = dist(m_rEngine);
									std::map<int, Coord> adder;
									adder[0] = Coord(0, 1);
									adder[1] = Coord(-1, 0);
									adder[2] = Coord(0, -1);
									adder[3] = Coord(1, 0);
									movingPiece = m_emptyCoord + adder[r];
									
									
									if(!isValidCoord(movingPiece))
										continue;
									else
									{
										break;
									}
								}
								
								
								shufflePieces(0, movingPiece);
								
							});
	}
};

