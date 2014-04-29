//
//  FlagSelector.h
//  DGproto
//
//  Created by LitQoo on 2014. 4. 29..
//
//

#ifndef __DGproto__FlagSelector__
#define __DGproto__FlagSelector__

#include <iostream>

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GraphDog.h"
#include "CommonButton.h"
#include "KSUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class FlagSelector :  public CCNode,public CCTableViewDelegate, public CCTableViewDataSource{
  CCTableView* fTable;
	CCSprite* selectedFlagSpr;
	vector<string> countryList;
	string selectedFlag;
public:
	static string getFlagString(string flag){
		const char* countries[] = {"ad",
			"ae",
			"af",
			"ag",
			"ai",
			"al",
			"am",
			"an",
			"ao",
			"aq",
			"ar",
			"as",
			"at",
			"au",
			"aw",
			"ax",
			"az",
			"ba",
			"bb",
			"bd",
			"be",
			"bf",
			"bg",
			"bh",
			"bi",
			"bj",
			"bl",
			"bm",
			"bn",
			"bo",
			"br",
			"bs",
			"bt",
			"bw",
			"by",
			"bz",
			"ca",
			"cd",
			"cf",
			"cg",
			"ch",
			"ci",
			"cl",
			"cm",
			"cn",
			"co",
			"cr",
			"cu",
			"cv",
			"cy",
			"cz",
			"de",
			"dj",
			"dk",
			"dm",
			"do",
			"dz",
			"ec",
			"ee",
			"eg",
			"eh",
			"er",
			"es",
			"et",
			"fi",
			"fj",
			"fk",
			"fm",
			"fo",
			"fr",
			"ga",
			"gb",
			"gd",
			"ge",
			"gg",
			"gh",
			"gl",
			"gm",
			"gn",
			"gq",
			"gr",
			"gs",
			"gt",
			"gu",
			"gw",
			"gy",
			"hk",
			"hn",
			"hr",
			"ht",
			"hu",
			"id",
			"ie",
			"il",
			"im",
			"in",
			"iq",
			"ir",
			"is",
			"it",
			"je",
			"jm",
			"jo",
			"jp",
			"ke",
			"kg",
			"kh",
			"ki",
			"km",
			"kn",
			"kp",
			"kr",
			"kw",
			"ky",
			"kz",
			"la",
			"lb",
			"lc",
			"li",
			"lk",
			"lr",
			"ls",
			"lt",
			"lu",
			"lv",
			"ly",
			"ma",
			"mc",
			"md",
			"me",
			"mg",
			"mh",
			"mk",
			"ml",
			"mm",
			"mn",
			"mo",
			"mp",
			"mr",
			"ms",
			"mt",
			"mu",
			"mv",
			"mw",
			"mx",
			"my",
			"mz",
			"na",
			"ne",
			"nf",
			"ng",
			"ni",
			"nl",
			"no",
			"np",
			"nr",
			"nz",
			"om",
			"pa",
			"pe",
			"pg",
			"ph",
			"pk",
			"pl",
			"pn",
			"pr",
			"ps",
			"pt",
			"pw",
			"py",
			"qa",
			"ro",
			"rs",
			"ru",
			"rw",
			"sa",
			"sb",
			"sc",
			"sd",
			"se",
			"sg",
			"sh",
			"si",
			"sk",
			"sl",
			"sm",
			"sn",
			"so",
			"sr",
			"st",
			"sv",
			"sy",
			"sz",
			"tc",
			"td",
			"tg",
			"th",
			"tj",
			"tl",
			"tm",
			"tn",
			"to",
			"tr",
			"tt",
			"tv",
			"tw",
			"tz",
			"ua",
			"ug",
			"us",
			"uy",
			"uz",
			"vc",
			"ve",
			"vg",
			"vi",
			"vn",
			"vu",
			"ws",
			"ye",
			"za",
			"zm",
			"zw"};
		
		for(int i=0; i<sizeof(countries) / sizeof(countries[0]); i++)
		{
			string f = countries[i];
			if(f==flag){
				return CCString::createWithFormat("%s.png",flag.c_str())->getCString();
			}
		}
		
		return "kr.png";

	}
  virtual bool init(){
		
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
		const char* countries[] = {"ad",
			"ae",
			"af",
			"ag",
			"ai",
			"al",
			"am",
			"an",
			"ao",
			"aq",
			"ar",
			"as",
			"at",
			"au",
			"aw",
			"ax",
			"az",
			"ba",
			"bb",
			"bd",
			"be",
			"bf",
			"bg",
			"bh",
			"bi",
			"bj",
			"bl",
			"bm",
			"bn",
			"bo",
			"br",
			"bs",
			"bt",
			"bw",
			"by",
			"bz",
			"ca",
			"cd",
			"cf",
			"cg",
			"ch",
			"ci",
			"cl",
			"cm",
			"cn",
			"co",
			"cr",
			"cu",
			"cv",
			"cy",
			"cz",
			"de",
			"dj",
			"dk",
			"dm",
			"do",
			"dz",
			"ec",
			"ee",
			"eg",
			"eh",
			"er",
			"es",
			"et",
			"fi",
			"fj",
			"fk",
			"fm",
			"fo",
			"fr",
			"ga",
			"gb",
			"gd",
			"ge",
			"gg",
			"gh",
			"gl",
			"gm",
			"gn",
			"gq",
			"gr",
			"gs",
			"gt",
			"gu",
			"gw",
			"gy",
			"hk",
			"hn",
			"hr",
			"ht",
			"hu",
			"id",
			"ie",
			"il",
			"im",
			"in",
			"iq",
			"ir",
			"is",
			"it",
			"je",
			"jm",
			"jo",
			"jp",
			"ke",
			"kg",
			"kh",
			"ki",
			"km",
			"kn",
			"kp",
			"kr",
			"kw",
			"ky",
			"kz",
			"la",
			"lb",
			"lc",
			"li",
			"lk",
			"lr",
			"ls",
			"lt",
			"lu",
			"lv",
			"ly",
			"ma",
			"mc",
			"md",
			"me",
			"mg",
			"mh",
			"mk",
			"ml",
			"mm",
			"mn",
			"mo",
			"mp",
			"mr",
			"ms",
			"mt",
			"mu",
			"mv",
			"mw",
			"mx",
			"my",
			"mz",
			"na",
			"ne",
			"nf",
			"ng",
			"ni",
			"nl",
			"no",
			"np",
			"nr",
			"nz",
			"om",
			"pa",
			"pe",
			"pg",
			"ph",
			"pk",
			"pl",
			"pn",
			"pr",
			"ps",
			"pt",
			"pw",
			"py",
			"qa",
			"ro",
			"rs",
			"ru",
			"rw",
			"sa",
			"sb",
			"sc",
			"sd",
			"se",
			"sg",
			"sh",
			"si",
			"sk",
			"sl",
			"sm",
			"sn",
			"so",
			"sr",
			"st",
			"sv",
			"sy",
			"sz",
			"tc",
			"td",
			"tg",
			"th",
			"tj",
			"tl",
			"tm",
			"tn",
			"to",
			"tr",
			"tt",
			"tv",
			"tw",
			"tz",
			"ua",
			"ug",
			"us",
			"uy",
			"uz",
			"vc",
			"ve",
			"vg",
			"vi",
			"vn",
			"vu",
			"ws",
			"ye",
			"za",
			"zm",
			"zw"};
		selectedFlagSpr=NULL;
		
		for(int i=0; i<sizeof(countries) / sizeof(countries[0]); i++)
		{
			countryList.push_back(countries[i]);
		}
		
		fTable = CCTableView::create(this,CCSizeMake(200-20,40));
		fTable->setDirection(kCCScrollViewDirectionHorizontal);
		fTable->setVerticalFillOrder(kCCTableViewFillTopDown);
		fTable->setAnchorPoint(ccp(0,0));
		fTable->setPosition(10,4);
		fTable->setTouchPriority(INT_MAX*-1);
		fTable->setTouchEnabled(true);
		fTable->setDelegate(this);
		addChild(fTable,2);
		
		string lang = KS::getLocalCode();
		if(lang =="ko")selectedFlag="kr";
		else if(lang=="ja")selectedFlag="jp";
		else selectedFlag="kr";

		selectedFlagSpr = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("%s.png",selectedFlag.c_str())->getCString());
		selectedFlagSpr->setPosition(ccp(100,60));
		selectedFlagSpr->setScale(1.5);
		addChild(selectedFlagSpr,2);
		
		CCScale9Sprite* back = CommonButton::getBackgroundByType(CommonButtonGray);
		back->setContentSize(CCSizeMake(200,100));
		back->setAnchorPoint(ccp(0,0));
		back->setPosition(0,0);
		addChild(back,1);
		
		return true;
	}
	
	
	CREATE_FUNC(FlagSelector);
	
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx){
		CCTableViewCell* cell = new CCTableViewCell();
		cell->init();
		cell->autorelease();
		
		CCSprite* f = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("%s.png",countryList[idx].c_str())->getCString());
		f->setPosition(ccp(f->getContentSize().width/2.f,f->getContentSize().height/2.f));
		f->setAnchorPoint(ccp(0.5,0.5));
		f->setTag(10);
		cell->addChild(f);
		
		return cell;
	}
	virtual void scrollViewDidScroll(CCScrollView* view){}
	virtual void scrollViewDidZoom(CCScrollView* view){}
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell){
		selectedFlag = countryList[cell->getIdx()].c_str();
		
		if(selectedFlagSpr!=NULL)selectedFlagSpr->removeFromParent();
		
		selectedFlagSpr = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("%s.png",selectedFlag.c_str())->getCString());
		selectedFlagSpr->setPosition(ccp(100,60));
		selectedFlagSpr->setScale(1.5);
		addChild(selectedFlagSpr,2);
	}
	virtual CCSize cellSizeForTable(CCTableView *table){
		return CCSizeMake(34,40);
	}
	virtual unsigned int numberOfCellsInTableView(CCTableView *table){
		return countryList.size();
	}
	
	string getFlag(){
		return selectedFlag;
	}
	

};

#endif /* defined(__DGproto__FlagSelector__) */


/*
 
 */