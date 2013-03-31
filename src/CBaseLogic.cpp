/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#include "CBaseLogic.h"
#include <stdio.h>
#include <vector>
#include <time.h>
#include <stdlib.h>
using namespace std;
CBaseLogic::CBaseLogic(int Player)
{
    m_Player=Player;
}

CBaseLogic::~CBaseLogic()
{
    //dtor
}


void CBaseLogic::OnRequestAction(CGameState::CMove *aMoves[3])
{
    printf("MoveRequest");
    for(int i = 0; i <3; ++i)
    {
        vector<CGameState::CMove*> possibleMoves=m_pGameState->GetPossibleMoves();
        if(possibleMoves.size()==0)
            break;
        srand (time(NULL));
        CGameState::CMove* tempMove = possibleMoves[(rand()%possibleMoves.size())];
        printf("\n Zug: %d, %d",i, m_pGameState->DoMove(tempMove));
         aMoves[i]=tempMove;
    }
}
void CBaseLogic::OnGameStateUpdate(CGameState *pNewState)
{
    printf("GameStateUpdate");
    if(pNewState != 0)
        m_pGameState=pNewState;
}

