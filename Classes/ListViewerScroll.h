// ListViewerScroll.h
//

#ifndef LZZ_ListViewerScroll_h
#define LZZ_ListViewerScroll_h
#include "cocos2d.h"
#include "DataStorageHub.h"

USING_NS_CC;
#define LZZ_INLINE inline
class ScrollingObject
{
public:
  virtual void setPercentage (float t_p) = 0;
};
class ListViewerScroll : public CCLayer, public ScrollingObject
{
public:
  static ListViewerScroll * create (CCRect t_rect, ScrollingObject * t_link, string filename, CCPoint t_min, CCPoint t_max);
  virtual void setPercentage (float t_p);
  void setPositionScrollY (float t_y);
private:
  CCRect my_rect;
  ScrollingObject * target_link;
  CCSprite * scroll_img;
  CCPoint min_position;
  CCPoint max_position;
  CCPoint touch_begin_point;
  bool is_touching;
  virtual bool ccTouchBegan (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent);
  virtual void ccTouchMoved (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent);
  virtual void ccTouchEnded (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent);
  virtual void ccTouchCancelled (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent);
  virtual void registerWithTouchDispatcher ();
  void myInit (CCRect t_rect, ScrollingObject * t_link, string filename, CCPoint t_min, CCPoint t_max);
};
#undef LZZ_INLINE
#endif
