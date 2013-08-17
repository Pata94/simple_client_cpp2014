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


}
int CGameState::GetIndex(int shape, int color)
{
   return shape*6+color;
}
const char *CGameState::GetColorName(int color)
{
    static const char* m_aColorNames[6] ={"BLUE", "GREEN", "MAGENTA", "ORANGE", "VIOLET", "YELLOW"};
    if(color >= 0 && color < 7)
        return m_aColorNames[color];
    return 0;
}
const char *CGameState::GetShapeName(int shape)
{
    static const char* m_aColorNames[6] ={"ACORN", "BELL", "CLUBS", "DIAMOND", "HEART", "SPADES"};
    if(shape >= 0 && shape < 7)
        return m_aShapeNames[shape];
    return 0;
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
        return ERROR_UNSPECIFIC;
    if(move->m_Player != m_CurrentPlayer)
        return ERROR_UNSPECIFIC;
    
    if(move->m_Mode==MODE_PLACE)
    {
        if(move->m_FieldIndex < 0 || move->m_Field >= FIELD_WIDTH*FIELD_HEIGHT)
            return ERROR_UNSPECIFIC;
        if(m_aField[m_FieldIndex]!=-1)
            return ERROR_UNSPECIFIC;



    }
    else if(move->m_Mode==MODE_EXCHANGE)
    {


    }
}

bool CGameState::PossibleStoneAt(int fieldindex, int shape, int color)
{
    int possible = -1;
    if(fieldindex-FIELD_WIDTH >= 0)
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
