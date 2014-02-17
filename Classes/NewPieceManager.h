//
//  NewPieceManager.h
//  DGproto
//
//  Created by 사원3 on 2014. 2. 17..
//
//

#ifndef __DGproto__NewPieceManager__
#define __DGproto__NewPieceManager__

#include "cocos2d.h"
#include <map>

USING_NS_CC;
using namespace std;

class NewPieceSelected : public CCSprite
{
public:
	static NewPieceSelected* create(string filename)
	{
		NewPieceSelected* t_nps = new NewPieceSelected();
		t_nps->initWithFile(filename.c_str());
		t_nps->autorelease();
		return t_nps;
	}
	
	virtual ~NewPieceSelected();
	
private:
	
};

class NewPieceManager
{
public:
	static NewPieceManager* sharedInstance()
	{
		static NewPieceManager* t_npm = NULL;
		if(!t_npm)
		{
			t_npm = new NewPieceManager();
			t_npm->myInit();
		}
		
		return t_npm;
	}
	
	void addNewPiece(int t_stage, CCNode* t_piece)
	{
		piece_list[t_stage] = t_piece;
	}
	
	void removeNewPiece(int t_stage, CCNode* t_piece)
	{
		map<int, CCNode*>::iterator iter = piece_list.find(t_stage);
		if(iter != piece_list.end())
			piece_list.erase(iter);
	}
	
	void resetPieceList()
	{
		piece_list.clear();
	}
	
	void regiSelectedPiece(int t_stage);
	void removeSelectedPiece(NewPieceSelected* t_selected_img)
	{
		if(t_selected_img == selected_piece)
			selected_piece = NULL;
	}
	
private:
	
	map<int, CCNode*> piece_list;
	NewPieceSelected* selected_piece;
	
	void myInit()
	{
		resetPieceList();
		selected_piece = NULL;
	}
};

#endif /* defined(__DGproto__NewPieceManager__) */
