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

    Matrix& operator[](int index)
    {
		assert(row > index && index >= 0);
        return *this;
    }

    const Matrix& operator[](int index) const
    {
        assert(row > index && index >= 0);
        return *this;
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
    Vector(float x, float y, float z, float w)
		: size(4)
		, data(new float[4] { x, y, z, w })
    {

    }
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

uint32_t djb2(const char* str);

struct FixedString
{
    FixedString(const char* data = "", size_t length = (size_t)-1)
		: data(data)
		, length(length)
	{
        hash = djb2(data);

        //...


        if (length == (size_t)-1)
        {
            for (int i = 0; i < 128; i++)
            {
				if (data[i] == '\0')
				{
                    this->length = i - 1;
                    return;
				}
            }

            this->data = nullptr;
			this->length = (size_t)-1;
        }
    }

    bool IsEqual(const FixedString& rhs) const;

	const char* data;
    size_t length;
    uint32_t hash;
};


//bool operator==(const String& rhs) const
//{
//       return IsEqual(rhs);
   //}

   //bool operator!=(const String& rhs) const
   //{
   //	return !(*this == rhs);
   //}


//   bool IsEqualHash(const String& rhs)
//   {
   //	if (length == rhs.length)
   //	{
//           // return hash == rhs.hash;
   //	}
   //	return false;
//   }
//   


template <typename T>
struct Hasher
{
    static uint32_t hash(T key)
    {
        return 0;
    }
};

template <>
struct Hasher<FixedString>
{
	static uint32_t hash(const FixedString& key)
	{
        return djb2(key.data);
	}
};

#pragma endregion

#pragma region>>> Global Functions
uint64_t sum_row_major(int** arr, int N, int M);
uint64_t sum_col_major(int** arr, int N, int M);

bool compare_string_naive(const std::string& str1, const std::string& str2);
bool compare_string_hash(const std::string& str1, const std::string& str2);


#pragma endregion

#endif
