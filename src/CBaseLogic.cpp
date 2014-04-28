/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#include "CBaseLogic.h"
#include <stdio.h>
#include <algorithm>


#include <stdlib.h>
#include "CGameHandler.h"
#include "CGameState.h"
#include "ext/range.h"
#include <time.h>
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

float CBaseLogic::RekursiveFunktion(CFieldHandler *pField, float currentProbability, float (&aProbabilities)[NUM_COLORS*NUM_SHAPES], float step, int var)
{

    CMoveHandler::CMoveContainer *pMoveContainer = pField->GetPossibleMoves();
//     printf(",%i", pMoveContainer->m_lpMoves.size());
    float points = 0;
    if(var < VAR_STONE_LOOPS-1 || (var < VAR_STONE_LOOPS && GetTime() < TIME_STONE_LOOPS))
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

                float val2 = RekursiveFunktion(pTempField, currentProbability*aProbabilities[a2],  aProbabilities, step, var+1);
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
    int var = 0;
    RekursiveFunktion(pTemp, 1.0f, aProbabilities, step, var);

   for(int i = 0; i < NUM_COLORS*NUM_SHAPES; ++i)
    {
        int color = i%NUM_COLORS;
        int shape = (i-color)/NUM_COLORS;

        //printf("// C: %s, S: %s, %f, %f",CGameState::m_aColorNames[color], CGameState::m_aShapeNames[shape], m_aValues[i], aProbabilities[i] );
    }
    delete pTemp;
}


vector<vector<CStoneHandler::CStone *>> CBaseLogic::NecessaryStonesForSixpack(CGameState *pState)
{
    bool fieldSearchedHorizontally[FIELD_WIDTH*FIELD_HEIGHT];
    bool fieldSearchedVertically[FIELD_WIDTH*FIELD_HEIGHT];
    vector<vector<CStoneHandler::CStone *>> Stones;
    int MODE;
    for(int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; i++)
    {
        if(!fieldSearchedVertically)
        {
            bool shapesNeeded[6];
            bool colorsNeeded[6];
            CStoneHandler::CStone currentStones[6];
            for(int a = i; a < FIELD_WIDTH * FIELD_HEIGHT; a = a + FIELD_WIDTH)
            {
                fieldSearchedVertically[a] = true;
                if(pState->m_pFieldHandler->IsFree(a))
                    break;
                currentStones[(a-i)/FIELD_WIDTH] = *(pState->m_pFieldHandler->m_aField[a].m_pStone);
            }
            if(currentStones[2].m_Color != COLOR_NONE) //ab einer Länge von 3 Objekten fängt der untere Prozess an
            {
                if(currentStones[0].m_Color == currentStones[1].m_Color)
                    MODE = MODE_COLOR;
                else
                    MODE = MODE_SHAPE;
                vector<CStoneHandler::CStone *> neededStones;
                if(MODE == MODE_COLOR)
                {
                    for(int b = 0; b < 6; b++)
                    {
                        switch (currentStones[b].m_Shape)
                        {
                            case SHAPE_ACORN:
                                shapesNeeded[0] = false;
                                break;
                            case SHAPE_BELL:
                                shapesNeeded[1] = false;
                                break;
                            case SHAPE_CLUBS:
                                shapesNeeded[2] = false;
                                break;
                            case SHAPE_DIAMOND:
                                shapesNeeded[3] = false;
                                break;
                            case SHAPE_HEART:
                                shapesNeeded[4] = false;
                                break;
                            case SHAPE_SPADES:
                                shapesNeeded[5] = false;
                                break;
                            default:
                                printf("WHAT THE FUCK IS GOING ON??????");
                                break;
                        }
                    }
                    for(int c = 0; c < 6; c++)
                    {
                        if(shapesNeeded[c])
                        {
                            CStoneHandler::CStone *neededStone = new CStoneHandler::CStone;
                            neededStone->m_Color = currentStones[0].m_Color;
                            neededStone->m_Shape = shapesNeeded[c];
                            neededStones.push_back(neededStone);
                        }
                    }
                }

                else
                {
                    for(int b = 0; b < 6; b++)
                    {
                        switch (currentStones[b].m_Color)
                        {
                            case COLOR_BLUE:
                                colorsNeeded[0] = false;
                                break;
                            case COLOR_GREEN:
                                colorsNeeded[1] = false;
                                break;
                            case COLOR_MAGENTA:
                                colorsNeeded[2] = false;
                                break;
                            case COLOR_ORANGE:
                                colorsNeeded[3] = false;
                                break;
                            case COLOR_VIOLET:
                                colorsNeeded[4] = false;
                                break;
                            case COLOR_YELLOW:
                                colorsNeeded[5] = false;
                                break;
                        }
                    }
                    for(int c = 0; c < 6; c++)
                    {
                        if(colorsNeeded[c])
                        {
                            CStoneHandler::CStone *neededStone = new CStoneHandler::CStone;
                            neededStone->m_Color = colorsNeeded[c];
                            neededStone->m_Shape = currentStones[0].m_Shape;
                            neededStones.push_back(neededStone);
                        }
                    }
                }
                Stones.push_back(neededStones);
            }
        }



        if(!fieldSearchedHorizontally)
        {
            CStoneHandler::CStone currentStones[6];
            bool shapesNeeded[6];
            bool colorsNeeded[6];
            for(int a = i; a < (i-i%FIELD_WIDTH)+FIELD_WIDTH; a++)
            {
                fieldSearchedHorizontally[a] = true;
                if(pState->m_pFieldHandler->IsFree(a))
                    break;
                currentStones[a-i] = *(pState->m_pFieldHandler->m_aField[a].m_pStone);
            }
            if(currentStones[2].m_Color != COLOR_NONE) //ab einer Länge von 3 Objekten fängt der untere Prozess an
            {
                if(currentStones[0].m_Color == currentStones[1].m_Color)
                    MODE = MODE_COLOR;
                else
                    MODE = MODE_SHAPE;
                vector<CStoneHandler::CStone *> neededStones;
                if(MODE == MODE_COLOR)
                {
                    for(int b = 0; b < 6; b++)
                    {
                        switch (currentStones[b].m_Shape)
                        {
                            case SHAPE_ACORN:
                                shapesNeeded[0] = false;
                                break;
                            case SHAPE_BELL:
                                shapesNeeded[1] = false;
                                break;
                            case SHAPE_CLUBS:
                                shapesNeeded[2] = false;
                                break;
                            case SHAPE_DIAMOND:
                                shapesNeeded[3] = false;
                                break;
                            case SHAPE_HEART:
                                shapesNeeded[4] = false;
                                break;
                            case SHAPE_SPADES:
                                shapesNeeded[5] = false;
                                break;
                        }
                    }
                    for(int c = 0; c < 6; c++)
                    {
                        if(shapesNeeded[c])
                        {
                            CStoneHandler::CStone *neededStone = new CStoneHandler::CStone;
                            neededStone->m_Color = currentStones[0].m_Color;
                            neededStone->m_Shape = shapesNeeded[c];
                            neededStones.push_back(neededStone);
                        }
                    }
                }

                else
                {
                    for(int b = 0; b < 6; b++)
                    {
                        switch (currentStones[b].m_Color)
                        {
                            case COLOR_BLUE:
                                colorsNeeded[0] = false;
                                break;
                            case COLOR_GREEN:
                                colorsNeeded[1] = false;
                                break;
                            case COLOR_MAGENTA:
                                colorsNeeded[2] = false;
                                break;
                            case COLOR_ORANGE:
                                colorsNeeded[3] = false;
                                break;
                            case COLOR_VIOLET:
                                colorsNeeded[4] = false;
                                break;
                            case COLOR_YELLOW:
                                colorsNeeded[5] = false;
                                break;
                        }
                    }
                    for(int c = 0; c < 6; c++)
                    {
                        if(colorsNeeded[c])
                        {
                            CStoneHandler::CStone *neededStone = new CStoneHandler::CStone;
                            neededStone->m_Color = colorsNeeded[c];
                            neededStone->m_Shape = currentStones[0].m_Shape;
                            neededStones.push_back(neededStone);
                        }
                    }
                }
                Stones.push_back(neededStones);
            }
        }
    }
    return Stones;
}


void CBaseLogic::StonesForInterestingSixpacks(CGameState *pState)
{
    vector<vector<CStoneHandler::CStone *>> Stones;
    vector<int> StonesInHandSpace;
    Stones = NecessaryStonesForSixpack(pState);
    CStoneHandler::CStone *existingStones = new CStoneHandler::CStone[11];

    for(vector<CStoneHandler::CStone *> StonePack : Stones)
    {
        int openStones = 0;
        int StonesInHand = 0;
        for(CStoneHandler::CStone *stone : StonePack)
        {
            bool found = false;
            for(int i = 0; i < 6; i++) //falls ein Stein in zwei Sixpacks benötigt wird, wird das im Moment noch nicht ordentlich berechnet
            {
                if(stone->m_Color == pState->m_apHandStones[pState->m_CurrentPlayer*6+i]->m_Color && stone->m_Shape == pState->m_apHandStones[pState->m_CurrentPlayer*6+i]->m_Shape)
                {
                    StonesInHand++;
                    StonesInHandSpace.push_back(pState->m_CurrentPlayer*6+i);
                    found = true;
                    continue;
                }
            }
            for(int i = 0; i < 6-StonesInHand; i++)
            {
                if(stone->m_Color == pState->m_apOpenStones[i]->m_Color && stone->m_Shape == pState->m_apOpenStones            [i]->m_Shape)
                {
                    openStones++;
                    found = true;
                    continue;
                }
            }
            if(!found)
            {
                StonesInHandSpace.clear();
                break;
            }
        }
        if(openStones != 0)
        {
            for(int index : StonesInHandSpace)
            {
                delete pState->m_apHandStones[index];
            }
        }

    }

}


void CBaseLogic::OnRequestAction(CMoveHandler::CMoveContainer **ppMoves)
{


  m_TimeStart = clock();
    printf("MoveRequest  ");
    StonesForInterestingSixpacks(m_pGameState);
    CMoveHandler::CMoveContainer *possibleMoves=m_pGameState->GetPossibleMoves(m_Player);


if(possibleMoves->m_MoveType == MOVE_PLACE_FIRST && possibleMoves->m_lpMoves.size()>0)
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

  CGameState *pTempState = m_pGameState->Clone();
    GetStoneValues(pTempState);
    if(possibleMoves->m_lpMoves.size()==0)
        {
            *ppMoves = new CMoveHandler::CMoveContainer();
            printf("No possible Moves");

           // pMove->m_FieldIndex = i;
            (*ppMoves)->m_MoveType = MOVE_EXCHANGE;


            //sort handstones

            bool update = true;
            while(update)
            {
                update = false;

                for(int i = 0; i < 5; ++i)
                {
                    bool bswap = false;
                    if(m_pGameState->m_apHandStones[m_Player*6+i] == 0 && m_pGameState->m_apHandStones[m_Player*6+i+1] != 0)
                        bswap = true;
                    else if(m_pGameState->m_apHandStones[m_Player*6+i+1] == 0)
                        bswap = false;
                    else
                    {
                        int a =   m_pGameState->m_apHandStones[m_Player*6+i]->m_Color+m_pGameState->m_apHandStones[m_Player*6+i]->m_Shape*NUM_COLORS;
                        int b =   m_pGameState->m_apHandStones[m_Player*6+i+1]->m_Color+m_pGameState->m_apHandStones[m_Player*6+i+1]->m_Shape*NUM_COLORS;
                        bswap = (m_aValues[a] > m_aValues[b]);
                    }
                    if(bswap)
                    {
                        CStoneHandler::CStone *p=m_pGameState->m_apHandStones[m_Player*6+i+1];
                        m_pGameState->m_apHandStones[m_Player*6+i+1] = m_pGameState->m_apHandStones[m_Player*6+i];
                        m_pGameState->m_apHandStones[m_Player*6+i] = p;
                        update = true;
                    }
                }

            }


              for(int i = 0; i < 6; ++i)
                {
                    if(m_pGameState->m_apHandStones[m_Player*6+i])
                    printf(" %i, %f", i, m_aValues[m_pGameState->m_apHandStones[m_Player*6+i]->m_Color+m_pGameState->m_apHandStones[m_Player*6+i]->m_Shape*NUM_COLORS]);
                }


                float valueA = m_aValues[m_pGameState->m_apHandStones[m_Player*6]->m_Color+m_pGameState->m_apHandStones[m_Player*6]->m_Shape*NUM_COLORS];
                float valueB = m_pGameState->m_apOpenStones[0] ? m_aValues[m_pGameState->m_apOpenStones[0]->m_Color+m_pGameState->m_apOpenStones[0]->m_Shape*NUM_COLORS] : 0.0f;
                int i = 1;

                while(i < 6)
                {
                    if(m_pGameState->m_apHandStones[m_Player*6+i] && m_pGameState->m_apOpenStones[i])
                    {
                       valueA += m_aValues[m_pGameState->m_apHandStones[m_Player*6+i]->m_Color+m_pGameState->m_apHandStones[m_Player*6+i]->m_Shape*NUM_COLORS];
                        valueB += m_aValues[m_pGameState->m_apOpenStones[i]->m_Color+m_pGameState->m_apOpenStones[i]->m_Shape*NUM_COLORS];

                        if(valueA < valueB*0.75f)
                            ++i;
                        else
                             break;
                    }
                    else
                        break;
                }
            //if(i > 1)
                --i;
            for(; i >= 0; --i)
            {
                CMoveHandler::CMove *pMove = new CMoveHandler::CMove();
                pMove->m_pStone = m_pGameState->m_apHandStones[m_Player*6+i]->Clone();
                pMove->m_Mode = MOVE_EXCHANGE;
                pMove->m_CardIndex = m_Player*6+i;
                (*ppMoves)->m_lpMoves.add(pMove);
            }

             delete possibleMoves;
             delete pTempState;
        }



    m_BestPoints = -9999999;
    num = 0;
    CMoveHandler::CMoveContainer *pTemp = new CMoveHandler::CMoveContainer();
    pTemp->m_MoveType = MOVE_PLACE;

    //pTempState->m_pFieldHandler->InitPossibleMoves();
    for(int i = 0; i < m_aTestStates.size(); ++i)
        m_aTestStates[i].release();

    TestFunc(pTempState, pTemp);

    if(m_aTestStates.size() > 24)
    {
        for(int i = m_aTestStates.size()-1; i > 23; --i)
        {
               m_aTestStates[i].release();
                m_aTestStates.remove_index_fast(i);
        }
    }

    CTest temp;
    temp.m_Points = 0;
    temp.m_pMoveContainer = new CMoveHandler::CMoveContainer();
    temp.m_pGameState = pTempState->Clone();
    m_aTestStates.add(temp);
    temp.m_pMoveContainer = 0;
    temp.m_pGameState = 0;

    if(pTempState->m_Turn < 39)
    {
        for(int i = 0; i < m_aTestStates.size() && i < 25 && GetTime() < TIME_ENEMYMOVE_LOOPS; ++i)
        {
            m_CurrentTestIndex = i;

            m_aTestStates[i].m_pGameState->EndRound();
            TestFunc(m_aTestStates[i].m_pGameState, m_aTestStates[i].m_pMoveContainer);
            m_aTestStates[i].m_Points -= m_aTestStates[i].m_BestEnemyPoints*CGameHandler::VAR_ENEMYPOINTS;
            printf("possible points: %i \n", m_aTestStates[i].m_BestEnemyPoints);
        }
        m_aTestStates.sort_range();

    }
     *ppMoves = m_aTestStates[0].m_pMoveContainer;
        m_aTestStates[0].m_pMoveContainer = 0;
   /* CMoveHandler::CMove *pTemp = new CMoveHandler::CMove();
    *pTemp = *(pointMoves.front().ppMove);*/
  //  *ppMoves = m_pBestMoveC;
    //m_pBestMoveC = 0;

    delete pTemp;
    delete pTempState;
    delete possibleMoves;
  printf ("|| %d ||\n",GetTime());
    printf("NU %i,,, %i/ %i [%i, %i]",m_aTestStates[0].m_BestEnemyPoints, m_aTestStates.size(), num, CGameState::NUM_GAMESTATES, CFieldHandler::NUM_HANDLERS);


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

    int points = pMoveC->GetPoints()*CGameHandler::VAR_OWNPOINTS;

   // points -= GetHandCardValues(pState, m_Player == 0 ? 1 : 0);

   CGameState *pTemp = pState->Clone();

   pTemp->EndRound();

    for(int i = 0; i < 6; ++i)
    {
        if(pTemp->m_apOpenStones[i] != 0)
            points -= GetStoneValue(pTemp->m_apOpenStones[i])*(1.2f-0.15f*(float)i);
    }
   // points -= GetHandCardValues(pTemp, pState->m_CurrentPlayer)*0.5;
   // points += GetHandCardValues(pTemp, pState->m_CurrentPlayer^1)*0.3;
    num++;
    printf("points %i\n", points);
    delete pTemp;

    return points;
}



void CBaseLogic::TestFunc(CGameState *pState, CMoveHandler::CMoveContainer* pMoveC)
{

      CMoveHandler::CMoveContainer *possibleMoves = pState->GetPossibleMoves(pState->m_CurrentPlayer);
   /*   CMoveHandler::CMoveContainer *possibleMovesB=pState->GetPossibleMoves(m_Player);
      int asd = 0;
      for(int i = 0; i < possibleMoves->m_lpMoves.size(); ++i)
      {
          if(possibleMoves->m_lpMoves[i]->m_pStone != 0)
          {
              for(int b = (m_Player*6); b < 6+m_Player*6; b++)
              {
                  if(pState->m_apHandStones[b] != 0 && pState->m_apHandStones[b]->m_Color == possibleMoves->m_lpMoves[i]->m_pStone->m_Color&& pState->m_apHandStones[b]->m_Shape == possibleMoves->m_lpMoves[i]->m_pStone->m_Shape)
                  {
                      ++asd;
                      //  printf("// C: %s, S: %s \n",CGameState::m_aColorNames[pState->m_apHandStones[b]->m_Color], CGameState::m_aShapeNames[pState->m_apHandStones[b]->m_Shape]);

                  }
              }
          }
      }*/

     //  printf("-------------------");
        /*for(int i = 0; i < possibleMoves->m_lpMoves.size(); ++i)
        {
            printf("// C: %s, S: %s \n",CGameState::m_aColorNames[pState->m_apHandStones[b]->m_Color], CGameState::m_aShapeNames[pState->m_apHandStones[b]->m_Shape]);
        }*/
   //   if( possibleMovesB->m_lpMoves.size() != asd)
     //  printf("ERROR! %i, %i ", possibleMovesB->m_lpMoves.size(), asd);

        if( pMoveC->m_lpMoves.size()>0)
        {
            if(pState->m_CurrentPlayer == m_Player )
            {
                //if(m_pBestMoveC)
               //     delete m_pBestMoveC;
                //m_pBestMoveC = pMoveC->Clone();
               // m_BestPoints = points;

                CTest temp;
                temp.m_pGameState = pState->Clone();

                temp.m_pMoveContainer = pMoveC->Clone();

                temp.m_Points = TestGameState(pState, pMoveC);

                temp.m_BestEnemyPoints = 0;
                m_aTestStates.add(temp);

                temp.m_pGameState = 0;
                temp.m_pMoveContainer= 0;

            }

                else if(pState->m_CurrentPlayer != m_Player)
                {

                    //we are testing enemy moves
                    int points = pMoveC->GetPoints();
                    if(m_aTestStates[m_CurrentTestIndex].m_BestEnemyPoints <points)
                    {
                            m_aTestStates[m_CurrentTestIndex].m_BestEnemyPoints = points;
                    }
                }
        }
        for(int i = 0; i < possibleMoves->m_lpMoves.size(); ++i)
        {

             /*if(possibleMoves->m_lpMoves[i]->m_pStone != 0)
          {
              for(int b = (m_Player*6); b < 6+m_Player*6; b++)
              {
                  if(pState->m_apHandStones[b] != 0 && pState->m_apHandStones[b]->m_Color == possibleMoves->m_lpMoves[i]->m_pStone->m_Color&& pState->m_apHandStones[b]->m_Shape == possibleMoves->m_lpMoves[i]->m_pStone->m_Shape)
                  {

                     possibleMoves->m_lpMoves[i]->m_pStone->m_Identifier = pState->m_apHandStones[b]->m_Identifier;*/

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
                    /* break;
                }
              }
          }*/
        }


    delete possibleMoves;
   // delete possibleMovesB;

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

