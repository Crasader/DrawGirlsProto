// GageBar.h
//

#ifndef LZZ_GageBar_h
#define LZZ_GageBar_h
#include "cocos2d.h"

USING_NS_CC;
using namespace std;
#define LZZ_INLINE inline
class GageBar : public CCSprite
{
public:
  static GageBar * create (string filename, float t_per);
  void setPercentage (float t_p);
  void actionPercentage (float t_p);
  virtual void setPosition (CCPoint t_p);
  virtual void visit ();
private:
  CCRect view_rect;
  float m_percentage;
  float a_percentage;
  float tick;
  bool is_animated;
  float max_length;
  int action_frame;
  void startMyAction ();
  void myAction ();
  void stopMyAction ();
  void myInit (string filename, float t_per);
};
#undef LZZ_INLINE
#endif
