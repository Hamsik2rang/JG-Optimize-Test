//
//  common.h
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/14/25.
//
#pragma once
#ifndef __COMMON_H__
#define __COMMON_H__

#include "Predefine.h"
#include "utility.h"

#pragma region>>> Objects

struct Matrix
{
    Matrix(size_t row = 4, size_t col = 4, bool fillZero = true)
        : row(row)
        , col(col)
        , data(new float[row * col])
    {
        for (size_t i = 0; i < row * col; i++)
        {
            data[i] = fillZero ? 0.0f : Random::NextFloat();
        }
    }
    ~Matrix() { delete[] data; }

    bool operator==(const Matrix& rhs)
    {
        if (row == rhs.row && col == rhs.col)
        {
            return false;
        }

        for (size_t i = 0; i < row * col; i++)
        {
            if (data[i] - rhs.data[i] > FLT_EPSILON)
            {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const Matrix& rhs)
    {
        return !(*this == rhs);
    }

    float* data;
    size_t row;
    size_t col;
};

struct Vector
{
    Vector(size_t size = 4)
        : size(size)
        , data(new float[size])
    {}
    ~Vector() { delete[] data; }

    bool operator==(const Vector& rhs)
    {
        if (size != rhs.size) return false;

        for (size_t i = 0; i < size; i++)
        {
            if (data[i] - rhs.data[i] > FLT_EPSILON)
            {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Vector& rhs)
    {
        return !(*this == rhs);
    }

    float& operator[](int index)
    {
        assert(size > index && index >= 0);
        return data[index];
    }

    float* data;
    size_t size;
};
#pragma endregion

#pragma region>>> Global Functions
uint64_t sum_row_major(int** arr, int N, int M);
uint64_t sum_col_major(int** arr, int N, int M);

#pragma endregion

#endif
