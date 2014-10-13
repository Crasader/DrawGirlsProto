// MailPopup.h
//
#pragma once
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
#include "TouchSuctionLayer.h"

#define LZZ_INLINE inline
enum SumranMailPopupZorder
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
enum SumranMailTableViewTag
{
	kMP_MT_title = 1,
	kMP_MT_score,
	kMP_MT_rank,
	kMP_MT_send,
	kMP_MT_getheart,
	kMP_MT_profileImg
};
// qwwqeqwe
enum SumranMailPopupMenuTag
{
	kMP_MT_close = 1,
	kMP_MT_invite,
	kMP_MT_send_close,
	kMP_MT_invite_close,
	kMP_MT_invite_rank,
	kMP_MT_invite_send,
	kMP_MT_invite_send_close
};

enum class SumranPostBoxState
{
	kNoMenu = 0,
	kMenu = 1
};
class ScrollBar;
enum class SumranMailFilter
{
	kTotal,
	kHeart,
	kChallenge,
	kHelp,
	kTicket,
	kInvite,
	kNews,
	kUnknownFriendRequest
};
class DownloadFile;
//class CopyFile;
class SumranMailPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
	public:
		static SumranMailPopup * create (CCObject * t_close, SEL_CallFunc d_close, std::function<void(void)> heartRefresh);
	//	void finishedOpen ();
	//	void finishedClose ();
		void myInit (CCObject * t_close, SEL_CallFunc d_close, std::function<void(void)> heartRefresh);
		void loadMail ();
		void drawMail (Json::Value obj);
	KSProtectVar<int> download_card_no;
	std::function<void(bool)> end_func_download_card;
	vector<DownloadFile> card_download_list;
//	vector<CopyFile> card_reduction_list;
	int ing_card_download;
	void startCardDownload();
	void successCardDownload();
	void failCardDownload();
	void resultGetCardInfo(Json::Value result_data);
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
		void rewardDown(Json::Value reward, std::function<void(bool)> func);
		void takedCheck(Json::Value reward, std::function<void(void)> func);
	//	void cardDown(int cardNo,std::function<void(bool)>finishFunc);
		void takedCard(int cardNo,std::function<void(void)>finishFunc);
	
	void confirmMessage(int type,Json::Value mail);
	Json::Value getButton(Json::Value mail);
	
	void takeAllReward(CCObject* sender);
	protected:
		int m_touchPriority;
		std::function<void(void)> m_heartRefresh;
		Json::Value m_mailList; // 이 정보를 기반으로 뿌려줌. 
		Json::Value m_filteredMailList; // 걸러진 데이터들
		CCTableView * mailTableView;
		ScrollBar * m_scrollBar;
		SumranMailFilter m_mailFilter;
		SumranPostBoxState m_popupState;
		CCSprite* main_case;
		TouchSuctionLayer* t_suction;
		CCMenuItemLambda* allReceive;
	CCSprite* gray;
	CCObject* target_final;
	SEL_CallFunc delegate_final;
		CCLabelTTF* m_nothingMessage; // 받은 메시지가 없습니다.
		int download_card_number;
		CCNode* loading_card_img;
		CCNode* addCardImg(int t_card_number, int t_card_level, string t_passive);
		void resultLoadedCardInfo (Json::Value result_data);
		vector <DownloadFile> df_list;
//		vector <CopyFile> cf_list;
		CCSprite* loading_circle;

		int ing_download_cnt;
		void successAction ();
		void failAction();
		void startDownloadCardImage ();
		void startDownload ();
		

	
	bool isLoaded;
};
#undef LZZ_INLINE

