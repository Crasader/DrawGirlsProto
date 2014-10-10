//
//  MapScanner.h
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 23..
//
//

#ifndef DrawingJack_MapScanner_h
#define DrawingJack_MapScanner_h

#include "cocos2d.h"
#include "GameData.h"
#include <queue>
#include "StarGoldData.h"
#include "StageImgLoader.h"
#include "SilhouetteData.h"
#include <pthread.h>
#include <random>
#include "StartMapLuckyItem.h"
#include "EffectSprite.h"

using namespace cocos2d;
using namespace std;
#include <functional>
using namespace placeholders;
#if defined __GNUC__ || defined __APPLE__
#include <ext/hash_map>
using namespace __gnu_cxx;
#else
#include <hash_map>
#endif
class BFS_Point
{
public:
	int x;
	int y;
	BFS_Point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	BFS_Point(){}
	bool operator== (const BFS_Point& a) const
    {
        return (a.x == x && a.y == y);
    }
	bool operator<( const BFS_Point& other) const
	{
		return x < other.x;
		if ( x == other.x )
		{
			return y < other.y;
		}
		
		
	}
};
template<>
struct std::hash<BFS_Point>
{
	typedef std::size_t result_type;
	
	result_type operator()(BFS_Point const& s) const
	{
		return s.x ^ (s.y << 1);
//		return s.x + s.y;
//		result_type const h1 ( std::hash<std::string>()(s.first_name) );
//		result_type const h2 ( std::hash<std::string>()(s.last_name) );
//		return h1 ^ (h2 << 1);
	}
};

//class BackObj : public CCSprite
//{
//public:
//	static BackObj* create(const char* filename)
//	{
//		BackObj* t_bo = new BackObj();
//		if(t_bo && t_bo->initWithFile(filename))
//		{
//			t_bo->myInit();
//			t_bo->autorelease();
//			return t_bo;
//		}
//		CC_SAFE_DELETE(t_bo);
//		return NULL;
//	}
//	
//	virtual void visit()
//	{
//		kmGLPushMatrix();
//		transform();
//		draw();
//		kmGLPopMatrix();
//	}
//	
//private:
//	
//	void myInit()
//	{
//		
//	}
//};

class InvisibleSprite : public CCNode
{
public:
	static InvisibleSprite* create(const char* filename, bool isPattern);
	
	virtual ~InvisibleSprite()
	{
		
	}
	
private:
	
	void myInit(const char* filename, bool isPattern);
};

//class TestEyeSprite : public CCSprite
//{
//public:
//	static TestEyeSprite* create(const char* filename, CCRect t_rect, CCArray* t_drawRects)
//	{
//		TestEyeSprite* t_tes = new TestEyeSprite();
//		t_tes->myInit(filename, t_rect, t_drawRects);
//		t_tes->autorelease();
//		return t_tes;
//	}
//	
//	virtual void visit()
//	{
//		unsigned int loopCnt = drawRects->count();
//		
//		for(int i=0;i<loopCnt;i++)
//		{
//			IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
//			
//			glEnable(GL_SCISSOR_TEST);
//			
//			int viewport [4];
//			glGetIntegerv (GL_VIEWPORT, viewport);
//			CCSize rSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize(); // getSize
//			float wScale = viewport[2] / rSize.width;
//			float hScale = viewport[3] / rSize.height;
//			float x = t_rect->origin.x*wScale + viewport[0];
//			float y = t_rect->origin.y*hScale + viewport[1];
//			float w = t_rect->size.width*wScale;
//			float h = t_rect->size.height*hScale;
//			glScissor(x,y,w,h);
//			
//			CCSprite::visit();
//			
//			glDisable(GL_SCISSOR_TEST);
//		}
//	}
//	
//private:
//	CCArray* drawRects;
//	
//	void myInit(const char* filename, CCRect t_rect, CCArray* t_drawRects)
//	{
//		initWithFile(filename, t_rect);
//		
//		drawRects = t_drawRects;
//	}
//};

typedef struct {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} Vertex3D;

typedef struct {
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
}tColor4B;

class RectsSprite : public EffectSprite
{
public:
	static RectsSprite* create(const std::string& str)
	{
		RectsSprite* t_rs = new RectsSprite();
		t_rs->initWithFile(str.c_str());
		t_rs->autorelease();
		return t_rs;
	}
	
	static RectsSprite* createWithTexture(CCTexture2D *pTexture)
	{
		RectsSprite *pobSprite = new RectsSprite();
		if (pobSprite && pobSprite->initWithTexture(pTexture))
		{
			pobSprite->autorelease();
			return pobSprite;
		}
		CC_SAFE_DELETE(pobSprite);
		return NULL;
	}
	
	void setVertex(Vertex3D* t_vertices, ccTex2F* t_textCoords, tColor4B* t_colors, int t_vertice_count)
	{
		m_vertices = t_vertices;
		m_textCoords = t_textCoords;
		m_colors = t_colors;
		vertice_count = t_vertice_count;
	}
	
	virtual void draw()
	{
//		08-25 11:38:09.535: D/cocos2d-x debug info(17448): 0x583e3dc8 0x583ec880 0x581878c0 108

//		KS::KSLog("% % % %", m_vertices, m_textCoords, m_colors, vertice_count);
		CC_NODE_DRAW_SETUP();
		ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
		//세팅
		//	getTexture()->getShaderProgram()->use();
		//
		//	getTexture()->getShaderProgram()->setUniformsForBuiltins();
		
		ccGLBindTexture2D(getTexture()->getName());
		//	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
		
#define kQuadSize sizeof(m_sQuad.bl)
		ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
		
		{
			glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);
			glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_textCoords);
			glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, m_colors);
			glDrawArrays(GL_TRIANGLES, 0, vertice_count);
		}
	}
	
private:
	Vertex3D* m_vertices;
	ccTex2F* m_textCoords;
	tColor4B* m_colors;
	
	int vertice_count;
};

class VisibleSprite : public EffectSprite
{
public:
	static VisibleSprite* create(const char* filename, bool isPattern, std::vector<IntRectSTL>* t_drawRects, string sil_filename, int t_card_number);
	
	void setMoveGamePosition(CCPoint t_p);
	
	CCPoint getMoveGamePosition();
	
	void setSceneNode(CCObject* t_scene_node);
	void visitForThumb();
	void replayVisitForThumb(int temp_time);
	virtual void visit();
	
	void setLight();
	void setDark();
	
	virtual void draw();
	void setRectToVertex();
	
	virtual ~VisibleSprite()
	{
		if(m_vertices)
		{
			delete [] m_vertices;
			m_vertices = nullptr;
		}
		if(m_textCoords)
		{
			delete [] m_textCoords;
			m_textCoords = nullptr;
		}
		if(light_vertices)
		{
			delete [] light_vertices;
			light_vertices = nullptr;
		}
		if(safety_vertices)
		{
			delete [] safety_vertices;
			safety_vertices = nullptr;
		}
		if(m_colors)
		{
			delete [] m_colors;
			m_colors = nullptr;
		}
	}
	
private:
	std::vector<IntRectSTL>* drawRects;
	
	Vertex3D* m_vertices;
//	Vertex3D* m_textCoords;
	ccTex2F* m_textCoords;
	int t_vertice_count;
	
	Vertex3D* light_vertices;
	Vertex3D* safety_vertices;
	tColor4B* m_colors;
	
	CCPoint jack_position;
	CCSize screen_size;
	CCSize design_resolution_size;
	int viewport[4];
	CCNode* scene_node;
	bool is_set_scene_node;
	
	RectsSprite* safety_img;
	RectsSprite* light_img;
	
	int light_r;
	int light_g;
	int light_b;
	int light_step;
	int light_frame;
	
	void myInit(const char* filename, bool isPattern, std::vector<IntRectSTL>* t_drawRects, string sil_filename, int t_card_number);
	
	CCTexture2D* createSafetyImage(string fullpath);
};

class VisibleParent : public CCNode
{
public:
	static VisibleParent* create(const char* filename, bool isPattern, string sil_filename, int t_card_number);
	
	void setDrawRects(const std::vector<IntRectSTL>& t_rects);
	
	virtual ~VisibleParent()
	{
	}
	
	const std::vector<IntRectSTL>& getDrawRects();
	
	CCSprite* getVisibleSprite()
	{
		return myVS;
	}
	
private:
	VisibleSprite* myVS;
	std::vector<IntRectSTL> drawRects;
//	CCArray* drawRects;
	
	void divideRect(IntPoint crashPoint);
	void divideRects(IntRect crashRect);
	
	void setMoveGamePosition(CCPoint t_p);
	
	CCPoint limitted_map_position;
	
	void setLimittedMapPosition();
	
	void changingGameStep(int t_step);
	
	void myInit(const char* filename, bool isPattern, string sil_filename, int t_card_number);
};

enum MSzorder{
	invisibleZorder = 1,
	visibleZorder,
	blockZorder,
//	topBottomZorder,
	gainEffectZorder,
	boarderZorder
};

class BackFilename
{
public:
	string filename;
	bool isPattern;
};

class MapScanner : public CCNode
{
public:
	static MapScanner* create();

	void scanMap();
	
	virtual void visit();
	
	void exchangeMS();
	
	void randomingRectView(CCPoint t_p);
	
	void stopRandomingRectView();
	
	void startGame();
	
	CCSprite* getVisibleSprite()
	{
		return visibleImg->getVisibleSprite();
	}
	
	void removeEmptyPoint();
	
private:
	InvisibleSprite* invisibleImg;
	VisibleParent* visibleImg;
	CCSprite* random_rect_img;
	CCNode* blockParent;
	
	int silType;
	
	IntRect init_rect;
	CCSize screen_size;
	int screen_height;
	
	bool my_tic_toc;
	
	CCObject* start_target;
	SEL_CallFunc start_delegate;
	
	void resetRects(bool is_after_scanmap);
	std::vector<IntRectSTL> getGainRects(mapType (*gainMap)[217]);
	mapType mapInitState[162][217];
	
	IntRectSTL newRectChecking(mapType (*mapArray)[217], vector<pair<IntPointSTL, mapType>>* checkingIndex, const IntMoveStateSTL& start);
	
	void bfsCheck(mapType beforeType, mapType afterType, IntPoint startPoint);
	
	BFS_Point directionVector(IntDirection direction);
	
	bool isInnerMap(const BFS_Point& t_p);
	
	BackFilename getBackVisibleFilename();
	
	BackFilename getBackInvisibleFilename();
	
	void ingNewlineToRealNewline();
	
	void setMapImg();
	
	CCSprite* top_boarder;
	CCSprite* bottom_boarder;
	CCSprite* left_boarder;
	CCSprite* right_boarder;
	
	CCSpriteBatchNode* top_block_manager;
	CCSpriteBatchNode* top_block_line_manager;
	CCSpriteBatchNode* bottom_block_manager;
	CCSpriteBatchNode* bottom_block_line_manager;
	
	CCSprite* top_block_lock;
	CCSprite* bottom_block_lock;
	
	void setTopBottomBlock();
	StartMapLuckyItem* start_map_lucky_item;
	
	hash_map<int, BFS_Point> direction2BfsPoint;
	bool is_removed_top_block, is_removed_bottom_block;
	int remove_block_cnt;
	
//	vector<pair<IntPoint, mapType>> checkingIndex;
	void startRemoveBlock();
	
	void removingBlock();
	
	void showEmptyPoint(CCPoint t_point);
	
	bool isCheckBossLocked();
	
	void myInit();
};

#endif
