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
}

CBaseLogic::~CBaseLogic()
{
    //dtor
}


void CBaseLogic::OnRequestAction(CGameState::CMoveContainer *pMoves)
{
    printf("MoveRequest");
    CGameState::CMoveContainer *possibleMoves=m_pGameState->GetPossibleMoves(m_Player);

      if(possibleMoves->m_MoveType == MOVE_PLACE_FIRST)
        {
            for(int i = 0; i < possibleMoves->m_lpMoves.size(); ++i)
            {
                  pMoves->m_MoveType = MOVE_PLACE;
                pMoves->m_lpMoves.push_back(possibleMoves->m_lpMoves[i]);
            }
            return;
        }
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
    CGameState::CMoveContainer *possibleEnemyMoves=m_pGameState->GetPossibleMoves(!m_Player);
    CGameState::CMoveContainer *interestingEnemyMoves;
    CGameState::CMoveContainer *differentMoves;
    CPoints a;
    vector<CPoints> pointMoves;
    if(possibleEnemyMoves->m_lpMoves.size() != 0)
    {
        std::sort(possibleMoves->m_lpMoves.begin(), possibleMoves->m_lpMoves.end());
        std::sort(possibleEnemyMoves->m_lpMoves.begin(), possibleEnemyMoves->m_lpMoves.end());
        interestingEnemyMoves->m_MoveType == MOVE_PLACE;
        std::set_intersection(possibleMoves->m_lpMoves.begin(), possibleMoves->m_lpMoves.end(), possibleEnemyMoves->m_lpMoves.begin(), possibleEnemyMoves->m_lpMoves.end(), back_inserter(interestingEnemyMoves->m_lpMoves));
        for(CGameState::CMove *pmove: interestingEnemyMoves->m_lpMoves)
        {
            a.points = m_pGameState->getPoints(pmove);
            a.ppMove = pmove;
            pointMoves.push_back(a);
        }
        std::sort(pointMoves.begin(), pointMoves.end());
        pointMoves[0].points = 2 * pointMoves[0].points - pointMoves[1].points;
        differentMoves->m_MoveType = MOVE_PLACE;
        std::set_difference(possibleMoves->m_lpMoves.begin(), possibleMoves->m_lpMoves.end(), interestingEnemyMoves->m_lpMoves.begin(), interestingEnemyMoves->m_lpMoves.end(), back_inserter(differentMoves->m_lpMoves));
        for(CGameState::CMove *pmove: differentMoves->m_lpMoves)
        {
            a.points = m_pGameState->getPoints(pmove);
            a.ppMove = pmove;
            pointMoves.push_back(a);
        }
    }
    else
    {
        for(CGameState::CMove *pmove : possibleMoves->m_lpMoves) //Goes throug every move from possible move with a range-based for loop
        {
            a.points = m_pGameState->getPoints(pmove);
            a.ppMove = pmove;

            pointMoves.push_back(a);
        }
    }
    std::sort(pointMoves.begin(), pointMoves.end(), std::greater<CPoints>());
    // m_pGameState->DoMove();
    CGameState::CMove *pTemp = new CGameState::CMove();
    *pTemp = *(pointMoves.front().ppMove);
    pMoves->m_lpMoves.push_back(pTemp);
    pMoves->m_MoveType = MOVE_PLACE;
    printf("Send Move");
    //Move muss jetzt nur noch ausgeführt werden
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

void CBaseLogic::OnGameStateUpdate(CGameState *pNewState)
{
    printf("GameStateUpdate");
  //  if(m_pGameState == NULL)
        m_pGameState = pNewState;
    //if(pNewState != NULL)
       // pNewState->CopyGameState(m_pGameState);
}

