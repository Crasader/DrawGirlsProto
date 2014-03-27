// LogData.h
//

#ifndef LZZ_LogData_h
#define LZZ_LogData_h
#include "cocos2d.h"
#include "SaveData.h"
#include "hspConnector.h"

USING_NS_CC;
using namespace std;

#define myLog LogData::sharedInstance()
#define LZZ_INLINE inline
enum LOG_KEY
{
  kLOG_count = 0,
  kLOG_data,
  kLOG_getPercent_f,
  kLOG_die_missileToLine,
  kLOG_die_shockwave,
  kLOG_die_other,
  kLOG_show_fever,
  kLOG_endCombo_i,
  kLOG_attackPattern_i,
  kLOG_getCoin_i,
  kLOG_getItem_s,
  kLOG_buyItem_s,
  kLOG_useItem_s,
  kLOG_remainHeart_i,
  kLOG_gacha_startMap,
  kLOG_gacha_onePercent,
  kLOG_puzzleAchievementMinimum_i,
  kLOG_puzzleAchievementMaximum_i,
  kLOG_typing_cardComment,
  kLOG_action_continue,
  kLOG_network_getUserData,
  kLOG_network_loadCardData,
  kLOG_network_downloadCardImg,
  kLOG_network_getPuzzleEventList,
  kLOG_network_downloadPuzzleEventImg,
  kLOG_network_setUserData,
  kLOG_getStageInfo_i,
  kLog_getPuzzleInfo_i
};
class LogData : public CCObject
{
public:
  static LogData * sharedInstance ();
  int getLogCount ();
  void addLog (LOG_KEY t_key, int t_time);
  void addLog (LOG_KEY t_key, int t_time, float t_percent);
  void addLog (LOG_KEY t_key, int t_time, int t_i1);
  void addLog (LOG_KEY t_key, int t_time, string t_str1);
  void sendLog (string t_category);
	CommandParam getSendLogCommand(string t_category);
private:
  SaveData * myDefault;
  string getLog (LOG_KEY t_key, int t_i);
  string getRKey (LOG_KEY t_key);
  void myInit ();
};
#undef LZZ_INLINE
#endif
