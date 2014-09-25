//
//  ScenarioScene.h
//  DGproto
//
//  Created by LitQoo on 2014. 9. 15..
//
//

#ifndef __DGproto__ScenarioScene__
#define __DGproto__ScenarioScene__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "GraphDog.h"
#include "MyLocalization.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "TypingBox.h"
#include "KSUtil.h"
#include "StyledLabelTTF.h"
#include "TouchSuctionLayer.h";
#include "EasingAction.h";
#include "CCMenuLambda.h";
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


class StoryLayer : public CCLayer{
public:
	string storyID;
	CCLayerColor *black;
	CCLayer *back;
	CCLayerColor *top;
	CCLayerColor *bottom;
	TouchSuctionLayer *suction;
	Json::Value storyData;
	int storySeq;
	function<void(void)> callbackFunc;
	bool isAdd;
    
	static StoryLayer* create(string _storyID)
	{
		StoryLayer* obj = new StoryLayer();
		obj->init(_storyID);
		obj->autorelease();
		return obj;
	}
    
	bool init(string _storyID){
		if(!CCLayer::init()){
			return false;
		}
		
		isAdd=false;
		suction = TouchSuctionLayer::create(-10000);
		addChild(suction);
		
		suction->setTouchEnabled(true);
		this->storyID=_storyID;
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		float ui_top = myDSH->ui_top;
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
       // CCLOG("init storyLayer %f,%f,%f,%f,%f,%f,%f",screen_size.width,screen_size.height,myDSH->ui_top);
		top = CCLayerColor::create(ccc4(0,0,0,255), screen_scale_x*480,50);
		top->setAnchorPoint(ccp(0.5,0));
        top->ignoreAnchorPointForPosition(false);
		top->setPosition(ccp(240,320*screen_scale_y));
		addChild(top);
		
		bottom = CCLayerColor::create(ccc4(0,0,0,255), screen_scale_x*480,50);
		bottom->setAnchorPoint(ccp(0.5,0));
        bottom->ignoreAnchorPointForPosition(false);
		bottom->setPosition(ccp(240,0-50));
		addChild(bottom);
		
		this->addChild(KSGradualValue<float>::create(0.f,50.f,0.5f,[this,screen_scale_y](float t){
			bottom->setPositionY(t-50);
			top->setPositionY((320-t)*screen_scale_y);
			
		},[](float t){}));
		
		
		black = CCLayerColor::create(ccc4(0,0,0,0), 480, 320);
        
        black->setScaleX(screen_scale_x);
        black->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
        

        addChild(black);
        
		back = CCLayer::create();
		addChild(back);
		
        CCSprite* n_skip = CCSprite::create("kt_skip.png");
		CCSprite* s_skip = CCSprite::create("kt_skip.png");
		s_skip->setColor(ccGRAY);
		
		CCMenuLambda* skip_menu = CCMenuLambda::create();
		skip_menu->setPosition(ccp(50, 320*screen_scale_y-30));
		skip_menu->setTouchPriority(-19999);
		skip_menu->setEnabled(true );
		
		CCMenuItemLambda* skip_item = CCMenuItemSpriteLambda::create(n_skip, s_skip, [skip_menu,this](CCObject* sender)
																	 {
																		 skip_menu->setEnabled(false);
                                                                         this->storySeq=this->storyData.size();
                                                                         this->playScript();
                                                                     });
        skip_menu->addChild(skip_item);
        
		//this->storyData = Json::Value(Json::arrayValue);
        this->storySeq=0;
		this->m_sss=0;
		this->addChild(skip_menu, 999);
        
        

		return true;
	}
	
	enum BoxType{
		kBoxBig,
		kBoxMiddle,
		kBoxSmall
	};
	

	
	enum ObjType{
		kObjTypeNone=0x00,
		kObjTypeActorLeft=0x01,
		kObjTypeActorRight=0x02,
		kObjTypeActorCenter=0x04,
		kObjTypeActor=0x07,
		kObjTypeText=0x08,
		kObjTypeTitle=0x10,
		kObjTypeSpot=0x20,
		kObjTypeAll=0xff
	};
	
	enum scriptType{
		kScriptTypeTalk = 1,
		kScriptTypeTitle = 2,
		kScriptTypeSpot = 3
	};
	
	static void startStory(CCLayer* obj,string _storyID,function<void(void)> _endFunc){
		
		StoryLayer *sl  = StoryLayer::create(_storyID);
        TRACE();
        sl->callbackFunc = _endFunc;
		TRACE();
        obj->addChild(sl,9999999);
        
		TRACE();
        sl->loadStory();
		TRACE();
        sl->addChild(KSGradualValue<float>::create(0, 150, 1, [sl](int a){
			sl->black->setOpacity(a);
		}, [sl](int a){
            CCLOG("play %s",sl->storyID.c_str());
			sl->playScript();
		}));
        TRACE();
	}
	
	
	void loadStory(){
		
        CCLOG("load story");
		
		beginScene("ko","puzzle1");
		addScript("","오래전부터 영주를 모시며 첩보,파괴,암살등을 생업으로 삼은 자들이 있었다.\n오랜기간 그 명맥을 유지하며 현대시대에도 보통사람들의 눈을 피해 활동하고있는 자들.\n그들을 우리는 닌자라 부른다.",kCCTextAlignmentCenter,kObjTypeText,kBoxBig);
		
		addScript("kt_cha_asuka_1.png","선생님.\n지시받은대로 전원집합했습니다!\n여긴 인기척도 없고 전력으로 훈련할 수 있겠는데요?",kCCTextAlignmentLeft,kObjTypeText,kBoxBig);
		addScript("kt_cha_asuka_1.png","네? 훈련 전에 우리들의 힘을 시험한다구요?\n으윽.. 시험은 그다지 좋아하지 않지만..\n알겠습니다.",kCCTextAlignmentLeft,kObjTypeText,kBoxBig);
		addScript("kt_cha_ikaruga_1.png","역시나\n시노비 결계 속에서 가상의 적을 상대하면 되는건가요?\n어서 시작하시죠.",kCCTextAlignmentRight,kObjTypeText,kBoxBig);
		addScript("kt_cha_asuka_1.png","자..잠깐 그전에 대전법을 조금 익혀야 할것 같은데요..",kCCTextAlignmentLeft,kObjTypeText,kBoxBig);
		addScript("kt_cha_ikaruga_1.png","이런.. 벌써 훈련법을 잊어버리시다니..\n닌자로서의 기본이 안되어있군요.",kCCTextAlignmentRight,kObjTypeText,kBoxBig);
		addScript("kt_cha_asuka_1.png","죄송합니다..",kCCTextAlignmentLeft,kObjTypeAll,kBoxBig);
		
		////////////////////////////////////
		
		beginScene("ko","puzzle2");
		addScript("kt_cha_asuka_1.png","휴... 이걸로 시험이 끝났겠지.. 첫발을 내딛었구나..",kCCTextAlignmentLeft,kObjTypeText|kObjTypeActorRight);
		addScript("kt_cha_hibari_1.png","그래 이제 시작이야..\n히바리도 아스카와 같이 최고의 닌자가 될꺼야!",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png","쉿. 일반 학생들이 다니는 교정 안에서는 닌자에 관한 이야기는 하지 않는게 좋아.\n이곳의 닌자 양성 클래스는 일반 학생들에겐 비밀이니까!",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_asuka_1.png","흐응... 비밀이라니...\n조금 긴장하게 되..",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png","히바리, 긴장할 필요 까진 없어.\n언제나처럼 시노비결계 안에서 훈련하기\n때문에 보통 사람들은 볼 수 없으니까.",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_hibari_1.png","응, 알고는 있는데... 아스카짱 고마워\n앞으로나 완전 열심히 할꺼야!",kCCTextAlignmentRight,kObjTypeAll);
		addScript("kt_cha_yagyu_1.png","히바리 위험해!!!\n긴장을 늦추지 마. 적은 남아 있다구!!",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_hibari_1.png","야규! 히바리를 도와주었구나! 고마워 야규짱!!",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_yagyu_1.png","내가 이렇게 항상 지켜봐줄수 없으니\n 전투시엔 조심하도록해..\n교내에도 아직 적이 남아 있으니\n주변의 적을 해치우자.",kCCTextAlignmentLeft,kObjTypeAll);
		
		////////////////////////////////////
		
		beginScene("ko","puzzle3");
		addScript("kt_cha_hibari_1.png","야규, 아까는 도와줘서 고마워.\n히바리가 거치적거리니까\n이대로면 모두한테 폐를 끼치게 돼.",kCCTextAlignmentLeft,kObjTypeText,kBoxBig);
		addScript("kt_cha_yagyu_1.png","히바리는 거치적 거리는 존재 따위가 아니야.\n우리들은 모두 강하다고 인정받았으니까.\n히바리의 강한 점은 내가 제일 잘 알고있고,\n또 모두들 알고 있어.\n그러니까 더 자신에게 자신감을 가져.",kCCTextAlignmentRight,kObjTypeText,kBoxBig);
		addScript("kt_cha_hibari_1.png","야규짱..",kCCTextAlignmentLeft,kObjTypeText,kBoxBig);
		addScript("kt_cha_yagyu_1.png","내.. 내가 말하고 싶은 건 그것뿐이야..\n히바리, 내일은 히바리가 좋아하는 우사네 찻집에 가자.",kCCTextAlignmentRight,kObjTypeText,kBoxBig);
		addScript("kt_cha_hibari_1.png","응!!",kCCTextAlignmentLeft,kObjTypeAll);
		addScript("kt_cha_katsuragi_1.png","이카루가.. 그리고\n야규, 아스카, 히바리도 마침 딱 있었네!",kCCTextAlignmentRight,kObjTypeText,kBoxBig);
		addScript("kt_cha_ikaruga_1.png","카츠라기님. 오늘 훈련은 끝났는데\n집에 안가세요?",kCCTextAlignmentLeft,kObjTypeText,kBoxBig);
		addScript("kt_cha_katsuragi_1.png","어, 응..음... 뭐라고 할까\n소화불량이라고나 할까...\n어쨌든 좀 더 몸을 움직이려고... 응?",kCCTextAlignmentRight,kObjTypeText,kBoxBig);
		addScript("kt_cha_katsuragi_1.png","아앗! 저건 뭐지?",kCCTextAlignmentRight,kObjTypeText,kBoxBig);
		addScript("kt_cha_ikaruga_1.png","소매치기예요!! 꼬마의 과자를 훔쳐가네요!!",kCCTextAlignmentLeft,kObjTypeText,kBoxBig);
		addScript("kt_cha_katsuragi_1.png","아이의 즐거움을 뺏는\n배짱 좋은 녀석이군.\n저런 썩은 자식은 우리들이 혼내줄테다!!",kCCTextAlignmentRight,kObjTypeAll);
		
		////////////////////////////////////
		beginScene("ko","puzzle4");
		addScript("kt_cha_katsuragi_1.png","헤헷, 봤지?\n과자도 돌아왔고 속이 시원하다!\n역시 몸을 움직이는건 최고로 좋아!",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png","카츠 언니 뭔가 흥분한 것 같네요.",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png","뭐어? 그래보여?\n그나저나 아스카 너 이자식, 또 가슴이 커진거 아니야?\n오랜만에 한번 만져볼까? 흐흐흐",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png","저기, 쫌… 이런 곳에서…!\n저..저기봐요\n도둑이 도망치고있어요.",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png","이런 젠장 오랜만에 만질기회였는데..\n뒷산으로 가는군 쫓아가자!",kCCTextAlignmentRight,kObjTypeAll);
		
		////////////////////////////////////
		beginScene("ko","puzzle5");
		addScript("kt_cha_katsuragi_1.png","자 이제 사건종료!",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png","모두 무사해서 다행이예요.\n기념으로 시장에서 재료를 사서 나베 파티하는거 어때요?",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png","나베파티 좋지! 그럼 재료는 내가 사올께..!",kCCTextAlignmentRight,kObjTypeActorLeft|kObjTypeText);
		addScript("kt_cha_katsuragi_1.png","라고 말했는데 왜 줄줄이 따라오는거야?",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","카츠라기님한테 맡기면 라면으로만 꽉찰거라고 아스카님이…",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png","그,그럴리가 없잖아. 대파랑 오뎅이랑 익힌 달걀이랑 여러가지...",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png","응? 상점가의 입구쪽이 뭔가 시끄럽네.",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","음.. 저 불량배들때문에 시장상인들이 문을 닫고있군요.",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","반장으로서, 저 불한당들을 눈감아줄 수는 없지..",kCCTextAlignmentLeft,kObjTypeAll);
		////////////////////////////////////////
		
		beginScene("ko","puzzle6");
		addScript("kt_cha_ikaruga_1.png","불량배들이 없어져서 겨우 가게 문을 다시 열고있네요.\n이걸로 안심하고 재료를 사러 갈 수 있겠네요.",kCCTextAlignmentLeft,kObjTypeAll);
		addScript("","[상인]\n자~ 오늘의 세일 상품 국산특급닭고기 완전 특별세일 선착선 10분에게 모십니다~!",kCCTextAlignmentCenter,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png","오~! 이건 우리들이 나설 차례네!\n저기, 비켜비켜! 그 국산 닭고기 우리거라고!",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","하핫, 국산 닭고기 전부 가져왔습니다. 임무 완료네요… 응?",kCCTextAlignmentLeft,kObjTypeAll);
		addScript("","[불량배]\n야, 너희들! 우리 친구들을 때려눕혔다 이거지!\n이 원한을 백배로 갚아주겠다!",kCCTextAlignmentCenter,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","아까 그 불량배 편들인 것 같은데요…\n여기까지 친히 나와주셨는데 미안하지만, 퇴장시켜드리지요.",kCCTextAlignmentLeft,kObjTypeAll);
		
		/////////////////////////////////////////
		
		beginScene("ko","puzzle7");
		addScript("kt_cha_katsuragi_1.png","또 이 상점가에 얼굴 내밀기만 해봐라!",kCCTextAlignmentRight,kObjTypeAll);
		addScript("","[불량배]\n제기랄! 잊지 않겠어!!",kCCTextAlignmentCenter,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png","어이쿠, 또 오시겠네\n뭐, 그때도 우리들이 아주 혼쭐을 내줄테니까 문제없겠지.",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","카츠라기님은 너무 난폭해지지 않도록 적당하게 해주세요",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_katsuragi_1.png","그래그래, 결과가 좋으면 뭐든지 오케이!라고들 하잖아\n딱딱하게 말하지 말고 응?",kCCTextAlignmentRight,kObjTypeAll);
		
		/////////////////////////////////////////
		
		beginScene("ko","puzzle8");
		addScript("kt_cha_hibari_1.png","아하하 히바리 완전 신나! 이걸 가질수있게 되다니!!",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_yagyu_1.png","다행이야. 이 마직막 남은 토깽이 인형을 살수있어서..",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_hibari_1.png","응 야규짱!\n한정판이라 더이상 구할수없는 건데. 너무 기뻐!",kCCTextAlignmentLeft,kObjTypeAll);
		addScript("","[불량배]\n이자식들! 그 토깽이 인형 내놔!",kCCTextAlignmentCenter,kObjTypeText);
		addScript("kt_cha_yagyu_1.png","왜 우리가 토깽이 인형을 줘야하지?",kCCTextAlignmentRight,kObjTypeAll);
		addScript("","[불량배]\n왜긴 왜야!\n그 토깽이 인형은 내꺼니까!",kCCTextAlignmentCenter,kObjTypeText);
		addScript("kt_cha_yagyu_1.png","무슨말 하는거야? 이건 히바리꺼야.",kCCTextAlignmentRight,kObjTypeAll);
		addScript("","[불량배]\n흥!\n그렇다면 힘으로 뺏는수밖에..!",kCCTextAlignmentCenter,kObjTypeText);
		
		//////////////////////////////////////////
		
		beginScene("ko","puzzle9");
		addScript("kt_cha_yagyu_1.png","포기해. 이 토깽이 인형은 내가 확실히 줄서서 산거라구.\n니가 정말 갖고 싶으면 더 빨리 줄을 서면 살 수 있었겠지.",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_hibari_1.png","다음부터는 줄 잘 서서 사도록 해.\n토깽이 인형이 좋아도 사람 물건을 뺏거나 그러면 안되는거야.",kCCTextAlignmentLeft,kObjTypeAll);
		addScript("","[불량배]\n그렇네. 내가 나빴어. 다음부터는 확실히 줄설게.\n하지만 다음의 한정품을 내가 손에 넣어도 절대 양보 안하는 건 기억해둬!",kCCTextAlignmentCenter,kObjTypeText);
		addScript("kt_cha_hibari_1.png","그래! 나도 양보 안할테니까. 내가 바라는바야!\n야규, 고마워.\n난 사실 같이 토깽이 좋아하는 사람이랑 싸우는 거 싫었어… ",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_yagyu_1.png","히바리는 누구한테나 친절하구나. 역시 나의 히바리야.",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_hibari_1.png","뭐? 야규 지금 뭐라고 했어?",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_yagyu_1.png","아,아무것도 아니야!.\n후..훈련이나 하러가자.",kCCTextAlignmentRight,kObjTypeAll);
		
		/////////////////////////////////////////////
		
		beginScene("ko","puzzle10");
		addScript("kt_cha_ikaruga_1.png","선생님, 급한 소집인데요, 오늘은 어떤 지령인가요?",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","음. 다른 소집된팀들과 시합해서 우리들이 얼마나 얼마나 성장했는지를 증명하라는거군요.",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png","와…확실히 본적은 없는 애들이 몇 명 있는 것 같아…\n우리들 이외에 몇 팀이나 있는거군요.",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_asuka_1.png","… 뭐! 이기지 못하는 팀은 오늘 당장 해산!?\n그건 너무 심한 거 아닌가요…",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","우리들의 실력을 정말로 시험받는 때군요.\n선생님의 은혜에 보답하기 위해서라도 이 시합 질 수 없어요!",kCCTextAlignmentRight,kObjTypeAll);
		
		//////////////////////////////////////////
		beginScene("ko","puzzle11");
		addScript("kt_cha_asuka_1.png","임무달성!\n순조롭군 순조로워! 아직도 더 할 수 있어요!",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","앞으로 8시합 이기지 않으면 우리들 팀도 해산되어버려요.\n이것도 닌자의 어려움이죠…. 질 수 없죠.",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","그러면 다음 시합도 긴장풀지 말고 합시다!",kCCTextAlignmentRight,kObjTypeAll);

		////////////////////////////////////
		beginScene("ko","puzzle12");
		addScript("kt_cha_hibari_1.png","아, 선생님, 저요, 앞으로 싸울 상대편 애의 시합을 보고왔어요.",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_hibari_1.png","뭔가 굉장히 강해보여서, 좀 무서워져서...",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","이대로 이기고 나가면, 히지카타님의 팀이랑 만나게 되네요…\n히지카타님은 실력자이기는 하지만 이기지 못하는 적은 아닙니다.",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","앞으로 4시합을 이기면, 우리들의 승리… 승부입니다!",kCCTextAlignmentRight,kObjTypeAll);
		
		
		
		////////////////////////////////////
		beginScene("ko","puzzle13");
		addScript("kt_cha_ikaruga_1.png","모의 시합은 여기서끝이 난 듯 하네요",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png","어려운 시합이었네요… 뭔가 불쌍하다.",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","불쌍하다고?",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png","아… 진 팀의 아이들이예요.\n최선을 다해서 자신을 단련시켜왔는데…",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","그럴지도 모르네요…\n닌자의 길은 그렇게 힘든 길이랍니다.\n우리들은 그 사람들 몫까지 열심히..!",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png","..그렇네요. 저 더욱 더 강해질래요.\n그리고 최강의 닌자가 될거야…!\n그러기 위해선 역시.. 훈련입니다! 훈련!",kCCTextAlignmentLeft,kObjTypeAll);
		
		////////////////////////////////////
		beginScene("ko","puzzle14");
		addScript("kt_cha_asuka_1.png","오랜만에 일찍 마쳤네. 어서가서 쉬어야 겠어. 휴식이다 휴식~!",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_asuka_1.png","으악",kCCTextAlignmentLeft,kObjTypeAll);
		addScript("","[???]\n앗, 미안해요, 아가씨.\n급해가지고 말이야… \n그럼 안녕! 미안해!",kCCTextAlignmentCenter,kObjTypeText);
		addScript("kt_cha_asuka_1.png","방금 그 사람 어딘가에서 본 듯한…\n아니! 내 지갑이 없어!\n앗! 저기 있다!뒷산으로 도망치잖아!",kCCTextAlignmentLeft,kObjTypeText);
		
		////////////////////////////////////
		beginScene("ko","puzzle15");
		addScript("kt_cha_asuka_1.png","이런 다잡았는데 놓쳤어!\n학학….아까 그 사람 어디 갔을까?",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_homura_1.png","응? 너는 아스카 아냐 그렇게 당황해서 어떻게 된 일이야?",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png","아, 호무라! 사실은 아까 지나가면서 지갑을 누가 훔쳐가서…",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_homura_1.png","뭐야, 또 잃어버린거야?\n도둑은 번화가로 도망간것 같아.\n지갑을 다시 되돌리고 싶다면 상대해주겠어… 가자.",kCCTextAlignmentRight,kObjTypeAll);
		
		
		////////////////////////////////////
		beginScene("ko","puzzle16");
		addScript("kt_cha_homura_1.png","… 이거봐 또 잃어버리지마. 다음은 더 이상 없으니까",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png","고, 고마워 호무라\n전에 지갑 잃어버렸을 때도 되찾아줬었지. 미안해",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_homura_1.png","쉽게 사과하지마. 그리고 나는 그 선생님을 찾고 있었을 분이고…\n잠깐 아무래도 또 마귀가 들어온 모양이네. 먼저 정리해야겠어.",kCCTextAlignmentRight,kObjTypeAll);
		
		
		////////////////////////////////////
		beginScene("ko","puzzle17");
		addScript("kt_cha_homura_1.png","흥, 이정도였군. 보람이 없군.\n그건 그렇고 너 내가 볼 때마다 지갑을 잃어버리고 있네…\n설마 너 바보니?",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png","뭐, 잃어버리고 싶어서 잃어버린 게 아니라구!\n하지만 지갑 다시 찾는 거 도와줘서 고마워.",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_homura_1.png","인사치레는 필요없어.\n너는 더 자신을 엄격하게 훈련시킬필요가 있을 것 같군.\n그럼이만~",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_asuka_1.png","어, 호무라…! …가버렸네.\n사이좋게 지내고 싶은데\n역시 실력을 키우는 수밖에. 훈련이다 훈련!",kCCTextAlignmentLeft,kObjTypeAll);
		
		
		////////////////////////////////////
		beginScene("ko","puzzle18");
		addScript("kt_cha_ikaruga_1.png","긴급지령이라고 듣고 급하게 여기까지 왔는데…\n슬슬 이번 임무내용을 알려주시겠어요?",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_ikaruga_1.png","선생님의 동료가 적에서부터 물자를 뺏은 다음에 이 부근에서 다시 뺏겼다고요…?",kCCTextAlignmentRight,kObjTypeText);
		addScript("kt_cha_yomi_1.png","그래서 우리들이 다시 한 번 물자를 가지고 오는거죠?\n뺏고 또 다시 뺏긴 건 좀 한심하네요…\n어쨌든 싸우는 일은 빨리 정리하는게 우선이죠",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_asuka_1.png","요미님! 지금 케이스를 가진 닌자가 뒷길을 달려갔어요!",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_yomi_1.png","아이고. 보통 이 부근에서 닌자는 전혀 눈에 띄지 않는데…\n저런, 알기 쉬운 사람같으니! 금방 쫓아갈께요.",kCCTextAlignmentLeft,kObjTypeAll);
		
		////////////////////////////////////
		beginScene("ko","puzzle19");

		addScript("kt_cha_asuka_1.png","도망가버렸어요…\n저 분들 케이스에 대해서 알고 있었던 것 같은데…\n이 부근은 복잡하게 얽혀있어서 어디에 숨었는지…",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_yomi_1.png","우후훗, 안심해주세요.\n이곳은 우리들의 학교 해비여자학원의 구역입니다.\n들어온 외부인이 있으면 냄새로 바로 알 수 있죠.\n지리에 대한 지식과 후각은 나에게 맡겨라죠. 절대 놓치지 않을 거예요!",kCCTextAlignmentLeft,kObjTypeAll);
		
		////////////////////////////////////
		beginScene("ko","puzzle20");
		
		addScript("kt_cha_yomi_1.png","훗, 두 번 다시 얼씬 거리지마라고! 라고나 할까\n그래서 덜빠진 공무원이 떨어뜨린 물자가 들어간 케이스가 이거죠?\n도대체 안에는 뭐가 들어있는건가요?",kCCTextAlignmentLeft,kObjTypeText);
		addScript("kt_cha_asuka_1.png","…요미님 기다려주세요.\n…아직도 다른 적이 있습니다! 쳐부숩시다!",kCCTextAlignmentLeft,kObjTypeAll);
		
	}
	
	void beginScene(string _lang,string _storyID){
		string cliLang = MyLocal::sharedInstance()->getLocalCode()->getCString();
		CCLOG("Scene %s,",this->storyID.c_str());
		if(this->storyID == _storyID && _lang == cliLang){
			isAdd=true;
		}else{
			isAdd=false;
		}
	}
	
	void addTitle(string title,ObjType isClear=kObjTypeAll){
		if(isAdd){
			Json::Value scriptInfo;
			scriptInfo["type"]=kScriptTypeTitle;
			scriptInfo["title"]=title;
			scriptInfo["clear"]=(int)isClear;
			storyData.append(scriptInfo);
		}
	}
	
	void addSpot(string stringdata,ObjType isClear=kObjTypeAll){
		if(isAdd){
			Json::Value scriptInfo;
			scriptInfo["type"]=kScriptTypeSpot;
			scriptInfo["stringdata"]=stringdata;
			scriptInfo["clear"]=(int)isClear;
			storyData.append(scriptInfo);
		}
	}
	void addScript(string image,string script,CCTextAlignment align,int isClear=kObjTypeNone,BoxType bt=kBoxBig){
		 CCLOG("%s\t%s",image.c_str(),script.c_str());
		if(isAdd){
           // CCLOG("add script %s",script.c_str());
			Json::Value scriptInfo;
			scriptInfo["type"]=kScriptTypeTalk;
			scriptInfo["image"]=image;
			scriptInfo["script"]=script;
			scriptInfo["align"]=(int)align;
			
			scriptInfo["box"]=(int)bt;
			scriptInfo["clear"]=(int)isClear;
			storyData.append(scriptInfo);
		}
	}
		
	void scriptTypeTalk(Json::Value script){
        
        TRACE();
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
        TRACE();
		
		if(script["image"].asString()!=""){
			CCSprite* actor=nullptr;
			CCArray* ch = back->getChildren();
            TRACE();
			bool isOldObj = false;
			if(ch){
				for(int i=0;i<ch->count();i++){
					CCNode* n = (CCNode*)ch->objectAtIndex(i);
					if(n->getStringData()==script["image"].asString()){
						actor = (CCSprite*)n;
						isOldObj=true;
					}
				}
			}
            TRACE();
			if(!actor){
				actor = CCSprite::create(script["image"].asString().c_str());
				actor->setStringData(script["image"].asString().c_str());
				back->addChild(actor, 10);
				CCLOG("addChild actor %s", actor->getStringData().c_str());
			}
			
			
            TRACE();
			if(!isOldObj){
				if(script["align"].asInt()==kCCTextAlignmentLeft){
					actor->setAnchorPoint(ccp(0,0));
					actor->setPosition(ccp(240-240*screen_scale_x-actor->getContentSize().width , 160-160*screen_scale_y));
					actor->setTag(kObjTypeActorLeft);
				}
				else if(script["align"].asInt()==kCCTextAlignmentRight){
					actor->setAnchorPoint(ccp(1,0));
					actor->setPosition(ccp(240+240*screen_scale_x+actor->getContentSize().width , 160-160*screen_scale_y));
					actor->setTag(kObjTypeActorRight);
				}else{
					actor->setAnchorPoint(ccp(1,0));
					actor->setPosition(ccp(240, 160-160*screen_scale_y-500));
					actor->setTag(kObjTypeActorCenter);
				}
				
                TRACE();
				
				back->addChild(KSGradualValue<float>::create(0.f,1.f,0.3f,[actor,screen_scale_x,screen_scale_y,script](float t){
					if(script["align"].asInt()==kCCTextAlignmentLeft)
						actor->setPositionX(240-240*screen_scale_x-actor->getContentSize().width + actor->getContentSize().width*2.f/3.f*t);
					else if(script["align"].asInt()==kCCTextAlignmentRight)
						actor->setPositionX(240+240*screen_scale_x+actor->getContentSize().width - actor->getContentSize().width*2.f/3.f*t);
					else
						actor->setPosition(ccp(240, 160-160*screen_scale_y-500+500*t));
					
				},[actor](float t){
					
					CCLog("action pos is %f",actor->getPositionX());
					
				}));
			}
			
            TRACE();
		}
		
		
		TypingBox* typing_box;
		
        TRACE();
        CCLOG("test1 %d",this->storySeq);
		if(script["box"].asInt()==kBoxBig){
			if(script["align"].asInt()==kCCTextAlignmentLeft){
				typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(241, 78));
			}else if(script["align"].asInt()==kCCTextAlignmentRight){
				typing_box = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 60), ccp(239, 96));
			}else{
				typing_box = TypingBox::create(-9999, "talk_mentbox.png", CCRectMake(0, 0, 35, 35), CCRectMake(12, 12, 23-12, 23-12), CCRectMake(7, 10, 23, 64), CCSizeMake(240, 110), ccp(239, 96));
				
//				TypeingBox::create(int t_touch_priority, string t_box_filename, cocos2d::CCRect t_box_9_out, cocos2d::CCRect t_box_9_in, cocos2d::CCRect t_box_in_label, cocos2d::CCSize t_box_in_size, cocos2d::CCPoint t_box_position)
			}
		}else{
			typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(241, 78));
		}
        TRACE();
		typing_box->setTag(kObjTypeText);
		back->addChild(typing_box, 11);
		
        TRACE();
        CCLOG("%s",script.toStyledString().c_str());
		typing_box->startTyping(script["script"].asString().c_str(),[this](){

            this->addChild(KSTimer::create(0.1,[this](){
                TRACE();
                CCLOG("test2-0 %d",this->storySeq);
                this->clearScript();
                CCLOG("test2-1 %d",this->storySeq);
                TRACE();
                
                CCLOG("test3-1 %d",this->storySeq);
                this->playScript();
                CCLOG("test3-2 %d",this->storySeq);
                TRACE();
                TRACE();
                CCLOG("test2-2 %d",this->storySeq);

            }));
           		});
        TRACE();
		typing_box->setTouchOffScrollAndButton();
	
	}
	
	void scriptTypeTitle(Json::Value script){
		
		StyledLabelTTF* titlelbl = StyledLabelTTF::create(script["title"].asString().c_str(), mySGD->getFont().c_str(), 20, 000, StyledAlignment::kCenterAlignment);
		titlelbl->setPosition(ccp(240+480,160));
		titlelbl->setTag(kObjTypeTitle);
		back->addChild(titlelbl);
		
		
		addChild(KSGradualValue<float>::create(0, 460, 0.5, [&titlelbl,this](float a){
			titlelbl->setPositionX(240+480-a);
		}, [&titlelbl,this](int a){
			addChild(KSGradualValue<float>::create(0, 40, 1.5, [&titlelbl,this](float a){
				titlelbl->setPositionX(240+480-460-a);
			}, [&titlelbl,this](int a){
				addChild(KSGradualValue<float>::create(0, 460, 0.5, [&titlelbl,this](float a){
					titlelbl->setPositionX(240+480-460-40-titlelbl->getPositionX()-a);
				}, [&titlelbl,this](int a){
					this->clearScript();
				}));
			}));
		}));
	}
	
	void scriptTypeSpot(Json::Value script){
		CCNode* obj =StoryLayer::findObject(CCDirector::sharedDirector()->getRunningScene(),script["stringdata"].asString());
		CCPoint pos =  obj->getParent()->convertToWorldSpace(obj->getPosition());
		if(obj)obj->runAction(CCBlink::create(0.5f, 2));
		
		CCDrawNode * dot = CCDrawNode::create();
		dot->drawDot(pos,obj->getContentSize().width/2.f, ccc4f (1, 1, 1, 1));
		back->addChild(dot);
		dot->setBlendFunc(ccBlendFunc{GL_DST_COLOR, GL_ONE});
		dot->setAnchorPoint(ccp(0.5f,0.5f));
		dot->setTag(kObjTypeSpot);
		
		addChild(KSTimer::create(0.3,[this](){
			this->clearScript();
			this->playScript();
		}));
	}

	
	int m_sss;
	void playScript(){
        TRACE();
        CCLOG("test3-1 %d",this->storySeq);
        TRACE();
		if(storySeq>=storyData.size()){
			
            TRACE();
			CCLOG("clear last1 %s",this->storyID.c_str());
            TRACE();
			addChild(KSGradualValue<float>::create(0, 50, 0.5, [this](int a){
                CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
                float screen_scale_x = screen_size.width/screen_size.height/1.5f;
                if(screen_scale_x < 1.f)
                    screen_scale_x = 1.f;
                float ui_top = myDSH->ui_top;
                float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
                
                
				bottom->setPositionY(-a);
				top->setPositionY(screen_scale_y*(320+a));
				black->setOpacity(150-a*3.f);
			}, [this](int a){
				CCLOG("all finish");
				if(callbackFunc)callbackFunc();
				this->removeFromParent();
				
			}));
            TRACE();
			CCLOG("clear last2 %s",this->storyID.c_str());
            TRACE();
			return;
		}
		
		
        CCLOG("get script");
		Json::Value script = storyData[storySeq];
		
		
		if(script["type"].asInt()==kScriptTypeTalk){
			scriptTypeTalk(script);
		}else if(script["type"].asInt()==kScriptTypeTitle){
			scriptTypeTitle(script);
		}else if(script["type"].asInt()==kScriptTypeSpot){
			scriptTypeSpot(script);
		}
	}
	

	void clearScript(){
        int nowSeq = this->storySeq;
        CCLOG("test4-1 %d",nowSeq);
		Json::Value script = storyData[nowSeq];
        CCLOG("test4-2 %d",nowSeq);
        
        TRACE();
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
        CCLOG("test4-3 %d",nowSeq);
        TRACE();
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCArray *cr = back->getChildren();
		
        CCLOG("test4-4 %d",nowSeq);
		
        TRACE();
		for(int i=0;i<cr->count();i++){
			CCNode *obj = (CCNode*)cr->objectAtIndex(i);
			
            TRACE();
			if(obj->getTag()==kObjTypeText && script["clear"].asInt()&kObjTypeText){
				CCLOG("remove text");
                CCLOG("test4-5 %d",nowSeq);
				obj->removeFromParent();
				
			}else if((obj->getTag()==kObjTypeActorLeft && script["clear"].asInt()&kObjTypeActorLeft) || (obj->getTag()==kObjTypeActorRight && script["clear"].asInt()&kObjTypeActorRight)){
                
                
                CCLOG("test4-6 %d",nowSeq);
				
				CCLOG("move actor %s", obj->getStringData().c_str());
				back->addChild(KSGradualValue<float>::create(1.f,0.f,0.3f,[this,obj,screen_scale_x](float t){
					CCNode *myobj = obj;
					if(myobj->getAnchorPoint().x==0.f){
						myobj->setPositionX(240-240*screen_scale_x-obj->getContentSize().width + obj->getContentSize().width*2.f/3.f*t);
					}else{
						myobj->setPositionX(240+240*screen_scale_x+obj->getContentSize().width - obj->getContentSize().width*2.f/3.f*t);
					}
					
				},[this,obj](float t){
						CCNode *myobj = obj;
						CCLOG("remove actor %s",myobj->getStringData().c_str());
						myobj->removeFromParent();
				}));
			}else if(obj->getTag()==kObjTypeTitle && script["clear"].asInt()&kObjTypeTitle){
                
                CCLOG("test4-7 %d",nowSeq);
				CCLOG("remove title");
				obj->removeFromParent();
			}else if(obj->getTag()==kObjTypeSpot && script["clear"].asInt()&kObjTypeSpot){
				
                CCLOG("test4-8 %d",nowSeq);
                CCLOG("remove spot");
				obj->removeFromParent();
			}
            
            TRACE();
			
		}
        
        CCLOG("test4-9 %d",nowSeq);
        this->storySeq++;
        TRACE();
		
	}
	
	static CCNode* findObject(CCNode* obj,string tagname){
		if(!obj || tagname=="")return nullptr;
		
		if(obj->getStringData()==tagname){
			return obj;
		}
		
		CCArray* children = obj->getChildren();
		if(children==nullptr)return nullptr;
		
		for(int i=0;i<children->count();i++){
			CCNode* child = (CCNode*)children->objectAtIndex(i);
			CCNode* objs = StoryLayer::findObject(child,tagname);
			if(objs!=nullptr)return objs;
		}
		
		return nullptr;
	}
	
};


#endif /* defined(__DGproto__ScenarioScene__) */
