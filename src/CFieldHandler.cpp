#include "CFieldHandler.h"

CFieldHandler::CFieldHandler()
{
    //ctor
    for(int i = 0; i < FIELD_WIDTH*FIELD_HEIGHT; ++i)   //initiate the field
    {
        CStoneHandler::flags stone;
        stone = CStoneHandler::SetEmpty(CStoneHandler::GetFullStone(), true);
        m_aField[i] = stone;
    }
}

CFieldHandler::~CFieldHandler()
{
    //dtor
}

bool CFieldHandler::CanPlace(int index, CStoneHandler::flags Stone)
{
    return CStoneHandler::CanPlace(m_aField[index], Stone);
}

int CFieldHandler::PlaceStone(int index, CStoneHandler::flags Stone)
{
    if(!CanPlace(index, Stone))
        return ERROR_UNSPECIFIC;
    int x = index%FIELD_WIDTH;
    int y = (index-x)/FIELD_WIDTH;

    m_aField[index] = Stone;

    int mode = 0;

    if(x > 0)
    {
        if(!CStoneHandler::IsEmpty(m_aField[index-1]))
           {
                if(CStoneHandler::CheckColor(m_aField[index-1], m_aField[index]))
                    mode = MODE_COLOR;
                else if(CStoneHandler::CheckShape(m_aField[index-1], m_aField[index]))
                    mode = MODE_SHAPE;
                else
                    return ERROR_UNSPECIFIC;
           }
    }
    int newmode = mode;
    if(x < FIELD_WIDTH-1)
    {
        if(!CStoneHandler::IsEmpty(m_aField[index+1]))
           {
                if(CStoneHandler::CheckColor(m_aField[index+1], m_aField[index]))
                    mode = MODE_COLOR;
                else if(CStoneHandler::CheckShape(m_aField[index+1], m_aField[index]))
                    mode = MODE_SHAPE;
                else
                    return ERROR_UNSPECIFIC;
           }
    }
    if(newmode != 0 && newmode != mode)
        return ERROR_UNSPECIFIC;

    Stone = CStoneHandler::GetFullStone();
    //Stone = CStoneHandler::SetFieldIndex(Stone);
    int lastIndex = -1;

    for(int i = x-1; i >= 0; --i)
    {

        if(!CStoneHandler::IsEmpty(m_aField[i+y*FIELD_WIDTH]))
        {
            if(mode == MODE_COLOR)
            {

            }

        }
        else
        {
            lastIndex = i;
            break;
        }

    }
}
