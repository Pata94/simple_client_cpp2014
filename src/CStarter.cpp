/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#include "CStarter.h"
#include <iostream>
#include "CStarter.h"
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[])
{
    CStarter temp;
    return temp.OnExecute(argc, argv);
}

CStarter::CStarter()
{
    //ctor
}

CStarter::~CStarter()
{
    //dtor
    if(m_pNetwork)
    {
        delete m_pNetwork;
        m_pNetwork=0;
    }
     if(m_pHandler)
    {
        delete m_pHandler;
        m_pHandler=0;
    }
}

int CStarter::OnExecute(int argc, char* argv[])
{
    m_pNetwork = new CNetwork();
    m_pHandler = 0;
    char *pHost="127.0.0.1";
    char *pRes=0;
    int Port=13050;
    for(int i=0; i < argc; i++)
    {
        char *temp=0;
        if(strncmp(argv[i], "--", 2)==0 && i<argc-1)
        {

            temp=argv[i]+2;
            if(strcmp(temp, "host")==0)
            {
                ++i;
                temp=argv[i];
                int size=strlen(temp);
                pHost=new char[size+1];
                strcpy(pHost, temp);
                pHost[size]='\0';
            }
            else if(strcmp(temp, "port")==0)
            {
                ++i;
                Port=atoi(argv[i]);
            }
            else if(strcmp(temp, "reservation")==0)
            {
                ++i;
                temp=argv[i];
                int size=strlen(temp);
                pRes=new char[size+1];
                strcpy(pRes, temp);
                pRes[size]='\0';
            }
             else if(strcmp(temp, "strategy")==0)
            {
                    //not used
            }
        }
        else if(strncmp(argv[i], "-", 1)==0 && i<argc-1)
        {
            temp=argv[i]+1;
            if(strcmp(temp, "h")==0)
            {
                ++i;
                temp=argv[i];
                int size=strlen(temp);
                pHost=new char[size+1];
                strcpy(pHost, temp);
                pHost[size]='\0';
            }
            else if(strcmp(temp, "p")==0)
            {
                ++i;
                Port=atoi(argv[i]);
            }
            else if(strcmp(temp, "r")==0)
            {
                ++i;
                temp=argv[i];
                int size=strlen(temp);
                pRes=new char[size+1];
                strcpy(pRes, temp);
                pRes[size]='\0';
            }
             else if(strcmp(temp, "s")==0)
            {
                    //not used
            }
        }
        else
        {
            if(strncmp(argv[i], "host", 4)==0)
            {
                temp=argv[i]+5;
                int size=strlen(temp);
                pHost=new char[size+1];
                strcpy(pHost, temp);
                pHost[size]='\0';
            }
            else if(strncmp(argv[i], "port", 4)==0)
            {
                Port=atoi(argv[i]+5);
            }
            else if(strncmp(argv[i], "reservation", 11)==0)
            {
                temp=argv[i]+12;
                int size=strlen(temp);
                pRes=new char[size+1];
                strcpy(pRes, temp);
                pRes[size]='\0';
            }
             else if(strncmp(argv[i], "strategy", 8)==0)
            {
                    //not used
            }
        }
    }
     if(strcmp(pHost, "localhost")==0)
    {
        delete[] pHost;
        pHost="127.0.0.1";
    }
    int ret = m_pNetwork->Connect(pHost, Port);
    if(ret != 0)
    {
        printf("Error: konnte keine Verbindung mit dem Server herstellen, fehler code: %d\n", ret);
        return ret;
    }
    m_pHandler = new CGameHandler(m_pNetwork, pRes);

    return m_pHandler->HandleGame();
}
