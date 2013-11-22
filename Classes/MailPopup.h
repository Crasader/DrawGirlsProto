// MailPopup.h
//

#ifndef LZZ_MailPopup_h
#define LZZ_MailPopup_h
#include "cocos2d.h"

#include "cocos-ext.h"
using namespace cocos2d::extension;
USING_NS_CC;

#include "jsoncpp/json.h"
#include <map>
#include "CCMenuLambda.h"
#define LZZ_INLINE inline
enum MailPopupZorder
{
  kMP_Z_gray = 1,
  kMP_Z_back,
  kMP_Z_close,
  kMP_Z_content,
  kMP_Z_mailTable,
  kMP_Z_profileImg,
	kMP_Z_helpAccept
};
enum MailTableViewTag
{
  kMP_MT_title = 1,
  kMP_MT_score,
  kMP_MT_rank,
  kMP_MT_send,
  kMP_MT_getheart,
  kMP_MT_profileImg
};
enum MailPopupMenuTag
{
  kMP_MT_close = 1,
  kMP_MT_invite,
  kMP_MT_send_close,
  kMP_MT_invite_close,
  kMP_MT_invite_rank,
  kMP_MT_invite_send,
  kMP_MT_invite_send_close
};
enum class AlignText
{
	kTotal = 1,
	kNews,
	kHeart,
	kChallenge,
	kHelp,
	kTicket,
	kClose,
	kNone
};

enum class PostBoxState
{
	kNoMenu = 0,
	kMenu = 1
};
class ScrollBar;
class MailPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
  static MailPopup * create (CCObject * t_close, SEL_CallFunc d_close);
  void finishedOpen ();
  void finishedClose ();
  void myInit (CCObject * t_close, SEL_CallFunc d_close);
  void loadMail ();
  void drawMail (Json::Value obj);
  void closePopup (CCControlButton * obj, CCControlEvent event);
  void removeMail (CCObject * _obj);
  virtual CCTableViewCell * tableCellAtIndex (CCTableView * table, unsigned int idx);
  virtual void scrollViewDidScroll (CCScrollView * view);
  virtual void scrollViewDidZoom (CCScrollView * view);
  virtual void tableCellTouched (CCTableView * table, CCTableViewCell * cell);
  virtual CCSize cellSizeForTable (CCTableView * table);
  virtual unsigned int numberOfCellsInTableView (CCTableView * table);
  void touchFriend (CCObject * a, CCControlEvent b);
  CCPoint getContentPosition (int t_tag);
  virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
  virtual void registerWithTouchDispatcher ();
	
	void setAlignText(AlignText at);
	void showLeftMenuToggle(bool show);
	void iHelpYou(int stage, long long user_id, const std::string& nick, Json::Value removeInfo);
protected:
  Json::Value m_mailList;
  CCTableView * mailTableView;
  CCObject * target_close;
  SEL_CallFunc delegate_close;
  ScrollBar * m_scrollBar;
	
	std::map<AlignText, CCSprite*> m_alignTexts;
	std::vector<CCMenuItemImageLambda*> m_menuList;
	PostBoxState m_popupState;
};
#undef LZZ_INLINE
#endif
