//
//  SaveData_android.cpp
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 4..
//
//



#include "SaveData_android.h"



/*
 
 string filePath = "KSDATA";
 NSArray* paths = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES );
 NSString* documentsDirectory = [paths objectAtIndex:0];
 NSString* nsfilePath = [NSString stringWithUTF8String:filePath.c_str()];
 NSString* fullFileName = [NSString stringWithFormat:@"%@/%@", documentsDirectory, nsfilePath ];
 const char* szFilePath = [fullFileName UTF8String];
 FILE* fp = fopen( szFilePath, "w" );
 fprintf(fp, "%s", tt.c_str());
 fclose(fp);
 }
 
 string readF()
 {
 string filePath = "KSDATA";
 NSArray* paths = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES );
 NSString* documentsDirectory = [paths objectAtIndex:0];
 NSString* nsfilePath = [NSString stringWithUTF8String:filePath.c_str()];
 NSString* fullFileName = [NSString stringWithFormat:@"%@/%@", documentsDirectory, nsfilePath ];
 const char* szFilePath = [fullFileName UTF8String];
 cout << szFilePath << endl;
 FILE* fp = fopen( szFilePath, "r" );
 string result;
 if(fp)
 {
 fseek(fp, 0L, SEEK_END);
 int s = ftell(fp);
 fseek(fp, 0L, SEEK_SET);
 char* buffer = new char[s+1];
 memset(buffer, 0, sizeof(s+1));
 fread(buffer, sizeof(char), s, fp);
 fclose(fp);
 
 
 result = buffer;
 delete [] buffer;
 }
 else
 {
 return "";
 }
 
 return result;
 }
 */


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//void testF(string tt)
//{
//	
//	string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "KSDATA";
//	FILE* fp = fopen( path.c_str(), "w" );
//	if(!fp)
//	{
//		CCLOG("file failure");
//	}
//	fprintf(fp, "%s", tt.c_str());
//	fclose(fp);
//}
//string readF()
//{
//	string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "KSDATA";
//	FILE* fp = fopen( path.c_str(), "r" );
//	string result;
//	if(fp)
//	{
//		fseek(fp, 0L, SEEK_END);
//		int s = ftell(fp);
//		fseek(fp, 0L, SEEK_SET);
//		char* buffer = new char[s+1];
//		memset(buffer, 0, sizeof(s+1));
//		fread(buffer, sizeof(char), s, fp);
//		fclose(fp);
//		
//		
//		result = buffer;
//		delete [] buffer;
//	}
//	else
//	{
//		CCLOG("read fail");
//		return "";
//	}
//	
//	return result;
//}

void testF(string filePath, string tt)
{
	string t_path = CCFileUtils::sharedFileUtils()->getWritablePath() + "temp";
	FILE* t_fp = fopen( t_path.c_str(), "w" );
	if(!t_fp)
	{
		CCLOG("file failure");
	}
	fprintf(t_fp, "%s", tt.c_str());
	fclose(t_fp);
	
	string path = CCFileUtils::sharedFileUtils()->getWritablePath() + filePath;
	int rename_result;
	rename_result = rename( t_path.c_str(), path.c_str() );
	if(rename_result != 0)
		CCLOG("file failure(rename)");
}
string readF(string filePath)
{
	string path = CCFileUtils::sharedFileUtils()->getWritablePath() + filePath;
	FILE* fp = fopen( path.c_str(), "r" );
	string result;
	if(fp)
	{
		fseek(fp, 0L, SEEK_END);
		int s = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		char* buffer = new char[s+1];
		memset(buffer, 0, sizeof(s+1));
		fread(buffer, sizeof(char), s, fp);
		fclose(fp);
		
		
		result = buffer;
		delete [] buffer;
	}
	else
	{
		CCLOG("read fail");
		return "";
	}
	
	return result;
}
void addF(string filePath, string tt)
{
	string path = CCFileUtils::sharedFileUtils()->getWritablePath() + filePath;
	FILE* fp = fopen( path.c_str(), "a" );
	if(!fp)
	{
		CCLOG("file failure");
	}
	fprintf(fp, "%s", tt.c_str());
	fclose(fp);
}

#endif
