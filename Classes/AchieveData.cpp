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
	data_map[kAchievementCode_mapGacha1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_mapGacha2] = AchieveConditionRewardData(20, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_mapGacha3] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_luckySeven1] = AchieveConditionRewardData(70, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_luckySeven2] = AchieveConditionRewardData(77, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_luckySeven3] = AchieveConditionRewardData(777, kAchieveRewardType_ruby, 1);
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
	data_map[kAchievementCode_fastClear1] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_fastClear2] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_fastClear3] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_changeMania1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_changeMania2] = AchieveConditionRewardData(20, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_changeMania3] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_fail1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_fail2] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_fail3] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_cardCollection1] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_cardCollection2] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_cardCollection3] = AchieveConditionRewardData(200, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_perfect1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_perfect2] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_perfect3] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_seqAttendance1] = AchieveConditionRewardData(7, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_seqAttendance2] = AchieveConditionRewardData(14, kAchieveRewardType_ruby, 1);
	data_map[kAchievementCode_seqAttendance3] = AchieveConditionRewardData(21, kAchieveRewardType_ruby, 1);
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
	else if(t_code == kAchievementCode_fastClear1)
	{	return_value.title = "초스피드클리어1";return_value.content = "30초안에 클리어";	}
	else if(t_code == kAchievementCode_fastClear2)
	{	return_value.title = "초스피드클리어2";return_value.content = "10초안에 클리어";	}
	else if(t_code == kAchievementCode_fastClear3)
	{	return_value.title = "초스피드클리어3";return_value.content = "5초안에 클리어";	}
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
	{	return_value = mySGD->getGoodsValue(kGoodsType_gold);	}
	else if(t_code == kAchievementCode_ruby1 || t_code == kAchievementCode_ruby2 || t_code == kAchievementCode_ruby3)
	{	return_value = mySGD->getGoodsValue(kGoodsType_ruby);	}
	else if(t_code == kAchievementCode_mapGacha1 || t_code == kAchievementCode_mapGacha2 || t_code == kAchievementCode_mapGacha3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_mapGachaCnt);	}
	else if(t_code == kAchievementCode_luckySeven1)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_luckySeven1);	}
	else if(t_code == kAchievementCode_luckySeven2)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_luckySeven2);	}
	else if(t_code == kAchievementCode_luckySeven3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_luckySeven3);	}
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
	else if(t_code == kAchievementCode_fastClear1)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_fastClear1);	}
	else if(t_code == kAchievementCode_fastClear2)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_fastClear2);	}
	else if(t_code == kAchievementCode_fastClear3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_fastClear3);	}
	else if(t_code == kAchievementCode_changeMania1 || t_code == kAchievementCode_changeMania2 || t_code == kAchievementCode_changeMania3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_changeCnt);	}
	else if(t_code == kAchievementCode_fail1 || t_code == kAchievementCode_fail2 || t_code == kAchievementCode_fail3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_failCnt);	}
	else if(t_code == kAchievementCode_cardCollection1 || t_code == kAchievementCode_cardCollection2 || t_code == kAchievementCode_cardCollection3)
	{
		return_value = mySGD->getHasGottenCardsSize();
	}
	else if(t_code == kAchievementCode_perfect1 || t_code == kAchievementCode_perfect2 || t_code == kAchievementCode_perfect3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_perfectClearCnt);	}
	else if(t_code == kAchievementCode_seqAttendance1 || t_code == kAchievementCode_seqAttendance2 || t_code == kAchievementCode_seqAttendance3)
	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_seqAttendanceCnt);	}
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