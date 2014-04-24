#ifndef CFIELDHANDLER_H
#define CFIELDHANDLER_H

#include "CStoneHandler.h"

#include "config.h"
#include "CMoveHandler.h"
class CFieldHandler
{
    public:
        static int NUM_HANDLERS;
        CFieldHandler();
        virtual ~CFieldHandler();

        bool CheckRestrictions(int FieldIndex);
         CMoveHandler::CMoveContainer m_PossibleStones;
        int m_aRestrictions[2];
        int CanPlace(int index, CStoneHandler::CStone* pStone);
        int PlaceStone(int index, CStoneHandler::CStone* pStone, bool update = false);
        bool UpdateFirstMove();
        bool IsFree(int index);
         CStoneHandler::CField m_aField[FIELD_WIDTH*FIELD_HEIGHT];
        int GetPoints(int index, CStoneHandler::CStone *pStone);
        void NewRound();


        CMoveHandler::CMoveContainer *GetPossibleMoves(){
        return m_PossibleStones.Clone();};


        void InitPossibleMoves();
        void UpdatePossibleMoves();
        void UpdatePossibleMoves(int Index);
        CFieldHandler* Clone();
    protected:
        bool m_IsFirstMove;
        int m_Moves;
    private:
};

#endif // CFIELDHANDLER_H
