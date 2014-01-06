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
{
    enum{
        STATE_REQUESTING,
        STATE_JOINING,
        STATE_JOINED,
        STATE_WAITING,
        STATE_MOVING,
        STATE_END,
        STATE_LEAVE,
        STATE_ERROR
    };
    public:
        CGameHandler(class CNetwork *pCon, const char *pReservation);
        virtual ~CGameHandler();
        int HandleGame();
        int OnMsg(rapidxml::xml_node<> *pNode);
        CGameState *GetStateFromXML(rapidxml::xml_node<> *pNode);
        int SendMoves(CGameState::CMove **ppMoves);
    protected:
    private:
        class CNetwork *m_pConnection;
        int m_State;
        char *m_pRoomID;
        char *m_pReservation;
        class CBaseLogic *m_pLogic;
};

#endif // CGAMEHANDLER_H
