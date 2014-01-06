/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#ifndef CNetwork_H
#define CNetwork_H

#ifdef WIN32
    #include <windows.h>
    #include <winsock2.h>
#else
    #include <sys/socket.h>
    #include <sys/un.h>
    #include <netinet/in.h>
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR   -1
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <sys/select.h>
    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;

    typedef fd_set FD_SET;
    typedef struct timeval TIMEVAL;
#endif
#include "stringbuffer.h"
using namespace std;
class CNetwork
{
    public:
        CNetwork();
        virtual ~CNetwork();
        int Init();
        int Connect(const char* address, int port);
        void Clean();
        void Send(const char *pMsg, int len=0);
        const char *Receive();
        bool Readable(SOCKET socket);
        CStringBuffer *m_pBuffer;
    protected:
    private:
        SOCKET m_Socket;
        SOCKADDR_IN m_SocketAddr;


};

#endif // CAPP_NET_H
