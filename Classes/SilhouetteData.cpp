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
	string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(CCString::createWithFormat("stage%d_level1_sildata.txt", myType)->getCString());
	
	unsigned long buff = 0;
	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rt", &buff);
	
	string t_sil((char*)pBuffer);
	
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
	string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(CCString::createWithFormat("stage%d_level2_sildata.txt", myType)->getCString());
	
	unsigned long buff = 0;
	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rt", &buff);
	
	string t_sil((char*)pBuffer);
	
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
	string return_value;
	if(t_type == 1)
	{
		if(level == 1)			return_value = "이 드레스는 조금 야하지 않아?";
		else if(level == 2)		return_value = "전화번호는 가르쳐드릴 수 없어요.";
		else if(level == 3)		return_value = "계속 그런 눈으로 쳐다볼 건가요?";
	}
	else if(t_type == 2)
	{
		if(level == 1)			return_value = "선물 고마워.(이게 끝은 아니겠지?)";
		else if(level == 2)		return_value = "루돌프, 다음은 어디야?";
		else if(level == 3)		return_value = "다음 스케쥴은 어디죠?";
	}
	else if(t_type == 3)
	{
		if(level == 1)			return_value = "주사 맞을 시간이예요. 잠시만요.";
		else if(level == 2)		return_value = "이번엔 엉덩이가 아니라 팔이랍니다.";
		else if(level == 3)		return_value = "이번엔 조금 따끔할거예요.";
	}
	else if(t_type == 4)
	{
		if(level == 1)			return_value = "어때? 아침에 나오니 상쾌하지?";
		else if(level == 2)		return_value = "조금만 더! 우리가 이길 수 있어!";
		else if(level == 3)		return_value = "여길 봐. 예쁘게 찍어줄게.";
	}
	else if(t_type == 5)
	{
		if(level == 1)			return_value = "아 떨려.. 과연 잘할 수 있을까..";
		else if(level == 2)		return_value = "모두 준비 됐어? 자! 신나게 가보자구!!";
		else if(level == 3)		return_value = "무대 위에 서면 다른 내가 되는 것 같아!";
	}
	else if(t_type == 6)
	{
		if(level == 1)			return_value = "어머! 어딜 보는거야!! 변태!!";
		else if(level == 2)		return_value = "오늘 새로 오신 환자분이죠??";
		else if(level == 3)		return_value = "아! 당신은 아까 그 변태!!";
	}
	else if(t_type == 7)
	{
		if(level == 1)			return_value = "내년 봄에는 나갈 수 있을까?";
		else if(level == 2)		return_value = "너 뭐야! 왜 남의 병실을 기웃거리는거야?";
		else if(level == 3)		return_value = "이거 나 주는거야? 고마워, 잘 어울려?";
	}
	else if(t_type == 8)
	{
		if(level == 1)			return_value = "병원에만 있기 답답해. 나가서 놀아요!!";
		else if(level == 2)		return_value = "후에에에엥... 아파!!";
		else if(level == 3)		return_value = "절대 사탕 때문에 그친거 아니예요.";
	}
	
	return return_value;
}
