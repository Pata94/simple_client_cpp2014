#ifndef CMOVEHANDLER_H
#define CMOVEHANDLER_H


#include "ext/array.h"
class CMoveHandler
{

    public:
     struct CMove{
            CMove()
            {
                // 0 == place, 1 == exchange
                m_Mode = 0;
                m_pStone=0;
                m_FieldIndex=0;
                m_Player=0;
            }
            ~CMove()
            {
                if(m_pStone)
                {
                    delete m_pStone;
                }


            }
            int m_Player;
            union{
                int m_FieldIndex;
                int m_CardIndex;
            };
            int m_Points;
            CMove* Clone()
            {
                CMove* pMove = new CMove;

                *pMove = *this;
                if(m_pStone != 0)
                    pMove->m_pStone = m_pStone->Clone();
              //  else
               //     pMove->m_pStone = 0;
                return pMove;
            }
            int m_Mode;
            CStoneHandler::CStone *m_pStone;

           bool operator<(CMove &a)
            {
                if(this->m_FieldIndex < a.m_FieldIndex)
                    return true;
                if(this->m_pStone == 0)
                    return true;
                if(a.m_pStone == 0)
                    return false;
                return (this->m_pStone < a.m_pStone);
            }
            bool operator==(CMove &a)
            {
                if(this->m_Mode == a.m_Mode &&
                    (this->m_CardIndex == a.m_CardIndex || this->m_FieldIndex == a.m_FieldIndex) &&
                   this->m_Player == a.m_Player &&
                   ((this->m_pStone == 0 && a.m_pStone == 0) || ((this->m_pStone != 0 && a.m_pStone != 0) && *this->m_pStone == *a.m_pStone)))
                    return true;
                return false;


            }
        };

        struct CMoveContainer{

            CMoveContainer()
            {

                m_MoveType = MOVE_PLACE;
                m_lpMoves.clear();

            }
            ~CMoveContainer()
            {
                m_lpMoves.delete_all();

            }
            CMoveContainer * Clone()
            {
                CMoveContainer * pMoveC = new CMoveContainer;

                *pMoveC = *this;
                 pMoveC->m_lpMoves = m_lpMoves; //TODO maybe needed?
                for(int i = 0; i < m_lpMoves.size(); ++i)
                {
                    if(m_lpMoves[i] != 0)
                    pMoveC->m_lpMoves[i] = m_lpMoves[i]->Clone();
                }
                return pMoveC;
            }

            bool IsExisting(CMove *pMove)
            {
                if(!pMove)
                {
                    //printf("ERROR MOVEHANDLER IS EXISTING");
                    return true;
                }

                for(int i = 0; i < m_lpMoves.size(); ++i)
                {
                    if(*m_lpMoves[i] == *pMove)
                        return true;
                }
                return false;
            }

            void DeleteMove(int index)
            {
                if(index < 0 || index >= m_lpMoves.size())
                    return;

            }

            int GetPoints()
            {
                int points = 0;
                 for(int i = 0; i < m_lpMoves.size(); ++i)
                {
                    if(m_lpMoves[i] != 0)
                        points += m_lpMoves[i]->m_Points;
                }
                return points;
            }
                        //int m_Player;
            int m_MoveType; // 0 == Lay 1 == change
            array<CMove *> m_lpMoves;
        };

};

#endif
