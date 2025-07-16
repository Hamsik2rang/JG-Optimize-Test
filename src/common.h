//
//  common.h
//  OptTest
//
//  Created by Yongsik Im on 7/14/25.
//
#pragma once
#ifndef __COMMON_H__
#define __COMMON_H__

#include "Predefine.h"

struct Matrix
{
    Matrix(size_t row = 4, size_t col = 4)
        : row(row)
        , col(col)
        , data(new float[row * col])
    {}
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

    float* data;
    size_t size;
};

struct Material
{
    
};

struct Mesh
{
    
};
// AOS - Array of Structs


//
//struct Monster
//{
//    Vector position;
//    Vector rotation;
//    Vector scale;
//    
//    Material material;
//    Mesh mesh;
//
//    int hp;
//    float atk;
//    float def;
//    float speed;
//};
//
//Monster monster_list[1500];


struct Monster
{
    Vector position[1500];
    Vector rotation[1500];
    Vector scale[1500];
    
    Material material[1500];
    Mesh mesh[1500];
    
    int hp[1500];
    float atk[1500];
    float def[1500];
    float speed[1500];
};

Monster monster_list;



#endif
