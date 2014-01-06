/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#ifndef CSTARTER_H
#define CSTARTER_H


#include "CNetwork.h"
#include "CGameState.h"
#include "CGameHandler.h"
class CStarter
{
    public:
        CStarter();
        virtual ~CStarter();
        int OnExecute(int argc, char* argv[]);
    protected:
    private:
        class CNetwork *m_pNetwork;
        class CGameHandler *m_pHandler;
};

#endif // CSTARTER_H
