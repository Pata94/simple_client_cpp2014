/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#include "CGameState.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include "stringbuffer.h" //TODO delete this
CGameState::CGameState()
{

    for(int i = 0; i < 12; ++i)
    {
             //   m_aPirates[i] = -1;
                m_aCards[i] = 0;
                m_aOpenCards[i] = 0;
    }

   // for(int i = 0; i < 32; ++i)
   // m_aField[i]= 0;

    m_aPoints[0] = 0;
    m_aPoints[1] = 0;
    m_CurrentPlayer = 0;
    m_NumMoves = 0;
}
int CGameState::SymbolToIndex(char* symbol)
{
    if(strcmp(symbol, "BOTTLE")== 0)
        return SYMBOL_BOTTLE;
    else if(strcmp(symbol, "DAGGER")== 0)
          return SYMBOL_DAGGER;
    else if(strcmp(symbol, "HAT")== 0)
        return SYMBOL_HAT;
    else if(strcmp(symbol, "KEY")== 0)
        return SYMBOL_KEY;
    else if(strcmp(symbol, "PISTOL")== 0)
        return SYMBOL_PISTOL;
    else if(strcmp(symbol, "SKULL")== 0)
        return SYMBOL_SKULL;
    else if(strcmp(symbol, "START")== 0)
        return SYMBOL_START;
    else if(strcmp(symbol, "FINISH")== 0)
        return SYMBOL_FINISH;
    return -1;
}
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
int CGameState::DoMove(CMove *move)
{
    if(!move)
    {
        ++m_NumMoves;
        return ERROR_NONE;
    }

    if(move->m_Player != m_CurrentPlayer)
        return ERROR_FALSE_PLAYER;
    if(move->m_PirateIndex < 0 || move->m_PirateIndex > 31)
        return ERROR_NO_PIRATE_ON_INDEX;
    if(m_NumMoves > 2)
        return ERROR_UNSPECIFIC;

    if(move->m_MoveIndex >= 0 && move->m_MoveIndex <= SYMBOL_SKULL)
    {
        if(m_aCards[move->m_MoveIndex+m_CurrentPlayer*6] < 1)
            return ERROR_NO_SUCH_CARD;
        if(m_aField[move->m_PirateIndex].m_NumPirates[m_CurrentPlayer] == 0)
            return ERROR_NO_PIRATE_ON_INDEX;

        if(move->m_PirateIndex == 31)
            return ERROR_UNSPECIFIC;
        int i = TryForwardMove(move->m_PirateIndex, move->m_MoveIndex);

        --m_aField[move->m_PirateIndex].m_NumPirates[m_CurrentPlayer];
        --m_aCards[move->m_MoveIndex+m_CurrentPlayer*6];
        --m_aNumCards[m_CurrentPlayer];
        ++m_aField[i].m_NumPirates[m_CurrentPlayer];
        ++m_NumMoves;
    }
    else if(move->m_MoveIndex == -1)
    {

         if(m_aField[move->m_PirateIndex].m_NumPirates[m_CurrentPlayer] == 0)
            return ERROR_NO_PIRATE_ON_INDEX;
        int i = TryBackmove(move->m_PirateIndex);
        if(i == 0)
            return ERROR_UNSPECIFIC;
        --m_aField[move->m_PirateIndex].m_NumPirates[m_CurrentPlayer];
        ++m_aField[i].m_NumPirates[m_CurrentPlayer];
        if(m_aNumCards[m_CurrentPlayer] < 8)
        {
             ++m_aCards[m_CurrentPlayer*6+GetNextOpenCard()];
             ++m_aNumCards[m_CurrentPlayer];
        }

        if(m_aNumCards[m_CurrentPlayer] < 8 && (m_aField[i].m_NumPirates[0]+ m_aField[i].m_NumPirates[1]) == 3)
         {
             ++m_aCards[m_CurrentPlayer*6+GetNextOpenCard()];
             ++m_aNumCards[m_CurrentPlayer];
         }
            ++m_NumMoves;
    }
    else if(move->m_MoveIndex == -2)
    {
            ++m_NumMoves;
    }
    else
        return ERROR_UNSPECIFIC;

    if(m_NumMoves > 2)
            return EndRound();
    return ERROR_NONE;
}
int CGameState::GetNextOpenCard()
{
    int index = m_aOpenCards[0];
    for(int i = 0; i < 11; ++i)
       m_aOpenCards[i] = m_aOpenCards[i+1];
    return index;
}
int CGameState::EndRound()
{
    m_CurrentPlayer ^= 1;
    m_NumMoves = 0;
    m_aPoints[0] = 0;
    m_aPoints[1] = 0;
    for(int i = 0; i < 32; ++i)
    {
        int points = ceil((float)i/6.0f);
        m_aPoints[0]+= points*m_aField[i].m_NumPirates[0];
        m_aPoints[1]+= points*m_aField[i].m_NumPirates[1];
    }
    return ERROR_NONE;
}
int CGameState::TryBackmove(int index)
{
     for(int i=index-1; i > 0; --i)
        {
            if((m_aField[i].m_NumPirates[0]+ m_aField[i].m_NumPirates[1]) > 0 && (m_aField[i].m_NumPirates[0]+ m_aField[i].m_NumPirates[1]) < 3)
                return i;
        }
    return 0;
}
int CGameState::TryForwardMove(int index, int symbol)
{
    for(int i =index; i < 32; ++i)
        {
            if(m_aField[i].m_Symbol == symbol)
                if(m_aField[i].m_NumPirates[0] == 0 && m_aField[i].m_NumPirates[1]== 0)
                    return i;
        }
    return 31;
}
std::vector<CGameState::CMove*> CGameState::GetPossibleMoves()
{
    std::vector<CGameState::CMove*> aMoveContainer;
    aMoveContainer.reserve(43);
    CMove *temp = 0;
    int NumPirates=0;
    for(int i=0; i < 32; ++i)
    {
        if(m_aField[i].m_NumPirates[m_CurrentPlayer] > 0)
        {
            if(i < 31)
            {
                for(int a = 0; a < 6; ++a)
                {
                    if(m_aCards[6*m_CurrentPlayer+a] > 0)
                    {
                        // printf("forward \n"); //Forward-Move

                        temp = new CMove();
                        temp->m_MoveIndex = a;
                        temp->m_PirateIndex=i;
                        temp->m_Player = m_CurrentPlayer;
                        aMoveContainer.push_back(temp);
                    }
                }
            }

            if(TryBackmove(i) > 0)
            {
                // printf("backward \n"); //Backward-Move
                temp = new CMove();
                temp->m_MoveIndex = -1;
                temp->m_PirateIndex=i;
                temp->m_Player = m_CurrentPlayer;
                aMoveContainer.push_back(temp);
            }
            NumPirates +=m_aField[i].m_NumPirates[m_CurrentPlayer];
        }
        else if(NumPirates>=6) //We got all Pirates
            break;
    }
   /*  printf("none \n"); //NO-MOVE can be done with a NULL-Move, too
    temp = new CMove();
    temp->m_MoveIndex = -2;
    temp->m_PirateIndex=0;
    temp->m_Player = m_CurrentPlayer;
     aMoveContainer.push_back(temp);*/
     return aMoveContainer;
}
CGameState::~CGameState()
{
    //dtor
}
