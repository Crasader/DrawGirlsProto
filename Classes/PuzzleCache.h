//
//  PuzzleCache.h
//  DGproto
//
//  Created by LitQoo on 13. 10. 31..
//
//

#ifndef __DGproto__PuzzleCache__
#define __DGproto__PuzzleCache__

#include <iostream>
#include <vector>
#include "cocos2d.h"
#include <thread>
using namespace cocos2d;
using namespace std;

enum PuzzleCachePieceType {
	kPuzzleCachePieceType_left = 1000,
	kPuzzleCachePieceType_right,
	kPuzzleCachePieceType_top,
	kPuzzleCachePieceType_bottom
	};
class PuzzleImage : public CCImage {
	
private:
	float m_posX;
	float m_posY;
	int m_puzzleNo;
	int m_pieceNo;
	string m_puzzleKey;
	bool m_isLoaded;
	
public:
	PuzzleImage(){
		m_isLoaded=false;
		m_puzzleNo=0;
	}
	
	bool isLoaded(){
		return m_isLoaded;
	}
	void setLoaded(bool isLoaded){
		m_isLoaded=isLoaded;
	}
	void setCutPoint(float x,float y){
		m_posX=x;
		m_posY=y;
	}
	
	CCPoint getCutPoint(){
		return CCPointMake(m_posX, m_posY);
	}
	
	int getPieceNo(){
		return m_pieceNo;
	}
	int getPuzzleNo(){
		return m_puzzleNo;
	}
	
	void setPuzzleNo(int puzzleNo){
		m_puzzleNo = puzzleNo;
	}
	
	void setPieceNo(int pieceNo){
		m_pieceNo = pieceNo;
	}
	
	void setPuzzleKey(string puzzlekey){
		m_puzzleKey = puzzlekey;
	}
	
	string getPuzzleKey(){
		return m_puzzleKey;
	}
	
	CCTexture2D* makeTexture(){
		CCTexture2D *test2 = new CCTexture2D;
		test2->initWithImage(this);
		test2->autorelease();
		return test2;
	}
};

class PuzzleCache : public CCObject{
private:
	//캐쉬
	map<string,PuzzleImage*> m_textureList;
	
	//로딩한 퍼즐목록
	list<PuzzleImage*> m_loadingPuzzleList;

	//퍼즐상태
	
	map<int,string> m_puzzleState;
	
	bool isLockedTextureList;
	bool isLockedLoadingPuzzleList;
	
	std::function<void(PuzzleImage*)> m_callbackfunc;
	
	int m_currentLoadPuzzleNo;
	bool m_useTread;
public:
	
	PuzzleCache(){
		isLockedLoadingPuzzleList=false;
		m_currentLoadPuzzleNo = 0;
		m_useTread=false;
	}
	
	//퍼즐로드 - 기본방식
	void loadImage(int puzzleNo){
		
		if(m_puzzleState[puzzleNo]=="loaded" || m_puzzleState[puzzleNo]=="loading"){
			callbackLoadedImage(puzzleNo);
			m_currentLoadPuzzleNo=0;
			return;
		}
		
		//퍼즐 원본 로드
		CCImage *puzzleImg = new CCImage;
		puzzleImg->initWithImageFile("puzzle1.png");

		CCImage *thumbImg = new CCImage;
		thumbImg->initWithImageFile("puzzle1_thumbnail.png");
		

		
		loadImage(puzzleNo,puzzleImg,thumbImg);
	}
	
	void cancelLoadingImage(int puzzleNo){
		m_currentLoadPuzzleNo=0;
	}

//	void setLockedTextureList(bool locked){
//		isLockedTextureList = locked;
//	}
	
	void setLockedLoadingPuzzleList(bool locked){
		//CCLog("setLockedLoadingPuzzleList %d",locked);
		isLockedLoadingPuzzleList = locked;
	}
	
//	void waitForTextureList(bool locked){
//		while(isLockedTextureList==true){}
//		setLockedTextureList(locked);
//	}
	
	void waitForLoadingPuzzleList(bool locked){
		//CCLog("waitForLoadPuzzleList %d",locked);
		while(isLockedLoadingPuzzleList==true){}
		setLockedLoadingPuzzleList(locked);
	}
	
	void addTextureList(string key,PuzzleImage* texture){
		texture->retain();
		this->m_textureList.insert(pair<string,PuzzleImage*>(key,texture));
	}


	void addLoadingPuzzleList(PuzzleImage* image){
	
		if(PuzzleCache::getInstance()->m_useTread){
			image->retain();
			waitForLoadingPuzzleList(true);
			this->m_loadingPuzzleList.push_back(image);
			setLockedLoadingPuzzleList(false);
		}else{
			image->retain();
			this->m_textureList.insert(pair<string,PuzzleImage*>(image->getPuzzleKey(),image));
		}
	}

	struct PuzzlePoint{
		int x;
		int y;
	};
	
	static PuzzleCache* getInstance()
	{
		static PuzzleCache* _ins = 0;
		if(_ins == 0)
			_ins = new PuzzleCache();
		return _ins;
	}
	void loadingPuzzle(float dt){
		//m_callbackfunc(NULL);
		
		
		list<PuzzleImage*>::iterator it;
		
		
		for (it=m_loadingPuzzleList.begin();it!=m_loadingPuzzleList.end();it++) {
			PuzzleImage* texture = (*it);
			if(m_loadingPuzzleList.size()==0)break;
			if(texture->getPuzzleKey()=="COMPLETE"){
				//CCLog("loadingPuzzle:COMPLETE");
				//퍼즐상태에 완료표시 할것
				texture->release();
				if(m_loadingPuzzleList.size()<=0){
					CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(PuzzleCache::loadingPuzzle), PuzzleCache::getInstance());
				}
				
			}else if(texture->getPuzzleKey()!=""){
				//CCLog("loadingPuzzle:callfunc");
				map<string,PuzzleImage*>::iterator it2;
				it2 = m_textureList.find(texture->getPuzzleKey());
				
				if(it2==m_textureList.end()){
					this->addTextureList(texture->getPuzzleKey(), texture);
					texture->release();
				}
				
				if(m_callbackfunc)m_callbackfunc(texture);
			
			}else{
				//CCLog("loadingPuzzle:else");
			}
		}
		
		if(m_loadingPuzzleList.size()>0){
			waitForLoadingPuzzleList(true);
			m_loadingPuzzleList.clear();
			setLockedLoadingPuzzleList(false);
		}

	}
	void loadImageWithCallback(int puzzleNo, std::function<void(PuzzleImage*)> func){
		m_callbackfunc = func;
		//여기서 m_loadingPuzzleList에 complete있으면 지워주기
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(PuzzleCache::loadingPuzzle), PuzzleCache::getInstance(), 0.f, kCCRepeatForever, 0.f, false);

		std::thread puzzleThread( [puzzleNo,func, this] ()
		{
			
			PuzzleCache::getInstance()->m_useTread = true;
			PuzzleCache::getInstance()->loadImage(puzzleNo);
			
			PuzzleImage* complete = new PuzzleImage();
			complete->setPuzzleKey("COMPLETE");
			PuzzleCache::getInstance()->addLoadingPuzzleList(complete);
			complete->release();
			
			PuzzleCache::getInstance()->m_useTread = false;
		} );
		puzzleThread.detach();
	}
	
	
	bool checkCancel(int loadingPuzzleNo){
		if(loadingPuzzleNo!=m_currentLoadPuzzleNo){
			m_puzzleState[loadingPuzzleNo]="cancel";

			return true;
		}
		return false;
	}
	
	void callbackLoadedImage(int puzzleNo){
		map<string,PuzzleImage*>::iterator it;
		for (it=m_textureList.begin();it!=m_textureList.end();it++) {
			PuzzleImage* texture = (PuzzleImage*)it->second;
			if(texture->getPuzzleNo()==puzzleNo){
				addLoadingPuzzleList(texture);
			}
		}
	}

	//퍼즐로드 - 원본 ccimage를 정해줄수있음
	void loadImage(int puzzleNo, CCImage* puzzleImg, CCImage* thumbImg){
		m_currentLoadPuzzleNo = puzzleNo;
		//중복검사
		
		if(m_puzzleState[puzzleNo]=="loaded" || m_puzzleState[puzzleNo]=="loading"){
			callbackLoadedImage(puzzleNo);
			m_currentLoadPuzzleNo=0;
			return;
		}
		
		m_puzzleState[puzzleNo]="loading";
		
		//틀만들기
		{//left
			CCImage *puzzleBoarder = new CCImage;
			puzzleBoarder->initWithImageFile("puzzle_stencli_1_left.png");
			
			PuzzleImage *texture = PuzzleCache::getInstance()->getTextureOriginByStensil(puzzleImg, puzzleBoarder, {(int)(puzzleBoarder->getWidth()/2),(int)(puzzleImg->getHeight()/2)},true);
			ostringstream oss;
			oss << "puzzle"<<puzzleNo<<"_left";
			string key=oss.str().c_str();
			
			texture->setPuzzleNo(puzzleNo);
			texture->setPieceNo(kPuzzleCachePieceType_left);
			texture->setPuzzleKey(key);
			
			addLoadingPuzzleList(texture);

			
			puzzleBoarder->release();
		}
		
		if(checkCancel(puzzleNo))return;
		
		{//right
			CCImage *puzzleBoarder = new CCImage;
			puzzleBoarder->initWithImageFile("puzzle_stencli_1_right.png");
			
			PuzzleImage *texture = PuzzleCache::getInstance()->getTextureOriginByStensil(puzzleImg, puzzleBoarder, {(int)(puzzleImg->getWidth()-puzzleBoarder->getWidth()/2),(int)(puzzleImg->getHeight()/2)},true);
			
			ostringstream oss;
			oss << "puzzle"<<puzzleNo<<"_right";
			string key=oss.str().c_str();
			
			texture->setPuzzleNo(puzzleNo);
			texture->setPieceNo(kPuzzleCachePieceType_right);
			texture->setPuzzleKey(key);
			
			addLoadingPuzzleList(texture);

			
			puzzleBoarder->release();
		}
		
		if(checkCancel(puzzleNo))return;
		
		{//top
			CCImage *puzzleBoarder = new CCImage;
			puzzleBoarder->initWithImageFile("puzzle_stencli_1_top.png");
			
			PuzzleImage *texture = PuzzleCache::getInstance()->getTextureOriginByStensil(puzzleImg, puzzleBoarder, {(int)(puzzleImg->getWidth()/2),(int)(puzzleImg->getHeight()-puzzleBoarder->getHeight()/2)},true);

			ostringstream oss;
			oss << "puzzle"<<puzzleNo<<"_top";
			string key=oss.str().c_str();
			
			texture->setPuzzleNo(puzzleNo);
			texture->setPieceNo(kPuzzleCachePieceType_top);
			texture->setPuzzleKey(key);
			
			addLoadingPuzzleList(texture);

			
			puzzleBoarder->release();
		}
		
		if(checkCancel(puzzleNo))return;
		
		{//bottom
			CCImage *puzzleBoarder = new CCImage;
			puzzleBoarder->initWithImageFile("puzzle_stencli_1_bottom.png");
			
			PuzzleImage *texture = PuzzleCache::getInstance()->getTextureOriginByStensil(puzzleImg, puzzleBoarder, {(int)(puzzleImg->getWidth()/2),(int)(puzzleBoarder->getHeight()/2)},true);
			
			ostringstream oss;
			oss << "puzzle"<<puzzleNo<<"_bottom";
			string key=oss.str().c_str();
			
			texture->setPuzzleNo(puzzleNo);
			texture->setPieceNo(kPuzzleCachePieceType_bottom);
			texture->setPuzzleKey(key);
			
			addLoadingPuzzleList(texture);
			
			puzzleBoarder->release();
		}
		
		
		//피스만들기
		
		CCImage *wPiece = new CCImage;
		wPiece->initWithImageFile("puzzle_stencli_1_pw.png");
		
		CCImage *hPiece = new CCImage;
		hPiece->initWithImageFile("puzzle_stencli_1_ph.png");
		
		int ycnt=0;
		int xcnt=0;
		int pw=1;
		for(int i=1;i<25;i++){
			if(checkCancel(puzzleNo))return;
			
			pw++;
			if(xcnt==6){pw++; ycnt++; xcnt=0;}
			CCImage *_img;
			if(pw%2)_img=wPiece;
			else _img=hPiece;
			
			
			
		//퍼즐 이미지
		
			PuzzleImage *pImg = PuzzleCache::getInstance()->getTextureOriginByStensil(puzzleImg, _img, {(int)(xcnt*68.5f*2+176),(int)(ycnt*68.5f*2+134)},false);
			
			ostringstream oss;
			oss << "puzzle"<<puzzleNo<<"_piece"<<i;
			string key=oss.str().c_str();
			
			pImg->setPuzzleNo(puzzleNo);
			pImg->setPieceNo(i);
			pImg->setPuzzleKey(key);

		
		
		//썸네일 이미지
		
			PuzzleImage *tImg = PuzzleCache::getInstance()->getTextureOriginByStensil(thumbImg, _img, {(int)(xcnt*204+102),(int)(ycnt*204+102)},false);
			tImg->setCutPoint(pImg->getCutPoint().x,pImg->getCutPoint().y);
			ostringstream oss2;
			oss2 << "puzzle"<<puzzleNo<<"_thumbnail"<<i;
			string key2=oss2.str().c_str();
			
			tImg->setPuzzleNo(puzzleNo);
			tImg->setPieceNo(i+100);
			tImg->setPuzzleKey(key2);
			
			addLoadingPuzzleList(tImg);
			
			addLoadingPuzzleList(pImg);
			
			xcnt++;
		}
		
		puzzleImg->release();
		wPiece->release();
		hPiece->release();
		
		m_puzzleState[puzzleNo]="loaded";
		m_currentLoadPuzzleNo=0;
	}
	
	//퍼즐이미지 가져오기
	PuzzleImage* getImage(string key){
		map<string,PuzzleImage*>::iterator it;
		it = m_textureList.find(key);
		
		if(it==m_textureList.end()) return NULL;
		
		return (PuzzleImage*)it->second;
	}
	
	//캐쉬모두지우기
	void removeUnuseImage(){
		{ // texture
			map<string,PuzzleImage*>::iterator it;
			for (it=m_textureList.begin();it!=m_textureList.end();it++) {
				PuzzleImage* texture = (PuzzleImage*)it->second;
				texture->release();
				m_textureList.erase(it--);
			}
		}
		
		m_loadingPuzzleList.clear();
	}
	
	void removeUnusePuzzle(int puzzleNo){
		{ // texture
			map<string,PuzzleImage*>::iterator it;
			for (it=m_textureList.begin();it!=m_textureList.end();it++) {
				PuzzleImage* texture = (PuzzleImage*)it->second;
				if(texture->getPuzzleNo()==puzzleNo){
					texture->release();
					m_textureList.erase(it--);
				}
			}
		}
		
		m_loadingPuzzleList.clear();
	}
	
	//키목록보여주기
	void logTextureKeys(){
		{ // texture
			map<string,PuzzleImage*>::iterator it;
			for (it=m_textureList.begin();it!=m_textureList.end();it++) {
				CCLog("PuzzleCache key : %s",((string)(it->first)).c_str());
			}
		}
	}
	
	//origin이미지에서 stencil이미지 만큼 잘라오기
	PuzzleImage* getTextureOriginByStensil(CCImage* origin, CCImage* stencil,PuzzlePoint cutPointInOrigin,bool isGLcoordinate){
		unsigned char* originData = origin->getData();
		PuzzlePoint originSize = {origin->getWidth(),origin->getHeight()};
		
		
		unsigned char* stencilData = stencil->getData();
		PuzzlePoint stencilSize = {stencil->getWidth(),stencil->getHeight()};
		PuzzlePoint stencilSizeHalf = {stencilSize.x/2,stencilSize.y/2};
		
		PuzzleImage* newImg = new PuzzleImage();
		newImg->initWithImageData(stencil->getData(),
								  stencil->getDataLen(),
								  CCImage::kFmtRawData,
								  stencil->getWidth(),
								  stencil->getHeight(),
								  8);
		unsigned char* newImgData = newImg->getData();
		
		int originImgByte = 3;
		int stencilImgByte =3;
		
		if(origin->hasAlpha()) originImgByte=4;
		if(stencil->hasAlpha()) stencilImgByte=4;
		
		
		
		if(isGLcoordinate==true){
			cutPointInOrigin.y = originSize.y-cutPointInOrigin.y;
			//newImg->setCutPoint(cutPointInOrigin.x/2,(cutPointInOrigin.y)/2);
		}else{
			//newImg->setCutPoint(cutPointInOrigin.x/2,(originSize.y-cutPointInOrigin.y)/2);
			
		}
		newImg->setCutPoint(cutPointInOrigin.x/2,(originSize.y-cutPointInOrigin.y)/2);
		
		
		int originLength =	origin->getDataLen();
		int pDataLengthX4 = originLength*4;
		
		unsigned char* stencilDataPos;
		unsigned char* newImgDataPos;
		unsigned char* originDataPos;
		
		PuzzlePoint tempPoint = {cutPointInOrigin.x-stencilSizeHalf.x,cutPointInOrigin.y-stencilSizeHalf.y};
		
		int px,py,origin_i,stencil_i;
		for(int y=0;y<stencilSize.y;y++){
			for(int x=0;x<stencilSize.x;x++){
				stencil_i = (y*stencilSize.x+x)*stencilImgByte;
				px = x+tempPoint.x;
				py = y+tempPoint.y;
				origin_i=(py*originSize.x+px)*originImgByte;
				
				if(origin_i>=pDataLengthX4 || py<0 || px<0)continue;
				
				stencilDataPos = &stencilData[stencil_i+3];
				newImgDataPos = &newImgData[stencil_i+3];

				float calcAlpha = (*stencilDataPos / 255.f);
				originDataPos = &originData[origin_i+3];
				*--newImgDataPos = *--originDataPos * calcAlpha;
				*--newImgDataPos = *--originDataPos * calcAlpha;
				*--newImgDataPos = *--originDataPos * calcAlpha;
				
			}
		}

		
		return newImg;
//		PuzzleCache::getInstance()->testImage = new CCImage;
//		PuzzleCache::getInstance()->testImage->initWithImageData(stencil->getData(), stencil->getDataLen());
//		PuzzleCache::getInstance()->testImage->retain();
//		
//		PuzzleImage *itex = new PuzzleImage;
//		itex->initWithImage(stencil);
//		itex->autorelease();
//		return itex;
	}
	

	
	
};


/*
class PuzzleSprite : CCNode {
public:
	CCSprite *m_image;
	
	bool init(){
		if ( !CCNode::init() )
		{
			return false;
		}
		this->setContentSize(CCSizeMake(100, 100));
		return true;
	}
	
	PuzzleSprite(){
		m_image=nullptr;
	}
	
	bool setTexture(CCTexture2D* texture){
		if(m_image==nullptr){
			m_image=CCSprite::createWithTexture(texture);
			m_image->setAnchorPoint(CCPointZero);
			m_image->setPosition(CCPointZero);
			return true;
		}
		return false;
	}
};
*/

#endif /* defined(__DGproto__PuzzleCache__) */
