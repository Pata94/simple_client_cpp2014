#ifndef CFIELDHANDLER_H
#define CFIELDHANDLER_H

#include "CStoneHandler.h"

class CFieldHandler
{
    public:
        CFieldHandler();
        virtual ~CFieldHandler();

        enum{

            FIELD_WIDTH=15,
            FIELD_HEIGHT=15

            FIELDINDEX_HIDDEN=-4,
            FIELDINDEX_OPEN,
            FIELDINDEX_HAND_RED,
            FIELDINDEX_HAND_BLUE
        };
    protected:
        CStoneHandler::flags m_aField[FIELD_WIDTH*FIELD_HEIGHT];
    private:
};

#endif // CFIELDHANDLER_H
