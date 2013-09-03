/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#ifndef CGAMESTATE_H
#define CGAMESTATE_H

#include <vector>
class CGameState
{
    public:
    enum{

        SHAPE_NONE = -1,
        SHAPE_ACORN, // Eichel
        SHAPE_BELL,
        SHAPE_CLUBS,// Kreuz
        SHAPE_DIAMOND, // Karo
        SHAPE_HEART, // Herz
        SHAPE_SPADES, // Pik


        COLOR_NONE = -1,
        COLOR_BLUE,
        COLOR_GREEN,
        COLOR_MAGENTA,
        COLOR_ORANGE,
        COLOR_VIOLET,
        COLOR_YELLOW,

        MODE_PLACE=0,
        MODE_EXCHANGE=1,

        ERROR_NONE = 0,
        ERROR_UNSPECIFIC,

        FIELD_WIDTH=15,
        FIELD_HEIGHT=15
    };


        CGameState();
        virtual ~CGameState();

        struct CMove{
            CMove()
            {
                // 0 == place, 1 == exchange
                m_Mode = 0;
                m_CardID=0;
                m_ExchangeStones=0;
                m_FieldIndex=0;
                m_Player=0;
            }
            int m_Player;
            int m_FieldIndex;
            int m_Mode;
            int m_CardID;
            int m_ExchangeStones;
        };
        struct CStone
        {
            int m_Color;
            int m_Shape;
            int m_Identifier;
        };

        struct CField
        {
            CField(int FieldIndex)
            {
                m_AllowedColors = 63;
                m_AllowedTypes = 63;
                m_pStone = 0;
                m_FieldIndex = FieldIndex;
            }
            bool CanPlace(CStone *pStone)
            {
                if(!pStone)
                    return false;
                if((m_AllowedColors&(1<<pStone->m_Color))!=0 && (m_AllowedShapes&(1<<pStone->m_Shape)) != 0)
                    return true;
                return false;
            }
            void Place(CStone *pStone)
            {
                if(m_pStone != 0 || !CanPlace(pStone) )
                    return;
                m_AllowedColors ^= (1<<pStone->m_Color);
                m_AllowedShapes ^= (1<<pStone->m_Shape);
                m_pStone = pStone;

                for(int i=0; i < 15; ++i)
                {
                    if(i < 0 || i > FIELD_HEIGHT*FIELD_WIDTH)
                        break;
                }
            }
            int m_AllowedColors;
            int m_AllowedShapes;
            CStone *m_pStone;
            int m_FieldIndex;
        };


        int m_aStones[12];
        int m_aNumStones[2];
        int m_aField[FIELD_WIDTH*FIELD_HEIGHT];
        int m_aPoints[2];
        int m_CurrentPlayer;
        int m_Turn;
        int EndRound();
        char *DataToString();
        int DoMove(CMove *move);
        std::vector<CMove*> GetPossibleMoves();

    protected:
    private:
};

#endif // CGAMESTATE_H
