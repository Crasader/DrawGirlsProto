//
//  DownloadFile.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 10..
//
//

#ifndef __DGproto__DownloadFile__
#define __DGproto__DownloadFile__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define CCSTR_CWF CCString::createWithFormat

class DownloadFile
{
public:
	int size;
	string img;
	string filename;
	string key;
	
	bool operator==(const DownloadFile& right) const
	{
		return /*img == right.img && */filename == right.filename;
	}
};

//class CopyFile
//{
//public:
//	string from_filename;
//	string to_filename;
//	bool is_ani;
//	
//	int cut_width;
//	int cut_height;
//	int position_x;
//	int position_y;
//	
//	string ani_filename;
//	bool is_ccb;
//	string ccb_filename;
//	
//	CopyFile() :
//	is_ani(false),
//	cut_width(0),
//	cut_height(0),
//	position_x(0),
//	position_y(0),
//	ani_filename(""),
//	is_ccb(false),
//	ccb_filename("")
//	{
//		
//	}
//};

#endif /* defined(__DGproto__DownloadFile__) */
