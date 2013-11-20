// JackMissile.h
//

#ifndef LZZ_JackMissile_h
#define LZZ_JackMissile_h
#include "cocos2d.h"
#include "GameData.h"
#include "MissileDamageData.h"
#include "DataStorageHub.h"
#include "ServerDataSave.h"
#include <random>

#define JM_SPEED 5.f
#define JM_CHANGE_DIRECTION_VAL	10.f
#define LZZ_INLINE inline
enum MyElemental
{
  kMyElementalNonElemental = 0,
  kMyElementalLife,
  kMyElementalFire,
  kMyElementalWater,
  kMyElementalWind,
  kMyElementalLightning,
  kMyElementalPlasma
};
class JackMissile : public CCNode
{
public:
  virtual void startMoving ();
protected:
  CCParticleSystem * particle;
  CCPoint particlePosition;
  CCNode * targetNode;
  float damage;
  float dex;
  MyElemental my_type;
};
enum ZorderJMU
{
  kZorderJMU_shootImg = 1,
  kZorderJMU_loadImg,
  kZorderJMU_particle,
  kZorderJMU_mainImg
};
class JM_UpgradeMissile : public JackMissile
{
public:
  static JM_UpgradeMissile * create (CCNode * t_target, int jm_type, float missile_speed);
  static JM_UpgradeMissile * create (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p);
  virtual ~ JM_UpgradeMissile ();
  virtual void startMoving ();
private:
  CCSprite * loadImg;
  CCSprite * shootImg;
  CCSprite * mainImg;
  CCSprite * edge;
  bool load_removing;
  bool shoot_removing;
  CCPoint load_dv;
  CCPoint shoot_dv;
  float shoot_dv_distance;
  int ingFrame;
  bool is_spin;
  string type_name;
  void stopMoving ();
  void moving ();
  void myInit (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p);
  void myInit (CCNode * t_target, int jm_type, float missile_speed);
  void realInit (CCNode * t_target, int jm_type, float missile_speed);
};
class JM_BasicMissile : public JackMissile
{
public:
  static JM_BasicMissile * create (CCNode * t_target, int jm_type, float missile_speed);
  static JM_BasicMissile * create (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p);
  virtual ~ JM_BasicMissile ();
  virtual void startMoving ();
private:
  float directionAngle;
  float myJM_SPEED;
  float myJM_CHANGE_DIRECTION_VAL;
  CCSprite * mainImg;
  bool is_spin;
  int ing_miss_counting;
  void moving ();
  void myInit (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p);
  void setStartPosition (CCPoint s_p);
  void myInit (CCNode * t_target, int jm_type, float missile_speed);
  void realInit (CCNode * t_target, int jm_type, float missile_speed);
};
#undef LZZ_INLINE
#endif
