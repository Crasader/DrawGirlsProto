// RankPopup.h
//
#ifndef LZZ_RankPopup_h
#define LZZ_RankPopup_h
#include "cocos2d.h"
#include "hspConnector.h"
#include "cocos-ext.h"
#include "KSUtil.h"
#include "CCMenuLambda.h"
#include "GDWebSprite.h"
#include "DataStorageHub.h"
#include <chrono>
#include "ScrollBar.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"
#include "SilhouetteData.h"
#include "CardCase.h"
#include "DownloadFile.h"
#include "StarGoldData.h"
#include "DimmedPopup.h"
#include "CommonButton.h"
USING_NS_CC;

#define SAFE_REMOVECHILD(X) do{if(X){ X->removeFromParentAndCleanup(true); X = 0;}}while(0);
#define LZZ_INLINE inline
enum RankPopupZorder
{
  kRP_Z_gray = 1,
  kRP_Z_back,
  kRP_Z_close,
  kRP_Z_content,
  kRP_Z_rankTable,
  kRP_Z_send,
  kRP_Z_profileImg,
  kRP_Z_usedCardImg
};
enum RankTableViewTag
{
  kRP_RT_title = 0x1000,
  kRP_RT_score,
  kRP_RT_rank,
  kRP_RT_send,
  kRP_RT_menu,
  kRP_RT_profileImg
};
enum RankPopupMenuTag
{
  kRP_MT_close = 0x2000,
  kRP_MT_invite,
  kRP_MT_send,
  kRP_MT_send_close,
  kRP_MT_invite_close,
  kRP_MT_invite_rank,
  kRP_MT_invite_send,
  kRP_MT_invite_send_close
};
enum RankPopupState
{
  kRP_State_rank = 1,
  kRP_State_send,
  kRP_State_invite,
  kRP_State_invite_send
};
class KSEaseBackOut : public CCActionEase
{
public:
  virtual void update (float time);
  virtual CCActionInterval * reverse ();
  virtual CCObject * copyWithZone (CCZone * pZone);
public:
  static CCEaseBackOut * create (CCActionInterval * pAction);
};
class RankTableView : public CCTableView
{
public:
  static RankTableView * create (CCTableViewDataSource * dataSource, CCSize size, CCNode * container);
  void setContentOffsetInDuration (CCPoint offset, float dt);
};


enum RankCategory
{
	kTotalFriend,
	kUnknownFriend,
	kKnownFriend,
	kRealTotalFriend
};

class RankPopup : public DimmedPopup, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
  static RankPopup * create (CCObject * t_close, SEL_CallFunc d_close);
  virtual ~ RankPopup ();
  //void finishedOpen ();
  //void finishedClose ();
  void myInit (CCObject * t_close, SEL_CallFunc d_close);
  void loadRank ();
  void drawRank (Json::Value obj);
	void drawTotalRank (Json::Value obj);
  //void closePopup (CCControlButton * obj, CCControlEvent event);
		
  virtual CCTableViewCell * tableCellAtIndex (CCTableView * table, unsigned int idx);

  virtual void scrollViewDidScroll (CCScrollView * view);
  virtual void scrollViewDidZoom (CCScrollView * view);
  CCSprite * used_card_img;
  int loading_card_number;
  int after_loading_card_number;
  int last_selected_card_number;
  vector <DownloadFile> df_list;
//  vector <CopyFile> cf_list;
  void resultLoadedCardInfo (Json::Value result_data);
  void successAction ();
  void failAction ();
  int ing_download_cnt;
  void startDownloadCardImage ();
  void startDownload ();
  void addCardImg (int t_card_number, int t_card_level, string t_passive);
  virtual void tableCellTouched (CCTableView * table, CCTableViewCell * cell);
  virtual CCSize cellSizeForTable (CCTableView * table);
  virtual unsigned int numberOfCellsInTableView (CCTableView * table);
  void touchFriend (CCObject * a, CCControlEvent b);
  CCPoint getContentPosition (int t_tag);
  void menuAction (CCObject * sender);
  virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
  virtual void registerWithTouchDispatcher ();
	void touchCellIndex(int idx);
		
	
protected:
  Json::Value m_scoreList;
  RankTableView * rankTableView;
  bool is_menu_enable;
  int touched_number;
  RankPopupState my_state;
  CCControlButton * closeBtn;
  CCSprite * m_currentSelectSprite;
  ScrollBar * m_scrollBar;
	CCLabelBMFont* m_highScore;
	CCLabelBMFont* m_highStage;
	
	
	CommonButton* m_onlyKatok;
	CommonButton* m_totalFriend;
	CommonButton* m_onlyGameFriend;
	
	RankCategory m_rankCategory;
};
#undef LZZ_INLINE
#endif
