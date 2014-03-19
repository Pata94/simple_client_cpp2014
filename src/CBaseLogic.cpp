/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#include "CBaseLogic.h"
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include "CGameHandler.h"
using namespace std;
CBaseLogic::CBaseLogic(int Player)
{
    m_Player=Player;
    m_pGameState = 0;
    m_pBestMoveC = 0;
}

CBaseLogic::~CBaseLogic()
{
    //dtor
}



void CBaseLogic::OnRequestAction(CGameState::CMoveContainer **ppMoves)
{



    printf("MoveRequest  ");
    CGameState::CMoveContainer *possibleMoves=m_pGameState->GetPossibleMoves(m_Player);

      if(possibleMoves->m_MoveType == MOVE_PLACE_FIRST)
        {
            *ppMoves = new CGameState::CMoveContainer();
            for(int i = 0; i < possibleMoves->m_lpMoves.size(); ++i)
            {
                  (*ppMoves)->m_MoveType = MOVE_PLACE;
                (*ppMoves)->m_lpMoves.push_back(possibleMoves->m_lpMoves[i]->Clone());
            }
             delete possibleMoves;
            return;
        }
     if(possibleMoves->m_lpMoves.size()==0)
        {
            *ppMoves = new CGameState::CMoveContainer();
            printf("No possible Moves");
             CGameState::CMove *pMove = new CGameState::CMove();

            pMove->m_pStone = new CStoneHandler::CStone;

            *pMove->m_pStone = *m_pGameState->m_apHandStones[m_Player*6];
            pMove->m_Mode = MOVE_EXCHANGE;
           // pMove->m_FieldIndex = i;
            (*ppMoves)->m_MoveType = MOVE_EXCHANGE;
            (*ppMoves)->m_lpMoves.push_back(pMove);
             delete possibleMoves;
            return;
        }

    m_BestPoints = 0;
    CGameState::CMoveContainer *pTemp = new CGameState::CMoveContainer();
    pTemp->m_MoveType = MOVE_PLACE;
    CGameState *pTempState = m_pGameState->Clone();
    TestFunc(pTempState, pTemp);
   /* CGameState::CMove *pTemp = new CGameState::CMove();
    *pTemp = *(pointMoves.front().ppMove);*/
    *ppMoves = m_pBestMoveC;
    m_pBestMoveC = 0;
    delete pTemp;
    delete pTempState;
    delete possibleMoves;
  // pMoves->m_MoveType = MOVE_PLACE;
  /* pMoves->m_lpMoves.clear();
   for(int i = 0; i < m_pBestMoveC->m_lpMoves.size(); ++i)
   {
       pMoves->m_lpMoves.push_back(m_pBestMoveC->m_lpMoves[i]->Copy());
   }*/

    //Move muss jetzt nur noch ausgeführt werden
    //sort function needs to be properly implemented
    /*for(int i = 0; i <3; ++i)
    {
        if(possibleMoves.size()==0)
            break;
        srand (time(NULL));
        CGameState::CMove* tempMove = possibleMoves[(rand()%possibleMoves.size())];
        printf("\n Zug: %d, %d",i, m_pGameState->DoMove(tempMove));
         aMoves[i]=tempMove;

    printf("MoveRequest %d \n", m_Player);
    {
        CGameState::CMoveContainer *possibleMoves=m_pGameState->GetPossibleMoves(m_Player);
        if(possibleMoves->m_lpMoves.size()==0)
        {
            printf("No possible Moves");
             CGameState::CMove *pMove = new CGameState::CMove();
            pMove->m_pStone = m_pGameState->m_apHandStones[m_Player*6];
            pMove->m_Mode = MOVE_EXCHANGE;
           // pMove->m_FieldIndex = i;
            pMoves->m_MoveType = MOVE_EXCHANGE;
         pMoves->m_lpMoves.push_back(pMove);
            return;
        }
        if(possibleMoves->m_MoveType == MOVE_PLACE_FIRST)
        {
            for(int i = 0; i < possibleMoves->m_lpMoves.size(); ++i)
            {
                  pMoves->m_MoveType = MOVE_PLACE;
                pMoves->m_lpMoves.push_back(possibleMoves->m_lpMoves[i]);
            }
        }
        else
        {

            srand (time(NULL));
            CGameState::CMove* tempMove = (possibleMoves->m_lpMoves)[(rand()%possibleMoves->m_lpMoves.size())];
            printf("\n Zug:  %d", m_pGameState->DoMove(tempMove));
            pMoves->m_MoveType = MOVE_PLACE;
             pMoves->m_lpMoves.push_back(tempMove);
        }
*/
}

/*CGameState::CMove* CBaseLogic::GetBestMove(CGameState::CMoveContainer *pMoves)
{

}*/
int CBaseLogic::TestGameState(CGameState *pState, CGameState::CMoveContainer* pMoveC)
{
    return pMoveC->GetPoints();
}

void CBaseLogic::TestFunc(CGameState *pState, CGameState::CMoveContainer* pMoveC)
{

      CGameState::CMoveContainer *possibleMoves=pState->GetPossibleMoves(m_Player);

 fflush(stdout);
        if(pMoveC->m_lpMoves.size()>0)
        {
            if(pMoveC->m_lpMoves.size()>1)
            {
                int asd=pMoveC->m_lpMoves.size();
            }
            int points = TestGameState(pState, pMoveC);
            if(m_BestPoints < points)
            {
                if(m_pBestMoveC)
                    delete m_pBestMoveC;
                m_pBestMoveC = pMoveC->Clone();
                m_BestPoints = points;
            }
        }

 fflush(stdout);
        for(int i = 0; i < possibleMoves->m_lpMoves.size(); ++i)
        {
            CGameState::CMove* pTemp = possibleMoves->m_lpMoves[i]->Clone();
            CGameState* pTempState = pState->Clone();
             CGameState::CMoveContainer *ABD = new CGameState::CMoveContainer();

             ABD->m_MoveType = MOVE_PLACE;
             ABD->m_lpMoves.push_back(pTemp);
            pTempState->DoMove(ABD);
            ABD->m_lpMoves.clear();
            delete ABD;

            CGameState::CMoveContainer *pTempContainer = pMoveC->Clone();

            pTempContainer->m_lpMoves.push_back(pTemp);
            TestFunc(pTempState, pTempContainer);
            delete pTempState;
            delete pTempContainer;

        }


    delete possibleMoves;

}
void CBaseLogic::OnGameStateUpdate(CGameState *pNewState)
{
    printf("GameStateUpdate");
    if(m_pGameState == NULL)
        m_pGameState = pNewState;
    else
    {
        delete m_pGameState;
        m_pGameState = pNewState;
    }
   // if(pNewState != NULL)
       // pNewState->CopyGameState(m_pGameState);
}

