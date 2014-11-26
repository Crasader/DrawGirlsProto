//
//  TCG.cpp
//  DGproto
//
//  Created by LitQoo on 2014. 11. 11..
//
//

#include "TCGSecene.h"


// on "init" you need to initialize your instance
bool TCGSecene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	
	string font_name;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	font_name = "jrNaver";
	//font_name = "RixGoEB";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	font_name = "fonts/jrNaver.ttf"; //RixHeadEB.ttf //RixMGoB.ttf //RixJGoB
#endif
	
	restartBtn = CommonButton::create("재시작", 15, CCSize(200,80), CommonButtonYellow, -999);
	restartBtn->setPosition(ccp(240,160));
	restartBtn->setVisible(false);
	restartBtn->setFunction([this](CCObject* obj){
		CCDirector::sharedDirector()->replaceScene(TCGSecene::scene(this->wins));
	});
	addChild(restartBtn);
	
	CommonButton*autoBtn = CommonButton::create("autoOFF", 15, CCSize(80,40), CommonButtonBlue, -999);
	autoBtn->setPosition(ccp(240+160,160+20));
	autoBtn->setFunction([this,autoBtn](CCObject* obj){
		if(actionCnt==1)return;
		
		this->isAutoPlay = !this->isAutoPlay;
		if(this->isAutoPlay){
			autoBtn->setTitle("autoON");
		
				addChild(KSTimer::create(0.5f,[this](){
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

		}else{
			autoBtn->setTitle("autoOFF");
		}
		
	});
	
	addChild(autoBtn);
	
	
	isAutoPlay = false;
	wins=0;
	
	myWin_lbl = KSLabelTTF::create("0승", font_name.c_str(), 15);
	myWin_lbl->setColor(ccc3(255,255,0));
	myWin_lbl->setPosition(ccp(240+160,160-20));
	addChild(myWin_lbl);
	
	otherWin_lbl = KSLabelTTF::create("0승", font_name.c_str(), 15);
	otherWin_lbl->setColor(ccc3(255,255,0));
	otherWin_lbl->setPosition(ccp(240-160,160+20));
	addChild(otherWin_lbl);
	
	
	myWin=0;
	otherWin=0;
	
	system_lbl = KSLabelTTF::create("1라운드 1턴\n카드를 선택해주세요", font_name.c_str(), 15);
	system_lbl->setPosition(ccp(240,160));
	addChild(system_lbl);
	
	resetCardInfo();
	table = CCLayer::create();
	addChild(table);
	addCards();
	
	round=1;
	turn=0;
	
	actionCnt=0;
	
	return true;
}


void TCGSecene::resetCardInfo(){
	cardInfo.clear();
	
	mycards[0]=1;
	mycards[1]=2;
	mycards[2]=3;
	mycards[3]=4;
	mycards[4]=5;
	
	othercards[0]=6;
	othercards[1]=7;
	othercards[2]=8;
	othercards[3]=9;
	othercards[4]=10;
	
	int cardNo=0;
	
	cardInfo[cardNo]["name"]="???";
	cardInfo[cardNo]["type"]=1;
	cardInfo[cardNo]["type_str"]="묵";
	cardInfo[cardNo]["atk"]=3;
	cardInfo[cardNo]["def"]=5;
	cardInfo[cardNo]["cardNo"]=1;
	cardInfo[cardNo]["image"]="tcg1.png";
	cardInfo[cardNo]["isGhost"]=false;
	
	cardNo++;
	
	cardInfo[cardNo]["name"]="아스카";
	cardInfo[cardNo]["type"]=1;
	cardInfo[cardNo]["type_str"]="묵";
	cardInfo[cardNo]["atk"]=2;
	cardInfo[cardNo]["def"]=6;
	cardInfo[cardNo]["cardNo"]=1;
	cardInfo[cardNo]["image"]="tcg1.png";
	cardInfo[cardNo]["isGhost"]=false;
	
	cardNo++;
	cardInfo[cardNo]["name"]="이카루가";
	cardInfo[cardNo]["type"]=2;
	cardInfo[cardNo]["type_str"]="찌";
	cardInfo[cardNo]["atk"]=6;
	cardInfo[cardNo]["def"]=1;
	cardInfo[cardNo]["cardNo"]=2;
	cardInfo[cardNo]["image"]="tcg2.png";
	cardInfo[cardNo]["isGhost"]=false;
	
	
	cardNo++;
	cardInfo[cardNo]["name"]="카츠라기";
	cardInfo[cardNo]["type"]=3;
	cardInfo[cardNo]["type_str"]="빠";
	cardInfo[cardNo]["atk"]=4;
	cardInfo[cardNo]["def"]=4;
	cardInfo[cardNo]["cardNo"]=3;
	cardInfo[cardNo]["image"]="tcg3.png";
	cardInfo[cardNo]["isGhost"]=false;
	
	
	cardNo++;
	cardInfo[cardNo]["name"]="야규";
	cardInfo[cardNo]["type"]=1;
	cardInfo[cardNo]["type_str"]="묵";
	cardInfo[cardNo]["atk"]=1;
	cardInfo[cardNo]["def"]=7;
	cardInfo[cardNo]["cardNo"]=4;
	cardInfo[cardNo]["image"]="tcg4.png";
	cardInfo[cardNo]["isGhost"]=false;
	
	cardNo++;
	
	cardInfo[cardNo]["name"]="히바리";
	cardInfo[cardNo]["type"]=3;
	cardInfo[cardNo]["type_str"]="빠";
	cardInfo[cardNo]["atk"]=3;
	cardInfo[cardNo]["def"]=5;
	cardInfo[cardNo]["cardNo"]=5;
	cardInfo[cardNo]["image"]="tcg5.png";
	cardInfo[cardNo]["isGhost"]=false;
	
	cardNo++;
	
	cardInfo[cardNo]["name"]="호무라";
	cardInfo[cardNo]["type"]=1;
	cardInfo[cardNo]["type_str"]="묵";
	cardInfo[cardNo]["atk"]=2;
	cardInfo[cardNo]["def"]=6;
	cardInfo[cardNo]["cardNo"]=6;
	cardInfo[cardNo]["image"]="tcg6.png";
	cardInfo[cardNo]["isGhost"]=false;
	
	cardNo++;
	
	cardInfo[cardNo]["name"]="요미";
	cardInfo[cardNo]["type"]=2;
	cardInfo[cardNo]["type_str"]="찌";
	cardInfo[cardNo]["atk"]=6;
	cardInfo[cardNo]["def"]=1;
	cardInfo[cardNo]["cardNo"]=7;
	cardInfo[cardNo]["image"]="tcg7.png";
	cardInfo[cardNo]["isGhost"]=false;
	
	cardNo++;
	
	cardInfo[cardNo]["name"]="히카게";
	cardInfo[cardNo]["type"]=3;
	cardInfo[cardNo]["type_str"]="빠";
	cardInfo[cardNo]["atk"]=4;
	cardInfo[cardNo]["def"]=4;
	cardInfo[cardNo]["cardNo"]=8;
	cardInfo[cardNo]["image"]="tcg8.png";
	cardInfo[cardNo]["isGhost"]=false;
	
	cardNo++;
	
	cardInfo[cardNo]["name"]="미라이";
	cardInfo[cardNo]["type"]=1;
	cardInfo[cardNo]["type_str"]="묵";
	cardInfo[cardNo]["atk"]=1;
	cardInfo[cardNo]["def"]=7;
	cardInfo[cardNo]["cardNo"]=9;
	cardInfo[cardNo]["image"]="tcg9.png";
	cardInfo[cardNo]["isGhost"]=false;
	
	
	cardNo++;
	cardInfo[cardNo]["name"]="하루카";
	cardInfo[cardNo]["type"]=2;
	cardInfo[cardNo]["type_str"]="찌";
	cardInfo[cardNo]["atk"]=3;
	cardInfo[cardNo]["def"]=5;
	cardInfo[cardNo]["cardNo"]=10;
	cardInfo[cardNo]["image"]="tcg10.png";
	cardInfo[cardNo]["isGhost"]=false;
	
}
