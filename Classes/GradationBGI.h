// GradationBGI.h
//

#ifndef LZZ_GradationBGI_h
#define LZZ_GradationBGI_h
#include "cocos2d.h"

USING_NS_CC;
#define LZZ_INLINE inline
class GradationMap : public CCSprite
{
public:
  static GradationMap * create (ccColor3B top_left, ccColor3B bottom_left, ccColor3B top_right, ccColor3B bottom_right, CCRect t_rect);
  virtual void visit ();
  virtual void draw ();
private:
  CCRect scissor_rect;
  void myInit (ccColor3B top_left, ccColor3B bottom_left, ccColor3B top_right, ccColor3B bottom_right, CCRect t_rect);
};
class GradationBGI : public CCSpriteBatchNode
{
public:
  static GradationBGI * create (float left_x, float right_x, float bottom_y, float top_y);
private:
  CCRect scissor_rect;
  void myInit (float left_x, float right_x, float bottom_y, float top_y);
};
#undef LZZ_INLINE
#endif
