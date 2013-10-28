//
//  SaveData_android.h
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 4..
//
//

#ifndef DrawingJack_SaveData_android_h
#define DrawingJack_SaveData_android_h
#include "cocos2d.h"
#include "platform/CCFileUtils.h"
#include <string>
USING_NS_CC;
using namespace std;

//void testF(string tt);
//string readF();
void testF(string filePath, string tt);
string readF(string filePath);
void addF(string filePath, string tt);



#endif /* defined(__BasketWorldCup2__SaveData_android__) */
