//
//  BalanceData.h
//  DrawingJack
//
//  Created by 사원3 on 12. 12. 18..
//
//

#ifndef DrawingJack_BalanceData_h
#define DrawingJack_BalanceData_h

#include "cocos2d.h"
#include "RandomSelector.h"

using namespace cocos2d;

#define CAUGHT_RATE		5

class BD_P11
{
public:
	int mType;
	float move_speed;
	CCSize crash_area;
	
	BD_P11(float t_move_speed, CCSize t_crash_area)
	{
		move_speed = t_move_speed;
		crash_area = t_crash_area;
	}
	BD_P11()
	{
		mType = 0;
		move_speed = 0;
		crash_area = CCSizeMake(0, 0);
	}
};

class BD_P2 : public BD_P11
{
public:
	int mCnt;
	
	BD_P2(float t_move_speed, int t_mCnt, CCSize t_crash_area) : BD_P11(t_move_speed, t_crash_area){	mCnt = t_mCnt;	}
	BD_P2() : BD_P11()
	{
		mCnt = 0;
	}
};

class BD_P1 : public BD_P2
{
public:
	float spin_radius;
	
	BD_P1(float t_move_speed, int t_mCnt, float t_spin_radius, CCSize t_crash_area) : BD_P2(t_move_speed, t_mCnt, t_crash_area){	spin_radius = t_spin_radius;	}
};

class BD_P3 : public BD_P11
{
public:
	int keep_frame;
	int shoot_frame;
	
	BD_P3(float t_move_speed, int t_keep_frame, int t_shoot_frame, CCSize t_crash_area) : BD_P11(t_move_speed, t_crash_area)
	{
		keep_frame = t_keep_frame;
		shoot_frame = t_shoot_frame;
	}
};

class BD_P4 : public BD_P3
{
public:
	float shoot_angle;
	int shoot_cnt;
	
	BD_P4(float t_move_speed, int t_keep_frame, int t_shoot_frame, CCSize t_crash_area, float t_shoot_angle, int t_shoot_cnt) : BD_P3(t_move_speed, t_keep_frame, t_shoot_frame, t_crash_area)
	{
		shoot_angle = t_shoot_angle;
		shoot_cnt = t_shoot_cnt;
	}
};

class BD_P5 : public BD_P2
{
public:
	BD_P5(float t_move_speed, int t_mCnt, CCSize t_crash_area) : BD_P2(t_move_speed, t_mCnt, t_crash_area){}
};

class BD_P8 : public BD_P2
{
public:
	BD_P8(float t_move_speed, int t_mCnt, CCSize t_crash_area) : BD_P2(t_move_speed, t_mCnt, t_crash_area){}
};

class BD_P9 : public BD_P11
{
public:
	int create_frame;

	BD_P9(float t_move_speed, CCSize t_crash_area, int t_create_frame) : BD_P11(t_move_speed, t_crash_area){	create_frame = t_create_frame;	}
};

class BD_P14 : public BD_P2
{
public:
	BD_P14(float t_move_speed, int t_mCnt, CCSize t_crash_area) : BD_P2(t_move_speed, t_mCnt, t_crash_area){}
	BD_P14() : BD_P2()
	{
		
	}
};

class BD_P18 : public BD_P2
{
public:
	int bomb_cnt;
	BD_P18(float t_move_speed, int t_mCnt, CCSize t_crash_area, int t_bomb_cnt) : BD_P2(t_move_speed, t_mCnt, t_crash_area){	bomb_cnt = t_bomb_cnt;	}
};

class BD_P28
{
public:
	int size_radius;
	int obj_cnt;
	
	BD_P28(int t_size_radius, int t_obj_cnt)
	{
		size_radius = t_size_radius;
		obj_cnt = t_obj_cnt;
	}
};

class BalanceData : public CCObject
{
public:
	static BalanceData* sharedInstance()
	{
		static BalanceData* t_bd = NULL;
		if(t_bd == NULL)
		{
			t_bd = new BalanceData();
			t_bd->myInit();
		}
		
		return t_bd;
	}
	
	int getSubCumberCnt(int world_map, int inside_map)
	{
		int return_value;
		if(world_map <= 20)
		{
			int changed_world_map = (world_map-1)%10+1;
			if(changed_world_map == 1)
			{
				if(world_map == 1)
				{
					if(inside_map == 1)				return_value = 0;
					else if(inside_map == 2)		return_value = 1;
					else if(inside_map == 3)		return_value = 0;
					else if(inside_map == 4)		return_value = 1;
					else if(inside_map == 5)		return_value = 1;
				}
				else
				{
					if(inside_map == 1)				return_value = 1;
					else if(inside_map == 2)		return_value = 1;
					else if(inside_map == 3)		return_value = 2;
					else if(inside_map == 4)		return_value = 1;
					else if(inside_map == 5)		return_value = 2;
				}
			}
			else if(changed_world_map == 2)
			{
				if(inside_map == 1)				return_value = 2;
				else if(inside_map == 2)		return_value = 3;
				else if(inside_map == 3)		return_value = 2;
				else if(inside_map == 4)		return_value = 3;
				else if(inside_map == 5)		return_value = 4;
			}
			else if(changed_world_map == 3)
			{
				if(inside_map == 1)				return_value = 1;
				else if(inside_map == 2)		return_value = 2;
				else if(inside_map == 3)		return_value = 2;
				else if(inside_map == 4)		return_value = 3;
				else if(inside_map == 5)		return_value = 2;
			}
			else if(changed_world_map == 4)
			{
				if(inside_map == 1)				return_value = 3;
				else if(inside_map == 2)		return_value = 3;
				else if(inside_map == 3)		return_value = 3;
				else if(inside_map == 4)		return_value = 4;
				else if(inside_map == 5)		return_value = 4;
			}
			else if(changed_world_map == 5)
			{
				if(inside_map == 1)				return_value = 2;
				else if(inside_map == 2)		return_value = 3;
				else if(inside_map == 3)		return_value = 3;
				else if(inside_map == 4)		return_value = 4;
				else if(inside_map == 5)		return_value = 3;
			}
			else if(changed_world_map == 6)
			{
				if(inside_map == 1)				return_value = 3;
				else if(inside_map == 2)		return_value = 2;
				else if(inside_map == 3)		return_value = 3;
				else if(inside_map == 4)		return_value = 4;
				else if(inside_map == 5)		return_value = 4;
			}
			else if(changed_world_map == 7)
			{
				if(inside_map == 1)				return_value = 3;
				else if(inside_map == 2)		return_value = 3;
				else if(inside_map == 3)		return_value = 4;
				else if(inside_map == 4)		return_value = 4;
				else if(inside_map == 5)		return_value = 3;
			}
			else if(changed_world_map == 8)
			{
				if(inside_map == 1)				return_value = 3;
				else if(inside_map == 2)		return_value = 2;
				else if(inside_map == 3)		return_value = 4;
				else if(inside_map == 4)		return_value = 4;
				else if(inside_map == 5)		return_value = 5;
			}
			else if(changed_world_map == 9)
			{
				if(inside_map == 1)				return_value = 3;
				else if(inside_map == 2)		return_value = 4;
				else if(inside_map == 3)		return_value = 3;
				else if(inside_map == 4)		return_value = 5;
				else if(inside_map == 5)		return_value = 3;
			}
			else if(changed_world_map == 10)
			{
				if(inside_map == 1)				return_value = 3;
				else if(inside_map == 2)		return_value = 4;
				else if(inside_map == 3)		return_value = 3;
				else if(inside_map == 4)		return_value = 5;
				else if(inside_map == 5)		return_value = 6;
			}
		}
		else
		{
			int changed_world_map = world_map%10;
			if(changed_world_map == 1)
			{
				if(inside_map == 1)				return_value = 3;
				else if(inside_map == 2)		return_value = 4;
				else if(inside_map == 3)		return_value = 3;
				else if(inside_map == 4)		return_value = 4;
				else if(inside_map == 5)		return_value = 5;
			}
			else if(changed_world_map == 2)
			{
				if(inside_map == 1)				return_value = 2;
				else if(inside_map == 2)		return_value = 4;
				else if(inside_map == 3)		return_value = 4;
				else if(inside_map == 4)		return_value = 3;
				else if(inside_map == 5)		return_value = 5;
			}
			else if(changed_world_map == 3) // auto pattern : create subCumber
			{
				if(inside_map == 1)				return_value = 2;
				else if(inside_map == 2)		return_value = 2;
				else if(inside_map == 3)		return_value = 1;
				else if(inside_map == 4)		return_value = 3;
				else if(inside_map == 5)		return_value = 2;
			}
			else if(changed_world_map == 4)
			{
				if(inside_map == 1)				return_value = 3;
				else if(inside_map == 2)		return_value = 3;
				else if(inside_map == 3)		return_value = 4;
				else if(inside_map == 4)		return_value = 4;
				else if(inside_map == 5)		return_value = 5;
			}
			else if(changed_world_map == 5)
			{
				if(inside_map == 1)				return_value = 4;
				else if(inside_map == 2)		return_value = 2;
				else if(inside_map == 3)		return_value = 4;
				else if(inside_map == 4)		return_value = 3;
				else if(inside_map == 5)		return_value = 5;
			}
			else if(changed_world_map == 6)
			{
				if(inside_map == 1)				return_value = 3;
				else if(inside_map == 2)		return_value = 4;
				else if(inside_map == 3)		return_value = 4;
				else if(inside_map == 4)		return_value = 3;
				else if(inside_map == 5)		return_value = 5;
			}
		}
		
		return return_value;
	}
	
	int getSecondSubCumberType(int worldMap, int insideMap)
	{
		if(worldMap <= 20)
		{
			int ten_value = worldMap > 10 ? 10 : 0;
			if(worldMap == 1 || worldMap == 11)
			{
				if(insideMap == 1)				return 1+ten_value;
				else if(insideMap == 2)			return 1+ten_value;
				else if(insideMap == 3)			return 3+ten_value;
				else if(insideMap == 4)			return 1+ten_value;
				else if(insideMap == 5)			return 5+ten_value;
			}
			else if(worldMap == 2 || worldMap == 12)
			{
				if(insideMap == 1)				return 4+ten_value;
				else if(insideMap == 2)			return 8+ten_value;
				else if(insideMap == 3)			return 6+ten_value;
				else if(insideMap == 4)			return 8+ten_value;
				else if(insideMap == 5)			return 10+ten_value;
			}
			else if(worldMap == 3 || worldMap == 13)
			{
				if(insideMap == 1)				return 3+ten_value;
				else if(insideMap == 2)			return 1+ten_value;
				else if(insideMap == 3)			return 5+ten_value;
				else if(insideMap == 4)			return 1+ten_value;
				else if(insideMap == 5)			return 7+ten_value;
			}
			else if(worldMap == 4 || worldMap == 14)
			{
				if(insideMap == 1)				return 6+ten_value;
				else if(insideMap == 2)			return 8+ten_value;
				else if(insideMap == 3)			return 10+ten_value;
				else if(insideMap == 4)			return 6+ten_value;
				else if(insideMap == 5)			return 10+ten_value;
			}
			else if(worldMap == 5 || worldMap == 15)
			{
				if(insideMap == 1)				return 1+ten_value;
				else if(insideMap == 2)			return 3+ten_value;
				else if(insideMap == 3)			return 7+ten_value;
				else if(insideMap == 4)			return 1+ten_value;
				else if(insideMap == 5)			return 9+ten_value;
			}
			else if(worldMap == 6 || worldMap == 16)
			{
				if(insideMap == 1)				return 4+ten_value;
				else if(insideMap == 2)			return 10+ten_value;
				else if(insideMap == 3)			return 8+ten_value;
				else if(insideMap == 4)			return 2+ten_value;
				else if(insideMap == 5)			return 8+ten_value;
			}
			else if(worldMap == 7 || worldMap == 17)
			{
				if(insideMap == 1)				return 1+ten_value;
				else if(insideMap == 2)			return 9+ten_value;
				else if(insideMap == 3)			return 5+ten_value;
				else if(insideMap == 4)			return 3+ten_value;
				else if(insideMap == 5)			return 5+ten_value;
			}
			else if(worldMap == 8 || worldMap == 18)
			{
				if(insideMap == 1)				return 10+ten_value;
				else if(insideMap == 2)			return 2+ten_value;
				else if(insideMap == 3)			return 4+ten_value;
				else if(insideMap == 4)			return 4+ten_value;
				else if(insideMap == 5)			return 10+ten_value;
			}
			else if(worldMap == 9 || worldMap == 19)
			{
				if(insideMap == 1)				return 1+ten_value;
				else if(insideMap == 2)			return 3+ten_value;
				else if(insideMap == 3)			return 5+ten_value;
				else if(insideMap == 4)			return 7+ten_value;
				else if(insideMap == 5)			return 1+ten_value;
			}
			else if(worldMap == 10 || worldMap == 20)
			{
				if(insideMap == 1)				return 4+ten_value;
				else if(insideMap == 2)			return 6+ten_value;
				else if(insideMap == 3)			return 8+ten_value;
				else if(insideMap == 4)			return 2+ten_value;
				else if(insideMap == 5)			return 4+ten_value;
			}
		}
		else
		{
			int base_value;
			if(worldMap < 30)			base_value = 20;
			else if(worldMap < 40)		base_value = 30;
			else if(worldMap < 50)		base_value = 40;
			
			// 1 : empty, 2 : water, 3 : life, 4 : life, 5 : fire, 6 : water
			
			int view_chapter_number = worldMap%10 + 10;
			if(view_chapter_number == 11) // empty
			{
				if(insideMap == 1)			return base_value + 2;
				else if(insideMap == 2)		return base_value + 3;
				else if(insideMap == 3)		return base_value + 5;
				else if(insideMap == 4)		return base_value + 4;
				else if(insideMap == 5)		return base_value + 6;
			}
			else if(view_chapter_number == 12)
			{
				if(insideMap == 1)			return base_value + 3;
				else if(insideMap == 2)		return base_value + 5;
				else if(insideMap == 3)		return base_value + 4;
				else if(insideMap == 4)		return base_value + 1;
				else if(insideMap == 5)		return base_value + 6;
			}
			else if(view_chapter_number == 13)
			{
				if(insideMap == 1)			return base_value + 2;
				else if(insideMap == 2)		return base_value + 5;
				else if(insideMap == 3)		return base_value + 1;
				else if(insideMap == 4)		return base_value + 6;
				else if(insideMap == 5)		return base_value + 4;
			}
			else if(view_chapter_number == 14)
			{
				if(insideMap == 1)			return base_value + 6;
				else if(insideMap == 2)		return base_value + 1;
				else if(insideMap == 3)		return base_value + 5;
				else if(insideMap == 4)		return base_value + 2;
				else if(insideMap == 5)		return base_value + 3;
			}
			else if(view_chapter_number == 15)
			{
				if(insideMap == 1)			return base_value + 6;
				else if(insideMap == 2)		return base_value + 2;
				else if(insideMap == 3)		return base_value + 4;
				else if(insideMap == 4)		return base_value + 3;
				else if(insideMap == 5)		return base_value + 1;
			}
			else if(view_chapter_number == 16)
			{
				if(insideMap == 1)			return base_value + 1;
				else if(insideMap == 2)		return base_value + 4;
				else if(insideMap == 3)		return base_value + 2;
				else if(insideMap == 4)		return base_value + 5;
				else if(insideMap == 5)		return base_value + 3;
			}
		}
	}
	
	float getBossSpeed(int world_map, int inside_map)
	{
		float return_value;
		if(world_map <= 20)
		{
			int changed_world_map = (world_map-1)%10+1;
			if(changed_world_map <= 2)			return_value = 1.3f;
			else if(changed_world_map <= 4)		return_value = 1.4f; // 1.8f
			else if(changed_world_map <= 6)		return_value = 1.5f; // 2.f
			else if(changed_world_map <= 8)		return_value = 1.6f; // 2.2f
			else if(changed_world_map <= 10)	return_value = 1.7f; // 2.5f
		}
		else
		{
			int changed_world_map = world_map%10;
			if(changed_world_map == 1)			return_value = 1.4f; //1.7f;
			else if(changed_world_map == 2)		return_value = 1.5f; //1.7f;
			else if(changed_world_map == 3)		return_value = 1.6f; //1.8f;
			else if(changed_world_map == 4)		return_value = 1.7f; //1.8f;
			else if(changed_world_map == 5)		return_value = 1.7f; //1.9f;
			else if(changed_world_map == 6)		return_value = 1.8f; //1.9f;
		}
		return return_value;
	}
	
	float getSubCumberSpeed(int world_map, int inside_map)
	{
		float return_value;
		if(world_map <= 20)
		{
			int changed_world_map = (world_map-1)%10+1;
			if(changed_world_map <= 2)			return_value = 0.7f; // 0.8f
			else if(changed_world_map <= 4)		return_value = 0.8f; // 1.f
			else if(changed_world_map <= 6)		return_value = 0.9f; // 1.3f
			else if(changed_world_map <= 8)		return_value = 1.f; // 1.5f
			else if(changed_world_map <= 10)	return_value = 1.1f; // 1.8f
		}
		else
		{
			int changed_world_map = world_map%10;
			if(changed_world_map == 1)			return_value = 0.8f; //1.1f;
			else if(changed_world_map == 2)		return_value = 0.9f; //1.2f;
			else if(changed_world_map == 3)		return_value = 1.f; //1.2f;
			else if(changed_world_map == 4)		return_value = 1.1f; //1.3f;
			else if(changed_world_map == 5)		return_value = 1.1f; //1.3f;
			else if(changed_world_map == 6)		return_value = 1.2f; //1.4f;
		}
		return return_value;
	}
	
	int getAttackRate(int world_map, int inside_map)
	{
		double start_rate, final_rate;
		if(world_map <= 20)
		{
			int changed_world_map = (world_map-1)%10+1;
			if(changed_world_map <= 2)
			{
				start_rate = 1500;
				final_rate = 1100;
			}
			else if(changed_world_map <= 4)
			{
				start_rate = 1400;
				final_rate = 1050;
			}
			else if(changed_world_map <= 6)
			{
				start_rate = 1300;
				final_rate = 1000;
			}
			else if(changed_world_map <= 8)
			{
				start_rate = 1200;
				final_rate = 900;
			}
			else if(changed_world_map <= 10)
			{
				start_rate = 1000;
				final_rate = 800;
			}
		}
		else
		{
			int changed_world_map = world_map%10;
			if(changed_world_map == 1)
			{
				start_rate = 1000;
				final_rate = 700;
			}
			else if(changed_world_map == 2)
			{
				start_rate = 900;
				final_rate = 650;
			}
			else if(changed_world_map == 3)
			{
				start_rate = 850;
				final_rate = 600;
			}
			else if(changed_world_map >= 4)
			{
				start_rate = 800;
				final_rate = 600;
			}
		}
		
		int return_value = round(start_rate - (start_rate - final_rate)/4*(inside_map-1));
		return return_value;
	}
	
	void setAttackPattern(RandomSelector* myRS, int worldmap, int insidemap)
	{
		myRS->setNumerator(20, kAP_CODE_pattern101);
		myRS->setNumerator(15, kAP_CODE_pattern102);
		myRS->setNumerator(20, kAP_CODE_pattern103);
		myRS->setNumerator(10, kAP_CODE_pattern10);
		myRS->setNumerator(5, kAP_CODE_pattern13);
		myRS->setNumerator(10, kAP_CODE_pattern17);
		myRS->setNumerator(10, kAP_CODE_pattern23);
		myRS->setNumerator(10, kAP_CODE_pattern32);
		
//		if(worldmap <= 20)
//		{
//			int change_worldmap = (worldmap-1)%10+1;
//			if(change_worldmap == 1)
//			{
//				if(worldmap > 10)
//				{
//					if(insidemap == 1)
//					{
//						myRS->setNumerator(30, kAP_CODE_pattern1);
//						myRS->setNumerator(50, kAP_CODE_pattern2);
//						myRS->setNumerator(30, kAP_CODE_pattern11);
//						myRS->setNumerator(10, kAP_CODE_pattern30);
//					}
//					else if(insidemap == 2)
//					{
//						myRS->setNumerator(50, kAP_CODE_pattern1);
//						myRS->setNumerator(30, kAP_CODE_pattern3);
//						myRS->setNumerator(30, kAP_CODE_pattern11);
//						myRS->setNumerator(10, kAP_CODE_pattern30);
//					}
//					else if(insidemap == 3)
//					{
//						myRS->setNumerator(20, kAP_CODE_pattern1);
//						myRS->setNumerator(50, kAP_CODE_pattern2);
//						myRS->setNumerator(10, kAP_CODE_pattern3);
//						myRS->setNumerator(30, kAP_CODE_pattern11);
//						myRS->setNumerator(10, kAP_CODE_pattern30);
//					}
//					else if(insidemap == 4)
//					{
//						myRS->setNumerator(20, kAP_CODE_pattern1);
//						myRS->setNumerator(40, kAP_CODE_pattern2);
//						myRS->setNumerator(30, kAP_CODE_pattern11);
//						myRS->setNumerator(20, kAP_CODE_pattern25);
//						myRS->setNumerator(10, kAP_CODE_pattern30);
//					}
//					else if(insidemap == 5)
//					{
//						myRS->setNumerator(40, kAP_CODE_pattern1);
//						myRS->setNumerator(30, kAP_CODE_pattern3);
//						myRS->setNumerator(40, kAP_CODE_pattern11);
//						myRS->setNumerator(10, kAP_CODE_pattern30);
//					}
//					
//					myRS->setNumerator(20, kAP_CODE_pattern25);
//				}
//			}
//			else if(change_worldmap == 2)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern1);
//					myRS->setNumerator(50, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern3);
//					myRS->setNumerator(30, kAP_CODE_pattern11);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern1);
//					myRS->setNumerator(40, kAP_CODE_pattern2);
//					myRS->setNumerator(30, kAP_CODE_pattern11);
//					myRS->setNumerator(20, kAP_CODE_pattern25);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(40, kAP_CODE_pattern1);
//					myRS->setNumerator(30, kAP_CODE_pattern3);
//					myRS->setNumerator(40, kAP_CODE_pattern11);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(30, kAP_CODE_pattern1);
//					myRS->setNumerator(40, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern3);
//					myRS->setNumerator(30, kAP_CODE_pattern11);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern1);
//					myRS->setNumerator(40, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern3);
//					myRS->setNumerator(30, kAP_CODE_pattern11);
//					myRS->setNumerator(10, kAP_CODE_pattern25);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				if(worldmap > 10)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern25);
//				}
//			}
//			else if(change_worldmap == 3)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(40, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern4);
//					myRS->setNumerator(30, kAP_CODE_pattern11);
//					myRS->setNumerator(20, kAP_CODE_pattern22);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(30, kAP_CODE_pattern2);
//					myRS->setNumerator(30, kAP_CODE_pattern4);
//					myRS->setNumerator(30, kAP_CODE_pattern14);
//					myRS->setNumerator(20, kAP_CODE_pattern22);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(30, kAP_CODE_pattern2);
//					myRS->setNumerator(30, kAP_CODE_pattern4);
//					myRS->setNumerator(30, kAP_CODE_pattern14);
//					myRS->setNumerator(20, kAP_CODE_pattern27);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(30, kAP_CODE_pattern4);
//					myRS->setNumerator(30, kAP_CODE_pattern5);
//					myRS->setNumerator(30, kAP_CODE_pattern14);
//					myRS->setNumerator(20, kAP_CODE_pattern22);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(30, kAP_CODE_pattern4);
//					myRS->setNumerator(30, kAP_CODE_pattern5);
//					myRS->setNumerator(20, kAP_CODE_pattern11);
//					myRS->setNumerator(20, kAP_CODE_pattern14);
//					myRS->setNumerator(10, kAP_CODE_pattern22);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				if(worldmap > 10)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern27);
//				}
//			}
//			else if(change_worldmap == 4)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern4);
//					myRS->setNumerator(30, kAP_CODE_pattern5);
//					myRS->setNumerator(30, kAP_CODE_pattern14);
//					myRS->setNumerator(10, kAP_CODE_pattern22);
//					myRS->setNumerator(20, kAP_CODE_pattern27);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern1);
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern4);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(30, kAP_CODE_pattern14);
//					myRS->setNumerator(10, kAP_CODE_pattern22);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern3);
//					myRS->setNumerator(30, kAP_CODE_pattern4);
//					myRS->setNumerator(30, kAP_CODE_pattern5);
//					myRS->setNumerator(30, kAP_CODE_pattern14);
//					myRS->setNumerator(10, kAP_CODE_pattern22);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern1);
//					myRS->setNumerator(10, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern4);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(30, kAP_CODE_pattern14);
//					myRS->setNumerator(10, kAP_CODE_pattern22);
//					myRS->setNumerator(10, kAP_CODE_pattern27);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern4);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(20, kAP_CODE_pattern11);
//					myRS->setNumerator(20, kAP_CODE_pattern14);
//					myRS->setNumerator(10, kAP_CODE_pattern22);
//					myRS->setNumerator(10, kAP_CODE_pattern27);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				if(worldmap > 10)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern27);
//				}
//			}
//			else if(change_worldmap == 5)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern4);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(30, kAP_CODE_pattern8);
//					myRS->setNumerator(40, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern21);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(30, kAP_CODE_pattern8);
//					myRS->setNumerator(40, kAP_CODE_pattern18);
//					myRS->setNumerator(20, kAP_CODE_pattern21);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern5);
//					myRS->setNumerator(30, kAP_CODE_pattern8);
//					myRS->setNumerator(40, kAP_CODE_pattern18);
//					myRS->setNumerator(20, kAP_CODE_pattern21);
//					myRS->setNumerator(10, kAP_CODE_pattern28);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern3);
//					myRS->setNumerator(10, kAP_CODE_pattern5);
//					myRS->setNumerator(20, kAP_CODE_pattern8);
//					myRS->setNumerator(20, kAP_CODE_pattern11);
//					myRS->setNumerator(20, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern21);
//					myRS->setNumerator(20, kAP_CODE_pattern28);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(30, kAP_CODE_pattern8);
//					myRS->setNumerator(40, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern21);
//					myRS->setNumerator(10, kAP_CODE_pattern28);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				if(worldmap > 10)
//				{
//					int rand_value = rand()%2;
//					if(rand_value == 0)		myRS->setNumerator(20, kAP_CODE_pattern25);
//					else					myRS->setNumerator(20, kAP_CODE_pattern27);
//				}
//			}
//			else if(change_worldmap == 6)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(30, kAP_CODE_pattern8);
//					myRS->setNumerator(40, kAP_CODE_pattern18);
//					myRS->setNumerator(20, kAP_CODE_pattern28);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern1);
//					myRS->setNumerator(30, kAP_CODE_pattern8);
//					myRS->setNumerator(40, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern21);
//					myRS->setNumerator(10, kAP_CODE_pattern28);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern2);
//					myRS->setNumerator(30, kAP_CODE_pattern8);
//					myRS->setNumerator(40, kAP_CODE_pattern18);
//					myRS->setNumerator(20, kAP_CODE_pattern21);
//					myRS->setNumerator(10, kAP_CODE_pattern28);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern2);
//					myRS->setNumerator(30, kAP_CODE_pattern8);
//					myRS->setNumerator(40, kAP_CODE_pattern18);
//					myRS->setNumerator(20, kAP_CODE_pattern21);
//					myRS->setNumerator(10, kAP_CODE_pattern28);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern2);
//					myRS->setNumerator(30, kAP_CODE_pattern8);
//					myRS->setNumerator(40, kAP_CODE_pattern18);
//					myRS->setNumerator(20, kAP_CODE_pattern21);
//					myRS->setNumerator(10, kAP_CODE_pattern28);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				if(worldmap > 10)
//				{
//					int rand_value = rand()%2;
//					if(rand_value == 0)		myRS->setNumerator(20, kAP_CODE_pattern25);
//					else					myRS->setNumerator(20, kAP_CODE_pattern27);
//				}
//			}
//			else if(change_worldmap == 7)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(30, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern3);
//					myRS->setNumerator(10, kAP_CODE_pattern9);
//					myRS->setNumerator(40, kAP_CODE_pattern13);
//					myRS->setNumerator(10, kAP_CODE_pattern28);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern31);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(30, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern4);
//					myRS->setNumerator(10, kAP_CODE_pattern9);
//					myRS->setNumerator(40, kAP_CODE_pattern13);
//					myRS->setNumerator(10, kAP_CODE_pattern26);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern31);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern5);
//					myRS->setNumerator(10, kAP_CODE_pattern9);
//					myRS->setNumerator(30, kAP_CODE_pattern13);
//					myRS->setNumerator(20, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern26);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern31);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(10, kAP_CODE_pattern9);
//					myRS->setNumerator(30, kAP_CODE_pattern13);
//					myRS->setNumerator(20, kAP_CODE_pattern18);
//					myRS->setNumerator(20, kAP_CODE_pattern26);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern31);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern1);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(10, kAP_CODE_pattern9);
//					myRS->setNumerator(40, kAP_CODE_pattern13);
//					myRS->setNumerator(20, kAP_CODE_pattern26);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern31);
//				}
//				if(worldmap > 10)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern26);
//				}
//			}
//			else if(change_worldmap == 8)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(30, kAP_CODE_pattern5);
//					myRS->setNumerator(10, kAP_CODE_pattern9);
//					myRS->setNumerator(20, kAP_CODE_pattern13);
//					myRS->setNumerator(20, kAP_CODE_pattern14);
//					myRS->setNumerator(10, kAP_CODE_pattern26);
//					myRS->setNumerator(10, kAP_CODE_pattern29);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern31);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(10, kAP_CODE_pattern9);
//					myRS->setNumerator(40, kAP_CODE_pattern13);
//					myRS->setNumerator(10, kAP_CODE_pattern26);
//					myRS->setNumerator(10, kAP_CODE_pattern28);
//					myRS->setNumerator(20, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern31);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(30, kAP_CODE_pattern5);
//					myRS->setNumerator(10, kAP_CODE_pattern9);
//					myRS->setNumerator(20, kAP_CODE_pattern11);
//					myRS->setNumerator(20, kAP_CODE_pattern13);
//					myRS->setNumerator(10, kAP_CODE_pattern26);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern31);
//					myRS->setNumerator(10, kAP_CODE_pattern34);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern1);
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern5);
//					myRS->setNumerator(10, kAP_CODE_pattern9);
//					myRS->setNumerator(40, kAP_CODE_pattern13);
//					myRS->setNumerator(10, kAP_CODE_pattern26);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern31);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(10, kAP_CODE_pattern9);
//					myRS->setNumerator(40, kAP_CODE_pattern13);
//					myRS->setNumerator(10, kAP_CODE_pattern26);
//					myRS->setNumerator(10, kAP_CODE_pattern29);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern31);
//					myRS->setNumerator(10, kAP_CODE_pattern34);
//				}
//				if(worldmap > 10)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern26);
//				}
//			}
//			else if(change_worldmap == 9)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern6);
//					myRS->setNumerator(20, kAP_CODE_pattern9);
//					myRS->setNumerator(40, kAP_CODE_pattern13);
//					myRS->setNumerator(20, kAP_CODE_pattern24);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern6);
//					myRS->setNumerator(10, kAP_CODE_pattern10);
//					myRS->setNumerator(40, kAP_CODE_pattern15);
//					myRS->setNumerator(20, kAP_CODE_pattern24);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern32);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(30, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern6);
//					myRS->setNumerator(30, kAP_CODE_pattern15);
//					myRS->setNumerator(20, kAP_CODE_pattern17);
//					myRS->setNumerator(20, kAP_CODE_pattern24);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern4);
//					myRS->setNumerator(10, kAP_CODE_pattern6);
//					myRS->setNumerator(30, kAP_CODE_pattern15);
//					myRS->setNumerator(20, kAP_CODE_pattern17);
//					myRS->setNumerator(10, kAP_CODE_pattern24);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(20, kAP_CODE_pattern32);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern6);
//					myRS->setNumerator(10, kAP_CODE_pattern10);
//					myRS->setNumerator(20, kAP_CODE_pattern15);
//					myRS->setNumerator(20, kAP_CODE_pattern17);
//					myRS->setNumerator(20, kAP_CODE_pattern24);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(20, kAP_CODE_pattern32);
//				}
//				if(worldmap > 10)
//				{
//					int rand_value = rand()%3;
//					if(rand_value == 0)			myRS->setNumerator(20, kAP_CODE_pattern25);
//					else if(rand_value == 1)	myRS->setNumerator(20, kAP_CODE_pattern26);
//					else						myRS->setNumerator(20, kAP_CODE_pattern27);
//				}
//			}
//			else if(change_worldmap == 10)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern6);
//					myRS->setNumerator(30, kAP_CODE_pattern15);
//					myRS->setNumerator(20, kAP_CODE_pattern17);
//					myRS->setNumerator(10, kAP_CODE_pattern24);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(20, kAP_CODE_pattern32);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern3);
//					myRS->setNumerator(10, kAP_CODE_pattern6);
//					myRS->setNumerator(30, kAP_CODE_pattern15);
//					myRS->setNumerator(20, kAP_CODE_pattern17);
//					myRS->setNumerator(20, kAP_CODE_pattern24);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern32);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(10, kAP_CODE_pattern6);
//					myRS->setNumerator(10, kAP_CODE_pattern10);
//					myRS->setNumerator(40, kAP_CODE_pattern15);
//					myRS->setNumerator(10, kAP_CODE_pattern24);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(20, kAP_CODE_pattern32);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern6);
//					myRS->setNumerator(20, kAP_CODE_pattern13);
//					myRS->setNumerator(30, kAP_CODE_pattern17);
//					myRS->setNumerator(20, kAP_CODE_pattern24);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern32);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(10, kAP_CODE_pattern6);
//					myRS->setNumerator(10, kAP_CODE_pattern10);
//					myRS->setNumerator(30, kAP_CODE_pattern15);
//					myRS->setNumerator(20, kAP_CODE_pattern17);
//					myRS->setNumerator(10, kAP_CODE_pattern24);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern32);
//				}
//				if(worldmap > 10)
//				{
//					int rand_value = rand()%3;
//					if(rand_value == 0)			myRS->setNumerator(20, kAP_CODE_pattern25);
//					else if(rand_value == 1)	myRS->setNumerator(20, kAP_CODE_pattern26);
//					else						myRS->setNumerator(20, kAP_CODE_pattern27);
//				}
//			}
//		}
//		else
//		{
//			int changed_world_map = worldmap%10;
//			if(changed_world_map == 1)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern1);
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern4);
//					myRS->setNumerator(10, kAP_CODE_pattern7);
//					myRS->setNumerator(30, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern34);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(30, kAP_CODE_pattern3);
//					myRS->setNumerator(10, kAP_CODE_pattern5);
//					myRS->setNumerator(20, kAP_CODE_pattern7);
//					myRS->setNumerator(20, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern34);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern4);
//					myRS->setNumerator(10, kAP_CODE_pattern5);
//					myRS->setNumerator(10, kAP_CODE_pattern7);
//					myRS->setNumerator(20, kAP_CODE_pattern8);
//					myRS->setNumerator(30, kAP_CODE_pattern17);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(20, kAP_CODE_pattern34);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern1);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(20, kAP_CODE_pattern6);
//					myRS->setNumerator(20, kAP_CODE_pattern7);
//					myRS->setNumerator(20, kAP_CODE_pattern13);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern34);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern3);
//					myRS->setNumerator(20, kAP_CODE_pattern7);
//					myRS->setNumerator(10, kAP_CODE_pattern9);
//					myRS->setNumerator(20, kAP_CODE_pattern15);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(20, kAP_CODE_pattern34);
//				}
//				
//				if(worldmap/10 - 2 > 0)
//				{
//					int rand_value = rand()%3;
//					if(rand_value == 0)			myRS->setNumerator(20, kAP_CODE_pattern25);
//					else if(rand_value == 1)	myRS->setNumerator(20, kAP_CODE_pattern26);
//					else						myRS->setNumerator(20, kAP_CODE_pattern27);
//				}
//			}
//			else if(changed_world_map == 2)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern3);
//					myRS->setNumerator(20, kAP_CODE_pattern10);
//					myRS->setNumerator(20, kAP_CODE_pattern11);
//					myRS->setNumerator(10, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern29);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern33);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern4);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(20, kAP_CODE_pattern8);
//					myRS->setNumerator(20, kAP_CODE_pattern14);
//					myRS->setNumerator(10, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(20, kAP_CODE_pattern33);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern1);
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern6);
//					myRS->setNumerator(10, kAP_CODE_pattern11);
//					myRS->setNumerator(20, kAP_CODE_pattern15);
//					myRS->setNumerator(10, kAP_CODE_pattern29);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern33);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern3);
//					myRS->setNumerator(30, kAP_CODE_pattern5);
//					myRS->setNumerator(20, kAP_CODE_pattern7);
//					myRS->setNumerator(10, kAP_CODE_pattern11);
//					myRS->setNumerator(10, kAP_CODE_pattern15);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(20, kAP_CODE_pattern33);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern8);
//					myRS->setNumerator(20, kAP_CODE_pattern9);
//					myRS->setNumerator(20, kAP_CODE_pattern14);
//					myRS->setNumerator(20, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(20, kAP_CODE_pattern33);
//				}
//				
//				if(worldmap/10 - 2 > 0)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern26);
//				}
//			}
//			else if(changed_world_map == 3)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern3);
//					myRS->setNumerator(30, kAP_CODE_pattern14);
//					myRS->setNumerator(10, kAP_CODE_pattern20);
//					myRS->setNumerator(20, kAP_CODE_pattern28);
//					myRS->setNumerator(20, kAP_CODE_pattern34);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern4);
//					myRS->setNumerator(20, kAP_CODE_pattern8);
//					myRS->setNumerator(20, kAP_CODE_pattern11);
//					myRS->setNumerator(10, kAP_CODE_pattern14);
//					myRS->setNumerator(20, kAP_CODE_pattern20);
//					myRS->setNumerator(10, kAP_CODE_pattern24);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern1);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(20, kAP_CODE_pattern9);
//					myRS->setNumerator(20, kAP_CODE_pattern17);
//					myRS->setNumerator(10, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern20);
//					myRS->setNumerator(10, kAP_CODE_pattern31);
//					myRS->setNumerator(10, kAP_CODE_pattern33);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern3);
//					myRS->setNumerator(20, kAP_CODE_pattern6);
//					myRS->setNumerator(10, kAP_CODE_pattern13);
//					myRS->setNumerator(20, kAP_CODE_pattern15);
//					myRS->setNumerator(20, kAP_CODE_pattern20);
//					myRS->setNumerator(10, kAP_CODE_pattern24);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern3);
//					myRS->setNumerator(20, kAP_CODE_pattern4);
//					myRS->setNumerator(10, kAP_CODE_pattern7);
//					myRS->setNumerator(20, kAP_CODE_pattern8);
//					myRS->setNumerator(10, kAP_CODE_pattern11);
//					myRS->setNumerator(10, kAP_CODE_pattern17);
//					myRS->setNumerator(20, kAP_CODE_pattern20);
//					myRS->setNumerator(10, kAP_CODE_pattern28);
//					myRS->setNumerator(10, kAP_CODE_pattern31);
//				}
//				
//				if(worldmap/10 - 2 > 0)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern25);
//				}
//			}
//			else if(changed_world_map == 4)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern1);
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(10, kAP_CODE_pattern10);
//					myRS->setNumerator(10, kAP_CODE_pattern12);
//					myRS->setNumerator(10, kAP_CODE_pattern17);
//					myRS->setNumerator(10, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern32);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern8);
//					myRS->setNumerator(20, kAP_CODE_pattern10);
//					myRS->setNumerator(20, kAP_CODE_pattern12);
//					myRS->setNumerator(10, kAP_CODE_pattern15);
//					myRS->setNumerator(10, kAP_CODE_pattern27);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern33);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern1);
//					myRS->setNumerator(10, kAP_CODE_pattern3);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(10, kAP_CODE_pattern10);
//					myRS->setNumerator(10, kAP_CODE_pattern12);
//					myRS->setNumerator(10, kAP_CODE_pattern17);
//					myRS->setNumerator(10, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern27);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern32);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern1);
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern10);
//					myRS->setNumerator(20, kAP_CODE_pattern12);
//					myRS->setNumerator(10, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern27);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern33);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern3);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(20, kAP_CODE_pattern8);
//					myRS->setNumerator(10, kAP_CODE_pattern10);
//					myRS->setNumerator(20, kAP_CODE_pattern12);
//					myRS->setNumerator(10, kAP_CODE_pattern15);
//					myRS->setNumerator(10, kAP_CODE_pattern27);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern33);
//				}
//				
//				if(worldmap/10 - 2 > 0)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern27);
//				}
//			}
//			else if(changed_world_map == 5)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(20, kAP_CODE_pattern8);
//					myRS->setNumerator(20, kAP_CODE_pattern13);
//					myRS->setNumerator(10, kAP_CODE_pattern16);
//					myRS->setNumerator(10, kAP_CODE_pattern28);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern34);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern3);
//					myRS->setNumerator(20, kAP_CODE_pattern9);
//					myRS->setNumerator(10, kAP_CODE_pattern14);
//					myRS->setNumerator(20, kAP_CODE_pattern16);
//					myRS->setNumerator(10, kAP_CODE_pattern24);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern32);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(20, kAP_CODE_pattern8);
//					myRS->setNumerator(20, kAP_CODE_pattern15);
//					myRS->setNumerator(10, kAP_CODE_pattern16);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern33);
//					myRS->setNumerator(10, kAP_CODE_pattern34);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(10, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern3);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(20, kAP_CODE_pattern8);
//					myRS->setNumerator(10, kAP_CODE_pattern11);
//					myRS->setNumerator(20, kAP_CODE_pattern16);
//					myRS->setNumerator(10, kAP_CODE_pattern24);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern32);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern3);
//					myRS->setNumerator(20, kAP_CODE_pattern9);
//					myRS->setNumerator(10, kAP_CODE_pattern13);
//					myRS->setNumerator(20, kAP_CODE_pattern16);
//					myRS->setNumerator(10, kAP_CODE_pattern24);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern33);
//				}
//				
//				if(worldmap/10 - 2 > 0)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern25);
//				}
//			}
//			else if(changed_world_map == 6)
//			{
//				if(insidemap == 1)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern1);
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(20, kAP_CODE_pattern4);
//					myRS->setNumerator(10, kAP_CODE_pattern10);
//					myRS->setNumerator(20, kAP_CODE_pattern16);
//					myRS->setNumerator(10, kAP_CODE_pattern23);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern32);
//				}
//				else if(insidemap == 2)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern3);
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(20, kAP_CODE_pattern6);
//					myRS->setNumerator(20, kAP_CODE_pattern16);
//					myRS->setNumerator(10, kAP_CODE_pattern20);
//					myRS->setNumerator(10, kAP_CODE_pattern23);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern31);
//				}
//				else if(insidemap == 3)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern2);
//					myRS->setNumerator(10, kAP_CODE_pattern7);
//					myRS->setNumerator(20, kAP_CODE_pattern8);
//					myRS->setNumerator(20, kAP_CODE_pattern16);
//					myRS->setNumerator(20, kAP_CODE_pattern18);
//					myRS->setNumerator(10, kAP_CODE_pattern23);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern34);
//				}
//				else if(insidemap == 4)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern4);
//					myRS->setNumerator(20, kAP_CODE_pattern10);
//					myRS->setNumerator(20, kAP_CODE_pattern13);
//					myRS->setNumerator(20, kAP_CODE_pattern16);
//					myRS->setNumerator(20, kAP_CODE_pattern23);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern33);
//				}
//				else if(insidemap == 5)
//				{
//					myRS->setNumerator(20, kAP_CODE_pattern5);
//					myRS->setNumerator(20, kAP_CODE_pattern9);
//					myRS->setNumerator(20, kAP_CODE_pattern16);
//					myRS->setNumerator(10, kAP_CODE_pattern18);
//					myRS->setNumerator(20, kAP_CODE_pattern23);
//					myRS->setNumerator(10, kAP_CODE_pattern30);
//					myRS->setNumerator(10, kAP_CODE_pattern31);
//					myRS->setNumerator(10, kAP_CODE_pattern32);
//				}
//				
//				if(worldmap/10 - 2 > 0)
//				{
//					int rand_value = rand()%3;
//					if(rand_value == 0)			myRS->setNumerator(20, kAP_CODE_pattern25);
//					else if(rand_value == 1)	myRS->setNumerator(20, kAP_CODE_pattern26);
//					else						myRS->setNumerator(20, kAP_CODE_pattern27);
//				}
//			}
//		}
	}
	
	float getClearPercentage(int worldMap, int insideMap)
	{
//		float clearPercentage;
//		if((worldMap <= 20 && worldMap > 10) || (worldMap >= 21 && worldMap >= 31))			clearPercentage = 0.85f;
//		else						clearPercentage = 0.75f;
//		return clearPercentage;
		return 0.85;
	}
	
	float getMaxBossLife(int worldMap, int insideMap)
	{
		float bossLife;
		if(worldMap == 1)
		{
			if(insideMap == 1)				bossLife = 400;
			else if(insideMap == 2)			bossLife = 425;
			else if(insideMap == 3)			bossLife = 450;
			else if(insideMap == 4)			bossLife = 475;
			else if(insideMap == 5)			bossLife = 500;
		}
		else if(worldMap == 2)
		{
			if(insideMap == 1)				bossLife = 540;
			else if(insideMap == 2)			bossLife = 580;
			else if(insideMap == 3)			bossLife = 620;
			else if(insideMap == 4)			bossLife = 660;
			else if(insideMap == 5)			bossLife = 700;
		}
		else if(worldMap == 3)
		{
			if(insideMap == 1)				bossLife = 736;
			else if(insideMap == 2)			bossLife = 771;
			else if(insideMap == 3)			bossLife = 807;
			else if(insideMap == 4)			bossLife = 843;
			else if(insideMap == 5)			bossLife = 879;
		}
		else if(worldMap == 4)
		{
			if(insideMap == 1)				bossLife = 914;
			else if(insideMap == 2)			bossLife = 950;
			else if(insideMap == 3)			bossLife = 989;
			else if(insideMap == 4)			bossLife = 1028;
			else if(insideMap == 5)			bossLife = 1067;
		}
		else if(worldMap == 5)
		{
			if(insideMap == 1)				bossLife = 1106;
			else if(insideMap == 2)			bossLife = 1144;
			else if(insideMap == 3)			bossLife = 1183;
			else if(insideMap == 4)			bossLife = 1222;
			else if(insideMap == 5)			bossLife = 1261;
		}
		else if(worldMap == 6)
		{
			if(insideMap == 1)				bossLife = 1300;
			else if(insideMap == 2)			bossLife = 1329;
			else if(insideMap == 3)			bossLife = 1357;
			else if(insideMap == 4)			bossLife = 1386;
			else if(insideMap == 5)			bossLife = 1414;
		}
		else if(worldMap == 7)
		{
			if(insideMap == 1)				bossLife = 1443;
			else if(insideMap == 2)			bossLife = 1471;
			else if(insideMap == 3)			bossLife = 1500;
			else if(insideMap == 4)			bossLife = 1529;
			else if(insideMap == 5)			bossLife = 1557;
		}
		else if(worldMap == 8)
		{
			if(insideMap == 1)				bossLife = 1586;
			else if(insideMap == 2)			bossLife = 1614;
			else if(insideMap == 3)			bossLife = 1643;
			else if(insideMap == 4)			bossLife = 1671;
			else if(insideMap == 5)			bossLife = 1700;
		}
		else if(worldMap == 9)
		{
			if(insideMap == 1)				bossLife = 1728;
			else if(insideMap == 2)			bossLife = 1756;
			else if(insideMap == 3)			bossLife = 1783;
			else if(insideMap == 4)			bossLife = 1811;
			else if(insideMap == 5)			bossLife = 1839;
		}
		else if(worldMap == 10)
		{
			if(insideMap == 1)				bossLife = 1867;
			else if(insideMap == 2)			bossLife = 1894;
			else if(insideMap == 3)			bossLife = 1922;
			else if(insideMap == 4)			bossLife = 1950;
			else if(insideMap == 5)			bossLife = 1978;
		}
		else if(worldMap == 11)
		{
			if(insideMap == 1)				bossLife = 2006;
			else if(insideMap == 2)			bossLife = 2033;
			else if(insideMap == 3)			bossLife = 2061;
			else if(insideMap == 4)			bossLife = 2089;
			else if(insideMap == 5)			bossLife = 2117;
		}
		else if(worldMap == 12)
		{
			if(insideMap == 1)				bossLife = 2144;
			else if(insideMap == 2)			bossLife = 2172;
			else if(insideMap == 3)			bossLife = 2200;
			else if(insideMap == 4)			bossLife = 2218;
			else if(insideMap == 5)			bossLife = 2236;
		}
		else if(worldMap == 13)
		{
			if(insideMap == 1)				bossLife = 2254;
			else if(insideMap == 2)			bossLife = 2272;
			else if(insideMap == 3)			bossLife = 2290;
			else if(insideMap == 4)			bossLife = 2308;
			else if(insideMap == 5)			bossLife = 2326;
		}
		else if(worldMap == 14)
		{
			if(insideMap == 1)				bossLife = 2344;
			else if(insideMap == 2)			bossLife = 2362;
			else if(insideMap == 3)			bossLife = 2380;
			else if(insideMap == 4)			bossLife = 2398;
			else if(insideMap == 5)			bossLife = 2416;
		}
		else if(worldMap == 15)
		{
			if(insideMap == 1)				bossLife = 2434;
			else if(insideMap == 2)			bossLife = 2452;
			else if(insideMap == 3)			bossLife = 2470;
			else if(insideMap == 4)			bossLife = 2488;
			else if(insideMap == 5)			bossLife = 2506;
		}
		else if(worldMap == 16)
		{
			if(insideMap == 1)				bossLife = 2524;
			else if(insideMap == 2)			bossLife = 2542;
			else if(insideMap == 3)			bossLife = 2560;
			else if(insideMap == 4)			bossLife = 2578;
			else if(insideMap == 5)			bossLife = 2596;
		}
		else if(worldMap == 17)
		{
			if(insideMap == 1)				bossLife = 2614;
			else if(insideMap == 2)			bossLife = 2632;
			else if(insideMap == 3)			bossLife = 2650;
			else if(insideMap == 4)			bossLife = 2667;
			else if(insideMap == 5)			bossLife = 2684;
		}
		else if(worldMap == 18)
		{
			if(insideMap == 1)				bossLife = 2702;
			else if(insideMap == 2)			bossLife = 2719;
			else if(insideMap == 3)			bossLife = 2736;
			else if(insideMap == 4)			bossLife = 2753;
			else if(insideMap == 5)			bossLife = 2770;
		}
		else if(worldMap == 19)
		{
			if(insideMap == 1)				bossLife = 2788;
			else if(insideMap == 2)			bossLife = 2805;
			else if(insideMap == 3)			bossLife = 2822;
			else if(insideMap == 4)			bossLife = 2839;
			else if(insideMap == 5)			bossLife = 2856;
		}
		else if(worldMap == 20)
		{
			if(insideMap == 1)				bossLife = 2873;
			else if(insideMap == 2)			bossLife = 2891;
			else if(insideMap == 3)			bossLife = 2908;
			else if(insideMap == 4)			bossLife = 2925;
			else if(insideMap == 5)			bossLife = 2942;
		}
		else
		{
			int new_stage_number = (worldMap%10 - 1) * 10 + insideMap + (worldMap/10 - 2) * 5;
			int start_life = 3000;
			int last_life = 5000;
			
			bossLife = start_life + (last_life-start_life)/89*(new_stage_number-1);
		}
		
//		if(worldMap <= 20)	bossLife = roundf(bossLife*0.7f);
		
//		if(worldMap <= 20)	bossLife = 1;
//		else				bossLife = 100;
		
		bossLife = roundf(bossLife*1000.f);
		
		return bossLife;
	}
	
	BD_P1 getValuePattern1(int worldMap, int insideMap)
	{
		int balance_case = 4;
		if(worldMap <= 20)
		{
			int changed_worldMap = (worldMap-1)%10+1;
			if(changed_worldMap == 1)						balance_case = 1;
			else if(changed_worldMap == 2)
			{
				if(insideMap <= 3)			balance_case = 2;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 4)
			{
				if(insideMap <= 2)			balance_case = 2;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 5 || changed_worldMap == 6)			balance_case = 3;
			else if(changed_worldMap == 7)							balance_case = 2;
			else if(changed_worldMap == 8)							balance_case = 3;
		}
		else
		{
			balance_case = 3;
		}
		
		if(balance_case == 1)			return BD_P1(1.f, 3, 12.f, CCSize(6.f, 6.f));
		else if(balance_case == 2)		return BD_P1(1.3f, 5, 15.f, CCSize(6.f, 6.f));
		else if(balance_case == 3)		return BD_P1(1.6f, 7, 20.f, CCSize(6.f, 6.f));
		else							return BD_P1(2.f, 7, 20.f, CCSize(6.f, 6.f));
	}
	
	BD_P2 getValuePattern2(int worldMap, int insideMap)
	{
		int balance_case = 7;
		if(worldMap <= 20)
		{
			int changed_worldMap = (worldMap-1)%10+1;
			if(changed_worldMap == 1)		balance_case = 1;
			else if(changed_worldMap == 2)
			{
				if(insideMap <= 3)			balance_case = 2;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 3)
			{
				if(insideMap == 1)			balance_case = 1;
				else						balance_case = 2;
			}
			else if(changed_worldMap == 4)
			{
				if(insideMap <= 2)			balance_case = 2;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 5)	balance_case = 4;
			else if(changed_worldMap == 6)
			{
				if(insideMap <= 1)			balance_case = 4;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 7)
			{
				if(insideMap == 1)			balance_case = 2;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 8)
			{
				if(insideMap <= 3)			balance_case = 3;
				else						balance_case = 4;
			}
			else if(changed_worldMap == 9)
			{
				if(insideMap == 1)			balance_case = 4;
				else if(insideMap <= 4)		balance_case = 5;
				else						balance_case = 6;
			}
			else if(changed_worldMap == 10)	balance_case = 6;
		}
		else
		{
			int changed_worldMap = worldMap%10;
			if((changed_worldMap == 3 && insideMap == 4) || (changed_worldMap == 5 && (insideMap == 2 || insideMap == 5)))		balance_case = 3;
			else if((changed_worldMap == 2 && insideMap == 5) || (changed_worldMap == 3 && insideMap == 1) || (changed_worldMap == 4 && (insideMap == 2 || insideMap == 4)))	balance_case = 4;
			else if((changed_worldMap == 1 && insideMap == 1) || (changed_worldMap == 3 && insideMap == 2) || (changed_worldMap == 4 && insideMap == 1))	balance_case = 5;
			else if((changed_worldMap == 1 && (insideMap == 2 || insideMap == 5)) || (changed_worldMap == 2 && insideMap == 3) || (changed_worldMap == 5 && (insideMap == 1 || insideMap == 3 || insideMap == 4)))	balance_case = 6;
			else if((changed_worldMap == 2 && insideMap == 1) || (changed_worldMap == 6 && (insideMap == 1 || insideMap == 3)))		balance_case = 7;
		}
		
		if(balance_case == 1)			return BD_P2(1.f, 10, CCSize(6.f, 6.f));
		else if(balance_case == 2)		return BD_P2(1.2f, 15, CCSize(6.f, 6.f));
		else if(balance_case == 3)		return BD_P2(1.4f, 20, CCSize(6.f, 6.f));
		else if(balance_case == 4)		return BD_P2(1.6f, 25, CCSize(6.f, 6.f));
		else if(balance_case == 5)		return BD_P2(1.8f, 25, CCSize(6.f, 6.f));
		else if(balance_case == 6)		return BD_P2(2.f, 30, CCSize(6.f, 6.f));
		else							return BD_P2(2.2f, 30, CCSize(6.f, 6.f));
	}
	
	BD_P3 getValuePattern3(int worldMap, int insideMap)
	{
		int balance_case = 7;
		if(worldMap <= 20)
		{
			int changed_worldMap = (worldMap-1)%10+1;
			if(changed_worldMap == 1)				balance_case = 1;
			else if(changed_worldMap == 2)
			{
				if(insideMap <= 3)			balance_case = 2;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 3 || changed_worldMap == 4)			balance_case = 2;
			else if(changed_worldMap == 5 || changed_worldMap == 6)			balance_case = 3;
			else if(changed_worldMap == 7 || changed_worldMap == 8)			balance_case = 1;
			else if(changed_worldMap == 9 || changed_worldMap == 10)		balance_case = 4;
		}
		else
		{
			int changed_worldMap = worldMap%10;
			if(changed_worldMap == 1 && (insideMap == 2 || insideMap == 5))	balance_case = 3;
			else if((changed_worldMap == 3 && (insideMap == 1 || insideMap == 5)) || (changed_worldMap == 4 && insideMap == 3))	balance_case = 4;
			else if((changed_worldMap == 2 && (insideMap == 1 || insideMap == 4)) || (changed_worldMap == 3 && insideMap == 4) || (changed_worldMap == 4 && insideMap == 5) || (changed_worldMap == 5 && (insideMap == 2 || insideMap == 4 || insideMap == 5)))	balance_case = 5;
			else if(changed_worldMap == 6 && insideMap == 2)	balance_case = 6;
		}
		
		if(balance_case == 1)			return BD_P3(1.f, 90, 15, CCSize(6.f, 6.f));
		else if(balance_case == 2)		return BD_P3(1.2f, 150, 15, CCSize(6.f, 6.f));
		else if(balance_case == 3)		return BD_P3(1.4f, 180, 15, CCSize(6.f, 6.f));
		else if(balance_case == 4)		return BD_P3(1.6f, 180, 12, CCSize(6.f, 6.f));
		else if(balance_case == 5)		return BD_P3(1.8f, 90, 9, CCSize(6.f, 6.f));
		else if(balance_case == 6)		return BD_P3(2.f, 120, 8, CCSize(6.f, 6.f));
		else							return BD_P3(2.2f, 90, 9, CCSize(6.f, 6.f));
	}
	
	BD_P4 getValuePattern4(int worldMap, int insideMap)
	{
		int balance_case = 7;
		if(worldMap <= 20)
		{
			int changed_worldMap = (worldMap-1)%10+1;
			if(changed_worldMap == 3)
			{
				if(insideMap <= 4)			balance_case = 1;
				else						balance_case = 2;
			}
			else if(changed_worldMap == 4)
			{
				if(insideMap <= 2)			balance_case = 2;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 5 || changed_worldMap == 6)			balance_case = 2;
			else if(changed_worldMap == 7 || changed_worldMap == 8)			balance_case = 2;
			else if(changed_worldMap == 9 || changed_worldMap == 10)		balance_case = 3;
		}
		else
		{
			int changed_worldMap = worldMap%10;
			if(changed_worldMap == 2 && insideMap == 2)				balance_case = 2;
			else if(changed_worldMap == 1 && (insideMap == 1 || insideMap == 3))	balance_case = 3;
			else if(changed_worldMap == 3 && (insideMap == 2 || insideMap == 5))	balance_case = 4;
			else if(changed_worldMap == 6 && insideMap == 1)	balance_case = 6;
			else if(changed_worldMap == 6 && insideMap == 4)	balance_case = 7;
		}
		
		if(balance_case == 1)			return BD_P4(1.f, 30, 15, CCSize(6.f,6.f), 20.f, 3);
		else if(balance_case == 2)		return BD_P4(1.2f, 60, 15, CCSize(6.f,6.f), 20.f, 4);
		else if(balance_case == 3)		return BD_P4(1.4f, 60, 15, CCSize(6.f,6.f), 25.f, 5);
		else if(balance_case == 4)		return BD_P4(1.6f, 30, 10, CCSize(6.f,6.f), 25.f, 5);
		else if(balance_case == 5)		return BD_P4(1.8f, 60, 10, CCSize(6.f,6.f), 30.f, 5);
		else if(balance_case == 6)		return BD_P4(2.f, 60, 10, CCSize(6.f,6.f), 35.f, 6);
		else							return BD_P4(2.2f, 30, 10, CCSize(6.f,6.f), 35.f, 7);
	}
	
	BD_P5 getValuePattern5(int worldMap, int insideMap)
	{
		int balance_case = 6;
		if(worldMap <= 20)
		{
			int changed_worldMap = (worldMap-1)%10+1;
			if(changed_worldMap == 3)		balance_case = 1;
			else if(changed_worldMap == 4)
			{
				if(insideMap <= 1)			balance_case = 1;
				else if(insideMap <= 4)		balance_case = 2;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 5)
			{
				if(insideMap == 1)			balance_case = 1;
				else if(insideMap <= 4)		balance_case = 2;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 6)	balance_case = 3;
			else if(changed_worldMap == 7)
			{
				if(insideMap <= 3)			balance_case = 1;
				else						balance_case = 2;
			}
			else if(changed_worldMap == 8)
			{
				if(insideMap <= 1)			balance_case = 2;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 9 || changed_worldMap == 10)		balance_case = 4;
		}
		else
		{
			int changed_worldMap = worldMap%10;
			if(changed_worldMap == 5 && (insideMap == 1 || insideMap == 3 || insideMap == 4))	balance_case = 2;
			else if(changed_worldMap == 2 && insideMap == 4)		balance_case = 3;
			else if((changed_worldMap == 1 && (insideMap == 2 || insideMap == 3 || insideMap == 4)) || (changed_worldMap == 2 && insideMap == 2) || (changed_worldMap == 3 && insideMap == 3) || (changed_worldMap == 4 && (insideMap == 1 || insideMap == 5)))		balance_case = 4;
			else if(changed_worldMap == 4 && insideMap == 3)		balance_case = 5;
			else if(changed_worldMap == 6 && (insideMap == 2 || insideMap == 5))	balance_case = 6;
		}
		
		if(balance_case == 1)			return BD_P5(1.f, 10, CCSize(6.f,6.f));
		else if(balance_case == 2)		return BD_P5(1.2f, 15, CCSize(6.f,6.f));
		else if(balance_case == 3)		return BD_P5(1.4f, 20, CCSize(6.f,6.f));
		else if(balance_case == 4)		return BD_P5(1.6f, 25, CCSize(6.f,6.f));
		else if(balance_case == 5)		return BD_P5(1.8f, 30, CCSize(6.f,6.f));
		else							return BD_P5(2.f, 35, CCSize(6.f,6.f));
	}
	
	BD_P8 getValuePattern8(int worldMap, int insideMap)
	{
		int balance_case = 5;
		if(worldMap <= 20)
		{
			int changed_worldMap = (worldMap-1)%10+1;
			if(changed_worldMap == 5)
			{
				if(insideMap <= 4)			balance_case = 1;
				else						balance_case = 2;
			}
			else if(changed_worldMap == 6)
			{
				if(insideMap <= 3)			balance_case = 2;
				else						balance_case = 3;
			}
		}
		else
		{
			int changed_worldMap = worldMap%10;
			if(changed_worldMap == 4 && insideMap == 2)		balance_case = 2;
			else if((changed_worldMap == 1 && insideMap == 3) || (changed_worldMap == 2 && insideMap == 2) || (changed_worldMap == 4 && insideMap == 5) || (changed_worldMap == 5 && (insideMap == 1 || insideMap == 3 || insideMap == 4)))	balance_case = 3;
			else if((changed_worldMap == 2 && insideMap == 5) || (changed_worldMap == 3 && (insideMap == 2 || insideMap == 5)))		balance_case = 4;
			else if(changed_worldMap == 6 && insideMap == 3)	balance_case = 5;
		}
		
		if(balance_case == 1)			return BD_P8(1.2f, 6, CCSize(6.f,6.f));
		else if(balance_case == 2)		return BD_P8(1.4f, 8, CCSize(6.f,6.f));
		else if(balance_case == 3)		return BD_P8(1.6f, 6, CCSize(6.f,6.f));
		else if(balance_case == 4)		return BD_P8(1.8f, 10, CCSize(6.f,6.f));
		else							return BD_P8(2.f, 14, CCSize(6.f,6.f));
	}
	
	BD_P9 getValuePattern9(int worldMap, int insideMap)
	{
		int balance_case = 3;
		if(worldMap <= 20)
		{
			int changed_worldMap = (worldMap-1)%10+1;
			if(changed_worldMap == 7)
			{
				if(insideMap <= 4)			balance_case = 1;
				else						balance_case = 2;
			}
			else if(changed_worldMap == 8)
			{
				if(insideMap <= 3)			balance_case = 2;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 9 || changed_worldMap == 10)		balance_case = 2;
		}
		else
		{
			int changed_worldMap = worldMap%10;
			if((changed_worldMap == 2 && insideMap == 5) || (changed_worldMap == 5 && (insideMap == 2 || insideMap == 5)))	balance_case = 2;
			else if((changed_worldMap == 1 && insideMap == 5) || (changed_worldMap == 3 && insideMap == 3) || (changed_worldMap == 6 && insideMap == 5))	balance_case = 3;
		}
		
		if(balance_case == 1)			return BD_P9(1.5f, CCSize(26.f,26.f), 40);
		else if(balance_case == 2)		return BD_P9(1.8f, CCSize(24.f,24.f), 30);
		else							return BD_P9(2.5f, CCSize(20.f,20.f), 20);
	}
	
	BD_P11 getValuePattern11(int worldMap, int insideMap)
	{
		int balance_case = 4;
		BD_P11 r_value;
		if(worldMap <= 20)
		{
			int changed_worldMap = (worldMap-1)%10+1;
			if(changed_worldMap == 1)			balance_case = 1;
			else if(changed_worldMap == 2)		balance_case = 2;
			else if(changed_worldMap >= 3)		balance_case = 1;
			r_value.mType = 1;
		}
		else
		{
			int changed_worldMap = worldMap%10;
			if(changed_worldMap == 2 && (insideMap == 1 || insideMap == 3))		balance_case = 2;
			else if((changed_worldMap == 2 && insideMap == 4) || (changed_worldMap == 3 && (insideMap == 2 || insideMap == 5)) || (changed_worldMap == 5 && insideMap == 4))	balance_case = 3;

			if(changed_worldMap == 2)	r_value.mType = 2;
			else						r_value.mType = 3;
		}
		
		if(balance_case == 1)
		{
			r_value.move_speed = 1.2f;
			r_value.crash_area = CCSizeMake(8.f, 8.f);
		}
		else if(balance_case == 2)
		{
			r_value.move_speed = 1.4f;
			r_value.crash_area = CCSizeMake(10.f, 10.f);
		}
		else if(balance_case == 3)
		{
			r_value.move_speed = 1.6f;
			r_value.crash_area = CCSizeMake(12.f, 12.f);
		}
		else
		{
			r_value.move_speed = 2.f;
			r_value.crash_area = CCSizeMake(10.f, 10.f);
		}
		
		return r_value;
	}
	
	BD_P14 getValuePattern14(int worldMap, int insideMap)
	{
		int balance_case = 4;
		BD_P14 r_value;
		if(worldMap <= 20)
		{
			int changed_worldMap = (worldMap-1)%10+1;
			if(changed_worldMap == 3)				balance_case = 1;
			else if(changed_worldMap == 4)
			{
				if(insideMap <= 3)			balance_case = 2;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 7 || changed_worldMap == 8)			balance_case = 1;
			r_value.mType = 11;
		}
		else
		{
			int changed_worldMap = worldMap%10;
			if(changed_worldMap == 3 && (insideMap == 1 || insideMap == 2))	balance_case = 2;
			else if((changed_worldMap == 2 && (insideMap == 2 || insideMap == 5)) || (changed_worldMap == 5 && insideMap == 2))		balance_case = 3;
			
			if(changed_worldMap == 2)			r_value.mType = 12;
			else								r_value.mType = 13;
		}
		
		if(balance_case == 1)
		{
			r_value.move_speed = 1.f;
			r_value.mCnt = 2;
			r_value.crash_area = CCSizeMake(4.f, 4.f);
		}
		else if(balance_case == 2)
		{
			r_value.move_speed = 1.2f;
			r_value.mCnt = 3;
			r_value.crash_area = CCSizeMake(4.f, 4.f);
		}
		else if(balance_case == 3)
		{
			r_value.move_speed = 1.5f;
			r_value.mCnt = 4;
			r_value.crash_area = CCSizeMake(4.f, 4.f);
		}
		else
		{
			r_value.move_speed = 1.5f;
			r_value.mCnt = 5;
			r_value.crash_area = CCSizeMake(4.f, 4.f);
		}
		
		return r_value;
	}
	
	BD_P18 getValuePattern18(int worldMap, int insideMap)
	{
		int balance_case = 4;
		if(worldMap <= 20)
		{
			int changed_worldMap = (worldMap-1)%10+1;
			if(changed_worldMap == 5)
			{
				if(insideMap <= 4)			balance_case = 1;
				else						balance_case = 2;
			}
			else if(changed_worldMap == 6)
			{
				if(insideMap <= 3)			balance_case = 2;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 7 || changed_worldMap == 8)			balance_case = 1;
		}
		else
		{
			int changed_worldMap = worldMap%10;
			if(changed_worldMap == 4 && (insideMap == 1 || insideMap == 3 || insideMap == 4))	balance_case = 2;
			else if((changed_worldMap == 1 && (insideMap == 1 || insideMap == 2)) || (changed_worldMap == 2 && (insideMap == 1 || insideMap == 2 || insideMap == 5)) || (changed_worldMap == 3 && insideMap == 3))	balance_case = 3;
			else if(changed_worldMap == 6 && (insideMap == 3 || insideMap == 5))	balance_case = 4;
		}
		
		if(balance_case == 1)			return BD_P18(1.2f, 3, CCSize(8.f,8.f), 4);
		else if(balance_case == 2)		return BD_P18(1.4f, 4, CCSize(8.f,8.f), 3);
		else if(balance_case == 3)		return BD_P18(1.6f, 5, CCSize(14.f,14.f), 5);
		else							return BD_P18(1.8f, 6, CCSize(14.f,14.f), 5);
	}
	
	BD_P28 getValuePattern28(int worldMap, int insideMap)
	{
		int balance_case = 5;
		if(worldMap <= 20)
		{
			int changed_worldMap = (worldMap-1)%10+1;
			if(changed_worldMap == 5)			balance_case = 1;
			else if(changed_worldMap == 6)
			{
				if(insideMap <= 2)			balance_case = 2;
				else						balance_case = 3;
			}
			else if(changed_worldMap == 7 || changed_worldMap == 8)			balance_case = 2;
		}
		else
		{
			int changed_worldMap = worldMap%10;
			if(changed_worldMap == 5 && insideMap == 1)		balance_case = 2;
			else if(changed_worldMap == 3 && (insideMap == 1 || insideMap == 5))	balance_case = 3;
		}
		
		if(balance_case == 1)			return BD_P28(100, 25);
		else if(balance_case == 2)		return BD_P28(90, 23);
		else if(balance_case == 3)		return BD_P28(80, 20);
		else if(balance_case == 4)		return BD_P28(70, 25);
		else							return BD_P28(70, 30);
	}
	
private:
	
	void myInit()
	{
		
	}
};

#endif
