#include "CStoneHandler.h"

bool CStoneHandler::IsStone(flags StoneFlags, flags Flags)
{
    return (StoneFlags==Flags); //TODO: Check Empty flag too?
}

flags CStoneHandler::Disable(flags StoneFlags, flags Flags)
{
    flags a = (FLAG_COLOR_BLUE<<Color); //the color flags start at FLAG_COLOR_BLUE
   if((StoneFlags & a))
        return StoneFlags & ~a;
    return StoneFlags;
}

flags CStoneHandler::DisableColor(flags StoneFlags, int Color)
{
    flags a = (FLAG_COLOR_BLUE<<Color); //the color flags start at FLAG_COLOR_BLUE
   if((StoneFlags & a))
        return StoneFlags & ~a;
    return StoneFlags;
}


flags CStoneHandler::DisableShape(flags StoneFlags, int Shape)
{
     flags a = (FLAG_SHAPE_ACORN<<Shape);  //the shape flags start at FLAG_SHAPE_ACORN
   if((StoneFlags & a))
        return StoneFlags & ~a;
    return StoneFlags;
}


flags CStoneHandler::EnableColor(flags StoneFlags, int Color)
{
    return (StoneFlags | (FLAG_COLOR_BLUE<<Color));
}

flags CStoneHandler::EnableColor(flags StoneFlags, flags Color)
{
    return (StoneFlags | (Color));
}


flags CStoneHandler::EnableShape(flags StoneFlags, int Shape)
{
    return (StoneFlags | (FLAG_SHAPE_ACORN<<Shape));
}

flags CStoneHandler::EnableShape(flags StoneFlags, flags Shape)
{
    return (StoneFlags | (Shape));
}


bool CStoneHandler::CanPlace(flags FieldFlags, flags StoneFlags)
{
    //TODO: Update function
    if((FieldFlags&FLAG_EMPTY) != (StoneFlags&FLAG_EMPTY))      //ignore empty field for checking
        FieldFlags ^= FLAG_EMPTY;
    return ((FieldFlags&StoneFlags) == StoneFlags);
}


bool CStoneHandler::IsEmpty(flags FieldFlags)
{
    return (FieldFlags&FLAG_EMPTY);
}


flags CStoneHandler::SetEmpty(flags Flags, bool Empty)
{
    if((Flags&FLAG_EMPTY) != Empty)
        Flags ^= FLAG_EMPTY;
    return Flags;
}


int CStoneHandler::GetFieldIndex(flags StoneFlags)
{
    if((StoneFlags&FLAG_PLACED))
        return 0;
    else if((StoneFlags&FLAG_BAG))
    {
        if((StoneFlags&FLAG_OWNER))
            return FIELDINDEX_OPEN;
        return FIELDINDEX_HIDDEN;
    }
    else
    {
         if((StoneFlags&FLAG_OWNER))
            return FIELDINDEX_HAND_RED;
        return FIELDINDEX_HAND_BLUE;
    }
}


flags CStoneHandler::SetFieldIndex(flags StoneFlags, int FieldIndex)
{
    StoneFlags = (StoneFlags | (FLAG_BAG | FLAG_OWNER | FLAG_PLACED))^(FLAG_BAG | FLAG_OWNER | FLAG_PLACED); //Enable all flags to disable them

    if(FieldIndex==FIELDINDEX_HIDDEN)
        return StoneFlags&FLAG_BAG;
    else if(FieldIndex == FIELDINDEX_OPEN)
        return StoneFlags&(FLAG_BAG | FLAG_OWNER);
    else if(FieldIndex == FIELDINDEX_HAND_BLUE)
        return StoneFlags;
    else if(FieldIndex == FIELDINDEX_HAND_BLUE)
        return StoneFlags&FLAG_OWNER;
    else if(FieldIndex >= 0)
        return StoneFlags&FLAG_PLACED;
    return StoneFlags;
}


flags CStoneHandler::GetFullStone()
{
    flags a = 0;
    return ~a;
}

flags CStoneHandler::GetEmptyStone()
{
    flags a = 0;
    return a;
}


flags CStoneHandler::CheckShape(flags a, flags b)
{
        return a & b & (FLAG_SHAPE_ACORN | FLAG_SHAPE_BELL | FLAG_SHAPE_CLUBS | FLAG_SHAPE_DIAMOND | FLAG_SHAPE_HEART | FLAG_SHAPE_SPADES);
}


flags CStoneHandler::CheckColor(flags a, flags b)
{
    return a & b & (FLAG_COLOR_BLUE | FLAG_COLOR_GREEN | FLAG_COLOR_MAGENTA | FLAG_COLOR_ORANGE | FLAG_COLOR_VIOLET | FLAG_COLOR_YELLOW);
}



