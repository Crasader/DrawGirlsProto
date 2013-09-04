//
//  StarGoldData.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 3. 12..
//
//

#include "StarGoldData.h"
#include "MyLocalization.h"


CCSprite* StarGoldData::getLoadingImg()
{
	if(after_loading == kImgType_Empty)
	{
		after_loading = getRandomImgType();
	}
	
	string filename;
	string ments;
	if(after_loading == kImgType_specialMap)
	{
		filename = "loadingimg_specialMap.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading1);
	}
	else if(after_loading == kImgType_control1)
	{
		filename = "loadingimg_control1.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading2);
	}
	else if(after_loading == kImgType_control2)
	{
		filename = "loadingimg_control2.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading3);
	}
	else if(after_loading == kImgType_facebook1)
	{
		filename = "loadingimg_facebook1.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading4);
	}
	else if(after_loading == kImgType_facebook2)
	{
		filename = "loadingimg_facebook2.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading5);
	}
	else if(after_loading == kImgType_facebook3)
	{
		filename = "loadingimg_facebook3.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading6);
	}
//	else if(after_loading == kImgType_item1)
//	{
//		filename = "loadingimg_item1.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading11);
//	}
//	else if(after_loading == kImgType_item2)
//	{
//		filename = "loadingimg_item2.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading12);
//	}
//	else if(after_loading == kImgType_item3)
//	{
//		filename = "loadingimg_item3.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading13);
//	}
//	else if(after_loading == kImgType_item4)
//	{
//		filename = "loadingimg_item4.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading14);
//	}
//	else if(after_loading == kImgType_item5)
//	{
//		filename = "loadingimg_item5.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading15);
//	}
//	else if(after_loading == kImgType_item6)
//	{
//		filename = "loadingimg_item6.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading16);
//	}
	else if(after_loading == kImgType_gallery)
	{
		filename = "loadingimg_gallery.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading21);
	}
	else if(after_loading == kImgType_gallery1)
	{
		filename = "loadingimg_gallery1.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading22);
	}
	else if(after_loading == kImgType_option)
	{
		filename = "loadingimg_option.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading23);
	}
	else if(after_loading == kImgType_pet)
	{
		filename = "loadingimg_pet.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading24);
	}
	else if(after_loading == kImgType_petbuff)
	{
		filename = "loadingimg_petbuff.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading25);
	}
	else if(after_loading == kImgType_elementalPresentation1)
	{
		filename = "loadingimg_elementalPresentation1.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading17);
	}
	else if(after_loading == kImgType_elemental)
	{
		filename = "loadingimg_elemental.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading19);
	}
	else if(after_loading == kImgType_facebookLoginReward)
	{
		filename = "loadingimg_facebookLoginReward.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading20);
	}
	
	after_loading = kImgType_Empty;
	
	CCSprite* t_loadingImg = CCSprite::create(filename.c_str());
	CCLabelTTF* t_loadingMents = CCLabelTTF::create(ments.c_str(), getFont().c_str(), 14, CCSizeMake(260, 300), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
	t_loadingMents->setAnchorPoint(ccp(0,1));
	t_loadingMents->setPosition(ccp(30,117));
	t_loadingImg->addChild(t_loadingMents);
	
	return t_loadingImg;
}

string StarGoldData::getFont()
{
	string font_name;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	font_name = "AppleGothic";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	font_name = "";
#endif
	return font_name;
}