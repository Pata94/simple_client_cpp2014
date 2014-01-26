#ifndef CFIELDHANDLER_H
#define CFIELDHANDLER_H

#include "CStoneHandler.h"

class CFieldHandler
{
    public:
        CFieldHandler();
        virtual ~CFieldHandler();



        enum asd{
            FIELD_WIDTH = 16,
            FIELD_HEIGHT = 16,

            FIELDINDEX_HIDDEN=-4,
            FIELDINDEX_OPEN,
            FIELDINDEX_HAND_RED,
            FIELDINDEX_HAND_BLUE,

            MODE_COLOR= 1,
            MODE_SHAPE = 2,



            ERROR_NONE = 0,
            ERROR_UNSPECIFIC,
        };
        int CanPlace(int index, CStoneHandler::CStone* pStone);
        int PlaceStone(int index, CStoneHandler::CStone* pStone);
        bool IsFree(int index);
         CStoneHandler::CField m_aField[FIELD_WIDTH*FIELD_HEIGHT];
    protected:

        int m_Moves;
    private:
};

#endif // CFIELDHANDLER_H
