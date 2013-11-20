// GradationBGI.cpp
//

#include "GradationBGI.h"
#define LZZ_INLINE inline
using namespace std;
GradationMap * GradationMap::create (ccColor3B top_left, ccColor3B bottom_left, ccColor3B top_right, ccColor3B bottom_right, CCRect t_rect)
        {
		GradationMap* t_g = new GradationMap();
		t_g->myInit(top_left, bottom_left, top_right, bottom_right, t_rect);
		t_g->autorelease();
		return t_g;
	}
void GradationMap::visit ()
        {
		glEnable(GL_SCISSOR_TEST);
		
		glScissor(scissor_rect.origin.x,scissor_rect.origin.y,scissor_rect.size.width,scissor_rect.size.height);
		
		CCSprite::visit();
		
		glDisable(GL_SCISSOR_TEST);
	}
void GradationMap::draw ()
        {
		CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
		
		CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");
		
		CC_NODE_DRAW_SETUP();
		
		ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
		
		ccGLBindTexture2D( m_pobTexture->getName() );
		ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
		
#define kQuadSize sizeof(m_sQuad.bl)
#ifdef EMSCRIPTEN
		long offset = 0;
		setGLBufferData(&m_sQuad, 4 * kQuadSize, 0);
#else
		long offset = (long)&m_sQuad;
#endif // EMSCRIPTEN
		
		// vertex
		int diff = offsetof( ccV3F_C4B_T2F, vertices);
		glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
		
		// texCoods
		diff = offsetof( ccV3F_C4B_T2F, texCoords);
		glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
		
		// color
		diff = offsetof( ccV3F_C4B_T2F, colors);
		glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
		
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		CHECK_GL_ERROR_DEBUG();
		
		
#if CC_SPRITE_DEBUG_DRAW == 1
		// draw bounding box
		CCPoint vertices[4]={
			ccp(m_sQuad.tl.vertices.x,m_sQuad.tl.vertices.y),
			ccp(m_sQuad.bl.vertices.x,m_sQuad.bl.vertices.y),
			ccp(m_sQuad.br.vertices.x,m_sQuad.br.vertices.y),
			ccp(m_sQuad.tr.vertices.x,m_sQuad.tr.vertices.y),
		};
		ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
		// draw texture box
		CCSize s = this->getTextureRect().size;
		CCPoint offsetPix = this->getOffsetPosition();
		CCPoint vertices[4] = {
			ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
			ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
		};
		ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW
		
		CC_INCREMENT_GL_DRAWS(1);
		
		CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
	}
void GradationMap::myInit (ccColor3B top_left, ccColor3B bottom_left, ccColor3B top_right, ccColor3B bottom_right, CCRect t_rect)
        {
		initWithFile("whitePaper.png");
		
		m_sQuad.tl.colors.r = top_left.r;
		m_sQuad.tl.colors.g = top_left.g;
		m_sQuad.tl.colors.b = top_left.b;
		m_sQuad.tl.colors.a = 255;
		
		m_sQuad.bl.colors.r = bottom_left.r;
		m_sQuad.bl.colors.g = bottom_left.g;
		m_sQuad.bl.colors.b = bottom_left.b;
		m_sQuad.bl.colors.a = 255;
		
		m_sQuad.tr.colors.r = top_right.r;
		m_sQuad.tr.colors.g = top_right.g;
		m_sQuad.tr.colors.b = top_right.b;
		m_sQuad.tr.colors.a = 255;
		
		m_sQuad.br.colors.r = bottom_right.r;
		m_sQuad.br.colors.g = bottom_right.g;
		m_sQuad.br.colors.b = bottom_right.b;
		m_sQuad.br.colors.a = 255;
		
		scissor_rect = t_rect;
	}
GradationBGI * GradationBGI::create (float left_x, float right_x, float bottom_y, float top_y)
        {
		GradationBGI* t_g = new GradationBGI();
		t_g->myInit(left_x, right_x, bottom_y, top_y);
		t_g->autorelease();
		return t_g;
	}
void GradationBGI::myInit (float left_x, float right_x, float bottom_y, float top_y)
        {
		CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		CCSize rSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize(); // getSize
		
		float wScale = frame_size.width / rSize.width;
		float hScale = frame_size.height / rSize.height;
		float xMargine = 0;
		float yMargine = 0;
		
		if(wScale >= hScale)
		{
			wScale = hScale;
			xMargine = (frame_size.width - rSize.width*wScale)/2.f;
		}
		else
		{
			hScale = wScale;
			yMargine = (frame_size.height - rSize.height*hScale)/2.f;
		}
		
		float x = 0*wScale + xMargine;
		float y = 0*hScale + yMargine;
		float w = 480*wScale;
		float h = 320*hScale;
		
		scissor_rect = CCRectMake(x, y, w, h);
		
		initWithFile("whitePaper.png", kDefaultSpriteBatchCapacity);
		
		int x_cnt = floorf((left_x-240.f-480.f)/480.f);
		int y_cnt = floorf((bottom_y-160.f-320.f)/320.f);
		int w_cnt = ceilf((right_x+240.f+480.f)/480.f);
		int h_cnt = ceilf((top_y+160.f+320.f)/320.f);
		
		ccColor3B color_array[12];
		color_array[0] = ccc3(255, 102, 102);
		color_array[1] = ccc3(255, 204, 102);
		color_array[2] = ccc3(255, 255, 102);
		color_array[3] = ccc3(204, 255, 102);
		color_array[4] = ccc3(102, 255, 102);
		color_array[5] = ccc3(102, 255, 204);
		color_array[6] = ccc3(102, 255, 255);
		color_array[7] = ccc3(102, 204, 255);
		color_array[8] = ccc3(102, 102, 255);
		color_array[9] = ccc3(204, 102, 255);
		color_array[10] = ccc3(255, 102, 255);
		color_array[11] = ccc3(255, 102, 204);
		
		for(int i=y_cnt;i<h_cnt;i++)
		{
			for(int j=x_cnt;j<w_cnt;j++)
			{
				GradationMap* t_g = GradationMap::create(color_array[(i+1)%12], color_array[i%12], color_array[(i+1)%12], color_array[i%12], scissor_rect);
				t_g->setPosition(ccp(240 + j*480, 160 + i*320));
				addChild(t_g);
			}
		}
	}
#undef LZZ_INLINE
