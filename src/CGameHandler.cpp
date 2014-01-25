/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#include "CGameHandler.h"
#include "rapidxml.hpp"
#include <string.h>
#include <stdio.h>
#include <vector>
using namespace rapidxml;
using namespace std;
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
                    CGameState::CMove *tempMove= (CGameState::CMove *) malloc(sizeof(CGameState::CMove));
                    m_pLogic->OnRequestAction(tempMove);
                    SendMove(tempMove);
                }
            }
            else if(strcmp(pName, "welcome")==0)
            {
                int player = strcmp("RED", pNode->first_node("data")->first_attribute("color")->value())==0? 0: 1;
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
    tempState->m_CurrentPlayer = strcmp(pNode->first_attribute("current")->value(),"RED")==0? 0 : 1;
    tempState->m_Turn = atoi(pNode->first_attribute("turn")->value());
    tempState->m_aPoints[0] = atoi(pNode->first_node("red")->first_attribute("points")->value());
    xml_node<> *tempNode = pNode->first_node("red")->first_node("stone");
    int index = 0;
    while(tempNode != 0)
    {
        int shape = CGameState::ShapeToIndex(tempNode->first_attribute("shape")->value());
        int color = CGameState::ColorToIndex(tempNode->first_attribute("color")->value());
        if(shape != -1 && color != -1)
        {
            tempState->m_aStones[index].m_Shape = shape;
            tempState->m_aStones[index++].m_Color = color;
            ++tempState->m_aNumStones[0];
        }
        tempNode = tempNode->next_sibling("stone");
    }

    tempState->m_aPoints[1] = atoi(pNode->first_node("blue")->first_attribute("points")->value());
    tempNode = pNode->first_node("blue")->first_node("cards")->first_node("stone");
    index = 0;
    while(tempNode != 0)
    {
        int shape = CGameState::ShapeToIndex(tempNode->first_attribute("shape")->value());
        int color = CGameState::ColorToIndex(tempNode->first_attribute("color")->value());
        if(shape != -1 && color != -1)
        {
            tempState->m_aStones[index+6].m_Shape = shape;
            tempState->m_aStones[index+6].m_Color = color;
            ++index;
            ++tempState->m_aNumStones[1];
        }
        tempNode = tempNode->next_sibling("stone");
    }
    tempNode = pNode->first_node("nextStones")->first_node("stone");
    index = 0;
    while(tempNode != 0)
    {
        int shape = CGameState::ShapeToIndex(tempNode->first_attribute("shape")->value());
        int color = CGameState::ColorToIndex(tempNode->first_attribute("color")->value());
         if(shape != -1 && color != -1)
        {
            tempState->m_aOpenStones[index].m_Shape = shape;
            tempState->m_aOpenStones[index++].m_Color = color;
            tempState->m_NumOpenStones++; //TODO: Set this to zero first
        }
        tempNode = tempNode->next_sibling("stone");
    }


    tempNode = pNode->first_node("board")->first_node("field");
    index = 0;

    while(tempNode != 0)
    {
        int x=atoi(tempNode->first_attribute("posX")->value());
        int y=atoi(tempNode->first_attribute("posY")->value());
        int shape= CGameState::SHAPE_NONE;
        int color=  CGameState::COLOR_NONE;
        if(tempNode->first_node("stone"))
        {
            shape = CGameState::ShapeToIndex(tempNode->first_node("stone")->first_attribute("shape")->value());
            color = CGameState::ColorToIndex(tempNode->first_node("stone")->first_attribute("color")->value());
        }
        tempState->m_aFields[y*CGameState::FIELD_WIDTH+x].m_Shape = shape;
        tempState->m_aFields[y*CGameState::FIELD_WIDTH+x].m_Color = color;


         /*   index  = CGameState::SymbolToIndex(tempNode->first_attribute("symbol")->value());
        else
            index = CGameState::SymbolToIndex(tempNode->first_attribute("type")->value());
        if(index > -1)
        {
            if(tempNode->first_node("pirates")->first_node("pirate"))
            {


                tempNode = tempNode->first_node("pirates")->first_node("pirate");
                int a = 0;
                while(1)
                {
                   if(strcmp(tempNode->first_attribute("owner")->value(),"RED")==0)
                        ++tempState->m_aField[i].m_NumPirates[0];
                    else
                        ++tempState->m_aField[i].m_NumPirates[1];
                    if(tempNode->next_sibling("pirate") != 0)
                        tempNode = tempNode->next_sibling("pirate");
                    else
                        break;
                }
                tempNode = tempNode->parent()->parent();
            }


            tempState->m_aField[i].m_Symbol=index;
            ++i;
        }*/

        tempNode = tempNode->next_sibling("field");
    }
    tempState->DataToString();
    return tempState;
}
int CGameHandler::SendMoves(CGameState::CMove *ppMoves[])
{

    CStringBuffer buf(2, 512);
    buf.append("<room roomId=\"");
    buf.append(m_pRoomID);
    buf.append("\"><data class=\"moveContainer\">");
    char *moveNames[] ={"<firstMove", "<secondMove", "<thirdMove"};
    char *symbolNames[] ={"BOTTLE", "DAGGER", "HAT", "KEY", "PISTOL","SKULL"};
    int curMove=0;
    for(int i = 0; i < 3; ++i)
    {
        if(ppMoves[i] && ppMoves[i]->m_MoveIndex> -2)
        {
          //  printf("%d" ppMoves);
            buf.append(moveNames[curMove++]);
            buf.append(" class=\"");
            if(ppMoves[i]->m_MoveIndex==-1)
            {
                buf.append("backwardMove\" fieldIndex=\"");
                char aBuf[16];
                sprintf(aBuf, "%d", ppMoves[i]->m_PirateIndex);
                buf.append(aBuf);
                buf.append("\"/>");
            }
            else
            {
                buf.append("forwardMove\" fieldIndex=\"");
                char aBuf[16];
                sprintf(aBuf, "%d", ppMoves[i]->m_PirateIndex);
                buf.append(aBuf);
                buf.append("\" symbol=\"");
                buf.append(symbolNames[ppMoves[i]->m_MoveIndex]);
                buf.append("\"/>");
            }
        }
    }
    buf.append("</data></room>");
    printf("Moves: %s \n", buf.toString());
    m_pConnection->Send(buf.toString());
    return curMove;
}
