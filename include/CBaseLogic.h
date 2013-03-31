/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#ifndef CBASELOGIC_H
#define CBASELOGIC_H

#include "CGameState.h"
class CBaseLogic
{
    public:
        CBaseLogic(int Player);
        virtual ~CBaseLogic();
        void OnRequestAction(CGameState::CMove *aMoves[3]);
        void OnGameStateUpdate(CGameState *pNewState);
    protected:
    private:
        int m_Player;
       class CGameState *m_pGameState;
};

#endif // CBASELOGIC_H
