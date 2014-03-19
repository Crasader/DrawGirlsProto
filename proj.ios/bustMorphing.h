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

class MyNode : public CCNode{
public:
  Vertex2D vertices[9];
	Vertex2D textCoords[9];
	CCTexture2D *texture;
	int cnt;
	bool dir;
	bool init(){
		if(!CCNode::init())return false;
		
		texture = CCTextureCache::sharedTextureCache()->addImage("bmTest.png");
		
		GLfloat size = texture->getPixelsWide()/2;
		
		//삼각형좌표들
		vertices[0] = Vertex2DMake(-size,size);
		vertices[1] = Vertex2DMake(size,size);
		vertices[2] = Vertex2DMake(-size/2,size/2);
		vertices[3] = Vertex2DMake(size/2,size/2);
		vertices[4] = Vertex2DMake(-size/2,-size/2);
		vertices[5] = Vertex2DMake(size/2,-size/2);
		vertices[6] = Vertex2DMake(-size,-size);
		vertices[7] = Vertex2DMake(size,-size);
		vertices[8] = Vertex2DMake(size,size);
		
		//우측상단이 (0,0), 좌측하단이 (1,1)
		textCoords[0] = Vertex2DMake(0,0);
		textCoords[1] = Vertex2DMake(1,0);
		textCoords[2] = Vertex2DMake(0.25,0.25);
		textCoords[3] = Vertex2DMake(0.75,0.25);
		textCoords[4] = Vertex2DMake(0.25,0.75);
		textCoords[5] = Vertex2DMake(0.75,0.75);
		textCoords[6] = Vertex2DMake(0,1);
		textCoords[7] = Vertex2DMake(1,1);
		textCoords[8] = Vertex2DMake(1,0);
		
		
		cnt = 0;
		dir = true;
		return true;
	}
	
	void draw(){
		
		
		
		//애니메이션을 위한 카운트값변경
		GLfloat dt = 0.5;
		if(dir)dt=-0.5;
		
		if(cnt>60){
			dir=!dir;
			cnt=0;
		}
		if(cnt==0)dt=0;
		cnt++;
		

		//점이동 시키기 애니메이션
		{
		Vertex2D old2 = vertices[2];
		vertices[2] = Vertex2DMake(old2.x, old2.y+dt*1);
		}
		{
		Vertex2D old2 = vertices[3];
			vertices[3] = Vertex2DMake(old2.x, old2.y+dt*1);
		}
		{
		Vertex2D old2 = vertices[4];
			vertices[4] = Vertex2DMake(old2.x, old2.y+dt*-1);
		}
		{
		Vertex2D old2 = vertices[5];
			vertices[5] = Vertex2DMake(old2.x, old2.y+dt*-1);
		}
		
		//세팅
		texture->getShaderProgram()->use();
		
		texture->getShaderProgram()->setUniformsForBuiltins();
		
		ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );
		
		//점배열설정
		glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, textCoords);
		glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		
		//배열값이용해서 삼각형 그리기
		glDrawArrays(GL_TRIANGLE_STRIP, 0,8);
		
		
		
	}
};


#endif /* defined(__DGproto__bustMorphing__) */

