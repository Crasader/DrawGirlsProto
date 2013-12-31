//
//  AchieveData.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 31..
//
//

#ifndef __DGproto__AchieveData__
#define __DGproto__AchieveData__

#include "cocos2d.h"
#include "KSProtect.h"
#include <map>

using namespace std;

enum AchievementCode
{
	kAchievementCode_gold1 = 1,//	골드서민 : 1000골드 모으기.
	kAchievementCode_gold2,//	골드부자 : 10000골드 모으기.
	kAchievementCode_gold3,//	골드재벌 : 100000골드 모으기.

	kAchievementCode_ruby1,//	루비서민 : 10골드모으기.
	kAchievementCode_ruby2,//	루비부자 : 100루비모으기.
	kAchievementCode_ruby3,//	루비재벌 : 1000루비모으기.
	
	kAchievementCode_heart1,//	큐피트 : 10하트보내기.
	kAchievementCode_heart2,//	바람둥이 : 30하트보내기.
	kAchievementCode_heart3,//	카사노바 : 50하트보내기.
	
	kAchievementCode_social1,//	스타: 10소셜포인트모으기.
	kAchievementCode_social2,//	탑스타 : 30소셜포인트모으기.
	kAchievementCode_social3,//	월드스타 : 50소셜포인트모으기.
	
	kAchievementCode_bonusGame1,//	보너스게임아마추어 : 보너스게임 10회 성공
	kAchievementCode_bonusGame2,//	보너스게임프로 : 보너스게임 20회 성공
	kAchievementCode_bonusGame3,//	보너스게임마스터 : 보너스게임 30회 성공
	
	kAchievementCode_mapGacha1,//	맵가챠맨1 : 맵가챠 10회 돌리기
	kAchievementCode_mapGacha2,//	맵가챠맨2 : 맵가챠 20회 돌리기
	kAchievementCode_mapGacha3,//	맵가챠맨3 : 맵가챠 30회 돌리기
	
	kAchievementCode_luckySeven1,//	럭키세븐1 : 한번에 7% 먹기
	kAchievementCode_luckySeven2,//	럭키세븐2 : 한번에 7.7% 먹기
	kAchievementCode_luckySeven3,//	럭키세븐3 : 한번에 77.7% 먹기
	
	kAchievementCode_feverMania1,//	피버매니아1: 한판에 피버모드 5회진입시
	kAchievementCode_feverMania2,//	피버매니아2: 전체 피버모드 30회진입시
	kAchievementCode_feverMania3,//	피버매니아3: 전체 피버모드 50회진입시
	
	kAchievementCode_comboMania1,//	콤보매니아1 : 10콤보달성
	kAchievementCode_comboMania2,//	콤보매니아2 : 20콤보달성
	kAchievementCode_comboMania3,//	콤보매니아3 : 30콤보달성
	
	kAchievementCode_noFail1,//	실패란없다1 : 5회연속 클리어
	kAchievementCode_noFail2,//	실패란없다2 : 10회연속클리어
	kAchievementCode_noFail3,//	실패란없다3 : 20회연속클리어
	
	kAchievementCode_attacker1,//	닥치고공격1 : 한판에 20회공격 성공
	kAchievementCode_attacker2,//	닥치고공격2 : 한판에 30회공격 성공
	kAchievementCode_attacker3,//	닥치고공격3 : 한판에 40회공격 성공
	
	kAchievementCode_hunter1,//	사냥꾼1 : 부하몹 5마리 가두어잡기
	kAchievementCode_hunter2,//	사냥꾼2 : 부하몹 10마리 가두어잡기
	kAchievementCode_hunter3,//	사냥꾼3 : 부하몹 20마리 가두어잡기
	
	kAchievementCode_dieHard1,//	다이하드1 : 1~3level 에서 한목숨도 안죽고 클리어
	kAchievementCode_dieHard2,//	다이하드2 : 4~9level 에서 한목숨도 안죽고 클리어
	kAchievementCode_dieHard3,//	다이하드3 : 10level 이상에서 한목숨도 안죽고 클리어
	
	kAchievementCode_scoreHigh1,//	경쟁의신1 : 50000점 이상 획득
	kAchievementCode_scoreHigh2,//	경쟁의신2 : 100000점 이상 획득
	kAchievementCode_scoreHigh3,//	경쟁의신3 : 150000점 이상 획득
	
	kAchievementCode_allPiece1,//	흑백퍼즐 : 모든피스가 흑백일때
	kAchievementCode_allPiece2,//	보더퍼즐 : 모든피스가 보더일때
	kAchievementCode_allPiece3,//	퍼즐완성 : 모든피스가 컬러일때
	
	kAchievementCode_fastClear1,//	초스피드클리어1 : 30초안에 클리어
	kAchievementCode_fastClear2,//	초스피드클리어2 : 10초안에 클리어
	kAchievementCode_fastClear3,//	초스피드클리어3 : 5초안에 클리어
	
	kAchievementCode_continue1,//	이어하기매니아 : 5회 이어하기
	kAchievementCode_continue2,//	이어하기매니아 : 10회 이어하기
	kAchievementCode_continue3,//	이어하기매니아 : 20회 이어하기
	
	kAchievementCode_attendance1,//	출석체크1 : 5회출첵
	kAchievementCode_attendance2,//	출석체크2 : 10회출첵
	kAchievementCode_attendance3,//	출석체크3 : 30회출첵
	
	kAchievementCode_changeMania1,//	체인지1 : 10회 체인지
	kAchievementCode_changeMania2,//	체인지2 : 20회 체인지
	kAchievementCode_changeMania3,//	체인지3 : 30회 체인지
	
	kAchievementCode_fail1,//	실패는 성공의 어머니1 : 게임실패 10회
	kAchievementCode_fail2,//	실패는 성공의 어머니2 : 게임실패 50회
	kAchievementCode_fail3,//	실패는 성공의 어머니3 : 게임실패 100회
	
	kAchievementCode_cardCollection1,//	카드컬렉터1: 카드 50장모으기
	kAchievementCode_cardCollection2,//	카드컬렉터2: 카드 100장모으기
	kAchievementCode_cardCollection3,//	카드컬렉터3: 카드 200장모으기
	
	kAchievementCode_friend1,//	마당발1 : 게임친구 10명이상
	kAchievementCode_friend2,//	마당발2 : 게임친구 30명이상
	kAchievementCode_friend3,//	마당발3 : 게임친구 50명달성
	
	kAchievementCode_invite1,//	초대1 : 카카오친구 10명이상초대
	kAchievementCode_invite2,//	초대2 : 카카오친구 20명이상초대
	kAchievementCode_invite3,//	초대3 : 카카오친구 30명이상초대
	
	kAchievementCode_challenger1,//	승부사1 : 도전하기 10번
	kAchievementCode_challenger2,//	승부사1 : 도전하기 20번
	kAchievementCode_challenger3,//	승부사1 : 도전하기 30번
	
	kAchievementCode_help1,//	도움요청1 : 도움요청 10번
	kAchievementCode_help2,//	도움요청2 : 도움요청 20번
	kAchievementCode_help3,//	도움요청3 : 도움요청 30번
	
	kAchievementCode_helper1,//	흑기사1 : 친구의 도움요청 들어주기 10회
	kAchievementCode_helper2,//	흑기사2 : 친구의 도움요청 들어주기 30회
	kAchievementCode_helper3,//	흑기사3 : 친구의 도움요청 들어주기 50회
	
	kAchievementCode_defender1,//	덤벼1 : 친구의 도전 수락 10회
	kAchievementCode_defender2,//	덤벼2 : 친구의 도전 수락 20회
	kAchievementCode_defender3,//	덤벼3 : 친구의 도전 수락 30회
	
	kAchievementCode_upgrade1,//	강화장인1 : 강화 10회성공
	kAchievementCode_upgrade2,//	강화장인2 : 강화 30회성공
	kAchievementCode_upgrade3,//	강화장인3 : 강화 50회성공
	
	kAchievementCode_perfect1,//	대단한성과1 : 100%클리어 10회
	kAchievementCode_perfect2,//	대단한성과2 : 100%클리어 30회
	kAchievementCode_perfect3,//	대단한성과3 : 100%클리어 50회
	
	kAchievementCode_seqAttendance1,//	개근1 : 7일 연속출첵
	kAchievementCode_seqAttendance2,//	개근2 : 14일 연속출첵
	kAchievementCode_seqAttendance3,//	개근3 : 21일 연속출첵
	
	kAchievementCode_gacha1,//	경마꾼 : 경마가챠 10회이상
	kAchievementCode_gacha2,//	야바위꾼 : 야바위가챠 10회이상
	kAchievementCode_gacha3,//	룰렛꾼 : 룰렛가챠 10회이상
	kAchievementCode_gacha4,//	슬롯버신꾼 : 슬롯머신 10회이상
	kAchievementCode_gachaKing,//	도박의신 : 경마&야바위&룰렛&슬롯머신 모두 달성시
	
	kAchievementCode_weeklyKing,//	주간킹 : 주간랭킹 1등 5회
	kAchievementCode_puzzleKing,//	퍼즐킹 : 한퍼즐내에 모든피스 땅따먹기 1등
	kAchievementCode_characterCollection,//	캐릭터컬렉터 : 캐릭터 5개이상 보유
	kAchievementCode_luckyGuy,//	럭키가이 : 시작행운아이템 5회성공
	kAchievementCode_goldMania,//	골드매니아 : 피버모드에서 한번에 100골드 이상획득.
	kAchievementCode_tutorial,//	초보자 : 튜토리얼다 보기
	kAchievementCode_cardSet,//	셋트완성 : 한스테이지의 1,2,3단계 카드셋트 완성
	
	kAchievementCode_reviewer,//	리뷰어 : 리뷰남겨주기
	
	kAchievementCode_hidden_returnee,//	(히든) 오랜만이야 : 일주일만에 접속.
	kAchievementCode_hidden_bigHand1,//	(히든) 욕심쟁이 : 한번에 먹은 영역이 50%이상일때
	kAchievementCode_hidden_bigHand2,//	(히든) 욕심쟁이2 : 한번에 먹은 영역이 100%이상일때

	kAchievementCode_hidden_holder,//	(히든) 잉여킹 : 대기실에 켜놓고 100초 이상 경과

	kAchievementCode_hidden_noSound,//	(히든) 사운드따윈필요없어 : 설정에 사운드 끄고 플레이후 클리어

	kAchievementCode_hidden_shopper1,//	(히든) 쇼핑홀릭 : 상점메뉴 100회 진입시.
	kAchievementCode_hidden_shopper2,//	(히든) 쇼핑홀릭2 : 아이템 50번째 구매
	kAchievementCode_hidden_shopper3,//	(히든) 쇼핑홀릭3 : 아이템 100번째 구매

	kAchievementCode_hidden_breathtaking1,//	(히든)아슬아슬 : 5초이하로 남기고 클리어
	kAchievementCode_hidden_breathtaking2,//	(히든)아슬아슬2 : 1초이하로 남기고 클리어

	kAchievementCode_hidden_dieEasy,//	(히든)다이이지 : 게임시작 5초안에 죽기

	kAchievementCode_hidden_speedMania//	(히든)스피드매니아 : 스피드 max찍기
};

enum AchieveRewardType
{
	kAchieveRewardType_gold = 0,
	kAchieveRewardType_ruby
};

class AchieveConditionRewardData
{
public:
	int getCondition()
	{
		return condition.getV();
	}
	AchieveRewardType getRewardType()
	{
		return (AchieveRewardType)reward_type.getV();
	}
	int getRewardValue()
	{
		return reward_value.getV();
	}
	
	AchieveConditionRewardData(int t_condition, int t_reward_type, int t_reward_value) : condition(t_condition), reward_type(t_reward_type), reward_value(t_reward_value)
	{}
	AchieveConditionRewardData() : condition(0), reward_type(0), reward_value(0)
	{}
	
private:
	KSProtectVar<int> condition;
	KSProtectVar<int> reward_type;
	KSProtectVar<int> reward_value;
};

class AchieveConditionReward
{
public:
	static AchieveConditionReward* sharedInstance()
	{
		static AchieveConditionReward* t_sg = NULL;
		if(t_sg == NULL)
		{
			t_sg = new AchieveConditionReward();
			t_sg->myInit();
		}
		return t_sg;
	}
	
	int getCondition(AchievementCode t_code)
	{
		return data_map[t_code].getCondition();
	}
	AchieveRewardType getRewardType(AchievementCode t_code)
	{
		return data_map[t_code].getRewardType();
	}
	int getRewardValue(AchievementCode t_code)
	{
		return data_map[t_code].getRewardValue();
	}
	
private:
	
	map<AchievementCode, AchieveConditionRewardData> data_map;
	
	void myInit()
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
		data_map[kAchievementCode_social1] = AchieveConditionRewardData(10, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_social2] = AchieveConditionRewardData(30, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_social3] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
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
		data_map[kAchievementCode_dieHard1] = AchieveConditionRewardData(0, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_dieHard2] = AchieveConditionRewardData(0, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_dieHard3] = AchieveConditionRewardData(0, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_scoreHigh1] = AchieveConditionRewardData(50000, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_scoreHigh2] = AchieveConditionRewardData(100000, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_scoreHigh3] = AchieveConditionRewardData(150000, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_allPiece1] = AchieveConditionRewardData(0, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_allPiece2] = AchieveConditionRewardData(0, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_allPiece3] = AchieveConditionRewardData(0, kAchieveRewardType_ruby, 1);
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
		data_map[kAchievementCode_puzzleKing] = AchieveConditionRewardData(0, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_characterCollection] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_luckyGuy] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_goldMania] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_tutorial] = AchieveConditionRewardData(0, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_cardSet] = AchieveConditionRewardData(0, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_reviewer] = AchieveConditionRewardData(0, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_hidden_returnee] = AchieveConditionRewardData(7, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_hidden_bigHand1] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_hidden_bigHand2] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_hidden_holder] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_hidden_noSound] = AchieveConditionRewardData(0, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_hidden_shopper1] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_hidden_shopper2] = AchieveConditionRewardData(50, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_hidden_shopper3] = AchieveConditionRewardData(100, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_hidden_breathtaking1] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_hidden_breathtaking2] = AchieveConditionRewardData(1, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_hidden_dieEasy] = AchieveConditionRewardData(5, kAchieveRewardType_ruby, 1);
		data_map[kAchievementCode_hidden_speedMania] = AchieveConditionRewardData(0, kAchieveRewardType_ruby, 1);
	}
};

class AchieveTitleContent
{
public:
	string title;
	string content;
	
	static AchieveTitleContent getAchievementScript(AchievementCode t_code);
};

#endif /* defined(__DGproto__AchieveData__) */
