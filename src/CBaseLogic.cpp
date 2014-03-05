/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#include "CBaseLogic.h"
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <CGameState.h>
#include <CFieldHandler.h>
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
    vector<CGameState::CMove*> possibleMoves=m_pGameState->GetPossibleMoves();
    vector<Points> pointMoves;
    Points a;
    for(CGameState::CMove *pmove : possibleMoves) //Goes throug every move from possible move range-based for loop
    {
        a.points = CFieldHandler::getPoints(pmove);
        a.ppmove = pmove;
        pointMoves.push_back(a);
    }
    //sort function needs to be properly implemented
    for(int i = 0; i <3; ++i)
    {
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

bool CBaseLogic::comparePoints(const CBaseLogic::Points &a, const CBaseLogic::Points &b)
{
    return a.points < b.points;
}

