//
//  SelectedMapData.h
//  DrawingJack
//
//  Created by 사원3 on 12. 12. 13..
//
//

#ifndef DrawingJack_SelectedMapData_h
#define DrawingJack_SelectedMapData_h

#include "cocos2d.h"
#include "BalanceData.h"
#include "DataStorageHub.h"

using namespace cocos2d;

enum ImageNumber{
	kIN_backObj = 1,
	kIN_backPattern,
	kIN_baseMissile,
	kIN_chapterMenu,
	kIN_stageMenu,
	kIN_bonusImage,
	kIN_mainCumber,
	kIN_subCumber
};

/*			chapter info
 1~10 : easy
 11~20 : hard(noShield)
 21~26 : easy
 31~36 : normal
 41~46 : hard(noShield)
*/

/*			public image number
 1~20
 back obj, back pattern, base missile, chapter menu, stage menu
	1,2		->	1
	3,4		->	2
	5,6		->	3
	7,8		->	4
	9,10	->	5
	11,12	->	11
	13,14	->	12
	15,16	->	13
	17,18	->	14
	19,20	->	15
 
 21~26, 31~36, 41~46
 back obj X
 base missile X(넣는다면 ?n 통일 : (21,31,41), (22,32,42), ...)
 back pattern	따로
 chapter menu	따로
 stage menu		따로
*/

class SelectedMapData : public CCObject
{
public:
	static SelectedMapData* sharedInstance()
	{
		static SelectedMapData* mySMD = NULL;
		if(mySMD == NULL)
		{
			mySMD = new SelectedMapData();
			mySMD->myInit();
		}
		return mySMD;
	}
	
	void setAttackPattern(RandomSelector* myRS)
	{
		myBD->setAttackPattern(myRS, selected_chapter, selected_stage);
	}
	
	int getAttackRate()
	{
		return myBD->getAttackRate(selected_chapter, selected_stage);
	}
	
	int getSubCumberCnt()
	{
		return myBD->getSubCumberCnt(selected_chapter, selected_stage);
	}
	
	int getSelectedChapter()
	{
		return selected_chapter;
	}
	
	int getViewChapterNumber()
	{
		int r_value;
		if(selected_chapter <= 20)
			r_value = (selected_chapter-1)%10+1;
		else
			r_value = selected_chapter%10+10;
		
		return r_value;
	}
	
	int getViewChapterNumberForChapter(int t_chapter)
	{
		int r_value;
		if(t_chapter <= 20)
			r_value = (t_chapter-1)%10+1;
		else
			r_value = t_chapter%10+10;
		
		return r_value;
	}
	
	int getViewChapterSubNumber()
	{
		int r_value;
		if(selected_chapter <= 20)
			r_value = (selected_chapter-1)/10 + 1;
		else
			r_value = (selected_chapter-20)/10 + 1;
		
		return r_value;
	}
	
	int getViewChapterSubNumberForChapter(int t_chapter)
	{
		int r_value;
		if(t_chapter <= 20)
			r_value = (t_chapter-1)/10 + 1;
		else
			r_value = (t_chapter-20)/10 + 1;
		
		return r_value;
	}
	
	int getNextChapterNumber()
	{
		int r_value;
		if(selected_chapter < 10)
			r_value = selected_chapter+1;
		else if(selected_chapter == 10)
			r_value = 21;
		else if(selected_chapter >= 21 && selected_chapter <= 26)
			r_value = selected_chapter + 10;
		else if(selected_chapter >= 31 && selected_chapter <= 36)
			r_value = selected_chapter - 10 + 1;
		else
			r_value = selected_chapter;
		
		return r_value;
	}
	
	int getImageNumber(ImageNumber t_i)
	{
		if(selected_chapter <= 20)
		{
			if(t_i == kIN_bonusImage || t_i == kIN_mainCumber || t_i == kIN_subCumber)
			{
				return selected_chapter;
			}
			else
			{
				int last_p = ((selected_chapter-1)%10)/2 + 1;
				if(t_i != kIN_stageMenu && selected_chapter > 10)
					last_p += 10;
				return last_p;
			}
		}
		else
		{
			CCAssert(t_i != kIN_backObj, "not use backObj");
			if(t_i == kIN_baseMissile)
			{
				return getViewChapterNumber()-10+20;
			}
			else
			{
				return selected_chapter;
			}
		}
	}
	
	int getImageNumberForChapter(ImageNumber t_i, int t_chapter)
	{
		if(t_chapter <= 20)
		{
			if(t_i == kIN_bonusImage || t_i == kIN_mainCumber || t_i == kIN_subCumber)
			{
				return t_chapter;
			}
			else
			{
				int last_p = ((t_chapter-1)%10)/2 + 1;
				if(t_chapter > 10) // t_i != kIN_stageMenu &&
					last_p += 10;
				return last_p;
			}
		}
		else
		{
			CCAssert(t_i != kIN_backObj, "not use backObj");
			if(t_i == kIN_baseMissile)
			{
				return t_chapter%10+20;
			}
			else
			{
				return t_chapter;
			}
		}
	}
	
	void setSelectedChapter(int chapter_number)
	{
		selected_chapter = chapter_number;
		myDSH->setIntegerForKey(kDSH_Key_lastSelectedChapter, selected_chapter);
		
		if(selected_chapter <= 20 && selected_chapter >= 11)
			is_noShield = true;
		else if(selected_chapter >= 41 && selected_chapter <= 46)
			is_noShield = true;
		else
			is_noShield = false;
	}
	
	bool getIsNoShield()
	{
		return is_noShield;
	}
	
	int getSelectedStage()
	{
		return selected_stage;
	}
	
	void setSelectedStage(int stage_number)
	{
		selected_stage = stage_number;
		myDSH->setIntegerForKey(kDSH_Key_chapter_int1_LastSelectedStage, selected_chapter, selected_stage);
		
	}
	
	float getMaxBossLife(){		return myBD->getMaxBossLife(selected_chapter, selected_stage);	}
	float getClearPercentage(){	return myBD->getClearPercentage(selected_chapter, selected_stage);	}
	float getBossSpeed(){		return myBD->getBossSpeed(selected_chapter, selected_stage);	}
	float getSubCumberSpeed(){	return myBD->getSubCumberSpeed(selected_chapter, selected_stage);	}
	
	BD_P1 getValuePattern1(){	return myBD->getValuePattern1(selected_chapter, selected_stage);	}
	BD_P2 getValuePattern2(){	return myBD->getValuePattern2(selected_chapter, selected_stage);	}
	BD_P3 getValuePattern3(){	return myBD->getValuePattern3(selected_chapter, selected_stage);	}
	BD_P4 getValuePattern4(){	return myBD->getValuePattern4(selected_chapter, selected_stage);	}
	BD_P5 getValuePattern5(){	return myBD->getValuePattern5(selected_chapter, selected_stage);	}
	BD_P8 getValuePattern8(){	return myBD->getValuePattern8(selected_chapter, selected_stage);	}
	BD_P9 getValuePattern9(){	return myBD->getValuePattern9(selected_chapter, selected_stage);	}
	BD_P11 getValuePattern11(){	return myBD->getValuePattern11(selected_chapter, selected_stage);	}
	BD_P14 getValuePattern14(){	return myBD->getValuePattern14(selected_chapter, selected_stage);	}
	BD_P18 getValuePattern18(){	return myBD->getValuePattern18(selected_chapter, selected_stage);	}
	BD_P28 getValuePattern28(){	return myBD->getValuePattern28(selected_chapter, selected_stage);	}
	
	int getSecondSubCumberType(){	return myBD->getSecondSubCumberType(selected_chapter, selected_stage);	}
	int getSecondSubCumberTypeForChapterStage(int t_chapter, int t_stage)
	{
		return myBD->getSecondSubCumberType(t_chapter, t_stage);
	}
	
private:
	BalanceData* myBD;
	
	
	
	int selected_chapter;
	int selected_stage;
	bool is_noShield;
	
	void myInit()
	{
		
		
		selected_chapter = 0;
		selected_stage = 0;
		is_noShield = false;
		
		myBD = BalanceData::sharedInstance();
	}
};


#endif
