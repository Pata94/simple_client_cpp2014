/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#ifndef CBASELOGIC_H
#define CBASELOGIC_H


#include "CGameState.h"
#include "ext/sorted_array.h"
#include "config.h"
#include <time.h>
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
        float RekursiveFunktion(CFieldHandler *pField, float currentProbability, float (&aProbabilities)[NUM_COLORS*NUM_SHAPES], float step, int var);
         int m_Player;
         class CGameState *m_pGameState;
    protected:
    private:

        int m_CurrentTestIndex;
        struct CTest{

            CTest()
            {
                m_pGameState = 0;
                m_pMoveContainer = 0;
                m_Points = 0;
                m_BestEnemyPoints = 0;

            }
            void release()
            {
               if( m_pGameState != 0)
                delete m_pGameState;
                m_pMoveContainer = 0;
                if( m_pMoveContainer != 0)
                delete m_pGameState;
                m_Points = 0;
                            }
            CGameState *m_pGameState;
            CMoveHandler::CMoveContainer *m_pMoveContainer;
            int m_Points;
            int m_BestEnemyPoints;
               bool operator<(CTest &a)
            {
                if(this->m_BestEnemyPoints == -1)
                    return false;
                return (this->m_Points > a.m_Points);
            }


        };
         clock_t m_TimeStart;
        sorted_array<CTest> m_aTestStates;
        int GetTime()
        {
            clock_t t = clock()-m_TimeStart;
          return (int)((((float)t)/(float)CLOCKS_PER_SEC)*1000.0f);

        }
};

inline bool operator>(const CBaseLogic::CPoints &a, const CBaseLogic::CPoints &b) {return a.points > b.points;};
#endif // CBASELOGIC_H
