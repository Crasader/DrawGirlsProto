//
//  bustMorphing.h
//  DGproto
//
//  Created by LitQoo on 2014. 3. 18..
//
//

#ifndef __DGproto__bustMorphing__
#define __DGproto__bustMorphing__


#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "KSUtil.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "GDWebSprite.h"
#include "CommonButton.h"
#include "KSUtil.h"
#include "ks19937.h"
#include "EasingAction.h"
#include "EffectSprite.h"
#include "CCGL.h"
#include "CommonButton.h"
#include "support/TransformUtils.h"
#include "ks19937.h"
#include "ServerDataSave.h"
#include "AchieveNoti.h"
#include "GraphDog.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

typedef struct {
	GLfloat x;
	GLfloat y;
} Vertex2D;

static inline Vertex2D Vertex2DMake(GLfloat inX,GLfloat inY){
	Vertex2D ret;
	ret.x = inX;
	ret.y = inY;
	return ret;
}

typedef struct {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} Vertex3D;

static inline Vertex3D Vertex3DMake(GLfloat inX,GLfloat inY, GLfloat inZ){
	Vertex3D ret;
	ret.x = inX;
	ret.y = inY;
	ret.z = inZ;
	return ret;
}
class Vertex{
public:
 float x, y, extraData;
 Vertex(){ x=y=0.0f; }
 Vertex(const Vertex& v){
	 x=v.x; y=v.y; extraData = v.extraData;
 }
 Vertex(float _x, float _y, float _extraData){
	 x=_x; y=_y; extraData = _extraData;
 }
 bool operator==(const Vertex& v){
  return (x==v.x && y==v.y);
 }
};

class Triangle{
public:
 int vt[3];
	Triangle(){ vt[0] = vt[1] = vt[2] = 0; }
 Triangle(const Triangle& t){ memcpy(vt, t.vt, sizeof(vt)); }
 Triangle(int v1, int v2, int v3){
  vt[0]=v1; vt[1]=v2; vt[2]=v3;
 }
};

#define INIT_SUPERTRI_SIZE 9999999.f
#define sqre(x) (x*x)
class DelaunayTriangulation{
private:
	vector<Vertex> mVertices;
	vector<Triangle> mTriangles;
public:
	const vector<Vertex>& getVertices(){return mVertices;}
	vector<Triangle> getTriangles(){
		vector<Triangle> retValue;
		for(auto& i : mTriangles)
		{
			if(fabsf(mVertices[i.vt[0]].x) == INIT_SUPERTRI_SIZE)
				continue;
			if(fabsf(mVertices[i.vt[0]].y) == INIT_SUPERTRI_SIZE)
				continue;
			if(fabsf(mVertices[i.vt[1]].x) == INIT_SUPERTRI_SIZE)
				continue;
			if(fabsf(mVertices[i.vt[1]].y) == INIT_SUPERTRI_SIZE)
				continue;
			if(fabsf(mVertices[i.vt[2]].x) == INIT_SUPERTRI_SIZE)
				continue;
			if(fabsf(mVertices[i.vt[2]].y) == INIT_SUPERTRI_SIZE)
				continue;

			retValue.push_back(i);
		}
		return retValue;
	}
	DelaunayTriangulation(){
		mVertices.push_back( Vertex(0, INIT_SUPERTRI_SIZE, 0) );
		mVertices.push_back( Vertex(-INIT_SUPERTRI_SIZE, -INIT_SUPERTRI_SIZE, 0) );
		mVertices.push_back( Vertex(INIT_SUPERTRI_SIZE, -INIT_SUPERTRI_SIZE, 0) );
		mTriangles.push_back( Triangle(0,1,2) );
	}
	void PushVertex(float x, float y, float extraData){
		Vertex vt(x,y, extraData);
		mVertices.push_back( vt );
		int iVtx = mVertices.size()-1;
		int iTri = SearchCoverTriangle(vt);
		while( iTri==-1 )
		{
			for(int i=0; i<3; i++){
				mVertices[i].x*=2.0f;
				mVertices[i].y*=2.0f;
			}
			iTri = SearchCoverTriangle(vt);
		}
		Triangle mainTri = mTriangles[iTri];
		mTriangles.erase(mTriangles.begin()+iTri);
		for(int i=0; i<3; i++){
			recTriangulation(mainTri.vt[i], mainTri.vt[(i+1)%3], iVtx);
		}
	}
	// print to consol
	void PrintTriangles(){
		vector<Triangle> mTriangles2 = getTriangles();
		int nTri = mTriangles2.size(); 
		for(int i=0; i<nTri; i++){
			for(int j=0; j<3; j++){
				CCLOG("(%f, %f)", mVertices[mTriangles2[i].vt[j]].x, mVertices[mTriangles2[i].vt[j]].y);
				//cout<<"("<<mVertices[mTriangles2[i].vt[j]].x<<","<<mVertices[mTriangles2[i].vt[j]].y<<")  ";
			}
			cout<<endl;
		}
	}
	//void RenderVertices(){
		//unsigned vtxCount = mVertices.size();
		//glBegin(GL_POINTS);
		//glColor3f(1, 0, 0);
		//for(unsigned i=0; i<vtxCount; i++){
			//glVertex2f(mVertices[i].x, mVertices[i].y);
		//}
		//glEnd();
	//}
	//void RenderTriangles(){
		//unsigned triCount = mTriangles.size();
		//glBegin(GL_TRIANGLES);
		//for(unsigned i=0; i<triCount; i++){
			//glColor3f((float)(i*2)/(float)triCount, 1.0f/(float)i, (float)i/(float)triCount);
			//for(int k=0; k<3; k++)
				//glVertex2f(mVertices[mTriangles[i].vt[k]].x, mVertices[mTriangles[i].vt[k]].y);
		//}
		//glEnd();
	//}
private:
	void recTriangulation(int edgeVtx1, int edgeVtx2, int iVtx){
		int iAdjTri, adjEdgeOrder;
		iAdjTri = SearchAdjecentTriangle(edgeVtx2, edgeVtx1, &adjEdgeOrder);
		if( iAdjTri!=-1 ){
			if( InCircumcircle(mTriangles[iAdjTri], mVertices[iVtx]) ){
				Triangle adjTri = mTriangles[iAdjTri];
				mTriangles.erase(mTriangles.begin()+iAdjTri);
				recTriangulation(adjTri.vt[adjEdgeOrder], adjTri.vt[(2+adjEdgeOrder)%3], iVtx);
				recTriangulation(adjTri.vt[(1+adjEdgeOrder)%3], adjTri.vt[(2+adjEdgeOrder)%3], iVtx);
				return;
			}
		}
		if( TriangleAreaX2(mVertices[iVtx], mVertices[edgeVtx1], mVertices[edgeVtx2]) >= 0 )
			mTriangles.push_back( Triangle(iVtx, edgeVtx1, edgeVtx2) );
		else
			mTriangles.push_back( Triangle(iVtx, edgeVtx2, edgeVtx1) );
	}
	// > 0 : ccw, < 0 : cw,  == 0 : on same line
	float TriangleAreaX2(const Vertex& v1, const Vertex& v2, const Vertex& v3){
		return (v2.x - v1.x)*(v3.y - v1.y) - (v2.y - v1.y)*(v3.x - v1.x);
	}
	bool IsIn(const Triangle& tri, const Vertex& vtx){
		if( TriangleAreaX2(vtx, mVertices[tri.vt[0]], mVertices[tri.vt[1]]) < 0 )
			return false;
		if( TriangleAreaX2(vtx, mVertices[tri.vt[1]], mVertices[tri.vt[2]]) < 0 )
			return false;
		if( TriangleAreaX2(vtx, mVertices[tri.vt[2]], mVertices[tri.vt[0]]) < 0 )
			return false;
		return true;
	}
	int SearchCoverTriangle(const Vertex& v){
		for(int i=mTriangles.size()-1; i>=0; i--){
			if( IsIn( mTriangles[i], v) )
				return i;
		}
		return -1;
	}
	// return triangle index. pOutEdgeOrder is important.
	// triangle index set and edge order is; 0-1: 0, 1-2: 1, 2-0: 2
	int SearchAdjecentTriangle(int iv1, int iv2, int* pOutEdgeOrder){
		for(int i=mTriangles.size()-1; i>=0; i--){
			for(int j=0; j<3; j++){
				if( mTriangles[i].vt[j]==iv1 && mTriangles[i].vt[(j+1)%3]==iv2 ){
					if( pOutEdgeOrder ) *pOutEdgeOrder = j;
					return i;
				}
				if( mTriangles[i].vt[j]==iv2 && mTriangles[i].vt[(j+1)%3]==iv1 ){
					if( pOutEdgeOrder ) *pOutEdgeOrder = j;
					return i;
				}
			}
		}
		return -1;
	}
	bool InCircumcircle(const Triangle& tri, const Vertex& vtx){
		return (sqre(mVertices[tri.vt[0]].x) + sqre(mVertices[tri.vt[0]].y)) * TriangleAreaX2(mVertices[tri.vt[1]], mVertices[tri.vt[2]], vtx) -
			(sqre(mVertices[tri.vt[1]].x) + sqre(mVertices[tri.vt[1]].y)) * TriangleAreaX2(mVertices[tri.vt[0]], mVertices[tri.vt[2]], vtx) +
			(sqre(mVertices[tri.vt[2]].x) + sqre(mVertices[tri.vt[2]].y)) * TriangleAreaX2(mVertices[tri.vt[0]], mVertices[tri.vt[1]], vtx) -
			(sqre(vtx.x) + sqre(vtx.y)) * TriangleAreaX2(mVertices[tri.vt[0]], mVertices[tri.vt[1]], mVertices[tri.vt[2]]) > 0;
	}
};


struct ValidArea
{
	float x;
	float y;
	float r;
};
class MyNode : public CCLayer{
public:
  Vertex3D* m_vertices;
	Vertex3D* m_2xVertices;
	map<Vertex3D*, Vertex3D> m_backupVertices;
	Vertex3D* m_textCoords;
	ccColor4F* m_colors;
	vector<Vertex3D> m_points;
	CCTexture2D *texture;
	GLfloat m_halfWidth;
	GLfloat m_halfHeight;
	int m_triCount;
	CCPoint m_beganTouchPoint;
	bool m_validTouch;
	CCPoint m_validTouchPosition;
	float m_waveRange;
	bool m_isWaving;
	float m_lastTouchTime; // 마지막 터치 시점.
	float m_touchTimer; // 증가하는 시간
	function<void()> touch_ccb_func;
	
	int m_touch_cnt;
	///
	
	int colorRampUniformLocation;   // 1
	CCTexture2D* colorRampTexture;  // 2
	vector<vector<ccColor4B>> m_silColors; // y, x 의 컬러값. loadRGB 할 때 로드 함.
	CCPoint m_greenCenter;
	CCPoint m_redCenter;
	CCPoint m_startPos;
//	FromToWithDuration2
	MyNode()
	{
		texture = nullptr;
		m_isWaving = false;
		m_isLoadedRGB = false;
		m_touchTimer = 0.f;
		m_lastTouchTime = 0.f;
		m_touch_cnt = 0;
		touch_ccb_func = nullptr;
	}
	///////////////////
	virtual ~MyNode()
	{
		CCLOG("~MyNode");
		
		if(m_textCoords)
		{
			delete [] m_textCoords;
		}
		if(m_vertices)
			delete [] m_vertices;
		if(m_2xVertices)
			delete [] m_2xVertices;
		if(m_colors)
			delete [] m_colors;
		
		
		texture->release();
	}
	
	
	
	void ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
	{
	
	}
	
	void ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
	{
		
	}
	
	void ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
	{
		
	}
	
	void ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
	{
		ccTouchesEnded(pTouches, pEvent);
	}
	
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		CCPoint touchLocation = pTouch->getLocation();
		CCPoint local = convertToNodeSpace(touchLocation);
		m_startPos = convertToNodeSpace(touchLocation);
		m_stopTouch = false;
		m_validTouch = false;
		
		m_waveRange = 0;
		
		float diffRad1 = atan2f(local.y - m_greenCenter.y, local.x - m_greenCenter.x );
		float diffRad2 = atan2f(local.y - m_redCenter.y, local.x - m_redCenter.x);
		int cnt=0;
		for(auto i : m_movingVertices){ // 움직여야 되는 점의 집합에 대해
			Vertex3D backup = *i;
				
			CCPoint t = ccp(backup.x, backup.y);
			if(ccpLength(t - local) <= 30)
			{
				m_waveRange=1;
			}
		}
			
		return true;
	}
	virtual void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, 0, false);
	}
	
	
	vector<Vertex3D*> m_movingVertices;
	map<Vertex3D*, ccColor4B> m_movingVertexColors; // 움직일 좌표의 rgb 임.
	map<Vertex3D*, float> m_distance; // 클릭한 곳으로부터로의 위치
	
	bool m_stopTouch;
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		if(m_stopTouch==true) return;
		
		CCPoint touchLocation = pTouch->getLocation();
		CCPoint local = convertToNodeSpace(touchLocation);
		CCPoint t = ccpMult(ccpSub(local,m_startPos),0.2f);
		float d = ccpLength(t);
		if(d>18){
			this->morphing(pTouch,pEvent);
			m_stopTouch=true;
			return;
		}
		
		m_validTouch = false;
		
		float diffRad1 = atan2f(local.y - m_greenCenter.y, local.x - m_greenCenter.x );
		float diffRad2 = atan2f(local.y - m_redCenter.y, local.x - m_redCenter.x);
		int cnt=0;
		for(auto i : m_movingVertices){ // 움직여야 되는 점의 집합에 대해
			Vertex3D backup = m_backupVertices[i];
			ccColor4B rgb = m_movingVertexColors[i];
			float waveValue = rgb.g + rgb.r;
			if(waveValue > 0)
			{
				
				*i = Vertex3DMake(backup.x + t.x * waveValue/255.f, backup.y + t.y* waveValue/255.f, backup.z);
				

			}
			cnt++;
		}

		return; // disable

	}
	
	void morphing(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		if(m_stopTouch){m_stopTouch=false; return;}
		
		
		
		CCPoint touchLocation = pTouch->getLocation();
		CCPoint local = convertToNodeSpace(touchLocation);
		CCPoint subP = ccpMult(ccpSub(local,m_startPos),0.2f);
		CCPoint t = ccpMult(ccpSub(local,m_startPos),0.2f);
			
			float diffRad1 = atan2f(local.y - m_greenCenter.y, local.x - m_greenCenter.x );
			float diffRad2 = atan2f(local.y - m_redCenter.y, local.x - m_redCenter.x);
			int cnt=0;
			for(auto i : m_movingVertices){ // 움직여야 되는 점의 집합에 대해
				Vertex3D backup = m_backupVertices[i];
				ccColor4B rgb = m_movingVertexColors[i];
				float waveValue = rgb.g + rgb.r;
				float time1 = 0.15f;
				float time2 = 0.5f;
				if(waveValue > 0)
				{
					CCPoint goalPosition = CCPointMake(backup.x + t.x * waveValue/255.f, backup.y + t.y* waveValue/255.f);
					addChild(KSGradualValue<CCPoint>::create(goalPosition, ccp(backup.x, backup.y), time2,
																									 [=](CCPoint k){
																										 *i = Vertex3DMake(k.x,k.y, backup.z);
																									 },
																									 [=](CCPoint t){
																										 *i = backup;
																									 },
																									 elasticOut));
				}
				cnt++;
			}
			
			
			return;
		
	}
	
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
#if 0
		CCPoint touchLocation = pTouch->getLocation();
		CCPoint local = convertToNodeSpace(touchLocation);
		
		m_validTouch = false;
		for(auto validArea : m_valids)
		{
			CCPoint point = ccp(validArea.x, validArea.y);
			float distance = ccpLength(point - local);
			if(distance <= validArea.r)
			{
				m_validTouchPosition = point;
				m_beganTouchPoint = local;
				m_validTouch = true;
				m_waveRange = validArea.r;
			}
		}
#endif
		if(m_touchTimer > m_lastTouchTime + 0.7f)
		{
			m_lastTouchTime = m_touchTimer;
//			setTouchEnabled(false);
		}
		else
		{
			return; // 너무 이른 터치가 들어왔으면 나감
		}
		if(m_isLoadedRGB == false)
			return;
		vector<int> tt;
		
		m_touch_cnt++;
		if(!myAchieve->isNoti(AchievementCode::kAchievementCode_hidden_holder) && !myAchieve->isCompleted(AchievementCode::kAchievementCode_hidden_holder) &&
		   m_touch_cnt >= myAchieve->getCondition(AchievementCode::kAchievementCode_hidden_holder))
		{
			myAchieve->changeIngCount(AchievementCode::kAchievementCode_hidden_holder, myAchieve->getCondition(AchievementCode::kAchievementCode_hidden_holder));
			AchieveNoti* t_noti = AchieveNoti::create(AchievementCode::kAchievementCode_hidden_holder);
			CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
		}
		
		int sound_cnt = NSDS_GI(kSDS_CI_int1_soundCnt_i, card_number);
		if(sound_cnt > 0 && !myDSH->getBoolForKey(kDSH_Key_isOffCardMorphingSound))
		{
			AudioEngine::sharedInstance()->playGroanEffect(NSDS_GS(kSDS_CI_int1_soundType_int1_s, card_number, ks19937::getIntValue(1, sound_cnt)));
		}
		
		if(touch_ccb_func != nullptr)
		{
			touch_ccb_func();
		}
		
		CCPoint touchLocation = pTouch->getLocation();
		CCPoint local = getParent()->convertToNodeSpace(touchLocation);
		
		m_validTouch = false;

		//			local = m_validTouchPosition;
		CCLOG("%f %f", local.x, local.y);


		float diffRad1 = atan2f(local.y - m_greenCenter.y, local.x - m_greenCenter.x );
		float diffRad2 = atan2f(local.y - m_redCenter.y, local.x - m_redCenter.x);
		for(auto i : m_movingVertices){ // 움직여야 되는 점의 집합에 대해
			Vertex3D backup = m_backupVertices[i];
			ccColor4B rgb = m_movingVertexColors[i];
			float waveValue = rgb.g+rgb.r; // (rgb.b >= 10 ? 40 : 0);
			float devider = -15.f;
			float time1 = 0.15f;
			float time2 = 0.5f;
			if(waveValue > 0)
			{
				float diffRad = atan2f(backup.y-local.y, backup.x-local.x );
				
				CCPoint k = ccpSub(ccp(backup.x,backup.y),local);
				if(k.x>15)k.x=15;
				if(k.x<-15)k.x=-15;
				if(k.y>15)k.y=15;
				if(k.y<-15)k.y=-15;
				
				CCPoint goalPosition = CCPointMake(k.x * waveValue/255.f, k.y* waveValue/255.f);
				//ccp(cosf(diffRad), sinf(diffRad)) * waveValue  / devider;
				//goalPosition = ccp(clampf(goalPosition.x, -20, 20), clampf(goalPosition.y, -20, 20));
				addChild(KSGradualValue<CCPoint>::create(ccp(0, 0), goalPosition, time1,
																								 [=](CCPoint t){
																									 *i = Vertex3DMake(backup.x + t.x, backup.y + t.y, backup.z);
																									 //																								 i->y = backup.y + t;
																								 },
																								 [=](CCPoint t){
																									 //for(auto i : movingVertices){

																									 addChild(KSGradualValue<CCPoint>::create(goalPosition, ccp(0, 0), time2,
																																														[=](CCPoint t){
																																															*i = Vertex3DMake(backup.x + t.x, backup.y + t.y, backup.z);
																																														},
																																														[=](CCPoint t){
																																															*i = backup;
																																														},
																																														elasticOut));
																									 //}
																								 },
																								 nullptr));
				//																							 expoIn));
			}
			
		}

	}
	void triangulationWithPoints()
	{
		triangulationWithPoints(m_points);
	}
	void triangulationWithPoints(const vector<Vertex3D>& points);
	static MyNode* create(CCTexture2D* tex, int t_card_number)
	{
		MyNode* n = new MyNode();
		
		n->init(tex, t_card_number);
		n->autorelease();
		
		return n;
	}
	static MyNode* create()
	{
		MyNode* n = new MyNode();
		
		n->init();
		n->autorelease();
		
		return n;
	}
	bool init()
	{
		CCLayer::init();
		
		scheduleUpdate();	
		return true;
	}
	void update(float dt)
	{
		m_touchTimer += 1 / 60.f;
		
	}
	
	void wave(float dt)
	{
		// 원래 대로 돌아가려는 힘.
		
		
	}
	void movingDistance(CCPoint t) // 영호.
	{
		if(m_touchTimer > m_lastTouchTime + 1.f)
		{
//			m_lastTouchTime = m_touchTimer;
			//			setTouchEnabled(false);
		}
		else
		{
			return; // 너무 이른 터치가 들어왔으면 나감
		}
//		CCLOG("%f %f", t.x, t.y);
		if(m_isLoadedRGB == false)
			return;

		// t 에 반대쪽으로 m_vertices 를 조작함.
		// 모든 m_vertices 에 대한 y, x 에 대한 RGB 값은 m_silColors[y][x] 로 참조하면 됨.
		for(int i=0; i<m_triCount * 3; i++)
		{
			Vertex3D original = m_2xVertices[i];
			ccColor4B color = m_silColors[original.y][original.x];
			float waveValue = color.g + (color.b >= 10 ? 10 : 0);

			// waveValue 가 클 수록 그만큼 반대로 움직여야 됨.
			CCPoint against = t * (float)waveValue / 10.f / 12.f;// / 12.f; //  / 50.f; // / 30.f;
			if( !(against.x == 0.f && against.y == 0.f) )
			{
				m_vertices[i].x = m_backupVertices[&m_vertices[i]].x - against.x;
				m_vertices[i].y = m_backupVertices[&m_vertices[i]].y - against.y;
			}
			else
			{
				// 여기서 부터는 출렁 해야됨.
				auto backupPosition = m_backupVertices[&m_vertices[i]];
				
				auto startPosition = m_vertices[i];
				CCPoint ccpStartPosition = ccp(startPosition.x, startPosition.y);
				auto goalPosition = m_backupVertices[&m_vertices[i]];
				CCPoint ccpGoalPosition = ccp(goalPosition.x, goalPosition.y);
				ccpGoalPosition = ccpGoalPosition - ccpStartPosition;
				if(ccpLength(ccpGoalPosition) > 0.5f)
				{
					addChild(KSGradualValue<CCPoint>::create(ccp(0, 0), ccpGoalPosition, 0.5f,
																									 [=](CCPoint t){
																										 m_vertices[i] = Vertex3DMake(startPosition.x + t.x,
																																									startPosition.y + t.y,
																																									m_vertices[i].z);
																										 //																								 i->y = backup.y + t;
																									 },
																									 [=](CCPoint t){
																										 m_vertices[i] = Vertex3DMake(startPosition.x + t.x,
																																									startPosition.y + t.y,
																																									m_vertices[i].z);
																									 },
																									 elasticOut));
				}
			}
		}
	}
	bool init(CCTexture2D* tex, int t_card_number){
		CCLayer::init();
		scheduleUpdate();
		ignoreAnchorPointForPosition(false);
//		if(!CCSprite::initWithTexture(tex))
//			return false;
		
		setTouchEnabled(true);
		m_imageRotationDegree = 0.f;
		m_imageRotationDegreeX = 0.f;
		card_number = t_card_number;
		tex->retain();
		texture = tex;//CCTextureCache::sharedTextureCache()->addImage("bmTest.png");
		setContentSize(texture->getContentSize());
		GLfloat halfWidth = texture->getPixelsWide()/2;
		GLfloat halfHeight = texture->getPixelsHigh()/2;	
		m_halfWidth = halfWidth;
		m_halfHeight = halfHeight;
		
		m_textCoords = nullptr;
		m_vertices = nullptr;
		m_colors = nullptr;
		m_2xVertices = nullptr;
//		m_points.push_back(Vertex3DMake(-halfWidth, halfHeight, 0));
//		m_points.push_back(Vertex3DMake(halfWidth, halfHeight, 0));
//		
//		m_points.push_back(Vertex3DMake(-halfWidth, -halfHeight, 0));
//		m_points.push_back(Vertex3DMake(halfWidth, -halfHeight, 0));
		putBasicInfomation();	 // 기본정보 들어가게.

		triangulationWithPoints(m_points);
		
		
		
		

		
		return true;
	}

	void putBasicInfomation()
	{
		m_points.clear();
		m_points.push_back(Vertex3DMake(0, texture->getPixelsHigh() - 1, 0));
		m_points.push_back(Vertex3DMake(texture->getPixelsWide() - 1, texture->getPixelsHigh() - 1, 0));

		m_points.push_back(Vertex3DMake(0, 0, 0));
		m_points.push_back(Vertex3DMake(texture->getPixelsWide() - 1, 0, 0));

		int hm = 30;
		int wm = 30;
		for(int y=hm; y<texture->getPixelsHigh()-hm; y+=hm)
		{
			for(int x=hm; x<texture->getPixelsWide()-hm; x+=wm)
			{
				m_points.push_back(Vertex3DMake(x, y, 0));
			}
		}
	}
	
	void loadRGB(const std::string& fullPath)
	{
		m_isLoadedRGB = true;
		m_silColors.clear();

		
		
#if 1
		CCImage* img = new CCImage();
		img->initWithEncryptedImageFileFullPath(fullPath.c_str());
		unsigned char* oData = img->getData();

		int height = img->getHeight();
		int width = img->getWidth();
		m_silColors = vector<vector<ccColor4B> >(height, vector<ccColor4B>(width));	
		CCPoint redWeightCenter = ccp(0, 0);
		CCPoint greenWeightCenter = ccp(0, 0);
		float greenWeightSum = 0.f;
		float redWeightSum = 0.f;
		for(int y=0;y<height;y++){
			for(int x=0;x<width;x++){
				int i = ((height - 1 - y)*width+x)*4;
//				CCLOG("i = %d", i);
				m_silColors[y][x] = ccc4(oData[i], oData[i + 1], oData[i + 2], oData[i + 3]);
				greenWeightCenter = greenWeightCenter + ccp(x, y) * oData[i + 1] / 255.f;
				greenWeightSum += oData[i + 1] / 255.f;
				redWeightCenter = redWeightCenter + ccp(x, y) * oData[i + 0] / 255.f;
				redWeightSum += oData[i + 0] / 255.f;
			}
		}
		if(greenWeightSum != 0.f)
			m_greenCenter = greenWeightCenter / greenWeightSum / 2.f;
		if(redWeightSum != 0.f)
			m_redCenter = redWeightCenter / redWeightSum / 2.f;
		int j = 0;
		// z 값을 주석처리 해놈.
//		for(auto& point : m_points)
//		{
//			int i = ((height - 1 - point.y) * width + (point.x))*4;
//			//			CCLOG("2. i = %d", i);
//			auto tt = (float)oData[i];
//			point.z = tt / 255.f * 50.f;
//		}
		img->release();
		/////////////////////
#endif
		
	gatherVertices();

	}
	
	
	void gatherVertices(){
		m_movingVertices.clear();
		m_movingVertexColors.clear();
		m_distance.clear();
		
		for(int i=0; i<m_triCount*3; i++)
		{
			CCPoint t = ccp(m_vertices[i].x, m_vertices[i].y);
			Vertex3D original = m_2xVertices[i];
			ccColor4B color = m_silColors[original.y][original.x];
			if(color.r<=0 && color.g<=0)continue;
			m_movingVertexColors[&m_vertices[i]] = color;
			
			m_movingVertices.push_back(&m_vertices[i]); // 움직일 대상이 되는 점을 모집함.
		}
	}
	
	vector<Vertex3D*> findVertices(Vertex3D v)
	{
		vector<Vertex3D*> retValue;
		for(int i=0; i<m_triCount*3; i++)
		{
			if(m_vertices[i].x == v.x && m_vertices[i].y == v.y)
			{
				retValue.push_back(&m_vertices[i]);
			}
		}
		return retValue;
	}	
	void draw(){
		CCLayer::draw();

		if(texture == nullptr)
			return;
		
//		glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//		glEnable(GL_DEPTH_TEST);
		//glLoadIdentity();
//		kmGLLoadMatrix
//		kmMat4 input;
//		kmGLLoadMatrix(&input);
//		kmGLMultMatrix(<#const kmMat4 *pIn#>)
//		kmGLScalef(0.5f, 0.5f, 1.f);
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		float zeye = CCDirector::sharedDirector()->getZEye();
		kmMat4 mv, mv2;
		kmGLGetMatrix(KM_GL_MODELVIEW, &mv);
		kmGLMatrixMode(KM_GL_MODELVIEW);
		kmGLPushMatrix();
		
		kmGLTranslatef(texture->getContentSizeInPixels().width/4.f,
									 texture->getContentSizeInPixels().height/4.f, 0);
		kmGLRotatef(m_imageRotationDegree, 0, 1, 0);
		kmGLRotatef(m_imageRotationDegreeX, 1, 0, 0);
		kmGLTranslatef(-texture->getContentSizeInPixels().width/4.f,
									 -texture->getContentSizeInPixels().height/4.f, 0.f);
		
		
		int verticesCount = m_triCount * 3;
		//세팅
		texture->getShaderProgram()->use();
		
		texture->getShaderProgram()->setUniformsForBuiltins();
		
		ccGLBindTexture2D(texture->getName());
		ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
	

		glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);
		glVertexAttribPointer(kCCVertexAttrib_TexCoords, 3, GL_FLOAT, GL_FALSE, 0, m_textCoords);
		glDrawArrays(GL_TRIANGLES, 0, verticesCount);
		
		kmGLPopMatrix();
		
		glDisable(GL_DEPTH_TEST);
	}
	bool m_isLoadedRGB;
	int card_number;
	CC_SYNTHESIZE(float, m_imageRotationDegree, ImageRotationDegree);
	CC_SYNTHESIZE(float, m_imageRotationDegreeX, ImageRotationDegreeX);
		
};


#endif /* defined(__DGproto__bustMorphing__) */

