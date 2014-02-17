//
//  NewPieceManager.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 2. 17..
//
//

#include "NewPieceManager.h"
#include "NewPuzzlePiece.h"
#include "ServerDataSave.h"

NewPieceSelected::~NewPieceSelected()
{
	NewPieceManager::sharedInstance()->removeSelectedPiece(this);
}

void NewPieceManager::regiSelectedPiece(int t_stage)
{
	if(selected_piece)
	{
		selected_piece->removeFromParent();
		selected_piece = NULL;
	}
	
	NewPuzzlePiece* t_piece = (NewPuzzlePiece*)piece_list[t_stage];
	
	int piece_no = NSDS_GI(NSDS_GI(t_stage, kSDS_SI_puzzle_i), kSDS_PZ_stage_int1_pieceNo_i, t_stage);
	
	int x,y;
	
	x = (piece_no-1)%6;
	y = (piece_no-1)/6;
	
	string piece_direction;
	if((x+y)%2 == 0)
		piece_direction = "h";
	else
		piece_direction = "w";
	
	NewPieceSelected* selected_img = NewPieceSelected::create(CCString::createWithFormat("temp_piece_selected_%s.png", piece_direction.c_str())->getCString());
	selected_img->setPosition(t_piece->getPosition());
	t_piece->getParent()->addChild(selected_img);
	
	selected_piece = selected_img;
}