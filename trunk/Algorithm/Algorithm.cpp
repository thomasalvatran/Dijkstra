#include "Algorithm.h"


inline void Algorithm::update(int sum, int node)
{
    ma_checked[node] = true;
    for(int i = 0; i < m_size ; i++)
    {
        //Checking Connection
        if( ma_value[node][i] > 0)
            if( sum + ma_value[node][i] + ma_value[i][i] <= ma_sumValue[i])
            {
                ma_sumValue[i] = sum + ma_value[node][i] + ma_value[i][i];
                ma_prevNode[i] = node ;
            }
    }
}

Algorithm::Algorithm(int** array,int size): ma_value(array),m_size(size)
{
    ma_sumValue = new int[m_size]; 
    ma_prevNode = new int[m_size]; 
    ma_checked = new char[m_size]; 

    for(int i = 0; i < m_size ; i ++)
    {
        ma_sumValue[i] = INT_MAX;
        ma_prevNode[i] = -1 ;
        ma_checked[i] = false;
    }
}

Algorithm::~Algorithm()
{
    for(int i = 0; i > m_size; i ++)
        delete ma_value[i];
    delete ma_value;
    delete ma_sumValue;
    delete ma_prevNode;
    delete ma_checked;
}

inline void Algorithm::findMin(int &min, int& index)
{
    min = INT_MAX;
    index = -1;
    for(int i = 0; i < m_size; i++)
    {
        if( !ma_checked[i] && ma_sumValue[i] < min)
        {
            min = ma_sumValue[i];
            index = i;
        }
    }
    if( min == INT_MAX)
        index = -1;
} 

int Algorithm::resolve(int start,int end, int* &node_id, int* &value)
{
    ma_sumValue[start] = ma_value[start][start];
    ma_prevNode[start] = -1;// prevNode not exist

    update(ma_sumValue[start],start);

    int min;
    int index;

    for( int k = 1 ; k < m_size ; k++)
    {
        findMin(min,index);

        if(index == -1)
            break;

        update(min,index);
    }

    return genSolution(start,end,node_id,value);    
}

inline int Algorithm::genSolution(int start, int end, int* &node_id, int* &value)
{
    node_id = new int[m_size];
    value = new int[m_size];

    if( start == -1 || end == -1)
        return false;

    int prev = end;
    node_id[0] = prev;
    value[0] = ma_sumValue[prev];
    int i = 1;
    while( prev != start)
    {
        prev = ma_prevNode[prev];
        if(prev == -1)
            return false;
        node_id[i] = prev;
        value[i] = ma_sumValue[prev];
        i++;
    }

    for(int i =0; i < m_size; i++)
    return true;
}
