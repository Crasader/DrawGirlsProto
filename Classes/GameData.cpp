//
//  GameData.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 6..
//
//

#include "GameData.h"
#include "SilhouetteData.h"
#include "DataStorageHub.h"
#include "ks19937.h"
#include "StarGoldData.h"
#include "MissileUnit.h"
float deg2Rad(float x) { return x * M_PI / 180.f;}
float rad2Deg(float x) { return x * 180.f / M_PI;}

CCPoint ip2ccp(const IntPoint& ip)
{
	return CCPoint((ip.x - 1) * pixelSize + 1.f,
				   (ip.y - 1) * pixelSize + 1.f);
}

IntPoint ccp2ip(const CCPoint& cc)
{
	IntPoint ip;
	ip.x = round((cc.x - 1) / pixelSize + 1);
	ip.y = round((cc.y - 1) / pixelSize + 1);
	return ip;
}

GameData* GameData::sharedGameData()
{
	static GameData* t_GD = NULL;
	if(t_GD == NULL)
	{
		t_GD = new GameData();
		t_GD->myInit();
	}
	return t_GD;
}

void GameData::resetGameData()
{
	jackState = 0; // normal
	jackPoint->release();
	jackPoint = new IntPoint();
	mainCumberPoint->release();
	mainCumberPoint = new IntPoint();
	otherTargetPoints->removeAllObjects();
	
	is_setted_jack = false;
	
	target_Main = NULL;

	for(int i=mapWidthOutlineBegin;i<mapWidthOutlineEnd;i++)
	{
		for(int j=mapHeightOutlineBegin;j<mapHeightOutlineEnd;j++)
		{
			if(i == mapWidthOutlineBegin || j == mapHeightOutlineBegin || i == mapWidthOutlineEnd-1 || j == mapHeightOutlineEnd-1)
				mapState[i][j] = mapOutline;
			else
				mapState[i][j] = mapEmpty;
		}
	}
}

CCPoint GameData::getCommunicationCCPoint(string funcName)
{
	CCAssert(CCP_V.find(funcName) != CCP_V.end(), funcName.c_str());
	return CCP_V[funcName]();
}

CCNode* GameData::getCommunicationNode(string funcName)
{
	if(CCN_V.find(funcName) != CCN_V.end())
		return CCN_V[funcName]();
	return NULL;
}

CCArray* GameData::getCommunicationArray(string funcName)
{
	CCAssert(CCA_V.find(funcName) != CCA_V.end(), funcName.c_str());
	return CCA_V[funcName]();
}

float GameData::Fcommunication(string funcName)
{
	CCAssert(F_V.find(funcName) != F_V.end(), funcName.c_str());
	return F_V[funcName]();
}

int GameData::communication(std::string &funcName, CCPoint t_p, std::string &t_i, KSCumberBase* cb, bool t_b)
{
	CCAssert(I_CCPStrCumberBaseB.find(funcName.c_str()) != I_CCPStrCumberBaseB.end(), funcName.c_str());
	return I_CCPStrCumberBaseB[funcName](t_p, t_i, cb, t_b);
}

//bool GameData::communication(string funcName, CCObject* t_obj, float f_val, float f_val2)
//{
	//CCAssert(B_CCOFF.find(funcName) != B_CCOFF.end(), funcName.c_str());
	//return B_CCOFF[funcName](t_obj, f_val, f_val2);
//}

bool GameData::communication(string funcName, CCObject* t_obj, float f_val, float f_val2, bool b1, bool b2)
{
	CCAssert(B_CCOFFBB.find(funcName) != B_CCOFFBB.end(), funcName.c_str());
	return B_CCOFFBB[funcName](t_obj, f_val, f_val2, b1, b2);
}
void GameData::communication(string funcName)
{
	CCAssert(V_V.find(funcName) != V_V.end(), funcName.c_str());
	V_V[funcName]();
	return;
}

void GameData::communication(string funcName, CCPoint t_p, int t_i)
{
	CCAssert(V_CCPI.find(funcName) != V_CCPI.end(), funcName.c_str());
	V_CCPI[funcName](t_p, t_i);
	return;
}

void GameData::communication(string funcName, CCPoint t_p, int t_i1, int t_i2)
{
	CCAssert(V_CCPII.find(funcName) != V_CCPII.end(), funcName.c_str());
	V_CCPII[funcName](t_p, t_i1, t_i2);
	return;
}

void GameData::communication(string funcName, float t_f, bool t_b, CCPoint t_p)
{
	CCAssert(V_FBCCP.find(funcName) != V_FBCCP.end(), funcName.c_str());
	V_FBCCP[funcName](t_f, t_b, t_p);
	return;
}

void GameData::communication(string funcName, bool t_b)
{
	CCAssert(V_B.find(funcName) != V_B.end(), funcName.c_str());
	V_B[funcName](t_b);
	return;
}

void GameData::communication(string funcName, CCObject* t_obj)
{
	CCAssert(V_CCO.find(funcName) != V_CCO.end(), funcName.c_str());
	V_CCO[funcName](t_obj);
	return;
}

void GameData::communication(string funcName, IntPointVector t_addPath)
{
	CCAssert(V_Ipv.find(funcName) != V_Ipv.end(), funcName.c_str());
	V_Ipv[funcName](t_addPath);
	return;
}

void GameData::communication(string funcName, IntPoint t_p)
{
	CCAssert(V_Ip.find(funcName) != V_Ip.end(), funcName.c_str());
	V_Ip[funcName](t_p);
	return;
}

void GameData::communication(string funcName, IntRect t_r)
{
	CCAssert(V_Ir.find(funcName) != V_Ir.end(), funcName.c_str());
	V_Ir[funcName](t_r);
	return;
}

void GameData::communication(string funcName, IntPoint t_p, int t_i1, int t_i2, int t_i3)
{
	CCAssert(V_IpIII.find(funcName) != V_IpIII.end(), funcName.c_str());
	V_IpIII[funcName](t_p, t_i1, t_i2, t_i3);
	return;
}

void GameData::communication(string funcName, IntPoint t_p, CCObject* t_t, SEL_CallFuncI t_d)
{
	CCAssert(V_IpCCOCallfunci.find(funcName) != V_IpCCOCallfunci.end(), funcName.c_str());
	V_IpCCOCallfunci[funcName](t_p, t_t, t_d);
	return;
}

void GameData::communication(string funcName, CCPoint t_p, CCObject* t_target, SEL_CallFuncO d_func)
{
	CCAssert(V_CCPCCOCallfuncO.find(funcName) != V_CCPCCOCallfuncO.end(), funcName.c_str());
	V_CCPCCOCallfuncO[funcName](t_p, t_target, d_func);
	return;
}

void GameData::communication(string funcName, CCPoint t_startFire, ccColor4F t_color, float t_angle)
{
	CCAssert(V_CCPCOLORF.find(funcName) != V_CCPCOLORF.end(), funcName.c_str());
	V_CCPCOLORF[funcName](t_startFire, t_color, t_angle);
	return;
}

void GameData::communication(string funcName, CCPoint t_startFire, bool t_b)
{
	CCAssert(V_CCPB.find(funcName) != V_CCPB.end(), funcName.c_str());
	V_CCPB[funcName](t_startFire, t_b);
	return;
}

void GameData::communication(string funcName, int t_int)
{
	CCAssert(V_I.find(funcName) != V_I.end(), funcName.c_str());
	V_I[funcName](t_int);
	return;
}

void GameData::communication(string funcName, int t_int1, int t_int2)
{
	CCAssert(V_II.find(funcName) != V_II.end(), funcName.c_str());
	V_II[funcName](t_int1, t_int2);
	return;
}

void GameData::communication(string funcName, int t_int1, int t_int2, float t_float)
{
	CCAssert(V_IIF.find(funcName) != V_IIF.end(), funcName.c_str());
	V_IIF[funcName](t_int1, t_int2, t_float);
	return;
}

void GameData::communication(string funcName, int t_int1, int t_int2, float t_float, CCPoint t_p)
{
	CCAssert(V_IIFCCP.find(funcName) != V_IIFCCP.end(), funcName.c_str());
	V_IIFCCP[funcName](t_int1, t_int2, t_float, t_p);
	return;
}

void GameData::communication(string funcName, float t_float)
{
	CCAssert(V_F.find(funcName) != V_F.end(), funcName.c_str());
	V_F[funcName](t_float);
	return;
}

void GameData::communication(string funcName, float t_float, bool t_bool)
{
	CCAssert(V_FB.find(funcName) != V_FB.end(), funcName.c_str());
	V_FB[funcName](t_float, t_bool);
	return;
}

void GameData::communication(string funcName, CCPoint t_p)
{
	CCAssert(V_CCP.find(funcName) != V_CCP.end(), funcName.c_str());
	V_CCP[funcName](t_p);
	return;
}

void GameData::communication(string funcName, CCObject* t_t1, SEL_CallFunc t_d1, CCObject* t_t2, SEL_CallFunc t_d2)
{
	CCAssert(V_TDTD.find(funcName) != V_TDTD.end(), funcName.c_str());
	V_TDTD[funcName](t_t1, t_d1, t_t2, t_d2);
	return;
}

void GameData::communication(string funcName, string t_str)
{
	CCAssert(V_Str.find(funcName) != V_Str.end(), funcName.c_str());
	V_Str[funcName](t_str);
	return;
}

void GameData::changeJackBaseSpeed( float t_s )
{
	jack_base_speed = t_s;
	F_V["Jack_getSpeedUpValue"]();
	F_V["Jack_getAlphaSpeed"]();
	V_F["Jack_changeSpeed"](jack_base_speed + F_V["Jack_getSpeedUpValue"]() +
		F_V["Jack_getAlphaSpeed"]());
}

void GameData::setInitRect( IntPoint initPoint, IntSize initSize )
{
	initPoint.x += mapWidthInnerBegin;
	initPoint.y += mapHeightInnerBegin;
	for(int i=initPoint.x;i<initPoint.x+initSize.width;i++)
	{
		for(int j=initPoint.y;j<initPoint.y+initSize.height;j++)
		{
			bool is_setted = false;
			if(j < myGD->limited_step_bottom)
				continue;
			else if(j == myGD->limited_step_bottom)
			{
				mapState[i][j] = mapOldline;
				is_setted = true;
			}
			
			if(j > myGD->limited_step_top)
				continue;
			else if(j == myGD->limited_step_top)
			{
				mapState[i][j] = mapOldline;
				is_setted = true;
			}
			
			if(!is_setted)
			{
				if(i == initPoint.x || j == initPoint.y || i == initPoint.x+initSize.width-1 || j == initPoint.y+initSize.height-1)
					mapState[i][j] = mapOldline;
				else
					mapState[i][j] = mapOldget;
			}
		}
	}
}

void GameData::removeMapNewline()
{
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(mapState[i][j] == mapNewline)
				mapState[i][j] = mapEmpty;
		}
	}
}

void GameData::setJackPoint( IntPoint t_jp )
{
	if(t_jp.isNull())
		return;
	jackPoint->x = t_jp.x;
	jackPoint->y = t_jp.y;
}

IntPoint GameData::getJackPoint()
{
	IntPoint r_p = IntPoint(jackPoint->x, jackPoint->y);
	return r_p;
}


void GameData::setMainCumberPoint(KSCumberBase* cb, IntPoint t_mcbp)
{
	mainCumberPoints[cb].x = t_mcbp.x;
	mainCumberPoints[cb].y = t_mcbp.y;
}
int GameData::getCommunication( string funcName )
{
	return I_V[funcName]();
}

bool GameData::getCommunicationBool( string funcName )
{
	return B_V[funcName ]();
}

bool GameData::getCommunicationBool( string funcName, IntPoint t_p)
{
	return B_Ip[funcName](t_p);
}

float GameData::getAlphaSpeed()
{
	return F_V["Jack_getAlphaSpeed"]();

	//		return NULL;
}

void GameData::setAlphaSpeed( float t_f )
{
	V_F["Jack_setAlphaSpeed"](t_f);
}

void GameData::setJackSpeed( float t_s )
{
	V_F["Jack_changeSpeed"](t_s);
	//		(target_Jack->*delegate_Jack_setJackSpeed)(t_s);
}

void GameData::setJackState( int t_s )
{
	jackState = t_s;
}

int GameData::getJackState()
{
	return jackState;
}

void GameData::setIsGameover( bool t_b )
{
	isGameover = t_b;
}

bool GameData::getIsGameover()
{
	return isGameover;
}

SetMapType GameData::getRecentMapType()
{
	return recent_map_type;
}

void GameData::initUserSelectedStartRect( IntRect t_rect )
{
	float scale_value = NSDS_GD(mySD->getSilType(), kSDS_SI_scale_d);
	if(scale_value < 0.1f)
		scale_value = 1.f;
	CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
	jack_base_speed = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_speed_d, t_history.characterIndex.getV(), t_history.characterLevel.getV());//*scale_value;
	
	if(mySGD->isUsingItem(kIC_baseSpeedUp))
	{
		int t_unit = mySD->getBaseSpeedUpItemOption();
		jack_base_speed += t_unit*0.1f;
	}
	
	game_scale = (720.f*scale_value-boarder_value*2)/(320.f);
	setInitRect(t_rect.origin, t_rect.size);
}

void GameData::setUItype( GAMESCREEN_TYPE t_type )
{
	if(t_type != myDSH->getIntegerForKey(kDSH_Key_uiType))
		myDSH->setIntegerForKey(kDSH_Key_uiType, t_type);
	gamescreen_type = t_type;
}

bool GameData::getEmptyRandomPoint(IntPoint* point, float radius)
{
	bool isGoodPointed = false;
	
	IntPoint mapPoint;
	std::vector<IntPoint> shuffledPositions;
	for(int x = 1; x <= mapLoopRange::mapWidthInnerEnd - 1; x++)
	{
		for(int y = 1; y <= mapLoopRange::mapHeightInnerEnd - 1; y++)
		{
			if(mapState[x][y] == mapType::mapEmpty)
			{
				shuffledPositions.push_back(IntPoint(x, y));
			}
		}
	}
	
	random_shuffle(shuffledPositions.begin(), shuffledPositions.end(), [=](int n){
		//return 1;
		return ks19937::getIntValue(0, n-1);
	});
	for(auto& mp : shuffledPositions)
	{
		mapPoint = mp;
		
		float myScale = 1.f;
		if(mapPoint.isInnerMap() && mapState[mapPoint.x][mapPoint.y] == mapEmpty)
		{
			float half_distance = radius*myScale; // 20.f : radius for base scale 1.f
			float calc_distance;
			IntPoint check_position;
			
			bool is_not_position = false;
			
			for(int i=mapPoint.x-half_distance/2;i<=mapPoint.x+half_distance/2 && !is_not_position;i++)
			{
				for(int j=mapPoint.y-half_distance/2;j<=mapPoint.y+half_distance/2 && !is_not_position;j++)
				{
					calc_distance = sqrtf(powf((mapPoint.x - i)*pixelSize,2) + powf((mapPoint.y - j)*pixelSize, 2));
					if(calc_distance < half_distance)
					{
						check_position = IntPoint(i,j);
						if(!check_position.isInnerMap())
						{
							is_not_position = true;
						}
					}
				}
			}
			if(!is_not_position)
			{
				isGoodPointed = true;
				break;
			}
		}
	}
	
	if(isGoodPointed == true)
	{
		point->x = mapPoint.x;
		point->y = mapPoint.y;
	}
	
	return isGoodPointed;
}
void GameData::myInit()
{
	boarder_value = 7.f;
	setUItype( GAMESCREEN_TYPE( myDSH->getIntegerForKey(kDSH_Key_uiType) ) );

	// init map
	for(int i=mapWidthOutlineBegin;i<mapWidthOutlineEnd;i++)
	{
		for(int j=mapHeightOutlineBegin;j<mapHeightOutlineEnd;j++)
		{
			if(i == mapWidthOutlineBegin || j == mapHeightOutlineBegin || i == mapWidthOutlineEnd-1 || j == mapHeightOutlineEnd-1)
				mapState[i][j] = mapOutline;
			else
				mapState[i][j] = mapEmpty;
		}
	}
	jackPoint = new IntPoint();
	mainCumberPoint = new IntPoint();
	otherTargetPoints = new CCArray(1);
	jackState = 0; // jackStateNormal
	m_jackIsUnbeatable = false;
	CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
	jack_base_speed = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_speed_d, t_history.characterIndex.getV(), t_history.characterLevel.getV());
//	int speed_tag = kJackBaseSpeedTag_level1;//myDSH->getIntegerForKey(kDSH_Key_jackBaseSpeed);
//	if(speed_tag == kJackBaseSpeedTag_level2)
//		jack_base_speed = 1.5f;
//	else if(speed_tag == kJackBaseSpeedTag_level3)
//		jack_base_speed = 2.f;
//	else
//		jack_base_speed = 1.2f;
}

void GameData::setStartMap( SetMapType set_map_type )
{
	recent_map_type = set_map_type;
	if(set_map_type == kSMT_side)
	{
		IntSize maxSize = IntSize(50,50);
		IntSize minSize = IntSize(20,20);

		IntSize initSize;
		initSize.width = rand()%(maxSize.width-minSize.width + 1) + minSize.width; // 20 <= width <= 83
		initSize.height = rand()%(maxSize.height-minSize.height + 1) + minSize.height; // 20 <= height <= 83

		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-initSize.width-2-mapWidthInnerBegin, mapHeightInnerEnd-initSize.height-2-mapHeightInnerBegin);

		IntPoint initPoint;
		initPoint.x = rand()%maxPoint.x;
		initPoint.y = rand()%maxPoint.y;

		setInitRect(initPoint, initSize);

		for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
		{
			mapState[i][mapHeightInnerBegin] = mapOldline;
			mapState[i][mapHeightInnerEnd-1] = mapOldline;
		}
		for(int i=mapHeightInnerBegin+1;i<mapHeightInnerEnd-1;i++)
		{
			mapState[mapWidthInnerBegin][i] = mapOldline;
			mapState[mapWidthInnerEnd-1][i] = mapOldline;
		}
	}
	else if(set_map_type == kSMT_randRect)
	{
		setInitRect(IntPoint(20,20), IntSize(20,20));
		setInitRect(IntPoint(120,24), IntSize(20,20));
		setInitRect(IntPoint(70,50), IntSize(20,20));
		setInitRect(IntPoint(10,94), IntSize(20,20));
		setInitRect(IntPoint(130,100), IntSize(20,20));
		setInitRect(IntPoint(65,142), IntSize(20,20));
		setInitRect(IntPoint(30,180), IntSize(20,20));
		setInitRect(IntPoint(115,177), IntSize(20,20));
	}
	else if(set_map_type == kSMT_dotLine)
	{
		IntSize maxSize = IntSize(50,50);
		IntSize minSize = IntSize(20,20);

		IntSize initSize;
		initSize.width = rand()%(maxSize.width-minSize.width + 1) + minSize.width; // 20 <= width <= 83
		initSize.height = rand()%(maxSize.height-minSize.height + 1) + minSize.height; // 20 <= height <= 83

		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-initSize.width-2-mapWidthInnerBegin, mapHeightInnerEnd-initSize.height-2-mapHeightInnerBegin);

		IntPoint initPoint;
		initPoint.x = rand()%maxPoint.x;
		initPoint.y = rand()%maxPoint.y;

		setInitRect(initPoint, initSize);

		for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
		{
			if((i-1)%30 > 14)
			{
				mapState[i][mapHeightInnerBegin] = mapOldline;
				mapState[i][mapHeightInnerEnd-1] = mapOldline;
			}
		}
		for(int i=mapHeightInnerBegin+1;i<mapHeightInnerEnd-1;i++)
		{
			if((i-1)%30 > 14)
			{
				mapState[mapWidthInnerBegin][i] = mapOldline;
				mapState[mapWidthInnerEnd-1][i] = mapOldline;
			}
		}
	}
	else if(set_map_type == kSMT_fixRect)
	{
		setInitRect(IntPoint(0,0), IntSize(20,20));
		setInitRect(IntPoint(70,0), IntSize(20,20));
		setInitRect(IntPoint(140,0), IntSize(20,20));
		setInitRect(IntPoint(0,97), IntSize(20,20));
		setInitRect(IntPoint(70,97), IntSize(20,20));
		setInitRect(IntPoint(140,97), IntSize(20,20));
		setInitRect(IntPoint(0,195), IntSize(20,20));
		setInitRect(IntPoint(70,195), IntSize(20,20));
		setInitRect(IntPoint(140,195), IntSize(20,20));
	}
	else if(set_map_type == kSMT_leftRight)
	{
		IntSize maxSize = IntSize(50,50);
		IntSize minSize = IntSize(20,20);

		IntSize initSize;
		initSize.width = rand()%(maxSize.width-minSize.width + 1) + minSize.width; // 20 <= width <= 83
		initSize.height = rand()%(maxSize.height-minSize.height + 1) + minSize.height; // 20 <= height <= 83

		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-initSize.width-2-mapWidthInnerBegin, mapHeightInnerEnd-initSize.height-2-mapHeightInnerBegin);

		IntPoint initPoint;
		initPoint.x = rand()%maxPoint.x;
		initPoint.y = rand()%maxPoint.y;

		setInitRect(initPoint, initSize);

		for(int i=mapHeightInnerBegin;i<mapHeightInnerEnd;i++)
		{
			mapState[mapWidthInnerBegin][i] = mapOldline;
			mapState[mapWidthInnerEnd-1][i] = mapOldline;
		}
	}
	else if(set_map_type == kSMT_topBottom)
	{
		IntSize maxSize = IntSize(50,50);
		IntSize minSize = IntSize(20,20);

		IntSize initSize;
		initSize.width = rand()%(maxSize.width-minSize.width + 1) + minSize.width; // 20 <= width <= 83
		initSize.height = rand()%(maxSize.height-minSize.height + 1) + minSize.height; // 20 <= height <= 83

		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-initSize.width-2-mapWidthInnerBegin, mapHeightInnerEnd-initSize.height-2-mapHeightInnerBegin);

		IntPoint initPoint;
		initPoint.x = rand()%maxPoint.x;
		initPoint.y = rand()%maxPoint.y;

		setInitRect(initPoint, initSize);

		for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
		{
			mapState[i][mapHeightInnerBegin] = mapOldline;
			mapState[i][mapHeightInnerEnd-1] = mapOldline;
		}
	}
	else if(set_map_type == kSMT_cross)
	{
		for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
		{
			if(i <= 80-40 || i >= 80+40)
				mapState[i][107] = mapOldline;
		}
		for(int i=mapHeightInnerBegin;i<mapHeightInnerEnd;i++)
		{
			if(i <= 107-53 || i >= 107+53)
				mapState[80][i] = mapOldline;
		}

		IntSize maxSize = IntSize(30,40);
		IntSize minSize = IntSize(20,20);

		IntSize initSize;
		initSize.width = rand()%(maxSize.width-minSize.width + 1) + minSize.width; // 20 <= width <= 83
		initSize.height = rand()%(maxSize.height-minSize.height + 1) + minSize.height; // 20 <= height <= 83

		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-initSize.width-2-mapWidthInnerBegin, mapHeightInnerEnd-initSize.height-2-mapHeightInnerBegin);

		IntPoint initPoint;
		initPoint.x = rand()%maxPoint.x;
		initPoint.y = rand()%maxPoint.y;

		setInitRect(initPoint, initSize);
	}
	else if(set_map_type == kSMT_oneRect)
	{
		IntSize maxSize = IntSize(50,50);
		IntSize minSize = IntSize(20,20);

		IntSize initSize;
		initSize.width = rand()%(maxSize.width-minSize.width + 1) + minSize.width; // 20 <= width <= 83
		initSize.height = rand()%(maxSize.height-minSize.height + 1) + minSize.height; // 20 <= height <= 83

		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-initSize.width-2-mapWidthInnerBegin, mapHeightInnerEnd-initSize.height-2-mapHeightInnerBegin);

		IntPoint initPoint;
		initPoint.x = rand()%maxPoint.x;
		initPoint.y = rand()%maxPoint.y;

		setInitRect(initPoint, initSize);
	}
	else if(set_map_type == kSMT2_sLine)
	{
		int height1 = 70, height2 = 140;
		if(rand()%2 == 0)
		{
			height1 = 140;
			height2 = 70;
		}

		for(int i=mapWidthInnerBegin;i<mapWidthInnerBegin+60;i++)
			mapState[i][height1] = mapOldline;
		for(int i=mapWidthInnerEnd-1;i>mapWidthInnerEnd-1-60;i--)
			mapState[i][height2] = mapOldline;

		IntSize maxSize = IntSize(50,50);
		IntSize minSize = IntSize(20,20);

		IntSize initSize;
		initSize.width = rand()%(maxSize.width-minSize.width + 1) + minSize.width; // 20 <= width <= 83
		initSize.height = rand()%(maxSize.height-minSize.height + 1) + minSize.height; // 20 <= height <= 83

		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-initSize.width-2-mapWidthInnerBegin, mapHeightInnerEnd-initSize.height-2-mapHeightInnerBegin);

		IntPoint initPoint;
		initPoint.x = rand()%maxPoint.x;
		initPoint.y = rand()%maxPoint.y;

		setInitRect(initPoint, initSize);
	}
	else if(set_map_type == kSMT2_cornerPrison)
	{
		for(int i=mapWidthInnerBegin;i<mapWidthInnerBegin+50;i++) // (topleft -> top) and (bottomleft -> bottom)
		{
			mapState[i][mapHeightInnerEnd-1] = mapOldline;
			mapState[i][mapHeightInnerBegin] = mapOldline;
		}
		for(int i=mapWidthInnerEnd-1;i>mapWidthInnerEnd-1-50;i--) // (topright -> top) and (bottomright -> bottom)
		{
			mapState[i][mapHeightInnerBegin] = mapOldline;
			mapState[i][mapHeightInnerEnd-1] = mapOldline;
		}
		for(int j=mapHeightInnerEnd-1;j>mapHeightInnerEnd-1-50;j--) // (topleft -> left) and (topright -> right)
		{
			mapState[mapWidthInnerBegin][j] = mapOldline;
			mapState[mapWidthInnerEnd-1][j] = mapOldline;
		}
		for(int j=mapHeightInnerBegin;j<mapHeightInnerBegin+50;j++) // (bottomleft -> left) and (bottomright -> right)
		{
			mapState[mapWidthInnerBegin][j] = mapOldline;
			mapState[mapWidthInnerEnd-1][j] = mapOldline;
		}
		for(int i=mapWidthInnerBegin;i<mapWidthInnerBegin+30;i++) // (topleft -> bottom) and (bottomleft -> top)
		{
			mapState[i][mapHeightInnerEnd-1-50] = mapOldline;
			mapState[i][mapHeightInnerBegin+50] = mapOldline;
		}
		for(int i=mapWidthInnerEnd-1;i>mapWidthInnerEnd-1-30;i--) // (topright -> bottom) and (bottomright -> top)
		{
			mapState[i][mapHeightInnerEnd-1-50] = mapOldline;
			mapState[i][mapHeightInnerBegin+50] = mapOldline;
		}
		for(int j=mapHeightInnerEnd-1;j>mapHeightInnerEnd-1-30;j--) // (topleft -> right) and (topright -> left)
		{
			mapState[mapWidthInnerBegin+50][j] = mapOldline;
			mapState[mapWidthInnerEnd-1-50][j] = mapOldline;
		}
		for(int j=mapHeightInnerBegin;j<mapHeightInnerBegin+30;j++) // (bottomleft -> right) and (bottomright -> left)
		{
			mapState[mapWidthInnerBegin+50][j] = mapOldline;
			mapState[mapWidthInnerEnd-1-50][j] = mapOldline;
		}

		IntSize maxSize = IntSize(50,50);
		IntSize minSize = IntSize(20,20);

		IntSize initSize;
		initSize.width = rand()%(maxSize.width-minSize.width + 1) + minSize.width; // 20 <= width <= 83
		initSize.height = rand()%(maxSize.height-minSize.height + 1) + minSize.height; // 20 <= height <= 83

		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-initSize.width-2-mapWidthInnerBegin, mapHeightInnerEnd-initSize.height-2-mapHeightInnerBegin);

		IntPoint initPoint;
		initPoint.x = rand()%maxPoint.x;
		initPoint.y = rand()%maxPoint.y;

		setInitRect(initPoint, initSize);
	}
	else if(set_map_type == kSMT2_centerPrison)
	{
		for(int i=mapWidthInnerBegin+40;i<=mapWidthInnerEnd-1-40;i++)
		{
			if(i >= 80-10 && i <= 80+10)	continue;
			mapState[i][mapHeightInnerBegin+60] = mapOldline;
			mapState[i][mapHeightInnerEnd-1-60] = mapOldline;
		}
		for(int j=mapHeightInnerBegin+60;j<mapHeightInnerEnd-1-60;j++)
		{
			if(j >= 108-15 && j <= 108+15)	continue;
			mapState[mapWidthInnerBegin+40][j] = mapOldline;
			mapState[mapWidthInnerEnd-1-40][j] = mapOldline;
		}

		IntSize maxSize = IntSize(50,50);
		IntSize minSize = IntSize(20,20);

		IntSize initSize;
		initSize.width = rand()%(maxSize.width-minSize.width + 1) + minSize.width; // 20 <= width <= 83
		initSize.height = rand()%(maxSize.height-minSize.height + 1) + minSize.height; // 20 <= height <= 83

		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-initSize.width-2-mapWidthInnerBegin, mapHeightInnerEnd-initSize.height-2-mapHeightInnerBegin);

		IntPoint initPoint;
		initPoint.x = rand()%maxPoint.x;
		initPoint.y = rand()%maxPoint.y;

		setInitRect(initPoint, initSize);
	}
	else if(set_map_type == kSMT2_dddPrison)
	{
		int random_value = rand()%4; // 0 : top , 1 : bottom , 2 : left , 3 : right
		for(int i=mapWidthInnerBegin+15;i<=mapWidthInnerBegin+15+60;i++)
		{
			if(random_value != 0)				mapState[i][mapHeightInnerEnd-1-28] = mapOldline;
			if(random_value != 1)				mapState[i][mapHeightInnerEnd-1-28-60] = mapOldline;
		}

		for(int j=mapHeightInnerEnd-1-28;j>mapHeightInnerEnd-1-28-60;j--)
		{
			if(random_value != 2)				mapState[mapWidthInnerBegin+15][j] = mapOldline;
			if(random_value != 3)				mapState[mapWidthInnerBegin+15+60][j] = mapOldline;
		}

		random_value = rand()%4;
		for(int i=mapWidthInnerBegin+15+60+20;i<=mapWidthInnerBegin+15+60+20+60;i++)
		{
			if(random_value != 0)				mapState[i][mapHeightInnerEnd-1-28-60+10] = mapOldline;
			if(random_value != 1)				mapState[i][mapHeightInnerEnd-1-28-60+10-60] = mapOldline;
		}
		for(int j=mapHeightInnerEnd-1-28-60+10;j>mapHeightInnerEnd-1-28-60+10-60;j--)
		{
			if(random_value != 2)				mapState[mapWidthInnerBegin+15+60+20][j] = mapOldline;
			if(random_value != 3)				mapState[mapWidthInnerBegin+15+60+20+60][j] = mapOldline;
		}

		random_value = rand()%4;
		for(int i=mapWidthInnerBegin+40;i<=mapWidthInnerBegin+40+60;i++)
		{
			if(random_value != 1)				mapState[i][mapHeightInnerBegin+30] = mapOldline;
			if(random_value != 0)				mapState[i][mapHeightInnerBegin+30+60] = mapOldline;
		}
		for(int j=mapHeightInnerBegin+30;j<mapHeightInnerBegin+30+60;j++)
		{
			if(random_value != 2)				mapState[mapWidthInnerBegin+40][j] = mapOldline;
			if(random_value != 3)				mapState[mapWidthInnerBegin+40+60][j] = mapOldline;
		}

		IntSize maxSize = IntSize(50,50);
		IntSize minSize = IntSize(20,20);

		IntSize initSize;
		initSize.width = rand()%(maxSize.width-minSize.width + 1) + minSize.width; // 20 <= width <= 83
		initSize.height = rand()%(maxSize.height-minSize.height + 1) + minSize.height; // 20 <= height <= 83

		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-initSize.width-2-mapWidthInnerBegin, mapHeightInnerEnd-initSize.height-2-mapHeightInnerBegin);

		IntPoint initPoint;
		initPoint.x = rand()%maxPoint.x;
		initPoint.y = rand()%maxPoint.y;

		setInitRect(initPoint, initSize);
	}
	else if(set_map_type == kSMT2_oneLineCenter)
	{
		int random_value = rand()%4; // 0 : top , 1 : bottom , 2 : left , 3 : right
		if(random_value == 0)
		{
			for(int j=mapHeightInnerEnd-1;j>mapHeightInnerEnd-1-150;j--)
				mapState[80][j] = mapOldline;
		}
		else if(random_value == 1)
		{
			for(int j=mapHeightInnerBegin;j<mapHeightInnerBegin+150;j++)
				mapState[80][j] = mapOldline;
		}
		else if(random_value == 2)
		{
			for(int i=mapWidthInnerBegin;i<mapWidthInnerBegin+100;i++)
				mapState[i][108] = mapOldline;
		}
		else if(random_value == 3)
		{
			for(int i=mapWidthInnerEnd-1;i>mapWidthInnerEnd-1-100;i--)
				mapState[i][108] = mapOldline;
		}

		IntSize maxSize = IntSize(50,50);
		IntSize minSize = IntSize(20,20);

		IntSize initSize;
		initSize.width = rand()%(maxSize.width-minSize.width + 1) + minSize.width; // 20 <= width <= 83
		initSize.height = rand()%(maxSize.height-minSize.height + 1) + minSize.height; // 20 <= height <= 83

		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-initSize.width-2-mapWidthInnerBegin, mapHeightInnerEnd-initSize.height-2-mapHeightInnerBegin);

		IntPoint initPoint;
		initPoint.x = rand()%maxPoint.x;
		initPoint.y = rand()%maxPoint.y;

		setInitRect(initPoint, initSize);
	}
	else if(set_map_type == kSMT2_centerCross)
	{
		for(int i=mapWidthInnerBegin+40;i<mapWidthInnerEnd-1-40;i++)
			mapState[i][108] = mapOldline;
		for(int j=mapHeightInnerBegin+60;j<mapHeightInnerEnd-1-60;j++)
			mapState[80][j] = mapOldline;

		IntSize maxSize = IntSize(50,50);
		IntSize minSize = IntSize(20,20);

		IntSize initSize;
		initSize.width = rand()%(maxSize.width-minSize.width + 1) + minSize.width; // 20 <= width <= 83
		initSize.height = rand()%(maxSize.height-minSize.height + 1) + minSize.height; // 20 <= height <= 83

		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-initSize.width-2-mapWidthInnerBegin, mapHeightInnerEnd-initSize.height-2-mapHeightInnerBegin);

		IntPoint initPoint;
		initPoint.x = rand()%maxPoint.x;
		initPoint.y = rand()%maxPoint.y;

		setInitRect(initPoint, initSize);
	}
	else if(set_map_type == kSMT2_man)
	{
		for(int i=mapWidthInnerBegin;i<mapWidthInnerBegin+40;i++)
			mapState[i][54] = mapOldline;
		for(int j=mapHeightInnerBegin+54;j<mapHeightInnerBegin+108;j++)
			mapState[40][j] = mapOldline;
		for(int i=mapWidthInnerBegin+80;i<mapWidthInnerBegin+80+40;i++)
			mapState[i][54] = mapOldline;
		for(int j=mapHeightInnerBegin;j<mapHeightInnerBegin+54;j++)
			mapState[120][j] = mapOldline;
		for(int i=mapWidthInnerEnd-1;i>mapWidthInnerEnd-1-40;i--)
			mapState[i][162] = mapOldline;
		for(int j=mapHeightInnerBegin+108;j<mapHeightInnerBegin+108+54;j++)
			mapState[120][j] = mapOldline;
		for(int i=mapWidthInnerBegin+40;i<mapWidthInnerBegin+40+40;i++)
			mapState[i][162] = mapOldline;
		for(int j=mapHeightInnerEnd-1;j>mapHeightInnerEnd-1-54;j--)
			mapState[40][j] = mapOldline;

		IntSize maxSize = IntSize(50,50);
		IntSize minSize = IntSize(20,20);

		IntSize initSize;
		initSize.width = rand()%(maxSize.width-minSize.width + 1) + minSize.width; // 20 <= width <= 83
		initSize.height = rand()%(maxSize.height-minSize.height + 1) + minSize.height; // 20 <= height <= 83

		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-initSize.width-2-mapWidthInnerBegin, mapHeightInnerEnd-initSize.height-2-mapHeightInnerBegin);

		IntPoint initPoint;
		initPoint.x = rand()%maxPoint.x;
		initPoint.y = rand()%maxPoint.y;

		setInitRect(initPoint, initSize);
	}
}
