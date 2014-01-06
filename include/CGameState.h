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
        NUM_SHAPES,

        COLOR_NONE = -1,
        COLOR_BLUE,
        COLOR_GREEN,
        COLOR_MAGENTA,
        COLOR_ORANGE,
        COLOR_VIOLET,
        COLOR_YELLOW,
        NUM_COLORS,

        MODE_COLOR= 1,
        MODE_SHAPE = 2,

        MODE_PLACE=0,
        MODE_EXCHANGE=1,

        ERROR_NONE = 0,
        ERROR_UNSPECIFIC,
        
    };


        CGameState();
        virtual ~CGameState();

        struct CMove{
            CMove()
            {
                // 0 == place, 1 == exchange
                m_Mode = 0;
                m_CardID=0;
                m_Stone=0;
                m_FieldIndex=0;
                m_Player=0;
            }
            int m_Player;
            int m_FieldIndex;
            int m_Mode;
            int m_CardID;
            int m_Stone;
        };
        struct CStone
        {
            int m_Color;
            int m_Shape;
            int m_Identifier;
            int m_FieldIndex;
        };

        struct CField
        {
            int m_AllowedColors;
            int m_AllowedShapes;
            int m_Mode;//0== none, 1==color, 2==shape
            int m_Stone;
            CStone *m_pStone;
        };

        int m_aHandStones[12];
        int m_aNumHandStones[2];
        CField m_aField[FIELD_WIDTH*FIELD_HEIGHT];
        int m_aPoints[2];
        int m_CurrentPlayer;
        int m_Turn;
        int EndRound();
        char *DataToString();
        int DoMove(CMove *move);
        std::vector<CMove*> GetPossibleMoves();


        static inline int GetColor(int Index)
        {   return  Idenfifier%6;       };
        static inline int GetShape(int Index)
        {   return    Idenfifier/6;     };
        static inline int GetIndex(int Color, int Shape)
        {   return (Shape*6+Color);     };

        bool PossibleStoneAt(int fieldindex, int shape, int color)
      /*  inline bool PossibleStoneAt(int fieldindex, int index)
        {
            PossibleStoneAt(int fieldindex, GetShape(index), GetColor(index));
        };*/

        int PlaceStoneAt(int FieldIndex, int StoneIndex);

    protected:
    private:
};
#endif // CGAMESTATE_H
