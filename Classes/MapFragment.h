// MapFragment.h
//

#ifndef LZZ_MapFragment_h
#define LZZ_MapFragment_h
#include "cocos2d.h"
#include "GameData.h"
#include "AudioEngine.h"

using namespace cocos2d;
#include <functional>
using namespace placeholders;
#define LZZ_INLINE inline
class MapFragment : public CCSprite
{
public:
  static MapFragment * create (CCPoint s_p);
  void startFalling ();
private:
  int changeRotateVal;
  CCPoint dv;
  void falling ();
  void myInit (CCPoint s_p);
};
class MapFragmentParent : public CCSpriteBatchNode
{
public:
  static MapFragmentParent * create ();
  void createNewFragment (IntPoint s_p);
private:
  bool isCreateNewFragment;
  IntPoint createPoint;
  bool createFragmenting;
  void createFragment ();
  void myInit ();
};
#undef LZZ_INLINE
#endif
