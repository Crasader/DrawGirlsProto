#ifndef EscapeTheSpace_MyLocalization_h
#define EscapeTheSpace_MyLocalization_h

#include "cocos2d.h"
#include "platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"

using namespace cocos2d;
using namespace std;

typedef enum MyLocalKey_ {
	kMyLocalKey_canNotConnectedServer = 1,
	kMyLocalKey_touchPlease,
	kMyLocalKey_tutorial1,
	kMyLocalKey_tutorial2,
	kMyLocalKey_tutorial3,
	kMyLocalKey_tutorial4,
	kMyLocalKey_tutorial5,
	kMyLocalKey_tutorial6,
	kMyLocalKey_tutorial7,
	kMyLocalKey_tutorial8,
	kMyLocalKey_tutorial9,
	kMyLocalKey_tutorial10,
	kMyLocalKey_tutorial11,
	kMyLocalKey_tutorial12,
	kMyLocalKey_tutorial13,
	kMyLocalKey_tutorial14,
	kMyLocalKey_tutorial15,
	kMyLocalKey_tutorial16,
	kMyLocalKey_tutorial17,
	kMyLocalKey_tutorial17_1,
	kMyLocalKey_tutorial18,
	kMyLocalKey_tutorial19,
	kMyLocalKey_tutorial20,
	kMyLocalKey_tutorial21,
	kMyLocalKey_tutorial22,
	kMyLocalKey_tutorial23,
	kMyLocalKey_tutorial24,
	kMyLocalKey_tutorial25,
	kMyLocalKey_tutorial26,
	kMyLocalKey_loading1,
	kMyLocalKey_loading2,
	kMyLocalKey_loading3,
	kMyLocalKey_loading4,
	kMyLocalKey_loading5,
	kMyLocalKey_loading6,
	kMyLocalKey_loading11,
	kMyLocalKey_loading12,
	kMyLocalKey_loading13,
	kMyLocalKey_loading14,
	kMyLocalKey_loading15,
	kMyLocalKey_loading16,
	kMyLocalKey_loading17,
	kMyLocalKey_loading18,
	kMyLocalKey_loading19,
	kMyLocalKey_loading20,
	kMyLocalKey_loading21,
	kMyLocalKey_loading22,
	kMyLocalKey_loading23,
	kMyLocalKey_loading24,
	kMyLocalKey_loading25,
	kMyLocalKey_notSelectedItem,
	kMyLocalKey_boughtItem,
	kMyLocalKey_multiSelect,
	kMyLocalKey_notEnoughGold,
	kMyLocalKey_goShop,
	kMyLocalKey_notEnoughBrush,
	kMyLocalKey_notEnoughRuby,
	kMyLocalKey_exit,
	kMyLocalKey_review,
	kMyLocalKey_regiGallery,
	kMyLocalKey_openHard,
	kMyLocalKey_shieldCancel,
	kMyLocalKey_item1,
	kMyLocalKey_item2,
	kMyLocalKey_item3,
	kMyLocalKey_item4,
	kMyLocalKey_item5,
	kMyLocalKey_item6,
	kMyLocalKey_exeAppReward,
	kMyLocalKey_giftAHour,
	kMyLocalKey_giftInvalid,
	kMyLocalKey_giftUsed,
	kMyLocalKey_giftLitqoo,
	kMyLocalKey_goHome,
	kMyLocalKey_star3clear,
	kMyLocalKey_itemEnable,
	kMyLocalKey_needUpdate,
	kMyLocalKey_newVersion,
	kMyLocalKey_facebookInvite,
	kMyLocalKey_facebookWith,
	kMyLocalKey_petEmpty,
	kMyLocalKey_petAttack,
	kMyLocalKey_petGold,
	kMyLocalKey_petFast,
	kMyLocalKey_petSilence,
	kMyLocalKey_petSlow,
	kMyLocalKey_getMonster
}MyLocalKey;

class MyLocal : public CCDictionary
{
	
public:
	
	static MyLocal* sharedInstance();
	
	~MyLocal()
	{
		
	}
	
	const char* getLocalForKey(MyLocalKey key);
	
	CCString* getLocalCode();
	
private:
	MyLocal() : CCDictionary()
	{
		this->initLocal();
	}
	void initLocal();
	void setLocal();
	CCString* languageType;
};



#endif
