//
//  SilhouetteData.cpp
//  galsprototype
//
//  Created by 사원3 on 13. 8. 27..
//
//

#include "SilhouetteData.h"
#include "StageImgLoader.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "MyLocalization.h"

void SilhouetteData::startSetting()
{
	must_cnt = 0;
	CCImage *tx = new CCImage;
	tx->initWithEncryptedImageFileFullPath(CCString::createWithFormat((mySIL->getDocumentPath()+"card%d_invisible.png").c_str(), NSDS_GI(myType, kSDS_SI_level_int1_card_i, 1))->getCString());
	
	unsigned char* pData = (unsigned char*)tx->getData();    //원본 이미지 데이터
	int imgByte = 3;
	int dataRate = 4;
	int dataWidth = tx->getWidth()/dataRate;
	int dataHeight = tx->getHeight()/dataRate;
	
	if(tx->hasAlpha())imgByte=4;
	
	
	for(int y=0;y<dataHeight;y++){
		//string xprint = "";
		for(int x=0;x<dataWidth;x++){
			int x_offset=2;
			int y_offset=2;
			if(x==dataWidth-1)x_offset=1;
			if(y==dataHeight-1)y_offset=1;
			int i = ((y*dataRate+y_offset)*tx->getWidth()+(x*dataRate+x_offset))*imgByte;
			
			//순서대로 r,g,b 값을 더하여 10 이상일때 실루엣으로 인식
			if(pData[i+2] > 200)//  pData[i]+pData[i+1]+pData[i+2]>=10)
			{
				must_cnt++;
				silData[x+1][215-y]=true;
				//xprint+="1";
			}else{
				silData[x+1][215-y]=false;
				//xprint+="0";
			}
		}
		//CCLOG("%d : %s",y,xprint.c_str());
	}
	
	tx->release();
	
//	for(int y=216;y>=0;y--){
//		string xprint="";
//		for(int x=0;x<162;x++){
//			if(silData[x][y]==false){
//				xprint+="0";
//			}else{
//				xprint+="1";
//			}
//		}
//		
//		CCLOG("%s",xprint.c_str());
//	}
	
//	string t_sil = NSDS_GS(kSDS_CI_int1_silImgInfoSilData_s, NSDS_GI(myType, kSDS_SI_level_int1_card_i, 1));
//	
//	must_cnt = 0;
//	
//	int i = mapHeightInnerEnd-1;
//	int j = mapWidthInnerBegin;
//	bool is_zero = true;
//	
//	int check_number = 0;
//	while(i >= mapHeightInnerBegin && j < mapWidthInnerEnd)
//	{
//		string cnt_str = t_sil.substr(check_number, 1);
//		check_number++;
//		if(cnt_str[0] >= 'A')
//		{
//			int adder = cnt_str[0]-'A'+2;
//			cnt_str = t_sil.substr(check_number, adder);
//			check_number += adder;
//		}
//		
//		int cnt = atoi(cnt_str.c_str());
//		if(is_zero)
//		{
//			must_cnt += cnt;
//			for(int k=0;k<cnt;k++)
//			{
//				silData[j++][i] = true;
//				if(j >= mapWidthInnerEnd)
//				{
//					j = mapWidthInnerBegin;
//					i--;
//					if(i < mapHeightInnerBegin)
//						break;
//				}
//			}
//			is_zero = false;
//		}
//		else
//		{
//			for(int k=0;k<cnt;k++)
//			{
//				silData[j++][i] = false;
//				if(j >= mapWidthInnerEnd)
//				{
//					j = mapWidthInnerBegin;
//					i--;
//					if(i < mapHeightInnerBegin)
//						break;
//				}
//			}
//			is_zero = true;
//		}
//	}
}

////////////////////////////////////////
void SilhouetteData::exchangeSilhouette()
{
	must_cnt = 0;
	CCImage *tx = new CCImage;
	tx->initWithEncryptedImageFileFullPath(CCString::createWithFormat((mySIL->getDocumentPath()+"card%d_invisible.png").c_str(), NSDS_GI(myType, kSDS_SI_level_int1_card_i, 3))->getCString());
	
	unsigned char* pData = (unsigned char*)tx->getData();    //원본 이미지 데이터
	int imgByte = 3;
	int dataRate = 4;
	int dataWidth = tx->getWidth()/dataRate;
	int dataHeight = tx->getHeight()/dataRate;
	
	if(tx->hasAlpha())imgByte=4;
	
//	for(int i=mapWidthOutlineBegin;i<mapWidthOutlineEnd;i++)
//	{
//		for(int j=mapHeightOutlineBegin;j<mapHeightOutlineEnd;j++)
//		{
//			silData[i][j] = false;
//		}
//	}
	
	for(int y=0;y<dataHeight;y++){
//		string xprint = "";
		for(int x=0;x<dataWidth;x++){
			int x_offset=2;
			int y_offset=2;
			if(x==dataWidth-1)x_offset=1;
			if(y==dataHeight-1)y_offset=1;
			int i = ((y*dataRate+y_offset)*tx->getWidth()+(x*dataRate+x_offset))*imgByte;
			
			//순서대로 r,g,b 값을 더하여 10 이상일때 실루엣으로 인식
			if(pData[i+2] > 200)//pData[i]+pData[i+1]+pData[i+2]>=10)
			{
				must_cnt++;
				silData[x+1][215-y]=true;
//				xprint+="1";
			}else{
				silData[x+1][215-y]=false;
//				xprint+="0";
			}
		}
//		CCLOG("%d : %s",y,xprint.c_str());
	}
	
	tx->release();
	
	
	
//	string t_sil = NSDS_GS(kSDS_CI_int1_silImgInfoSilData_s, NSDS_GI(myType, kSDS_SI_level_int1_card_i, 2));
//	
//	must_cnt = 0;
//	
//	int i = mapHeightInnerEnd-1;
//	int j = mapWidthInnerBegin;
//	bool is_zero = true;
//	
//	int check_number = 0;
//	while(i >= mapHeightInnerBegin && j < mapWidthInnerEnd)
//	{
//		string cnt_str = t_sil.substr(check_number, 1);
//		check_number++;
//		if(cnt_str[0] >= 'A')
//		{
//			int adder = cnt_str[0]-'A'+2;
//			cnt_str = t_sil.substr(check_number, adder);
//			check_number += adder;
//		}
//		
//		int cnt = atoi(cnt_str.c_str());
//		if(is_zero)
//		{
//			must_cnt += cnt;
//			for(int k=0;k<cnt;k++)
//			{
//				silData[j++][i] = true;
//				if(j >= mapWidthInnerEnd)
//				{
//					j = mapWidthInnerBegin;
//					i--;
//					if(i < mapHeightInnerBegin)
//						break;
//				}
//			}
//			is_zero = false;
//		}
//		else
//		{
//			for(int k=0;k<cnt;k++)
//			{
//				silData[j++][i] = false;
//				if(j >= mapWidthInnerEnd)
//				{
//					j = mapWidthInnerBegin;
//					i--;
//					if(i < mapHeightInnerBegin)
//						break;
//				}
//			}
//			is_zero = true;
//		}
//	}
}

//string SilhouetteData::getScriptString(int level)
//{
//	return getScriptString(myType, level);
//}
//string SilhouetteData::getScriptString(int t_type, int level)
//{
//	return NSDS_GS(kSDS_CI_int1_script_s, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, level));
//}

SilhouetteData* SilhouetteData::sharedSilhouetteData()
{
	static SilhouetteData* t_SD = NULL;
	if(t_SD == NULL)
	{
		t_SD = new SilhouetteData();
		t_SD->myInit();
	}
	return t_SD;
}

void SilhouetteData::setSilType( int t1 )
{
	myType = t1;

//	if(isAnimationStage())
//	{
//		setAnimationLoop(myType);
//	}
}

std::string SilhouetteData::getPassiveData()
{
	return "{}";
}

//void SilhouetteData::setAnimationLoop( int t_type )
//{
//	animation_frame.clear();
//	int loop_length = NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopLength_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));
//	for(int i=0;i<loop_length;i++)
//		animation_frame.push_back(NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3), i));
//}

int SilhouetteData::getSilType()
{
	return myType;
}

//int SilhouetteData::getCardDurability( int stage, int level )
//{
//	return NSDS_GI(kSDS_CI_int1_durability_i, NSDS_GI(stage, kSDS_SI_level_int1_card_i, level));
//}

//std::string SilhouetteData::getCardOptionScript( int stage, int level )
//{
//	string return_value;
//
//	return_value = "ㅁ니우라ㅓㅁ";
//
//	return return_value;
//}

//void SilhouetteData::setCardOptions( deque<int>& t_list, int card_number )
//{
//	int ability_cnt = NSDS_GI(kSDS_CI_int1_abilityCnt_i, card_number);
//
//	for(int i=0;i<ability_cnt;i++)
//		t_list.push_back(NSDS_GI(kSDS_CI_int1_ability_int2_type_i, card_number, i));
//}

CLEAR_CONDITION SilhouetteData::getClearCondition()
{
	return getClearCondition(myType);
}

CLEAR_CONDITION SilhouetteData::getClearCondition( int t_type ) /* stage */
{
	return CLEAR_CONDITION(NSDS_GI(t_type, kSDS_SI_missionType_i));
}

float SilhouetteData::getBossMaxLife()
{
	Json::Value t_json;
	Json::Reader reader;
	reader.parse(NSDS_GS(myType, kSDS_SI_boss_s), t_json);
	Json::Value t_array = t_json;
	Json::Value t_boss = t_array[0];
	return float(t_boss["hp"].asInt());
}

std::string SilhouetteData::getConditionTitle()
{
	string return_value;

	CLEAR_CONDITION my_clear_condition = getClearCondition();

	if(my_clear_condition == kCLEAR_bossLifeZero)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionTitle1);
	else if(my_clear_condition == kCLEAR_subCumberCatch)	return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionTitle2);
	else if(my_clear_condition == kCLEAR_bigArea)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionTitle3);
	else if(my_clear_condition == kCLEAR_itemCollect)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionTitle4);
	else if(my_clear_condition == kCLEAR_perfect)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionTitle5);
	else if(my_clear_condition == kCLEAR_sequenceChange)	return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionTitle6);
	else if(my_clear_condition == kCLEAR_timeLimit)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionTitle7);
	else if(my_clear_condition == kCLEAR_hellMode)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionTitle8);
	else if(my_clear_condition == kCLEAR_percentage)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionTitle9);
	else if(my_clear_condition == kCLEAR_score)				return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionTitle10);
	else if(my_clear_condition == kCLEAR_combo)				return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionTitle11);
	else if(my_clear_condition == kCLEAR_gold)				return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionTitle12);
	else if(my_clear_condition == kCLEAR_turns)				return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionTitle13);
	else if(my_clear_condition == kCLEAR_casting)			return_value = getLocal(LK::kMyLocalKey_missionTitle14);
	else if(my_clear_condition == kCLEAR_littlePercent)		return_value = getLocal(LK::kMyLocalKey_missionTitle15);
	else													return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionTitle0);

	return return_value;
}

std::string SilhouetteData::getConditionContent()
{
	return getConditionContent(myType);
}

std::string SilhouetteData::getConditionContent( int t_type )
{
	string return_value;

	t_type = getClearCondition(t_type);

	if(t_type == kCLEAR_bossLifeZero)				return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription1);
	else if(t_type == kCLEAR_subCumberCatch)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription2);
	else if(t_type == kCLEAR_bigArea)				return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription3);
	else if(t_type == kCLEAR_itemCollect)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription4);
	else if(t_type == kCLEAR_perfect)				return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription5);
	else if(t_type == kCLEAR_sequenceChange)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription6);
	else if(t_type == kCLEAR_timeLimit)				return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription7);
	else if(t_type == kCLEAR_hellMode)				return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription8);
	else if(t_type == kCLEAR_percentage)			return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription9), NSDS_GI(getSilType(), kSDS_SI_missionOptionPercent_i));
	else if(t_type == kCLEAR_score)					return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription10);
	else if(t_type == kCLEAR_combo)					return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription11);
	else if(t_type == kCLEAR_gold)					return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription12);
	else if(t_type == kCLEAR_turns)					return_value = myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription13);
	else if(t_type == kCLEAR_casting)				return_value = getLocal(LK::kMyLocalKey_missionDiscription14);
	else if(t_type == kCLEAR_littlePercent)			return_value = getLocal(LK::kMyLocalKey_missionDiscription15);
	else
	{
		if(mySGD->isTimeEvent(kTimeEventType_clear))
		{
			return_value = CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription0), mySGD->getTimeEventFloatValue(kTimeEventType_clear))->getCString();
		}
		else
			return_value = CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_missionDiscription0), 85.f)->getCString();
	}

	return return_value;
}

int SilhouetteData::getClearConditionCatchSubCumber()
{
	return getClearConditionCatchSubCumber(myType);
}

int SilhouetteData::getClearConditionCatchSubCumber( int t_type )
{
	return NSDS_GI(t_type, kSDS_SI_missionOptionCount_i);
}

float SilhouetteData::getClearConditionBigAreaPer()
{
	return getClearConditionBigAreaPer(myType);
}

float SilhouetteData::getClearConditionBigAreaPer( int t_type )
{
	return NSDS_GI(t_type, kSDS_SI_missionOptionPercent_i)/100.f;
}

int SilhouetteData::getClearConditionBigAreaCnt()
{
	return getClearConditionBigAreaCnt(myType);
}

int SilhouetteData::getClearConditionBigAreaCnt( int t_type )
{
	return NSDS_GI(t_type, kSDS_SI_missionOptionCount_i);
}

int SilhouetteData::getClearConditionItemCollect()
{
	return getClearConditionItemCollect(myType);
}

int SilhouetteData::getClearConditionItemCollect( int t_type )
{
	return NSDS_GI(t_type, kSDS_SI_missionOptionCount_i);
}

float SilhouetteData::getClearConditionPerfectBase()
{
	return getClearConditionPerfectBase(myType);
}

float SilhouetteData::getClearConditionPerfectBase( int t_type )
{
	return NSDS_GI(t_type, kSDS_SI_missionOptionPercent_i)/100.f;
}

float SilhouetteData::getClearConditionPerfectRange()
{
	return getClearConditionPerfectRange(myType);
}

float SilhouetteData::getClearConditionPerfectRange( int t_type )
{
	return 0.01f;
}

int SilhouetteData::getClearConditionTimeLimit()
{
	return getClearConditionTimeLimit(myType);
}

int SilhouetteData::getClearConditionTimeLimit( int t_type )
{
	return NSDS_GI(t_type, kSDS_SI_missionOptionSec_i);
}

vector<ITEM_CODE> SilhouetteData::getStageItemList()
{
	return getStageItemList(myType);
}

vector<ITEM_CODE> SilhouetteData::getStageItemList( int t_type )
{
	vector<ITEM_CODE> return_value;

	int item_cnt = NSDS_GI(t_type, kSDS_SI_shopItemsCnt_i);

	for(int i=0;i<item_cnt;i++)
	{
		ITEM_CODE t_code = ITEM_CODE(NSDS_GI(t_type, kSDS_SI_shopItems_int1_type_i, i));
		if(t_code == kIC_baseSpeedUp || t_code == kIC_doubleItem || t_code == kIC_magnet)
			return_value.push_back(ITEM_CODE(NSDS_GI(t_type, kSDS_SI_shopItems_int1_type_i, i)));
	}

	return return_value;
}

std::string SilhouetteData::getItemScript( ITEM_CODE t_code )
{
	string return_value;

	if(t_code == kIC_attack)				return_value = "획득시 보스에게 미사일 공격을 가한다.";
	else if(t_code == kIC_speedUp)			return_value = "획득시 유저의 속도가 올라간다.";
	else if(t_code == kIC_addTime)			return_value = "획득시 남은 시간이 일정량 증가한다.";
	else if(t_code == kIC_fast)				return_value = myLoc->getLocalForKey(LK::kMyLocalKey_item4ment);
	else if(t_code == kIC_subOneDie)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_item5ment);
	else if(t_code == kIC_doubleItem)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_doubleItemContent);
	else if(t_code == kIC_silence)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_item7ment);
	else if(t_code == kIC_longTime)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_item8ment);
	else if(t_code == kIC_baseSpeedUp)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_baseSpeedUpContent);
	else if(t_code == kIC_magnet)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_magnetContent);
	else if(t_code == kIC_itemGacha)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_itemGachaContent);
	else if(t_code == kIC_heartUp)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_item10ment);
	else									return_value = "아이템을 사용할 수 있습니다.";

	return return_value;
}

string SilhouetteData::getItemPrice( ITEM_CODE t_code )
{
	string return_value;

	return NSDS_GS(kSDS_GI_shopItem_int1_priceName_s, t_code);
}

string SilhouetteData::getItemCurrency(ITEM_CODE t_code)
{
	string return_value;
	
	int shop_item_cnt = NSDS_GI(myType, kSDS_SI_shopItemsCnt_i);
	bool is_found = false;
	for(int i=0;i<shop_item_cnt && !is_found;i++)
	{
		if(t_code == NSDS_GI(myType, kSDS_SI_shopItems_int1_type_i, i))
		{
			is_found = true;
			return_value = NSDS_GS(myType, kSDS_SI_shopItems_int1_currency_s, i);
		}
	}
	
	return return_value;
}

int SilhouetteData::getSilenceItemOption()
{
	return NSDS_GI(myType, kSDS_SI_itemOptionSilenceSec_i);
}

int SilhouetteData::getDoubleItemOption()
{
	return NSDS_GI(myType, kSDS_SI_itemOptionDoubleItemPercent_i);
}

//int SilhouetteData::getCardDoubleItemOption( int card_number )
//{
//	return NSDS_GI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, card_number);
//}

int SilhouetteData::getLongTimeItemOption()
{
	return NSDS_GI(myType, kSDS_SI_itemOptionLongTimeSec_i);
}

//int SilhouetteData::getCardLongTimeItemOption( int card_number )
//{
//	return NSDS_GI(kSDS_CI_int1_abilityLongTimeOptionSec_i, card_number);
//}

int SilhouetteData::getBaseSpeedUpItemOption()
{
	return NSDS_GI(myType, kSDS_SI_itemOptionBaseSpeedUpUnit_i);
}

//int SilhouetteData::getCardBossLittleEnergyItemOption( int card_number )
//{
//	return NSDS_GI(kSDS_CI_int1_abilityBossLittleEnergyOptionPercent_i, card_number);
//}

//int SilhouetteData::getSubSmallSizeItemOption()
//{
//	return NSDS_GI(myType, kSDS_SI_itemOptionSubSmallSizePercent_i);
//}

//int SilhouetteData::getCardSubSmallSizeItemOption( int card_number )
//{
//	return NSDS_GI(kSDS_CI_int1_abilitySubSmallSizeOptionPercent_i, card_number);
//}

//int SilhouetteData::getSmallAreaItemOption()
//{
//	return NSDS_GI(myType, kSDS_SI_itemOptionSmallAreaPercent_i);
//}

//int SilhouetteData::getCardSmallAreaItemOption( int card_number )
//{
//	return NSDS_GI(kSDS_CI_int1_abilitySmallAreaOptionPercent_i, card_number);
//}

//int SilhouetteData::getWidePerfectItemOption()
//{
//	return NSDS_GI(myType, kSDS_SI_itemOptionWidePerfectPercent_i);
//}

//int SilhouetteData::getCardWidePerfectItemOption( int card_number )
//{
//	return NSDS_GI(kSDS_CI_int1_abilityWidePerfectOptionPercent_i, card_number);
//}

//bool SilhouetteData::isAnimationStage()
//{
//	return isAnimationStage(myType);
//}

//bool SilhouetteData::isAnimationStage( int t_type )
//{
//	return NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));
//}

//CCSize SilhouetteData::getAnimationCutSize()
//{
//	return getAnimationCutSize(myType);
//}

//CCSize SilhouetteData::getAnimationCutSize( int t_type )
//{
//	CCSize return_value;
//
//	return_value.width = NSDS_GI(kSDS_CI_int1_aniInfoDetailCutWidth_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));
//	return_value.height = NSDS_GI(kSDS_CI_int1_aniInfoDetailCutHeight_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));
//
//	return return_value;
//}

//int SilhouetteData::getAnimationCutLength()
//{
//	return getAnimationCutLength(myType);
//}

//int SilhouetteData::getAnimationCutLength( int t_type )
//{
//	return NSDS_GI(kSDS_CI_int1_aniInfoDetailCutLength_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));
//}

//CCPoint SilhouetteData::getAnimationPosition()
//{
//	return getAnimationPosition(myType);
//}

//CCPoint SilhouetteData::getAnimationPosition( int t_type )
//{
//	CCPoint return_value;
//
//	return_value.x = NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionX_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));
//	return_value.y = NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionY_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));
//
//	return return_value;
//}

//int SilhouetteData::getAnimationLoopLength()
//{
//	return getAnimationLoopLength(myType);
//}

//int SilhouetteData::getAnimationLoopLength( int t_type )
//{
//	return NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopLength_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));
//}

//int SilhouetteData::getAnimationLoopPoint( int t_frame )
//{
//	return animation_frame[t_frame];
//}
