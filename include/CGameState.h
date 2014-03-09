/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#ifndef CGAMESTATE_H
#define CGAMESTATE_H

#include <vector>
#include <CFieldHandler.h>
#include "config.h"
class CGameState
{
    public:
    static const char* m_aColorNames[6];
        static const char* m_aShapeNames[6];
    enum{


    };
int m_Moves;
        CGameState();
        ~CGameState();

        struct CMove{
            CMove()
            {
                // 0 == place, 1 == exchange
                m_Mode = 0;
                m_pStone=0;
                m_FieldIndex=0;
                m_Player=0;
            }
            int m_Player;
            int m_FieldIndex;
            int m_Mode;
            CStoneHandler::CStone *m_pStone;
        };

        struct CMoveContainer{
            ~CMoveContainer()
            {
                for(int i = 0; i < m_lpMoves.size(); ++i)
                    delete m_lpMoves[i];
            };
            int m_MoveType; // 0 == Lay 1 == change
            std::vector<CMove *> m_lpMoves;
        };



        CFieldHandler *m_pFieldHandler;
        CStoneHandler::CStone *m_apHandStones[12];
        //int m_NumOpenStones;
        int m_NumBagStones;
        CStoneHandler::CStone *m_apOpenStones[12];

        int m_aPoints[2];
        int m_CurrentPlayer;
        int m_PlayerID;
        int m_Turn;
        int EndRound();
        char *DataToString();

        int DoMove(CMove *move);
        CMoveContainer *GetPossibleMoves(int player);


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

        void CopyGameState(CGameState *gameState);

    protected:
    private:
};
#endif // CGAMESTATE_H
