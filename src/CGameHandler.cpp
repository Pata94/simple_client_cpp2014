/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#include "CGameHandler.h"
#include "CGameState.h"
#include "rapidxml.hpp"
#include <string.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace rapidxml;
using namespace std;
    int CGameHandler::VAR_OWNPOINTS = 150;
          int  CGameHandler::VAR_ENEMYPOINTS = 50;
          int  CGameHandler::VAR_WINS = -1;
          // static int range = 1000;
    static int step = 50;
    static int range_min = 0;
    static int range_max = 1000;
     static int num = 0;
CGameHandler::CGameHandler(class CNetwork *pCon, const char *pReservation)
{
    //ctor
    m_pConnection = pCon;
    if(pReservation != 0)
    {
        int size=strlen(pReservation);
        m_pReservation = new char[size+1];
        strcpy(m_pReservation, pReservation);
        m_pReservation[size]='\0';
    }
    else
    {
        m_pReservation = 0;
    }
    m_State = STATE_REQUESTING;
    m_pLogic = 0;

}

CGameHandler::~CGameHandler()
{
    //dtor
    if(m_pReservation!=0)
    {
         delete[] m_pReservation;
         m_pReservation=0;
    }

     if(m_pRoomID!=0)
     {
         delete[] m_pRoomID;
         m_pRoomID=0;
     }
     if(m_pLogic!=0)
     {
         delete m_pLogic;
         m_pLogic=0;
     }


}
int CGameHandler::HandleGame()
{

    //just for testing:
/*
    char *pTestMsg = "<state turn=\"18\" stonesInBag=\"56\" start=\"red\" current=\"red\"><nextStones><stone color=\"VIOLET\" shape=\"SPADES\" identifier=\"90\"/><stone color=\"MAGENTA\" shape=\"HEART\" identifier=\"49\"/><stone color=\"VIOLET\" shape=\"ACORN\" identifier=\"74\"/><stone color=\"GREEN\" shape=\"HEART\" identifier=\"32\"/><stone color=\"BLUE\" shape=\"HEART\" identifier=\"15\"/><stone color=\"GREEN\" shape=\"BELL\" identifier=\"22\"/><stone color=\"BLUE\" shape=\"CLUBS\" identifier=\"7\"/><stone color=\"GREEN\" shape=\"ACORN\" identifier=\"21\"/><stone color=\"BLUE\" shape=\"DIAMOND\" identifier=\"12\"/><stone color=\"BLUE\" shape=\"CLUBS\" identifier=\"8\"/><stone color=\"BLUE\" shape=\"BELL\" identifier=\"4\"/><stone color=\"VIOLET\" shape=\"CLUBS\" identifier=\"80\"/></nextStones><red displayName=\"sc-client 2\" points=\"66\"><stone color=\"ORANGE\" shape=\"DIAMOND\" identifier=\"65\"/><stone color=\"ORANGE\" shape=\"BELL\" identifier=\"60\"/><stone color=\"GREEN\" shape=\"SPADES\" identifier=\"34\"/><stone color=\"VIOLET\" shape=\"DIAMOND\" identifier=\"83\"/><stone color=\"MAGENTA\" shape=\"DIAMOND\" identifier=\"46\"/><stone color=\"YELLOW\" shape=\"HEART\" identifier=\"104\"/></red><blue displayName=\"simple_client_cpp 1\" points=\"51\"><stone color=\"GREEN\" shape=\"HEART\" identifier=\"33\"/><stone color=\"YELLOW\" shape=\"ACORN\" identifier=\"93\"/><stone color=\"MAGENTA\" shape=\"ACORN\" identifier=\"39\"/><stone color=\"GREEN\" shape=\"CLUBS\" identifier=\"25\"/><stone color=\"BLUE\" shape=\"ACORN\" identifier=\"1\"/><stone color=\"ORANGE\" shape=\"CLUBS\" identifier=\"63\"/></blue><board><field posX=\"0\" posY=\"0\"/><field posX=\"0\" posY=\"1\"/><field posX=\"0\" posY=\"2\"/><field posX=\"0\" posY=\"3\"/><field posX=\"0\" posY=\"4\"/><field posX=\"0\" posY=\"5\"/><field posX=\"0\" posY=\"6\"/><field posX=\"0\" posY=\"7\"/><field posX=\"0\" posY=\"8\"/><field posX=\"0\" posY=\"9\"/><field posX=\"0\" posY=\"10\"/><field posX=\"0\" posY=\"11\"/><field posX=\"0\" posY=\"12\"/><field posX=\"0\" posY=\"13\"/><field posX=\"0\" posY=\"14\"/><field posX=\"0\" posY=\"15\"/><field posX=\"1\" posY=\"0\"/><field posX=\"1\" posY=\"1\"/><field posX=\"1\" posY=\"2\"/><field posX=\"1\" posY=\"3\"/><field posX=\"1\" posY=\"4\"/><field posX=\"1\" posY=\"5\"/><field posX=\"1\" posY=\"6\"/><field posX=\"1\" posY=\"7\"/><field posX=\"1\" posY=\"8\"/><field posX=\"1\" posY=\"9\"><stone color=\"BLUE\" shape=\"SPADES\" identifier=\"17\"/></field><field posX=\"1\" posY=\"10\"><stone color=\"YELLOW\" shape=\"SPADES\" identifier=\"107\"/></field><field posX=\"1\" posY=\"11\"><stone color=\"VIOLET\" shape=\"SPADES\" identifier=\"89\"/></field><field posX=\"1\" posY=\"12\"><stone color=\"GREEN\" shape=\"SPADES\" identifier=\"35\"/></field><field posX=\"1\" posY=\"13\"><stone color=\"MAGENTA\" shape=\"SPADES\" identifier=\"54\"/></field><field posX=\"1\" posY=\"14\"><stone color=\"ORANGE\" shape=\"SPADES\" identifier=\"71\"/></field><field posX=\"1\" posY=\"15\"/><field posX=\"2\" posY=\"0\"/><field posX=\"2\" posY=\"1\"/><field posX=\"2\" posY=\"2\"/><field posX=\"2\" posY=\"3\"/><field posX=\"2\" posY=\"4\"/><field posX=\"2\" posY=\"5\"/><field posX=\"2\" posY=\"6\"/><field posX=\"2\" posY=\"7\"/><field posX=\"2\" posY=\"8\"><stone color=\"ORANGE\" shape=\"HEART\" identifier=\"69\"/></field><field posX=\"2\" posY=\"9\"><stone color=\"BLUE\" shape=\"HEART\" identifier=\"13\"/></field><field posX=\"2\" posY=\"10\"><stone color=\"YELLOW\" shape=\"HEART\" identifier=\"103\"/></field><field posX=\"2\" posY=\"11\"/><field posX=\"2\" posY=\"12\"/><field posX=\"2\" posY=\"13\"/><field posX=\"2\" posY=\"14\"/><field posX=\"2\" posY=\"15\"/><field posX=\"3\" posY=\"0\"/><field posX=\"3\" posY=\"1\"/><field posX=\"3\" posY=\"2\"/><field posX=\"3\" posY=\"3\"><stone color=\"ORANGE\" shape=\"ACORN\" identifier=\"55\"/></field><field posX=\"3\" posY=\"4\"><stone color=\"GREEN\" shape=\"ACORN\" identifier=\"19\"/></field><field posX=\"3\" posY=\"5\"/><field posX=\"3\" posY=\"6\"/><field posX=\"3\" posY=\"7\"><stone color=\"MAGENTA\" shape=\"SPADES\" identifier=\"52\"/></field><field posX=\"3\" posY=\"8\"><stone color=\"ORANGE\" shape=\"SPADES\" identifier=\"72\"/></field><field posX=\"3\" posY=\"9\"/><field posX=\"3\" posY=\"10\"/><field posX=\"3\" posY=\"11\"/><field posX=\"3\" posY=\"12\"/><field posX=\"3\" posY=\"13\"><stone color=\"YELLOW\" shape=\"DIAMOND\" identifier=\"101\"/></field><field posX=\"3\" posY=\"14\"/><field posX=\"3\" posY=\"15\"/><field posX=\"4\" posY=\"0\"/><field posX=\"4\" posY=\"1\"/><field posX=\"4\" posY=\"2\"/><field posX=\"4\" posY=\"3\"/><field posX=\"4\" posY=\"4\"><stone color=\"GREEN\" shape=\"HEART\" identifier=\"31\"/></field><field posX=\"4\" posY=\"5\"><stone color=\"GREEN\" shape=\"CLUBS\" identifier=\"26\"/></field><field posX=\"4\" posY=\"6\"/><field posX=\"4\" posY=\"7\"/><field posX=\"4\" posY=\"8\"><stone color=\"ORANGE\" shape=\"ACORN\" identifier=\"56\"/></field><field posX=\"4\" posY=\"9\"/><field posX=\"4\" posY=\"10\"><stone color=\"VIOLET\" shape=\"DIAMOND\" identifier=\"82\"/></field><field posX=\"4\" posY=\"11\"><stone color=\"GREEN\" shape=\"DIAMOND\" identifier=\"29\"/></field><field posX=\"4\" posY=\"12\"><stone color=\"YELLOW\" shape=\"DIAMOND\" identifier=\"102\"/></field><field posX=\"4\" posY=\"13\"><stone color=\"ORANGE\" shape=\"DIAMOND\" identifier=\"64\"/></field><field posX=\"4\" posY=\"14\"/><field posX=\"4\" posY=\"15\"/><field posX=\"5\" posY=\"0\"/><field posX=\"5\" posY=\"1\"/><field posX=\"5\" posY=\"2\"><stone color=\"ORANGE\" shape=\"CLUBS\" identifier=\"62\"/></field><field posX=\"5\" posY=\"3\"><stone color=\"YELLOW\" shape=\"CLUBS\" identifier=\"97\"/></field><field posX=\"5\" posY=\"4\"><stone color=\"GREEN\" shape=\"CLUBS\" identifier=\"27\"/></field><field posX=\"5\" posY=\"5\"><stone color=\"MAGENTA\" shape=\"CLUBS\" identifier=\"43\"/></field><field posX=\"5\" posY=\"6\"/><field posX=\"5\" posY=\"7\"><stone color=\"YELLOW\" shape=\"BELL\" identifier=\"95\"/></field><field posX=\"5\" posY=\"8\"><stone color=\"ORANGE\" shape=\"BELL\" identifier=\"58\"/></field><field posX=\"5\" posY=\"9\"><stone color=\"MAGENTA\" shape=\"BELL\" identifier=\"41\"/></field><field posX=\"5\" posY=\"10\"><stone color=\"VIOLET\" shape=\"BELL\" identifier=\"78\"/></field><field posX=\"5\" posY=\"11\"/><field posX=\"5\" posY=\"12\"/><field posX=\"5\" posY=\"13\"/><field posX=\"5\" posY=\"14\"/><field posX=\"5\" posY=\"15\"/><field posX=\"6\" posY=\"0\"/><field posX=\"6\" posY=\"1\"/><field posX=\"6\" posY=\"2\"/><field posX=\"6\" posY=\"3\"/><field posX=\"6\" posY=\"4\"><stone color=\"GREEN\" shape=\"BELL\" identifier=\"24\"/></field><field posX=\"6\" posY=\"5\"/><field posX=\"6\" posY=\"6\"/><field posX=\"6\" posY=\"7\"><stone color=\"YELLOW\" shape=\"CLUBS\" identifier=\"99\"/></field><field posX=\"6\" posY=\"8\"><stone color=\"ORANGE\" shape=\"CLUBS\" identifier=\"61\"/></field><field posX=\"6\" posY=\"9\"/><field posX=\"6\" posY=\"10\"/><field posX=\"6\" posY=\"11\"/><field posX=\"6\" posY=\"12\"/><field posX=\"6\" posY=\"13\"/><field posX=\"6\" posY=\"14\"/><field posX=\"6\" posY=\"15\"/><field posX=\"7\" posY=\"0\"/><field posX=\"7\" posY=\"1\"/><field posX=\"7\" posY=\"2\"/><field posX=\"7\" posY=\"3\"/><field posX=\"7\" posY=\"4\"><stone color=\"GREEN\" shape=\"DIAMOND\" identifier=\"28\"/></field><field posX=\"7\" posY=\"5\"><stone color=\"VIOLET\" shape=\"DIAMOND\" identifier=\"84\"/></field><field posX=\"7\" posY=\"6\"><stone color=\"BLUE\" shape=\"DIAMOND\" identifier=\"10\"/></field><field posX=\"7\" posY=\"7\"><stone color=\"YELLOW\" shape=\"DIAMOND\" identifier=\"100\"/></field><field posX=\"7\" posY=\"8\"><stone color=\"ORANGE\" shape=\"DIAMOND\" identifier=\"66\"/></field><field posX=\"7\" posY=\"9\"/><field posX=\"7\" posY=\"10\"/><field posX=\"7\" posY=\"11\"/><field posX=\"7\" posY=\"12\"/><field posX=\"7\" posY=\"13\"/><field posX=\"7\" posY=\"14\"/><field posX=\"7\" posY=\"15\"/><field posX=\"8\" posY=\"0\"/><field posX=\"8\" posY=\"1\"/><field posX=\"8\" posY=\"2\"/><field posX=\"8\" posY=\"3\"/><field posX=\"8\" posY=\"4\"/><field posX=\"8\" posY=\"5\"><stone color=\"BLUE\" shape=\"DIAMOND\" identifier=\"11\"/></field><field posX=\"8\" posY=\"6\"><stone color=\"BLUE\" shape=\"ACORN\" identifier=\"3\"/></field><field posX=\"8\" posY=\"7\"/><field posX=\"8\" posY=\"8\"/><field posX=\"8\" posY=\"9\"/><field posX=\"8\" posY=\"10\"/><field posX=\"8\" posY=\"11\"/><field posX=\"8\" posY=\"12\"/><field posX=\"8\" posY=\"13\"/><field posX=\"8\" posY=\"14\"/><field posX=\"8\" posY=\"15\"/><field posX=\"9\" posY=\"0\"/><field posX=\"9\" posY=\"1\"/><field posX=\"9\" posY=\"2\"/><field posX=\"9\" posY=\"3\"/><field posX=\"9\" posY=\"4\"/><field posX=\"9\" posY=\"5\"/><field posX=\"9\" posY=\"6\"><stone color=\"BLUE\" shape=\"BELL\" identifier=\"6\"/></field><field posX=\"9\" posY=\"7\"/><field posX=\"9\" posY=\"8\"/><field posX=\"9\" posY=\"9\"/><field posX=\"9\" posY=\"10\"/><field posX=\"9\" posY=\"11\"/><field posX=\"9\" posY=\"12\"/><field posX=\"9\" posY=\"13\"/><field posX=\"9\" posY=\"14\"/><field posX=\"9\" posY=\"15\"/><field posX=\"10\" posY=\"0\"/><field posX=\"10\" posY=\"1\"/><field posX=\"10\" posY=\"2\"/><field posX=\"10\" posY=\"3\"/><field posX=\"10\" posY=\"4\"/><field posX=\"10\" posY=\"5\"/><field posX=\"10\" posY=\"6\"/><field posX=\"10\" posY=\"7\"/><field posX=\"10\" posY=\"8\"/><field posX=\"10\" posY=\"9\"/><field posX=\"10\" posY=\"10\"/><field posX=\"10\" posY=\"11\"/><field posX=\"10\" posY=\"12\"/><field posX=\"10\" posY=\"13\"/><field posX=\"10\" posY=\"14\"/><field posX=\"10\" posY=\"15\"/><field posX=\"11\" posY=\"0\"/><field posX=\"11\" posY=\"1\"/><field posX=\"11\" posY=\"2\"/><field posX=\"11\" posY=\"3\"/><field posX=\"11\" posY=\"4\"/><field posX=\"11\" posY=\"5\"/><field posX=\"11\" posY=\"6\"/><field posX=\"11\" posY=\"7\"/><field posX=\"11\" posY=\"8\"/><field posX=\"11\" posY=\"9\"/><field posX=\"11\" posY=\"10\"/><field posX=\"11\" posY=\"11\"/><field posX=\"11\" posY=\"12\"/><field posX=\"11\" posY=\"13\"/><field posX=\"11\" posY=\"14\"/><field posX=\"11\" posY=\"15\"/><field posX=\"12\" posY=\"0\"/><field posX=\"12\" posY=\"1\"/><field posX=\"12\" posY=\"2\"/><field posX=\"12\" posY=\"3\"/><field posX=\"12\" posY=\"4\"/><field posX=\"12\" posY=\"5\"/><field posX=\"12\" posY=\"6\"/><field posX=\"12\" posY=\"7\"/><field posX=\"12\" posY=\"8\"/><field posX=\"12\" posY=\"9\"/><field posX=\"12\" posY=\"10\"/><field posX=\"12\" posY=\"11\"/><field posX=\"12\" posY=\"12\"/><field posX=\"12\" posY=\"13\"/><field posX=\"12\" posY=\"14\"/><field posX=\"12\" posY=\"15\"/><field posX=\"13\" posY=\"0\"/><field posX=\"13\" posY=\"1\"/><field posX=\"13\" posY=\"2\"/><field posX=\"13\" posY=\"3\"/><field posX=\"13\" posY=\"4\"/><field posX=\"13\" posY=\"5\"/><field posX=\"13\" posY=\"6\"/><field posX=\"13\" posY=\"7\"/><field posX=\"13\" posY=\"8\"/><field posX=\"13\" posY=\"9\"/><field posX=\"13\" posY=\"10\"/><field posX=\"13\" posY=\"11\"/><field posX=\"13\" posY=\"12\"/><field posX=\"13\" posY=\"13\"/><field posX=\"13\" posY=\"14\"/><field posX=\"13\" posY=\"15\"/><field posX=\"14\" posY=\"0\"/><field posX=\"14\" posY=\"1\"/><field posX=\"14\" posY=\"2\"/><field posX=\"14\" posY=\"3\"/><field posX=\"14\" posY=\"4\"/><field posX=\"14\" posY=\"5\"/><field posX=\"14\" posY=\"6\"/><field posX=\"14\" posY=\"7\"/><field posX=\"14\" posY=\"8\"/><field posX=\"14\" posY=\"9\"/><field posX=\"14\" posY=\"10\"/><field posX=\"14\" posY=\"11\"/><field posX=\"14\" posY=\"12\"/><field posX=\"14\" posY=\"13\"/><field posX=\"14\" posY=\"14\"/><field posX=\"14\" posY=\"15\"/><field posX=\"15\" posY=\"0\"/><field posX=\"15\" posY=\"1\"/><field posX=\"15\" posY=\"2\"/><field posX=\"15\" posY=\"3\"/><field posX=\"15\" posY=\"4\"/><field posX=\"15\" posY=\"5\"/><field posX=\"15\" posY=\"6\"/><field posX=\"15\" posY=\"7\"/><field posX=\"15\" posY=\"8\"/><field posX=\"15\" posY=\"9\"/><field posX=\"15\" posY=\"10\"/><field posX=\"15\" posY=\"11\"/><field posX=\"15\" posY=\"12\"/><field posX=\"15\" posY=\"13\"/><field posX=\"15\" posY=\"14\"/><field posX=\"15\" posY=\"15\"/></board><move type=\"LAY\"><stoneToField><stone color=\"GREEN\" shape=\"SPADES\" identifier=\"35\"/><field posX=\"1\" posY=\"12\"/></stoneToField><stoneToField><stone color=\"MAGENTA\" shape=\"SPADES\" identifier=\"54\"/><field posX=\"1\" posY=\"13\"/></stoneToField><stoneToField><stone color=\"ORANGE\" shape=\"SPADES\" identifier=\"71\"/><field posX=\"1\" posY=\"14\"/></stoneToField></move></state>";
    int size2=strlen(pTestMsg)+1;
             char *pChr2 = new char[size2];
            strcpy(pChr2, pTestMsg);
            pChr2[size2-1]='\0';

    xml_document<> doc2;
      doc2.parse<0>(pChr2);
            xml_node<> *pNode2=doc2.first_node();

    CGameState *pTestState = GetStateFromXML(pNode2);//->first_node("state"));
    CMoveHandler::CMoveContainer *possibleMoves =  pTestState->GetPossibleMoves(pTestState->m_CurrentPlayer);

    m_pLogic = new CBaseLogic(pTestState->m_CurrentPlayer);
    m_pLogic->OnGameStateUpdate(pTestState);
    CMoveHandler::CMoveContainer *pMoves = 0;
    m_pLogic->OnRequestAction(&pMoves);
    for(int i = 0; i < pMoves->m_lpMoves.size();i++)
    {   int x = pMoves->m_lpMoves[i]->m_FieldIndex%FIELD_WIDTH;
        int y = (pMoves->m_lpMoves[i]->m_FieldIndex-x)/FIELD_WIDTH;
        printf("Move %i, index: %i x: %i, y: %i, Color: %s, shape: %s \n", i, pMoves->m_lpMoves[i]->m_FieldIndex,x,y, CGameState::m_aColorNames[pMoves->m_lpMoves[i]->m_pStone->m_Color], CGameState::m_aShapeNames[possibleMoves->m_lpMoves[i]->m_pStone->m_Shape]) ;
    }
     printf("points: %d", pMoves->GetPoints());
    return 0;*/
    if(m_pConnection == 0)
        return -1;
    if(m_pReservation != 0)
    {
        char aBuf[512];
        sprintf(aBuf, "<protocol><joinPrepared reservationCode=\"%s\"/>", m_pReservation);
        m_pConnection->Send(aBuf);

    }
    else
        m_pConnection->Send("<protocol><join gameType=\"swc_2014_sixpack\"/>\0");


    xml_document<> doc;
    char *pChr=0;
    while(m_State != STATE_END && m_State != STATE_ERROR)
    {
        if(m_pConnection != 0 && m_pConnection->Receive() != 0)
        {
            m_pConnection->m_pBuffer->toString();

            if(pChr)
                delete[] pChr;
            int size=m_pConnection->m_pBuffer->GetSize()+1;
            pChr = new char[size];
            strcpy(pChr, m_pConnection->m_pBuffer->toString());
            pChr[size-1]='\0';

            while(1)
            {
                char *temp = strstr(pChr, "<protocol>");
                if(!temp)
                    break;
                strncpy(temp, "          ", 10);
            }
            if(strstr(pChr, "</protocol>")!=0 || strstr(pChr, "<protocol/>")!=0)
            {
                printf("ENDE: %s \n", pChr);
                m_State=STATE_END;
                break;
            }


            doc.parse<0>(pChr);
            xml_node<> *pNode=0;
            for(pNode = doc.first_node(); pNode!=0 && m_State != STATE_ERROR && m_State != STATE_END; pNode=pNode->next_sibling())
                m_State = OnMsg(pNode);
        }
        else
             m_State = STATE_ERROR;

    }
    if(pChr)
        delete[] pChr;


    /*if(m_pLogic->m_pGameState->m_aPoints[m_pLogic->m_Player] > m_pLogic->m_pGameState->m_aPoints[m_pLogic->m_Player^1])
    {
        //gewonnen
          fstream f;
            f.open("parameter.dat", ios::out|ios::app);

            //char aBuf[512];
            //    sprintf(aBuf, "%i, %i, %i", VAR_OWNPOINTS, VAR_ENEMYPOINTS, VAR_WINS+1);
                 f  << VAR_OWNPOINTS <<" " << VAR_ENEMYPOINTS <<" " << (VAR_WINS+1) << endl;
            f.close();

    }
    else
    {
        //verloren
          fstream f;
            f.open("parameter.dat", ios::out|ios::app);

            char aBuf[512];


            int a = (VAR_OWNPOINTS+step);
            int b = VAR_ENEMYPOINTS;
            if(a < range_min)
                a = range_min;


            if(a > range_max)
            {

                a = range_min;
            }
            if(a < VAR_OWNPOINTS)
            {
                b = (VAR_ENEMYPOINTS+step);
                if( b < range_min)
                    b = range_min;
                if(b > range_max)
                {
                    a = 0;
                    b = 0;
                }
            }
            sprintf(aBuf, "%i, %i, %i", a, b, 0);
            f  << a <<" " << b <<" " << 0 << endl;
            f.close();
    }*/
    return m_State;
}

int CGameHandler::OnMsg(xml_node<> *pNode)
{

    char *pName=pNode->name();
    if(strcmp(pName, "room")==0)
    {
        if(strcmp(pNode->first_attribute("roomId")->value(), m_pRoomID)==0)
        {
            pName = pNode->first_node("data")->first_attribute("class")->value();
            if(m_pLogic)
            {
                if(strcmp(pName, "memento")==0)
                {

                    m_pLogic->OnGameStateUpdate(GetStateFromXML(pNode->first_node("data")->first_node("state")));
                }
                else if(strcmp(pName, "sc.framework.plugins.protocol.MoveRequest")==0)
                {


                    if(VAR_WINS == -1)
                    {
                          Sleep(1000);
                        VAR_OWNPOINTS = range_min;
                        VAR_ENEMYPOINTS = range_min;

                         fstream f;
                        char astring[512];
                        char cstring[512];
                        f.open("parameter.dat", ios::in);
                        bool bnew = false;
                        if(!f)
                        {
                            f.open("parameter.dat", ios::out|ios::app);

                                f << "";
                                f.close();
                                bnew = true;
                        }
                        ifstream infile("parameter.dat");
                        string line;
                         printf("-----");
                        while (getline(infile, line))
                        {
                            printf("%s \n", line.c_str());
                            istringstream iss(line);
                            int a, b, c;
                            if (!(iss >> a >> b >> c)) { break; }
                            VAR_OWNPOINTS =a;
                            VAR_ENEMYPOINTS = b;
                            VAR_WINS = c;
                        }
                          printf("----- %i, %i, %i", VAR_OWNPOINTS, VAR_ENEMYPOINTS, VAR_WINS);
                        infile.close();
                    }
                    CMoveHandler::CMoveContainer *pMoves = 0;
                    m_pLogic->OnRequestAction(&pMoves);
                    SendMove(pMoves);
                    delete pMoves;
                }
            }
            else if(strcmp(pName, "welcome")==0)
            {
                int player = strcmp("red", pNode->first_node("data")->first_attribute("color")->value())==0? 0: 1;
                m_pLogic = new CBaseLogic(player);
            }
        }
        else
            printf("RoomID-Error, ours:%s, theirs:%s", m_pRoomID, pNode->first_attribute("roomId")->value());
    }
    else if(strcmp(pName, "joined")==0)
    {
        int len = strlen(pNode->first_attribute("roomId")->value());
        m_pRoomID= new char[len+1];
        strcpy(m_pRoomID, pNode->first_attribute("roomId")->value());
        m_pRoomID[len] = '\0';
        printf("game joined id: %s", m_pRoomID);
    }
    else if(strcmp(pName, "left")==0)
        return STATE_END;

    return 0;
}


CGameState *CGameHandler::GetStateFromXML(xml_node<> *pNode)
{
    CGameState *tempState = new CGameState();
    tempState->m_CurrentPlayer = strcmp(pNode->first_attribute("current")->value(),"red")==0? 0 : 1;
    tempState->m_Turn = atoi(pNode->first_attribute("turn")->value());
    tempState->m_NumBagStones = atoi(pNode->first_attribute("stonesInBag")->value());
    xml_node<> *tempNode = pNode->first_node("nextStones")->first_node("stone");
    int index = 0;
    while(tempNode != 0)
    {
        int shape = CGameState::ShapeToIndex(tempNode->first_attribute("shape")->value());
        int color = CGameState::ColorToIndex(tempNode->first_attribute("color")->value());
        int identifier = atoi(tempNode->first_attribute("identifier")->value());
        if(shape != -1 && color != -1)
        {
            tempState->m_apOpenStones[index] = 0;
            tempState->m_apOpenStones[index] = new CStoneHandler::CStone();
            tempState->m_apOpenStones[index]->m_Color = color;
            tempState->m_apOpenStones[index]->m_Shape = shape;
            tempState->m_apOpenStones[index]->m_Position = POSITION_OPEN;
            tempState->m_apOpenStones[index]->m_Identifier = identifier;

        }
        else
        printf("ERROR 0");
        ++index;
        tempNode = tempNode->next_sibling("stone");
    }
    for(int i = index; i < 12; ++i)
        tempState->m_apOpenStones[i] = 0; //set all other stones to null

    tempState->m_aPoints[0] = atoi(pNode->first_node("red")->first_attribute("points")->value());
    tempNode = pNode->first_node("red")->first_node("stone");
    index = 0;
    while(tempNode != 0)
    {
        int shape = CGameState::ShapeToIndex(tempNode->first_attribute("shape")->value());
        int color = CGameState::ColorToIndex(tempNode->first_attribute("color")->value());
        int identifier = atoi(tempNode->first_attribute("identifier")->value());
        if(shape != -1 && color != -1)
        {
            tempState->m_apHandStones[index] = 0;
            tempState->m_apHandStones[index] = new CStoneHandler::CStone();
            tempState->m_apHandStones[index]->m_Color = color;
            tempState->m_apHandStones[index]->m_Shape = shape;
            tempState->m_apHandStones[index]->m_Position = POSITION_RED;
            tempState->m_apHandStones[index]->m_Identifier = identifier;

        }
        else
            printf("ERROR 1");
        ++index;
        tempNode = tempNode->next_sibling("stone");
    }
     for(int i = index; i < 6; ++i)
            tempState->m_apHandStones[i] = 0;

    tempState->m_aPoints[1] = atoi(pNode->first_node("blue")->first_attribute("points")->value());
    tempNode = pNode->first_node("blue")->first_node("stone");
    index = 6;
    while(tempNode != 0)
    {
        int shape = CGameState::ShapeToIndex(tempNode->first_attribute("shape")->value());
        int color = CGameState::ColorToIndex(tempNode->first_attribute("color")->value());
        int identifier = atoi(tempNode->first_attribute("identifier")->value());
        if(shape != -1 && color != -1)
        {
            tempState->m_apHandStones[index] = 0;
            tempState->m_apHandStones[index] = new CStoneHandler::CStone();
            tempState->m_apHandStones[index]->m_Color = color;
            tempState->m_apHandStones[index]->m_Shape = shape;
            tempState->m_apHandStones[index]->m_Position = POSITION_BLUE;
            tempState->m_apHandStones[index]->m_Identifier = identifier;

        }
        else
            printf("ERROR 2");
        ++index;
        tempNode = tempNode->next_sibling("stone");
    }
     for(int i = index; i < 12; ++i)
            tempState->m_apHandStones[i] = 0;


    tempNode = pNode->first_node("board")->first_node("field");

    while(tempNode != 0)
    {
        int x = atoi(tempNode->first_attribute("posX")->value());
        int y = atoi(tempNode->first_attribute("posY")->value());
        if(tempNode->first_node("stone"))
        {
            tempNode = tempNode->first_node("stone");
            int shape = CGameState::ShapeToIndex(tempNode->first_attribute("shape")->value());
            int color = CGameState::ColorToIndex(tempNode->first_attribute("color")->value());
            int identifier = atoi(tempNode->first_attribute("identifier")->value());
            if(shape != -1 && color != -1)
            {
                CStoneHandler::CField *pField = &tempState->m_pFieldHandler->m_aField[x+y*FIELD_WIDTH]; //TODO 15 !
                pField->m_pStone = new CStoneHandler::CStone();
                pField->m_pStone->m_Color = color;
                pField->m_pStone->m_Shape = shape;
                pField->m_pStone->m_Identifier = identifier;
              //  printf("%d", identifier);
                pField->m_pStone->m_Position = POSITION_FIELD;
            }
            else
                printf("ERROR 3");
            tempNode = tempNode->parent();
        }
        tempNode = tempNode->next_sibling("field");
    }

   // tempState->DataToString();
    return tempState;
}
int CGameHandler::SendMove(CMoveHandler::CMoveContainer *pMoves)
{
    array<CMoveHandler::CMove *> *p = &pMoves->m_lpMoves;
    CStringBuffer buf(pMoves->m_lpMoves.size()+1, 512);
    buf.append("<room roomId=\"");
    buf.append(m_pRoomID);
    if(pMoves->m_MoveType == MOVE_PLACE)
    {
        buf.append("\"><data class=\"laymove\">");
        for(int i = 0; i < pMoves->m_lpMoves.size(); ++i)
        {
            if(!pMoves->m_lpMoves[i]->m_pStone)
                continue;
            int x = pMoves->m_lpMoves[i]->m_FieldIndex%16;
            int y = (pMoves->m_lpMoves[i]->m_FieldIndex-x)/16;
            buf.append("<stoneToField>");
             buf.append("<stone color=\"");
             buf.append(CGameState::m_aColorNames[pMoves->m_lpMoves[i]->m_pStone->m_Color]);
             buf.append("\" shape=\"");
             buf.append(CGameState::m_aShapeNames[pMoves->m_lpMoves[i]->m_pStone->m_Shape]);
             buf.append("\" identifier=\"");

            char aBuf[16];
            sprintf(aBuf, "%d", pMoves->m_lpMoves[i]->m_pStone->m_Identifier);
            buf.append(aBuf);
            buf.append("\"/><field posX=\"");
            sprintf(aBuf, "%d", x);
            buf.append(aBuf);
            buf.append("\" posY=\"");
            sprintf(aBuf, "%d", y);
            buf.append(aBuf);
            buf.append("\"/></stoneToField>");
        }
        buf.append( "<hint content=\" Points:");
            char aBuf[16];
            sprintf(aBuf, "%d", pMoves->GetPoints());
            buf.append(aBuf);
            buf.append("\"/>");

    }
    else if(pMoves->m_MoveType == MOVE_EXCHANGE)
    {
        buf.append("\"><data class=\"exchangemove\">");
         for(int i = 0; i < pMoves->m_lpMoves.size(); ++i)
        {
            buf.append("<select color=\"");
            buf.append(CGameState::m_aColorNames[pMoves->m_lpMoves[i]->m_pStone->m_Color]);
            buf.append("\" shape=\"");
            buf.append(CGameState::m_aShapeNames[pMoves->m_lpMoves[i]->m_pStone->m_Shape]);
            buf.append("\" identifier=\"");
             char aBuf[16];
            sprintf(aBuf, "%d", pMoves->m_lpMoves[i]->m_pStone->m_Identifier);
            buf.append(aBuf);
            buf.append("\"/>");
        }
    }



    buf.append("</data></room>");


    printf("Moves: %d, %s \n",pMoves->m_lpMoves.size(),  buf.toString());
    m_pConnection->Send(buf.toString());
    return 0;
}
