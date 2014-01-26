#include "CFieldHandler.h"
CFieldHandler::CFieldHandler()
{
    //ctor
    for(int i = 0; i < FIELD_WIDTH*FIELD_HEIGHT; ++i)   //initiate the field
    {
       // CStoneHandler::flags stone;
        m_aField[i].m_Index = i;
        m_aField[i].m_pStone = 0;
        m_aField[i].m_Flags = 2^16-1;
    }
    m_Moves = 1;
}

CFieldHandler::~CFieldHandler()
{
    //dtor
    for(int i = 0; i < FIELD_WIDTH*FIELD_HEIGHT; ++i)   //initiate the field
    {
        delete m_aField[i].m_pStone;
    }
}
bool CFieldHandler::IsFree(int index)
{
    return m_aField[index].m_pStone == 0;
}

int CFieldHandler::CanPlace(int index, CStoneHandler::CStone *pStone)
{

    if(!IsFree(index))
        return 0;   //field not empty

    int x = index%FIELD_WIDTH;
    int y = (index-x)/FIELD_WIDTH;

    index = 0; //TODO dont use index
    int points = 1;
    for(int i = x-1; i >= 0; --i)
    {
        if(IsFree(i+y*FIELD_WIDTH))
        {
            index = i+1;
            break;
        }
    }
    int num_colors = 0;
    int num_shapes = 0;
    int num = 0;
    bool gotNeighbours = false;
    for(int i = index; i < FIELD_WIDTH; ++i)
    {
        if(i == x)
            continue;

        if(i-index >= 6) //bereits 6 steine gelegt
           return 0;

        if(IsFree(i+y*FIELD_WIDTH))
            break;

        ++num;
        if(CStoneHandler::CheckShape(&m_aField[i+y*FIELD_WIDTH], pStone))
            ++num_shapes;
        if(CStoneHandler::CheckColor(&m_aField[i+y*FIELD_WIDTH], pStone))
            ++num_colors;
    }

    if(num > 0)
    {
         if(!((num_shapes == 0 && num_colors == num) || (num_shapes == num && num_colors == 0)))
            return 0;
        gotNeighbours = true;
        points += num;
        if(num == 5)
            points += 6;
    }


    index = 0;
    for(int i = y-1; i >= 0; --i)
    {
        if(IsFree(x+i*FIELD_WIDTH))
        {
            index = i+1;
            break;
        }
    }

    num_colors = 0;
    num_shapes = 0;
    num = 0;

    for(int i = index; i < FIELD_HEIGHT; ++i)
    {
        if(i == y)
            continue;

        if(i-index >= 6) //bereits 6 steine gelegt
           return 0;

        if(IsFree(x+i*FIELD_WIDTH))
            break;

        ++num;
        if(CStoneHandler::CheckShape(&m_aField[x+i*FIELD_WIDTH], pStone))
            ++num_shapes;
        if(CStoneHandler::CheckColor(&m_aField[x+i*FIELD_WIDTH], pStone))
            ++num_colors;
    }

     if(num > 0)
    {
         if(!((num_shapes == 0 && num_colors == num) || (num_shapes == num && num_colors == 0)))
            return 0;
        gotNeighbours = true;

         points += num;
        if(num == 5)
            points += 6;
    }


    if(m_Moves > 0 && gotNeighbours)
        return points;
    else if(m_Moves == 0) // we can lay wherever we want
        return points;
    else
        return 0;
   }

int CFieldHandler::PlaceStone(int index, CStoneHandler::CStone *pStone)
{

    m_aField[index].m_pStone = pStone;
    ++m_Moves;
  /*  if(!CanPlace(index, Stone))
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
                Stone = CStoneHandler::EnableShape(Stone, m_aField[i+y*FIELD_WIDTH]);
            }
            else
            {
                Stone = CStoneHandler::EnableColor(Stone, m_aField[i+y*FIELD_WIDTH]);
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
                Stone = CStoneHandler::EnableShape(Stone, m_aField[i+y*FIELD_WIDTH]);
            }
            else
            {
                Stone = CStoneHandler::EnableColor(Stone, m_aField[i+y*FIELD_WIDTH]);
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
        m_aField[lastIndexB+y*FIELD_WIDTH] = CStoneHandler::Disable(m_aField[lastIndexB+y*FIELD_WIDTH], Stone);
    }


    mode = 0;

    if(y > 0)
    {
        if(!CStoneHandler::IsEmpty(m_aField[index-FIELD_WIDTH]))
           {
                if(CStoneHandler::CheckColor(m_aField[index-FIELD_WIDTH], m_aField[index]))
                    mode = MODE_COLOR;
                else if(CStoneHandler::CheckShape(m_aField[index-FIELD_WIDTH], m_aField[index]))
                    mode = MODE_SHAPE;
                else
                    return ERROR_UNSPECIFIC;
           }
    }
    newmode = mode;
    if(y < FIELD_HEIGHT-1)
    {
        if(!CStoneHandler::IsEmpty(m_aField[index+FIELD_WIDTH]))
           {
                if(CStoneHandler::CheckColor(m_aField[index+FIELD_WIDTH], m_aField[index]))
                    mode = MODE_COLOR;
                else if(CStoneHandler::CheckShape(m_aField[index+FIELD_WIDTH], m_aField[index]))
                    mode = MODE_SHAPE;
                else
                    return ERROR_UNSPECIFIC;
           }
    }
    if(newmode != 0 && newmode != mode)
        return ERROR_UNSPECIFIC;

    Stone = CStoneHandler::GetEmptyStone();
    lastIndexA = -1;
    lastIndexB = -1;

    for(int i = y-1; y >= 0; y--)
    {
         if(!CStoneHandler::IsEmpty(m_aField[x+i*FIELD_WIDTH]))
        {
            if(mode == MODE_COLOR)
            {
                Stone = CStoneHandler::EnableShape(Stone, m_aField[x+i*FIELD_WIDTH]);
            }
            else
            {
                Stone = CStoneHandler::EnableColor(Stone, m_aField[x+i*FIELD_WIDTH]);
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
                Stone = CStoneHandler::EnableShape(Stone, m_aField[x+i*FIELD_WIDTH]);
            }
            else
            {
                Stone = CStoneHandler::EnableColor(Stone, m_aField[x+i*FIELD_WIDTH]);
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

    return 0;*/
}
