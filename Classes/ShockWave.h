// ShockWave.h
//

#ifndef LZZ_ShockWave_h
#define LZZ_ShockWave_h
#include "cocos2d.h"
#include "GameData.h"
#include "AudioEngine.h"
#include "LogData.h"

using namespace placeholders;

using namespace cocos2d;
#include <functional>
#define LZZ_INLINE inline
class IntPoint;
class ShockWave : public CCSpriteBatchNode
{
public:
  static ShockWave * create (IntPoint t_createPoint);
  void stopSW ();
private:
  CCPoint create_position;
  float radius;
  int ing_frame;
  bool is_removing;
  void removeProcess ();
  void startSW ();
  void ingSW ();
  void myInit (IntPoint t_createPoint);
};
class SW_Parent : public CCNode
{
public:
  static SW_Parent * create ();
  void createSW (IntPoint t_create_point);
  void createJDSW (IntPoint t_create_point);
  void stopAllSW ();
private:
  bool is_justDie;
  void myInit ();
};
#undef LZZ_INLINE
#endif
