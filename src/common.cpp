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

uint32_t djb2(const char* str)
{
    uint32_t hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

bool FixedString::IsEqual(const FixedString& rhs) const
{
    return hash == rhs.hash;
}



//
//
//if (length == rhs.length)
//{
//    for (size_t i = 0; i < length; i++)
//    {
//        if (data[i] != rhs.data[i])
//        {
//            return false;
//        }
//    }
//    return true;
//}
//return false;

size_t get_size()
{
    return (size_t)100000;
}