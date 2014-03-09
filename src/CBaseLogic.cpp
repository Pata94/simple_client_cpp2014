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
}

CBaseLogic::~CBaseLogic()
{
    //dtor
}


void CBaseLogic::OnRequestAction(CGameState::CMoveContainer *pMoves)
{
    printf("MoveRequest");
    CGameState::CMoveContainer *possibleMoves=m_pGameState->GetPossibleMoves(m_Player);
    vector<Points> pointMoves;
    Points a;
    for(CGameState::CMove *pmove : possibleMoves->m_lpMoves) //Goes throug every move from possible move with a range-based for loop
    {
        a.points = m_pGameState->getPoints(pmove);
        a.ppMove = pmove;

        pointMoves.push_back(a);
    }
    std::sort(pointMoves.begin(), pointMoves.end(), std::greater<Points>());
    m_pGameState->DoMove(pointMoves.front().ppMove);
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


void CBaseLogic::OnGameStateUpdate(CGameState *pNewState)
{
    printf("GameStateUpdate");
    if(pNewState != 0)
        m_pGameState=pNewState;
}

