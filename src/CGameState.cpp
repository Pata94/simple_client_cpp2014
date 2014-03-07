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
    m_aColorNames[6] ={"BLUE", "GREEN", "MAGENTA", "ORANGE", "VIOLET", "YELLOW"};
    m_aShapeNames[6] ={"ACORN", "BELL", "CLUBS", "DIAMOND", "HEART", "SPADES"};
}


const char *CGameState::GetColorName(int color) const
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

    if(move->m_Mode==MODE_PLACE)
    {
        if(move->m_FieldIndex < 0 || move->m_Field >= FIELD_WIDTH*FIELD_HEIGHT)
            return ERROR_UNSPECIFIC;
        if(!PossibleStoneAt(move->m_FieldIndex, GetShape(move->m_Stone), GetColor(move->m_Stone)))
            return ERROR_UNSPECIFIC;
        int points=PlaceStoneAt(move->m_FieldIndex, move->m_Stone);

    }
    else if(move->m_Mode==MODE_EXCHANGE)
    {


    }
}


int PlaceStoneAt(int FieldIndex, int StoneIndex)
{
    int x = FieldIndex%FIELD_WIDTH;
    int y= (FieldIndex-x)/FIELD_WIDTH;
    int points =0;

    m_aField[FieldIndex].m_Stone = StoneIndex;
    if(m_aField[FieldIndex].m_Mode==0)
    {
        //check for neighbours
        //up
        if(y>0)
        {
            if(m_aField[FieldIndex-FIELD_WIDTH].m_Stone != -1)
            {
                UpdateMode(FieldIndex, FieldIndex-FIELD_WIDTH, MODE_SHAPE_V, MODE_COLOR_V);
            }
        }

        //down

        if(y + 1 < FIELD_HEIGHT)
        {
            if(m_aField[FieldIndex+FIELD_WIDTH].m_Stone != -1)
            {
               UpdateMode(FieldIndex, FieldIndex+FIELD_WIDTH, MODE_SHAPE_V, MODE_COLOR_V);
            }
        }

        //right
        if(x + 1 < FIELD_WIDTH)
        {
            if(m_aField[FieldIndex+1].m_Stone != -1)
            {
                UpdateMode(FieldIndex, FieldIndex+1, MODE_SHAPE_H, MODE_COLOR_H);
            }
        }

        //left
        if(x > 0)
        {
            if(m_aField[FieldIndex-1].m_Stone != -1)
            {
                UpdateMode(FieldIndex, FieldIndex-1, MODE_SHAPE_H, MODE_COLOR_H);
            }
        }
    }
    int AllowedColors;
    if((m_aField[FieldIndex].m_Mode&MODE_SHAPE_H)!=0)
    {
            //update next stones to the sides
            for(int i = 1; i < FIELD_WIDTH-x; ++i)
            {
                if(m_aField[FieldIndex+i].m_Stone != -1)
                {
                    m_aField[FieldIndex+i].m_Mode |= MODE_SHAPE_H;
                    m_aField[FieldIndex+i].m_AllowedColors = AllowedColors;
                    m_aField[FieldIndex+i].m_AllowedShapes=AllowedShapes;

                    points += i;
                    break;
                }
            }
    }


    //RETURN POINTS HERE
}


void CGameState::UpdateMode(int FieldIndex, int NextFieldIndex, int modeshape, int modecolor)
{

     if(GetShape(m_aField[FieldIndex].m_Stone)==GetShape(m_aField[NextFieldIndex].m_Stone))
        {
            m_aField[FieldIndex].m_Mode |= modeshape;
            m_aField[NextFieldIndex].m_Mode |= modeshape;
        }
        else if(GetColor(m_aField[FieldIndex].m_Stone)==GetColor(m_aField[NextFieldIndex].m_Stone))
        {
            m_aField[FieldIndex].m_Mode |= modecolor;
            m_aField[NextFieldIndex].m_Mode |= modecolor;
        }
        else
        {
            //ERROR
        }
}


bool CGameState::PossibleStoneAt(int fieldindex, int shape, int color)
{
    if(m_aField[fieldindex].m_pStone != 0)
        return false;
      return (((m_aField[fieldindex].m_AllowedColors&(1<<color))!=0 && (m_aField[fieldindex].m_AllowedShapes&(1<<shape) != 0)));
}


std::vector<CGameState::CMove*> CGameState::GetPossibleMoves()
{
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
