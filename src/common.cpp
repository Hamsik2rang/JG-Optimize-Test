//
//  common.cpp
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/14/25.
//
#include "common.h"

uint64_t sum_row_major(int** arr, int N, int M)
{
    uint64_t sum = 0;
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < M; col++)
        {
            sum += arr[row][col];
        }
    }
    
    return sum;
}

uint64_t sum_col_major(int** arr, int N, int M)
{
    uint64_t sum = 0;
    for (int col = 0; col < N; col++)
    {
        for (int row = 0; row < M; row++)
        {
            sum += arr[row][col];
        }
    }
    
    return sum;
}
