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
        void OnRequestAction(CMoveHandler::CMoveContainer **pMoves);
        void OnGameStateUpdate(CGameState *pNewState);
        CMoveHandler::CMoveContainer *m_pBestMoveC;//[10];
        CMoveHandler::CMoveContainer *m_pOldBestMoveC[10];
        int GetCardValue(CGameState *pState, CStoneHandler::CStone *pStone);
        float GetHandCardValues(CGameState *pState, int player);
          int m_BestPoints;//[10];
        int m_OldBestPoints[10];
        struct CPoints
        {
            int points;
            CMoveHandler::CMove *ppMove;

        };
        void TestFunc(CGameState *pState, CMoveHandler::CMoveContainer* pMoveC);
       int TestGameState(CGameState *pState, CMoveHandler::CMoveContainer* pMoveC);


       int GetStoneValues(CGameState *pState);
        float GetStoneProbability(CGameState *pState, CStoneHandler::CStone *pStone);
        float m_aValues[NUM_COLORS*NUM_SHAPES];
        float GetStoneValue(CStoneHandler::CStone *pStone);
        float RekursiveFunktion(CFieldHandler *pField, float currentProbability, float (&aProbabilities)[NUM_COLORS*NUM_SHAPES], float step);
    protected:
    private:
        int m_Player;
        class CGameState *m_pGameState;
};

inline bool operator>(const CBaseLogic::CPoints &a, const CBaseLogic::CPoints &b) {return a.points > b.points;};
#endif // CBASELOGIC_H
