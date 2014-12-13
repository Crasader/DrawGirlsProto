//
//  PlayTutorial.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 3. 19..
//
//

#include "PlayTutorial.h"
#include "DataStorageHub.h"
#include "KSUtil.h"
#include "StageImgLoader.h"
#include "ServerDataSave.h"
#include "StoryManager.h"
#include "StarGoldData.h"
#include "LoadingTipScene.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "LoadingLayer.h"
#include "StyledLabelTTF.h"
#include "TouchSuctionLayer.h"
#include "AchieveNoti.h"
#include "TypingBox.h"
#include "JoystickPositionSelectPopup.h"
#include "ASPopupView.h"
#include "FiveRocksCpp.h"

void TutoPathManager::myInit(function<TutoMapType(IntPoint)> t_getMapData, function<void(IntPoint, TutoMapType)> t_setMapData, int t_height)
{
	getMapData = t_getMapData;
	setMapData = t_setMapData;
	height_value = t_height;
	
	overlap_map.clear();
	overlap_map.resize(240*height_value);
	
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<height_value;j++)
		{
			overlap_map[j*240+i] = 0;
		}
	}
	
	CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
	path_color_value = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_int2_lineColor_i, t_history.characterIndex.getV(), t_history.characterLevel.getV());
//	if(path_color_code == 1)
//		path_color = "life";
//	else if(path_color_code == 2)
//		path_color = "fire";
//	else if(path_color_code == 3)
//		path_color = "water";
//	else if(path_color_code == 4)
//		path_color = "wind";
//	else if(path_color_code == 5)
//		path_color = "lightning";
//	else if(path_color_code == 6)
//		path_color = "plasma";
//	else
		path_color = "empty";
}

void TutoCharacter::changeDirection(IntDirection t_d, IntDirection t_sd)
{
	if(my_point.isNull() || is_controler_backing())
		return;
	
	if((getMapData(my_point) != kTutoMapType_line && getMapData(my_point) != kTutoMapType_newLine) || (
	   getMapData(IntPoint(my_point.x-1, my_point.y)) != kTutoMapType_line && getMapData(IntPoint(my_point.x-1, my_point.y)) != kTutoMapType_newLine &&
	   getMapData(IntPoint(my_point.x+1, my_point.y)) != kTutoMapType_line && getMapData(IntPoint(my_point.x+1, my_point.y)) != kTutoMapType_newLine &&
	   getMapData(IntPoint(my_point.x, my_point.y-1)) != kTutoMapType_line && getMapData(IntPoint(my_point.x, my_point.y-1)) != kTutoMapType_newLine &&
	   getMapData(IntPoint(my_point.x, my_point.y+1)) != kTutoMapType_line && getMapData(IntPoint(my_point.x, my_point.y+1)) != kTutoMapType_newLine))
	{
		escapeCharacter();
	}
	
	IntDirection c_d = directionStop;
	IntDirection c_sd = directionStop;
	if(t_d == directionLeftDown)
	{
		c_d = t_sd;
		if(t_sd == directionLeft)						c_sd = directionDown;
		else if(t_sd == directionDown)					c_sd = directionLeft;
	}
	else if(t_d == directionRightDown)
	{
		c_d = t_sd;
		if(t_sd == directionDown)						c_sd = directionRight;
		else if(t_sd == directionRight)					c_sd = directionDown;
	}
	else if(t_d == directionRightUp)
	{
		c_d = t_sd;
		if(t_sd == directionRight)						c_sd = directionUp;
		else if(t_sd == directionUp)					c_sd = directionRight;
	}
	else if(t_d == directionLeftUp)
	{
		c_d = t_sd;
		if(t_sd == directionLeft)						c_sd = directionUp;
		else if(t_sd == directionUp)					c_sd = directionLeft;
	}
	else
	{
		c_d = t_d;
		c_sd = t_sd;
	}
	
	IntVector d_vector = IntVector::directionVector(c_d);
	IntVector sd_vector = IntVector::directionVector(c_sd);
	
	IntPoint d_point = IntPoint(my_point.x + d_vector.dx, my_point.y + d_vector.dy);
	IntPoint sd_point = IntPoint(my_point.x + sd_vector.dx, my_point.y + sd_vector.dy);
	
	bool d_valid = d_point.x >= 0 && d_point.x < 240 && d_point.y >= 0 && d_point.y <= height_value;
	bool sd_valid = sd_point.x >= 0 && sd_point.x < 240 && sd_point.y >= 0 && sd_point.y <= height_value;
	
	if(c_d != directionStop && d_valid && getMapData(d_point) == kTutoMapType_line)
	{
//		if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
//		{
        line_edge->setVisible(false);
			IntDirection t_direction = c_d;
			
			if(t_direction == directionLeft)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("move_left"))
					character_manager->runAnimationsForSequenceNamed("move_left");
			}
			else if(t_direction == directionRight)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("move_right"))
					character_manager->runAnimationsForSequenceNamed("move_right");
			}
			else if(t_direction == directionUp)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("move_up"))
					character_manager->runAnimationsForSequenceNamed("move_up");
			}
			else if(t_direction == directionDown)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("move_down"))
					character_manager->runAnimationsForSequenceNamed("move_down");
			}
			
			jack_img_direction = t_direction;
//		}
//		else
//		{
//			if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("move"))
//				character_manager->runAnimationsForSequenceNamed("move");
//			
//			if(c_d == directionLeft && jack_img_direction == directionRight)
//			{
//				character_img->setScaleX(-1.f);
//				jack_img_direction = directionLeft;
//			}
//			else if(c_d == directionRight && jack_img_direction == directionLeft)
//			{
//				character_img->setScaleX(1.f);
//				jack_img_direction = directionRight;
//			}
//		}
		
		if(getMapData(my_point) == kTutoMapType_line)
		{
			setCharacterPoint(d_point);
		}
		else if(getMapData(my_point) == kTutoMapType_newLine)
		{
			setCharacterPoint(d_point);
			cleanPath();
			scanMap();
			if((getMapData(my_point) != kTutoMapType_line && getMapData(my_point) != kTutoMapType_newLine) || (
			   getMapData(IntPoint(my_point.x-1, my_point.y)) != kTutoMapType_line && getMapData(IntPoint(my_point.x-1, my_point.y)) != kTutoMapType_newLine &&
			   getMapData(IntPoint(my_point.x+1, my_point.y)) != kTutoMapType_line && getMapData(IntPoint(my_point.x+1, my_point.y)) != kTutoMapType_newLine &&
			   getMapData(IntPoint(my_point.x, my_point.y-1)) != kTutoMapType_line && getMapData(IntPoint(my_point.x, my_point.y-1)) != kTutoMapType_newLine &&
			   getMapData(IntPoint(my_point.x, my_point.y+1)) != kTutoMapType_line && getMapData(IntPoint(my_point.x, my_point.y+1)) != kTutoMapType_newLine))
			{
				escapeCharacter();
			}
		}
	}
	else if(c_d != directionStop && d_valid && isDrawingOn && (getMapData(d_point) == kTutoMapType_empty || getMapData(d_point) == kTutoMapType_newLine))
	{
//		if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
//		{
        line_edge->setVisible(true);
			IntDirection t_direction = c_d;
			
			if(t_direction == directionLeft)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("draw_left"))
					character_manager->runAnimationsForSequenceNamed("draw_left");
			}
			else if(t_direction == directionRight)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("draw_right"))
					character_manager->runAnimationsForSequenceNamed("draw_right");
			}
			else if(t_direction == directionUp)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("draw_up"))
					character_manager->runAnimationsForSequenceNamed("draw_up");
			}
			else if(t_direction == directionDown)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("draw_down"))
					character_manager->runAnimationsForSequenceNamed("draw_down");
			}
			
			jack_img_direction = t_direction;
//		}
//		else
//		{
//			if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("draw"))
//				character_manager->runAnimationsForSequenceNamed("draw");
//			
//			if(c_d == directionLeft && jack_img_direction == directionRight)
//			{
//				character_img->setScaleX(-1.f);
//				jack_img_direction = directionLeft;
//			}
//			else if(c_d == directionRight && jack_img_direction == directionLeft)
//			{
//				character_img->setScaleX(1.f);
//				jack_img_direction = directionRight;
//			}
//		}
		
		addPath(IntPointVector(my_point.x, my_point.y, d_vector.dx, d_vector.dy));
		setCharacterPoint(d_point);
		setMapData(d_point, kTutoMapType_newLine);
	}
	else if(c_sd != directionStop && sd_valid && getMapData(sd_point) == kTutoMapType_line)
	{
//		if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
//		{
        line_edge->setVisible(false);
			IntDirection t_direction = c_sd;
			
			if(t_direction == directionLeft)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("move_left"))
					character_manager->runAnimationsForSequenceNamed("move_left");
			}
			else if(t_direction == directionRight)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("move_right"))
					character_manager->runAnimationsForSequenceNamed("move_right");
			}
			else if(t_direction == directionUp)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("move_up"))
					character_manager->runAnimationsForSequenceNamed("move_up");
			}
			else if(t_direction == directionDown)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("move_down"))
					character_manager->runAnimationsForSequenceNamed("move_down");
			}
			
			jack_img_direction = t_direction;
//		}
//		else
//		{
//			if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("move"))
//				character_manager->runAnimationsForSequenceNamed("move");
//			
//			if(c_sd == directionLeft && jack_img_direction == directionRight)
//			{
//				character_img->setScaleX(-1.f);
//				jack_img_direction = directionLeft;
//			}
//			else if(c_sd == directionRight && jack_img_direction == directionLeft)
//			{
//				character_img->setScaleX(1.f);
//				jack_img_direction = directionRight;
//			}
//		}
		
		if(getMapData(my_point) == kTutoMapType_line)
		{
			setCharacterPoint(sd_point);
		}
		else if(getMapData(my_point) == kTutoMapType_newLine)
		{
			setCharacterPoint(sd_point);
			cleanPath();
			scanMap();
			if((getMapData(my_point) != kTutoMapType_line && getMapData(my_point) != kTutoMapType_newLine) || (
			   getMapData(IntPoint(my_point.x, my_point.y)) != kTutoMapType_line && getMapData(IntPoint(my_point.x, my_point.y)) != kTutoMapType_newLine &&
			   getMapData(IntPoint(my_point.x-1, my_point.y)) != kTutoMapType_line && getMapData(IntPoint(my_point.x-1, my_point.y)) != kTutoMapType_newLine &&
			   getMapData(IntPoint(my_point.x+1, my_point.y)) != kTutoMapType_line && getMapData(IntPoint(my_point.x+1, my_point.y)) != kTutoMapType_newLine &&
			   getMapData(IntPoint(my_point.x, my_point.y-1)) != kTutoMapType_line && getMapData(IntPoint(my_point.x, my_point.y-1)) != kTutoMapType_newLine &&
			   getMapData(IntPoint(my_point.x, my_point.y+1)) != kTutoMapType_line && getMapData(IntPoint(my_point.x, my_point.y+1)) != kTutoMapType_newLine))
			{
				escapeCharacter();
			}
		}
	}
	else if(c_sd != directionStop && sd_valid && isDrawingOn && (getMapData(sd_point) == kTutoMapType_empty || getMapData(sd_point) == kTutoMapType_newLine))
	{
//		if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
//		{
        line_edge->setVisible(true);
			IntDirection t_direction = c_sd;
			
			if(t_direction == directionLeft)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("draw_left"))
					character_manager->runAnimationsForSequenceNamed("draw_left");
			}
			else if(t_direction == directionRight)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("draw_right"))
					character_manager->runAnimationsForSequenceNamed("draw_right");
			}
			else if(t_direction == directionUp)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("draw_up"))
					character_manager->runAnimationsForSequenceNamed("draw_up");
			}
			else if(t_direction == directionDown)
			{
				if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("draw_down"))
					character_manager->runAnimationsForSequenceNamed("draw_down");
			}
			
			jack_img_direction = t_direction;
//		}
//		else
//		{
//			if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("draw"))
//				character_manager->runAnimationsForSequenceNamed("draw");
//			
//			if(c_sd == directionLeft && jack_img_direction == directionRight)
//			{
//				character_img->setScaleX(-1.f);
//				jack_img_direction = directionLeft;
//			}
//			else if(c_sd == directionRight && jack_img_direction == directionLeft)
//			{
//				character_img->setScaleX(1.f);
//				jack_img_direction = directionRight;
//			}
//		}
		
		addPath(IntPointVector(my_point.x, my_point.y, sd_vector.dx, sd_vector.dy));
		setCharacterPoint(sd_point);
		setMapData(sd_point, kTutoMapType_newLine);
	}
	else
	{
		if(!isDrawingOn)
		{
            line_edge->setVisible(false);
			if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("stop"))
			{
				character_manager->runAnimationsForSequenceNamed("stop");
//				if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
					jack_img_direction = directionStop;
			}
		}
		else
		{
			if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("draw_stop"))
			{
				character_manager->runAnimationsForSequenceNamed("draw_stop");
//				if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
					jack_img_direction = directionStop;
			}
		}
	}
}

void TutoCharacter::escapeCharacter()
{
	queue<IntMoveState> bfsArray;
	bfsArray.push(IntMoveState(my_point.x, my_point.y, directionStop));
	
	bool isFinded = false;
	IntPoint findedPoint;
	
	while(!bfsArray.empty() && !isFinded)
	{
		IntMoveState t_ms = bfsArray.front();
		bfsArray.pop();
		if(getMapData(t_ms.origin) == kTutoMapType_line)
		{
			isFinded = true;
			findedPoint = t_ms.origin;
		}
		else
		{
			if(t_ms.direction == directionStop)
			{
				for(int i = directionLeftUp;i <= directionUp; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionLeftUp)
			{
				for(int i = directionLeftUp;i <= directionLeft; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);
				}
				
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionLeft)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x-1, t_ms.origin.y, directionLeft);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionLeftDown)
			{
				for(int i = directionLeft;i <= directionDown; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionDown)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y-1, directionDown);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionRightDown)
			{
				for(int i = directionDown;i <= directionRight; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionRight)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionRightUp)
			{
				for(int i = directionRight;i <= directionUp; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionUp)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);}
			}
		}
	}
	
	if(isFinded && !findedPoint.isNull())
	{
		setCharacterPoint(findedPoint);
	}
	else // escape point not found
	{
		CCLOG("escape point not found!");
	}
}

void TutoCharacter::setCharacterPoint(IntPoint t_point)
{
	my_point = t_point;
	setPosition(ccpAdd(my_point.convertToCCP(), ccp(2,2)));
	
	if(getRecentStep() == 1 && my_point.x == 120 && my_point.y == (height_value+1)/2 + 11)
	{
		controlerStop();
		nextStep();
	}
}

void TutoCharacter::rewindAnimation()
{
    line_edge->setVisible(false);
	if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("rewind"))
	{
		character_manager->runAnimationsForSequenceNamed("rewind");
//		if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
			jack_img_direction = directionStop;
	}
}

void TutoCharacter::drawAnimation()
{
    line_edge->setVisible(true);
//	if(mySGD->getSelectedCharacterHistory().characterNo.getV() != 2)
//		if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("draw"))
//			character_manager->runAnimationsForSequenceNamed("draw");
//	else
		if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("draw_down"))
			character_manager->runAnimationsForSequenceNamed("draw_down");
}

void TutoCharacter::myInit(function<TutoMapType(IntPoint)> t_getMapData, function<void(IntPoint, TutoMapType)> t_setMapData, int t_height, function<void(IntPointVector)> t_addPath, function<void(void)> t_cleanPath, function<void(void)> t_scanMap, function<int(void)> t_getRecentStep, function<void(void)> t_nextStep)
{
	getMapData = t_getMapData;
	setMapData = t_setMapData;
	addPath = t_addPath;
	cleanPath = t_cleanPath;
	scanMap = t_scanMap;
	
	getRecentStep = t_getRecentStep;
	nextStep = t_nextStep;
	
	height_value = t_height;
	
    line_edge = CCSprite::create("jack_drawing_point.png");//("path_edge_" + path_color + ".png").c_str());
	line_edge->setVisible(false);
	line_edge->setScale(0.5f);
	addChild(line_edge);
    
	auto t_pair = KS::loadCCBIForFullPath<CCSprite*>(this, StageImgLoader::sharedInstance()->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, mySGD->getSelectedCharacterHistory().characterIndex.getV()) + ".ccbi");
	
	character_img = t_pair.first;
	character_manager = t_pair.second;
	
	addChild(character_img);
	
	if(character_manager->getRunningSequenceName() == NULL || character_manager->getRunningSequenceName() != string("stop"))
	{
		character_manager->runAnimationsForSequenceNamed("stop");
//		if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
			jack_img_direction = directionStop;
	}
    jack_img_direction = directionStop;
//	if(mySGD->getSelectedCharacterHistory().characterNo.getV() != 2)
//		jack_img_direction = directionRight;
	
	isDrawingOn = false;
}

void TutoControler::touchAction(CCPoint t_p, bool t_b)
{
	CCPoint distancePoint = ccp(t_p.x - control_circle->getPositionX(), t_p.y - control_circle->getPositionY());
	float distanceValue = sqrtf(powf(distancePoint.x, 2.f) + powf(distancePoint.y, 2.f));
	float angle = atan2(distancePoint.y, distancePoint.x)/M_PI*180.0; // -180 ~ 180
	
	IntDirection angleDirection;
	IntDirection secondDirection;
	
	if(t_b)
	{
		if(distanceValue > minimumDistanceJ && (t_p.x > 480.f-TUTO_TouchOutWidth || t_p.x < TUTO_TouchOutWidth || t_p.y > myDSH->ui_top-TUTO_TouchOutWidth || t_p.y < TUTO_TouchOutWidth))
		{
			CCLOG("Touch Out Action!!!");
			
			if(!myDSH->getBoolForKey(kDSH_Key_isOffJoystickVib))
				GraphDog::get()->vibAction();
			
//			IntPoint jackPoint = my_char->my_point;
//			
//			if(angle < -180.f+regular_spectrum/2.f)
//			{
//				angleDirection = directionLeft;
//				if(jackPoint.y == 0)				secondDirection = directionUp;
//				else												secondDirection = directionDown;
//			}
//			else if(angle < -180.f+(regular_spectrum+irregular_spectrum)/2.f)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionLeftDown;
//					if(jackPoint.x == 0)				secondDirection = directionDown;
//					else												secondDirection = directionLeft;
//				}
//				else
//				{
//					angleDirection = directionLeft;
//					if(jackPoint.y == 0)				secondDirection = directionUp;
//					else												secondDirection = directionDown;
//				}
//			}
//			else if(angle < -180.f+regular_spectrum/2.f+irregular_spectrum)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionLeftDown;
//					if(jackPoint.y == 0)				secondDirection = directionLeft;
//					else												secondDirection = directionDown;
//				}
//				else
//				{
//					angleDirection = directionDown;
//					if(jackPoint.x == 0)				secondDirection = directionRight;
//					else												secondDirection = directionLeft;
//				}
//			}
//			else if(angle < -180.f+regular_spectrum+irregular_spectrum)
//			{
//				angleDirection = directionDown;
//				if(jackPoint.x == 0)				secondDirection = directionRight;
//				else												secondDirection = directionLeft;
//			}
//			else if(angle < -180.f+regular_spectrum*3.f/2.f+irregular_spectrum)
//			{
//				angleDirection = directionDown;
//				if(jackPoint.x == 239)				secondDirection = directionLeft;
//				else												secondDirection = directionRight;
//			}
//			else if(angle < -180.f+(regular_spectrum+irregular_spectrum)*3.f/2.f)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionRightDown;
//					if(jackPoint.y == 0)				secondDirection = directionRight;
//					else												secondDirection = directionDown;
//				}
//				else
//				{
//					angleDirection = directionDown;
//					if(jackPoint.x == 239)				secondDirection = directionLeft;
//					else												secondDirection = directionRight;
//				}
//			}
//			else if(angle < -180.f+regular_spectrum*3.f/2.f+irregular_spectrum*2.f)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionRightDown;
//					if(jackPoint.x == 239)				secondDirection = directionDown;
//					else												secondDirection = directionRight;
//				}
//				else
//				{
//					angleDirection = directionRight;
//					if(jackPoint.y == 0)				secondDirection = directionUp;
//					else												secondDirection = directionDown;
//				}
//			}
//			else if(angle < 0.f)
//			{
//				angleDirection = directionRight;
//				if(jackPoint.y == 0)				secondDirection = directionUp;
//				else												secondDirection = directionDown;
//			}
//			else if(angle < regular_spectrum/2.f)
//			{
//				angleDirection = directionRight;
//				if(jackPoint.y == height_value)				secondDirection = directionDown;
//				else												secondDirection = directionUp;
//			}
//			else if(angle < regular_spectrum/2.f+irregular_spectrum/2.f)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionRightUp;
//					if(jackPoint.x == 239)				secondDirection = directionUp;
//					else												secondDirection = directionRight;
//				}
//				else
//				{
//					angleDirection = directionRight;
//					if(jackPoint.y == height_value)				secondDirection = directionDown;
//					else												secondDirection = directionUp;
//				}
//			}
//			else if(angle < regular_spectrum/2.f+irregular_spectrum)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionRightUp;
//					if(jackPoint.y == height_value)				secondDirection = directionRight;
//					else												secondDirection = directionUp;
//				}
//				else
//				{
//					angleDirection = directionUp;
//					if(jackPoint.x == 239)				secondDirection = directionLeft;
//					else												secondDirection = directionRight;
//				}
//			}
//			else if(angle < regular_spectrum+irregular_spectrum)
//			{
//				angleDirection = directionUp;
//				if(jackPoint.x == 239)				secondDirection = directionLeft;
//				else												secondDirection = directionRight;
//			}
//			else if(angle < regular_spectrum*3.f/2.f+irregular_spectrum)
//			{
//				angleDirection = directionUp;
//				if(jackPoint.x == 0)				secondDirection = directionRight;
//				else												secondDirection = directionLeft;
//			}
//			else if(angle < (regular_spectrum+irregular_spectrum)*3.f/2.f)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionLeftUp;
//					if(jackPoint.y == height_value)				secondDirection = directionLeft;
//					else												secondDirection = directionUp;
//				}
//				else
//				{
//					angleDirection = directionUp;
//					if(jackPoint.x == 0)				secondDirection = directionRight;
//					else												secondDirection = directionLeft;
//				}
//			}
//			else if(angle < regular_spectrum*3.f/2.f+irregular_spectrum*2.f)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionLeftUp;
//					if(jackPoint.x == 0)				secondDirection = directionUp;
//					else												secondDirection = directionLeft;
//				}
//				else
//				{
//					angleDirection = directionLeft;
//					if(jackPoint.y == height_value)				secondDirection = directionDown;
//					else												secondDirection = directionUp;
//				}
//			}
//			else
//			{
//				angleDirection = directionLeft;
//				if(jackPoint.y == height_value)				secondDirection = directionDown;
//				else												secondDirection = directionUp;
//			}
//			
//			if(angleDirection != beforeDirection || isButtonAction)
//			{
//				isButtonAction = false;
//				my_char->changeDirection(angleDirection, secondDirection);
//				schedule(schedule_selector(TutoControler::directionKeeping));
//				beforeDirection = angleDirection;
//				beforeSecondDirection = secondDirection;
//			}
//			else if(beforeSecondDirection != secondDirection)
//			{
//				my_char->changeDirection(angleDirection, secondDirection);
//				schedule(schedule_selector(TutoControler::directionKeeping));
//				beforeDirection = angleDirection;
//				beforeSecondDirection = secondDirection;
//			}
		}
//		else
//		{
			my_char->changeDirection(directionStop, directionStop);
			beforeDirection = directionStop;
			beforeSecondDirection = directionStop;
			unschedule(schedule_selector(TutoControler::directionKeeping));
			return;
//		}
	}
	
	if(button_touch && !my_char->isDrawingOn)
	{
		my_char->isDrawingOn = true;
	}
	
	if(distanceValue > minimumDistanceJ)
	{
		IntPoint jackPoint = my_char->my_point;
		
		if(angle < -180.f+regular_spectrum/2.f)
		{
			angleDirection = directionLeft;
			if(jackPoint.y == 0)				secondDirection = directionUp;
			else												secondDirection = directionDown;
		}
		else if(angle < -180.f+(regular_spectrum+irregular_spectrum)/2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionLeftDown;
				if(jackPoint.x == 0)				secondDirection = directionDown;
				else												secondDirection = directionLeft;
			}
			else
			{
				angleDirection = directionLeft;
				if(jackPoint.y == 0)				secondDirection = directionUp;
				else												secondDirection = directionDown;
			}
		}
		else if(angle < -180.f+regular_spectrum/2.f+irregular_spectrum)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionLeftDown;
				if(jackPoint.y == 0)				secondDirection = directionLeft;
				else												secondDirection = directionDown;
			}
			else
			{
				angleDirection = directionDown;
				if(jackPoint.x == 0)				secondDirection = directionRight;
				else												secondDirection = directionLeft;
			}
		}
		else if(angle < -180.f+regular_spectrum+irregular_spectrum)
		{
			angleDirection = directionDown;
			if(jackPoint.x == 0)				secondDirection = directionRight;
			else												secondDirection = directionLeft;
		}
		else if(angle < -180.f+regular_spectrum*3.f/2.f+irregular_spectrum)
		{
			angleDirection = directionDown;
			if(jackPoint.x == 239)				secondDirection = directionLeft;
			else												secondDirection = directionRight;
		}
		else if(angle < -180.f+(regular_spectrum+irregular_spectrum)*3.f/2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionRightDown;
				if(jackPoint.y == 0)				secondDirection = directionRight;
				else												secondDirection = directionDown;
			}
			else
			{
				angleDirection = directionDown;
				if(jackPoint.x == 239)				secondDirection = directionLeft;
				else												secondDirection = directionRight;
			}
		}
		else if(angle < -180.f+regular_spectrum*3.f/2.f+irregular_spectrum*2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionRightDown;
				if(jackPoint.x == 239)				secondDirection = directionDown;
				else												secondDirection = directionRight;
			}
			else
			{
				angleDirection = directionRight;
				if(jackPoint.y == 0)				secondDirection = directionUp;
				else												secondDirection = directionDown;
			}
		}
		else if(angle < 0.f)
		{
			angleDirection = directionRight;
			if(jackPoint.y == 0)				secondDirection = directionUp;
			else												secondDirection = directionDown;
		}
		else if(angle < regular_spectrum/2.f)
		{
			angleDirection = directionRight;
			if(jackPoint.y == height_value)				secondDirection = directionDown;
			else												secondDirection = directionUp;
		}
		else if(angle < regular_spectrum/2.f+irregular_spectrum/2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionRightUp;
				if(jackPoint.x == 239)				secondDirection = directionUp;
				else												secondDirection = directionRight;
			}
			else
			{
				angleDirection = directionRight;
				if(jackPoint.y == height_value)				secondDirection = directionDown;
				else												secondDirection = directionUp;
			}
		}
		else if(angle < regular_spectrum/2.f+irregular_spectrum)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionRightUp;
				if(jackPoint.y == height_value)				secondDirection = directionRight;
				else												secondDirection = directionUp;
			}
			else
			{
				angleDirection = directionUp;
				if(jackPoint.x == 239)				secondDirection = directionLeft;
				else												secondDirection = directionRight;
			}
		}
		else if(angle < regular_spectrum+irregular_spectrum)
		{
			angleDirection = directionUp;
			if(jackPoint.x == 239)				secondDirection = directionLeft;
			else												secondDirection = directionRight;
		}
		else if(angle < regular_spectrum*3.f/2.f+irregular_spectrum)
		{
			angleDirection = directionUp;
			if(jackPoint.x == 0)				secondDirection = directionRight;
			else												secondDirection = directionLeft;
		}
		else if(angle < (regular_spectrum+irregular_spectrum)*3.f/2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionLeftUp;
				if(jackPoint.y == height_value)				secondDirection = directionLeft;
				else												secondDirection = directionUp;
			}
			else
			{
				angleDirection = directionUp;
				if(jackPoint.x == 0)				secondDirection = directionRight;
				else												secondDirection = directionLeft;
			}
		}
		else if(angle < regular_spectrum*3.f/2.f+irregular_spectrum*2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionLeftUp;
				if(jackPoint.x == 0)				secondDirection = directionUp;
				else												secondDirection = directionLeft;
			}
			else
			{
				angleDirection = directionLeft;
				if(jackPoint.y == height_value)				secondDirection = directionDown;
				else												secondDirection = directionUp;
			}
		}
		else
		{
			angleDirection = directionLeft;
			if(jackPoint.y == height_value)				secondDirection = directionDown;
			else												secondDirection = directionUp;
		}
		
		if(angleDirection != beforeDirection || isButtonAction)
		{
			isButtonAction = false;
			my_char->changeDirection(angleDirection, secondDirection);
			schedule(schedule_selector(TutoControler::directionKeeping));
			beforeDirection = angleDirection;
			beforeSecondDirection = secondDirection;
		}
		else if(beforeSecondDirection != secondDirection)
		{
			my_char->changeDirection(angleDirection, secondDirection);
			schedule(schedule_selector(TutoControler::directionKeeping));
			beforeDirection = angleDirection;
			beforeSecondDirection = secondDirection;
		}
	}
	else
	{
		if(my_char->isDrawingOn)
		{
			my_char->changeDirection(directionStop, directionStop);
			beforeDirection = directionStop;
			unschedule(schedule_selector(TutoControler::directionKeeping));
		}
	}
}

void TutoControler::directionKeeping()
{
	my_char->changeDirection(beforeDirection, beforeSecondDirection);
}

void TutoControler::stopMySchedule()
{
	setTouchEnabled(false);
	my_char->changeDirection(directionStop, directionStop);
	beforeDirection = directionStop;
	beforeSecondDirection = directionStop;
	unschedule(schedule_selector(TutoControler::directionKeeping));
}

#define TUTO_JOYSTICK_IN_DISTANCE	50
#define TUTO_BUTTON_IN_DISTANCE		50

void TutoControler::resetTouch()
{
	button_touch = NULL;
	joystick_touch = NULL;
	offButton();
	
	if(controlJoystickDirection == kControlJoystickDirection_right)
	{
		control_circle->setPosition(ccp(480-TUTO_JOYSTICK_IN_DISTANCE-7.f, TUTO_JOYSTICK_IN_DISTANCE));
		control_ball->setPosition(ccp(480-TUTO_JOYSTICK_IN_DISTANCE-7.f, TUTO_JOYSTICK_IN_DISTANCE));
	}
	else
	{
		control_circle->setPosition(ccp(TUTO_JOYSTICK_IN_DISTANCE+7.f, TUTO_JOYSTICK_IN_DISTANCE));
		control_ball->setPosition(ccp(TUTO_JOYSTICK_IN_DISTANCE+7.f, TUTO_JOYSTICK_IN_DISTANCE));
	}
}

void TutoControler::invisibleControl()
{
	offButton();
	control_ball->setVisible(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick);
	control_circle->setVisible(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick);
	
	if(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick)
	{
		if(controlJoystickDirection == kControlJoystickDirection_right)
		{
			control_circle->setPosition(ccp(480-TUTO_JOYSTICK_IN_DISTANCE-7.f, TUTO_JOYSTICK_IN_DISTANCE));
			control_ball->setPosition(ccp(480-TUTO_JOYSTICK_IN_DISTANCE-7.f, TUTO_JOYSTICK_IN_DISTANCE));
		}
		else
		{
			control_circle->setPosition(ccp(TUTO_JOYSTICK_IN_DISTANCE+7.f, TUTO_JOYSTICK_IN_DISTANCE));
			control_ball->setPosition(ccp(TUTO_JOYSTICK_IN_DISTANCE+7.f, TUTO_JOYSTICK_IN_DISTANCE));
		}
	}
}

void TutoControler::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator iter;
	CCTouch* touch;
	
	for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		
		bool is_button_x;
		
		if(controlJoystickDirection == kControlJoystickDirection_left)
		{
			if(location.x > 480-100)
				is_button_x = true;
			else
				is_button_x = false;
		}
		else
		{
			if(location.x < 100)
				is_button_x = true;
			else
				is_button_x = false;
		}
		
		if(!isDisableDrawButton && is_button_x)
		{
			// button or ui
			
			
			if(!button_touch && location.y < 100)
			{
				// button
				button_touch = touch;
				my_char->isDrawingOn = true;
				onButton();
				
				if(isBacking)
				{
					my_char->drawAnimation();
					(target_main->*pauseBackTracking)();
					
					IntPoint jackPoint = my_char->my_point;
					if(getMapData(jackPoint) == kTutoMapType_empty)
					{
						if(!isDisableLineOver)
							checkBeforeNewLine(jackPoint);
						setMapData(jackPoint, kTutoMapType_newLine);
					}
					
					if(joystick_touch)
					{
						isButtonAction = true;
						CCPoint joystick_location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(joystick_touch->getLocationInView()));
						touchAction(joystick_location, false);
						continue;
					}
				}
				
				if(joystick_touch && !my_char->isMoving)
				{
					isButtonAction = true;
					CCPoint joystick_location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(joystick_touch->getLocationInView()));
					touchAction(joystick_location, false);
				}
			}
			else
			{
				// ui
			}
		}
		else
		{
			if(!joystick_touch)
			{
				CCPoint after_circle_position = location;
				
				if(controlJoystickDirection == kControlJoystickDirection_right)
				{
					if(after_circle_position.x < 90)
						after_circle_position.x = 90;
					else if(after_circle_position.x > 480-JoystickCenterLimit)
						after_circle_position.x = 480-JoystickCenterLimit;
					if(after_circle_position.y < JoystickCenterLimit)
						after_circle_position.y = JoystickCenterLimit;
					else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
						after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
					
					if(isControlJoystickNotFixed)
						control_circle->setPosition(after_circle_position);
					else
					{
						if(after_circle_position.x < 480-TUTO_JOYSTICK_IN_DISTANCE-120 || after_circle_position.y > TUTO_JOYSTICK_IN_DISTANCE+120)
							return;
						else
							control_circle->setPosition(after_circle_position);
					}
				}
				else
				{
					if(after_circle_position.x < JoystickCenterLimit)
						after_circle_position.x = JoystickCenterLimit;
					else if(after_circle_position.x > 390)
						after_circle_position.x = 390;
					if(after_circle_position.y < JoystickCenterLimit)
						after_circle_position.y = JoystickCenterLimit;
					else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
						after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
					
					if(isControlJoystickNotFixed)
						control_circle->setPosition(after_circle_position);
					else
					{
						if(after_circle_position.x > TUTO_JOYSTICK_IN_DISTANCE+120 || after_circle_position.y > TUTO_JOYSTICK_IN_DISTANCE+120)
							return;
						else
							control_circle->setPosition(after_circle_position);
					}
				}
				
				control_circle->setVisible(true);
				
				control_ball->setPosition(location);
				control_ball->setVisible(true);
				
				joystick_touch = touch;
				
				touchAction(location, false);
			}
			
			if(isDisableDrawButton && !joystick_touch && isBacking)
			{
				my_char->drawAnimation();
				(target_main->*pauseBackTracking)();
				
				IntPoint jackPoint = my_char->my_point;
				if(getMapData(jackPoint) == kTutoMapType_empty)
				{
					if(!isDisableLineOver)
						checkBeforeNewLine(jackPoint);
					setMapData(jackPoint, kTutoMapType_newLine);
				}
				
				joystick_touch = touch;
				CCPoint after_circle_position = location;
				
				if(controlJoystickDirection == kControlJoystickDirection_right)
				{
					if(after_circle_position.x < 90)
						after_circle_position.x = 90;
					else if(after_circle_position.x > 480-JoystickCenterLimit)
						after_circle_position.x = 480-JoystickCenterLimit;
					if(after_circle_position.y < JoystickCenterLimit)
						after_circle_position.y = JoystickCenterLimit;
					else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
						after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
					
					if(isControlJoystickNotFixed)
						control_circle->setPosition(after_circle_position);
					else
					{
						if(after_circle_position.x < 480-TUTO_JOYSTICK_IN_DISTANCE-120 || after_circle_position.y > TUTO_JOYSTICK_IN_DISTANCE+120)
							return;
						else
							control_circle->setPosition(after_circle_position);
					}
				}
				else
				{
					if(after_circle_position.x < JoystickCenterLimit)
						after_circle_position.x = JoystickCenterLimit;
					else if(after_circle_position.x > 390)
						after_circle_position.x = 390;
					if(after_circle_position.y < JoystickCenterLimit)
						after_circle_position.y = JoystickCenterLimit;
					else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
						after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
					
					if(isControlJoystickNotFixed)
						control_circle->setPosition(after_circle_position);
					else
					{
						if(after_circle_position.x > TUTO_JOYSTICK_IN_DISTANCE+120 || after_circle_position.y > TUTO_JOYSTICK_IN_DISTANCE+120)
							return;
						else
							control_circle->setPosition(after_circle_position);
					}
				}
				
				
				control_circle->setVisible(true);
				
				control_ball->setPosition(location);
				control_ball->setVisible(true);
				
				touchAction(location, false);
				
				if(joystick_touch)
				{
					isButtonAction = true;
					CCPoint joystick_location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(joystick_touch->getLocationInView()));
					touchAction(joystick_location, false);
					continue;
				}
			}
		}
	}
}

void TutoControler::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator iter;
	CCTouch* touch;
	
	for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		
		if(touch == button_touch)
		{
			
		}
		else if(touch == joystick_touch)
		{
//			if(!myJack->willBackTracking && !isStun)
//			{
				CCPoint distancePoint = ccp(location.x - control_circle->getPositionX(), location.y - control_circle->getPositionY());
				float distanceValue = sqrtf(powf(distancePoint.x, 2.0) + powf(distancePoint.y, 2.0));
				float angle = atan2(distancePoint.y, distancePoint.x)/M_PI*180.0; // -180 ~ 180
				
				CCPoint after_circle_position = CCPointZero;
				if(distanceValue > TUTO_JOYSTICK_FOLLOW_DISTANCE)
				{
					float sub_value = distanceValue - TUTO_JOYSTICK_FOLLOW_DISTANCE;
					after_circle_position.x = 1;
					after_circle_position.y = tanf(angle/180.f*M_PI);
					
					float div_value = sqrtf(powf(after_circle_position.x, 2.f) + powf(after_circle_position.y, 2.f));
					if(angle >= 90 || angle <= -90)
						after_circle_position = ccpMult(after_circle_position, -sub_value/div_value);
					else
						after_circle_position = ccpMult(after_circle_position, sub_value/div_value);
				}
				
				after_circle_position = ccpAdd(after_circle_position, control_circle->getPosition());
				
				if(controlJoystickDirection == kControlJoystickDirection_right)
				{
					if(after_circle_position.x < 90)
						after_circle_position.x = 90;
					else if(after_circle_position.x > 480-JoystickCenterLimit)
						after_circle_position.x = 480-JoystickCenterLimit;
					if(after_circle_position.y < JoystickCenterLimit)
						after_circle_position.y = JoystickCenterLimit;
					else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
						after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
					
					if(!isJoystickCenterNotFixed) // !myDSH->getBoolForKey(kDSH_Key_isControlJoystickFixed) &&
					{
						if(!isControlJoystickNotFixed)
						{
							CCPoint circle_position = after_circle_position;
							if(circle_position.x < 480-TUTO_JOYSTICK_IN_DISTANCE-120)
								circle_position.x = 480-TUTO_JOYSTICK_IN_DISTANCE-120;
							if(circle_position.y > TUTO_JOYSTICK_IN_DISTANCE+120)
								circle_position.y = TUTO_JOYSTICK_IN_DISTANCE+120;
							
							control_circle->setPosition(circle_position);
							
							float t_distance = distanceValue;
							if(distanceValue > 25*joystick_size_value)
								t_distance = 25*joystick_size_value;
							
							CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
							
							control_ball->setPosition(inner_position);
						}
						else
						{
							control_circle->setPosition(after_circle_position);
							float t_distance = distanceValue;
							if(distanceValue > 25*joystick_size_value)
								t_distance = 25*joystick_size_value;
							
							CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
							control_ball->setPosition(inner_position);
						}
					}
					else
					{
						float t_distance = distanceValue;
						if(distanceValue > 25*joystick_size_value)
							t_distance = 25*joystick_size_value;
						
						CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
						
						control_ball->setPosition(inner_position);
					}
				}
				else
				{
					if(after_circle_position.x < JoystickCenterLimit)
						after_circle_position.x = JoystickCenterLimit;
					else if(after_circle_position.x > 390)
						after_circle_position.x = 390;
					if(after_circle_position.y < JoystickCenterLimit)
						after_circle_position.y = JoystickCenterLimit;
					else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
						after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
					
					if(!isJoystickCenterNotFixed) // !myDSH->getBoolForKey(kDSH_Key_isControlJoystickFixed) &&
					{
						if(!isControlJoystickNotFixed)
						{
							CCPoint circle_position = after_circle_position;
							if(circle_position.x > TUTO_JOYSTICK_IN_DISTANCE+120)
								circle_position.x = TUTO_JOYSTICK_IN_DISTANCE+120;
							if(circle_position.y > TUTO_JOYSTICK_IN_DISTANCE+120)
								circle_position.y = TUTO_JOYSTICK_IN_DISTANCE+120;
							
							control_circle->setPosition(circle_position);
							
							float t_distance = distanceValue;
							if(distanceValue > 25*joystick_size_value)
								t_distance = 25*joystick_size_value;
							
							CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
							
							control_ball->setPosition(inner_position);
						}
						else
						{
							control_circle->setPosition(after_circle_position);
							float t_distance = distanceValue;
							if(distanceValue > 25*joystick_size_value)
								t_distance = 25*joystick_size_value;
							
							CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
							control_ball->setPosition(inner_position);
						}
					}
					else
					{
						float t_distance = distanceValue;
						if(distanceValue > 25*joystick_size_value)
							t_distance = 25*joystick_size_value;
						
						CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
						
						control_ball->setPosition(inner_position);
					}
				}
				
				
				touchAction(location, false);
//			}
		}
	}
}

void TutoControler::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator iter;
	CCTouch* touch;
	
	for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		
		if(touch == button_touch)
		{
			offButton();
			
			if(my_char->isDrawingOn)
			{
				my_char->rewindAnimation();
                
                my_char->changeDirection(directionStop, directionStop);
                beforeDirection = directionStop;
                unschedule(schedule_selector(TutoControler::directionKeeping));
                
				(target_main->*delegate_readyBack)();
			}
			
			my_char->isDrawingOn = false;
			button_touch = NULL;
		}
		else if(touch == joystick_touch)
		{
			CCPoint distancePoint = ccp(location.x - control_circle->getPositionX(), location.y - control_circle->getPositionY());
			float distanceValue = sqrt(pow(distancePoint.x, 2.0) + pow(distancePoint.y, 2.0));
			float angle = atan2(distancePoint.y, distancePoint.x)/M_PI*180.0; // -180 ~ 180
			
			CCPoint after_circle_position = CCPointZero;
			if(distanceValue > 37)
			{
				after_circle_position.x = 1;
				after_circle_position.y = tanf(angle/180.f*M_PI);
				
				float div_value = sqrtf(powf(after_circle_position.x, 2.f) + powf(after_circle_position.y, 2.f));
				if(angle >= 90 || angle <= -90)
					after_circle_position = ccpMult(after_circle_position, -1.f/div_value);
				else
					after_circle_position = ccpMult(after_circle_position, 1.f/div_value);
			}
			after_circle_position = ccpAdd(after_circle_position, control_circle->getPosition());
			
			if(controlJoystickDirection == kControlJoystickDirection_right)
			{
				if(after_circle_position.x < 90)
					after_circle_position.x = 90;
				else if(after_circle_position.x > 480-JoystickCenterLimit)
					after_circle_position.x = 480-JoystickCenterLimit;
				if(after_circle_position.y < JoystickCenterLimit)
					after_circle_position.y = JoystickCenterLimit;
				else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
					after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
				
				if(!isJoystickCenterNotFixed)
				{
					if(!isControlJoystickNotFixed)
					{
						CCPoint circle_position = after_circle_position;
						if(circle_position.x < 480-TUTO_JOYSTICK_IN_DISTANCE-120)
							circle_position.x = 480-TUTO_JOYSTICK_IN_DISTANCE-120;
						if(circle_position.y > TUTO_JOYSTICK_IN_DISTANCE+120)
							circle_position.y = TUTO_JOYSTICK_IN_DISTANCE+120;
						
						control_circle->setPosition(circle_position);
						
						float t_distance = distanceValue;
						if(distanceValue > 25*joystick_size_value)
							t_distance = 25*joystick_size_value;
						
						CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
						
						control_ball->setPosition(inner_position);
					}
					else
					{
						control_circle->setPosition(after_circle_position);
						control_ball->setPosition(location);
					}
				}
				else
				{
					float t_distance = distanceValue;
					if(distanceValue > 25*joystick_size_value)
						t_distance = 25*joystick_size_value;
					
					CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
					
					control_ball->setPosition(inner_position);
				}
			}
			else
			{
				if(after_circle_position.x < JoystickCenterLimit)
					after_circle_position.x = JoystickCenterLimit;
				else if(after_circle_position.x > 390)
					after_circle_position.x = 390;
				if(after_circle_position.y < JoystickCenterLimit)
					after_circle_position.y = JoystickCenterLimit;
				else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
					after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
				
				if(!isJoystickCenterNotFixed)
				{
					if(!isControlJoystickNotFixed)
					{
						CCPoint circle_position = after_circle_position;
						if(circle_position.x > TUTO_JOYSTICK_IN_DISTANCE+120)
							circle_position.x = TUTO_JOYSTICK_IN_DISTANCE+120;
						if(circle_position.y > TUTO_JOYSTICK_IN_DISTANCE+120)
							circle_position.y = TUTO_JOYSTICK_IN_DISTANCE+120;
						
						control_circle->setPosition(circle_position);
						
						float t_distance = distanceValue;
						if(distanceValue > 25*joystick_size_value)
							t_distance = 25*joystick_size_value;
						
						CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
						
						control_ball->setPosition(inner_position);
					}
					else
					{
						control_circle->setPosition(after_circle_position);
						control_ball->setPosition(location);
					}
				}
				else
				{
					float t_distance = distanceValue;
					if(distanceValue > 25*joystick_size_value)
						t_distance = 25*joystick_size_value;
					
					CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
					
					control_ball->setPosition(inner_position);
				}
			}
			
			touchAction(location, true);
			if(isControlJoystickNotFixed && isAlwaysInvisibleJoystick)
			{
				control_circle->setVisible(false);
				control_ball->setVisible(false);
			}
			else
			{
				if(controlJoystickDirection == kControlJoystickDirection_right)
				{
					control_circle->setPosition(ccp(480-TUTO_JOYSTICK_IN_DISTANCE-7.f, TUTO_JOYSTICK_IN_DISTANCE));
					control_ball->setPosition(ccp(480-TUTO_JOYSTICK_IN_DISTANCE-7.f, TUTO_JOYSTICK_IN_DISTANCE));
				}
				else
				{
					control_circle->setPosition(ccp(TUTO_JOYSTICK_IN_DISTANCE+7.f, TUTO_JOYSTICK_IN_DISTANCE));
					control_ball->setPosition(ccp(TUTO_JOYSTICK_IN_DISTANCE+7.f, TUTO_JOYSTICK_IN_DISTANCE));
				}
			}
			
			if(isDisableDrawButton && my_char->isDrawingOn)
			{
				my_char->rewindAnimation();
                
                my_char->changeDirection(directionStop, directionStop);
                beforeDirection = directionStop;
                unschedule(schedule_selector(TutoControler::directionKeeping));
                
				(target_main->*delegate_readyBack)();
			}
			
			joystick_touch = NULL;
		}
	}
}

void TutoControler::ccTouchesCancelled(CCSet *pTouch, CCEvent *pEvent)
{
	ccTouchesEnded(pTouch, pEvent);
}

void TutoControler::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, -110);
}

TutoControler* TutoControler::create(TutoCharacter* t_char, int t_height, function<TutoMapType(IntPoint)> t_getMapData, function<void(IntPoint, TutoMapType)> t_setMapData, function<void(IntPoint)> t_checkBeforeNewLine)
{
	TutoControler* t_cjf = new TutoControler();
	t_cjf->myInit(t_char, t_height, t_getMapData, t_setMapData, t_checkBeforeNewLine);
	t_cjf->autorelease();
	return t_cjf;
}

void TutoControler::buttonSetVisible(bool t_b)
{
	isDisableDrawButton = !t_b;
	draw_button->setVisible(t_b);
}
void TutoControler::joystickSetVisible(bool t_b)
{
	control_ball->setVisible(t_b);
	control_circle->setVisible(t_b);
	
	setTouchEnabled(t_b);
}

void TutoControler::myInit(TutoCharacter* t_char, int t_height, function<TutoMapType(IntPoint)> t_getMapData, function<void(IntPoint, TutoMapType)> t_setMapData, function<void(IntPoint)> t_checkBeforeNewLine)
{
	CCLayer::init();
	
	my_char = t_char;
	height_value = t_height;
	getMapData = t_getMapData;
	setMapData = t_setMapData;
	checkBeforeNewLine = t_checkBeforeNewLine;
	
	joystick_size_value = (myDSH->getIntegerForKey(kDSH_Key_joystickSize)+10)/10.f;
	
	TUTO_TouchOutWidth = 10.0*((myDSH->getIntegerForKey(kDSH_Key_joystickSize)+10)/10.f);
	TUTO_JOYSTICK_FOLLOW_DISTANCE = 70.0*((myDSH->getIntegerForKey(kDSH_Key_joystickSize)+10)/10.f);
	
	minimumDistanceJ = 8.0*((myDSH->getIntegerForKey(kDSH_Key_joystickSize)+10)/10.f);
	JoystickCenterLimit = 30.0*((myDSH->getIntegerForKey(kDSH_Key_joystickSize)+10)/10.f);
	
	regular_spectrum = 70.f;
	irregular_spectrum = 20.f;
	
	isButtonAction = false;
	button_touch = NULL;
	joystick_touch = NULL;
	isBacking = false;
	
	isEnableIrregularDirection = myDSH->getBoolForKey(kDSH_Key_isEnableIrregularDirection);
	isControlJoystickNotFixed = myDSH->getBoolForKey(kDSH_Key_isControlJoystickNotFixed);
	isAlwaysInvisibleJoystick = myDSH->getBoolForKey(kDSH_Key_isAlwaysInvisibleJoystick);
	controlJoystickDirection = myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection);
	isDisableDrawButton = myDSH->getBoolForKey(kDSH_Key_isDisableDrawButton);
	isDisableLineOver = myDSH->getBoolForKey(kDSH_Key_isDisableLineOver);
	isJoystickCenterNotFixed = myDSH->getBoolForKey(kDSH_Key_isJoystickCenterNotFixed);
	
	beforeDirection = directionStop;
	control_circle = CCSprite::create("control_joystick_big_circle.png");
	control_circle->setScale(((myDSH->getIntegerForKey(kDSH_Key_joystickSize)+10)/10.f));
	control_circle->setVisible(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick);
	addChild(control_circle);
	
	control_ball = CCSprite::create("control_joystick_big_ball.png");
	control_ball->setScale(((myDSH->getIntegerForKey(kDSH_Key_joystickSize)+10)/10.f));
	control_ball->setVisible(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick);
	addChild(control_ball);
	
	if(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick)
	{
		if(controlJoystickDirection == kControlJoystickDirection_right)
		{
			control_circle->setPosition(ccp(480-TUTO_JOYSTICK_IN_DISTANCE-7.f, TUTO_JOYSTICK_IN_DISTANCE));
			control_ball->setPosition(ccp(480-TUTO_JOYSTICK_IN_DISTANCE-7.f, TUTO_JOYSTICK_IN_DISTANCE));
		}
		else
		{
			control_circle->setPosition(ccp(TUTO_JOYSTICK_IN_DISTANCE+7.f, TUTO_JOYSTICK_IN_DISTANCE));
			control_ball->setPosition(ccp(TUTO_JOYSTICK_IN_DISTANCE+7.f, TUTO_JOYSTICK_IN_DISTANCE));
		}
	}
	
	if(!isDisableDrawButton)
	{
//		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//		CCBReader* reader = new CCBReader(nodeLoader);
//		draw_button = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile(CCString::createWithFormat("gameui_button_%s.ccbi", myLoc->getLocalCode().c_str())->getCString(),this));
		auto t_ccb = KS::loadCCBI<CCSprite*>(this, CCString::createWithFormat("gameui_button_%s.ccbi", myLoc->getSupportLocalCode())->getCString());
		draw_button = t_ccb.first;
		button_ani = t_ccb.second;
		//		draw_button = CCSprite::create("ui_draw.png");
		if(controlJoystickDirection == kControlJoystickDirection_left)		draw_button->setPosition(ccp(480-TUTO_BUTTON_IN_DISTANCE,TUTO_BUTTON_IN_DISTANCE));
		else								draw_button->setPosition(ccp(TUTO_BUTTON_IN_DISTANCE,TUTO_BUTTON_IN_DISTANCE));
		addChild(draw_button);
		
		click_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_controlClick), mySGD->getFont().c_str(), 18);
		click_label->enableOuterStroke(ccBLACK, 0.5f);
		click_label->setPosition(ccp(draw_button->getContentSize().width/2.f, draw_button->getContentSize().height/2.f));
		click_label->setVisible(false);
		draw_button->addChild(click_label);
	}
}

void TutoControler::onButton()
{
	if(button_ani)
	{
		button_ani->runAnimationsForSequenceNamed("cast1start");
		click_label->stopAllActions();
		CCFadeTo* t_fade = CCFadeTo::create(0.5f, 50);
		click_label->runAction(t_fade);
	}
}

void TutoControler::offButton()
{
	if(button_ani)
	{
		button_ani->runAnimationsForSequenceNamed("cast1stop");
		click_label->stopAllActions();
		CCFadeTo* t_fade = CCFadeTo::create(0.3f, 255);
		click_label->runAction(t_fade);
	}
}

void BackImg::visit()
{
	silhouette->visit();
	
	unsigned int loopCnt = drawRects->count();
	
	glEnable(GL_SCISSOR_TEST);
	
	for(int i=0;i<loopCnt;i++)
	{
		IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
		
		float wScale = viewport[2] / design_resolution_size.width;
		float hScale = viewport[3] / design_resolution_size.height;
		
		float x, y, w, h;
		
		x = (t_rect->origin.x)*wScale + viewport[0]-1;
		y = (t_rect->origin.y)*hScale + viewport[1]-1;
		w = (t_rect->size.width)*wScale+2;
		h = (t_rect->size.height)*hScale+2;
		
		glScissor(x,y,w,h);
		draw();
	}
	
	glDisable(GL_SCISSOR_TEST);
}

void BackImg::startSilhouette()
{
	ing_cnt = 0;
	schedule(schedule_selector(BackImg::silhouetteAction));
}

void BackImg::silhouetteAction()
{
	ing_cnt++;
	if(ing_cnt%30 == 0)
	{
		int tick_cnt = ing_cnt/30;
		if(tick_cnt%2 == 1)
			silhouette->setColor(ccc3(100, 100, 255));
		else
			silhouette->setColor(ccBLUE);
		
		if(tick_cnt == 6)
			stopSilhouette();
	}
}

void BackImg::stopSilhouette()
{
	silhouette->setColor(ccBLUE);
	unschedule(schedule_selector(BackImg::silhouetteAction));
}

void BackImg::myInit()
{
	initWithFile("play_tutorial_real.png");
	setPosition(ccp(240,myDSH->ui_center_y));
	
	drawRects = NULL;
	
	silhouette = CCSprite::create("play_tutorial_sil.png");
	silhouette->setPosition(ccp(getContentSize().width/2.f,getContentSize().height/2.f));
	addChild(silhouette, -1);
	silhouette->setColor(ccBLUE);
	
	screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	design_resolution_size = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	drawRects = new CCArray(1);
}


CCScene* PlayTutorial::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	PlayTutorial *layer = PlayTutorial::create();
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool PlayTutorial::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	if(!mySGD->is_option_tutorial)
		myDSH->setIntegerForKey(kDSH_Key_showedScenario, 4);
	
	map_data.clear();
	height_value = int(myDSH->ui_top/2);
	map_data.resize(240*height_value);
	
	//초기값 넣어주기
	for(int j=0;j<24;j++)
	{
		for(int i=0;i<30;i++)
		{
			if(j == 0 || j == 24-1 || i == 0 || i == 30-1)
				setMapData(120-15+i, height_value/2-12+j, kTutoMapType_line);
			else
				setMapData(120-15+i, height_value/2-12+j, kTutoMapType_get);
		}
	}

	view_img = BackImg::create();
	addChild(view_img, 1);
	
	resetRects();
	
	path_manager = TutoPathManager::create([=](IntPoint t_p){ return getMapData(t_p.x, t_p.y); }, [=](IntPoint t_p, TutoMapType t_type){ setMapData(t_p.x, t_p.y, t_type); }, height_value);
	addChild(path_manager, 2);
	
	character = TutoCharacter::create([=](IntPoint t_p){ return getMapData(t_p.x, t_p.y); }, [=](IntPoint t_p, TutoMapType t_type){ setMapData(t_p.x, t_p.y, t_type); }, height_value-1,
									  [=](IntPointVector t_pv){ path_manager->addPath(t_pv); }, [=](){ path_manager->cleanPath(); }, [=](){ scanMap(); }, [=](){ return getRecentStep(); }, [=](){nextStep();});
	character->setCharacterPoint(IntPoint(120,height_value/2-12));
	addChild(character, 3);
	
	
	
	top_label = KSLabelTTF::create("", mySGD->getFont().c_str(), 18);
	top_label->setColor(ccc3(255, 170, 20));
	top_label->enableOuterStroke(ccBLACK, 2);
	top_label->setPosition(ccp(240, myDSH->ui_top-30));
	addChild(top_label, 6);
	
	mark_img = KS::loadCCBI<CCSprite*>(this, "tutorial_target.ccbi").first;
	mark_img->setVisible(false);
	mark_img->setPosition(ccp(240, height_value+24));
	addChild(mark_img, 2);
	
	
	
	tutorial_step = 0;
	
	CCNode* curtain_node = LoadingTipScene::getOpenCurtainNode();
	curtain_node->setPosition(ccp(240,myDSH->ui_center_y));
	curtain_node->setScale(myDSH->screen_convert_rate);
	addChild(curtain_node, 200);
	
	
	CCNode* scenario_node = CCNode::create();
	addChild(scenario_node, 99);
	
	CCNode* t_stencil_node = CCNode::create();
	
	CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
	t_clipping->setAlphaThreshold(0.1f);
	
	float change_scale_y = 1.f;
	CCPoint change_origin = ccp(0,0);
	change_scale_y = myDSH->ui_top/320.f;
	CCSize win_size = CCSizeMake(480, 320);
	t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width, win_size.height*change_scale_y));
	
	CCSprite* t_gray = CCSprite::create("back_gray.png");
	t_gray->setScaleY(change_scale_y);
	t_gray->setOpacity(0);
	t_gray->setPosition(ccp(240,myDSH->ui_center_y));
	t_clipping->addChild(t_gray);
	
	t_clipping->setInverted(true);
	scenario_node->addChild(t_clipping, 0);
	
	CCSprite* asuka = CCSprite::create("kt_cha_asuka_1.png");
	asuka->setAnchorPoint(ccp(1,0));
	asuka->setPosition(ccp(480+asuka->getContentSize().width, 0));
	scenario_node->addChild(asuka, 1);
	
	CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
	ikaruga->setAnchorPoint(ccp(0,0));
	ikaruga->setPosition(ccp(-ikaruga->getContentSize().width/3.f, 0));
	ikaruga->setVisible(false);
	scenario_node->addChild(ikaruga, 1);
	
	TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(225, 50), ccp(480.f-56.f,32.5f));
	typing_box->setBoxScale(myDSH->screen_convert_rate);
	scenario_node->addChild(typing_box, 2);
	
	typing_box->setTouchOffScrollAndButton();
	typing_box->setVisible(false);
	typing_box->setTouchSuction(false);
	
	TypingBox* typing_box2 = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 60), ccp(255, 60), ccp(480.f-56.f,32.5f));
	typing_box2->setBoxScale(myDSH->screen_convert_rate);
	scenario_node->addChild(typing_box2, 2);
	typing_box2->setHide();
	
	function<void()> end_func3 = [=]()
	{
		StoryManager* t_sm = StoryManager::create(-500);
		addChild(t_sm, 100);
		
		addChild(KSTimer::create(0.5f, [=]()
								 {
									 CCSprite* gray = CCSprite::create("back_gray.png");
									 gray->setOpacity(0);
									 gray->setPosition(ccp(0,-160+myDSH->ui_center_y));
									 gray->setScaleY(myDSH->ui_top/320.f);
									 gray->runAction(CCFadeTo::create(0.5f, 255));
									 t_sm->back_node->addChild(gray);
									 
//									 AudioEngine::sharedInstance()->playEffect("ment_tutorial_start.mp3", false, true);
//									 addChild(KSTimer::create(2.8f, [=](){AudioEngine::sharedInstance()->playEffect("ment_tutorial1.mp3", false, true);
//										 addChild(KSTimer::create(2.4f, [=](){AudioEngine::sharedInstance()->playEffect("ment_tutorial2.mp3", false, true);
//										 }));
//									 }));
									 
									 
									 
									 t_sm->addMent(true, "", "", myLoc->getLocalForKey(LK::kMyLocalKey_tutorial1), [=]()
												   //"가운데 빨간 보석이 캐릭터 입니다.\n캐릭터를 이동시켜서 영역 가장자리를 이동할 수도 있고\n영역을 획득할 수도 있습니다.", [=]()
												   {
													   gray->runAction(CCFadeTo::create(0.3f, 0));
													   controler->joystickSetVisible(true);
													   mark_img->setVisible(true);
													   
//													   AudioEngine::sharedInstance()->playEffect("ment_tutorial3.mp3", false, true);
//													   
//													   addChild(KSTimer::create(2.6f, [=](){AudioEngine::sharedInstance()->playEffect("ment_tutorial4.mp3", false, true);
//														   addChild(KSTimer::create(3.7f, [=](){AudioEngine::sharedInstance()->playEffect("ment_tutorial5.mp3", false, true);
//															   
//														   }));
//													   }));
													   t_sm->addMent(true, "", "", myLoc->getLocalForKey(LK::kMyLocalKey_tutorial2), [=]()
																	 //"먼저 영역 위를 이동하는 방법에 대해 소개해드릴게요.\n아래에 조이스틱이 있습니다.\n이 조이스틱으로 캐릭터를 원하는 방향으로 이동시킬 수 있어요.\n조이스틱으로 캐릭터를 위로 이동시켜보세요.", [=]()
																	 {
                                                                         if(!mySGD->is_option_tutorial)
                                                                             fiverocks::FiveRocksBridge::trackEvent("Game", "FirstUserTrace", "T05_Tutorial1", myHSP->getStoreID().c_str());
																		 top_label->setString(myLoc->getLocalForKey(LK::kMyLocalKey_tutorial3));//"캐릭터를 위로 이동시키기");
																		 tutorial_step = 1;
																		 t_sm->removeFromParent();
																	 }, CCSizeMake(350,100), ccp(0,-110), 12, true);
												   }, CCSizeMake(350,100), ccp(0,-110), 12, true);
								 }));
		
		addChild(KSTimer::create(0.1f, [=]()
								 {
									scenario_node->removeFromParent();
								 }));
	};
	
	function<void()> end_func2 = [=]()
	{
		typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent20), end_func3);
	};
	
	function<void()> end_func1 = [=]()
	{
		TypingBox::changeTypingBox(typing_box2, typing_box, asuka, ikaruga);
		typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent19), end_func2);
	};
	
    JoystickPositionSelectPopup* t_popup = JoystickPositionSelectPopup::create(-99999, [=]()
                                                                               {
                                                                                   controler = TutoControler::create(character, height_value-1, [=](IntPoint t_p){ return getMapData(t_p.x, t_p.y); }, [=](IntPoint t_p, TutoMapType t_type){ setMapData(t_p.x, t_p.y, t_type); },
                                                                                                                     [=](IntPoint t_p){ path_manager->checkBeforeNewline(t_p); });
                                                                                   controler->target_main = this;
                                                                                   controler->delegate_readyBack = callfunc_selector(PlayTutorial::startBackTracking);
                                                                                   controler->pauseBackTracking = callfunc_selector(PlayTutorial::stopBackTracking);
                                                                                   addChild(controler, 5);
                                                                                   
                                                                                   character->is_controler_backing = [=]()
                                                                                   {
                                                                                       return controler->isBacking;
                                                                                   };
                                                                                   
                                                                                   character->controlerStop = [=](){controler->stopMySchedule(); controler->resetTouch();};
                                                                                   
                                                                                   controler->buttonSetVisible(false);
                                                                                   controler->joystickSetVisible(false);
                                                                                   
                                                                                   typing_box2->showAnimation(0.3f);
                                                                                   
                                                                                   scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
                                                                                                                                         {
                                                                                                                                             t_gray->setOpacity(t*255);
                                                                                                                                             asuka->setPositionX(480+asuka->getContentSize().width - asuka->getContentSize().width*2.f/3.f*t);
                                                                                                                                         }, [=](float t)
                                                                                                                                         {
                                                                                                                                             t_gray->setOpacity(255);
                                                                                                                                             asuka->setPositionX(480+asuka->getContentSize().width - asuka->getContentSize().width*2.f/3.f*t);
                                                                                                                                             
                                                                                                                                             typing_box2->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent18), end_func1);
                                                                                                                                         }));
                                                                               });
    addChild(t_popup, 99999);
    
    
	return true;
}

void PlayTutorial::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
	AudioEngine::sharedInstance()->playSound("sound_back_maingame.mp3", true);
}

int PlayTutorial::getRecentStep()
{
	return tutorial_step;
}

void PlayTutorial::nextStep()
{
	tutorial_step++;
	
	if(tutorial_step == 2)
	{
		top_label->setString("");
		mark_img->setVisible(false);
		
		
		StoryManager* t_sm = StoryManager::create(-500);
		addChild(t_sm, 101);
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(0,-160+myDSH->ui_center_y));
		gray->setScaleY(myDSH->ui_top/320.f);
		t_sm->back_node->addChild(gray);
		
		controler->buttonSetVisible(true);
		
		area_take_sample = CCClippingNode::create(CCSprite::create("tutorial_ccb_mask.png"));
        
        string ccbi_name;
        if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_left)
            ccbi_name = "tutorial_new_left.ccbi";
        else
            ccbi_name = "tutorial_new_right.ccbi";
        
		CCSprite* t_ccbi = KS::loadCCBI<CCSprite*>(this, ccbi_name.c_str()).first;
		area_take_sample->addChild(t_ccbi);
		area_take_sample->setPosition(ccp(240,210));
		addChild(area_take_sample, 100);
		
//		AudioEngine::sharedInstance()->playEffect("ment_tutorial6.mp3", false, true);
//		addChild(KSTimer::create(2.2f, [=](){AudioEngine::sharedInstance()->playEffect("ment_tutorial7.mp3", false, true);
//			addChild(KSTimer::create(5.2f, [=](){AudioEngine::sharedInstance()->playEffect("ment_tutorial8.mp3", false, true);
//				
//			}));
//		}));
		
		t_sm->addMent(true, "", "", myLoc->getLocalForKey(LK::kMyLocalKey_tutorial4), [=]()//"다음에는 영역을 획득하는 방법을 알아보도록 해요.\n아래의 꾸욱 버튼을 누르고 있으면\n영역 바깥으로 나갈 수 있답니다.\n보이는 것처럼 영역을 획득해보세요.", [=]()
		{
            if(!mySGD->is_option_tutorial)
                fiverocks::FiveRocksBridge::trackEvent("Game", "FirstUserTrace", "T06_Tutorial2", myHSP->getStoreID().c_str());
            
			controler->setTouchEnabled(true);
			area_take_sample->removeFromParent();
			mark_img->setPosition(ccp(80,myDSH->ui_center_y));
			mark_img->setVisible(true);
			
			top_label->setString(myLoc->getLocalForKey(LK::kMyLocalKey_tutorial5));//"영역 획득하기");
			tutorial_step = 3;

			startCatching();
			
			t_sm->removeFromParent();
		}, CCSizeMake(350,100), ccp(0,-110), 12, true);
	}
	else if(tutorial_step == 4)
	{
		top_label->setString("");
		mark_img->setVisible(false);
		
		StoryManager* t_sm = StoryManager::create(-500);
		addChild(t_sm, 100);
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(0,-160+myDSH->ui_center_y));
		gray->setScaleY(myDSH->ui_top/320.f);
		t_sm->back_node->addChild(gray);
		
		CCSprite* ui_percent = CCSprite::create("play_tutorial_gage.png");
		ui_percent->setPosition(ccp(240,myDSH->ui_top-15));
		addChild(ui_percent, 4);
		
		ui_percent->setVisible(false);
		
		CCSequence* t_sequ = CCSequence::create(CCDelayTime::create(0.5f), CCShow::create(), NULL);
		CCSequence* t_seq = CCSequence::create(CCDelayTime::create(0.5f), CCHide::create(), CCDelayTime::create(0.5f), CCShow::create(), NULL);
		CCRepeat* t_repeat = CCRepeat::create(t_seq, 3);
		CCSequence* t_seqq = CCSequence::create(t_sequ, t_repeat, NULL);
		
		ui_percent->runAction(t_seqq);
		
		view_img->startSilhouette();
		
//		AudioEngine::sharedInstance()->playEffect("ment_tutorial9.mp3", false, true);
		
        if(!mySGD->is_option_tutorial)
            fiverocks::FiveRocksBridge::trackEvent("Game", "FirstUserTrace", "T07_Tutorial3", myHSP->getStoreID().c_str());
        
		t_sm->addMent(true, "", "", myLoc->getLocalForKey(LK::kMyLocalKey_tutorial6), [=]()
					  //"파란 실루엣 영역을 획득해야 게임 달성도가 올라갑니다.", [=]()
					  {
						  t_sm->cleanSM();
						  
						  view_img->stopSilhouette();
						  ui_percent->stopAllActions();
						  ui_percent->setVisible(true);
						  
						  CCSprite* clear_condition = CCSprite::create(CCString::createWithFormat("play_tutorial_gage_guide_%s.png", myLoc->getSupportLocalCode())->getCString());
						  clear_condition->setPosition(ccp(240,myDSH->ui_top-58));
						  addChild(clear_condition, 4);
						  
						  CCSequence* t_seq = CCSequence::create(CCDelayTime::create(0.5f), CCHide::create(), CCDelayTime::create(0.5f), CCShow::create(), NULL);
						  CCRepeat* t_repeat = CCRepeat::create(t_seq, 3);
						  clear_condition->runAction(t_repeat);
						  
						  CCLabelBMFont* time_label = CCLabelBMFont::create("100", "timefont.fnt");
						  time_label->setColor(ccYELLOW);
						  time_label->setPosition(ccp(240,17 + 5));
						  addChild(time_label, 4);
						  
						  CCSequence* t_seq2 = CCSequence::create(CCDelayTime::create(0.5f), CCHide::create(), CCDelayTime::create(0.5f), CCShow::create(), NULL);
						  CCRepeat* t_repeat2 = CCRepeat::create(t_seq2, 3);
						  time_label->runAction(t_repeat2);
						  
						  CCSprite* time_case = CCSprite::create(CCString::createWithFormat("play_tutorial_time_guide_%s.png", myLoc->getSupportLocalCode())->getCString());
						  time_case->setPosition(ccp(240,37));
						  addChild(time_case, 4);
						  
						  CCSequence* t_seq3 = CCSequence::create(CCDelayTime::create(0.5f), CCHide::create(), CCDelayTime::create(0.5f), CCShow::create(), NULL);
						  CCRepeat* t_repeat3 = CCRepeat::create(t_seq3, 3);
						  time_case->runAction(t_repeat3);
						  
						  CCScale9Sprite* ment_box = CCScale9Sprite::create("talk_mentbox.png", CCRectMake(0, 0, 35, 35), CCRectMake(12, 12, 23-12, 23-12));
						  ment_box->setContentSize(CCSizeMake(350, 60));
						  ment_box->setPosition(ccp(240,myDSH->ui_center_y));
						  addChild(ment_box, 4);
						  
						  StyledLabelTTF* ment_label = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_tutorial7), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
							ment_label->setOldAnchorPoint();
						  ment_label->setPosition(ccp(ment_box->getContentSize().width/2.f, ment_box->getContentSize().height/2.f));
						  ment_box->addChild(ment_label);
						  
						  CCLabelTTF* next_label = CCLabelTTF::create("TAP_", mySGD->getFont().c_str(), 12);
						  next_label->setAnchorPoint(ccp(1,0));
						  next_label->setPosition(ccp(ment_box->getContentSize().width-12, 12));
						  next_label->setVisible(false);
						  ment_box->addChild(next_label);
						  
						  addChild(KSTimer::create(0.5f, [=]()
						  {
							  ment_label->setVisible(false);
							  addChild(KSTimer::create(0.5f, [=]()
							  {
								  ment_label->setVisible(true);
								  addChild(KSTimer::create(0.5f, [=]()
								  {
									  ment_label->setVisible(false);
									  addChild(KSTimer::create(0.5f, [=]()
									  {
										  ment_label->setVisible(true);
										  next_label->setVisible(true);
										  
										  TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-600);
										  t_suction->touch_began_func = [=]()
										  {
											  ment_box->removeFromParent();
											  
											  clear_condition->stopAllActions();
											  clear_condition->setVisible(true);
											  
											  time_label->stopAllActions();
											  time_label->setVisible(true);
											  
											  time_case->stopAllActions();
											  time_case->setVisible(true);
											  
											  
//											  AudioEngine::sharedInstance()->playEffect("ment_tutorial10.mp3", false, true);
//											  addChild(KSTimer::create(1.8f, [=](){AudioEngine::sharedInstance()->playEffect("ment_tutorial11.mp3", false, true);
//												  addChild(KSTimer::create(2.2f, [=](){AudioEngine::sharedInstance()->playEffect("ment_tutorial12.mp3", false, true);
//													  
//												  }));
//											  }));
											  
											  
											  bool is_achieve = false;
											  AchievementCode i = kAchievementCode_tutorial;
											  
											  if(!myAchieve->isCompleted(i) && !myAchieve->isAchieve(i))
											  {
												  if(!myAchieve->isNoti(AchievementCode(i)))
												  {
													  is_achieve = true;
													  myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
													  AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
													  CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
												  }
											  }
											  
											  myHSP->IgawAdbrixFirstTimeExperience("TutorialComplete");
											  myHSP->IgawAdbrixRetention("TutorialComplete");
											  
											  
											  CCNode* scenario_node = CCNode::create();
											  addChild(scenario_node, 101);
											  
											  CCNode* t_stencil_node = CCNode::create();
											  
											  CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
											  t_clipping->setAlphaThreshold(0.1f);
											  
											  float change_scale_y = 1.f;
											  CCPoint change_origin = ccp(0,0);
											  change_scale_y = myDSH->ui_top/320.f;
											  CCSize win_size = CCSizeMake(480, 320);
											  t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width, win_size.height*change_scale_y));
											  
											  CCSprite* t_gray = CCSprite::create("back_gray.png");
											  t_gray->setScaleY(change_scale_y);
											  t_gray->setPosition(ccp(240,myDSH->ui_center_y));
											  t_clipping->addChild(t_gray);
											  
											  t_clipping->setInverted(true);
											  scenario_node->addChild(t_clipping, 0);
											  
											  CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
											  ikaruga->setAnchorPoint(ccp(0,0));
											  ikaruga->setPosition(ccp(-ikaruga->getContentSize().width/3.f, 0));
											  scenario_node->addChild(ikaruga, 1);
											  
											  TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(225, 50), ccp(480.f-56.f,32.5f));
											  typing_box->setBoxScale(myDSH->screen_convert_rate);
											  scenario_node->addChild(typing_box, 2);
											  
											  function<void()> end_func1 = [=]()
											  {
												  if(mySGD->is_option_tutorial)
												  {
													  if(is_achieve)
													  {
														  t_loading = LoadingLayer::create(-9999, true);
														  addChild(t_loading, 9999);
														  t_scenario_node = scenario_node;
														  myAchieve->updateAchieve(json_selector(this, PlayTutorial::resultAchieve));
													  }
													  else
													  {
														  mySGD->setNextSceneName("mainflow");
														  
														  AudioEngine::sharedInstance()->stopSound();
														  AudioEngine::sharedInstance()->unloadEffectScene("playtutorial");
														  
														  LoadingTipScene* loading_tip = LoadingTipScene::getLoadingTipSceneLayer();
														  CCNode* t_main_node = loading_tip->getChildByTag(9999);
														  if(t_main_node)
														  {
															  t_main_node->setScale(myDSH->screen_convert_rate);
															  t_main_node->setPositionY(t_main_node->getPositionY()-160+myDSH->ui_center_y);
														  }
														  
														  addChild(loading_tip, 999);
														  
														  addChild(KSTimer::create(0.1f, [=]()
																				   {
																					   scenario_node->removeFromParent();
																				   }));
													  }
												  }
												  else
												  {
													  myDSH->setIntegerForKey(kDSH_Key_showedScenario, 5);
													  mySGD->setNextSceneName("maingame");
													  
													  AudioEngine::sharedInstance()->stopSound();
													  AudioEngine::sharedInstance()->unloadEffectScene("playtutorial");
													  
													  LoadingTipScene* loading_tip = LoadingTipScene::getLoadingTipSceneLayer();
													  CCNode* t_main_node = loading_tip->getChildByTag(9999);
													  if(t_main_node)
													  {
														  t_main_node->setScale(myDSH->screen_convert_rate);
														  t_main_node->setPositionY(t_main_node->getPositionY()-160+myDSH->ui_center_y);
													  }
													  
													  addChild(loading_tip, 999);
													  
													  addChild(KSTimer::create(0.1f, [=]()
																			   {
																				   scenario_node->removeFromParent();
																			   }));
													}
											  };
											  
											  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent21), end_func1);
											  
											  
											  
											  
//											  t_sm->addMent(true, "", "", myLoc->getLocalForKey(LK::kMyLocalKey_tutorial8), [=]()
//															//"기본 튜토리얼을 모두 진행하셨습니다.\n본 게임으로 들아갑니다.", [=]()
//															{
//																AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
//																t_sm->removeFromParent();
//																mySGD->setNextSceneName("maingame");
//																
//																AudioEngine::sharedInstance()->unloadEffectScene("playtutorial");
//																
//																LoadingTipScene* loading_tip = LoadingTipScene::getLoadingTipSceneLayer();
//																CCNode* t_main_node = loading_tip->getChildByTag(9999);
//																if(t_main_node)
//																{
//																	t_main_node->setScale(myDSH->screen_convert_rate);
//																	t_main_node->setPositionY(t_main_node->getPositionY()-160+myDSH->ui_center_y);
//																}
//																
//																addChild(loading_tip, 999);
//															});
											  
											  t_suction->removeFromParent();
											  t_sm->removeFromParent();
										  };
										  t_suction->is_on_touch_began_func = true;
										  t_suction->setTouchEnabled(true);
										  addChild(t_suction);
									  }));
								  }));
							  }));
						  }));
					  }, CCSizeMake(350,100), ccp(0,-110), 12, true);
	}
}

void PlayTutorial::resultAchieve(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		
		t_loading->removeFromParent();
		
		mySGD->setNextSceneName("mainflow");
		
		AudioEngine::sharedInstance()->stopSound();
		AudioEngine::sharedInstance()->unloadEffectScene("playtutorial");
		
		LoadingTipScene* loading_tip = LoadingTipScene::getLoadingTipSceneLayer();
		CCNode* t_main_node = loading_tip->getChildByTag(9999);
		if(t_main_node)
		{
			t_main_node->setScale(myDSH->screen_convert_rate);
			t_main_node->setPositionY(t_main_node->getPositionY()-160+myDSH->ui_center_y);
		}
		
		addChild(loading_tip, 999);
		
		addChild(KSTimer::create(0.1f, [=]()
								 {
									 t_scenario_node->removeFromParent();
								 }));
	}
	else
	{
		mySGD->network_check_cnt++;
		
		if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
		{
			mySGD->network_check_cnt = 0;
			
			ASPopupView* alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4), [=](){
				myAchieve->updateAchieve(json_selector(this, PlayTutorial::resultAchieve));
			}, 1);
			
			//											  ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
			//												  tryTransaction(t_loading);
			//											  });
			if(alert)
				((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
									 {
										 myAchieve->updateAchieve(json_selector(this, PlayTutorial::resultAchieve));
									 }));
		}
	}
}

void PlayTutorial::startCatching()
{
	schedule(schedule_selector(PlayTutorial::catchingAction));
}
void PlayTutorial::catchingAction()
{
	if(getMapData(40, height_value/2) == kTutoMapType_get || getMapData(40, height_value/2) == kTutoMapType_line)
	{
		stopCatching();
	}
}
void PlayTutorial::stopCatching()
{
	unschedule(schedule_selector(PlayTutorial::catchingAction));
	controler->stopMySchedule();
	controler->resetTouch();
	nextStep();
}

TutoMapType PlayTutorial::getMapData(int x, int y)
{
	if(!(x >= 0 && x < 240) || !(y >= 0 && y < height_value))
	{
		return kTutoMapType_empty;
	}
//	CCAssert(x >= 0 && x < 240, "invalid x");
//	CCAssert(y >= 0 && y < height_value, "invalid y");
	
	return map_data[y*240+x];
}
void PlayTutorial::setMapData(int x, int y, TutoMapType value)
{
	CCAssert(x >= 0 && x < 240, "invalid x");
	CCAssert(y >= 0 && y < height_value, "invalid y");
	
	map_data[y*240+x] = value;
}

void PlayTutorial::startBackTracking()
{
	CCLOG("start backtracking");
	
	controler->isBacking = true;
	back_tracking_cnt = 0;
	ing_back_tracking_cnt = 0;
	schedule(schedule_selector(PlayTutorial::backTracking));
}
void PlayTutorial::backTracking()
{
	back_tracking_cnt += mySGD->rewind_cnt_per_frame.getV();
	while(back_tracking_cnt >= ing_back_tracking_cnt + 1)
	{
		++ing_back_tracking_cnt;
		IntPoint afterJackPoint = path_manager->pathBackTracking();
		
		if(afterJackPoint.isNull())
		{
			stopBackTracking();
			return;
		}
		else
		{
			character->setCharacterPoint(afterJackPoint);
		}
	}
	
//	if(mySGD->rewind_cnt_per_frame >= 2)
//	{
//		afterJackPoint = path_manager->pathBackTracking();
//		
//		if(afterJackPoint.isNull())
//		{
//			stopBackTracking();
//			return;
//		}
//		else
//		{
//			character->setCharacterPoint(afterJackPoint);
//		}
//	}
//	else
//		return;
//	
//	if(mySGD->rewind_cnt_per_frame >= 3)
//	{
//		afterJackPoint = path_manager->pathBackTracking();
//		
//		if(afterJackPoint.isNull())
//		{
//			stopBackTracking();
//			return;
//		}
//		else
//		{
//			character->setCharacterPoint(afterJackPoint);
//		}
//	}
//	else
//		return;
}
void PlayTutorial::stopBackTracking()
{
	CCLOG("stop backtracking");
	unschedule(schedule_selector(PlayTutorial::backTracking));
	controler->isBacking = false;
}

void PlayTutorial::scanMap()
{
	for(int i=0;i<240;i++)
	{
		if(getMapData(i, 0) == kTutoMapType_empty)
			bfsCheck(kTutoMapType_empty, kTutoMapType_scanningEmpty, IntPoint(i, 0));
		if(getMapData(i, height_value-1) == kTutoMapType_empty)
			bfsCheck(kTutoMapType_empty, kTutoMapType_scanningEmpty, IntPoint(i, height_value-1));
	}
	
	for(int j=0;j<height_value;j++)
	{
		if(getMapData(0, j) == kTutoMapType_empty)
			bfsCheck(kTutoMapType_empty, kTutoMapType_scanningEmpty, IntPoint(0, j));
		if(getMapData(240-1, j) == kTutoMapType_empty)
			bfsCheck(kTutoMapType_empty, kTutoMapType_scanningEmpty, IntPoint(240-1, j));
	}
	
	// new inside check
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<height_value;j++)
		{
			if(getMapData(i, j) == kTutoMapType_empty)
				bfsCheck(kTutoMapType_empty, kTutoMapType_newGet, IntPoint(i, j));
		}
	}
	
	// outside recovery and new inside add show
//	int newInsideCnt = 0;
//	int sil_inside_cnt = 0;
//	int empty_inside_cnt = 0;
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<height_value;j++)
		{
			if(getMapData(i, j) == kTutoMapType_scanningEmpty)
				setMapData(i, j, kTutoMapType_empty);
			else if(getMapData(i, j) == kTutoMapType_newLine)
			{
				setMapData(i, j, kTutoMapType_line);
//				newInsideCnt++;
//				if(mySD->silData[i][j])		sil_inside_cnt++;
//				else						empty_inside_cnt++;
			}
			else if(getMapData(i, j) == kTutoMapType_newGet)
			{
				setMapData(i, j, kTutoMapType_get);
//				newInsideCnt++;
//				if(mySD->silData[i][j])		sil_inside_cnt++;
//				else						empty_inside_cnt++;
			}
		}
	}
	
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<height_value;j++)
		{
			if(getMapData(i, j) == kTutoMapType_line &&
			   getMapData(i-1, j) != kTutoMapType_empty && getMapData(i-1, j+1) != kTutoMapType_empty &&
			   getMapData(i-1, j-1) != kTutoMapType_empty && getMapData(i, j+1) != kTutoMapType_empty &&
			   getMapData(i, j-1) != kTutoMapType_empty && getMapData(i+1, j+1) != kTutoMapType_empty &&
			   getMapData(i+1, j) != kTutoMapType_empty && getMapData(i+1, j-1) != kTutoMapType_empty &&
			   
			   i >= 0 && i < 240 && j >= 0 && j < height_value)
			{
				setMapData(i, j, kTutoMapType_get);
			}
		}
	}
	
	for(int i=0;i<240;i++)
	{
		if(getMapData(i, 0) != kTutoMapType_empty)
			setMapData(i, 0, kTutoMapType_line);
		if(getMapData(i, height_value-1) != kTutoMapType_empty)
			setMapData(i, height_value-1, kTutoMapType_line);
	}
	for(int j=0;j<height_value;j++)
	{
		if(getMapData(0, j) != kTutoMapType_empty)
			setMapData(0, j, kTutoMapType_line);
		if(getMapData(240-1, j) != kTutoMapType_empty)
			setMapData(240-1, j, kTutoMapType_line);
	}
	
	resetRects();
}

void PlayTutorial::bfsCheck(TutoMapType beforeType, TutoMapType afterType, IntPoint startPoint)
{
	T_BFS_Point s_p;
	s_p.x = startPoint.x;
	s_p.y = startPoint.y;
	queue<T_BFS_Point> bfsArray;
	setMapData(s_p.x, s_p.y, afterType);
	bfsArray.push(s_p);
	
	while(!bfsArray.empty())
	{
		T_BFS_Point t_p = bfsArray.front();
		bfsArray.pop();
		for(int i=directionLeft;i<=directionUp;i+=2)
		{
			IntVector t_iv = IntVector::directionVector((IntDirection)i);
			
			T_BFS_Point t_v;
			t_v.x = t_iv.dx;
			t_v.y = t_iv.dy;
			
			T_BFS_Point a_p;
			a_p.x = t_p.x+t_v.x;
			a_p.y = t_p.y+t_v.y;
			
			if(a_p.x >= 0 && a_p.x < 240 && a_p.y >= 0 && a_p.y < height_value && getMapData(a_p.x, a_p.y) == beforeType)
			{
				setMapData(a_p.x, a_p.y, afterType);
				bfsArray.push(a_p);
			}
		}
	}
}

void PlayTutorial::resetRects()
{
	CCArray* rects = CCArray::createWithCapacity(256);
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<height_value;j++)
		{
			if(getMapData(i, j) == kTutoMapType_line || getMapData(i, j) == kTutoMapType_get)
			{
				IntRect* t_rect = newRectChecking(IntMoveState(i, j, directionRightUp));
				rects->addObject(t_rect);
			}
		}
	}
	
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<height_value;j++)
		{
			if(getMapData(i, j) == kTutoMapType_checkLine)
				setMapData(i, j, kTutoMapType_line);
			else if(getMapData(i, j) == kTutoMapType_checkGet)
				setMapData(i, j, kTutoMapType_get);
		}
	}
	
	view_img->setDrawRects(rects);
	AudioEngine::sharedInstance()->playEffect("se_area.mp3", false);
}

IntRect* PlayTutorial::newRectChecking(IntMoveState start)
{
	IntPoint origin = IntPoint(start.origin.x, start.origin.y);
	IntSize size = IntSize(0, 0);
	
	bool isUpper = true;
	bool isRighter = true;
	queue<IntMoveState> loopArray;
	loopArray.push(start);
	
	queue<IntMoveState> nextLoopArray;
	
	//	int loopCnt;
	
	while(!loopArray.empty())
	{
		if(isUpper)				size.height++;
		if(isRighter)			size.width++;
		
		bool upable = isUpper;
		bool rightable = isRighter;
		
		while(!loopArray.empty())
		{
			//			loopCnt++;
			IntMoveState t_ms = loopArray.front();
			loopArray.pop();
			
			
			if(t_ms.direction == directionUp && !isUpper)
				continue;
			if(t_ms.direction == directionRight && !isRighter)
				continue;
			
			if(getMapData(t_ms.origin.x, t_ms.origin.y) == kTutoMapType_get)
				setMapData(t_ms.origin.x, t_ms.origin.y, kTutoMapType_checkGet);
			else if(getMapData(t_ms.origin.x, t_ms.origin.y) == kTutoMapType_line)
				setMapData(t_ms.origin.x, t_ms.origin.y, kTutoMapType_checkLine);
			
			if(t_ms.direction == directionUp)
			{
				if(isUpper)
				{
					IntMoveState n_msUp = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
					if(n_msUp.origin.x >= 0 && n_msUp.origin.x < 240 && n_msUp.origin.y >= 0 && n_msUp.origin.y < height_value &&
					   (getMapData(n_msUp.origin.x, n_msUp.origin.y) == kTutoMapType_line || getMapData(n_msUp.origin.x, n_msUp.origin.y) == kTutoMapType_get))
						nextLoopArray.push(n_msUp);
					else		upable = false;
				}
			}
			else if(t_ms.direction == directionRight)
			{
				if(isRighter)
				{
					IntMoveState n_msRight = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
					if(n_msRight.origin.x >= 0 && n_msRight.origin.x < 240 && n_msRight.origin.y >= 0 && n_msRight.origin.y < height_value &&
					   (getMapData(n_msRight.origin.x, n_msRight.origin.y) == kTutoMapType_line || getMapData(n_msRight.origin.x, n_msRight.origin.y) == kTutoMapType_get))
						nextLoopArray.push(n_msRight);
					else		rightable = false;
				}
			}
			else if(t_ms.direction == directionRightUp)
			{
				if(isUpper)
				{
					IntMoveState n_msUp = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
					if(n_msUp.origin.x >= 0 && n_msUp.origin.x < 240 && n_msUp.origin.y >= 0 && n_msUp.origin.y < height_value &&
					   (getMapData(n_msUp.origin.x, n_msUp.origin.y) == kTutoMapType_line || getMapData(n_msUp.origin.x, n_msUp.origin.y) == kTutoMapType_get))
						nextLoopArray.push(n_msUp);
					else		upable = false;
				}
				
				if(isRighter)
				{
					IntMoveState n_msRight = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
					if(n_msRight.origin.x >= 0 && n_msRight.origin.x < 240 && n_msRight.origin.y >= 0 && n_msRight.origin.y < height_value &&
					   (getMapData(n_msRight.origin.x, n_msRight.origin.y) == kTutoMapType_line || getMapData(n_msRight.origin.x, n_msRight.origin.y) == kTutoMapType_get))
						nextLoopArray.push(n_msRight);
					else		rightable = false;
				}
				
				if(upable && rightable)
				{
					IntMoveState n_msRightUp = IntMoveState(t_ms.origin.x+1, t_ms.origin.y+1, directionRightUp);
					if(n_msRightUp.origin.x >= 0 && n_msRightUp.origin.x < 240 && n_msRightUp.origin.y >= 0 && n_msRightUp.origin.y < height_value &&
					   (getMapData(n_msRightUp.origin.x, n_msRightUp.origin.y) == kTutoMapType_line || getMapData(n_msRightUp.origin.x, n_msRightUp.origin.y) == kTutoMapType_get))
						nextLoopArray.push(n_msRightUp);
					else		rightable = false;
				}
			}
		}
		
		isUpper = upable;
		isRighter = rightable;
		
		if(isUpper || isRighter)
		{
			while(!nextLoopArray.empty())
			{
				loopArray.push(nextLoopArray.front());
				nextLoopArray.pop();
			}
		}
	}
	
	//	CCLOG("loop count : %d", loopCnt);
	
	IntRect* r_rect = new IntRect(origin.x*pixelSize, origin.y*pixelSize, size.width*pixelSize, size.height*pixelSize);
	r_rect->autorelease();
	return r_rect;
}