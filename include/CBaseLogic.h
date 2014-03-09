/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#ifndef CBASELOGIC_H
#define CBASELOGIC_H


#include "CGameState.h"

#include "config.h"
class CBaseLogic
{
    public:
        CBaseLogic(int Player);
        virtual ~CBaseLogic();
        void OnRequestAction(CGameState::CMoveContainer *pMoves);
        void OnGameStateUpdate(CGameState *pNewState);
        struct Points
        {
            int points;
            CGameState::CMove *ppMove;

        };
    protected:
    private:
        int m_Player;
        class CGameState *m_pGameState;
};

bool operator>(const CBaseLogic::Points &a, const CBaseLogic::Points &b) {return a.points > b.points;}
#endif // CBASELOGIC_H
