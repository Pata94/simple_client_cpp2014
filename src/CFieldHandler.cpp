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
    m_Moves = 0; // Needed? YEAH
    m_IsFirstMove = true;
    m_aRestrictions[0] = -1;
    m_aRestrictions[1] = -1;


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

bool CFieldHandler::UpdateFirstMove()
{

    m_IsFirstMove = true;
    for(int i = 0; i < FIELD_WIDTH*FIELD_HEIGHT; ++i)
    {
         if(!IsFree(i))
         {
             m_IsFirstMove = false;
            break;
        }
    }
    return m_IsFirstMove;
}
int CFieldHandler::CanPlace(int index, CStoneHandler::CStone *pStone)
{

    /*int x = index%FIELD_WIDTH;
    int y = (index-x)/FIELD_WIDTH;
    int temp = 0;
    int points = 0;
    if(!IsFree(index))
        return 0;   //field not empty

    int dirx = 0;
    int diry = 0;
    if(x > 0)
    {
        if(!IsFree(index-1))
            dirx = -1;
    }

    if(x < FIELD_WIDTH-1)
    {
        if(!IsFree(index+1))
        {
            if(dirx != 0) //dont allow chaining
                return 0;
            dirx = 1;
        }
    }

    if(y > 0)
    {
        if(!IsFree(index-FIELD_WIDTH))
            diry = -FIELD_WIDTH;
    }

    if(y < FIELD_HEIGHT-1)
    {
        if(!IsFree(index+FIELD_WIDTH))
        {
            if(diry != 0) //dont allow chaining
                return 0;
            diry = FIELD_WIDTH;
        }
    }


    for(int i = index; i < (y+1)*FIELD_WIDTH; i += dirx)
    {

    }*/
    if(CheckRestrictions(index) == false)
        return 0;
    int x = index%FIELD_WIDTH;
    int y = (index-x)/FIELD_WIDTH;
    int temp = 0;
    int points = 0;
    if(!IsFree(index))
        return 0;   //field not empty
    for(int i = x-1; i >= 0; --i)
    {
        if(IsFree(i+y*FIELD_WIDTH))
        {
            temp = i+1;
            break;
        }
    }
    unsigned short flags = 0;
    int num_colors = 0;
    int num_shapes = 0;
    int num = 0;
    bool gotNeighbours = false;
    for(int i = temp; i < FIELD_WIDTH; ++i)
    {

        if(num >= 6) //bereits 6 steine gelegt
            return 0;
        if(i == x)
            continue;

      //  if(i-temp >= 5)
        //   return 0;

        if(IsFree(i+y*FIELD_WIDTH))
            break;

        ++num;

        if(CStoneHandler::CheckShape(&m_aField[i+y*FIELD_WIDTH], pStone))
            ++num_shapes;
        if(CStoneHandler::CheckColor(&m_aField[i+y*FIELD_WIDTH], pStone))
            ++num_colors;

        if(num_colors == 0 && (flags & (1<<pStone->m_Shape)))
        {
            return 0;
        }
        else
            flags |= (1<<pStone->m_Shape);

         if(num_shapes == 0 && (flags & (64<<pStone->m_Color)))
        {
            return 0;
        }
        else
            flags |= (64<<pStone->m_Color);

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


    temp = 0;
    flags = 0;
    for(int i = y-1; i >= 0; --i)
    {
        if(IsFree(x+i*FIELD_WIDTH))
        {
            temp = i+1;
            break;
        }
    }

    num_colors = 0;
    num_shapes = 0;
    num = 0;

    for(int i = temp; i < FIELD_HEIGHT; ++i)
    {
         if(num >= 6) //bereits 6 steine gelegt
            return 0;

        if(i == y)
            continue;

       // if(i-temp >= 5) //bereits 6 steine gelegt
       //    return 0;

        if(IsFree(x+i*FIELD_WIDTH))
            break;

        ++num;

        if(CStoneHandler::CheckShape(&m_aField[x+i*FIELD_WIDTH], pStone))
            ++num_shapes;
        if(CStoneHandler::CheckColor(&m_aField[x+i*FIELD_WIDTH], pStone))
            ++num_colors;

        if(num_colors == 1 && (flags & (1<<pStone->m_Shape)))
        {
            return 0;
        }
        else
            flags |= (1<<pStone->m_Shape);

         if(num_shapes == 1 && (flags & (64<<pStone->m_Color)))
        {
            return 0;
        }
        else
            flags |= (64<<pStone->m_Color);
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


    if(!m_IsFirstMove && gotNeighbours)
        return points;
    else if(m_IsFirstMove) // we can lay wherever we want
        return points;
    else
        return 0;
   }



int CFieldHandler::GetPoints(int index, CStoneHandler::CStone *pStone)
{

        int points = 0;
        int blocks = 0;
        int x = index%FIELD_WIDTH;
        int y = index/FIELD_WIDTH;


        for(int i = x-1; i>=0; i--)
        {
            if(!IsFree(i+y*FIELD_WIDTH))
            {
                points++;
                blocks++;
            }
            else
            {
                break;
            }
        }

        for(int i = x+1; i<FIELD_WIDTH; i++)
        {
            if(!IsFree(i+y*FIELD_WIDTH))
            {
                points++;
                blocks++;
            }
            else
            {
                break;
            }
        }

        if(blocks > 0)
        {
            points ++; //für den eigenen Stein
            if(blocks = 5) //Sixpack
            {
                points +=6;
            }
        }
        blocks = 0;

        for(int i = y-1; i>=0; i--)
        {
            if(!IsFree(i*FIELD_WIDTH+x))
            {
                points++;
                blocks++;
            }
            else
            {
                break;
            }
        }

        for(int i = y+1; i<FIELD_HEIGHT; i++)
        {
            if(!IsFree(i*FIELD_WIDTH+x))
            {
                points++;
                blocks++;
            }
            else
            {
                break;
            }
        }

        if(blocks > 0)
        {
            points ++; //für den eigenen Stein
            if(blocks = 5) //Sixpack
            {
                points +=6;
            }
        }

        return points;
}

bool CFieldHandler::CheckRestrictions(int FieldIndex)
{
    if(m_Moves >= 5)
        return false;
    if(m_aRestrictions[0] != -1)
    {
        int x1 = m_aRestrictions[0]%FIELD_WIDTH;
        int y1 = (m_aRestrictions[0]-x1)/FIELD_WIDTH;
        int x2 = FieldIndex%FIELD_WIDTH;
        int y2 = (FieldIndex-x2)/FIELD_WIDTH;
        int diffX = x2-x1;
        int diffY = y2-y1;


          if(diffX != 0 && diffY != 0)
                return false;

        if(m_aRestrictions[1] != -1)
        {
            int x3 = m_aRestrictions[1]%FIELD_WIDTH;
            int y3 = (m_aRestrictions[1]-x3)/FIELD_WIDTH;
            if(diffX == 0 && (x3-x1) != 0)
            {
                return false;
            }
            else if(diffY == 0 && (y3-y1) != 0)
            {
                return false;
            }

        }

        {

            int dir = 1;

            if(diffY != 0)
                dir = diffY > 0 ? FIELD_WIDTH : -FIELD_WIDTH;
            else if(diffX != 0)
                dir = diffX > 0 ? 1 : -1;

            for(int i = m_aRestrictions[0]; i < FIELD_WIDTH*FIELD_WIDTH; i += dir)
            {
                    if(i == FieldIndex)
                        break;
                    if(IsFree(i))
                        return false;
            }
        }
    }
    return true;
}

int CFieldHandler::PlaceStone(int index, CStoneHandler::CStone *pStone)
{


    m_aField[index].m_pStone = pStone;
    if(m_Moves < 2)
    {
        m_aRestrictions[m_Moves] = index;
    }
    ++m_Moves;
}
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

