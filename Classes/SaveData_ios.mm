//
//  SaveData.m
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 4..
//
//

#import "SaveData_ios.h"

#include <iostream>
using namespace std;
//void testF(string tt)
//{
//	string filePath = "KSDATA";
//	NSArray* paths = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES );
//	NSString* documentsDirectory = [paths objectAtIndex:0];
//	NSString* nsfilePath = [NSString stringWithUTF8String:filePath.c_str()];
//	NSString* fullFileName = [NSString stringWithFormat:@"%@/%@", documentsDirectory, nsfilePath ];
//	const char* szFilePath = [fullFileName UTF8String];
//	FILE* fp = fopen( szFilePath, "w" );
//	fprintf(fp, "%s", tt.c_str());
//	fclose(fp);
//}
//
//string readF()
//{
//	string filePath = "KSDATA";
//	NSArray* paths = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES );
//	NSString* documentsDirectory = [paths objectAtIndex:0];
//	NSString* nsfilePath = [NSString stringWithUTF8String:filePath.c_str()];
//	NSString* fullFileName = [NSString stringWithFormat:@"%@/%@", documentsDirectory, nsfilePath ];
//	const char* szFilePath = [fullFileName UTF8String];
//	FILE* fp = fopen( szFilePath, "r" );
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
//		return "";
//	}
//	
//	return result;
//}

int testF(string filePath, string tt)
{
    int return_value = 0;
    
	NSArray* t_paths = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES );
	NSString* t_documentsDirectory = [t_paths objectAtIndex:0];
	NSString* t_nsfilePath = [NSString stringWithUTF8String:filePath.c_str()];
	NSString* t_fullFileName = [NSString stringWithFormat:@"%@/%@", t_documentsDirectory, t_nsfilePath ];
	const char* t_szFilePath = [t_fullFileName UTF8String];
	FILE* t_fp = fopen( t_szFilePath, "w" );
	return_value = fprintf(t_fp, "%s", tt.c_str());
	fclose(t_fp);
	
    return return_value;
	
//	NSArray* paths = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES );
//	NSString* documentsDirectory = [paths objectAtIndex:0];
//	NSString* nsfilePath = [NSString stringWithUTF8String:filePath.c_str()];
//	NSString* fullFileName = [NSString stringWithFormat:@"%@/%@", documentsDirectory, nsfilePath ];
//	const char* szFilePath = [fullFileName UTF8String];
//	
//	int rename_result;
//	rename_result = rename(t_szFilePath, szFilePath);
//	
//	if(rename_result != 0)
//	{
//		// file fail(rename)
//	}
	
//	FILE* fp = fopen( szFilePath, "w" );
//	fprintf(fp, "%s", tt.c_str());
//	fclose(fp);
}
string readF(string filePath)
{
	NSArray* paths = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES );
	NSString* documentsDirectory = [paths objectAtIndex:0];
	NSString* nsfilePath = [NSString stringWithUTF8String:filePath.c_str()];
	NSString* fullFileName = [NSString stringWithFormat:@"%@/%@", documentsDirectory, nsfilePath ];
	const char* szFilePath = [fullFileName UTF8String];
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
void addF(string filePath, string tt)
{
	NSArray* paths = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES );
	NSString* documentsDirectory = [paths objectAtIndex:0];
	NSString* nsfilePath = [NSString stringWithUTF8String:filePath.c_str()];
	NSString* fullFileName = [NSString stringWithFormat:@"%@/%@", documentsDirectory, nsfilePath ];
	const char* szFilePath = [fullFileName UTF8String];
	FILE* fp = fopen( szFilePath, "a" );
	fprintf(fp, "%s", tt.c_str());
	fclose(fp);
}