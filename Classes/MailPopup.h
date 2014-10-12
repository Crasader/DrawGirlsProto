// MailPopup.h
//

#ifndef LZZ_MailPopup_h
#define LZZ_MailPopup_h
#include "cocos2d.h"

#include "cocos-ext.h"
using namespace cocos2d::extension;
USING_NS_CC;
using namespace std;

#include "jsoncpp/json.h"
#include <map>
#include "CCMenuLambda.h"
#include "DownloadFile.h"
#include "DimmedPopup.h"
#include "CommonButton.h"

#define LZZ_INLINE inline
enum MailPopupZorder
{
	kMP_Z_gray = 1,
	kMP_Z_back,
	kMP_Z_close,
	kMP_Z_content,
	kMP_Z_mailTable,
	kMP_Z_profileImg,
	kMP_Z_helpAccept,
	kMP_Z_popup
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
// qwwqeqwe
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

enum class PostBoxState
{
	kNoMenu = 0,
	kMenu = 1
};
class ScrollBar;
enum class MailFilter
{
	kTotal,
	kHeart,
	kChallenge,
	kHelp,
	kTicket,
	kInvite,
	kNews,
	kUnknownFriendRequest,
	kGift
};
class MailPopup : public DimmedPopup, public CCTableViewDataSource, public CCTableViewDelegate
{
	public:
		static MailPopup * create (CCObject * t_close, SEL_CallFunc d_close, std::function<void(void)> heartRefresh);
	//	void finishedOpen ();
	//	void finishedClose ();
		void myInit (CCObject * t_close, SEL_CallFunc d_close, std::function<void(void)> heartRefresh);
		void loadMail ();
		void drawMail (Json::Value obj);
	//	void closePopup (CCControlButton * obj, CCControlEvent event);
		void removeMail (CCObject * _obj);
		virtual CCTableViewCell * tableCellAtIndex (CCTableView * table, unsigned int idx);
		virtual void scrollViewDidScroll (CCScrollView * view);
		virtual void scrollViewDidZoom (CCScrollView * view);
		virtual void tableCellTouched (CCTableView * table, CCTableViewCell * cell);
		virtual CCSize cellSizeForTable (CCTableView * table);
		virtual unsigned int numberOfCellsInTableView (CCTableView * table);
		void touchFriend (CCObject * a, CCControlEvent b);
		//CCPoint getContentPosition (int t_tag);
		virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
		virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
		virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
		virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
		virtual void registerWithTouchDispatcher ();

		void onReceiveStageSuccess();
		void onReceiveStageFail();
		void iHelpYou(int stage, long long user_id, const std::string& nick, Json::Value removeInfo);
		void removeMessage(int mailNo, long long memberID, std::function<void(Json::Value)> f);
		void removeMessageByList(vector<int> mailNo, long long memberID, std::function<void(Json::Value)> f);
		void filterWithMailFilter();
	protected:
		std::function<void(void)> m_heartRefresh;
		Json::Value m_mailList; // 이 정보를 기반으로 뿌려줌. 
		Json::Value m_filteredMailList; // 걸러진 데이터들
		CCTableView * mailTableView;
		ScrollBar * m_scrollBar;
		MailFilter m_mailFilter;
		PostBoxState m_popupState;

		int download_card_number;
		CCNode* loading_card_img;
		CCNode* addCardImg(int t_card_number, int t_card_level, string t_passive);
		void resultLoadedCardInfo (Json::Value result_data);
		vector <DownloadFile> df_list;
//		vector <CopyFile> cf_list;

		int ing_download_cnt;
		void successAction ();
		void failAction();
		void startDownloadCardImage ();
		void startDownload ();
	
	bool isLoaded;
};
#undef LZZ_INLINE
#endif
