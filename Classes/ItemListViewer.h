// ItemListViewer.h
//

#ifndef LZZ_ItemListViewer_h
#define LZZ_ItemListViewer_h
#include "cocos2d.h"
#include "ListViewerScroll.h"
#include "DataStorageHub.h"

USING_NS_CC;
#define LZZ_INLINE inline
class ItemListViewer : public CCLayer, public ScrollingObject
{
public:
  static ItemListViewer * create ();
  virtual void visit ();
  virtual void setPositionY (float t_y);
  void setPercentage (float t_p);
  void setMaxPositionY ();
  void setScroll (ScrollingObject * t_link);
private:
  CCRect view_rect;
  bool is_touching;
  int touched_index;
  CCPoint touch_begin_point;
  CCPoint first_touch_point;
  float max_positionY;
  float touchy;
  long int touchStartTime;
  float touchStartY;
  float moveSpeed;
  bool isAnimated;
  ScrollingObject * target_link;
  virtual bool ccTouchBegan (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent);
  virtual void ccTouchMoved (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent);
  virtual void ccTouchEnded (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent);
  virtual void ccTouchCancelled (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent);
  virtual void registerWithTouchDispatcher ();
  void myInit ();
  void moveList (float dy);
  void moveAnimation ();
};
#undef LZZ_INLINE
#endif
