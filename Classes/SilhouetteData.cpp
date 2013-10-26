//
//  SilhouetteData.cpp
//  galsprototype
//
//  Created by 사원3 on 13. 8. 27..
//
//

#include "SilhouetteData.h"

void SilhouetteData::startSetting()
{
//	string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(CCString::createWithFormat("stage%d_level1_sildata.txt", myType)->getCString());
//	
//	unsigned long buff = 0;
//	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rt", &buff);
//	
//	string t_sil((char*)pBuffer);
	
	string t_sil = NSDS_GS(kSDS_CI_int1_silImgInfoSilData_s, NSDS_GI(myType, kSDS_SI_level_int1_card_i, 1));
	
	must_cnt = 0;
	
	int i = mapHeightInnerEnd-1;
	int j = mapWidthInnerBegin;
	bool is_zero = true;
	
	int check_number = 0;
	while(i >= mapHeightInnerBegin && j < mapWidthInnerEnd)
	{
		string cnt_str = t_sil.substr(check_number, 1);
		check_number++;
		if(cnt_str[0] >= 'A')
		{
			int adder = cnt_str[0]-'A'+2;
			cnt_str = t_sil.substr(check_number, adder);
			check_number += adder;
		}
		
		int cnt = atoi(cnt_str.c_str());
		if(is_zero)
		{
			must_cnt += cnt;
			for(int k=0;k<cnt;k++)
			{
				silData[j++][i] = true;
				if(j >= mapWidthInnerEnd)
				{
					j = mapWidthInnerBegin;
					i--;
					if(i < mapHeightInnerBegin)
						break;
				}
			}
			is_zero = false;
		}
		else
		{
			for(int k=0;k<cnt;k++)
			{
				silData[j++][i] = false;
				if(j >= mapWidthInnerEnd)
				{
					j = mapWidthInnerBegin;
					i--;
					if(i < mapHeightInnerBegin)
						break;
				}
			}
			is_zero = true;
		}
	}
}

////////////////////////////////////////
void SilhouetteData::exchangeSilhouette()
{
//	string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(CCString::createWithFormat("stage%d_level2_sildata.txt", myType)->getCString());
//	
//	unsigned long buff = 0;
//	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rt", &buff);
//	
//	string t_sil((char*)pBuffer);
	
	string t_sil = NSDS_GS(kSDS_CI_int1_silImgInfoSilData_s, NSDS_GI(myType, kSDS_SI_level_int1_card_i, 2));
	
	must_cnt = 0;
	
	int i = mapHeightInnerEnd-1;
	int j = mapWidthInnerBegin;
	bool is_zero = true;
	
	int check_number = 0;
	while(i >= mapHeightInnerBegin && j < mapWidthInnerEnd)
	{
		string cnt_str = t_sil.substr(check_number, 1);
		check_number++;
		if(cnt_str[0] >= 'A')
		{
			int adder = cnt_str[0]-'A'+2;
			cnt_str = t_sil.substr(check_number, adder);
			check_number += adder;
		}
		
		int cnt = atoi(cnt_str.c_str());
		if(is_zero)
		{
			must_cnt += cnt;
			for(int k=0;k<cnt;k++)
			{
				silData[j++][i] = true;
				if(j >= mapWidthInnerEnd)
				{
					j = mapWidthInnerBegin;
					i--;
					if(i < mapHeightInnerBegin)
						break;
				}
			}
			is_zero = false;
		}
		else
		{
			for(int k=0;k<cnt;k++)
			{
				silData[j++][i] = false;
				if(j >= mapWidthInnerEnd)
				{
					j = mapWidthInnerBegin;
					i--;
					if(i < mapHeightInnerBegin)
						break;
				}
			}
			is_zero = true;
		}
	}
}

string SilhouetteData::getScriptString(int level)
{
	return getScriptString(myType, level);
}
string SilhouetteData::getScriptString(int t_type, int level)
{
	return NSDS_GS(kSDS_CI_int1_script_s, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, level));
}
