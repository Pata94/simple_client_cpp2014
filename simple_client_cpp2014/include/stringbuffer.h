/*(c)2013 by Patrick Rathje visit https://github.com/Pata94 and ReadMe for more information
made for Software-Challenge 2013 visit http://www.informatik.uni-kiel.de/software-challenge/  for more information*/
#ifndef STRINGBUFFER_H
#define STRINGBUFFER_H
// by Patrick Rathje
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
class CStringBuffer
{
public:

	void init(int numbuffers, int buffersize)
	{
        m_NumBuffers = numbuffers;
        m_Buffersize = buffersize;
        m_Index = -1;
        m_Size = 0;
        m_CurrentSize = 0;
        m_paBuffers = new char*[m_NumBuffers];
        m_paBuffers[++m_Index]=(char*) malloc(m_Buffersize);
        m_pChr = 0;
	}
	void clean()
	{
		 if(m_pChr)
            free(m_pChr);
		if(m_paBuffers)
			free(m_paBuffers);
		m_pChr = 0;
		m_NumBuffers = 0;
		m_Buffersize=0;
		m_paBuffers = 0;
	};
	CStringBuffer(int numbuffers, int buffersize)
    {
        m_pChr = 0;
        m_NumBuffers = 0;
		m_Buffersize=0;
		m_paBuffers = 0;
		init(numbuffers, buffersize);
   	 };


    ~CStringBuffer()
    {
        clean();
    };

    int append(const char *pSrc, int length = 0)
    {
        if(m_pChr)
        {
            free(m_pChr);
            m_pChr = 0;
        }
        if(length <= 0)
            length = strlen(pSrc);

	int offset = 0;
	while(length > 0)
	{
		if(length+m_CurrentSize > m_Buffersize)
		{
			int size=m_Buffersize-m_CurrentSize;
			memcpy(m_paBuffers[m_Index]+m_CurrentSize, pSrc+offset, size);
			offset+=size;
			length-=size;

			m_paBuffers[++m_Index]=(char*) malloc(m_Buffersize);
   			m_CurrentSize=0;
		}
		else
		{
			memcpy(m_paBuffers[m_Index]+m_CurrentSize, pSrc+offset, length);
			m_CurrentSize+=length;
			length = 0;
		}
	}
        /*int Max=(length/m_Buffersize)+1;
        int offset = 0;
        int curlength=0;
        //printf("2: %s:%d:%d:%d \n",pSrc, length, Max, m_Buffersize);
        for(int i=0; i < Max; ++i)
        {
            if(m_CurrentSize>=m_Buffersize)
            {
               // printf("ad: %d:%d \n", i,m_Index);
                m_paBuffers[++m_Index]=(char*) malloc(m_Buffersize);
                m_CurrentSize=0;
            }
            curlength= length > m_Buffersize-m_CurrentSize? m_Buffersize-m_CurrentSize: length;
            memcpy(m_paBuffers[m_Index]+m_CurrentSize, pSrc+offset, curlength);
            offset+=curlength;
            length-=curlength;
            m_CurrentSize+=curlength;
        }*/
        m_Size = m_Buffersize*m_Index+m_CurrentSize;
        return m_Size;
    };
    const char *toString()
    {
        if(m_pChr != 0)
            return m_pChr;
        else if(m_paBuffers == 0)
            return 0;

        m_pChr= (char*) malloc(m_Size+1);
        for(int i = 0; i < m_Index; ++i)
        	memcpy(m_pChr+m_Buffersize*i, m_paBuffers[i], m_Buffersize);

	memcpy(m_pChr+m_Buffersize*m_Index, m_paBuffers[m_Index], m_CurrentSize);
        m_pChr[m_Size]= '\0';
        return m_pChr;
    };
    int GetSize()
    {
        return m_Size;
    }
private:
   int m_Size;
   int m_CurrentSize;
   int m_NumBuffers;
   int m_Buffersize;
   int m_Index;
   char **m_paBuffers;
   char *m_pChr;
};
#endif // STRINGBUFFER_H
