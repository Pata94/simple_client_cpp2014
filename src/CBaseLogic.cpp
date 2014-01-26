/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#include "CBaseLogic.h"
#include <stdio.h>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include "CGameHandler.h"
using namespace std;
CBaseLogic::CBaseLogic(int Player)
{
    m_Player=Player;
}

CBaseLogic::~CBaseLogic()
{
    //dtor
}


void CBaseLogic::OnRequestAction(CGameState::CMoveContainer *pMoves)
{
    printf("MoveRequest");
    {
        vector<CGameState::CMove*> *possibleMoves=m_pGameState->GetPossibleMoves(m_Player);
        if(possibleMoves->size()==0)
        {
            printf("No possible Moves");
             CGameState::CMove *pMove = new CGameState::CMove();
            pMove->m_pStone = m_pGameState->m_apHandStones[m_Player*6];
            pMove->m_Mode = CGameHandler::MODE_EXCHANGE;
           // pMove->m_FieldIndex = i;
            pMoves->m_MoveType = CGameHandler::MODE_EXCHANGE;
         pMoves->m_lpMoves.push_back(pMove);
            return;
        }
        srand (time(NULL));
        CGameState::CMove* tempMove = (*possibleMoves)[(rand()%possibleMoves->size())];
        printf("\n Zug:  %d", m_pGameState->DoMove(tempMove));
        pMoves->m_MoveType = CGameHandler::MODE_PLACE;
         pMoves->m_lpMoves.push_back(tempMove);
    }
}
void CBaseLogic::OnGameStateUpdate(CGameState *pNewState)
{
    printf("GameStateUpdate");
    if(pNewState != 0)
        m_pGameState=pNewState;
}

