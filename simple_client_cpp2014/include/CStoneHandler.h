#ifndef CSTONEHANDLER_H
#define CSTONEHANDLER_H


class CStoneHandler
{
    public:
        typedef unsigned short flags;

        enum{
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
        };

        static bool IsStone(flags StoneFlags, flags Flags);
        static flags DisableColor(flags StoneFlags, int Color);
        static flags DisableShape(flags StoneFlags, int Shape);
        static flags Disable(flags StoneFlags, flags Flags);
        static flags EnableColor(flags StoneFlags, int Color);
        static flags EnableShape(flags StoneFlags, int Shape);
        static bool CanPlace(flags FieldFlags, flags StoneFlags);
        static flags CheckShape(flags a, flags b);
        static flags CheckColor(flags a, flags b);
        static int GetFieldIndex(flags StoneFlags);
    protected:
    private:
};

#endif // CSTONEHANDLER_H
