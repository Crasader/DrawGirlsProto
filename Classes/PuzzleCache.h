// PuzzleCache.h
//

#ifndef LZZ_PuzzleCache_h
#define LZZ_PuzzleCache_h
#include <iostream>
#include <vector>
#include "cocos2d.h"
#include <thread>
USING_NS_CC;
using namespace std;
#define LZZ_INLINE inline
enum PuzzleCachePieceType
{
  kPuzzleCachePieceType_left = 1000,
  kPuzzleCachePieceType_right,
  kPuzzleCachePieceType_top,
  kPuzzleCachePieceType_bottom
};


class PuzzleImageData
{
private:
  unsigned char * m_data;
  unsigned long int m_size;
public:
  void loadImageDataWithFullpath (string fullpath);
  void loadImageData (string filename);
  unsigned char * getData ();
  unsigned long int getDataLen ();
};
class PuzzleImage : public CCImage
{
private:
  float m_posX;
  float m_posY;
  int m_puzzleNo;
  int m_pieceNo;
  string m_puzzleKey;
  bool m_isLoaded;
public:
  PuzzleImage ();
  static PuzzleImage * create (string path);
  static PuzzleImage * createWithImageFileThreadSafe (string fullpath);
  static PuzzleImage * createWithImageData (unsigned char * data, unsigned long int size);
  static PuzzleImage * createWithPuzzleImageData (PuzzleImageData * puzzleImageData);
  bool initWithPuzzleImageData (PuzzleImageData * puzzleImageData);
  bool isLoaded ();
  void setLoaded (bool isLoaded);
  void setCutPoint (float x, float y);
  CCPoint getCutPoint ();
  int getPieceNo ();
  int getPuzzleNo ();
  void setPuzzleNo (int puzzleNo);
  void setPieceNo (int pieceNo);
  void setPuzzleKey (string puzzlekey);
  string getPuzzleKey ();
  CCTexture2D * makeTexture ();
};
class PuzzleCache : public CCObject
{
private:
  map <string,PuzzleImage*> m_textureList;
  list <PuzzleImage*> m_loadingPuzzleList;
  map <int,string> m_puzzleState;
  bool isLockedTextureList;
  bool isLockedLoadingPuzzleList;
  std::function <void(PuzzleImage*)> m_callbackfunc;
  int m_currentLoadPuzzleNo;
  bool m_useTread;
  string m_documentPath;
public:
  PuzzleCache ();
  void loadImage (int puzzleNo);
  void cancelLoadingImage (int puzzleNo);
  void setLockedLoadingPuzzleList (bool locked);
  void waitForLoadingPuzzleList (bool locked);
  void addTextureList (string key, PuzzleImage * texture);
  void addLoadingPuzzleList (PuzzleImage * image);
  struct PuzzlePoint
  {
    int x;
    int y;
  };
  static PuzzleCache * getInstance ();
  void loadingPuzzle (float dt);
  void loadImageWithCallback (int puzzleNo, std::function <void(PuzzleImage*)> func);
  bool checkCancel (int loadingPuzzleNo);
  void callbackLoadedImage (int puzzleNo);
  void startToLoadImage (int puzzleNo);
  PuzzleImage * getImage (string key);
  void removeUnuseImage ();
  void removeUnusePuzzle (int puzzleNo);
  void logTextureKeys ();
  void changeStencilByOrigin (PuzzleImage * stencil, PuzzleImage * origin, PuzzlePoint cutPointInOrigin, bool isGLcoordinate);
  PuzzleImage * cutImageByStensil (CCImage * origin, CCImage * stencil, PuzzlePoint cutPointInOrigin, bool isGLcoordinate);
};
#undef LZZ_INLINE
#endif
