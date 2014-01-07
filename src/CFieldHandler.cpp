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

    Stone = CStoneHandler::GetEmptyStone(); // GetEmptyStone() needs to get implemented
    //Stone = CStoneHandler::SetFieldIndex(Stone);
    int lastIndexA = -1;
    int lastIndexB = -1;

    for(int i = x-1; i >= 0; --i)
    {

        if(!CStoneHandler::IsEmpty(m_aField[i+y*FIELD_WIDTH]))
        {
            if(mode == MODE_COLOR)
            {
                Stone = CStoneHandler::EnableShape(Stone, /*Shape*/);
            }
            else
            {
                Stone = CStoneHandler::EnableColor(Stone, /*Color*/);
            }

        }
        else
        {
            lastIndexA = i;
            break;
        }
    }

    for(int i = x; i <FIELD_WIDTH; ++i)
    {
        if(!CStoneHandler::IsEmpty(m_aField[i+y*FIELD_WIDTH]))
        {
            if(mode == MODE_COLOR)
            {
                Stone = CStoneHandler::EnableShape(Stone, /*Shape*/);
            }
            else
            {
                Stone = CStoneHandler::EnableColor(Stone, /*Color*/);
            }
        }
        else
        {
            lastIndexB = i;
            break;
        }
    }

    if(lastIndexA != -1)
    {
        m_aField[lastIndexA+y*FIELD_WIDTH] = CStoneHandler::Disable(m_aField[lastIndexA+y*FIELD_WIDTH], Stone);
    }

    if(lastIndexB != -1)
    {
        m_aField[lastIndexB+y*FIELD_WIDTH] = CStoneHandler::Disable(m_aField[lastIndexB+y*FIELD_WIDTH, Stone]);
    }

    lastIndexA = -1;
    lastIndexB = -1;
    Stone = CStoneHandler::GetEmptyStone();

    for(int i = y-1; y >= 0; y--)
    {
         if(!CStoneHandler::IsEmpty(m_aField[x+i*FIELD_WIDTH]))
        {
            if(mode == MODE_COLOR)
            {
                Stone = CStoneHandler::EnableShape(Stone, /*Shape*/);
            }
            else
            {
                Stone = CStoneHandler::EnableColor(Stone, /*Color*/);
            }
        }
        else
        {
            lastIndexA = i;
            break;
        }
    }

    for(int i = y; y < FIELD_HEIGHT; y++)
    {
         if(!CStoneHandler::IsEmpty(m_aField[x+i*FIELD_WIDTH]))
        {
            if(mode == MODE_COLOR)
            {
                Stone = CStoneHandler::EnableShape(Stone, CStoneHandler::GetShape(m_aField[x+i*FIELD_WIDTH]));
            }
            else
            {
                Stone = CStoneHandler::EnableColor(Stone, CStoneHandler::GetColor(m_aField[x+i*FIELD_WIDTH]));
            }
        }
        else
        {
            lastIndexB = i;
            break;
        }
    }

    if(lastIndexA != -1)
    {
        m_aField[x+lastIndexA*FIELD_WIDTH] = CStoneHandler::Disable(m_aField[x+lastIndexA*FIELD_WIDTH], Stone);
    }

    if(lastIndexB != -1)
    {
        m_aField[x+lastIndexB*FIELD_WIDTH] = CStoneHandler::Disable(m_aField[x+lastIndexB*FIELD_WIDTH, Stone]);
    }

    return 0;
}
