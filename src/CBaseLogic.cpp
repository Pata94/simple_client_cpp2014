/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#include "CBaseLogic.h"
#include <stdio.h>
#include <algorithm>

#include <time.h>
#include <stdlib.h>
#include "CGameHandler.h"
#include "CGameState.h"
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

static int num = 0;


/*int CBaseLogic::GetCardValue(CGameState *pState, CStoneHandler::CStone *pStone)
{
     int points = 0;
     CFieldHandler *pTemp = pState->m_pFieldHandler->Clone();
     pTemp->NewRound(); //delete restrictions
     for(int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; ++i)
                {
                    int p = pTemp->CanPlace( i, pStone);
                    if(p > 0)
                    {
                        if(p > points)
                            points = p;
                        else
                            points += (int)(p*0.3f);
                    }
                }
    delete pTemp;
    return points;
}*/

float CBaseLogic::GetStoneProbability(CGameState *pState, CStoneHandler::CStone *pStone)
{
    for(int i = 0; i < 12; ++i)
    {
        if(pState->m_apHandStones[i] != 0)
            if(pState->m_apHandStones[i]->m_Color == pStone->m_Color && pState->m_apHandStones[i]->m_Shape == pStone->m_Shape)
                return 1.0f; //stone on a hand
    }
    for(int i = 0; i < 12; ++i)
    {
        if(pState->m_apOpenStones[i] != 0)
            if(pState->m_apOpenStones[i]->m_Color == pStone->m_Color && pState->m_apOpenStones[i]->m_Shape == pStone->m_Shape)
            {
                if(i > 5)
                    return 1.0f-i*0.05f; //stone on a hand
                return 0.9f;
            }
    }

    int num = 0;
      for(int a = 0; a < FIELD_WIDTH*FIELD_HEIGHT; ++a)
      {
           if(!pState->m_pFieldHandler->IsFree(a))
           {
               if(pState->m_pFieldHandler->m_aField[a].m_pStone->m_Color == pStone->m_Color && pState->m_pFieldHandler->m_aField[a].m_pStone->m_Shape == pStone->m_Shape)
                    num++;
           }
      }

    if(pState->m_NumBagStones <= 0)
        return 0.0f;
    num = 3 - num;
    return ((float)num/(float)pState->m_NumBagStones)*0.5f;
}

float CBaseLogic::RekursiveFunktion(CFieldHandler *pField, float currentProbability, float (&aProbabilities)[NUM_COLORS*NUM_SHAPES], float step)
{

    CMoveHandler::CMoveContainer *pMoveContainer = pField->GetPossibleMoves();
     printf(",%i", pMoveContainer->m_lpMoves.size());
    float points = 0;
    for(int i = 0; i < pMoveContainer->m_lpMoves.size(); i++)
    {

       CFieldHandler *pTempField = pField->Clone();

        if(!pMoveContainer->m_lpMoves[i])
             printf("Move NULL");
        if(!pMoveContainer->m_lpMoves[i]->m_pStone)
             printf("stone NULL");

         int a2 = pMoveContainer->m_lpMoves[i]->m_pStone->m_Color+pMoveContainer->m_lpMoves[i]->m_pStone->m_Shape*NUM_COLORS;


        float val1 = pTempField->CanPlace(pMoveContainer->m_lpMoves[i]->m_FieldIndex, pMoveContainer->m_lpMoves[i]->m_pStone)*aProbabilities[a2];
        if(val1 > 0.0f)
            {

                pTempField->PlaceStone(pMoveContainer->m_lpMoves[i]->m_FieldIndex, pMoveContainer->m_lpMoves[i]->m_pStone, true);

                float val2 = RekursiveFunktion(pTempField, currentProbability*aProbabilities[a2],  aProbabilities, step);
                                   float val3 = val1+val2;
                 if( m_aValues[a2] < val3)
                        m_aValues[a2] = val3;
                points += val3*step;

            }

        delete pTempField;

    }
    delete pMoveContainer;

    return points;
}
int CBaseLogic::GetStoneValues(CGameState *pState)
{
    CFieldHandler *pTemp = pState->m_pFieldHandler->Clone();
    pTemp->NewRound();

    pTemp->InitPossibleMoves();
    float aProbabilities[NUM_COLORS*NUM_SHAPES] = {0.0f};

   for(int i = 0; i < NUM_COLORS*NUM_SHAPES; ++i)
    {
       // apStones[i] = new CStoneHandler::CStone();
       CStoneHandler::CStone Stone;
        Stone.m_Color = i%NUM_COLORS;
        Stone.m_Shape = (i-Stone.m_Color)/NUM_COLORS;
        aProbabilities[i] = GetStoneProbability(pState, &Stone);
        m_aValues[i] = 0.0f;
    }
    float step = 0.5f;

    RekursiveFunktion(pTemp, 1.0f, aProbabilities, step);

   for(int i = 0; i < NUM_COLORS*NUM_SHAPES; ++i)
    {
        int color = i%NUM_COLORS;
        int shape = (i-color)/NUM_COLORS;

        printf("// C: %s, S: %s, %f, %f",CGameState::m_aColorNames[color], CGameState::m_aShapeNames[shape], m_aValues[i], aProbabilities[i] );
    }
    delete pTemp;
}

void CBaseLogic::OnRequestAction(CMoveHandler::CMoveContainer **ppMoves)
{



    printf("MoveRequest  ");
    CMoveHandler::CMoveContainer *possibleMoves=m_pGameState->GetPossibleMoves(m_Player);

    if(possibleMoves->m_lpMoves.size()==0)
        {
            *ppMoves = new CMoveHandler::CMoveContainer();
            printf("No possible Moves");

           // pMove->m_FieldIndex = i;
            (*ppMoves)->m_MoveType = MOVE_EXCHANGE;
            for(int i = 0; i < 6; ++i)
            {
                CMoveHandler::CMove *pMove = new CMoveHandler::CMove();
                pMove->m_pStone = m_pGameState->m_apHandStones[m_Player*6+i]->Clone();
                pMove->m_Mode = MOVE_EXCHANGE;
                pMove->m_CardIndex = m_Player*6+i;
                (*ppMoves)->m_lpMoves.add(pMove);
            }

             delete possibleMoves;
            return;
        }
      if(possibleMoves->m_MoveType == MOVE_PLACE_FIRST)
        {
            *ppMoves = new CMoveHandler::CMoveContainer();
            for(int i = 0; i < possibleMoves->m_lpMoves.size(); ++i)
            {
                  (*ppMoves)->m_MoveType = MOVE_PLACE;
                (*ppMoves)->m_lpMoves.add(possibleMoves->m_lpMoves[i]->Clone());
            }
             delete possibleMoves;
            return;
        }


    m_BestPoints = -9999999;
    num = 0;
    CMoveHandler::CMoveContainer *pTemp = new CMoveHandler::CMoveContainer();
    pTemp->m_MoveType = MOVE_PLACE;
    CGameState *pTempState = m_pGameState->Clone();
    GetStoneValues(pTempState);
    TestFunc(pTempState, pTemp);
   /* CMoveHandler::CMove *pTemp = new CMoveHandler::CMove();
    *pTemp = *(pointMoves.front().ppMove);*/
    *ppMoves = m_pBestMoveC;
    m_pBestMoveC = 0;
    delete pTemp;
    delete pTempState;
    delete possibleMoves;

    printf("NUM: %i", num);
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
        CMoveHandler::CMove* tempMove = possibleMoves[(rand()%possibleMoves.size())];
        printf("\n Zug: %d, %d",i, m_pGameState->DoMove(tempMove));
         aMoves[i]=tempMove;

    printf("MoveRequest %d \n", m_Player);
    {
        CMoveHandler::CMoveContainer *possibleMoves=m_pGameState->GetPossibleMoves(m_Player);
        if(possibleMoves->m_lpMoves.size()==0)
        {
            printf("No possible Moves");
             CMoveHandler::CMove *pMove = new CMoveHandler::CMove();
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
            CMoveHandler::CMove* tempMove = (possibleMoves->m_lpMoves)[(rand()%possibleMoves->m_lpMoves.size())];
            printf("\n Zug:  %d", m_pGameState->DoMove(tempMove));
            pMoves->m_MoveType = MOVE_PLACE;
             pMoves->m_lpMoves.push_back(tempMove);
        }
*/
}

/*CMoveHandler::CMove* CBaseLogic::GetBestMove(CMoveHandler::CMoveContainer *pMoves)
{

}*/
float CBaseLogic::GetStoneValue(CStoneHandler::CStone *pStone)
{
    int index = pStone->m_Color+  pStone->m_Shape*NUM_COLORS;
    return m_aValues[index];
}
float CBaseLogic::GetHandCardValues(CGameState *pState, int player)
{
    int points = 0;
    for(int i = player*6; i < player*6+6; ++i)
    {
        if(pState->m_apHandStones[i] != 0)
        {
            points += GetStoneValue(pState->m_apHandStones[i]);
        }
    }
    return points;
}
int CBaseLogic::TestGameState(CGameState *pState, CMoveHandler::CMoveContainer* pMoveC)
{


    int points = pMoveC->GetPoints()*7;
   // points -= GetHandCardValues(pState, m_Player == 0 ? 1 : 0);
   CGameState *pTemp = pState->Clone();
   pTemp->EndRound();

    for(int i = 0; i < 6; ++i)
    {
        if(pTemp->m_apOpenStones[i] != 0)
            points -= GetStoneValue(pTemp->m_apOpenStones[i]);
    }
    points += GetHandCardValues(pTemp, pState->m_CurrentPlayer)*0.5;
    num++;
    return points;
}

void CBaseLogic::TestFunc(CGameState *pState, CMoveHandler::CMoveContainer* pMoveC)
{

      CMoveHandler::CMoveContainer *possibleMoves=pState->GetPossibleMoves(m_Player);
        if(pMoveC->m_lpMoves.size()>0)
        {
            int points = TestGameState(pState, pMoveC);
            if(m_BestPoints < points)
            {
                if(m_pBestMoveC)
                    delete m_pBestMoveC;
                m_pBestMoveC = pMoveC->Clone();
                m_BestPoints = points;
            }
        }
        for(int i = 0; i < possibleMoves->m_lpMoves.size(); ++i)
        {
            CMoveHandler::CMove* pTemp = possibleMoves->m_lpMoves[i]->Clone();
            CGameState* pTempState = pState->Clone();
             CMoveHandler::CMoveContainer *ABD = new CMoveHandler::CMoveContainer();

             ABD->m_MoveType = MOVE_PLACE;
             ABD->m_lpMoves.add(pTemp);
            pTempState->DoMove(ABD);
            ABD->m_lpMoves.clear();
            delete ABD;

            CMoveHandler::CMoveContainer *pTempContainer = pMoveC->Clone();

            pTempContainer->m_lpMoves.add(pTemp);
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

