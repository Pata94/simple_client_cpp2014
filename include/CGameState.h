/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#ifndef CGAMESTATE_H
#define CGAMESTATE_H


#include <CFieldHandler.h>
#include "config.h"
#include <stdio.h>
class CGameState
{
    public:
        static int NUM_GAMESTATES;

    static const char* m_aColorNames[6];
        static const char* m_aShapeNames[6];

        int m_Moves;
        CGameState();
        ~CGameState();


        CFieldHandler *m_pFieldHandler;
        CStoneHandler::CStone *m_apHandStones[12];
        //int m_NumOpenStones;
        int m_NumBagStones;
        CStoneHandler::CStone *m_apOpenStones[12];

        int m_aPoints[2];
        int m_CurrentPoints;
        int m_CurrentPlayer;
       // int m_PlayerID;
        int m_Turn;
        bool m_RoundEnd;
        int EndRound();
        char *DataToString();
        int DoPlaceMove(CMoveHandler::CMove *move);
        int DoExchangeMove(int Player, int CardIndex);
        int DoMove(CMoveHandler::CMoveContainer *moveC);
        CMoveHandler::CMoveContainer *GetPossibleMoves(int player);


        /*static inline int GetColor(int Index)
        {   return  Index%6;       };
        static inline int GetShape(int Index)
        {   return    Idenfifier/6;     };
        static inline int GetIndex(int Color, int Shape)
        {   return (Shape*6+Color);     };*/

        bool PossibleStoneAt(int fieldindex, int shape, int color);
      /*  inline bool PossibleStoneAt(int fieldindex, int index)
        {
            PossibleStoneAt(int fieldindex, GetShape(index), GetColor(index));
        };*/

        int PlaceStoneAt(int FieldIndex, int StoneIndex);

        static int ShapeToIndex(char *pName);
        static int ColorToIndex(char *pName);
        int getPoints(CMoveHandler::CMove* pMove);

        int PopOpenCards(int Player, int num);

        CGameState* Clone();

    protected:
    private:
};
#endif // CGAMESTATE_H
