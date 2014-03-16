/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#include "CGameState.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include "stringbuffer.h" //TODO delete this




const char*        CGameState::m_aColorNames[6] ={"BLUE", "GREEN", "MAGENTA", "ORANGE", "VIOLET", "YELLOW"};
const char*  CGameState::m_aShapeNames[6] ={"ACORN", "BELL", "CLUBS", "DIAMOND", "HEART", "SPADES"};
CGameState::CGameState()
{
    m_pFieldHandler = new CFieldHandler();
    m_Moves = 0;
}
int CGameState::ShapeToIndex(char *pName)
{
    for(int i = 0; i < 6; ++i)
        if(strcmp(m_aShapeNames[i], pName) == 0)
                        return i;
    return -1;
}
int CGameState::ColorToIndex(char *pName)
{
    for(int i = 0; i < 6; ++i)
        if(strcmp(m_aColorNames[i], pName) == 0)
            return i;
    return -1;
}
CGameState::~CGameState()
{
    delete m_pFieldHandler;
    for(int i = 0; i < 12; ++i)
    {
        if( m_apHandStones[i])
        {
            delete m_apHandStones[i];
            m_apHandStones[i] = 0;
        }
         if( m_apOpenStones[i])
        {
            delete m_apOpenStones[i];
            m_apOpenStones[i] = 0;
        }
    }

}

/*const char *CGameState::GetColorName(int color) const
{
    if(color >= 0 && color < 6)
        return m_aColorNames[color];
    return 0;
}


const char *CGameState::GetShapeName(int shape) const
{
    if(shape >= 0 && shape < 6)
        return m_aShapeNames[shape];
    return 0;
}*/

/*
char *CGameState::DataToString()
{
    CStringBuffer buf(125, 512);
    buf.append("Field: \n");
     char aBuf[16];
    for(int i = 0; i < 31; ++i)
    {

        sprintf(aBuf, "%d", i);
        buf.append("Field[");
        buf.append(aBuf);
        buf.append("] Symbol[");
        sprintf(aBuf, "%d", m_aField[i].m_Symbol);
        buf.append(aBuf);
        buf.append("] Red[");
        sprintf(aBuf, "%d", m_aField[i].m_NumPirates[0]);
        buf.append(aBuf);
        buf.append("] Blue[");
        sprintf(aBuf, "%d", m_aField[i].m_NumPirates[1]);
        buf.append(aBuf);
        buf.append("] \n");
    }
    buf.append("red cards");
    for(int i=0; i < 6; ++i)
    {
        buf.append("symbol[");
        sprintf(aBuf, "%d", i);
        buf.append(aBuf);
        buf.append("]:");
        sprintf(aBuf, "%d", m_aCards[i]);
        buf.append(aBuf);
        buf.append("\n");
    }
    buf.append("blue cards");
    for(int i=0; i < 6; ++i)
    {
        buf.append("symbol[");
        sprintf(aBuf, "%d", i);
        buf.append(aBuf);
        buf.append("]:");
        sprintf(aBuf, "%d", m_aCards[i+6]);
        buf.append(aBuf);
        buf.append("\n");
    }
    printf(buf.toString());
    return 0;
}
*/
int CGameState::DoPlaceMove(CMove *move)
{
    if(!move)
        return 0;
    if(move->m_FieldIndex < 0 || move->m_FieldIndex >= 256)
            return ERROR_UNSPECIFIC;
        int points= m_pFieldHandler->CanPlace( move->m_FieldIndex, move->m_pStone);
        if(m_Moves < 5 && points != 0)
        {
            CStoneHandler::CStone *pStone = (CStoneHandler::CStone*)operator new(sizeof(CStoneHandler::CStone));
            *pStone = *move->m_pStone;
            m_pFieldHandler->PlaceStone(move->m_FieldIndex, pStone); //dont use the stone from the move it should be cleared by the move de-constructor
            //delete this stone from the hand
            int player = move->m_Player;
            int index;
            for(index = player*6; index < player*6+6; ++index)
            {
                if(m_apHandStones[index] != NULL && move->m_pStone->m_Identifier == m_apHandStones[index]->m_Identifier)
                {
                    delete m_apHandStones[index];
                    m_apHandStones[index] = 0;
                    break;
                }
            }

            if(m_apHandStones[index] == NULL)
            {
                m_apHandStones[index] = m_apOpenStones[0];
                for(int i = 0; i < 11; ++i)
                {
                    m_apOpenStones[i] = m_apOpenStones[i+1];
                }
                m_apOpenStones[11] = 0;
                     return points;
            }
            else
                return 0;


        }
        else
            return 0;
}

int CGameState::DoExchangeMove(int Player, int CardIndex)
{
    if(Player < 0 || Player > 1)
        return ERROR_UNSPECIFIC;
    if(CardIndex < 0 || CardIndex > 5)
        return ERROR_UNSPECIFIC;
   if(!m_apHandStones[Player*6+CardIndex])
        return ERROR_UNSPECIFIC;
    delete m_apHandStones[Player*6+CardIndex];

    m_apHandStones[Player*6+CardIndex] = m_apOpenStones[0];
    for(int i = 0; i < 11; ++i)
    {
        m_apOpenStones[i] = m_apOpenStones[i+1];
    }
    m_apOpenStones[11] = 0;
    return ERROR_NONE;

}
int CGameState::DoMove(CMoveContainer *moveC)
{
    if(!moveC)
        return ERROR_UNSPECIFIC;
    //if(moveC->m_Player != m_CurrentPlayer)
   //    return ERROR_UNSPECIFIC;

    if(moveC->m_MoveType==MOVE_PLACE)
    {
        int points = 0;
        for(int i = 0; i< moveC->m_lpMoves.size(); ++i) // every move is a move for itself
        {
            points  += DoPlaceMove(moveC->m_lpMoves[i]);
        }

    }
    else if(moveC->m_MoveType==MOVE_EXCHANGE) // all moves together
    {
        //TODO
         for(int i = 0; i< moveC->m_lpMoves.size(); ++i)
        {
            DoExchangeMove(moveC->m_lpMoves[i]->m_Player, moveC->m_lpMoves[i]->m_CardIndex);
        }

    }
    return 0;
}
CGameState::CMoveContainer* CGameState::GetPossibleMoves(int player)
{
    bool IsFirstMove = m_pFieldHandler->UpdateFirstMove();
    //std::vector<CGameState::CMoveContainer*> *possibleMoves = new std::vector<CGameState::CMoveContainer*>();
    //possibleMoves->reserve(50);
    CGameState::CMove* pMove = 0;
    CGameState::CMoveContainer* pMoveContainer = new CGameState::CMoveContainer();

    int p = player == 0 ? 0 : 6;

    if(IsFirstMove)
    {
        pMoveContainer->m_MoveType = MOVE_PLACE_FIRST;

        //look for possible stones
        for(int a = 0; a < 6; ++a)
        {
            if(m_apHandStones[a+p] == 0)
                continue;
            for(int b = 0; b < 6; ++b)
            {
                if(b == a || m_apHandStones[p+b] == 0)
                    continue;
                if((CStoneHandler::CheckShape(m_apHandStones[a+p], m_apHandStones[p+b])
                   &&  !CStoneHandler::CheckColor(m_apHandStones[a+p], m_apHandStones[p+b]))

                   || (!CStoneHandler::CheckShape(m_apHandStones[a+p], m_apHandStones[p+b])
                   &&  CStoneHandler::CheckColor(m_apHandStones[a+p], m_apHandStones[p+b])))
                {
                    pMove = new CGameState::CMove();
                    pMove->m_pStone = (CStoneHandler::CStone*) operator new (sizeof(CStoneHandler::CStone)); //do not use the original pointer
                    *pMove->m_pStone = *m_apHandStones[a+p]; //do not use the original pointer
                    pMove->m_Mode = MOVE_PLACE;
                    pMove->m_FieldIndex = 0;
                    pMove->m_Player = player;
                    pMoveContainer->m_lpMoves.push_back(pMove);

                    pMove = new CGameState::CMove();
                    pMove->m_pStone = (CStoneHandler::CStone*) operator new (sizeof(CStoneHandler::CStone)); //do not use the original pointer
                    *pMove->m_pStone = *m_apHandStones[b+p]; //do not use the original pointer
                    pMove->m_Mode = MOVE_PLACE;
                    pMove->m_FieldIndex = 1;
                    pMove->m_Player = player;
                    pMoveContainer->m_lpMoves.push_back(pMove);

                    return pMoveContainer;
                }
                            }
        }
    }
    else
    {
        pMoveContainer->m_MoveType = MOVE_PLACE;
        for(int a = 0; a < 6; ++a)
        {
            if(m_apHandStones[a+p] != 0)
                for(int i = 0; i < 256; ++i)
                {
                    if(m_pFieldHandler->CanPlace( i, m_apHandStones[a+p]) > 0)
                    {
                        pMove = new CGameState::CMove();
                         pMove->m_pStone = (CStoneHandler::CStone*) operator new (sizeof(CStoneHandler::CStone)); //do not use the original pointer
                        *pMove->m_pStone = *m_apHandStones[a+p]; //do not use the original pointer
                        pMove->m_Mode = MOVE_PLACE;
                        pMove->m_FieldIndex = i;
                        pMove->m_Player = player;
                        printf("asdadasdasdasd: %d, %d", player, a+p);
                        pMoveContainer->m_lpMoves.push_back(pMove);

                    }
                }
        }
    }
    //TODO: do exchange stuff

    return pMoveContainer;
}

int CGameState::getPoints(CGameState::CMove* ppMove)
{
    if(ppMove == 0)
    {
        printf("Nullpointer in function getPoints");
        return -1;
    }
    else if(ppMove->m_Mode == MOVE_EXCHANGE)
    {
        return 2;
    }

    if( m_pFieldHandler->GetPoints(ppMove->m_FieldIndex, ppMove->m_pStone) != m_pFieldHandler->CanPlace(ppMove->m_FieldIndex, ppMove->m_pStone))
    {
            printf("get Points Difference, A: %d, B %d", m_pFieldHandler->GetPoints(ppMove->m_FieldIndex, ppMove->m_pStone), m_pFieldHandler->CanPlace(ppMove->m_FieldIndex, ppMove->m_pStone));
    }

    return m_pFieldHandler->CanPlace(ppMove->m_FieldIndex, ppMove->m_pStone);
}



void CGameState::CopyGameState(CGameState *gameState)
{
    if(!gameState)
    {
      printf("Nullzeiger übergeben in CopyGameState versuche speicher zu bekommen");

        gameState = (CGameState*)operator new(sizeof(CGameState));
        if(!gameState)
        {
              printf("fehler 1");
                return;
        }

    }

    *gameState = *(CGameState *)this;


    //gameState->m_pFieldHandler = new CFieldHandler();
    gameState->m_pFieldHandler = (CFieldHandler*)operator new(sizeof(CFieldHandler));
    *gameState->m_pFieldHandler = *m_pFieldHandler;
    for(int i = 0; i < 12; ++i)
    {
        if(m_apHandStones[i] != 0)
        {
            gameState->m_apHandStones[i] = (CStoneHandler::CStone*) operator new (sizeof(CStoneHandler::CStone));
            *gameState->m_apHandStones[i] = *m_apHandStones[i];
        }
        else
            gameState->m_apHandStones[i] = 0;

        if(m_apOpenStones[i] != 0)
        {
              gameState->m_apOpenStones[i] = (CStoneHandler::CStone*) operator new (sizeof(CStoneHandler::CStone));
            *gameState->m_apOpenStones[i] = *m_apOpenStones[i];
        }
        else
            gameState->m_apOpenStones[i] = 0;

    }
}


