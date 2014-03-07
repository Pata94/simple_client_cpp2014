#ifndef CFIELDHANDLER_H
#define CFIELDHANDLER_H

#include "CStoneHandler.h"
#include "CGameState.h"

class CFieldHandler
{
    public:
        CFieldHandler();
        virtual ~CFieldHandler();


        enum{
            FIELD_WIDTH = 15,
            FIELD_HEIGHT = 15,

            FIELDINDEX_HIDDEN=-4,
            FIELDINDEX_OPEN,
            FIELDINDEX_HAND_RED,
            FIELDINDEX_HAND_BLUE,

            MODE_COLOR= 1,
            MODE_SHAPE = 2,

            MODE_PLACE=0,
            MODE_EXCHANGE=1,

            ERROR_NONE = 0,
            ERROR_UNSPECIFIC,
        };
        bool CanPlace(int index, CStoneHandler::flags Stone);
        int PlaceStone(int index, CStoneHandler::flags Stone);
    protected:
        CStoneHandler::flags m_aField[FIELD_WIDTH*FIELD_HEIGHT];
    private:
};

#endif // CFIELDHANDLER_H
