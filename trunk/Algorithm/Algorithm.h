#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <climits>
#include <stdio.h>


class Algorithm{

    private:

    int** ma_value;
    int* ma_sumValue;
    int* ma_prevNode;
    char* ma_checked;
    int m_size;

    /*
     * updateTable for first place in table
     */
    inline void update(int sum, int node);

    public:
    /*
     * Constructor
     */
    Algorithm(int** array,int size);

    ~Algorithm();

    /*
     * Finding minimum and index in value array 
     */

    inline void findMin(int &min, int& index);

    /* Function resolving dijkstra algorithm
     * @param[in] node - start node number 
     */
    int resolve(int start,int end, int* &node_id, int* &value);

    /*
     * Generate solution
     */
    int genSolution(int start, int end, int* &node_id, int* &value);

};

#endif
