//
//  AchieveData.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 31..
//
//

#include "AchieveData.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "UnknownFriends.h"
#include "ServerDataSave.h"

void AchieveConditionReward::myInit()
{
	data_map[kAchievementCode_gold1] = AchieveConditionRewardData(1000, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_gold2] = AchieveConditionRewardData(10000, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_gold3] = AchieveConditionRewardData(100000, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_ruby1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_ruby2] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_ruby3] = AchieveConditionRewardData(1000, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_heart1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_heart2] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_heart3] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_bonusGame1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_bonusGame2] = AchieveConditionRewardData(20, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_bonusGame3] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_mapGacha1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_mapGacha2] = AchieveConditionRewardData(20, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_mapGacha3] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_luckySeven1] = AchieveConditionRewardData(70, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_luckySeven2] = AchieveConditionRewardData(77, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_luckySeven3] = AchieveConditionRewardData(777, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_feverMania1] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_feverMania2] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_feverMania3] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_comboMania1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_comboMania2] = AchieveConditionRewardData(20, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_comboMania3] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_noFail1] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_noFail2] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_noFail3] = AchieveConditionRewardData(20, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_attacker1] = AchieveConditionRewardData(20, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_attacker2] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_attacker3] = AchieveConditionRewardData(40, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hunter1] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hunter2] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hunter3] = AchieveConditionRewardData(20, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_dieHard1] = AchieveConditionRewardData(1, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_dieHard2] = AchieveConditionRewardData(1, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_dieHard3] = AchieveConditionRewardData(1, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_scoreHigh1] = AchieveConditionRewardData(50000, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_scoreHigh2] = AchieveConditionRewardData(100000, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_scoreHigh3] = AchieveConditionRewardData(150000, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_allPiece1] = AchieveConditionRewardData(1, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_allPiece2] = AchieveConditionRewardData(1, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_allPiece3] = AchieveConditionRewardData(1, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_fastClear1] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_fastClear2] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_fastClear3] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_continue1] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_continue2] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_continue3] = AchieveConditionRewardData(20, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_attendance1] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_attendance2] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_attendance3] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_changeMania1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_changeMania2] = AchieveConditionRewardData(20, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_changeMania3] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_fail1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_fail2] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_fail3] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_cardCollection1] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_cardCollection2] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_cardCollection3] = AchieveConditionRewardData(200, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_friend1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_friend2] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_friend3] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_invite1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_invite2] = AchieveConditionRewardData(20, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_invite3] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_challenger1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_challenger2] = AchieveConditionRewardData(20, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_challenger3] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_help1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_help2] = AchieveConditionRewardData(20, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_help3] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_helper1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_helper2] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_helper3] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_defender1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_defender2] = AchieveConditionRewardData(20, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_defender3] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_upgrade1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_upgrade2] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_upgrade3] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_perfect1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_perfect2] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_perfect3] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_seqAttendance1] = AchieveConditionRewardData(7, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_seqAttendance2] = AchieveConditionRewardData(14, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_seqAttendance3] = AchieveConditionRewardData(21, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_gacha1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_gacha2] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_gacha3] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_gacha4] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_gachaKing] = AchieveConditionRewardData(4, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_weeklyKing] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_puzzleKing] = AchieveConditionRewardData(1, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_characterCollection] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_luckyGuy] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_goldMania] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_tutorial] = AchieveConditionRewardData(1, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_cardSet] = AchieveConditionRewardData(1, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_reviewer] = AchieveConditionRewardData(1, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hidden_returnee] = AchieveConditionRewardData(7, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hidden_bigHand1] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hidden_bigHand2] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hidden_holder] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hidden_noSound] = AchieveConditionRewardData(1, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hidden_shopper1] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hidden_shopper2] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hidden_shopper3] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hidden_breathtaking1] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hidden_breathtaking2] = AchieveConditionRewardData(1, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hidden_dieEasy] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_hidden_speedMania] = AchieveConditionRewardData(1, kAchieveRewardType_ruby, 1);
}

AchieveTitleContent AchieveTitleContent::getAchievementScript(AchievementCode t_code)
{
	AchieveTitleContent return_value;
	
	if(t_code == kAchievementCode_gold1)
	{	return_value.title = "골드서민";return_value.content = "1000골드 모으기";		}
	else if(t_code == kAchievementCode_gold2)
	{	return_value.title = "골드부자";return_value.content = "10000골드 모으기";	}
	else if(t_code == kAchievementCode_gold3)
	{	return_value.title = "골드재벌";return_value.content = "100000골드 모으기";	}
	else if(t_code == kAchievementCode_ruby1)
	{	return_value.title = "루비서민";return_value.content = "10루비모으기";	}
	else if(t_code == kAchievementCode_ruby2)
	{	return_value.title = "루비부자";return_value.content = "100루비모으기";	}
	else if(t_code == kAchievementCode_ruby3)
	{	return_value.title = "루비재벌";return_value.content = "1000루비모으기";	}
	else if(t_code == kAchievementCode_heart1)
	{	return_value.title = "큐피트";return_value.content = "10하트보내기";	}
	else if(t_code == kAchievementCode_heart2)
	{	return_value.title = "바람둥이";return_value.content = "30하트보내기";	}
	else if(t_code == kAchievementCode_heart3)
	{	return_value.title = "카사노바";return_value.content = "50하트보내기";	}
	else if(t_code == kAchievementCode_bonusGame1)
	{	return_value.title = "보너스게임아마추어";return_value.content = "보너스게임 10회 성공";	}
	else if(t_code == kAchievementCode_bonusGame2)
	{	return_value.title = "보너스게임프로";return_value.content = "보너스게임 20회 성공";	}
	else if(t_code == kAchievementCode_bonusGame3)
	{	return_value.title = "보너스게임마스터";return_value.content = "보너스게임 30회 성공";	}
	else if(t_code == kAchievementCode_mapGacha1)
	{	return_value.title = "맵가챠맨1";return_value.content = "맵가챠 10회 돌리기";	}
	else if(t_code == kAchievementCode_mapGacha2)
	{	return_value.title = "맵가챠맨2";return_value.content = "맵가챠 20회 돌리기";	}
	else if(t_code == kAchievementCode_mapGacha3)
	{	return_value.title = "맵가챠맨3";return_value.content = "맵가챠 30회 돌리기";	}
	else if(t_code == kAchievementCode_luckySeven1)
	{	return_value.title = "럭키세븐1";return_value.content = "한번에 7% 먹기";	}
	else if(t_code == kAchievementCode_luckySeven2)
	{	return_value.title = "럭키세븐2";return_value.content = "한번에 7.7% 먹기";	}
	else if(t_code == kAchievementCode_luckySeven3)
	{	return_value.title = "럭키세븐3";return_value.content = "한번에 77.7% 먹기";	}
	else if(t_code == kAchievementCode_feverMania1)
	{	return_value.title = "피버매니아1";return_value.content = "한판에 피버모드 5회진입시";	}
	else if(t_code == kAchievementCode_feverMania2)
	{	return_value.title = "피버매니아2";return_value.content = "전체 피버모드 30회진입시";	}
	else if(t_code == kAchievementCode_feverMania3)
	{	return_value.title = "피버매니아3";return_value.content = "전체 피버모드 50회진입시";	}
	else if(t_code == kAchievementCode_comboMania1)
	{	return_value.title = "콤보매니아1";return_value.content = "10콤보달성";	}
	else if(t_code == kAchievementCode_comboMania2)
	{	return_value.title = "콤보매니아2";return_value.content = "20콤보달성";	}
	else if(t_code == kAchievementCode_comboMania3)
	{	return_value.title = "콤보매니아3";return_value.content = "30콤보달성";	}
	else if(t_code == kAchievementCode_noFail1)
	{	return_value.title = "실패란없다1";return_value.content = "5회연속 클리어";	}
	else if(t_code == kAchievementCode_noFail2)
	{	return_value.title = "실패란없다2";return_value.content = "10회연속클리어";	}
	else if(t_code == kAchievementCode_noFail3)
	{	return_value.title = "실패란없다3";return_value.content = "20회연속클리어";	}
	else if(t_code == kAchievementCode_attacker1)
	{	return_value.title = "닥치고공격1";return_value.content = "한판에 20회공격 성공";	}
	else if(t_code == kAchievementCode_attacker2)
	{	return_value.title = "닥치고공격2";return_value.content = "한판에 30회공격 성공";	}
	else if(t_code == kAchievementCode_attacker3)
	{	return_value.title = "닥치고공격3";return_value.content = "한판에 40회공격 성공";	}
	else if(t_code == kAchievementCode_hunter1)
	{	return_value.title = "사냥꾼1";return_value.content = "부하몹 5마리 가두어잡기";	}
	else if(t_code == kAchievementCode_hunter2)
	{	return_value.title = "사냥꾼2";return_value.content = "부하몹 10마리 가두어잡기";	}
	else if(t_code == kAchievementCode_hunter3)
	{	return_value.title = "사냥꾼3";return_value.content = "부하몹 20마리 가두어잡기";	}
	else if(t_code == kAchievementCode_dieHard1)
	{	return_value.title = "다이하드1";return_value.content = "1~3level 에서 한목숨도 안죽고 클리어";	}
	else if(t_code == kAchievementCode_dieHard2)
	{	return_value.title = "다이하드2";return_value.content = "4~9level 에서 한목숨도 안죽고 클리어";	}
	else if(t_code == kAchievementCode_dieHard3)
	{	return_value.title = "다이하드3";return_value.content = "10level 이상에서 한목숨도 안죽고 클리어";	}
	else if(t_code == kAchievementCode_scoreHigh1)
	{	return_value.title = "경쟁의신1";return_value.content = "50000점 이상 획득";	}
	else if(t_code == kAchievementCode_scoreHigh2)
	{	return_value.title = "경쟁의신2";return_value.content = "100000점 이상 획득";	}
	else if(t_code == kAchievementCode_scoreHigh3)
	{	return_value.title = "경쟁의신3";return_value.content = "150000점 이상 획득";	}
	else if(t_code == kAchievementCode_allPiece1)
	{	return_value.title = "흑백퍼즐";return_value.content = "모든피스가 흑백일때";	}
	else if(t_code == kAchievementCode_allPiece2)
	{	return_value.title = "보더퍼즐";return_value.content = "모든피스가 보더일때";	}
	else if(t_code == kAchievementCode_allPiece3)
	{	return_value.title = "퍼즐완성";return_value.content = "모든피스가 컬러일때";	}
	else if(t_code == kAchievementCode_fastClear1)
	{	return_value.title = "초스피드클리어1";return_value.content = "30초안에 클리어";	}
	else if(t_code == kAchievementCode_fastClear2)
	{	return_value.title = "초스피드클리어2";return_value.content = "10초안에 클리어";	}
	else if(t_code == kAchievementCode_fastClear3)
	{	return_value.title = "초스피드클리어3";return_value.content = "5초안에 클리어";	}
	else if(t_code == kAchievementCode_continue1)
	{	return_value.title = "이어하기매니아1";return_value.content = "5회 이어하기";	}
	else if(t_code == kAchievementCode_continue2)
	{	return_value.title = "이어하기매니아2";return_value.content = "10회 이어하기";	}
	else if(t_code == kAchievementCode_continue3)
	{	return_value.title = "이어하기매니아3";return_value.content = "20회 이어하기";	}
	else if(t_code == kAchievementCode_attendance1)
	{	return_value.title = "출석체크1";return_value.content = "5회출첵";	}
	else if(t_code == kAchievementCode_attendance2)
	{	return_value.title = "출석체크2";return_value.content = "10회출첵";	}
	else if(t_code == kAchievementCode_attendance3)
	{	return_value.title = "출석체크3";return_value.content = "30회출첵";	}
	else if(t_code == kAchievementCode_changeMania1)
	{	return_value.title = "체인지1";return_value.content = "10회 체인지";	}
	else if(t_code == kAchievementCode_changeMania2)
	{	return_value.title = "체인지2";return_value.content = "20회 체인지";	}
	else if(t_code == kAchievementCode_changeMania3)
	{	return_value.title = "체인지3";return_value.content = "30회 체인지";	}
	else if(t_code == kAchievementCode_fail1)
	{	return_value.title = "실패는 성공의 어머니1";return_value.content = "게임실패 10회";	}
	else if(t_code == kAchievementCode_fail2)
	{	return_value.title = "실패는 성공의 어머니2";return_value.content = "게임실패 50회";	}
	else if(t_code == kAchievementCode_fail3)
	{	return_value.title = "실패는 성공의 어머니3";return_value.content = "게임실패 100회";	}
	else if(t_code == kAchievementCode_cardCollection1)
	{	return_value.title = "카드컬렉터1";return_value.content = "카드 50장모으기";	}
	else if(t_code == kAchievementCode_cardCollection2)
	{	return_value.title = "카드컬렉터2";return_value.content = "카드 100장모으기";	}
	else if(t_code == kAchievementCode_cardCollection3)
	{	return_value.title = "카드컬렉터3";return_value.content = "카드 200장모으기";	}
	else if(t_code == kAchievementCode_friend1)
	{	return_value.title = "마당발1";return_value.content = "게임친구 10명이상";	}
	else if(t_code == kAchievementCode_friend2)
	{	return_value.title = "마당발2";return_value.content = "게임친구 30명이상";	}
	else if(t_code == kAchievementCode_friend3)
	{	return_value.title = "마당발3";return_value.content = "게임친구 50명달성";	}
	else if(t_code == kAchievementCode_invite1)
	{	return_value.title = "초대1";return_value.content = "카카오친구 10명이상초대";	}
	else if(t_code == kAchievementCode_invite2)
	{	return_value.title = "초대2";return_value.content = "카카오친구 20명이상초대";	}
	else if(t_code == kAchievementCode_invite3)
	{	return_value.title = "초대3";return_value.content = "카카오친구 30명이상초대";	}
	else if(t_code == kAchievementCode_challenger1)
	{	return_value.title = "승부사1";return_value.content = "도전하기 10번";	}
	else if(t_code == kAchievementCode_challenger2)
	{	return_value.title = "승부사2";return_value.content = "도전하기 20번";	}
	else if(t_code == kAchievementCode_challenger3)
	{	return_value.title = "승부사3";return_value.content = "도전하기 30번";	}
	else if(t_code == kAchievementCode_help1)
	{	return_value.title = "도움요청1";return_value.content = "도움요청 10번";	}
	else if(t_code == kAchievementCode_help2)
	{	return_value.title = "도움요청2";return_value.content = "도움요청 20번";	}
	else if(t_code == kAchievementCode_help3)
	{	return_value.title = "도움요청3";return_value.content = "도움요청 30번";	}
	else if(t_code == kAchievementCode_helper1)
	{	return_value.title = "흑기사1";return_value.content = "친구의 도움요청 들어주기 10회";	}
	else if(t_code == kAchievementCode_helper2)
	{	return_value.title = "흑기사2";return_value.content = "친구의 도움요청 들어주기 30회";	}
	else if(t_code == kAchievementCode_helper3)
	{	return_value.title = "흑기사3";return_value.content = "친구의 도움요청 들어주기 50회";	}
	else if(t_code == kAchievementCode_defender1)
	{	return_value.title = "덤벼1";return_value.content = "친구의 도전 수락 10회";	}
	else if(t_code == kAchievementCode_defender2)
	{	return_value.title = "덤벼2";return_value.content = "친구의 도전 수락 20회";	}
	else if(t_code == kAchievementCode_defender3)
	{	return_value.title = "덤벼3";return_value.content = "친구의 도전 수락 30회";	}
	else if(t_code == kAchievementCode_upgrade1)
	{	return_value.title = "강화장인1";return_value.content = "강화 10회성공";	}
	else if(t_code == kAchievementCode_upgrade2)
	{	return_value.title = "강화장인2";return_value.content = "강화 30회성공";	}
	else if(t_code == kAchievementCode_upgrade3)
	{	return_value.title = "강화장인3";return_value.content = "강화 50회성공";	}
	else if(t_code == kAchievementCode_perfect1)
	{	return_value.title = "대단한성과1";return_value.content = "100%클리어 10회";	}
	else if(t_code == kAchievementCode_perfect2)
	{	return_value.title = "대단한성과2";return_value.content = "100%클리어 30회";	}
	else if(t_code == kAchievementCode_perfect3)
	{	return_value.title = "대단한성과3";return_value.content = "100%클리어 50회";	}
	else if(t_code == kAchievementCode_seqAttendance1)
	{	return_value.title = "개근1";return_value.content = "7일 연속출첵";	}
	else if(t_code == kAchievementCode_seqAttendance2)
	{	return_value.title = "개근2";return_value.content = "14일 연속출첵";	}
	else if(t_code == kAchievementCode_seqAttendance3)
	{	return_value.title = "개근3";return_value.content = "21일 연속출첵";	}
	else if(t_code == kAchievementCode_gacha1)
	{	return_value.title = "경마꾼";return_value.content = "경마가챠 10회이상";	}
	else if(t_code == kAchievementCode_gacha2)
	{	return_value.title = "야바위꾼";return_value.content = "야바위가챠 10회이상";	}
	else if(t_code == kAchievementCode_gacha3)
	{	return_value.title = "룰렛꾼";return_value.content = "룰렛가챠 10회이상";	}
	else if(t_code == kAchievementCode_gacha4)
	{	return_value.title = "슬롯버신꾼";return_value.content = "슬롯머신 10회이상";	}
	else if(t_code == kAchievementCode_gachaKing)
	{	return_value.title = "도박의신";return_value.content = "경마&야바위&룰렛&슬롯머신 모두 달성시";	}
	else if(t_code == kAchievementCode_weeklyKing)
	{	return_value.title = "주간킹";return_value.content = "주간랭킹 1등 5회";	}
	else if(t_code == kAchievementCode_puzzleKing)
	{	return_value.title = "퍼즐킹";return_value.content = "한퍼즐내에 모든피스 땅따먹기 1등";	}
	else if(t_code == kAchievementCode_characterCollection)
	{	return_value.title = "캐릭터컬렉터";return_value.content = "캐릭터 5개이상 보유";	}
	else if(t_code == kAchievementCode_luckyGuy)
	{	return_value.title = "럭키가이";return_value.content = "시작행운아이템 5회성공";	}
	else if(t_code == kAchievementCode_goldMania)
	{	return_value.title = "골드매니아";return_value.content = "피버모드에서 한번에 100골드 이상획득";	}
	else if(t_code == kAchievementCode_tutorial)
	{	return_value.title = "초보자";return_value.content = "튜토리얼다 보기";	}
	else if(t_code == kAchievementCode_cardSet)
	{	return_value.title = "셋트완성";return_value.content = "한스테이지의 1,2,3단계 카드셋트 완성";	}
	
	else if(t_code == kAchievementCode_reviewer)
	{	return_value.title = "리뷰어";return_value.content = "리뷰남겨주기";	}
	
	else if(t_code == kAchievementCode_hidden_returnee) // (히든)
	{	return_value.title = "오랜만이야";return_value.content = "일주일만에 접속";	}
	else if(t_code == kAchievementCode_hidden_bigHand1) // (히든)
	{	return_value.title = "욕심쟁이1";return_value.content = "한번에 먹은 영역이 50%이상일때";	}
	else if(t_code == kAchievementCode_hidden_bigHand2) // (히든)
	{	return_value.title = "욕심쟁이2";return_value.content = "한번에 먹은 영역이 100%이상일때";	}
	else if(t_code == kAchievementCode_hidden_holder) // (히든)
	{	return_value.title = "잉여킹";return_value.content = "대기실에 켜놓고 100초 이상 경과";	}
	else if(t_code == kAchievementCode_hidden_noSound) // (히든)
	{	return_value.title = "사운드따윈필요없어";return_value.content = "설정에 사운드 끄고 플레이후 클리어";	}
	else if(t_code == kAchievementCode_hidden_shopper1) // (히든)
	{	return_value.title = "쇼핑홀릭1";return_value.content = "상점메뉴 100회 진입시";	}
	else if(t_code == kAchievementCode_hidden_shopper2) // (히든)
	{	return_value.title = "쇼핑홀릭2";return_value.content = "아이템 50번째 구매";	}
	else if(t_code == kAchievementCode_hidden_shopper3) // (히든)
	{	return_value.title = "쇼핑홀릭3";return_value.content = "아이템 100번째 구매";	}
	else if(t_code == kAchievementCode_hidden_breathtaking1) // (히든)
	{	return_value.title = "아슬아슬1";return_value.content = "5초이하로 남기고 클리어";	}
	else if(t_code == kAchievementCode_hidden_breathtaking2) // (히든)
	{	return_value.title = "아슬아슬2";return_value.content = "1초이하로 남기고 클리어";	}
	else if(t_code == kAchievementCode_hidden_dieEasy) // (히든)
	{	return_value.title = "다이이지";return_value.content = "게임시작 5초안에 죽기";	}
	else if(t_code == kAchievementCode_hidden_speedMania) // (히든)
	{	return_value.title = "스피드매니아";return_value.content = "스피드 max찍기";	}
	
	return return_value;
}

bool AchieveConditionReward::isAchieve(AchievementCode t_code)
{
	bool return_value;
	
	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, t_code) == 1;
	
	return return_value;
}

int AchieveConditionReward::getRecentValue(AchievementCode t_code) // -1 인지 먼저 확인한 후 들어옴, condition 과 비교되는 값을 보여줌, 즉 달성률 확인용
{
	int return_value;
	
	if(t_code == kAchievementCode_gold1 || t_code == kAchievementCode_gold2 || t_code == kAchievementCode_gold3)
	{	return_value = mySGD->getGold();	}
	else if(t_code == kAchievementCode_ruby1 || t_code == kAchievementCode_ruby2 || t_code == kAchievementCode_ruby3)
	{	return_value = mySGD->getStar();	}
	else if(t_code == kAchievementCode_heart1 || t_code == kAchievementCode_heart2 || t_code == kAchievementCode_heart3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_sendHeartCnt);	}
	else if(t_code == kAchievementCode_bonusGame1 || t_code == kAchievementCode_bonusGame2 || t_code == kAchievementCode_bonusGame3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_playBonusGameCnt);	}
	else if(t_code == kAchievementCode_mapGacha1 || t_code == kAchievementCode_mapGacha2 || t_code == kAchievementCode_mapGacha3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_mapGachaCnt);	}
	else if(t_code == kAchievementCode_luckySeven1)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_luckySeven1);	}
	else if(t_code == kAchievementCode_luckySeven2)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_luckySeven2);	}
	else if(t_code == kAchievementCode_luckySeven3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_luckySeven3);	}
	else if(t_code == kAchievementCode_feverMania1)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_feverMania1);	}
	else if(t_code == kAchievementCode_feverMania2 || kAchievementCode_feverMania3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_totalFeverCnt);	}
	else if(t_code == kAchievementCode_comboMania1)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_comboMania1);	}
	else if(t_code == kAchievementCode_comboMania2)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_comboMania2);	}
	else if(t_code == kAchievementCode_comboMania3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_comboMania3);	}
	else if(t_code == kAchievementCode_noFail1 || t_code == kAchievementCode_noFail2 || t_code == kAchievementCode_noFail3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_seqNoFailCnt);	}
	else if(t_code == kAchievementCode_attacker1)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_attacker1);	}
	else if(t_code == kAchievementCode_attacker2)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_attacker2);	}
	else if(t_code == kAchievementCode_attacker3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_attacker3);	}
	else if(t_code == kAchievementCode_hunter1 || t_code == kAchievementCode_hunter2 || t_code == kAchievementCode_hunter3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_catchMonsterCnt);	}
	else if(t_code == kAchievementCode_dieHard1)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_dieHard1);	}
	else if(t_code == kAchievementCode_dieHard2)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_dieHard2);	}
	else if(t_code == kAchievementCode_dieHard3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_dieHard3);	}
	else if(t_code == kAchievementCode_scoreHigh1)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_scoreHigh1);	}
	else if(t_code == kAchievementCode_scoreHigh2)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_scoreHigh2);	}
	else if(t_code == kAchievementCode_scoreHigh3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_scoreHigh3);	}
	else if(t_code == kAchievementCode_allPiece1)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_allPiece1);	}
	else if(t_code == kAchievementCode_allPiece2)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_allPiece2);	}
	else if(t_code == kAchievementCode_allPiece3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_allPiece3);	}
	else if(t_code == kAchievementCode_fastClear1)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_fastClear1);	}
	else if(t_code == kAchievementCode_fastClear2)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_fastClear2);	}
	else if(t_code == kAchievementCode_fastClear3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_fastClear3);	}
	else if(t_code == kAchievementCode_continue1 || t_code == kAchievementCode_continue2 || t_code == kAchievementCode_continue3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_continueCnt);	}
	else if(t_code == kAchievementCode_attendance1 || t_code == kAchievementCode_attendance2 || t_code == kAchievementCode_attendance3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_attendanceCnt);	}
	else if(t_code == kAchievementCode_changeMania1 || t_code == kAchievementCode_changeMania2 || t_code == kAchievementCode_changeMania3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_changeCnt);	}
	else if(t_code == kAchievementCode_fail1 || t_code == kAchievementCode_fail2 || t_code == kAchievementCode_fail3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_failCnt);	}
	else if(t_code == kAchievementCode_cardCollection1 || t_code == kAchievementCode_cardCollection2 || t_code == kAchievementCode_cardCollection3)
	{
		int recent_take_card_number = 0;
		
		int take_card_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
		for(int i=1;i<=take_card_cnt;i++)
		{
			int card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
			if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0)
				recent_take_card_number++;
		}
		
		return_value = recent_take_card_number;
	}
	else if(t_code == kAchievementCode_friend1 || t_code == kAchievementCode_friend2 || t_code == kAchievementCode_friend3)
	{
		int recent_game_friend_cnt = UnknownFriends::getInstance()->getFriends().size();
		return recent_game_friend_cnt;
	}
	else if(t_code == kAchievementCode_invite1 || t_code == kAchievementCode_invite2 || t_code == kAchievementCode_invite3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_inviteCnt);	}
	else if(t_code == kAchievementCode_challenger1 || t_code == kAchievementCode_challenger2 || t_code == kAchievementCode_challenger3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_challengeCnt);	}
	else if(t_code == kAchievementCode_help1 || t_code == kAchievementCode_help2 || t_code == kAchievementCode_help3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_helpCnt);	}
	else if(t_code == kAchievementCode_helper1 || t_code == kAchievementCode_helper2 || t_code == kAchievementCode_helper3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_helpAcceptCnt);	}
	else if(t_code == kAchievementCode_defender1 || t_code == kAchievementCode_defender2 || t_code == kAchievementCode_defender3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_challengeAcceptCnt);	}
	else if(t_code == kAchievementCode_upgrade1 || t_code == kAchievementCode_upgrade2 || t_code == kAchievementCode_upgrade3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_upgradeSuccessCnt);	}
	else if(t_code == kAchievementCode_perfect1 || t_code == kAchievementCode_perfect2 || t_code == kAchievementCode_perfect3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_perfectClearCnt);	}
	else if(t_code == kAchievementCode_seqAttendance1 || t_code == kAchievementCode_seqAttendance2 || t_code == kAchievementCode_seqAttendance3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_seqAttendanceCnt);	}
	else if(t_code == kAchievementCode_gacha1)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_gacha1Cnt);	}
	else if(t_code == kAchievementCode_gacha2)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_gacha2Cnt);	}
	else if(t_code == kAchievementCode_gacha3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_gacha3Cnt);	}
	else if(t_code == kAchievementCode_gacha4)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_gacha4Cnt);	}
	else if(t_code == kAchievementCode_gachaKing)
	{
		return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_gachaKing);
		if(getRecentValue(kAchievementCode_gacha1) >= getCondition(kAchievementCode_gacha1) &&
		   getRecentValue(kAchievementCode_gacha2) >= getCondition(kAchievementCode_gacha2) &&
		   getRecentValue(kAchievementCode_gacha3) >= getCondition(kAchievementCode_gacha3) &&
		   getRecentValue(kAchievementCode_gacha4) >= getCondition(kAchievementCode_gacha4))
			return_value = 1;
	}
	else if(t_code == kAchievementCode_weeklyKing)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_weeklyTopCnt);	}
	else if(t_code == kAchievementCode_puzzleKing)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_puzzleKing);	}
	else if(t_code == kAchievementCode_characterCollection)
	{
		int unlocked_character_cnt = 0;
		for(int i=1;i<NSDS_GI(kSDS_GI_characterCount_i);i++)
		{
			if(myDSH->getBoolForKey(kDSH_Key_isCharacterUnlocked_int1, i))
				unlocked_character_cnt++;
		}
		return_value = unlocked_character_cnt;
	}
	else if(t_code == kAchievementCode_luckyGuy)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_startLuckyCnt);	}
	else if(t_code == kAchievementCode_goldMania)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_goldMania);	}
	else if(t_code == kAchievementCode_tutorial)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_tutorial);	}
	else if(t_code == kAchievementCode_cardSet)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_cardSet);	}
	
	else if(t_code == kAchievementCode_reviewer)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_reviewer);	}
	
	else if(t_code == kAchievementCode_hidden_returnee) // (히든)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_returnee);	}
	else if(t_code == kAchievementCode_hidden_bigHand1) // (히든)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_bigHand1);	}
	else if(t_code == kAchievementCode_hidden_bigHand2) // (히든)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_bigHand2);	}
	else if(t_code == kAchievementCode_hidden_holder) // (히든)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_holder);	}
	else if(t_code == kAchievementCode_hidden_noSound) // (히든)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_noSound);	}
	else if(t_code == kAchievementCode_hidden_shopper1) // (히든)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_enterShopCnt);	}
	else if(t_code == kAchievementCode_hidden_shopper2 || t_code == kAchievementCode_hidden_shopper3) // (히든)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_itemBuyCnt);	}
	else if(t_code == kAchievementCode_hidden_breathtaking1) // (히든)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_breathtaking1);	}
	else if(t_code == kAchievementCode_hidden_breathtaking2) // (히든)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_breathtaking2);	}
	else if(t_code == kAchievementCode_hidden_dieEasy) // (히든)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_dieEasy);	}
	else if(t_code == kAchievementCode_hidden_speedMania) // (히든)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_speedMania);	}
	
	return return_value;
}