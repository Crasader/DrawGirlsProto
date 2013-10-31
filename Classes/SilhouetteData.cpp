//
//  SilhouetteData.cpp
//  galsprototype
//
//  Created by 사원3 on 13. 8. 27..
//
//

#include "SilhouetteData.h"
#include "StageImgLoader.h"

void SilhouetteData::startSetting()
{
	must_cnt = 0;
	CCImage *tx = new CCImage;
	tx->initWithImageFileThreadSafe(CCString::createWithFormat((mySIL->getDocumentPath()+"stage%d_level%d_invisible.png").c_str(), myType, 1)->getCString());
	
	unsigned char* pData = (unsigned char*)tx->getData();    //원본 이미지 데이터
	
	for(int y=0;y<215;y++){
		for(int x=0;x<160;x++){
			
			int offset_x = 2;
			int offset_y = 2;
			
			if(y==214)offset_y = 1;
			if(x==159)offset_x = 1;
			
			int i = (160*4*(y*4+offset_y))+x*4+offset_x;
			
			//순서대로 r,g,b 값을 더하여 10 이상일때 실루엣으로 인식
			if(pData[i*4]+pData[i*4+1]+pData[i*4+2]>=10)
			{
				must_cnt++;
				silData[x+1][215-y]=true;
			}else{
				silData[x+1][215-y]=false;
			}
		}
	}
	
	tx->release();
	
//	for(int y=216;y>=0;y--){
//		string xprint="";
//		for(int x=0;x<162;x++){
//			if(silData[x][y]==false){
//				xprint+="0";
//			}else{
//				xprint+="1";
//			}
//		}
//		
//		CCLog("%s",xprint.c_str());
//	}
	
//	string t_sil = NSDS_GS(kSDS_CI_int1_silImgInfoSilData_s, NSDS_GI(myType, kSDS_SI_level_int1_card_i, 1));
//	
//	must_cnt = 0;
//	
//	int i = mapHeightInnerEnd-1;
//	int j = mapWidthInnerBegin;
//	bool is_zero = true;
//	
//	int check_number = 0;
//	while(i >= mapHeightInnerBegin && j < mapWidthInnerEnd)
//	{
//		string cnt_str = t_sil.substr(check_number, 1);
//		check_number++;
//		if(cnt_str[0] >= 'A')
//		{
//			int adder = cnt_str[0]-'A'+2;
//			cnt_str = t_sil.substr(check_number, adder);
//			check_number += adder;
//		}
//		
//		int cnt = atoi(cnt_str.c_str());
//		if(is_zero)
//		{
//			must_cnt += cnt;
//			for(int k=0;k<cnt;k++)
//			{
//				silData[j++][i] = true;
//				if(j >= mapWidthInnerEnd)
//				{
//					j = mapWidthInnerBegin;
//					i--;
//					if(i < mapHeightInnerBegin)
//						break;
//				}
//			}
//			is_zero = false;
//		}
//		else
//		{
//			for(int k=0;k<cnt;k++)
//			{
//				silData[j++][i] = false;
//				if(j >= mapWidthInnerEnd)
//				{
//					j = mapWidthInnerBegin;
//					i--;
//					if(i < mapHeightInnerBegin)
//						break;
//				}
//			}
//			is_zero = true;
//		}
//	}
}

////////////////////////////////////////
void SilhouetteData::exchangeSilhouette()
{
	must_cnt = 0;
	CCImage *tx = new CCImage;
	tx->initWithImageFileThreadSafe(CCString::createWithFormat((mySIL->getDocumentPath()+"stage%d_level%d_invisible.png").c_str(), myType, 2)->getCString());
	
	unsigned char* pData = (unsigned char*)tx->getData();    //원본 이미지 데이터
	
	for(int y=0;y<215;y++){
		for(int x=0;x<160;x++){
			
			int offset_x = 2;
			int offset_y = 2;
			
			if(y==214)offset_y = 1;
			if(x==159)offset_x = 1;
			
			int i = (160*4*(y*4+offset_y))+x*4+offset_x;
			
			//순서대로 r,g,b 값을 더하여 10 이상일때 실루엣으로 인식
			if(pData[i*4]+pData[i*4+1]+pData[i*4+2]>=10)
			{
				must_cnt++;
				silData[x+1][215-y]=true;
			}else{
				silData[x+1][215-y]=false;
			}
		}
	}
	
	tx->release();
	
//	string t_sil = NSDS_GS(kSDS_CI_int1_silImgInfoSilData_s, NSDS_GI(myType, kSDS_SI_level_int1_card_i, 2));
//	
//	must_cnt = 0;
//	
//	int i = mapHeightInnerEnd-1;
//	int j = mapWidthInnerBegin;
//	bool is_zero = true;
//	
//	int check_number = 0;
//	while(i >= mapHeightInnerBegin && j < mapWidthInnerEnd)
//	{
//		string cnt_str = t_sil.substr(check_number, 1);
//		check_number++;
//		if(cnt_str[0] >= 'A')
//		{
//			int adder = cnt_str[0]-'A'+2;
//			cnt_str = t_sil.substr(check_number, adder);
//			check_number += adder;
//		}
//		
//		int cnt = atoi(cnt_str.c_str());
//		if(is_zero)
//		{
//			must_cnt += cnt;
//			for(int k=0;k<cnt;k++)
//			{
//				silData[j++][i] = true;
//				if(j >= mapWidthInnerEnd)
//				{
//					j = mapWidthInnerBegin;
//					i--;
//					if(i < mapHeightInnerBegin)
//						break;
//				}
//			}
//			is_zero = false;
//		}
//		else
//		{
//			for(int k=0;k<cnt;k++)
//			{
//				silData[j++][i] = false;
//				if(j >= mapWidthInnerEnd)
//				{
//					j = mapWidthInnerBegin;
//					i--;
//					if(i < mapHeightInnerBegin)
//						break;
//				}
//			}
//			is_zero = true;
//		}
//	}
}

string SilhouetteData::getScriptString(int level)
{
	return getScriptString(myType, level);
}
string SilhouetteData::getScriptString(int t_type, int level)
{
	return NSDS_GS(kSDS_CI_int1_script_s, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, level));
}
