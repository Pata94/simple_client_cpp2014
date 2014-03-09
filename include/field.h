#ifndef FIELD_H
#define FIELD_H

template<typename T>
class field_base
{

private:
    T* m_pField;
    int m_Width;
    int m_Height;
public:

    field_base(int Width, int Height)
    {
        if(Width*Height > 0)
        {
             m_pField = calloc(Width*Height, sizeof(T));
             m_Width = Width;
             m_Height = Height;
        }

    }

    T& operator[] (int index)
	{
	    if(!m_pField)
            return 0;
		return m_pField+index*sizeof(T);
	}
	 T& operator[][] (int index0, int index1)
	{
	    if(!m_pField)
            return 0;
		return m_pField+sizeof(T)*(index0*+index1*m_Width);
	}


};
#endif
