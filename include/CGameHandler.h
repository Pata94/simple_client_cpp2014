/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#ifndef CGAMEHANDLER_H
#define CGAMEHANDLER_H

#include "CNetwork.h"
#define RAPIDXML_ALIGNMENT 4
#include "rapidxml.hpp"
#include "CBaseLogic.h"
#include "CGameState.h"
class CGameHandler
{public:

        static int VAR_OWNPOINTS;
         static int  VAR_ENEMYPOINTS;
         static int  VAR_WINS;
        CGameHandler(class CNetwork *pCon, const char *pReservation);
        virtual ~CGameHandler();
        int HandleGame();
        int OnMsg(rapidxml::xml_node<> *pNode);
        CGameState *GetStateFromXML(rapidxml::xml_node<> *pNode);
        int SendMove(CMoveHandler::CMoveContainer *pMoves);
    protected:
    private:

        class CNetwork *m_pConnection;
        int m_State;
        char *m_pRoomID;
        char *m_pReservation;
        class CBaseLogic *m_pLogic;
};

#endif // CGAMEHANDLER_H
