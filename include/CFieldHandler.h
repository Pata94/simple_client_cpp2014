#ifndef CFIELDHANDLER_H
#define CFIELDHANDLER_H

#include "CStoneHandler.h"

#include "config.h"

class CFieldHandler
{
    public:
        CFieldHandler();
        virtual ~CFieldHandler();

        bool CheckRestrictions(int FieldIndex);
        int m_aRestrictions[2];
        int CanPlace(int index, CStoneHandler::CStone* pStone);
        int PlaceStone(int index, CStoneHandler::CStone* pStone);
        bool UpdateFirstMove();
        bool IsFree(int index);
         CStoneHandler::CField m_aField[FIELD_WIDTH*FIELD_HEIGHT];
        int GetPoints(int index, CStoneHandler::CStone *pStone);
        void NewRound();
    protected:
        bool m_IsFirstMove;
        int m_Moves;
    private:
};

#endif // CFIELDHANDLER_H
