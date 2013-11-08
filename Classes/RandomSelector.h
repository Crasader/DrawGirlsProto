//
//  RandomSelector.h
//  DrawingJack
//
//  Created by 사원3 on 12. 12. 22..
//
//

#ifndef DrawingJack_RandomSelector_h
#define DrawingJack_RandomSelector_h

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;


enum class StateType : unsigned short
{
	ABC = 1,
};
	

//typedef AP_CODE std::string;
enum AP_CODE{
	kNonTargetAttack1=1,
	kNonTargetAttack2,
	kNonTargetAttack3,
	kNonTargetAttack4,
	kNonTargetAttack5,
	kNonTargetAttack6,
	kNonTargetAttack7,
	kNonTargetAttack8,
	kNonTargetAttack9, // 당구공 // ok
	kNonTargetAttack10, //
	
	kTargetAttack1=101,
	kTargetAttack2,
	kTargetAttack3,
	kTargetAttack4,
	kTargetAttack5, // 나무 토막 // ok
	kTargetAttack6, // 사과 던지기 // 14 번... // ok
	kTargetAttack7,// 태양 광선 // ok
	kTargetAttack8, // 슈퍼 타입... 다용도.
	kTargetAttack9, // 몸통박치기
	kSpecialAttack1 = 1001, // 먹물 // 21
	kSpecialAttack2, // 태양권 // 24
	kSpecialAttack3, // 거미줄 // 23
	kSpecialAttack4, // 감옥   // 28
	kSpecialAttack5, // 빙결   // 26
	kSpecialAttack6, // 혼란, 반대 컨트롤. // 33
	kSpecialAttack7, // 텔레포트.          // 32
	
	kSpecialAttack8, // 인비지블
	kSpecialAttack9, // 불지르기
	kSpecialAttack10, // 위성빔
	kSpecialAttack11, // 레이저 스캔
	kSpecialAttack12, // 방사능 라인
	kSpecialAttack13, // 메테오.
	kSpecialAttack14, // 떨어지는 돌.
	kSpecialAttack15, // 풍차
	kSpecialAttack16, // 다이너마이트
	kAP_CODE_pattern0 =999990,
	kAP_CODE_pattern1,
	kAP_CODE_pattern2,
	kAP_CODE_pattern3,
	kAP_CODE_pattern4,
	kAP_CODE_pattern5,
	kAP_CODE_pattern6,
	kAP_CODE_pattern7,
	kAP_CODE_pattern8,
	kAP_CODE_pattern9,
	kAP_CODE_pattern10,
	kAP_CODE_pattern11,
	kAP_CODE_pattern12,
	kAP_CODE_pattern13,
	kAP_CODE_pattern14,
	kAP_CODE_pattern15,
	kAP_CODE_pattern16,
	kAP_CODE_pattern17,
	kAP_CODE_pattern18,
	kAP_CODE_pattern19,
	kAP_CODE_pattern20,
	kAP_CODE_pattern21,
	kAP_CODE_pattern22,
	kAP_CODE_pattern23,
	kAP_CODE_pattern24,
	kAP_CODE_pattern25,
	kAP_CODE_pattern26,
	kAP_CODE_pattern27,
	kAP_CODE_pattern28,
	kAP_CODE_pattern29,
	kAP_CODE_pattern30,
	kAP_CODE_pattern31,
	kAP_CODE_pattern32,
	kAP_CODE_pattern33,
	kAP_CODE_pattern34,
	kAP_CODE_pattern35,
	
	kAP_CODE_pattern101, // pattern2 multi
	kAP_CODE_pattern102, // pattern4 guided
	kAP_CODE_pattern103 // pattern3 fast
};

//class MyNumerator
//{
//public:
//	int numerator;
//	int return_code;
//};
//
//class RandomSelector : public CCObject
//{
//public:
//	void setNumerator(int numerator, int return_code)
//	{
//		MyNumerator t_mn;
//		t_mn.numerator = numerator;
//		t_mn.return_code = return_code;
//		numeratorArray.push_back(t_mn);
//	}
//	
//	int getRandomCode()
//	{
//		int random_value = rand()%denominator;
//		
//		int limit=0;
//		
//		for(int i=0;i<numeratorArray.size();i++)
//		{
//			MyNumerator t_mn = numeratorArray[i];
//			limit += t_mn.numerator;
//			if(random_value < limit)
//			{
//				return t_mn.return_code;
//			}
//		}
//		
//		return -1;
//	}
//	
//	int getNotCrashCode()
//	{
//		bool is_searched = false;
//		int t_rc;
//		while(!is_searched)
//		{
//			t_rc = getRandomCode();
//			if(t_rc < kAP_CODE_pattern11 || t_rc > kAP_CODE_pattern19)
//				return t_rc;
//		}
//		
//		return -1;
//	}
//	
//	int getCrashCode()
//	{
//		bool is_searched = false;
//		int t_rc;
//		while(!is_searched)
//		{
//			t_rc = getRandomCode();
//			if(t_rc >= kAP_CODE_pattern11 && t_rc <= kAP_CODE_pattern19)
//				return t_rc;
//		}
//		
//		return -1;
//	}
//	
//	void myInit(int t_denominator)
//	{
//		denominator = t_denominator;
//	}
//	
//private:
//	int denominator;
//	vector<MyNumerator> numeratorArray;
//};

#endif
