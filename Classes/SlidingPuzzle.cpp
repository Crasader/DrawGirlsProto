#include "SlidingPuzzle.h"
#include "PuzzleCache.h"
#include "KSUtil.h"
#include "StarGoldData.h"

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

bool SlidingPuzzle::init(int priority, const std::function<void(CCObject*, SEL_CallFunc)>& hideFunction)
{
	CCLayer::init();
	
	CCDrawNode* shape = CCDrawNode::create();
	
	CCPoint pts[4];
	pts[0] = ccp(25, 320 - 297);
	pts[1] = ccp(366, 320 - 297);
	pts[2] = ccp(366, 320 - 24);
	pts[3] = ccp(25, 320 - 24);
	shape->drawPolygon(pts, 4, ccc4f(1, 1, 1, 1), 0, ccc4f(1, 0, 0, 1));
	
	CCSprite* back = CCSprite::create("bonusgame_back.png");
	back->setPosition(ccp(240, 160));
	addChild(back);
	// add shape in stencil
	m_thiz = CCClippingNode::create();
	m_thiz->setAnchorPoint(ccp(0.5, 0.5));
	m_thiz->setStencil(shape);
	this->addChild(m_thiz);
	
	// setup content
	m_hideFunction = hideFunction;
	m_priority = priority;
	setTouchEnabled(true);
	std::random_device rd;
	m_rEngine.seed(rd());
	CCNode* containerNode = CCNode::create();
	m_menu = CCMenuLambda::create();
	m_menu->setPosition(ccp(0, 0));
	m_menu->setPropaOnBegan(true);
	m_menu->setTouchPriority(m_priority);
	m_menu->setTouchEnabled(false);
	containerNode->addChild(m_menu);
	m_thiz->addChild(containerNode);
	
	
	m_timeFnt = CCLabelBMFont::create("", "allfont.fnt");
	m_timeFnt->setPosition(ccp(420, 270));
	m_timeFnt->setString(CCString::createWithFormat("%.1f", m_remainTime)->getCString());
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
//		PuzzleImage* pi = PuzzleCache::getInstance()->cutImageByStensil(img, st, {cutx, cuty}, true);
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
		CCMenuItemSpriteLambda* item = CCMenuItemSpriteLambda::create
		(pps, pps2,
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
			 /*
				m_emptyCoord 와 클릭한 좌표는 수평선 상에 있어야 함.
				m_emptyCoord 에서  coord 으로 방향으로 몇 번 가야 도착하는지 알아내야 하고,
				n 번 가야 된다면, n - 1 번 째는 그냥 이동 시키고
				마지막 함수는 검사함.
				
				*/
			 if(m_emptyCoord.x == coord.x ^ m_emptyCoord.y == coord.y)
			 {
				 if(m_state == SlidingState::kNormal)
				 {
					 m_state = SlidingState::kMovingInPlay;
					 Coord direction = coord + -m_emptyCoord;
					 direction.y /= direction.y ? abs(direction.y) : 1;
					 direction.x /= direction.x ? abs(direction.x) : 1;
					 int moveCount = 0;
					 for(Coord emptyCoord = m_emptyCoord; !(emptyCoord == coord); emptyCoord = emptyCoord + direction)
					 {
						 moveCount++;
					 }
					 int i = 0;
					 for(Coord emptyCoord = m_emptyCoord; i < moveCount ; emptyCoord = emptyCoord + direction)
					 {
						 if(i == moveCount - 1) // 마지막 조건
						 {
							 this->movePiece
							 (emptyCoord + direction, 0.2f,
								[=](Coord prevEmptyCoord) // 다 움직였을 때 올바른가 검사.
								{
									m_state = SlidingState::kNormal;
									CCLOG("com");
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
										
										CCSprite* successSprite = CCSprite::create("bonusgame_succes.png");
										successSprite->setPosition(ccp(240, 160));
										addChild(successSprite);
										CCSprite* reward = CCSprite::create("price_ruby_img.png");
										reward->setPosition(ccp(205, 133));
										addChild(reward);
										reward->setScale(2.f);
										CCLOG("correct!!");
										m_menu->setTouchEnabled(false);
										unscheduleUpdate();
//										mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) + 1);
//										myDSH->saveUserData({kSaveUserData_Key_star}, [=](Json::Value v)
//																				{
																					addChild(KSTimer::create(3.f, [=](){
																						m_hideFunction(this, callfunc_selector(ThisClassType::removeFromParent));
																					}));
//																				});
									}
								});
						 }
						 else
						 {
							 this->movePiece(emptyCoord + direction, 0.2f, [=](Coord ){});
						 }
//						 moveCount++;
						 i++;
					 }
				 }
			 }
//			 CCLOG("%f ", coord.length(m_emptyCoord));
//			 if(0.9f <= coord.length(m_emptyCoord) &&
//					coord.length(m_emptyCoord) <= 1.1f)
//			 {
//				 if(m_state == SlidingState::kNormal)
//				 {
//					 m_state = SlidingState::kMovingInPlay;
//					 this->movePiece
//					 (coord, 0.2f,
//						[=](Coord prevEmptyCoord) // 다 움직였을 때 올바른가 검사.
//						{
//							m_state = SlidingState::kNormal;
//							CCLOG("com");
//							Coord coord(-1, -1);
//							
//							bool valid = true;
//							for(int y=0; y<m_pieces.size(); y++)
//							{
//								for(int x=0; x<m_pieces[y].size(); x++)
//								{
//									CCMenuItemLambda* menuItem = m_pieces[y][x];
//									if(!(Coord(x, y) == Coord((int)menuItem->getUserData()%10, (int)menuItem->getUserData()/10)))
//									{
//										valid = false;
//										break;
//									}
//								}
//							}
//							if(valid)
//							{
//								
//								CCSprite* successSprite = CCSprite::create("bonusgame_succes.png");
//								successSprite->setPosition(ccp(240, 160));
//								addChild(successSprite);
//								CCLOG("correct!!");
//								m_menu->setTouchEnabled(false);
//								unscheduleUpdate();
//								mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) + 1);
//								myDSH->saveUserData({kSaveUserData_Key_star}, [=](Json::Value v)
//																		{
//																			addChild(KSTimer::create(3.f, [=](){
//																				m_hideFunction();
//																			}));
//																		});
//							}
//						});
//				 }
//			 }
			 
			 
			 
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
		containerNode->addChild(spr,1000);
		
		
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
		containerNode->addChild(spr,1000);
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
		containerNode->addChild(spr,1000);
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
		containerNode->addChild(spr,1000);
		
		//메모리해제
		st->release();
	}
#endif
	//메모리해제
	img->release();
	
	containerNode->setPosition(ccp(30, 25));
	m_pieces[0][PUZZLE_WIDTH - 1]->setVisible(false);
	m_emptyCoord.y = 0;
	m_emptyCoord.x = PUZZLE_WIDTH - 1;
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
	shufflePieces(15, movingPiece);
//	movePiece(Coord(3, 0), [=]()
//						{
//							CCLOG("%d %d", m_emptyCoord.x, m_emptyCoord.y);
//							movePiece(Coord(4, 0), [=]()
//												{
//													CCLOG("%d %d", m_emptyCoord.x, m_emptyCoord.y);
//												});
//						});
	
	return true;
}

void SlidingPuzzle::startSchedule()
{
	
	schedule(schedule_selector(SlidingPuzzle::shuffle));
	
}
void SlidingPuzzle::shuffle(float dt)
{
	
}

void SlidingPuzzle::update(float dt)
{
	m_timer += dt;
	if(m_remainTime - m_timer <= 0.f)
	{
		CCSprite* failSprite = CCSprite::create("bonusgame_fail.png");
		failSprite->setPosition(ccp(240, 160));
		addChild(failSprite);
		m_menu->setTouchEnabled(false);
		addChild(KSTimer::create(3.f, [=]()
														 {
															 m_hideFunction(this, callfunc_selector(ThisClassType::removeFromParent));
														 }));
		unscheduleUpdate();
	}
	
	m_timeFnt->setString(CCString::createWithFormat("%.1f", m_remainTime - m_timer)->getCString());
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
		m_pieces[emptyCoord.y][emptyCoord.x]->setPosition(a);
		std::swap(m_pieces[piece.y][piece.x], m_pieces[emptyCoord.y][emptyCoord.x]);
		addChild(KSGradualValue<CCPoint>::create(a,
																						 b, duration,
																						 [=](CCPoint t)
																						 {
																							 m_pieces[emptyCoord.y][emptyCoord.x]->setPosition(t);
																						 },
																						 [=](CCPoint t)
																						 {
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
									// 섞기가 끝났다.
									m_state = SlidingState::kNormal;
									m_menu->setTouchEnabled(true);
									scheduleUpdate();
									
									auto ready_go = KS::loadCCBI<CCSprite*>(this, "ui_ready.ccbi");
	addChild(ready_go.first);
									ready_go.first->setPosition(ccp(240, 160));
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

