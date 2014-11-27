//
//  SpecialMissileUnit.cpp
//  DGproto
//
//  Created by ksoo k on 2014. 11. 18..
//
//

#include "MissileUnit.h"

#include "StarGoldData.h"
#include "KSUtil.h"

#include "ProbSelector.h"
#include "MissileDamageData.h"

#include "KSUtil.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "KSUtil.h"
#include "GameData.h"
#include "DataStorageHub.h"
#include "Jack.h"
#include "EffectSprite.h"
#include "EasingAction.h"
#include "KSCumberBase.h"
using namespace std;
using namespace placeholders;


WindmillObject * WindmillObject::create (IntPoint t_sp, int t_thornsFrame)
{
	WindmillObject* t_to = new WindmillObject();
	t_to->myInit(t_sp, t_thornsFrame);
	t_to->autorelease();
	return t_to;
}
void WindmillObject::startMyAction ()
{
	ingFrame = -30;
	
	CCRotateBy* t_rotate = CCRotateBy::create(1.f, 240);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_rotate);
	
	runAction(t_repeat);
	
	schedule(schedule_selector(WindmillObject::myAction));
}
void WindmillObject::myAction ()
{
	ingFrame++;
	
	int surroundCnt = 0;
	IntPoint checkPoint = IntPoint(myPoint.x-1, myPoint.y);
	if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
	checkPoint = IntPoint(myPoint.x+1, myPoint.y);
	if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
	checkPoint = IntPoint(myPoint.x, myPoint.y-1);
	if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
	checkPoint = IntPoint(myPoint.x, myPoint.y+1);
	if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
	
	if(surroundCnt == 0)
	{
		stopMyAction();
		return;
	}
	
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
	
	CCPoint subPosition = ccpSub(jackPosition, getPosition());
	
	float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
	
	if(distance < 16*getScale() && myGD->getCommunicationBool("Jack_isDie") == false)
	{
		myGD->communication("CP_jackCrashDie");
		myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
		is_action = false;
		ingFrame = 1;
		//			return;
	}
	
	if(!is_action)
	{
		if(ingFrame < 0)
			setScale((30+ingFrame)*0.033);
		else if(ingFrame == 0)
		{
			setScale((30+ingFrame)*0.033);
			is_action = true;
		}
		else if(ingFrame > 0)
		{
			setScale((30-ingFrame)*0.033);
			if(ingFrame >= 30)
			{
				stopMyAction();
				return;
			}
		}
	}
	else
	{
		if(ingFrame >= thornsFrame)
		{
			is_action = false;
			ingFrame = 0;
		}
	}
}
void WindmillObject::stopMyAction ()
{
	unschedule(schedule_selector(WindmillObject::myAction));
	removeFromParentAndCleanup(true);
}
void WindmillObject::myInit (IntPoint t_sp, int t_thornsFrame)
{
	CCSprite::init();
	myPoint = t_sp;
	is_action = false;
	thornsFrame = t_thornsFrame;
	//		initWithFile("thorns_wall.png");
	
	auto ret = KS::loadCCBI<CCSprite*>(this, "pattern_thornswall1.ccbi");
	addChild(ret.first);
	setScale(0.01f);
	setPosition(ccp((myPoint.x-1)*pixelSize+1,(myPoint.y-1)*pixelSize+1));
	
	startMyAction();
}


SightOut * SightOut::create (int totalFrame)
{
	SightOut* t_so = new SightOut();
	t_so->myInit(totalFrame);
	t_so->autorelease();
	return t_so;
}
void SightOut::startAction ()
{
	ingFrame = 0;
	schedule(schedule_selector(SightOut::myAction));
}
void SightOut::myAction ()
{
	ingFrame++;
	
	if(ingFrame <= 20)
	{
		setScale(ingFrame/5.f);
	}
	else if(ingFrame <= 100)
	{
		
	}
	else if(ingFrame <= 300)
	{
		opa_value -= 255.f/200.f;
		
		setOpacity(opa_value);
	}
	
	if(ingFrame >= totalFrame)
	{
		stopAction();
	}
}
void SightOut::stopAction ()
{
	unschedule(schedule_selector(SightOut::myAction));
	removeFromParentAndCleanup(true);
}
void SightOut::myInit (int _totalFrame)
{
	initWithFile("sight_out.png");
	totalFrame = _totalFrame;
	setScale(0);
	opa_value = 255;
}
BlindDrop * BlindDrop::create (CCPoint t_sp, CCPoint t_fp, int t_movingFrame, int t_blindFrame, float sc)
{
	BlindDrop* t_bd = new BlindDrop();
	t_bd->myInit(t_sp, t_fp, t_movingFrame, t_blindFrame, sc);
	t_bd->autorelease();
	return t_bd;
}
void BlindDrop::startAction ()
{
	AudioEngine::sharedInstance()->playEffect("sound_throw_obj_shot.mp3", false);
	ingFrame = 0;
	schedule(schedule_selector(BlindDrop::myAction));
}
void BlindDrop::completedAnimationSequenceNamed (char const * name)
{
	string t_name = name;
	if(t_name == "tornado_stop")
	{
		oilImg->removeFromParentAndCleanup(true);
		removeFromParentAndCleanup(true);
	}
}
void BlindDrop::myAction ()
{
	ingFrame++;
	
	//if(ingFrame <= blindFrame)
	//{
	//oilImg->setOpacity(oilImg->getOpacity()-(200/(blindFrame/2)));
	//}
	if(ingFrame + 1 == blindFrame)
	{
		stopAction();
	}
}
void BlindDrop::stopAction ()
{
	unschedule(schedule_selector(BlindDrop::myAction));
	reader->runAnimationsForSequenceNamed("tornado_stop");
}
void BlindDrop::myInit (CCPoint t_sp, CCPoint t_fp, int t_movingFrame, int t_blindFrame, float sc)
{
	CCLOG("BlindDrop!");
	//		subPosition = ccpSub(t_fp, t_sp);
	//		subPosition = ccpMult(subPosition, 1.f/t_movingFrame);
	movingFrame = t_movingFrame;
	blindFrame = t_blindFrame;
	m_scale = sc;
	//		dropImg = CCSprite::create("blind_drop.png");
	//		addChild(dropImg);
	
	reader = NULL;
	
	auto t_ccb = KS::loadCCBI<CCSprite*>(this, "fx_tornado1.ccbi");
	//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	//	reader = new CCBReader(nodeLoader);
	oilImg = t_ccb.first;// dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("fx_tornado1.ccbi", this));
	t_ccb.second->setDelegate(this);
	reader = t_ccb.second;
	//	reader->getAnimationManager()->setDelegate(this);
	//	reader->release();
	//	KS::setBlendFunc(oilImg, ccBlendFunc{GL_ONE_MINUS_DST_COLOR, GL_ONE});
	addChild(oilImg);
	oilImg->setScale(m_scale);
	setPosition(t_fp); // t_sp
}
PrisonObject * PrisonObject::create (CCPoint t_sp, int t_type, int t_prisonFrame)
{
	PrisonObject* t_po = new PrisonObject();
	t_po->myInit(t_sp, t_type, t_prisonFrame);
	t_po->autorelease();
	return t_po;
}
void PrisonObject::startMyAction ()
{
	ingFrame = 0;
	schedule(schedule_selector(PrisonObject::myAction));
}
void PrisonObject::myAction ()
{
	ingFrame++;
	
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
	
	CCPoint subPosition = ccpSub(jackPosition, getPosition());
	
	float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
	
	if(distance < 10)
	{
		myGD->communication("CP_jackCrashDie");
		myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
		
		unschedule(schedule_selector(PrisonObject::myAction));
		CCFadeTo* t_fade = CCFadeTo::create(getOpacity()/255.f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PrisonObject::removeFromParent));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
		runAction(t_seq);
		return;
	}
	
	if(ingFrame <= 51)
	{
		setOpacity(ingFrame*5);
	}
	
	if(ingFrame >= prisonFrame-51)
	{
		setOpacity(255-(ingFrame-prisonFrame+51)*5);
	}
	
	
	if(ingFrame >= prisonFrame)
	{
		stopMyAction();
	}
}
void PrisonObject::stopMyAction ()
{
	unschedule(schedule_selector(PrisonObject::myAction));
	removeFromParentAndCleanup(true);
}
void PrisonObject::myInit (CCPoint t_sp, int t_type, int t_prisonFrame)
{
	prisonFrame = t_prisonFrame;
	if(t_type == 1) // fire
	{
		initWithFile("prison_fire.png");
	}
	else if(t_type == 2) // thorn
	{
		initWithFile("prison_thorn.png");
	}
	
	setOpacity(0);
	
	setPosition(t_sp);
}

KSSequenceAndRemove::~ KSSequenceAndRemove ()
{}
CCSequence * KSSequenceAndRemove::create (CCNode * thiz, std::initializer_list <CCFiniteTimeAction*> initList)
{
	
	CCArray* actions = CCArray::create();
	for(auto action : initList)
	{
		actions->addObject(action);
	}
	
	auto _remove = CCCallFunc::create(thiz, callfunc_selector(CCNode::removeFromParent));
	actions->addObject(_remove);
	
	return CCSequence::create(actions);
}
AlongOfTheLine * AlongOfTheLine::create (CCPoint cumberPosition, CCPoint jackPosition, int totalFrame, int number, float speed)
{
	AlongOfTheLine* t_bf = new AlongOfTheLine();
	t_bf->myInit(cumberPosition, jackPosition, totalFrame, number, speed);
	t_bf->autorelease();
	return t_bf;
}
void AlongOfTheLine::myInit (CCPoint cumberPosition, CCPoint jackPosition, int totalFrame, int number, float speed)
{
	m_step = 1;
	m_totalFrame = totalFrame;
	m_lineSpeed = speed;
	vector<IntPoint> oldLines;
	for(int y=0; y<mapLoopRange::mapHeightInnerEnd; y++)
	{
		for(int x=0; x<mapLoopRange::mapWidthInnerEnd; x++)
		{
			if(myGD->mapState[x][y] == mapType::mapOldline)
			{
				IntPoint ip;
				ip.x = x;
				ip.y = y;
				
				queue<AlongPath> bfs;
				if(m_directions.find(ip) != m_directions.end())
					continue;
				bfs.push(AlongPath(ip, IntPoint(0, 1)));
				
				while(1)
				{
					auto path = bfs.front();
					bfs.pop();
					CCLOG("path == %d %d, %d %d", path.point.x, path.point.y, ip.x, ip.y);
					IntPoint left(-1, 0);
					if(path.direction == IntPoint(-1, 0))
					{
						left = left.rotation(90 * M_PI / 180.f);
					}
					else if(path.direction == IntPoint(0, -1))
					{
						left = left.rotation(180 * M_PI / 180.f);
					}
					else if(path.direction == IntPoint(1, 0))
					{
						left = left.rotation(270 * M_PI / 180.f);
					}
					IntPoint top = left.rotation(-90 * M_PI / 180.f);
					IntPoint right = left.rotation(-180 * M_PI / 180.f);
					IntPoint bottom = left.rotation(90 * M_PI / 180.f);
					
					IntPoint checkPoint1 = path.point + left;
					IntPoint checkPoint2 = path.point + top;
					IntPoint checkPoint3 = path.point + right;
					if(myGD->mapState[checkPoint1.x][checkPoint1.y] == mapType::mapOldline)
					{
						m_directions[path.point] = left;
						if(checkPoint1 == ip)
							break;
						bfs.push(AlongPath(checkPoint1, left));
					}
					else if(myGD->mapState[checkPoint2.x][checkPoint2.y] == mapType::mapOldline)
					{
						m_directions[path.point] = top;
						if(checkPoint2 == ip)
							break;
						bfs.push(AlongPath(checkPoint2, top));
					}
					else if(myGD->mapState[checkPoint3.x][checkPoint3.y] == mapType::mapOldline)
					{
						m_directions[path.point] = right;
						if(checkPoint3 == ip)
							break;
						bfs.push(AlongPath(checkPoint3, right));
						
					}
					else // 아무곳도 갈곳이 없음.
					{
						m_directions[path.point] = bottom;
						IntPoint checkPoint4 = path.point + bottom;
						if(checkPoint4 == ip)
							break;
						
						if(myGD->mapState[checkPoint4.x][checkPoint4.y] == mapType::mapOldline)
							bfs.push(AlongPath(checkPoint4, bottom));
						else
							break;
					}
				}
				CCLOG("end while");
			}
		}
	}
	
	//	for(auto dir : m_directions)
	//	{
	//		CCSprite* spr = CCSprite::create();
	//		spr->setTextureRect(CCRectMake(0, 0, 1, 1));
	//		IntPoint left(-1, 0);
	//		IntPoint top = left.rotation(-90 * M_PI / 180.f);
	//		IntPoint right = left.rotation(-180 * M_PI / 180.f);
	//		IntPoint bottom = left.rotation(90 * M_PI / 180.f);
	//		if(dir.second == left)
	//		{
	//			spr->setColor(ccc3(255, 0, 0));
	//		}
	//		else if(dir.second == top)
	//		{
	//			spr->setColor(ccc3(0, 255, 0));
	//		}
	//		else if(dir.second == right)
	//		{
	//			spr->setColor(ccc3(0, 0, 255));
	//		}
	//		else if(dir.second == bottom)
	//		{
	//			spr->setColor(ccc3(80, 0, 80));
	//		}
	//		spr->setPosition(ip2ccp(dir.first));
	//		addChild(spr, 100);
	//	}
	//
	// m_directions 은 위치에 따른 directions 을 가짐.
	IntPoint left(-1, 0);
	IntPoint right(1, 0);
	IntPoint up(0, 1);
	IntPoint down(0, -1);
	
	int numbers = number;
	for(int i=0; i<numbers; i++)
	{
		auto iter = m_directions.begin();
		std::advance( iter, m_well512.GetValue(0, m_directions.size() - 1) );
		auto point = iter->first;
		auto direction = iter->second;
		
		// 목표 위치 부착.
		CCSprite* goal = KS::loadCCBI<CCSprite*>(this, "target1.ccbi").first; // CCSprite::create("satelliteBeam_targeting.png");
		addChild(goal);
		goal->setPosition(ip2ccp(point));
		
		if(iter->second == down)
		{
			goal->setRotation(90);
		}
		else if(iter->second == up)
		{
			goal->setRotation(-90);
		}
		else if(iter->second == left)
		{
			goal->setRotation(180);
		}
		Pollution pollution;
		//pollution.glue.init(cumberPosition, ip2ccp(point), 0.005f * ccpLength(ip2ccp(point) - cumberPosition));
		pollution.glue.init(cumberPosition, ip2ccp(point), 1.3f);
		pollution.alongPath.point = point;
		pollution.alongPath.direction = direction;
		pollution.spr = KS::loadCCBI<CCSprite*>(this, "fx_pollution5.ccbi").first;
		//		KS::setBlendFunc(pollution.spr, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		pollution.goal = goal;
		
		m_pollutions.push_back(pollution);
		addChild(pollution.spr);
	}
	
	//	scheduleUpdate();
	schedule(schedule_selector(ThisClassType::update), 1/60.f);
}
void AlongOfTheLine::setTwoStep ()
{
	m_step = 2;
}
void AlongOfTheLine::hidingAnimation (float dt)
{
	bool r = m_scaleTo.step(1 / 30.f);
	if(!r) // finish
	{
		removeFromParent();
	}
	else for(auto i : m_pollutions){
		i.spr->setScale(m_scaleTo.getValue());
	}
	
}
void AlongOfTheLine::update (float dt)
{
	m_frame++;
	
	if(m_frame >= m_totalFrame)
	{
		unschedule(schedule_selector(ThisClassType::update));
		m_scaleTo.init(1.f, 0.f, 0.5f);
		schedule(schedule_selector(ThisClassType::hidingAnimation));
	}
	else for(auto i = m_pollutions.begin(); i != m_pollutions.end();)
	{
		bool r = i->glue.step(1/60.f);
		
		i->spr->setPosition(i->glue.getValue());
		if(r)
		{
			i->spr->setVisible(false);
		}
		else
		{
			i->spr->setVisible(true);
		}
		if(!r && i->step == 1)
		{
			// 원하는 위치에 선따라가기가 부착이 딱 되었을 때!!,
			//i->goal->removeFromParent();
			addChild(KSGradualValue<float>::create(255, 0, 0.4f,
																						 [=](float t)
																						 {
																							 KS::setOpacity(i->goal, t);
																						 },
																						 [=](float t)
																						 {
																							 i->goal->removeFromParent();
																							 i->step = 3;
																						 }));
			i->step = 2;
			
		}
		bool erased = false;
		if(i->step == 3)
		{
			i->spr->setPosition(ip2ccp(i->alongPath.point));
			if(myGD->mapState[i->alongPath.point.x - 1][i->alongPath.point.y] == mapType::mapOldget &&
				 myGD->mapState[i->alongPath.point.x + 1][i->alongPath.point.y] == mapType::mapOldget &&
				 myGD->mapState[i->alongPath.point.x][i->alongPath.point.y + 1] == mapType::mapOldget &&
				 myGD->mapState[i->alongPath.point.x][i->alongPath.point.y - 1] == mapType::mapOldget)
			{
				//				i->spr->runAction(KSSequenceAndRemove::create(i->spr, {CCScaleTo::create(0.5f, 0.f)}));
				i->spr->removeFromParentAndCleanup(true);
				i = m_pollutions.erase(i);
				
				erased = true;
				continue;
			}
			IntPoint jackPoint = myGD->getJackPoint();
			
			if(i->alongPath.point == jackPoint)
			{
				myGD->communication("CP_jackCrashDie");
				myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
				unschedule(schedule_selector(ThisClassType::update));
				m_scaleTo.init(1.f, 0.f, 0.5f);
				CCLOG("abc");
				schedule(schedule_selector(ThisClassType::hidingAnimation));
				break;
			}
			i->alongPath.point = i->alongPath.point + i->alongPath.direction; // 방향 대로 움직임.
			
			IntPoint left(-1, 0);
			if(i->alongPath.direction == IntPoint(-1, 0))
			{
				left = left.rotation(90 * M_PI / 180.f);
			}
			else if(i->alongPath.direction == IntPoint(0, -1))
			{
				left = left.rotation(180 * M_PI / 180.f);
			}
			else if(i->alongPath.direction == IntPoint(1, 0))
			{
				left = left.rotation(270 * M_PI / 180.f);
			}
			IntPoint top = left.rotation(-90 * M_PI / 180.f);
			IntPoint right = left.rotation(-180 * M_PI / 180.f);
			IntPoint bottom = left.rotation(90 * M_PI / 180.f);
			
			IntPoint checkPoint1 = i->alongPath.point + left;
			IntPoint checkPoint2 = i->alongPath.point + top;
			IntPoint checkPoint3 = i->alongPath.point + right;
			if(myGD->mapState[checkPoint1.x][checkPoint1.y] == mapType::mapOldline)
			{
				i->alongPath.direction = left;
			}
			else if(myGD->mapState[checkPoint2.x][checkPoint2.y] == mapType::mapOldline)
			{
				i->alongPath.direction = top;
			}
			else if(myGD->mapState[checkPoint3.x][checkPoint3.y] == mapType::mapOldline)
			{
				i->alongPath.direction = right;
			}
			else // 아무곳도 갈곳이 없음.
			{
				i->alongPath.direction = bottom;
			}
		}
		if(erased == false)
			++i;
	}
	
}
AlongOfTheLine::AlongPath::AlongPath (IntPoint const & pt, IntPoint const & dir)
: point (pt), direction (dir)
{}
AlongOfTheLine::AlongPath::AlongPath ()
{}
AlongOfTheLine::Pollution::Pollution ()
: step (1)
{}
CloudBomb * CloudBomb::create (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern)
{
	CloudBomb* t_bf = new CloudBomb();
	t_bf->myInit(cumberPosition, jackPosition, pattern);
	t_bf->autorelease();
	return t_bf;
}
void CloudBomb::myInit (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern)
{
	m_step = 1;
	m_bombProb = 0.004f;
	m_pattern = pattern;
	m_parentMissile = CCParticleSystemQuad::create("cloudbomb.plist");
	m_parentMissile->setPositionType(kCCPositionTypeRelative);
	m_parentMissile->setStartColor(ccc4f(0, 1, 1, 1)); //##
	m_parentMissile->setEndColor(ccc4f(0, 1, 1, 1)); //##
	
	
	m_parentMissile->setPosition(jackPosition);
	addChild(m_parentMissile);
	
	scheduleUpdate();
	
	int m_color = m_pattern.get("color", 1).asInt();;
	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);
	
	addChild(batchNode);
}
void CloudBomb::setTwoStep ()
{
	addChild(KSGradualValue<float>::create(1.f, 0.f, 2.f, [=](float t){
		m_parentMissile->setStartColor(ccc4f(1 - t, t, t, 1));
		m_parentMissile->setEndColor(ccc4f(1 - t, t, t, 1));
	},
																				 [=](float t){
																					 m_step = 2;
																					 m_frame = 0;
																					 m_sourcePosition = m_parentMissile->getPosition();
																					 m_parentMissile->setStartColor(ccc4f(0, 0, 0, 0));
																					 m_parentMissile->setEndColor(ccc4f(0, 0, 0, 0));
																					 
																					 m_parentMissile->runAction(KSSequenceAndRemove::create(m_parentMissile, {CCDelayTime::create(3.f)}));
																					 
																					 schedule(schedule_selector(ThisClassType::selfRemove));
																				 }));
}
void CloudBomb::selfRemove (float dt)
{
	if(batchNode->getChildrenCount() == 0 && getChildrenCount() == 1)
	{
		removeFromParentAndCleanup(true);
	}
}
void CloudBomb::update (float dt)
{
	//		CCLOG("pokjuk %d", m_frame);
	if(m_step == 1)
	{
		m_frame++;
		IntPoint pos = ccp2ip(m_parentMissile->getPosition());
		IntPoint afterPos;
		while(1)
		{
			afterPos = IntPoint(pos.x + m_well512.GetValue(-2, +2),
													pos.y + m_well512.GetValue(-2, +2));
			if(afterPos.isInnerMap())
				break;
		}
		
		m_parentMissile->setPosition(ip2ccp(afterPos));
		
		int p = ProbSelector::sel(m_bombProb, 1.f - m_bombProb, 0.f);
		if(p == 0)
		{
			setTwoStep();
		}
	}
	
	if(m_step == 2) // 폭발.
	{
		m_frame++;
		
		int number = m_pattern.get("bulletnumber", 36).asInt();
		float bulletSpeed = m_pattern.get("bulletspeed", 500.f).asFloat() / 100.f;
		int m_color = m_pattern.get("color", 1).asInt();
		std::string imgFileName;
		std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
		if(KS::isExistFile(fileName))
			imgFileName = fileName;
		else
			imgFileName = "cumber_missile1.png";
		CCSize t_mSize = CCSize(4.f, 4.f);
		for(int i=0; i<=360; i+= 360 / number)
		{
			MissileUnit* t_mu = MissileUnit::create(m_sourcePosition, i, bulletSpeed,
																							imgFileName.c_str(), t_mSize,0, 0);
			batchNode->addChild(t_mu);
		}
		m_step = 3;
	}
}
PoisonLine * PoisonLine::create (IntPoint t_sp, int frame, bool deco)
{
	PoisonLine* t_pl = new PoisonLine();
	t_pl->myInit(t_sp, frame, deco);
	t_pl->autorelease();
	return t_pl;
}
void PoisonLine::startMyAction ()
{
	ingFrame = 0;
	schedule(schedule_selector(PoisonLine::myAction));
}
void PoisonLine::myAction ()
{
	ingFrame++;
	
	//		ccColor3B t_color = getColor();
	//		t_color.r += rand()%3-1;
	//		t_color.g += rand()%3-1;
	//		t_color.b += rand()%3-1;
	//
	//		setColor(t_color);
	
	int emptyCnt = 0;
	if(myGD->mapState[mapPoint.x-1][mapPoint.y] == mapEmpty)	emptyCnt++;
	if(myGD->mapState[mapPoint.x+1][mapPoint.y] == mapEmpty)	emptyCnt++;
	if(myGD->mapState[mapPoint.x][mapPoint.y-1] == mapEmpty)	emptyCnt++;
	if(myGD->mapState[mapPoint.x][mapPoint.y+1] == mapEmpty)	emptyCnt++;
	
	if(emptyCnt == 0)
	{
		unschedule(schedule_selector(PoisonLine::myAction));
		removeFromParentAndCleanup(true);
		return;
	}
	
	IntPoint jackPoint = myGD->getJackPoint();
	if(jackPoint.x == mapPoint.x && jackPoint.y == mapPoint.y && decoration == false)
	{
		myGD->communication("CP_jackCrashDie");
		myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
		stopMyAction();
		
		return;
	}
	
	if(ingFrame >= totalFrame)
	{
		stopMyAction();
	}
}
void PoisonLine::stopMyAction ()
{
	unschedule(schedule_selector(PoisonLine::myAction));
	
	line->setEmissionRate(0.f);
	addChild(KSGradualValue<float>::create(255.f, 0.f, 0.4f, [=](float t){
		//KS::setOpacity(line, t);
	},
																				 [=](float t){
																					 getParent()->removeFromParentAndCleanup(true);
																				 }));
	//		removeFromParentAndCleanup(true);
}
void PoisonLine::myInit (IntPoint t_sp, int frame, bool deco)
{
	totalFrame = frame;
	mapPoint = t_sp;
	decoration = deco;
	
	//		initWithFile("poison_line.png");
	if(decoration)
	{
		auto ret = KS::loadCCBI<CCParticleSystemQuad*>(this, "fx_pollution_whie.ccbi");
		
		line = ret.first;
		line->setScale(0.7f);
		//	KS::setBlendFunc(line, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		line->setPositionType(kCCPositionTypeGrouped);
		addChild(line);
	}
	else
	{
		auto ret = KS::loadCCBI<CCParticleSystemQuad*>(this, "fx_pollution7.ccbi");
		line = ret.first;
		line->setScale(0.7f);
		//	KS::setBlendFunc(line, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		line->setPositionType(kCCPositionTypeGrouped);
		addChild(line);
		
	}
	CCPoint myPosition = ccp((t_sp.x-1)*pixelSize+1, (t_sp.y-1)*pixelSize+1);
	setPosition(myPosition);
	
	//		setColor(ccc3(0, 66, 75));
}
PoisonDrop * PoisonDrop::create (CCPoint t_sp, CCPoint t_fp, int t_movingFrame, int area, int totalframe, float enableRatio)
{
	PoisonDrop* t_bd = new PoisonDrop();
	t_bd->myInit(t_sp, t_fp, t_movingFrame, area, totalframe, enableRatio);
	t_bd->autorelease();
	return t_bd;
}
void PoisonDrop::startAction ()
{
	AudioEngine::sharedInstance()->playEffect("sound_throw_obj_shot.mp3", false);
	ingFrame = 0;
	schedule(schedule_selector(PoisonDrop::myAction));
}
void PoisonDrop::myAction ()
{
	ingFrame++;
	
	if(ingFrame <= movingFrame)
	{
		CCPoint afterPosition = ccpAdd(dropImg->getPosition(), subPosition);
		dropImg->setPosition(afterPosition);
		
		if(ingFrame == movingFrame)
		{
			AudioEngine::sharedInstance()->playEffect("sound_threecusion_bomb.mp3",false);
			initParticle();
			addChild(KSGradualValue<float>::create(255, 0, 0.4f, [=](float t){
				if(targetImg){
					KS::setOpacity(targetImg, t);
				}
			},
																						 [=](float t){
																						 }));
			IntPoint basePoint = IntPoint((afterPosition.x-1)/pixelSize + 1, (afterPosition.y-1)/pixelSize + 1);
			
			for(int i=-m_area;i<=m_area;i++)
			{
				for(int j=-m_area;j<=m_area;j++)
				{
					IntPoint checkPoint = IntPoint(basePoint.x+i,basePoint.y+j);
					float maxDistance = m_area;
					float curDistance = sqrtf(i * i + j * j);
					if(maxDistance * m_enableRatio >= curDistance)
					{
						if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapOldline)
						{
							PoisonLine* t_pl = PoisonLine::create(IntPoint(checkPoint.x, checkPoint.y), m_totalFrame, false);
							getParent()->addChild(t_pl, 2);
							t_pl->startMyAction();
						}
						
					}
					else
					{
						if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapOldline)
						{
							PoisonLine* t_pl = PoisonLine::create(IntPoint(checkPoint.x, checkPoint.y), m_totalFrame, true);
							getParent()->addChild(t_pl);
							t_pl->startMyAction();
						}
						
					}
				}
			}
			dropImg->removeFromParentAndCleanup(true);
		}
	}
	
	
	if(ingFrame >= movingFrame)
	{
		stopAction();
	}
}
void PoisonDrop::initParticle ()
{
	auto ret = KS::loadCCBI<CCSprite*>(this, "bomb_8_9.ccbi");
	CCSprite* particle = ret.first;
	
	//	KS::setBlendFunc(particle, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	//		particle->setPosVar(CCPointZero);
	addChild(particle);
	particle->setPosition(dropImg->getPosition());
}
void PoisonDrop::stopAction ()
{
	unschedule(schedule_selector(PoisonDrop::myAction));
	CCDelayTime* t_delay = CCDelayTime::create(1.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PoisonDrop::selfRemove));
	
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	
	runAction(t_seq);
}
void PoisonDrop::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void PoisonDrop::myInit (CCPoint t_sp, CCPoint t_fp, int t_movingFrame, int area, int totalframe, float enableRatio)
{
	m_area = area;
	m_totalFrame = totalframe;
	m_enableRatio = enableRatio;
	targetImg = KS::loadCCBI<CCSprite*>(this, "target5.ccbi").first;
	targetImg->setPosition(t_fp);
	addChild(targetImg);
	
	
	
	subPosition = ccpSub(t_fp, t_sp);
	subPosition = ccpMult(subPosition, 1.f/t_movingFrame);
	movingFrame = t_movingFrame;
	
	dropImg = KS::loadCCBI<CCSprite*>(this, "pattern_radioactivity_1.ccbi").first;
	addChild(dropImg);
	
	dropImg->setPosition(t_sp);
}
ReflectionLazer * ReflectionLazer::create (CCPoint t_sp, CCPoint t_fp, int t_frame, int t_type)
{
	ReflectionLazer* t_rl = new ReflectionLazer();
	t_rl->myInit(t_sp, t_fp, t_frame, t_type);
	t_rl->autorelease();
	return t_rl;
}
void ReflectionLazer::startMyAction ()
{
	ing_frame = 0;
	schedule(schedule_selector(ReflectionLazer::myAction));
}
void ReflectionLazer::myAction ()
{
	ing_frame++;
	
	setPosition(ccpAdd(getPosition(), dv));
	
	if(getScaleX() < MAX_SCALE_X)
	{
		setScaleX(getScaleX() + ADD_SCALE_X);
	}
	
	if(ing_frame >= move_frame)
	{
		stopMyAction();
	}
}
void ReflectionLazer::stopMyAction ()
{
	unschedule(schedule_selector(ReflectionLazer::myAction));
	schedule(schedule_selector(ReflectionLazer::selfRemove));
}
void ReflectionLazer::selfRemove ()
{
	if(getScaleX() > ADD_SCALE_X)
	{
		setScaleX(getScaleX() - ADD_SCALE_X);
	}
	else if(getScaleX() == ADD_SCALE_X)
	{
		setScaleX(0.f);
	}
	else
	{
		unschedule(schedule_selector(ReflectionLazer::selfRemove));
		removeFromParentAndCleanup(true);
	}
}
void ReflectionLazer::myInit (CCPoint t_sp, CCPoint t_fp, int t_frame, int t_type)
{
	move_frame = t_frame;
	initWithFile(CCString::createWithFormat("lazerMissile%d.png", t_type)->getCString());
	setAnchorPoint(ccp(0,0.5));
	setPosition(t_sp);
	setScaleX(0);
	
	CCPoint subPosition = ccpSub(t_fp, t_sp);
	
	dv = ccpMult(subPosition, 1.f/move_frame);
	
	float t_angle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + 180;
	setRotation(-t_angle);
}
MirrorReflection * MirrorReflection::create (int t_sn, float t_r, int t_direction, int t_type)
{
	MirrorReflection* t_mr = new MirrorReflection();
	t_mr->myInit(t_sn, t_r, t_direction, t_type);
	t_mr->autorelease();
	return t_mr;
}
void MirrorReflection::startMyAction ()
{
	ing_frame = -1;
	schedule(schedule_selector(MirrorReflection::myAction));
}
CCPoint MirrorReflection::getNumberPosition (int t_number)
{
	float t_angle = t_number*40.f + base_angle;
	CCPoint r_value;
	r_value.x = 1;
	r_value.y = tanf(t_angle/180.f*M_PI);
	
	if(t_angle >= 90 && t_angle < 270)
		r_value = ccpMult(r_value, -1.f);
	
	float div_value = sqrtf(powf(r_value.x, 2.f) + powf(r_value.y, 2.f));
	
	r_value = ccpMult(r_value, 1.f/div_value);
	r_value = ccpMult(r_value, radius);
	
	return r_value;
}
void MirrorReflection::myAction ()
{
	ing_frame++;
	
	setRotation(getRotation() + rand_rotate);
	int checking_value = ing_frame%create_frame;
	if(checking_value == 0)
	{
		CCPoint s_p = getNumberPosition(sn);
		CCPoint f_p = getNumberPosition(fn);
		
		ReflectionLazer* t_rl = ReflectionLazer::create(s_p, f_p, create_frame, myType);
		addChild(t_rl, 1);
		t_rl->startMyAction();
		
		CCPoint subPosition = ccpSub(f_p, s_p);
		
		float rotate_value = -(atan2f(subPosition.y, subPosition.x)/M_PI*180.f + 90.f);
		float keep_rotate = effectImg->getRotation();
		
		effectImg->setRotation(rotate_value);
		effectImg->setPosition(ccpMidpoint(f_p, s_p));
		
		impactImg->setPosition(s_p);
		impactImg->setRotation(keep_rotate);
		impactImg->setOpacity(255);
		
		sn = fn;
		fn = (fn+direction)%9;
	}
	else if(checking_value == 4)
	{
		effectImg->setOpacity(255);
		impactImg->setOpacity(0);
	}
	else if(checking_value == 7)
	{
		effectImg->setOpacity(0);
	}
	
	if(ing_frame >= 900)
	{
		stopMyAction();
	}
}
void MirrorReflection::stopMyAction ()
{
	unschedule(schedule_selector(MirrorReflection::myAction));
	removeFromParentAndCleanup(true);
}
void MirrorReflection::myInit (int t_sn, float t_r, int t_direction, int t_type)
{
	effectImg = CCSprite::create("lazer_effect.png");
	//		ccBlendFunc t_b;
	//		t_b.src = GL_SRC_ALPHA;
	//		t_b.dst = GL_ONE;
	//		effectImg->setBlendFunc(t_b);
	effectImg->setOpacity(0);
	addChild(effectImg, 2);
	
	impactImg = CCSprite::create("reflection_impact.png");
	impactImg->setAnchorPoint(ccp(0.5,0));
	impactImg->setOpacity(0);
	addChild(impactImg, 2);
	
	rand_rotate = rand()%2;
	if(rand_rotate == 0) rand_rotate = -1;
	base_angle = rand()%40;
	create_frame = rand()%5 + 9;
	myType = t_type;
	radius = t_r;
	sn = t_sn;
	if(t_direction == 1)
		direction = 4;
	else if(t_direction == -1)
		direction = 5;
	
	fn = (sn + direction)%9;
}


VMesh* VMesh::create(const Json::Value& param)
{
	VMesh* t_to = new VMesh();
	t_to->myInit(param);
	t_to->autorelease();
	return t_to;
}
void VMesh::stopMyAction ()
{
	
}
void VMesh::myInit(const Json::Value& param)
{
	m_pos = param.get("x", 0).asFloat();
	m_originalDelayFrames = m_delayFrames = param.get("delayframes", 180).asInt();
	m_currentFrames = 0;
	schedule(schedule_selector(ThisClassType::myAction));
	float prob = param.get("enableprob", 1.f).asFloat();
	ProbSelector ps = {prob, 1.f - prob};
	m_enabled = ps.getResult() == 0;
	
	m_mesh = EffectSprite::create("lazer_sub.png");
	if(m_enabled == false)
	{
		m_mesh->setGray();
	}
	m_mesh->setRotation(90);
	m_mesh->setScaleX(5.0f);
	m_mesh->setPosition(ccp(m_pos, 0));
	//	m_hMesh->setColor(ccc3(255, 0, 0));
	m_dColor = 255.f / (float)m_delayFrames;
	//	m_hMesh->setOpacity(0);
	addChild(m_mesh);
	
	m_laserContainer = CCNode::create();
	addChild(m_laserContainer);
	m_attacked = false;
	m_thickness = param.get("thickness", 12).asInt();
	
	
}

void VMesh::myAction(float dt)
{
	m_currentFrames++;
	if(m_currentFrames <= m_delayFrames)
	{
		//
		float progress = (float)m_currentFrames / (float)m_originalDelayFrames;
		auto c = m_mesh->getColor();
		c.r -= m_dColor;
		c.b -= m_dColor;
		m_mesh->setColor(c);
		//		m_hMesh->setOpacity(255.f * progress);
	}
	else if(m_attacked == false)
	{
		m_attacked = true;
		AudioEngine::sharedInstance()->playEffect("se_destructionlaser_2.mp3", false);
		m_mesh->setVisible(false);
		if(m_enabled)
		{
			auto ret2 = CCSprite::create("pattern_laser_yellow_h.png");
			CCSprite* laser3 = ret2;
			//				laser3->setContentSize(lazer_main->getContentSize());
			KS::setBlendFunc(laser3, ccBlendFunc{GL_ONE, GL_ONE_MINUS_SRC_ALPHA});
			laser3->setPosition(ccp(m_mesh->getPositionX(), 0));
			laser3->setScaleY(40);
			//laser3->setScaleY(m_crashSize/8.f);
			//laser3->setScaleY(2.f);
			m_laserContainer->addChild(laser3);
			
			auto jackPosition = myGD->getJackPoint().convertToCCP();
			CCLOG("diff jack %f", fabsf(jackPosition.x - m_pos));
			if(fabsf(jackPosition.x - m_pos) <= m_thickness)
			{
				myGD->communication("CP_jackCrashDie");
				myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
			}
		}
		
		
		
		
		addChild(KSGradualValue<float>::create(1.f, 0.1f, 0.3f, [=](float t){
			for(int i=0; i<m_laserContainer->getChildrenCount(); i++)
			{
				CCSprite* ts = (CCSprite*)m_laserContainer->getChildren()->objectAtIndex(i);
				ts->setScaleX(t);
			}
		}, [=](float t) {
			for(int i=0; i<m_laserContainer->getChildrenCount(); i++)
			{
				CCSprite* ts = (CCSprite*)m_laserContainer->getChildren()->objectAtIndex(i);
				ts->setScaleX(t);
			}
			
			removeFromParent();
		}));
		
		
		
	}
	
}





HMesh* HMesh::create(const Json::Value& param)
{
	HMesh* t_to = new HMesh();
	t_to->myInit(param);
	t_to->autorelease();
	return t_to;
}
void HMesh::stopMyAction ()
{
	
}
void HMesh::myInit(const Json::Value& param)
{
	m_pos = param.get("y", 0).asFloat();
	m_originalDelayFrames = m_delayFrames = param.get("delayframes", 180).asInt();
	m_currentFrames = 0;
	
	
	
	
	schedule(schedule_selector(ThisClassType::myAction));
	
	float prob = param.get("enableprob", 1.f).asFloat();
	ProbSelector ps = {prob, 1.f - prob};
	m_enabled = ps.getResult() == 0;
	
	m_mesh = EffectSprite::create("lazer_sub.png");
	if(m_enabled == false)
	{
		m_mesh->setGray();
	}
	
	
	
	m_mesh->setScaleX(5.0f);
	m_mesh->setPosition(ccp(0, m_pos));
	//	m_hMesh->setColor(ccc3(255, 0, 0));
	m_dColor = 255.f / (float)m_delayFrames;
	//	m_hMesh->setOpacity(0);
	addChild(m_mesh);
	
	m_laserContainer = CCNode::create();
	addChild(m_laserContainer);
	m_attacked = false;
	m_thickness = param.get("thickness", 12).asInt();
}

void HMesh::myAction(float dt)
{
	m_currentFrames++;
	if(m_currentFrames <= m_delayFrames)
	{
		//
		float progress = (float)m_currentFrames / (float)m_originalDelayFrames;
		auto c = m_mesh->getColor();
		c.r -= m_dColor;
		c.b -= m_dColor;
//		c.g -= m_dColor;
		m_mesh->setColor(c);
		//		m_hMesh->setOpacity(255.f * progress);
	}
	else if(m_attacked == false)
	{
		AudioEngine::sharedInstance()->playEffect("se_destructionlaser_2.mp3", false);
		m_mesh->setVisible(false);
		m_attacked = true;
		
		if(m_enabled)
		{
			auto ret2 = CCSprite::create("pattern_laser_yellow_w.png");
			CCSprite* laser3 = ret2;
			//				laser3->setContentSize(lazer_main->getContentSize());
			KS::setBlendFunc(laser3, ccBlendFunc{GL_ONE, GL_ONE_MINUS_SRC_ALPHA});
			laser3->setPosition(ccp(0, m_mesh->getPositionY()));
			laser3->setScaleX(40);
			//laser3->setScaleY(m_crashSize/8.f);
			//laser3->setScaleY(2.f);
			m_laserContainer->addChild(laser3);
			auto jackPosition = myGD->getJackPoint().convertToCCP();
			CCLOG("diff jack %f", fabsf(jackPosition.y - m_pos));
			if(fabsf(jackPosition.y - m_pos) <= m_thickness)
			{
				myGD->communication("CP_jackCrashDie");
				myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
			}
			
		}
		
		addChild(KSGradualValue<float>::create(1.f, 0.1f, 0.3f, [=](float t){
			for(int i=0; i<m_laserContainer->getChildrenCount(); i++)
			{
				CCSprite* ts = (CCSprite*)m_laserContainer->getChildren()->objectAtIndex(i);
				ts->setScaleY(t);
			}
		}, [=](float t) {
			for(int i=0; i<m_laserContainer->getChildrenCount(); i++)
			{
				CCSprite* ts = (CCSprite*)m_laserContainer->getChildren()->objectAtIndex(i);
				ts->setScaleY(t);
			}
			
			removeFromParent();
		}));
		
		
		
	}
	
}
HideCloud* HideCloud::create(KSCumberBase* cumber, const Json::Value& param)
{
	HideCloud* t_bd = new HideCloud();
	t_bd->myInit(cumber, param);
	t_bd->autorelease();
	return t_bd;
}

void HideCloud::myInit(KSCumberBase* cumber, const Json::Value& param)
{
	// fx_tornado1.ccbi
	
	//	mapLoopRange::mapWidth
	CCPoint jackPoint = myGD->getJackPointCCP();
	
	CCPoint targetPoint = CCPointZero;
	targetPoint = ccp(ks19937::getFloatValue(MAX(0, jackPoint.x - 150), MIN(mapLoopRange::mapWidthInnerEnd*2 - 1, jackPoint.x + 150)),
										ks19937::getFloatValue(MAX(0, jackPoint.y - 150), MIN(mapLoopRange::mapHeightInnerEnd*2 - 1, jackPoint.y + 150)));
	m_cloudSprite = KS::loadCCBI<CCSprite*>(this, "pattern_cloud.ccbi").first;
	float tempProb = param.get("enableprob", 1.f).asFloat();
	ProbSelector ps = {tempProb, 1.f - tempProb};
	float goalOpacity;
	addChild(KSGradualValue<float>::create(0, 1.f, 0.5f, [=](float t){
		m_cloudSprite->setScale(t);
		
	}, [=](float t){
		m_cloudSprite->setScale(t);
		
	}, elasticOut2));
	if(ps.getResult() == 0)
	{
		goalOpacity = 255.f;
	}
	else
	{
		goalOpacity = 100.f;
	}
	
	addChild(KSGradualValue<float>::create(0, goalOpacity, 0.5f, [=](float t)
																				 {
																					 KS::setOpacity(m_cloudSprite, t);
																					 
																				 }, [=](float t)
																				 {
																					 KS::setOpacity(m_cloudSprite, t);
																					 
																				 }));
	m_cloudSprite->setPosition(targetPoint);
	addChild(m_cloudSprite);
	
	m_durations = param.get("duration", 300).asInt();
	m_speed = param.get("speed", 100.f).asFloat() / 100.f;
	m_currentFrames = 0;
	
	
	m_hided = false;
	scheduleUpdate();
}

void HideCloud::update(float dt)
{
	m_currentFrames++;
	
	
	if(m_currentFrames >= m_durations && m_hided == false)
	{
		m_hided = true;
		addChild(KSGradualValue<float>::create(1.f, 0.1f, 0.5f, [=](float t)
																					 {
																						 m_cloudSprite->setScale(t);
																					 }, [=](float t)
																					 {
																						 m_cloudSprite->setScale(t);
																						 removeFromParent();
																					 }));
	}
	else
	{
		float theta = ks19937::getFloatValue(0, 2 * M_PI);
		CCPoint dP = ccpMult(ccp(cosf(theta), sin(theta)), m_speed);
		m_cloudSprite->setPosition(m_cloudSprite->getPosition() + dP);
	}
}


Jelly* Jelly::create(KSCumberBase* cumber, const Json::Value& param)
{
	Jelly* t_bd = new Jelly();
	t_bd->myInit(cumber, param);
	t_bd->autorelease();
	return t_bd;
}
Jelly::~Jelly()
{
	
	if(m_step == 3)
	{
		myGD->setAlphaSpeed(myGD->getAlphaSpeed() + m_decreaseSpeed);
	}
	
	CCLOG("~Jelly");
}

void Jelly::myInit(KSCumberBase* cumber, const Json::Value& param)
{
	/// pattern_cloud.ccbi 쓰자...
	
	
	m_jellySprite = KS::loadCCBI<CCSprite*>(this, "pattern_cloud.ccbi").first;
	addChild(m_jellySprite);
	m_jellySprite->setPosition(cumber->getPosition());
	float randomRad = ks19937::getFloatValue(0, 2*M_PI);
	addChild(KSGradualValue<CCPoint>::create(cumber->getPosition(), cumber->getPosition() + ccp(30.f * cosf(randomRad), 30 * sinf(randomRad)), 0.5f, [=](CCPoint t){
		m_jellySprite->setPosition(t);
		
	}, [=](CCPoint t){
		m_jellySprite->setPosition(t);
		scheduleUpdate();
		
	}));
	m_currentSeek = 0;
	m_lifeTime = param.get("life", 200).asInt();
	m_seekTime = param.get("seek", 300).asInt();
	m_step = 1;
	m_decreaseSpeed = 0.5f;
	m_currentSpeed = 1.f;
	
	float tempProb = param.get("enableprob", 1.f).asFloat();
	ProbSelector ps = {tempProb, 1.f - tempProb};

	m_enabled = ps.getResult() == 0;
	
	if(m_enabled == false)
	{
		m_jellySprite->setColor(ccc3(0, 0, 0)); // 비활성화
	}
}

void Jelly::update(float dt)
{
	CCPoint goalPosition = myGD->getJackPointCCP();
	m_currentSpeed = MAX(0.6f, m_currentSpeed - 0.005f);
	float rad = atan2f(goalPosition.y - m_jellySprite->getPositionY(), goalPosition.x - m_jellySprite->getPositionX() );
	if(m_step == 1)
	{
		m_jellySprite->setPosition(m_jellySprite->getPosition() + ccp(m_currentSpeed * cosf(rad), m_currentSpeed * sin(rad)));
		
		
		float distance = ccpLength(m_jellySprite->getPosition() - goalPosition);
		if(distance <= 30)
		{
			m_step = 2;
		}
		//
	}
	else if(m_step == 2)
	{
		// 근처에 도달함
		m_currentSeek++;
		if(m_currentSeek < m_seekTime)
		{
			CCPoint goalPosition = myGD->getJackPointCCP();
//			float rad = atan2f(m_jellySprite->getPositionY() - goalPosition.y, m_jellySprite->getPositionX() - goalPosition.x);
			m_jellySprite->setPosition(m_jellySprite->getPosition() + ccp(m_currentSpeed * cosf(rad), m_currentSpeed * sin(rad)));
			
			
			float distance = ccpLength(m_jellySprite->getPosition() - goalPosition);
			if(distance <= 2)
			{
				myGD->setAlphaSpeed(myGD->getAlphaSpeed() - m_decreaseSpeed);
				m_step = 3;
			}
		}
		else
		{
			// 못 물어서 사라져야됨
			removeFromParent();
		}
		
	}
	else if(m_step == 3)
	{
		// 물었다.
		if(m_enabled == false)
		{
			removeFromParent();
		}
		else
		{
			m_currentLife++;
			CCPoint goalPosition = myGD->getJackPointCCP();
			m_jellySprite->setPosition(goalPosition);
			
			if(m_currentLife < m_lifeTime)
			{
				
			}
			else
			{
				removeFromParent();
			}
			
		}
	}
	
	else
	{
		
	}
	
}


