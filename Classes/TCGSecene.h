//
//  TCG.h
//  DGproto
//
//  Created by LitQoo on 2014. 11. 11..
//
//

#ifndef __DGproto__TCG__
#define __DGproto__TCG__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"
#include "jsoncpp/json.h"
#include "KSLabelTTF.h"
#include "CommonButton.h"
#include "KSUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class CardSet : public cocos2d::CCLayerColor
{
public:
	CCSprite *image;
	
	KSLabelTTF *type_lbl;
	KSLabelTTF *atk_lbl;
	KSLabelTTF *def_lbl;
	KSLabelTTF *ghost_lbl;
	KSLabelTTF *used_lbl;
	
	
	
	CCSprite* cardCover;
	CCSprite* cardImage;
	int cardNo;
	int type;
	int atk;
	int def;
	int delayTurn;
	CCPoint oldPosition;
	bool isUsed;
	bool isGhost;
	std::function<void(CardSet*,bool)> func;
	
	
	void setFunction(std::function<void(CardSet*,bool)> _func){
		func = _func;
	}
	void setOldPosition(CCPoint pos){
		oldPosition = pos;
	}
	
	static CardSet* create(Json::Value cardInfo){
		CardSet* n = new CardSet();
		n->init(cardInfo);
		n->autorelease();
		return n;
	}
	
	void setAtk(int _n){
		atk = _n;
		atk_lbl->setString(CCString::createWithFormat("%d",atk)->getCString());
	}
	
	
	void setDef(int _n){
		def = _n;
		if(def<=0){
			def=0;
			addChild(KSTimer::create(1,[this](){ghost_lbl->setVisible(true);}));
			isGhost=true;
		}
		
		def_lbl->setString(CCString::createWithFormat("%d",def)->getCString());
		
	}
	
	void setUsed(bool _n){
		isUsed=_n;
		if(isUsed){
			cardImage->setColor(ccc3(100,100,100));
			cardCover->setColor(ccc3(100,100,100));
		}else{
			cardImage->setColor(ccc3(255,255,255));
			cardCover->setColor(ccc3(255,255,255));
		}
		//used_lbl->setVisible(isUsed);
	}
	
	void setDelay(int d){
		delayTurn = d;
	}
	
	void minusDelay(){
		delayTurn--;
		if(delayTurn<=0){
			delayTurn=0;
			setUsed(false);
			used_lbl->setVisible(false);
		}else{
			setUsed(true);
			used_lbl->setVisible(true);
			used_lbl->setString(CCString::createWithFormat("%d턴후\n사용가능",delayTurn)->getCString());
		}
	}
	
	void damageAction(){
		addChild(KSTimer::create(0.1, [this](){
			addChild(KSIntervalCall::create(10, 3, [this](int n){
				this->cardImage->setColor(ccc3(255,0,0));
				this->addChild(KSTimer::create(0.1, [this](){
					this->cardImage->setColor(ccc3(255,255,255));
				}));
			}));
		}));
	}
	
	bool init(Json::Value cardInfo)
	{
		//////////////////////////////
		// 1. super init first
		if (!CCLayerColor::initWithColor(ccc4(100, 0, 0, 100), 80, 100));
		{
			//return false;
		}
		
		ignoreAnchorPointForPosition(false);
		
		string font_name;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		font_name = "jrNaver";
		//font_name = "RixGoEB";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		font_name = "fonts/jrNaver.ttf"; //RixHeadEB.ttf //RixMGoB.ttf //RixJGoB
#endif
		
		cardNo = cardInfo["cardNo"].asInt();
		
		isUsed = false;
		isGhost = false;
		delayTurn = 0;
		
		cardImage = CCSprite::create(cardInfo["image"].asString().c_str());
		cardImage->setPosition(ccp(40,50));
		addChild(cardImage);
		
		
		cardCover = CCSprite::create("tcgset.png");
		
		CommonButton* setBtn = CommonButton::create(cardCover, -999);
		setBtn->setFunction([this](CCObject *obj){
			if(func)func(this,false);
		});
		setBtn->setPosition(ccp(40,50));
		addChild(setBtn);
		
		
		ghost_lbl = KSLabelTTF::create("GHOST", font_name.c_str(), 15);
		ghost_lbl->setColor(ccc3(0,0,0));
		ghost_lbl->enableOuterStroke(ccc3(255,255,255), 2);
		ghost_lbl->setPosition(ccp(40,50));
		addChild(ghost_lbl);
		ghost_lbl->setVisible(false);
		
		used_lbl = KSLabelTTF::create("사용불가", font_name.c_str(), 15);
		used_lbl->setPosition(ccp(40,20));
		used_lbl->setColor(ccc3(255,0,0));
		addChild(used_lbl);
		used_lbl->setVisible(false);
		
		type_lbl = KSLabelTTF::create(cardInfo["type_str"].asString().c_str(), font_name.c_str(), 15);
		type_lbl->setPosition(ccp(5,3));
		type_lbl->setColor(ccc3(0,0,0));
		addChild(type_lbl);
		type = cardInfo["type"].asInt();
		
		atk_lbl = KSLabelTTF::create(cardInfo["atk"].asString().c_str(), font_name.c_str(), 15);
		atk_lbl->setPosition(ccp(5,93));
		addChild(atk_lbl);
		atk = cardInfo["atk"].asInt();
		
		def_lbl = KSLabelTTF::create(cardInfo["def"].asString().c_str(), font_name.c_str(), 15);
		def_lbl->setPosition(ccp(75,93));
		addChild(def_lbl);
		def = cardInfo["def"].asInt();
		
		
		return true;
	}
};

class TCGSecene : public cocos2d::CCLayer//, public BackKeyBase//, public CCTextFieldDelegate
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	virtual bool init();
	
	int wins;
	// there's no 'id' in cpp, so we recommend to return the class instance pointer
	
	static CCScene* scene(int wins)
	{
		// 'scene' is an autorelease object
		CCScene *scene = CCScene::create();
		
		// 'layer' is an autorelease object
		TCGSecene *layer = TCGSecene::create();
		layer->wins = wins;
		scene->addChild(layer);
		
		return scene;
	}

	
	// preprocessor macro for "static create()" constructor ( node() deprecated )
	//CREATE_FUNC(TCGSecene);
	
	static TCGSecene* create(){
		TCGSecene* n = new TCGSecene();
		n->init();
		n->autorelease();
		return n;
	}
	Json::Value cardInfo;
	
	int round;
	int turn;
	int mySelectedCard;
	int otherSelectedCard;
	int mycards[5];
	int othercards[5];
	void resetCardInfo();
	bool isAutoPlay;
	CCLayer* table;
	
	KSLabelTTF* system_lbl;
	
	KSLabelTTF *myWin_lbl;
	KSLabelTTF *otherWin_lbl;
	
	int myWin;
	int otherWin;
	
	
	CommonButton* restartBtn;
	
	
	void addMyWin(){
		myWin++;
		myWin_lbl->setString(CCString::createWithFormat("%d승",myWin)->getCString());
	}
	
	
	void addOtherWin(){
		otherWin++;
		otherWin_lbl->setString(CCString::createWithFormat("%d승",otherWin)->getCString());
	}

	
	void addCards(){
		
		for(int i=0;i<5;i++){
			int cardNo = mycards[i];
			Json::Value card = cardInfo[cardNo];
			CardSet* cd = CardSet::create(card);
			cd->setFunction(std::bind(&TCGSecene::selectCard, this, std::placeholders::_1, std::placeholders::_2));
			cd->setPosition(ccp(i*85+70,60));
			cd->setOldPosition(ccp(i*85+70,60));
			cd->setTag(cardNo);
			table->addChild(cd);
			
		}
		
		
		for(int i=0;i<5;i++){
			int cardNo = othercards[i];
			Json::Value card = cardInfo[cardNo];
			CardSet* cd = CardSet::create(card);
			cd->setPosition(ccp(i*85+70,260));
			cd->setOldPosition(ccp(i*85+70,260));
			cd->setTag(cardNo);
			table->addChild(cd);
			
		}
		
	}
	
	int actionCnt;
	
	void selectCard(CardSet* card,bool isAuto){
		if(actionCnt!=0)return;
		if(card->isUsed)return;
		if(isAuto==false && isAutoPlay==true)return;
		
		
		actionCnt = 1;
		
		card->runAction(CCSpawn::create(CCMoveTo::create(0.3, ccp(240+100,160)),CCScaleTo::create(0.3, 2),NULL));
		card->setZOrder(100);
		mySelectedCard = card->cardNo;
		CardSet* otherCard;
		while(1){
			int rand = std::rand()%5;
			int otherCardNo=othercards[rand];
			otherCard = (CardSet*)table->getChildByTag(otherCardNo);
			
			if(otherCard->isUsed==false){
				otherCard->runAction(CCSpawn::create(CCMoveTo::create(0.3, ccp(240-100,160)),CCScaleTo::create(0.3, 2),NULL));
				otherCard->setZOrder(100);
				otherSelectedCard = otherCard->cardNo;
				break;
			}
		}
		
		turn++;
		
		
		// 1단계 데미지 주는 액션
		addChild(KSTimer::create(0.8, [this,card,otherCard](){
			
			
			if(card->type == otherCard->type){ // 비김
				card->setDef(card->def-1);
				otherCard->setDef(otherCard->def-1);
				card->damageAction();
				otherCard->damageAction();
				otherCard->runAction(CCSequence::create(CCMoveBy::create(0.1,ccp(30,0)),CCMoveBy::create(0.1,ccp(-30,0)),NULL));
				card->runAction(CCSequence::create(CCMoveBy::create(0.1,ccp(-30,0)),CCMoveBy::create(0.1,ccp(30,0)),NULL));
			}else if((card->type==2 && otherCard->type==1) || (card->type==3 && otherCard->type==2) || (card->type==1 && otherCard->type==3)){ // 짐
			
				card->setDef(card->def-otherCard->atk);
				card->damageAction();
				
				otherCard->runAction(CCSequence::create(CCMoveBy::create(0.1,ccp(30,0)),CCMoveBy::create(0.1,ccp(-30,0)),NULL));
				this->addOtherWin();
			}else{ // 이김
				
				otherCard->setDef(otherCard->def-card->atk);
				otherCard->damageAction();
				
				card->runAction(CCSequence::create(CCMoveBy::create(0.1,ccp(-30,0)),CCMoveBy::create(0.1,ccp(30,0)),NULL));
				this->addMyWin();
			}
			
			
			// 2단계 제자리로 이동
			addChild(KSTimer::create(0.8, [this,card,otherCard](){
				
				otherCard->setZOrder(10);
				card->setZOrder(10);
				//card->setDelay(4);
				//otherCard->setDelay(4);
				card->setUsed(true);
				otherCard->setUsed(true);
				
				this->actionCnt=0;
				
				
				card->runAction(CCSpawn::create(CCMoveTo::create(0.3, card->oldPosition),CCScaleTo::create(0.3, 1),NULL));
				otherCard->runAction(CCSpawn::create(CCMoveTo::create(0.3, otherCard->oldPosition),CCScaleTo::create(0.3, 1),NULL));
				
				string state="";
				
				//게임오버 검사
				
				int myGhostCnt=0;
				int otherGhostCnt=0;
				
				for(int i=0;i<5;i++){
					int otherCardNo = othercards[i];
					int myCardNo = mycards[i];
					CardSet* otherCard = (CardSet*)table->getChildByTag(otherCardNo);
					CardSet* myCard = (CardSet*)table->getChildByTag(myCardNo);
					if(otherCard->isGhost)otherGhostCnt++;
					if(myCard->isGhost)myGhostCnt++;
				}
				
				if(otherGhostCnt==5 && myGhostCnt==5){
					state=CCString::createWithFormat("비겼습니다.\n다시하기(%d연승중)",wins)->getCString();
					restartBtn->setTitle(state);
					restartBtn->setVisible(true);
					actionCnt=1;
				}else if(otherGhostCnt==5){
					wins++;
					
					state=CCString::createWithFormat("이겼습니다.\n%d연승도전",wins+1)->getCString();
					restartBtn->setTitle(state);
					restartBtn->setVisible(true);
					actionCnt=1;
				}else if(myGhostCnt==5){
					state=CCString::createWithFormat("졌습니다.(%d연승)\n다시하기",wins)->getCString();
					restartBtn->setTitle(state);
					restartBtn->setVisible(true);
					actionCnt=1;
					wins=0;
				}
				
				
//				for(int i=0;i<5;i++){
//					int otherCardNo = othercards[i];
//					int myCardNo = mycards[i];
//					CardSet* otherCard = (CardSet*)table->getChildByTag(otherCardNo);
//					otherCard->minusDelay();
//					CardSet* myCard = (CardSet*)table->getChildByTag(myCardNo);
//					myCard->minusDelay();
//				}
				
				//라운드 종료
				if(turn==5){
				
					for(int i=0;i<5;i++){
						int otherCardNo = othercards[i];
						int myCardNo = mycards[i];
						CardSet* otherCard = (CardSet*)table->getChildByTag(otherCardNo);
						otherCard->setUsed(false);
						CardSet* myCard = (CardSet*)table->getChildByTag(myCardNo);
						myCard->setUsed(false);
					}
					
					turn=0;
					round++;
				}
				
				if(state==""){
					CCString* str = CCString::createWithFormat("%d라운드 %d턴\n카드를 선택해주세요",round,turn+1);
					state = str->getCString();
					
					if(isAutoPlay){
						addChild(KSTimer::create(1,[this](){
							while(1){
								int rand = std::rand()%5;
								int myCardNo=mycards[rand];
								CardSet* myCard = (CardSet*)table->getChildByTag(myCardNo);
								
								if(myCard->isUsed==false){
									selectCard(myCard,true);
									break;
								}
							}
						}));
					}

				}
				system_lbl->setString(state.c_str());
					
			}));
			
		}));
	
	}
	
};



#endif /* defined(__DGproto__TCG__) */
