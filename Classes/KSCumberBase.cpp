//
//  KSCumberBase.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 6..
//
//

#include "KSCumberBase.h"

void KSCumberBase::crashMapForIntPoint( IntPoint t_p )
{
	IntPoint jackPoint = myGD->getJackPoint();

	if(t_p.isInnerMap() && (myGD->mapState[t_p.x][t_p.y] == mapOldline || myGD->mapState[t_p.x][t_p.y] == mapOldget)) // just moment, only map crash
	{
		myGD->mapState[t_p.x][t_p.y] = mapEmpty;
		for(int k = -1;k<=1;k++)
		{
			for(int l = -1;l<=1;l++)
			{
				if(k == 0 && l == 0)	continue;
				if(myGD->mapState[t_p.x+k][t_p.y+l] == mapOldget)		myGD->mapState[t_p.x+k][t_p.y+l] = mapOldline;
			}
		}
		myGD->communication("EP_crashed");
		myGD->communication("MFP_createNewFragment", t_p);
		myGD->communication("VS_divideRect", t_p);
	}

	if(jackPoint.x == t_p.x && jackPoint.y == t_p.y)
	{
		//			showEmotion(kEmotionType_fun);
		myGD->communication("Jack_startDieEffect");
		//			if(isFuryMode) //##
		//				furyModeOff();
	}

	if(t_p.isInnerMap() && myGD->mapState[t_p.x][t_p.y] == mapNewline)
	{
		//					myGD->communication("PM_pathChainBomb", t_p);

		myGD->communication("Jack_startDieEffect");
		myGD->communication("Main_showLineDiePosition", t_p);
		//			if(isFuryMode) //##
		//				furyModeOff();
	}
}
