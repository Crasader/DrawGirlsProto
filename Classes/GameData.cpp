//
//  GameData.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 6..
//
//

#include "GameData.h"

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

GameData* gameData = myGD;

void GameData::resetGameData()
{
	jackState = 0; // normal
	jackPoint->release();
	jackPoint = new IntPoint();
	mainCumberPoint->release();
	mainCumberPoint = new IntPoint();
	otherTargetPoints->removeAllObjects();

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

void GameData::changeJackBaseSpeed( float t_s )
{
	jack_base_speed = t_s;
	F_V["Jack_getSpeedUpValue"]();
	F_V["Jack_getAlphaSpeed"]();
	V_F["Jack_changeSpeed"](jack_base_speed + F_V["Jack_getSpeedUpValue"]() +
		F_V["Jack_getAlphaSpeed"]());
}

void GameData::initStartRect()
{
	int chapter_number = SelectedMapData::sharedInstance()->getSelectedChapter();
	int stage_number = SelectedMapData::sharedInstance()->getSelectedStage();

	float rate_arg;

	if(chapter_number <= 20)
	{
		if(chapter_number%2 == 1)
		{
			if(stage_number == 1)								setStartMap(kSMT_side);
			else if(stage_number == 2)
			{
				if(((chapter_number-1)%10+1)%4 == 1)			setStartMap(kSMT_randRect);
				else if(((chapter_number-1)%10+1)%4 == 3)		setStartMap(kSMT_fixRect);
			}
			else if(stage_number == 3)							setStartMap(kSMT_dotLine);
			else if(stage_number == 4)
			{
				if(((chapter_number-1)%10+1)%4 == 1)			setStartMap(kSMT_fixRect);
				else if(((chapter_number-1)%10+1)%4 == 3)		setStartMap(kSMT_randRect);
			}
			else												setStartMap(kSMT_oneRect);
		}
		else
		{
			if(stage_number == 1)
			{
				if((chapter_number-1)%10+1 == 2 || (chapter_number-1)%10+1 == 8)			setStartMap(kSMT_leftRight);
				else if((chapter_number-1)%10+1 == 4 || (chapter_number-1)%10+1 == 10)		setStartMap(kSMT_topBottom);
				else																		setStartMap(kSMT_cross);
			}
			else if(stage_number == 2)
			{
				if((chapter_number-1)%10+1 == 2 || (chapter_number-1)%10+1 == 10)			setStartMap(kSMT_cross);
				else if((chapter_number-1)%10+1 == 4)										setStartMap(kSMT_leftRight);
				else																		setStartMap(kSMT_topBottom);
			}
			else if(stage_number == 3)														setStartMap(kSMT_oneRect);
			else if(stage_number == 4)
			{
				if((chapter_number-1)%10+1 == 2)											setStartMap(kSMT_topBottom);
				else if((chapter_number-1)%10+1 == 4 || (chapter_number-1)%10+1 == 8)		setStartMap(kSMT_cross);
				else																		setStartMap(kSMT_leftRight);
			}
			else																			setStartMap(kSMT_oneRect);
		}
	}
	else
	{
		int view_chapter_number = chapter_number%10;
		int sub_chapter_number = chapter_number/10 - 1;
		if(view_chapter_number == 1)
		{
			if(sub_chapter_number == 1)
			{
				if(stage_number == 1)				setStartMap(kSMT2_sLine);
				else if(stage_number == 2)			setStartMap(kSMT2_centerCross);
				else if(stage_number == 3)			setStartMap(kSMT2_oneLineCenter);
				else if(stage_number == 4)			setStartMap(kSMT2_centerPrison);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
			else if(sub_chapter_number == 2)
			{
				if(stage_number == 1)				setStartMap(kSMT2_dddPrison);
				else if(stage_number == 2)			setStartMap(kSMT2_cornerPrison);
				else if(stage_number == 3)			setStartMap(kSMT_oneRect);
				else if(stage_number == 4)			setStartMap(kSMT2_man);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
			else if(sub_chapter_number == 3)
			{
				if(stage_number == 1)				setStartMap(kSMT2_sLine);
				else if(stage_number == 2)			setStartMap(kSMT2_dddPrison);
				else if(stage_number == 3)			setStartMap(kSMT2_cornerPrison);
				else if(stage_number == 4)			setStartMap(kSMT2_man);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
		}
		else if(view_chapter_number == 2)
		{
			if(sub_chapter_number == 1)
			{
				if(stage_number == 1)				setStartMap(kSMT2_centerCross);
				else if(stage_number == 2)			setStartMap(kSMT2_sLine);
				else if(stage_number == 3)			setStartMap(kSMT2_oneLineCenter);
				else if(stage_number == 4)			setStartMap(kSMT2_man);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
			else if(sub_chapter_number == 2)
			{
				if(stage_number == 1)				setStartMap(kSMT2_centerPrison);
				else if(stage_number == 2)			setStartMap(kSMT2_dddPrison);
				else if(stage_number == 3)			setStartMap(kSMT_oneRect);
				else if(stage_number == 4)			setStartMap(kSMT2_cornerPrison);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
			else if(sub_chapter_number == 3)
			{
				if(stage_number == 1)				setStartMap(kSMT2_oneLineCenter);
				else if(stage_number == 2)			setStartMap(kSMT2_centerPrison);
				else if(stage_number == 3)			setStartMap(kSMT2_man);
				else if(stage_number == 4)			setStartMap(kSMT2_dddPrison);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
		}
		else if(view_chapter_number == 3)
		{
			if(sub_chapter_number == 1)
			{
				if(stage_number == 1)				setStartMap(kSMT2_oneLineCenter);
				else if(stage_number == 2)			setStartMap(kSMT2_centerCross);
				else if(stage_number == 3)			setStartMap(kSMT2_sLine);
				else if(stage_number == 4)			setStartMap(kSMT2_cornerPrison);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
			else if(sub_chapter_number == 2)
			{
				if(stage_number == 1)				setStartMap(kSMT2_man);
				else if(stage_number == 2)			setStartMap(kSMT2_centerPrison);
				else if(stage_number == 3)			setStartMap(kSMT_oneRect);
				else if(stage_number == 4)			setStartMap(kSMT2_dddPrison);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
			else if(sub_chapter_number == 3)
			{
				if(stage_number == 1)				setStartMap(kSMT2_centerCross);
				else if(stage_number == 2)			setStartMap(kSMT2_sLine);
				else if(stage_number == 3)			setStartMap(kSMT2_cornerPrison);
				else if(stage_number == 4)			setStartMap(kSMT2_man);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
		}
		else if(view_chapter_number == 4)
		{
			if(sub_chapter_number == 1)
			{
				if(stage_number == 1)				setStartMap(kSMT2_centerCross);
				else if(stage_number == 2)			setStartMap(kSMT2_dddPrison);
				else if(stage_number == 3)			setStartMap(kSMT2_cornerPrison);
				else if(stage_number == 4)			setStartMap(kSMT2_oneLineCenter);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
			else if(sub_chapter_number == 2)
			{
				if(stage_number == 1)				setStartMap(kSMT2_sLine);
				else if(stage_number == 2)			setStartMap(kSMT2_centerPrison);
				else if(stage_number == 3)			setStartMap(kSMT_oneRect);
				else if(stage_number == 4)			setStartMap(kSMT2_man);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
			else if(sub_chapter_number == 3)
			{
				if(stage_number == 1)				setStartMap(kSMT2_cornerPrison);
				else if(stage_number == 2)			setStartMap(kSMT2_oneLineCenter);
				else if(stage_number == 3)			setStartMap(kSMT2_centerCross);
				else if(stage_number == 4)			setStartMap(kSMT2_dddPrison);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
		}
		else if(view_chapter_number == 5)
		{
			if(sub_chapter_number == 1)
			{
				if(stage_number == 1)				setStartMap(kSMT2_sLine);
				else if(stage_number == 2)			setStartMap(kSMT2_man);
				else if(stage_number == 3)			setStartMap(kSMT2_oneLineCenter);
				else if(stage_number == 4)			setStartMap(kSMT2_centerCross);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
			else if(sub_chapter_number == 2)
			{
				if(stage_number == 1)				setStartMap(kSMT2_cornerPrison);
				else if(stage_number == 2)			setStartMap(kSMT2_centerPrison);
				else if(stage_number == 3)			setStartMap(kSMT_oneRect);
				else if(stage_number == 4)			setStartMap(kSMT2_dddPrison);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
			else if(sub_chapter_number == 3)
			{
				if(stage_number == 1)				setStartMap(kSMT2_centerCross);
				else if(stage_number == 2)			setStartMap(kSMT2_centerPrison);
				else if(stage_number == 3)			setStartMap(kSMT2_cornerPrison);
				else if(stage_number == 4)			setStartMap(kSMT2_man);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
		}
		else if(view_chapter_number == 6)
		{
			if(sub_chapter_number == 1)
			{
				if(stage_number == 1)				setStartMap(kSMT2_cornerPrison);
				else if(stage_number == 2)			setStartMap(kSMT2_oneLineCenter);
				else if(stage_number == 3)			setStartMap(kSMT2_dddPrison);
				else if(stage_number == 4)			setStartMap(kSMT2_man);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
			else if(sub_chapter_number == 2)
			{
				if(stage_number == 1)				setStartMap(kSMT2_sLine);
				else if(stage_number == 2)			setStartMap(kSMT2_centerPrison);
				else if(stage_number == 3)			setStartMap(kSMT_oneRect);
				else if(stage_number == 4)			setStartMap(kSMT2_centerCross);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
			else if(sub_chapter_number == 3)
			{
				if(stage_number == 1)				setStartMap(kSMT2_sLine);
				else if(stage_number == 2)			setStartMap(kSMT2_dddPrison);
				else if(stage_number == 3)			setStartMap(kSMT2_man);
				else if(stage_number == 4)			setStartMap(kSMT2_cornerPrison);
				else if(stage_number == 5)			setStartMap(kSMT_oneRect);
			}
		}
	}
	float t_arg = 2.5f + SelectedMapData::sharedInstance()->getViewChapterNumber()*0.5f;
	t_arg = t_arg > 8.f ? 8.f : t_arg;
	rate_arg = t_arg * (0.7f + SelectedMapData::sharedInstance()->getViewChapterSubNumber()*0.3f);

	myDSH->setGoldGetRate(myDSH->getGoldGetRate()*5.f/3.f*rate_arg);
}

void GameData::setInitRect( IntPoint initPoint, IntSize initSize )
{
	initPoint.x += mapWidthInnerBegin;
	initPoint.y += mapHeightInnerBegin;
	for(int i=initPoint.x;i<initPoint.x+initSize.width;i++)
	{
		for(int j=initPoint.y;j<initPoint.y+initSize.height;j++)
		{
			if(i == initPoint.x || j == initPoint.y || i == initPoint.x+initSize.width-1 || j == initPoint.y+initSize.height-1)
				mapState[i][j] = mapOldline;
			else
				mapState[i][j] = mapOldget;
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

void GameData::setMainCumberPoint( IntPoint t_mcbp )
{
	mainCumberPoint->x = t_mcbp.x;
	mainCumberPoint->y = t_mcbp.y;
}

IntPoint GameData::getMainCumberPoint()
{
	IntPoint r_p = IntPoint(mainCumberPoint->x, mainCumberPoint->y);
	return r_p;
}

int GameData::getCommunication( string funcName )
{
	return I_V[funcName]();
}

bool GameData::getCommunicationBool( string funcName )
{
	return B_V[funcName ]();
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
	setInitRect(t_rect.origin, t_rect.size);
}

void GameData::setUItype( GAMESCREEN_TYPE t_type )
{
	if(t_type != myDSH->getIntegerForKey(kDSH_Key_uiType))
		myDSH->setIntegerForKey(kDSH_Key_uiType, t_type);
	gamescreen_type = t_type;

	if(gamescreen_type != kGT_full)
		game_scale = (215-boarder_value)/(160.f);
	else
		game_scale = (480.f-boarder_value*2)/(320.f);
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
	jack_base_speed = 1.5f;
}

void GameData::setStartMap( SetMapType set_map_type )
{
	recent_map_type = set_map_type;
	if(set_map_type == kSMT_side)
	{
		myDSH->setGoldGetRate(0.5f);

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
		myDSH->setGoldGetRate(0.7f);

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
		myDSH->setGoldGetRate(0.8f);

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
		myDSH->setGoldGetRate(0.6f);

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
		myDSH->setGoldGetRate(0.6f);

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
		myDSH->setGoldGetRate(0.7f);

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
		myDSH->setGoldGetRate(0.8f);
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
		myDSH->setGoldGetRate(1.f);

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

		//			for(int i=mapWidthInnerBegin;i<mapWidthInnerBegin+80;i++)
		//			{
		//				mapState[i][69] = mapBlock;
		//				mapState[i][70] = mapBlock;
		//				mapState[i][71] = mapBlock;
		//			}
		//			for(int i=mapWidthInnerEnd-1;i>mapWidthInnerEnd-1-80;i--)
		//			{
		//				mapState[i][139] = mapBlock;
		//				mapState[i][140] = mapBlock;
		//				mapState[i][141] = mapBlock;
		//			}
	}
	else if(set_map_type == kSMT2_sLine)
	{
		myDSH->setGoldGetRate(0.7f);

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
		myDSH->setGoldGetRate(0.8f);

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
		myDSH->setGoldGetRate(0.8f);

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
		myDSH->setGoldGetRate(0.9f);

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
		myDSH->setGoldGetRate(0.7f);

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
		myDSH->setGoldGetRate(0.7f);

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
		myDSH->setGoldGetRate(0.8f);

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
