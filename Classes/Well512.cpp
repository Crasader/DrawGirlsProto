//
//  Well512.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 5. 30..
//
//

#include "Well512.h"


Well512::Well512()
{
	//시드값 자동 생성
	index = 0;
//	unsigned int s = static_cast<unsigned int>(time(NULL));
	
	
//	for(int i=0; i<100; i++)
//	{
//		printf("%u--", (unsigned int)time(NULL));
//	}
//	printf("--------------------");
//	for(int i=0; i<100; i++)
//	{
//		auto now = std::chrono::system_clock::now();
//		auto duration = now.time_since_epoch();
//		auto rep = duration.count();
//		unsigned int s = rep & 0xFFFFFFFF;
//		printf("%u zzz--", s);
//	}
		
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto rep = duration.count();
	unsigned int s = rep & 0xFFFFFFFF;
	for (int i = 0; i < 16; i++)
	{
		state[i] = s;
		s += s + DIFFER_VALUE;
	}
}