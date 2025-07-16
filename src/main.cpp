#include "Predefine.h"
#include "simd.h"

#include <iostream>
#include <chrono>
#include <string>
#include <vector>

#define N 10000
int arr[N][N]{};

uint64_t sum_row_major()
{
    uint64_t sum = 0;
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            sum += arr[row][col];
        }
    }

    return sum;
}

uint64_t sum_col_major()
{
    uint64_t sum = 0;
    for (int col = 0; col < N; col++)
    {
        for (int row = 0; row < N; row++)
        {
            sum += arr[row][col];
        }
    }
    
    return sum;
}

matrix a;
matrix b;

int main()
{
    //    srand(time(NULL));
    // --- 첫 번째 코드 블록 실행 및 시간 측정 ---

    std::vector<matrix> lhs_matrix;
    std::vector<matrix> rhs_matrix;
    std::vector<matrix> result1, result2;
    
    lhs_matrix.resize(1024768);
    rhs_matrix.resize(1024768);
    
    volatile uint64_t warm = sum_row_major();
    warm = sum_col_major();
    
    multiply_many_matrix_simd(lhs_matrix, rhs_matrix, result1);
    multiply_many_matrix_scalar(lhs_matrix, rhs_matrix, result2);

    std::cout << "## sum_row_major 시작 ##" << std::endl;
    auto start1 = std::chrono::high_resolution_clock::now();
//    multiply_many_matrix_simd(lhs_matrix, rhs_matrix, result1);
    uint64_t sum1 = sum_row_major();
    
    auto end1      = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);

    std::cout << "수행 시간 : " << duration1.count() / 1000 << " 밀리초" << std::endl;
    std::cout << "---------------------------------" << std::endl;

    // --- 두 번째 코드 블록 실행 및 시간 측정 ---
    std::cout << "## sum_col_major 시작 ##" << std::endl;
    auto start2 = std::chrono::high_resolution_clock::now();

//    multiply_many_matrix_scalar(lhs_matrix, rhs_matrix, result2);
    auto sum2 = sum_col_major();

    auto end2      = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
    
    std::cout << "수행 시간 : " << duration2.count() / 1000 << " 밀리초" << std::endl;
    std::cout << "---------------------------------" << std::endl;

    // --- 결과 비교 및 시간 비교 ---
    std::cout << "## 최종 비교 ##" << std::endl;

    // 1. 결과가 동일한지 비교
//    bool isSame = true;
//    size_t resultSize = result1.size();
//    for(size_t i = 0; i < resultSize; i++)
//    {
//        if (result1[i] != result2[i])
//        {
//            isSame = false;
//            break;
//        }
//    }
//    if (isSame)
    if(sum1 == sum2)
    {
        std::cout << "Success: results are same." << std::endl;
    }
    else
    {
        std::cout << "Error: results are not same." << std::endl;
    }

    // 2. 시간 비교
    if (duration1 < duration2)
    {
        auto diff = (duration2 - duration1).count();
        float diff_percentage = (float)diff / (float)duration1.count() * 100.0f;
        std::cout << "sum_row_major is " << diff / 1000 << "ms(" << diff_percentage<< "%) faster." << std::endl;
    }
    else if (duration2 < duration1)
    {
        auto diff = (duration1 - duration2).count();
        float diff_percentage = (float)diff / (float)duration2.count() * 100.0f;
        std::cout << "sum_row_major is " << diff / 1000 << "ms(" << diff_percentage<< "%) faster." << std::endl;
    }
    else
    {
        std::cout << "⌛ Same speed" << std::endl;
    }

    // --- 종료 ---
    return 0;
}
