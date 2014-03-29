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
 Triangle(){ memset(vt, 0, sizeof(vt)); }
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
	vector<Vertex> getVertices(){return mVertices;}
	vector<Triangle> getTriangles(){
		vector<Triangle> retValue;
		for(auto i : mTriangles)
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
				CCLog("(%f, %f)", mVertices[mTriangles2[i].vt[j]].x, mVertices[mTriangles2[i].vt[j]].y);
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
		
	///
	
	int colorRampUniformLocation;   // 1
	CCTexture2D* colorRampTexture;  // 2
	MyNode()
	{
		texture = nullptr;
	}
	///////////////////
	virtual ~MyNode()
	{
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
	virtual void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, 0, false);
	}
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		return; // disable
		CCPoint touchLocation = pTouch->getLocation();
		CCPoint local = convertToNodeSpace(touchLocation);
		
		CCPoint diff = -(local - m_beganTouchPoint);
		
		if(m_validTouch && ccpLength(diff) > 30)
		{
			m_validTouch = false;
			local = m_validTouchPosition;
			CCLog("%f %f", local.x, local.y);
			
			vector<Vertex3D*> movingVertices;
			map<Vertex3D*, float> distance;
			for(int i=0; i<m_triCount*3; i++)
			{
				CCPoint t = ccp(m_vertices[i].x, m_vertices[i].y);
				if(ccpLength(t - local) <= m_waveRange)
				{
					movingVertices.push_back(&m_vertices[i]);
					distance[&m_vertices[i]] = ccpLength(t-local);
				}
			}
			
			for(auto i : movingVertices){
				Vertex3D backup = m_backupVertices[i];
				float r = distance[i];
				float diffRad = atan2f(diff.y, diff.x);
//				CCPoint goalPosition = ccp(cosf(diffRad) * -800 / r, sinf(diffRad) * -800 / r);
				CCPoint goalPosition = ccp(cosf(diffRad) * -200 / powf(r, 0.9f), sinf(diffRad) * -200 / powf(r, 0.9f));
				goalPosition = ccp(clampf(goalPosition.x, -20, 20), clampf(goalPosition.y, -20, 20));
				addChild(KSGradualValue<CCPoint>::create(ccp(0, 0), goalPosition, 0.3f,
																							 [=](CCPoint t){
																								 *i = Vertex3DMake(backup.x + t.x, backup.y + t.y, backup.z);
//																								 i->y = backup.y + t;
																							 },
																							 [=](CCPoint t){
																								 //for(auto i : movingVertices){
																								 
																								 addChild(KSGradualValue<CCPoint>::create(goalPosition, ccp(0, 0), 1.f,
																																												[=](CCPoint t){
																																													*i = Vertex3DMake(backup.x + t.x, backup.y + t.y, backup.z);
																																												},
																																												[=](CCPoint t){
																																													*i = backup;
																																												},
																																												elasticOut));
																								 //}
																							 }));
			}
		}
	}
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
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
		CCPoint touchLocation = pTouch->getLocation();
		CCPoint local = convertToNodeSpace(touchLocation);
		
		m_validTouch = false;

				

		
		
		m_validTouch = false;
		//			local = m_validTouchPosition;
		CCLog("%f %f", local.x, local.y);
		m_waveRange = 100;
		vector<Vertex3D*> movingVertices;
		map<Vertex3D*, float> distance;
		for(int i=0; i<m_triCount*3; i++)
		{
			CCPoint t = ccp(m_vertices[i].x, m_vertices[i].y);
			if(ccpLength(t - local) <= m_waveRange)
			{
				movingVertices.push_back(&m_vertices[i]);
				distance[&m_vertices[i]] = ccpLength(t-local);
			}
		}

		for(auto i : movingVertices){
			Vertex3D backup = m_backupVertices[i];
			float r = distance[i];
			float diffRad = atan2f(-1.f, 0.f); // 아래쪽으로.
			//				CCPoint goalPosition = ccp(cosf(diffRad) * -800 / r, sinf(diffRad) * -800 / r);
			CCPoint goalPosition = ccp(cosf(diffRad) * -200 / powf(r, 0.9f), sinf(diffRad) * -200 / powf(r, 0.9f));
			goalPosition = ccp(clampf(goalPosition.x, -20, 20), clampf(goalPosition.y, -20, 20));
			addChild(KSGradualValue<CCPoint>::create(ccp(0, 0), goalPosition, 0.3f,
																							 [=](CCPoint t){
																								 *i = Vertex3DMake(backup.x + t.x, backup.y + t.y, backup.z);
																								 //																								 i->y = backup.y + t;
																							 },
																							 [=](CCPoint t){
																								 //for(auto i : movingVertices){

																								 addChild(KSGradualValue<CCPoint>::create(goalPosition, ccp(0, 0), 1.f,
																																													[=](CCPoint t){
																																														*i = Vertex3DMake(backup.x + t.x, backup.y + t.y, backup.z);
																																													},
																																													[=](CCPoint t){
																																														*i = backup;
																																													},
																																													elasticOut));
																								 //}
																							 }));
		}

		
		return true;
	}
	void triangulationWithPoints()
	{
		triangulationWithPoints(m_points);
	}
	void triangulationWithPoints(const vector<Vertex3D>& points)
	{
		DelaunayTriangulation delaunay;
		for(auto i : points)
		{
			delaunay.PushVertex(i.x, i.y, i.z);
		}
		vector<Triangle> mTriangles2 = delaunay.getTriangles();
		int nTri = mTriangles2.size();
		m_triCount = nTri;
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
		
		m_textCoords = new Vertex3D[nTri * 3];
		m_vertices = new Vertex3D[nTri * 3];
		m_2xVertices = new Vertex3D[nTri * 3];
		m_colors = new ccColor4F[nTri * 3];
		for(int i=0; i<nTri; i++){
			for(int j=0; j<3; j++){
				float ss = ks19937::getIntValue(0, 0);
				Vertex3D temp = Vertex3DMake(delaunay.getVertices()[mTriangles2[i].vt[j]].x / 2.f,
																		 delaunay.getVertices()[mTriangles2[i].vt[j]].y / 2.f,
																		 delaunay.getVertices()[mTriangles2[i].vt[j]].extraData);
//				temp.z = points
				m_vertices[i*3 + j] = temp;
				m_2xVertices[i * 3 + j] = Vertex3DMake(delaunay.getVertices()[mTriangles2[i].vt[j]].x,
										 delaunay.getVertices()[mTriangles2[i].vt[j]].y,
										 delaunay.getVertices()[mTriangles2[i].vt[j]].extraData);
				m_textCoords[i*3 + j] = Vertex3DMake(m_2xVertices[i * 3 + j].x / (2 * m_halfWidth),
																						 1.f - (m_2xVertices[i * 3 + j].y) / (2 * m_halfHeight), temp.z);
				m_colors[i*3 + j] = ccc4f(ks19937::getFloatValue(0, 1), ks19937::getFloatValue(0, 1), ks19937::getFloatValue(0, 1), 1.f);
			}
		}
		
		
		
		
		for(int i=0; i<nTri; i++){
			for(int j=0; j<3; j++){
				m_backupVertices[&m_vertices[i*3 + j]] = m_vertices[i*3 + j];
			}
		}
	}
	static MyNode* create(CCTexture2D* tex)
	{
		MyNode* n = new MyNode();
		
		n->init(tex);
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
		
		
		return true;
	}
	void movingDistance(CCPoint t) // 영호.
	{
	}
	bool init(CCTexture2D* tex){
		CCLayer::init();
		ignoreAnchorPointForPosition(false);
//		if(!CCSprite::initWithTexture(tex))
//			return false;
		
		setTouchEnabled(true);
		m_imageRotationDegree = 0.f;
		m_imageRotationDegreeX = 0.f;
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
		//loadRGB(CCFileUtils::sharedFileUtils()->fullPathForFilename("bmTest2.png").c_str()); // 실루엣 z 정보 넣는 곳.

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

		int hm = 20;
		int wm = 20;
		for(int y=0 + hm; y<texture->getPixelsHigh() - 20; y+=hm)
		{
			for(int x=0 + wm; x<texture->getPixelsWide() - 20; x+=wm)
			{
				m_points.push_back(Vertex3DMake(x, y, 0));
			}
		}
	}
	void loadRGB(const std::string& fullPath)
	{
#if 1
		CCImage* img = new CCImage();
		img->initWithImageFileThreadSafe(fullPath.c_str());
		unsigned char* oData = img->getData();	
		int height = img->getHeight();
		int width = img->getWidth();

		//		for(int y=0;y<height;y++){
		//			for(int x=0;x<width;x++){
		//				int i = (y*width+x)*4;
		//				CCLog("i = %d", i);
		//			}
		//		}
		int j = 0;
		for(auto& point : m_points)
		{
			int i = ((height - 1 - point.y) * width + (point.x))*4;
			//			CCLog("2. i = %d", i);
			auto tt = (float)oData[i];
			point.z = tt / 255.f * 50.f;
		}
		img->release();
		/////////////////////
#endif
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
	
	CC_SYNTHESIZE(float, m_imageRotationDegree, ImageRotationDegree);
	CC_SYNTHESIZE(float, m_imageRotationDegreeX, ImageRotationDegreeX);
};


#endif /* defined(__DGproto__bustMorphing__) */

