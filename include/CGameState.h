/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#ifndef CGAMESTATE_H
#define CGAMESTATE_H

#include <vector>
class CGameState
{
    public:
    enum{

        SYMBOL_BOTTLE = 0,
        SYMBOL_DAGGER,
        SYMBOL_HAT,
        SYMBOL_KEY,
        SYMBOL_PISTOL,
        SYMBOL_SKULL,
        SYMBOL_START,
        SYMBOL_FINISH,

        ERROR_NONE = 0,
        ERROR_UNSPECIFIC,
        ERROR_FALSE_PLAYER,
        ERROR_NO_SUCH_CARD,
        ERROR_NO_PIRATE_ON_INDEX
    };

        CGameState();
        virtual ~CGameState();

        struct CMove{
            CMove()
            {
                m_PirateIndex=0;
                m_MoveIndex=0;
                m_Player=0;
            }
            int m_PirateIndex;
            int m_MoveIndex; //-1 = backwardMove, -2 = no move, 0-5 Karten
            int m_Player;
        };

        struct CField{
            CField()
            {
                m_Symbol=0;
                m_NumPirates[0]=0;
                m_NumPirates[1]=0;
            }
            int m_Symbol;
            int m_NumPirates[2];
        } m_aField[32];
        int m_aPoints[2];
        int m_aOpenCards[12];
        int m_aCards[12];
        int m_aNumCards[2];
        int m_CurrentPlayer;
        int m_Turn;
        int m_NumMoves;
        int GetNextOpenCard();
        int EndRound();
        char *DataToString();
        int DoMove(CMove *move);
        std::vector<CMove*> GetPossibleMoves();
        int TryBackmove(int index);
        int TryForwardMove(int index, int symbol);
        static int SymbolToIndex(char* symbol);

    protected:
    private:
};

#endif // CGAMESTATE_H
