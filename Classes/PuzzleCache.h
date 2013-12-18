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
#include "StageImgLoader.h"

using namespace cocos2d;
using namespace std;

enum PuzzleCachePieceType {
	kPuzzleCachePieceType_left = 1000,
	kPuzzleCachePieceType_right,
	kPuzzleCachePieceType_top,
	kPuzzleCachePieceType_bottom
};

class PuzzleImageData{
private:
	unsigned char* m_data;
	unsigned long  m_size;
public:
	void loadImageDataWithFullpath(string fullpath){
		m_data= CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "rb", &m_size);
	}
	
	void loadImageData(string filename){
		string path =  CCFileUtils::sharedFileUtils()->fullPathForFilename(filename.c_str()).c_str();
		loadImageDataWithFullpath(path);
	}
	unsigned char* getData(){
		return m_data;
	}
	
	unsigned long getDataLen(){
		return m_size;
	}
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
	
	static PuzzleImage* create(string path){
		PuzzleImage* image = new PuzzleImage();
		image->initWithImageFile(path.c_str());
		image->autorelease();
		return image;
	}
	
	static PuzzleImage* createWithImageFileThreadSafe(string fullpath){
		PuzzleImage* image = new PuzzleImage();
		image->initWithImageFileThreadSafe(fullpath.c_str());
		image->autorelease();
		return image;
	}
	
	static PuzzleImage* createWithImageData(unsigned char* data , unsigned long size){
		PuzzleImage* image = new PuzzleImage();
		image->initWithImageData(data, size);
		image->autorelease();
		return image;
	}
	static PuzzleImage* createWithPuzzleImageData(PuzzleImageData *puzzleImageData){
		PuzzleImage* image = new PuzzleImage();
		image->initWithPuzzleImageData(puzzleImageData);
		image->autorelease();
		return image;
	}
	
	bool initWithPuzzleImageData(PuzzleImageData *puzzleImageData){
		return this->initWithImageData(puzzleImageData->getData(), puzzleImageData->getDataLen(), CCImage::kFmtUnKnown);
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
	
	//std::function<void(PuzzleImage*)> m_callbackfunc;
	map<int,std::function<void(PuzzleImage*)>> m_callbackfuncList;
	
	//int m_currentLoadPuzzleNo;
	bool m_useTread;
	bool m_isStop;
	string m_documentPath;
	
public:
	std::function<void(string)> m_func;
	
	PuzzleCache(){
		CCLog("klasdfjlksadfaslkdfjlsakdjflkasjdfklajsdlkfjaskldfjsalkdfjlkasdf");
		isLockedLoadingPuzzleList=false;
		m_useTread=false;
		m_documentPath = mySIL->getDocumentPath();
	}
	
	
	
	
	//퍼즐로드 - 기본방식
	void loadImage(int puzzleNo){
		
		if(m_puzzleState[puzzleNo]=="loaded" || m_puzzleState[puzzleNo]=="loading"){
			callbackLoadedImage(puzzleNo);
			return;
		}
		
		startToLoadImage(puzzleNo);
	}
	

	bool loadImageOnThread(int puzzleNo){
		if(m_puzzleState[puzzleNo]=="loaded" || m_puzzleState[puzzleNo]=="loading"){
			callbackLoadedImage(puzzleNo);
			return true;
		}
		
		return startToLoadImage(puzzleNo);
	}
	
	void cancelLoadingImage(int puzzleNo){
		//return;
		
		return;
//		if(m_currentLoadPuzzleNo>0){
//			
//			CCLog("puzzlecache : cancel schedule");
//			//CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(PuzzleCache::loadingPuzzle), PuzzleCache::getInstance());
//			
//			
//			CCLog("puzzlecache : m_loadingPuzzleList before size is %luu",m_loadingPuzzleList.size());
//			waitForLoadingPuzzleList(true);
//			list<PuzzleImage*>::iterator it;
//			for (it=m_loadingPuzzleList.begin();it!=m_loadingPuzzleList.end();it++) {
//				PuzzleImage* texture = (*it);
//				texture->release();
//			}
//			
//			m_loadingPuzzleList.clear();
//			setLockedLoadingPuzzleList(false);
//			CCLog("puzzlecache : m_loadingPuzzleList after size is %luu",m_loadingPuzzleList.size());
//			
//			//m_currentLoadPuzzleNo=0;
//		}
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
		CCLog("add image puzzlekey is %s",image->getPuzzleKey().c_str());
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
		
		int itCnt=0;
		
		for (it=m_loadingPuzzleList.begin();it!=m_loadingPuzzleList.end();it++) {
			itCnt++;
			PuzzleImage* texture = (*it);
			if(m_loadingPuzzleList.size()==0)break;
			if(texture->getPuzzleKey()=="COMPLETE"){
				//CCLog("loadingPuzzle:COMPLETE");
				//퍼즐상태에 완료표시 할것
				texture->release();
				//if(m_loadingPuzzleList.size()==itCnt){
				CCLog("puzzlecache : load complete stop schedule %lu",m_loadingPuzzleList.size());
				//CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(PuzzleCache::loadingPuzzle), PuzzleCache::getInstance());
				//}
				
			}else if(texture->getPuzzleKey()!=""){
				//CCLog("loadingPuzzle:callfunc");
				map<string,PuzzleImage*>::iterator it2;
				it2 = m_textureList.find(texture->getPuzzleKey());
				
				
				
				
				int pNo = texture->getPuzzleNo();
				auto func =m_callbackfuncList[pNo];
				if(func)func(texture);
				
//				if(m_callbackfunc && (m_puzzleState[pNo]=="loaded" || m_puzzleState[pNo]=="loading")){
//					CCLog("callfunc %d",pNo);
//					m_callbackfunc(texture);
//				}else{
//					CCLog("callfunc cancel %d",pNo);
//				}
				
				
				if(it2==m_textureList.end()){
					this->addTextureList(texture->getPuzzleKey(), texture);
					texture->release();
				}
				
				
				
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
		m_callbackfuncList[puzzleNo]=func;
		//m_callbackfunc = func;
		//여기서 m_loadingPuzzleList에 complete있으면 지워주기
		
		CCLog("puzzlecache : start schedule!!!!!!!!!!!!%d",puzzleNo);
		//CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(PuzzleCache::loadingPuzzle), PuzzleCache::getInstance(), 0.f, kCCRepeatForever, 0.f, false);
		
		std::thread puzzleThread( [puzzleNo,func, this] ()
								 {
									 
									 PuzzleCache::getInstance()->m_useTread = true;
									 
									 if(PuzzleCache::getInstance()->loadImageOnThread(puzzleNo)){
										 CCLog("puzzlecache : add complete %d",puzzleNo);
										 PuzzleImage* complete = new PuzzleImage();
										 complete->setPuzzleKey("COMPLETE");
										 PuzzleCache::getInstance()->addLoadingPuzzleList(complete);
										 complete->release();
										 
									 }
									 CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(PuzzleCache::loadingPuzzle), PuzzleCache::getInstance(), 0.1f, 1, 0.1f, false);
									 
									 CCLog("puzzlecache : thread complete %d",puzzleNo);
									 PuzzleCache::getInstance()->m_useTread = false;
								 } );
		puzzleThread.detach();
	}
	
	
	bool checkCancel(int loadingPuzzleNo,int thread){
		
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(PuzzleCache::loadingPuzzle), PuzzleCache::getInstance(), 0.f, 1, 0.f, false);
		
		return false;
		
//		if(loadingPuzzleNo!=m_currentLoadPuzzleNo){
//			m_puzzleState[loadingPuzzleNo]="cancel";
//			
//			list<PuzzleImage*>::iterator it;
//			for (it=m_loadingPuzzleList.begin();it!=m_loadingPuzzleList.end();it++) {
//				PuzzleImage* texture = (*it);
//				texture->release();
//			}
//			waitForLoadingPuzzleList(true);
//			m_loadingPuzzleList.clear();
//			setLockedLoadingPuzzleList(false);
//			
//			CCLog("checkCancel true %d,%d",loadingPuzzleNo,thread);
//			return true;
//		}
//		
//
//		
//		CCLog("checkCancel false %d,%d",loadingPuzzleNo,thread);
//		return false;
	}
	
	void callbackLoadedImage(int puzzleNo){
		map<string,PuzzleImage*>::iterator it;
		for (it=m_textureList.begin();it!=m_textureList.end();it++) {
			PuzzleImage* texture = (PuzzleImage*)it->second;
			if(texture->getPuzzleNo()==puzzleNo){
				addLoadingPuzzleList(texture);
				texture->release();
			}
		}
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(PuzzleCache::loadingPuzzle), PuzzleCache::getInstance(), 0.f, 1, 0.f, false);
	}
	
	//퍼즐로드 - 원본 ccimage를 정해줄수있음
	bool startToLoadImage(int puzzleNo){
		
		static int b = 1;
		

		
		b++;
		
		CCLog("puzzlecache : call startToLoadImage %d",puzzleNo);
		//퍼즐 원본 로드
		CCString *pFilename = new CCString();
		pFilename->initWithFormat((m_documentPath+"puzzle%d_original.png").c_str(), puzzleNo);
		
		
		PuzzleImage* puzzleImg = new PuzzleImage();
		puzzleImg->initWithImageFileThreadSafe(pFilename->getCString());
		
		
		pFilename->release();
		
		//CCString::createWithFormat((mySIL->getDocumentPath()+"puzzle%d_original.png").c_str()
		PuzzleImage* thumbImg = new PuzzleImage();
		CCString *tFilename = new CCString();
		tFilename->initWithFormat((m_documentPath+"puzzle%d_face.png").c_str(), puzzleNo);
		thumbImg->initWithImageFileThreadSafe(tFilename->getCString());
		tFilename->release();
		//CCString::createWithFormat((mySIL->getDocumentPath()+"puzzle%d_face.png").c_str(), puzzleNo)->getCString()
		
		//m_currentLoadPuzzleNo = puzzleNo;
		//중복검사
		
//		if(m_puzzleState[puzzleNo]=="loaded" || m_puzzleState[puzzleNo]=="loading"){
//			CCLog("puzzlecache : start schedule by cache, puzzle No is %d,%d",m_currentLoadPuzzleNo,b);
//			callbackLoadedImage(puzzleNo);
//			//m_currentLoadPuzzleNo=0;
//			return true;
//		}
		
		CCLog("puzzlecache : start schedule, puzzle No is %d,%d",puzzleNo,b);
		
		m_puzzleState[puzzleNo]="loading";
		
		//틀만들기
		{//left
			PuzzleImageData *imgdata = new PuzzleImageData();
			imgdata->loadImageData("puzzle_stencil_1_left.png");
			PuzzleImage *stencil = new PuzzleImage();
			stencil->initWithPuzzleImageData(imgdata);
			PuzzlePoint cutPoint = {(int)(stencil->getWidth()/2),(int)(stencil->getHeight()/2)};
			PuzzleCache::getInstance()->changeStencilByOrigin(stencil, puzzleImg, cutPoint, true);
			
			CCString *key =  new CCString();
			key->initWithFormat("puzzle%d_left",puzzleNo);
			stencil->setPuzzleNo(puzzleNo);
			stencil->setPieceNo(kPuzzleCachePieceType_left);
			stencil->setPuzzleKey(key->getCString());
			addLoadingPuzzleList(stencil);
			stencil->release();
			key->release();
			delete imgdata;
		}
		

		{//right
			
			PuzzleImageData *imgdata = new PuzzleImageData();
			imgdata->loadImageData("puzzle_stencil_1_right.png");
			PuzzleImage *stencil = new PuzzleImage();
			stencil->initWithPuzzleImageData(imgdata);
			PuzzlePoint cutPoint = {(int)(puzzleImg->getWidth()-stencil->getWidth()/2),(int)(puzzleImg->getHeight()/2)};
			PuzzleCache::getInstance()->changeStencilByOrigin(stencil, puzzleImg, cutPoint, true);
			
			
			CCString *key =  new CCString();
			key->initWithFormat("puzzle%d_right",puzzleNo);
			stencil->setPuzzleNo(puzzleNo);
			stencil->setPieceNo(kPuzzleCachePieceType_right);
			stencil->setPuzzleKey(key->getCString());
			addLoadingPuzzleList(stencil);
			stencil->release();
			key->release();
			delete imgdata;
			
		}

		{//top
			
			PuzzleImageData *imgdata = new PuzzleImageData();
			imgdata->loadImageData("puzzle_stencil_1_top.png");
			PuzzleImage *stencil = new PuzzleImage();
			stencil->initWithPuzzleImageData(imgdata);
			PuzzlePoint cutPoint =  {(int)(puzzleImg->getWidth()/2),(int)(puzzleImg->getHeight()-stencil->getHeight()/2)};
			PuzzleCache::getInstance()->changeStencilByOrigin(stencil, puzzleImg, cutPoint, true);
			
			
			CCString *key =  new CCString();
			key->initWithFormat("puzzle%d_top",puzzleNo);
			stencil->setPuzzleNo(puzzleNo);
			stencil->setPieceNo(kPuzzleCachePieceType_top);
			stencil->setPuzzleKey(key->getCString());
			addLoadingPuzzleList(stencil);
			stencil->release();
			key->release();
			delete imgdata;
		}

		{//bottom
			
			PuzzleImageData *imgdata = new PuzzleImageData();
			imgdata->loadImageData("puzzle_stencil_1_bottom.png");
			PuzzleImage *stencil = new PuzzleImage();
			stencil->initWithPuzzleImageData(imgdata);
			PuzzlePoint cutPoint =  {(int)(puzzleImg->getWidth()/2),(int)(stencil->getHeight()/2)};
			PuzzleCache::getInstance()->changeStencilByOrigin(stencil, puzzleImg, cutPoint, true);
			
			
			CCString *key =  new CCString();
			key->initWithFormat("puzzle%d_bottom",puzzleNo);
			stencil->setPuzzleNo(puzzleNo);
			stencil->setPieceNo(kPuzzleCachePieceType_bottom);
			stencil->setPuzzleKey(key->getCString());
			addLoadingPuzzleList(stencil);
			stencil->release();
			key->release();
			delete imgdata;
		}
		
		if(checkCancel(puzzleNo,b)){
			puzzleImg->release();
			thumbImg->release();
			return false;
		}
		//피스만들기
		
		
		PuzzleImageData* hPieceData = new PuzzleImageData();
		hPieceData->loadImageData("puzzle_stencil_1_ph.png");
		
		PuzzleImageData* wPieceData = new PuzzleImageData();
		wPieceData->loadImageData("puzzle_stencil_1_pw.png");
		
		PuzzleImageData* _pieceData;
		
		int ycnt=0;
		int xcnt=0;
		int pw=1;
		for(int i=1;i<25;i++){
			if(checkCancel(puzzleNo,b)){
				delete hPieceData;
				delete wPieceData;
				puzzleImg->release();
				thumbImg->release();
				return false;
			}
			
			pw++;
			if(xcnt==6){pw++; ycnt++; xcnt=0;}
			if(pw%2){_pieceData=wPieceData;
			}else{ _pieceData=hPieceData;}
			
			
			//퍼즐 이미지
			
			PuzzleImage *whPiece = new PuzzleImage();
			whPiece->initWithPuzzleImageData(_pieceData);
			
			PuzzleCache::getInstance()->changeStencilByOrigin(whPiece, puzzleImg, {(int)(xcnt*136+180),(int)(ycnt*136+136)}, false);
			//PuzzleImage *pImg = PuzzleCache::getInstance()->cutImageByStensil(puzzleImg, _img, {(int)(xcnt*136+180),(int)(ycnt*136+136)},false);
			
			
			CCString *key =  new CCString();
			key->initWithFormat("puzzle%d_piece%d",puzzleNo,i);
			whPiece->setPuzzleNo(puzzleNo);
			whPiece->setPieceNo(i);
			whPiece->setPuzzleKey(key->getCString());
			key->release();
			
			
			//썸네일 이미지
			PuzzleImage *tPiece = new PuzzleImage();
			tPiece->initWithPuzzleImageData(_pieceData);
			
			PuzzleCache::getInstance()->changeStencilByOrigin(tPiece, thumbImg, {(int)(xcnt*202+101),(int)(ycnt*202+101)}, false);
			
			tPiece->setCutPoint(whPiece->getCutPoint().x,whPiece->getCutPoint().y);
			
			
			CCString *key2 =  new CCString();
			key2->initWithFormat("puzzle%d_thumbnail%d",puzzleNo,i);
			
			tPiece->setPuzzleNo(puzzleNo);
			tPiece->setPieceNo(i+100);
			tPiece->setPuzzleKey(key2->getCString());
			key2->release();
			
			addLoadingPuzzleList(tPiece);
			tPiece->release();
			addLoadingPuzzleList(whPiece);
			whPiece->release();
			
			xcnt++;
		}
		
		if(checkCancel(puzzleNo,b)){
			delete hPieceData;
			delete wPieceData;
			puzzleImg->release();
			thumbImg->release();
			return false;
		}
		
		delete hPieceData;
		delete wPieceData;
		puzzleImg->release();
		thumbImg->release();
		//wPiece->release();
		//	hPiece->release();
		
		m_puzzleState[puzzleNo]="loaded";
		CCLog("puzzlecache : load complete, puzzle No is %d",puzzleNo);
		//m_currentLoadPuzzleNo=0;
		
		return true;
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
	
	bool cutImageAndSave(CCImage *stencil, CCImage *origin,PuzzlePoint cutPointInOrigin,bool isGLcoordinate,string filename){
		changeStencilByOrigin(stencil, origin, cutPointInOrigin, isGLcoordinate);
		return stencil->saveToFile(filename.c_str(),false);
	}
	
	void changeStencilByOrigin(CCImage* stencil,CCImage* origin,PuzzlePoint cutPointInOrigin,bool isGLcoordinate){
		
		stencil->retain();
		origin->retain();
		
		unsigned char* originData = origin->getData();
		PuzzlePoint originSize = {origin->getWidth(),origin->getHeight()};
		
		
		unsigned char* stencilData = stencil->getData();
		PuzzlePoint stencilSize = {stencil->getWidth(),stencil->getHeight()};
		PuzzlePoint stencilSizeHalf = {stencilSize.x/2,stencilSize.y/2};
		
		int originImgByte = 3;
		int stencilImgByte =3;
		
		if(origin->hasAlpha()) originImgByte=4;
		if(stencil->hasAlpha()) stencilImgByte=4;
		
		if(isGLcoordinate==true){
			cutPointInOrigin.y = originSize.y-cutPointInOrigin.y;
		}
		
		//stencil->setCutPoint(cutPointInOrigin.x/2,(originSize.y-cutPointInOrigin.y)/2);
		
		int originLength =	origin->getDataLen();
		int pDataLengthX4 = originLength*4;
		
		
		PuzzlePoint tempPoint = {cutPointInOrigin.x-stencilSizeHalf.x,cutPointInOrigin.y-stencilSizeHalf.y};
		
		int px=0,py=0,origin_i=0,stencil_i=0;
		float calcAlpha=0;
		for(int y=0;y<stencilSize.y;y++){
			for(int x=0;x<stencilSize.x;x++){
				stencil_i = (y*stencilSize.x+x)*stencilImgByte;
				px = x+tempPoint.x;
				py = y+tempPoint.y;
				origin_i=(py*originSize.x+px)*originImgByte;
				
				if(origin_i>=pDataLengthX4 || py<0 || px<0)continue;
				
				calcAlpha = (stencilData[stencil_i+3] / 255.f);
				if(stencilData[stencil_i+3]>0){
					stencilData[stencil_i] = originData[origin_i]*calcAlpha;
					stencilData[stencil_i+1] = originData[origin_i+1]*calcAlpha;
					stencilData[stencil_i+2] = originData[origin_i+2]*calcAlpha;
				}
			}
		}
		
		stencil->release();
		origin->release();
	}
	
	//stencil
	void changeStencilByOrigin(PuzzleImage* stencil,PuzzleImage* origin,PuzzlePoint cutPointInOrigin,bool isGLcoordinate){
		
		unsigned char* originData = origin->getData();
		PuzzlePoint originSize = {origin->getWidth(),origin->getHeight()};
		
		
		unsigned char* stencilData = stencil->getData();
		PuzzlePoint stencilSize = {stencil->getWidth(),stencil->getHeight()};
		PuzzlePoint stencilSizeHalf = {stencilSize.x/2,stencilSize.y/2};
		
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
		
		stencil->setCutPoint(cutPointInOrigin.x/2,(originSize.y-cutPointInOrigin.y)/2);
		
		int originLength =	origin->getDataLen();
		int pDataLengthX4 = originLength*4;
		
		
		PuzzlePoint tempPoint = {cutPointInOrigin.x-stencilSizeHalf.x,cutPointInOrigin.y-stencilSizeHalf.y};
		
		int px=0,py=0,origin_i=0,stencil_i=0;
		float calcAlpha=0;
		for(int y=0;y<stencilSize.y;y++){
			for(int x=0;x<stencilSize.x;x++){
				stencil_i = (y*stencilSize.x+x)*stencilImgByte;
				px = x+tempPoint.x;
				py = y+tempPoint.y;
				origin_i=(py*originSize.x+px)*originImgByte;
				
				if(origin_i>=pDataLengthX4 || py<0 || px<0)continue;
				
				calcAlpha = (stencilData[stencil_i+3] / 255.f);
				if(stencilData[stencil_i+3]>0){
					stencilData[stencil_i] = originData[origin_i]*calcAlpha;
					stencilData[stencil_i+1] = originData[origin_i+1]*calcAlpha;
					stencilData[stencil_i+2] = originData[origin_i+2]*calcAlpha;
				}
			}
		}
		
	}
		
	//origin이미지에서 stencil이미지 만큼 잘라오기
	PuzzleImage* cutImageByStensil(CCImage* origin, CCImage* stencil,PuzzlePoint cutPointInOrigin,bool isGLcoordinate){
		
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
