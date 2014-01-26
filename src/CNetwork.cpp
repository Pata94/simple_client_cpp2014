/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#include "CNetwork.h"
#include <stdio.h>
#include <vector>
using namespace std;
#ifdef WIN32
int startWinsock()
{
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2,0),&wsa);
}
#endif
CNetwork::CNetwork()
{
    m_pBuffer = new CStringBuffer(125, 512);
}
int CNetwork::Init()
{
    Clean();

    #ifdef WIN32
        int ret = startWinsock();

        if(ret!= 0)
        {
            printf("Network-Fehler: startWinsock, fehler code: %d\n",ret);
            return ret;
        }
    #endif

    m_Socket=socket(AF_INET,SOCK_STREAM,0);

    if(m_Socket == INVALID_SOCKET)
    {
        printf("Network-Fehler: invalid socket");
        Clean();
        return -1;
    }
    return 0;
}

int CNetwork::Connect(const char* address, int port)
{
    long ret = Init();
    if(ret!= 0)
    {
        printf("Network-Fehler: Initialisieren fehlgeschlagen, fehler code: %d\n", ret);
        return ret;
    }
    memset(&m_SocketAddr,0,sizeof(SOCKADDR_IN));
    m_SocketAddr.sin_family=AF_INET;
    m_SocketAddr.sin_port=htons(port);
    m_SocketAddr.sin_addr.s_addr=inet_addr(address);
    ret=connect(m_Socket,(SOCKADDR*)&m_SocketAddr,sizeof(SOCKADDR));
    if(ret==SOCKET_ERROR)
    {
        #ifdef WIN32
          printf("Fehler: connect gescheitert, fehler code: %d\n",WSAGetLastError());
          return WSAGetLastError();
        #else
            printf("Fehler: connect gescheitert, fehler code: %d\n",ret);
          return ret;
        #endif
    }
    printf("Verbunden mit %s:%d\n", address, port);
    return 0;
}

void CNetwork::Clean()
{
    #ifdef WIN32
        if(m_Socket != 0)
        {
            closesocket(m_Socket);
            m_Socket = 0;
        }
        WSACleanup();
    #else
        if(m_Socket != 0)
        {
            close(m_Socket);
            m_Socket = 0;
        }
    #endif
}

CNetwork::~CNetwork()
{
    //dtor
    Clean();
    if(m_pBuffer)
        delete m_pBuffer;
}

void CNetwork::Send(const char *pMsg, int len){

    if(m_Socket==0)
        return;
    if(len > 0)
        send (m_Socket, pMsg, len, 0);
    else
        send (m_Socket, pMsg, strlen(pMsg), 0);

};
bool CNetwork::Readable(SOCKET socket)
{
    FD_SET fdSet;
    TIMEVAL timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    FD_ZERO(&fdSet);
    FD_SET(socket,&fdSet);
    if(select(socket+1,&fdSet,0,0,&timeout) <= 0)
      {
          FD_ZERO(&fdSet);
      }
    if(!FD_ISSET(socket,&fdSet))
      {
          return false;
      }
    return true;
}
const char *CNetwork::Receive()
{
        if(m_Socket==0)
        return 0;
    m_pBuffer->clean();
    m_pBuffer->init(125, 512);
    char aBuf[512];
    int len=0;
    int times = 0;

    for(len=recv(m_Socket,aBuf,512,0);; len=recv(m_Socket,aBuf,512,0))
    {
        m_pBuffer->append(aBuf, len);
        if(!Readable(m_Socket))
        {

            #ifdef WIN32
                Sleep(100);
            #else
                usleep(100000);
            #endif
             if(!Readable(m_Socket))
                   break;
        }
        else if(strstr(aBuf, "</protocol>")!=0 || strstr(aBuf, "<protocol/>")!=0)
            break;
    }
    //printf("Stringbuffer: %s \n", m_pBuffer->toString());
    return m_pBuffer->toString();
}
