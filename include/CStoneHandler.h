#ifndef CSTONEHANDLER_H
#define CSTONEHANDLER_H


class CStoneHandler
{
    public:
        //typedef unsigned short flags;

        /*enum{
            FLAG_EMPTY = 1, //used by field
            FLAG_BAG = 2,
            FLAG_PLACED = 4,
            FLAG_OWNER = 8,
            FLAG_SHAPE_ACORN = 16, // Eichel
            FLAG_SHAPE_BELL = 32,
            FLAG_SHAPE_CLUBS = 64,// Kreuz
            FLAG_SHAPE_DIAMOND = 128, // Karo
            FLAG_SHAPE_HEART = 256, // Herz
            FLAG_SHAPE_SPADES = 512, // Pik

            FLAG_COLOR_BLUE = 1024,
            FLAG_COLOR_GREEN = 2048,
            FLAG_COLOR_MAGENTA = 4096,
            FLAG_COLOR_ORANGE = 8192,
            FLAG_COLOR_VIOLET = 16384,
            FLAG_COLOR_YELLOW = 32768
        }; */

        struct CStone{
            unsigned char m_Identifier;
            unsigned char m_Color;
            unsigned char m_Shape;
            unsigned char m_Position;
            unsigned int m_Index;
            CStone *Clone()
            {
                CStone *pStone = new CStone;
                *pStone = *this;
                return pStone;
            }
           bool operator<(CStone &a)
            {
                if(this->m_Color < a.m_Color)
                    return true;
                if(this->m_Shape < a.m_Shape)
                    return true;
                return this->m_Identifier < a.m_Identifier;
            }
               bool operator==(CStone &a)
            {
                if(this->m_Color == a.m_Color && this->m_Shape == a.m_Shape)
                    return true;
                return false;
            }
        };

        struct CField{
            unsigned char m_Index;
            unsigned short m_Flags;
            CStone *m_pStone;

            CField* Clone()
            {
                CField *pField = new CField;
                *pField = *this;
                if(m_pStone != 0)
                    pField->m_pStone = m_pStone->Clone();
                return pField;
            }
        };

        /*static bool IsStone(flags StoneFlags, flags Flags);
        static flags DisableColor(flags StoneFlags, int Color);
        static flags DisableShape(flags StoneFlags, int Shape);
        static flags Disable(flags StoneFlags, flags Flags);
        static flags EnableColor(flags StoneFlags, int Color);
        static flags EnableColor(flags StoneFlags, flags Color);
        static flags EnableShape(flags StoneFlags, int Shape);
        static flags EnableShape(flags StoneFlags, flags Shape);
        static bool CanPlace(flags FieldFlags, flags StoneFlags);
        static flags CheckShape(flags a, flags b);
        static flags CheckColor(flags a, flags b);
        static int GetFieldIndex(flags StoneFlags);
        static flags GetFullStone();
        static flags SetEmpty(flags Flags, bool Empty);
        static bool IsEmpty(flags FieldFlags);*/
        static bool CheckColor(CField *pField, CStone *pStone);
        static bool CheckShape(CField *pField, CStone *pStone);

         static bool CheckColor(CStone *pStoneA, CStone *pStoneB);
        static bool CheckShape(CStone *pStoneA, CStone *pStoneB);
    protected:
    private:
};

#endif // CSTONEHANDLER_H
