#ifndef ACCOUNTMANAGERPOPUP_H
#define ACCOUNTMANAGERPOPUP_H

#include "cocos2d.h"
#include "HSPEnums.h"
#include "jsoncpp/json.h"
USING_NS_CC;

class StyledLabelTTF;
class ASPopupView;

void saveOAuthType(int i);
int getSavedOAuthType(int def);
int newOAuthTypeToServerOAuthType(int newT);

class AccountManagerPopup : public CCLayer
{
public:
	AccountManagerPopup();
	virtual ~AccountManagerPopup();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(int touchP);
	static AccountManagerPopup* create(int touchP)
	{
		AccountManagerPopup* t = new AccountManagerPopup();
		t->init(touchP);
		t->autorelease();
		return t;
	}
	
	void showWarning(const std::string& desc, HSPMapping hspmap, 
																							StyledLabelTTF* announce,
																							std::function<void(void)> loginProcess);
	//virtual void registerWithTouchDispatcher();
	std::function<void(Json::Value)> m_logoutRepeater1;
	std::function<void(Json::Value)> m_logoutRepeater2;
	std::function<void(Json::Value)> m_logoutRepeater3;
protected:
	int m_touchP;
	ASPopupView* m_managerPopup;
};

#endif
