#ifndef CFIELDHANDLER_H
#define CFIELDHANDLER_H

#include "CStoneHandler.h"
#include "CGameState.h"

#include "config.h"

class CFieldHandler
{
    public:
        CFieldHandler();
        virtual ~CFieldHandler();

        int getPoints(CGameState::CMove* ppMove);


        int CanPlace(int index, CStoneHandler::CStone* pStone);
        int PlaceStone(int index, CStoneHandler::CStone* pStone);
        bool UpdateFirstMove();
        bool IsFree(int index);
         CStoneHandler::CField m_aField[FIELD_WIDTH*FIELD_HEIGHT];
    protected:
        bool m_IsFirstMove;
        int m_Moves;
    private:
};

#endif // CFIELDHANDLER_H
