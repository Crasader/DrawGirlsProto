// StagePiece.h
//

#ifndef LZZ_StagePiece_h
#define LZZ_StagePiece_h
#include "cocos2d.h"
#include "StageImgLoader.h"
#include "DataStorageHub.h"
#include "GraySprite.h"
#include "ScrollMenu.h"
#include "PuzzleCache.h"
#include "ServerDataSave.h"
#include "StarGoldData.h"

USING_NS_CC;
#define LZZ_INLINE inline
enum PuzzleMode
{
  kPM_default = 0,
  kPM_thumb
};
class StagePiece : public CCNode
{
public:
  static StagePiece * create (string t_piece, int t_number, int t_level, CCPoint t_p, CCRect t_rect, bool t_gray, bool t_boarder, string t_type, CCObject * t_t, SEL_MenuHandler t_d);
  bool isInnerRect (CCPoint t_p);
  void setChangable (string t_thumb, bool t_card1, bool t_card2, bool t_card3);
  bool isChangable ();
  void setBackPuzzle ();
  void setPuzzleMode (PuzzleMode t_mode);
  void mySetTouchEnable (bool t_b);
  bool touchBegan (CCTouch * touch, CCEvent * event);
  void touchMoved (CCTouch * touch, CCEvent * event);
  void touchEnded (CCTouch * touch, CCEvent * event);
  void touchCancelled (CCTouch * touch, CCEvent * event);
  int getStageNumber ();
  void setTouchBegin ();
  void setTouchCancel ();
  bool isBoarder ();
  void startGetPieceAnimation (CCObject * t_create_particle, SEL_CallFuncCCp d_create_particle);
  CCSprite * shadow_node;
private:
  PuzzleMode get_animation_mode;
  GraySprite * piece_img;
  CCObject * target_create_particle;
  SEL_CallFuncCCp delegate_create_particle;
  string piece_name;
  int stage_number;
  int stage_level;
  CCRect touch_rect;
  bool is_gray;
  bool is_boarder;
  string piece_type;
  bool is_changable;
  string thumb_name;
  bool (is_have_card) [3];
  ScrollMenu * s_menu;
  void originalMode ();
  void showParticle ();
  void myChangeAction ();
  void myInit (string t_piece, int t_number, int t_level, CCPoint t_p, CCRect t_rect, bool t_gray, bool t_boarder, string t_type, CCObject * t_t, SEL_MenuHandler t_d);
};
#undef LZZ_INLINE
#endif
